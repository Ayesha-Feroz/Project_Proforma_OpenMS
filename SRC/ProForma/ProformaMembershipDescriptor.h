#pragma once

#include "IProFormaDescriptor.h"

using namespace ProForma;

namespace ProForma {
	/**
	 * \class ProFormaMembershipDescriptor
	 *
	 * \brief A member of a tag group.
	 *
	 */

	class ProFormaMembershipDescriptor {
	public:
        /** \brief  Initializes a new instance of the ProFormaMembershipDescriptor class
		  * \param  zeroBasedIndex The zero-based index of the modified amino acid in the sequence.
          * \param  weight The weight..
		  * \return void
		  */
        ProFormaMembershipDescriptor(size_t zeroBasedIndex, double weight = 0.0)
        {
            this->_zeroBasedStartIndex = zeroBasedIndex;
            this->_zeroBasedEndIndex = zeroBasedIndex;
            this->_weight = weight;
        }

        /** \brief  Initializes a new instance of the ProFormaMembershipDescriptor class
		  * \param  zeroBasedIndex The zero-based index of the modified amino acid in the sequence.
          * \param  zeroBasedEndIndex The zero-based end index of the modified amino acid in the sequence.
          * \param  weight The weight..
		  * \return void
		  */
        ProFormaMembershipDescriptor(size_t zeroBasedStartIndex, size_t zeroBasedEndIndex, double weight = 0.0)
        {
            this->_zeroBasedStartIndex = zeroBasedStartIndex;
            this->_zeroBasedEndIndex = zeroBasedEndIndex;
            this->_weight = weight;
        }

        bool ProFormaMembershipDescriptor::operator==(const ProFormaMembershipDescriptor& descriptor) 
        {
            if ((this->_zeroBasedStartIndex == descriptor._zeroBasedStartIndex) &&
                (this->_zeroBasedEndIndex == descriptor._zeroBasedEndIndex) &&
                (this->_weight == descriptor._weight)) {
                return true;
            }
            return false;
        }

        /** \brief  Gets the zero-based start index in the sequence. */
        size_t ZeroBasedStartIndex() { return _zeroBasedStartIndex; } 

        /** \brief  Gets the zero-based end index in the sequence. */
        size_t ZeroBasedEndIndex() { return _zeroBasedEndIndex; } 

        /** \brief  The weight this member has on the group. */
        double Weight() { return _weight; }
    private:
        size_t _zeroBasedStartIndex;
        size_t _zeroBasedEndIndex;
        double _weight;
	};
}
