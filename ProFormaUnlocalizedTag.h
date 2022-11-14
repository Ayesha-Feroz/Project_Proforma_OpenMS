#pragma once

#include <list>

#include "ProFormaDescriptor.h"

namespace ProForma {
	/**
	 * \class ProFormaUnlocalizedTag
	 *
	 * \brief The specified way of writing an unlocalized modification. Everything between ‘[‘ and ‘]’ (inclusive) that is followed by a '?'.
	 *
	 */
	class ProFormaUnlocalizedTag {
	public:
        /** \brief  Initializes a new instance of the ProFormaUnlocalizedTag class
		  * \param  count The number of unlocalized modifications applied.
          * \param  descriptors The descriptors.
		  * \return void
		  */
        ProFormaUnlocalizedTag(int count, std::list<ProFormaDescriptor> descriptors)
        {
            _count = count;
            _descriptors = descriptors;
        }

        /** \brief  The number of unlocalized modifications applied. */
        int Count() { return _count; }

        /** \brief  Gets the descriptors. */
        std::list<ProFormaDescriptor> Descriptors() { return _descriptors; }

    private:
        int _count;
        std::list<ProFormaDescriptor> _descriptors;
	};
}
