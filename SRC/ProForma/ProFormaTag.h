#pragma once

#include <list>

#include "ProFormaDescriptor.h"

namespace ProForma {
	/**
	 * \class ProFormaTag
	 *
	 * \brief The specified way of writing a modification. Everything between ‘[‘ and ‘]’ (inclusive). A collection of descriptors..
	 *
	 */

	class ProFormaTag {
	public:
        /** \brief  Initializes a new instance of the ProFormaTag class
		  * \param  zeroBasedIndex The zero-based index of the modified amino acid in the sequence.
          * \param  descriptors The descriptors.
		  * \return void
		  */
        ProFormaTag(size_t zeroBasedIndex, std::list<ProFormaDescriptor> descriptors)
        { 
            this->_zeroBasedStartIndex = zeroBasedIndex;
            this->_zeroBasedEndIndex = zeroBasedIndex;
            this->_descriptors = descriptors;
        }

        /** \brief  Initializes a new instance of the ProFormaTag class
		  * \param  zeroBasedStartIndex The zero-based start index of the modified amino acid in the sequence.
          * \param  zeroBasedEndIndex The zero-based end index of the modified amino acid in the sequence.
          * \param  descriptors The descriptors.
		  * \return void
		  */
        ProFormaTag(size_t zeroBasedStartIndex, size_t zeroBasedEndIndex, std::list<ProFormaDescriptor> descriptors)
        {
            this->_zeroBasedStartIndex = zeroBasedStartIndex;
            this->_zeroBasedEndIndex = zeroBasedEndIndex;
            this->_descriptors = descriptors;
        }

        /** \brief  Gets the zero-based start index in the sequence.
		  */
        size_t ZeroBasedStartIndex() { return _zeroBasedStartIndex; }

        /** \brief  Gets the zero-based end index in the sequence.
		  */
        size_t ZeroBasedEndIndex() { return _zeroBasedEndIndex; }

        /** \brief  Gets the descriptors.
		  */
        std::list<ProFormaDescriptor> Descriptors() { return _descriptors; }
    private:
        size_t _zeroBasedStartIndex;
        size_t _zeroBasedEndIndex;
        std::list<ProFormaDescriptor> _descriptors;
	};
}
