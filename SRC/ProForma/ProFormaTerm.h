#pragma once

#include <string>
#include <list>
#include <vector>
#include <map>

#include "ProFormaTag.h"
#include "ProFormaTagGroup.h"
#include "ProFormaGlobalModification.h"
#include "ProFormaUnlocalizedTag.h"

namespace ProForma {
	/**
	 * \class ProFormaTerm
	 *
	 * \brief Represents a ProForma string in memory.
	 *
	 */
	class EXPORT ProFormaTerm {
    public:
        /** \brief  Initializes a new instance of the ProFormaTerm class
		  * \param  sequence The sequence.
          * \param  tags The tags.
          * \param  nTerminalDescriptors The n terminal descriptors.
          * \param  cTerminalDescriptors The c terminal descriptors.
          * \param  labileDescriptors The labile modification descriptors.
          * \param  unlocalizedTags Unlocalized modification tags.
          * \param  tagGroups The tag groups.
          * \param  globalModifications The global modifications.
		  * \return void
		  */
        ProFormaTerm( 
            std::string sequence, 
            std::list<ProFormaTag> tags = std::list<ProFormaTag>(), 
            std::list<ProFormaDescriptor> nTerminalDescriptors = std::list<ProFormaDescriptor>(), 
            std::list<ProFormaDescriptor> cTerminalDescriptors = std::list<ProFormaDescriptor>(),
            std::list<ProFormaDescriptor> labileDescriptors = std::list<ProFormaDescriptor>(),
            std::list<ProFormaUnlocalizedTag> unlocalizedTags = std::list<ProFormaUnlocalizedTag>(),
            std::map<std::string, ProFormaTagGroup*> tagGroups = std::map<std::string, ProFormaTagGroup*>(),
            std::list<ProFormaGlobalModification> globalModifications = std::list<ProFormaGlobalModification>()
        )
        {
            this->_sequence = sequence;
            this->_nTerminalDescriptors = nTerminalDescriptors;
            this->_cTerminalDescriptors = cTerminalDescriptors;
            this->_labileDescriptors = labileDescriptors;
            this->_tags = tags;
            this->_unlocalizedTags = unlocalizedTags;
            this->_tagGroups = tagGroups;
            this->_globalModifications = globalModifications;
        }

        /** \brief  The amino acid sequence. */
        std::string Sequence() { return _sequence; }

        /** \brief  Modifications that apply globally based on a target or targets. */
        std::list<ProFormaGlobalModification> GlobalModifications() { return _globalModifications; }

        /** \brief  N-Terminal descriptors. */
        std::list<ProFormaDescriptor> NTerminalDescriptors() { return _nTerminalDescriptors; }

        /** \brief  C-Terminal descriptors. */
        std::list<ProFormaDescriptor> CTerminalDescriptors() { return _cTerminalDescriptors; }

        /** \brief  Labile modifications (not visible in the fragmentation MS2 spectrum) descriptors. */
        std::list<ProFormaDescriptor> LabileDescriptors() { return _labileDescriptors; }

        /** \brief  All tags on this term. */
        std::list<ProFormaTag> Tags() { return _tags; }

        /** \brief  Descriptors for modifications that are completely unlocalized. */
        std::list<ProFormaUnlocalizedTag> UnlocalizedTags() { return _unlocalizedTags; }

        /** \brief  All tag groups for this term. */
        std::map<std::string, ProFormaTagGroup*> TagGroups() { return _tagGroups; }
    private:
        std::string _sequence;
        std::list<ProFormaGlobalModification> _globalModifications;
        std::list<ProFormaDescriptor> _nTerminalDescriptors;
        std::list<ProFormaDescriptor> _cTerminalDescriptors;
        std::list<ProFormaDescriptor> _labileDescriptors;
        std::list<ProFormaTag> _tags;
        std::list<ProFormaUnlocalizedTag> _unlocalizedTags;
        std::map<std::string, ProFormaTagGroup*> _tagGroups;
	};
}
