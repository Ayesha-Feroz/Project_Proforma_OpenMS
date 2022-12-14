#pragma once

#include <list>
#include <vector>

#include "ProFormaDescriptor.h"

using namespace ProForma;

namespace ProForma {
	/**
	 * \class ProFormaGlobalModification
	 *
	 * \brief A modification to applies globally based on a target or targets.
	 *
	 */
	class ProFormaGlobalModification {
	public:
        /** \brief  Initializes a new instance of the ProFormaGlobalModification class
		  * \param  count The number of unlocalized modifications applied.
          * \param  descriptors The descriptors.
		  * \return void
		  */
        ProFormaGlobalModification(std::list<ProFormaDescriptor> descriptors, std::vector<char> targetAminoAcids)
        {
            _descriptors = descriptors;
            _targetAminoAcids = targetAminoAcids;
        }

        /** \brief  The descriptors for this global modification. */
        std::list<ProFormaDescriptor> Descriptors() { return _descriptors; }

        /** \brief  The amino acids targeted by this global modification (null if representing isotopes). */
        std::vector<char> TargetAminoAcids() { return _targetAminoAcids; }

    private:
        std::vector<char> _targetAminoAcids;
        std::list<ProFormaDescriptor> _descriptors;
	};
}
