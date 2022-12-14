#pragma once

#include <string>

#include "ProFormaKey.h"

namespace ProForma {
	/**
	 * \class IProFormaDescriptor
	 *
	 * \brief Anything that describes a modification.
	 *
	 */
	class IProFormaDescriptor {
	public:
		/** \brief  Key getter */
		virtual ProFormaKey Key() = 0;

        /** \brief The type of the evidence getter */
        virtual ProFormaEvidenceType EvidenceType() = 0;

        /** \brief The value getter. */
        virtual std::string Value() = 0;
	};
}
