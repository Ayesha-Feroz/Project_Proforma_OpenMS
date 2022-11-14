#include <sstream>
#include <exception>
#include <algorithm>

#include "ProFormaWriter.h"
#include "ProFormaLogger.h"
#include "ProFormaTagGroupChangingValue.h"
#include "nlohmann/json.hpp"

using namespace ProForma;
using json = nlohmann::json;

/*****************************************************************************/
// PUBLIC
/*****************************************************************************/
 
std::string ProFormaWriter::TermToString(ProFormaTerm& term)
{
    std::stringstream text;

    // Check global modifications
    if (term.GlobalModifications().size())
    {
        for(auto globalMod : term.GlobalModifications())
        {
            if (globalMod.TargetAminoAcids().size())
            {
                std::string info = CreateDescriptorsText(globalMod.Descriptors()) + "@";
                int id = 0;
                for (auto targetAminoacid : globalMod.TargetAminoAcids()) {
                    info += targetAminoacid;
                    if (id < globalMod.TargetAminoAcids().size() - 1) info += ',';
                    id++;
                }
            }
            else
                text << CreateDescriptorsText(globalMod.Descriptors());
        }
    }

    // Check labile modifications
    if (term.LabileDescriptors().size())
    {
        text << CreateDescriptorsText(term.LabileDescriptors());
    }

    // Check unlocalized modifications
    if (term.UnlocalizedTags().size())
    {
        for(auto tag : term.UnlocalizedTags())
        {
            if (tag.Descriptors().size())
                text << CreateDescriptorsText(tag.Descriptors());

            if (tag.Count() != 1)
                text << tag.Count();
        }

        // Only write out a single question mark
        text << '?';
    }

    // Check N-terminal modifications
    if (term.NTerminalDescriptors().size() > 0)
    {
        text << CreateDescriptorsText(term.NTerminalDescriptors());
    }


    std::vector<std::tuple<void*, size_t, size_t, bool, double>> tagsAndGroups;

    if (term.Tags().size()) {
        for (auto tag : term.Tags()) {
            tagsAndGroups.push_back(std::make_tuple(&tag, tag.ZeroBasedStartIndex(), tag.ZeroBasedEndIndex(), true, 0.0));
        }
    }

    if (term.TagGroups().size()) {
        for (auto item : term.TagGroups()) {
            ProFormaTagGroup* tagGroup = item.second;
            for (auto member : tagGroup->Members()) {
                auto it = std::find(tagGroup->Members().begin(), tagGroup->Members().end(), member);
                bool isFirst = (it == tagGroup->Members().begin());
                tagsAndGroups.push_back(std::make_tuple(&tagGroup, member.ZeroBasedStartIndex(), member.ZeroBasedEndIndex(), isFirst, member.Weight()));
            }
        }
    }

    // Check indexed modifications
    if (tagsAndGroups.size())
    {
        // Sort by start index
        std::sort(tagsAndGroups.begin(), tagsAndGroups.end(), &ProFormaWriter::sortBySec);
       
        size_t currentIndex = 0;
        for(const std::tuple<void*, size_t, size_t, bool, double>& item : tagsAndGroups)
        {
            void* obj;
            size_t startIndex, endIndex;
            bool displayValue;
            double weight;

            std::tie(obj, startIndex, endIndex, displayValue, weight) = item;
            if (startIndex == endIndex)
            {
                // Write sequence up to tag
                text << term.Sequence().substr(currentIndex, startIndex - currentIndex + 1);
                currentIndex = startIndex + 1;
            }
            else // Handle ambiguity range
            {
                // Write sequence up to range
                text << term.Sequence().substr(currentIndex, startIndex - currentIndex);

                // Write sequence in range
                text << term.Sequence().substr(startIndex, endIndex - startIndex + 1);
                currentIndex = endIndex + 1;
            }

            if (typeid(obj) == typeid(ProFormaTag))
            {
                ProFormaTag* tag = static_cast<ProFormaTag*>(obj);
                text << CreateDescriptorsText(tag->Descriptors());
            }
            else if (typeid(obj) == typeid(ProFormaTagGroup))
            {
                ProFormaTagGroup* tagGroup = static_cast<ProFormaTagGroup*>(obj);
                if (displayValue)
                    text << CreateDescriptorText(*tagGroup) << "#" << tagGroup->Name();
                else
                    text << "[#" << tagGroup->Name();

                if (weight > 0.0)
                    text << "(" << weight << ")]";
                else
                    text << ']';
            }
        }

        // Write the rest of the sequence
        text << term.Sequence().substr(currentIndex);
    }
    else
    {
        text << term.Sequence();
    }
    // Check C-terminal modifications
    if (term.CTerminalDescriptors().size() > 0)
    {
        text << CreateDescriptorsText(term.CTerminalDescriptors());
    }

    return text.str();         
}

std::string ProFormaWriter::TermToJson(ProFormaTerm& term)
{
    nlohmann::ordered_json json_term;

    // Test examples here:  https://github.com/HUPO-PSI/ProForma

    ProFormaLogger::GetLogger()->Log("TERM has %d unlocalized tags", term.UnlocalizedTags().size());

    // Add sequence
    json_term["Sequence"] = term.Sequence();

    // Add global modifications:  i.e.  <13C>ATPEILTVNSIGQLK
    // with targetAminoacids:  <[MOD:01090]@C>[Phospho]?EM[Oxidation]EVTSECSPEK 
    json json_globalModifications = nullptr;
    for (auto modification : term.GlobalModifications()) {
        if (json_globalModifications == nullptr) json_globalModifications = json::array();
        // append modification to the array
        nlohmann::ordered_json json_modification;
        nlohmann::ordered_json json_modification_descriptors = nullptr;
        for (auto descriptor : modification.Descriptors()) {
            if (json_modification_descriptors == nullptr) json_modification_descriptors = json::array();
            nlohmann::ordered_json json_modification_descriptor;
            json_modification_descriptor["Key"] = descriptor.Key();
            json_modification_descriptor["EvidenceType"] = descriptor.EvidenceType();
            json_modification_descriptor["Value"] = descriptor.Value();
            json_modification_descriptors.push_back(json_modification_descriptor);
        }
        json_modification["Descriptors"] = json_modification_descriptors;
        // Target aminoacids
        json json_targetAminoAcids = nullptr;
        for (auto aminoacid : modification.TargetAminoAcids()) {
            if (json_targetAminoAcids == nullptr) json_targetAminoAcids = json::array();
            json_targetAminoAcids.push_back(aminoacid);
        }
        json_modification["TargetAminoacids"] = json_targetAminoAcids;
        json_globalModifications.push_back(json_modification);
    }

    json_term["GlobalModifications"] = json_globalModifications;

    // Add NTerminalDescriptors
    nlohmann::ordered_json json_nterminal_descriptors = nullptr;
    for (auto descriptor : term.NTerminalDescriptors()) {
        if (json_nterminal_descriptors == nullptr) json_nterminal_descriptors = json::array();
        nlohmann::ordered_json json_descriptor;
        json_descriptor["Key"] = descriptor.Key();
        json_descriptor["EvidenceType"] = descriptor.EvidenceType();
        json_descriptor["Value"] = descriptor.Value();
        json_nterminal_descriptors.push_back(json_descriptor);
    }
    json_term["NTerminalDescriptors"] = json_nterminal_descriptors;

    // Add CTerminalDescriptors
    nlohmann::ordered_json json_cterminal_descriptors = nullptr;
    for (auto descriptor : term.CTerminalDescriptors()) {
        if (json_cterminal_descriptors == nullptr) json_cterminal_descriptors = json::array();
        nlohmann::ordered_json json_descriptor;
        json_descriptor["Key"] = descriptor.Key();
        json_descriptor["EvidenceType"] = descriptor.EvidenceType();
        json_descriptor["Value"] = descriptor.Value();
        json_cterminal_descriptors.push_back(json_descriptor);
    }
    json_term["CTerminalDescriptors"] = json_cterminal_descriptors;

    // Add LabileDescriptors
    nlohmann::ordered_json json_labile_descriptors = nullptr;
    for (auto descriptor : term.LabileDescriptors()) {
        if (json_labile_descriptors == nullptr) json_labile_descriptors = json::array();
        nlohmann::ordered_json json_descriptor;
        json_descriptor["Key"] = descriptor.Key();
        json_descriptor["EvidenceType"] = descriptor.EvidenceType();
        json_descriptor["Value"] = descriptor.Value();
        json_labile_descriptors.push_back(json_descriptor);
    }
    json_term["LabileDescriptors"] = json_labile_descriptors;


    // Add Tags
    nlohmann::ordered_json json_tags = nullptr;

    for (auto tag : term.Tags()) {
        if (json_tags == nullptr) json_tags = json::array();
        // append tag to the array
        nlohmann::ordered_json json_tag;
        json_tag["ZeroBasedStartIndex"] = tag.ZeroBasedStartIndex();
        json_tag["ZeroBasedEndIndex"] = tag.ZeroBasedEndIndex();
        nlohmann::ordered_json json_tag_descriptors = nullptr;
        for (auto descriptor : tag.Descriptors()) {
            if (json_tag_descriptors == nullptr) json_tag_descriptors = json::array();
            nlohmann::ordered_json json_tag_descriptor;
            json_tag_descriptor["Key"] = descriptor.Key();
            json_tag_descriptor["EvidenceType"] = descriptor.EvidenceType();
            json_tag_descriptor["Value"] = descriptor.Value();
            json_tag_descriptors.push_back(json_tag_descriptor);
        }
        json_tag["Descriptors"] = json_tag_descriptors;
        json_tags.push_back(json_tag);
    }
    json_term["Tags"] = json_tags;

    // Add unlocalized tags. i.e   <[MOD:01090]@C>[Phospho]?EM[Oxidation]EVTSECSPEK
    /*
    * "UnlocalizedTags":[{"Count":1,"Descriptors":[{"Key":1,"EvidenceType":0,"Value":"Phospho"}]}]
    */
    nlohmann::ordered_json json_unlocalized_tags = nullptr;
    for (auto tag : term.UnlocalizedTags()) {
        if (json_unlocalized_tags == nullptr) json_unlocalized_tags = json::array();
        // append tag to the array
        nlohmann::ordered_json json_tag;
        json_tag["Count"] = tag.Count();
        nlohmann::ordered_json json_tag_descriptors = nullptr;
        for (auto descriptor : tag.Descriptors()) {
            if (json_tag_descriptors == nullptr) json_tag_descriptors = json::array();
            nlohmann::ordered_json json_tag_descriptor;
            json_tag_descriptor["Key"] = descriptor.Key();
            json_tag_descriptor["EvidenceType"] = descriptor.EvidenceType();
            json_tag_descriptor["Value"] = descriptor.Value();
            json_tag_descriptors.push_back(json_tag_descriptor);
        }
        json_tag["Descriptors"] = json_tag_descriptors;
        json_unlocalized_tags.push_back(json_tag);
    }
    json_term["UnlocalizedTags"] = json_unlocalized_tags;

    nlohmann::ordered_json json_tagGroups = nullptr;

    // Add TagGroups
    for (auto item : term.TagGroups()) {
        if (json_tagGroups == nullptr) json_tagGroups = json::array();
        // append group to the array
        nlohmann::ordered_json json_group;
        auto group = item.second;
        if (group->IsChanging()) {
            ProFormaTagGroupChangingValue* g = static_cast<ProFormaTagGroupChangingValue*>(group);
            json_group["ValueFlux"] = g->ValueFlux();
            json_group["KeyFlux"] = g->KeyFlux();
            json_group["EvidenceFlux"] = g->EvidenceFlux();
        }
        json_group["Name"] = group->Name();
        json_group["Key"] = group->Key();
        json_group["EvidenceType"] = group->EvidenceType();
        json_group["Value"] = group->Value();
        nlohmann::ordered_json json_group_members = nullptr;
        for (auto member : group->Members()) {
            if (json_group_members == nullptr) json_group_members = json::array();
            nlohmann::ordered_json json_member;
            json_member["ZeroBasedStartIndex"] = member.ZeroBasedStartIndex();
            json_member["ZeroBasedEndIndex"] = member.ZeroBasedEndIndex();
            json_member["Weight"] = member.Weight();
            json_group_members.push_back(json_member);
        }
        json_group["Members"] = json_group_members;

        json_tagGroups.push_back(json_group);

    }
    json_term["TagGroups"] = json_tagGroups;
    
    return json_term.dump();
}

/*****************************************************************************/
// PRIVATE
/*****************************************************************************/

std::string ProFormaWriter::CreateDescriptorsText(const std::list<ProFormaDescriptor>& descriptors)
{
    std::stringstream text;
    int i = 0;

    for (auto descriptor : descriptors)
    {
        IProFormaDescriptor* id = static_cast<IProFormaDescriptor*>(&descriptor);
        text << CreateDescriptorText(*id);
        if (i < descriptors.size() - 1)
            text << '|';
        i++;
    }

    return text.str();
}

std::string ProFormaWriter::CreateDescriptorText(IProFormaDescriptor& descriptor)
{
    std::string text = descriptor.Value();

    switch (descriptor.Key())
    {
    case ProFormaKey::Formula:
        text = "Formula:" + descriptor.Value();
        break;
    case ProFormaKey::Glycan:
        text = "Glycan:" + descriptor.Value();
        break;
    case ProFormaKey::Info:
        text = "Info:" + descriptor.Value();
        break;
    case ProFormaKey::Name:
    case ProFormaKey::Mass:
        switch (descriptor.EvidenceType()) 
        {
        case ProFormaEvidenceType::None:        text = descriptor.Value(); break;// We assume the name is enough
        case ProFormaEvidenceType::Observed:    text = "Obs:" + descriptor.Value(); break;
        case ProFormaEvidenceType::Unimod:      text = "U:" + descriptor.Value(); break;
        case ProFormaEvidenceType::Resid:       text = "R:" + descriptor.Value(); break;
        case ProFormaEvidenceType::PsiMod:      text = "M:" + descriptor.Value(); break;
        case ProFormaEvidenceType::XlMod:       text = "X:" + descriptor.Value(); break;
        case ProFormaEvidenceType::Gno:         text = "G:" + descriptor.Value(); break;
        default: 
            throw new std::exception(std::string("Can't handle " + std::to_string(static_cast<int>(descriptor.Key())) + " with evidence type: " + std::to_string(static_cast<int>(descriptor.EvidenceType()))).c_str()); 
            break;
        }
        break;
    case ProFormaKey::Identifier:
        text = "Formula:" + descriptor.Value();
        break;
    default: // use descriptor value initialized
            break;
    }

    return text;
}

bool ProFormaWriter::sortBySec(const std::tuple<void*, size_t, size_t, bool, double>& a,    const std::tuple<void*, size_t, size_t, bool, double>& b)
{
    return (std::get<1>(a) < std::get<1>(b));
}
