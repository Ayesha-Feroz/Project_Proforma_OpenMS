#include <cctype>
#include <tuple>
#include <typeinfo>

#include "ProFormaParser.h"
#include "ProFormaParseException.h"
#include "ProFormaTagGroupChangingValue.h"

using namespace ProForma;

/*****************************************************************************/
// PUBLIC
/*****************************************************************************/

ProFormaParser::ProFormaParser() : _logger(ProFormaLogger::GetLogger())
{ 
}


ProFormaTerm ProFormaParser::ParseString(const std::string& proFormaString)
{
    auto stringLength = proFormaString.length();

    if(stringLength == 0)
        throw new ProFormaParseException("Empty proforma string");

    // Define the ProFormaTerm fields to be calculated
    std::stringstream sequence;
    std::list<ProFormaTag> tags; 
    std::list<ProFormaDescriptor> nTerminalDescriptors; 
    std::list<ProFormaDescriptor> cTerminalDescriptors;
    std::list<ProFormaDescriptor> labileDescriptors;
    std::list<ProFormaUnlocalizedTag> unlocalizedTags;
    std::map<std::string, ProFormaTagGroup*> tagGroups;
    std::list<ProFormaGlobalModification> globalModifications;
    std::stringstream tag;

    bool inTag = false;
    bool inGlobalTag = false;
    bool inCTerminalTag = false;
    int openLeftBrackets = 0;
    int openLeftBraces = 0;
    size_t startRange = 0xFF;
    size_t endRange = 0xFF;

    // Don't love doing a global index of performance wise, but would need to restructure things to handle multiple unlocalized tags
    auto unlocalizedIndex = proFormaString.find_first_of('?');

    _logger->Log("unlocalizedIndex [%d]", unlocalizedIndex);

    for (int i = 0; i < stringLength; i++)
    {
        if (unlocalizedIndex == i) continue; // Skip unlocalized separator

        char current = proFormaString[i];

        _logger->Log("Processing char [%c]", current);

        if (current == '<')
        {
            _logger->Log("Starting global tag <");
            inGlobalTag = true;
        }
        else if (current == '>')
        {
            std::string tagText = tag.str();

            _logger->Log("Finished global tag >");

            // Make sure nothing happen before this global mod
            if (sequence.str().length() > 0 || unlocalizedTags.size() > 0 || nTerminalDescriptors.size() > 0 || tagGroups.size() > 0)
                throw new ProFormaParseException("Global modifications must be the first element in ProForma string.");

            HandleGlobalModification(tagGroups, globalModifications, sequence, startRange, endRange, tagText);

            inGlobalTag = false;
            tag.str("");
        }
        else if (current == '(' && !inTag)
        {
            if (startRange != 0xFF)
                throw new ProFormaParseException("Overlapping ranges are not allowed.");

            startRange = sequence.str().length();
        }
        else if (current == ')' && !inTag)
        {
            endRange = sequence.str().length();

            // Ensure a tag comes next
            if (proFormaString[i + 1] != '[')
                throw new ProFormaParseException("Ranges must end next to a tag.");
        }
        else if (current == '{' && openLeftBraces++ == 0)
        {
            inTag = true;
        }
        else if (current == '}' && --openLeftBraces == 0)
        {
            std::string tagText = tag.str();

            _logger->Log("Processing labile descriptors for [%s]", tagText.c_str());

            labileDescriptors = ProcessTag(tagText, endRange != 0xFF? startRange : 0xFF, sequence.str().length() - 1, tagGroups);

            inTag = false;
            tag.str("");
        }
     
        else if (!inGlobalTag && current == '[' && openLeftBrackets++ == 0)
        {
            inTag = true;
        }      
        else if (!inGlobalTag && current == ']' && --openLeftBrackets == 0)
        {
            // Don't allow 2 tags right next to eachother in the sequence
            if (sequence.str().length() > 0 && proFormaString.length() > i + 1 && proFormaString[i + 1] == '[')
                throw new ProFormaParseException("Two tags next to eachother are not allowed.");

            std::string tagText = tag.str();

            // Handle terminal modifications and prefix tags
            if (inCTerminalTag)
            {
                cTerminalDescriptors = ProcessTag(tagText, 0xFF, 0xFF, tagGroups);
            }           
            else if (sequence.str().length() == 0 && proFormaString[i + 1] == '-')
            {
                nTerminalDescriptors = ProcessTag(tagText, 0xFF, 0xFF, tagGroups);
                i++; // Skip the - character
            }            
            else if (unlocalizedIndex != std::string::npos && unlocalizedIndex >= i)
            {
                _logger->Log("unlocalized candidate at i=[%d]", i);

                // Make sure the prefix came before the N-terminal modification
                if (nTerminalDescriptors.size())
                    throw new ProFormaParseException("Unlocalized modification must come before an N-terminal modification.");

                auto descriptors = ProcessTag(tagText, 0xFF, 0xFF, tagGroups);

                _logger->Log("unlocalized descriptors size is [%d]", descriptors.size());

                if (descriptors.size())
                {
                    int count = 1;

                    // Check for higher count
                    if (proFormaString[i + 1] == '^')
                    {
                        int j = i + 2;
                        while (std::isdigit(proFormaString[j]))
                            j++;

                        // verify it is an integer
                        auto slice = proFormaString.substr(i + 2, j - i - 2);
                        char* p;
                        strtol(slice.c_str(), &p, 10);
                        if (!p)
                            throw new ProFormaParseException("Can't process number after '^' character.");

                        i = j - 1; // Point i at the last digit
                    }

                    unlocalizedTags.push_back(*(new ProFormaUnlocalizedTag(count, descriptors)));

                    _logger->Log("unlocalized tags size is [%d]", unlocalizedTags.size());
                }

                //i++; // skip the ? character              
            }
            else if (sequence.str().length() == 0)
            {
                throw new ProFormaParseException("Invalid n terminal descriptor, sequence [%s]", sequence.str().c_str());
            }
            else
            {
                ProcessTag(tagText, endRange != 0xFF ? startRange : 0xFF, sequence.str().length() - 1, tags, tagGroups);
            }

            inTag = false;
            tag.str("");

            // Reset the range if we have processed the tag on the end of it
            if (endRange != 0xFF)
            {
                startRange = 0xFF;
                endRange = 0xFF;
            }           
        }        
        else if (inTag || inGlobalTag)
        {
            tag << current;
        }
        else if (current == '-')
        {
            if (inCTerminalTag)
                throw new ProFormaParseException("- at index %d is not allowed.", i);

            inCTerminalTag = true;
        }
        else
        {
            // Validate amino acid character
            if (!std::isupper(current))
                throw new ProFormaParseException("%c is not an upper case letter.", current);
            //else if (current == 'X')
            //    throw new ProFormaParseException("X is not allowed.");

            sequence << current;
        }      
    }

    if (openLeftBrackets != 0)
        throw new ProFormaParseException("There are %d open brackets in ProForma string %s", std::abs(openLeftBrackets), proFormaString.c_str());

    if (openLeftBraces != 0)
        throw new ProFormaParseException("There are %d open braces in ProForma string %s", std::abs(openLeftBraces), proFormaString.c_str());

    return *(new ProFormaTerm(sequence.str(), tags, nTerminalDescriptors, cTerminalDescriptors, labileDescriptors,
        unlocalizedTags, tagGroups, globalModifications));

    //return *(new ProFormaTerm("test", 1));   // DEBUG: testing purposes only     
}

/*****************************************************************************/
// PRIVATE
/*****************************************************************************/

void ProFormaParser::HandleGlobalModification
(
    std::map<std::string, ProFormaTagGroup*>& tagGroups,
    std::list<ProFormaGlobalModification>& globalModifications, 
    std::stringstream& sequence,
    size_t startRange, 
    size_t endRange,
    std::string& tagText
)
{   
    // Check for '@' to specify targets
    auto atSymbolIndex = tagText.find_last_of('@');
    std::string innerTagText;
    std::vector<char> targets;

    _logger->Log("Processing global modification: %s", tagText.c_str());

    if (atSymbolIndex > 0)
    {
        // Handle fixed modification with targets   
        innerTagText = tagText.substr(1, atSymbolIndex - 2);

        for (auto k = atSymbolIndex + 1; k < tagText.length(); k++)
        {
            if (std::isupper(tagText[k]))
                targets.push_back(tagText[k]);
            else if (tagText[k] != ',')
                throw new ProFormaParseException("Unexpected character %c in global modification target list.", tagText[k]);
        }
    }
    else
    {
        // No targets, global isotope ... assume whole thing should be read
        innerTagText = tagText;
    }
    std::list<ProFormaDescriptor> descriptors = ProcessTag(innerTagText, endRange != 0xFF ? startRange : 0xFF, static_cast<int>(sequence.str().length() - 1), tagGroups);

    if (descriptors.size())
    {
        globalModifications.push_back(ProFormaGlobalModification(descriptors, targets));
    }    
}

void ProFormaParser::SplitStr(std::string const &str, const char delim, std::vector<std::string>& out)  
{  
    // create a stream from the string  
    std::stringstream s(str);  
        
    std::string s2;  
    while (std::getline(s, s2, delim) )  
    {  
        out.push_back(s2); // store the string in s2 and pass it to vector  
    }  
} 

std::tuple<ProFormaKey, ProFormaEvidenceType, std::string, std::string, double> ProFormaParser::ParseDescriptor(std::string& text)
{   
    if (text.length() == 0)
        throw new ProFormaParseException("Cannot have an empty descriptor.");


    _logger->Log("Processing descriptor: %s", text.c_str());

    // Let's look for a group
    auto groupIndex = text.find_first_of('#');
    std::string groupName;
    double weight = 0.0;

    if (groupIndex != std::string::npos)
    {
        // Check for weight
        auto weightIndex = text.find_first_of('(');

        if (weightIndex != std::string::npos && weightIndex > groupIndex)
        {
            // Make sure descriptor ends in ')' to close out weight
            if (text[text.length() - 1] != ')')
                throw new ProFormaParseException("Descriptor with weight must end in ')'.");

            auto weightText = text.substr(weightIndex + 1, text.length() - weightIndex - 2);
            // convert into double and verify result
            char* end = nullptr;
            weight = strtod(weightText.c_str(), &end);
            if (end == weightText.c_str() || *end != '\0' || weight == HUGE_VAL)
                throw new ProFormaParseException("Could not parse weight value: %s", weightText.c_str());

            groupName = text.substr(groupIndex + 1, weightIndex - groupIndex - 1);
        }
        else
        {
            groupName = text.substr(groupIndex + 1);
        }

        text = text.substr(0, groupIndex);

        if (groupName.empty())
            throw new ProFormaParseException("Group name cannot be empty.");
    }

    _logger->Log("Processing descriptor before colon: %s", text.c_str());

    // Check for naked group tag
    if (text.empty())
        return std::make_tuple(ProFormaKey::None, ProFormaEvidenceType::None, text, groupName, weight);

    // Let's look for a colon
    size_t colon = text.find_first_of(':');

    if (colon == std::string::npos)
    {
        _logger->Log("Processing isMass descriptor: %s", text.c_str());

        bool isMass2 = (text[0] == '+' || text[0] == '-');

        return std::make_tuple(ProFormaParser::GetKey(isMass2), ProFormaEvidenceType::None, text, groupName, weight);
    }

    // Let's see if the bit before the colon is a known key
    auto newText = text.substr(0, colon);
    ProFormaParser::ToLower(newText);
    ProFormaParser::Trim(newText);
    std::string keyText = newText;
    bool isMass = text[colon + 1] == '+' || text[colon + 1] == '-';

    _logger->Log("Descriptor keyText: %s", keyText.c_str());

    // Check text and return tuple
    if(keyText == "formula")     return std::make_tuple(ProFormaKey::Formula, ProFormaEvidenceType::None, text.substr(colon + 1), groupName, weight);
    else if(keyText == "glycan") return std::make_tuple(ProFormaKey::Glycan, ProFormaEvidenceType::None, text.substr(colon + 1), groupName, weight);
    else if(keyText == "info")   return std::make_tuple(ProFormaKey::Info, ProFormaEvidenceType::None, text.substr(colon + 1), groupName, weight);

    else if(keyText == "mod")    return std::make_tuple(ProFormaKey::Identifier, ProFormaEvidenceType::PsiMod, text, groupName, weight);
    else if (keyText == "unimod") { ProFormaParser::ToUpper(text); return std::make_tuple(ProFormaKey::Identifier, ProFormaEvidenceType::Unimod, text, groupName, weight); }
    else if(keyText == "xlmod")  return std::make_tuple(ProFormaKey::Identifier, ProFormaEvidenceType::XlMod, text, groupName, weight);
    else if(keyText == "gno")    return std::make_tuple(ProFormaKey::Identifier, ProFormaEvidenceType::Gno, text, groupName, weight);

    // Special case for RESID id, don't inclue bit with colon
    else if(keyText ==  "resid")  return std::make_tuple(ProFormaKey::Identifier, ProFormaEvidenceType::Resid, text.substr(colon + 1), groupName, weight);

        // Handle names and masses
    else if(keyText == "u")       return std::make_tuple(GetKey(isMass), ProFormaEvidenceType::Unimod, text.substr(colon + 1), groupName, weight);
    else if(keyText == "m")       return std::make_tuple(GetKey(isMass), ProFormaEvidenceType::PsiMod, text.substr(colon + 1), groupName, weight);
    else if(keyText == "r")       return std::make_tuple(GetKey(isMass), ProFormaEvidenceType::Resid, text.substr(colon + 1), groupName, weight);
    else if(keyText == "x")       return std::make_tuple(GetKey(isMass), ProFormaEvidenceType::XlMod, text.substr(colon + 1), groupName, weight);
    else if(keyText == "g")       return std::make_tuple(GetKey(isMass), ProFormaEvidenceType::Gno, text.substr(colon + 1), groupName, weight);
    else if(keyText == "b")       return std::make_tuple(GetKey(isMass), ProFormaEvidenceType::Brno, text.substr(colon + 1), groupName, weight);
    else if(keyText == "obs")     return std::make_tuple(GetKey(isMass), ProFormaEvidenceType::Observed, text.substr(colon + 1), groupName, weight);

    else { return std::make_tuple(ProFormaKey::Name, ProFormaEvidenceType::None, text, groupName, weight); }
    // return std::tuple(ProFormaKey::Formula, ProFormaEvidenceType::Brno, "", "", 0.2);  // TODO: testing purposes only  
}

void ProFormaParser::ProcessTag(std::string& tag, size_t startIndex, size_t index, std::list<ProFormaTag>& tags, std::map<std::string, ProFormaTagGroup*>& tagGroups)
{
    auto descriptors = ProcessTag(tag, startIndex, index, tagGroups);

    _logger->Log("Processing tag with list: %s", tag.c_str());

    // Only add a tag if descriptors come back
    if (descriptors.size())
    {
        if (startIndex != 0xFF)
            tags.push_back(*(new ProFormaTag(startIndex, index, descriptors)));
        else
            tags.push_back(*(new ProFormaTag(index, descriptors)));
    }
}

std::list<ProFormaDescriptor> ProFormaParser::ProcessTag(std::string& tag, size_t startIndex, size_t index, std::map<std::string, ProFormaTagGroup*>& tagGroups)
{
    std::list<ProFormaDescriptor> descriptors;
    std::vector<std::string> descriptorText;

    _logger->Log("Processing tag: %s", tag.c_str());
    
    ProFormaParser::SplitStr(tag, '|', descriptorText);

    for (auto i = 0; i < descriptorText.size(); i++)
    {
        ProFormaKey key;
        ProFormaEvidenceType evidence;
        std::string value;
        std::string group;
        double weight;

        std::tie(key, evidence, value, group, weight) = ParseDescriptor(descriptorText[i].substr(descriptorText[i].find_first_not_of(' ')));
      
        _logger->Log("Descriptor info obtained: %d, %d, %s, %s, %f", 
            static_cast<int>(key),
            static_cast<int>(evidence),
            value.c_str(),
            group.c_str(),
            weight);

        if (group.length())
        {
            auto item = tagGroups.find(group);
            if (item == tagGroups.end())
            {
                ProFormaTagGroupChangingValue* newTagGroup = new ProFormaTagGroupChangingValue(group, key, evidence, std::list<ProFormaMembershipDescriptor>());
                tagGroups.insert(make_pair(group, newTagGroup));
                _logger->Log("New group created: %s, %d", group.c_str(), newTagGroup->IsChanging());
            }
            else
            {
                _logger->Log("Group already exists: %s and has %d members", group.c_str(), item->second->Members().size());
            }

            auto currentGroup = tagGroups.at(group);

            _logger->Log("Current group: %s, %d", group.c_str(), currentGroup->IsChanging());

            // Fix up name of TagGroup
            if (value.length() && currentGroup->IsChanging())
            {
                _logger->Log("Current group is ProFormaTagGroupChangingValue: %s", group.c_str());

                ProFormaTagGroupChangingValue* x = static_cast<ProFormaTagGroupChangingValue*>(currentGroup);
                // Only allow the value of the group to be set once
                if (x->Value().length())
                    throw new ProFormaParseException("You may only set the value of the group %s once.", group.c_str());

                x->SetValueFlux(value);
                x->SetKey(key);
                x->SetEvidenceType(evidence);
            }

            _logger->Log("Current input index value is: %d", index);

            // If the group was defined before the sequence, don't include it in the membership
            if (index != 0xFF)
            {
                _logger->Log("Adding member for index: %d", index);

               if (startIndex != 0xFF)
                   currentGroup->AddMember(*(new ProFormaMembershipDescriptor(startIndex, index, weight)));
                else
                   currentGroup->AddMember(*(new ProFormaMembershipDescriptor(index, weight)));


               _logger->Log("Group %s after adding member has %d members", group.c_str(), currentGroup->Members().size());
            }
        }
        else if (key != ProFormaKey::None) // typical descriptor
        {
            descriptors.push_back(*(new ProFormaDescriptor(key, evidence, value)));
        }
        else if (value.length() > 0) // keyless descriptor (UniMod or PSI-MOD annotation)
        {
            descriptors.push_back(*(new ProFormaDescriptor(value)));
        }
        else
        {
            throw new ProFormaParseException("Empty descriptor within tag %s", tag.c_str());
        }    
    }

    return descriptors;
}


ProFormaKey ProFormaParser::GetKey(bool isMass) { return (isMass ? ProFormaKey::Mass : ProFormaKey::Name); }
void ProFormaParser::ToLower(std::string& input) { for(auto& c : input) { c = tolower(c);} }
void ProFormaParser::ToUpper(std::string& input) { for (auto& c : input) { c = toupper(c); } }
void ProFormaParser::Trim(std::string& input) {  std::stringstream trimmer; trimmer << input; input.clear(); trimmer >> input; }
