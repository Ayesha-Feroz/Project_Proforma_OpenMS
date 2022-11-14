#pragma once

#include <string>

using namespace ProForma;

namespace ProForma {
	/**
	 * \class ProFormaAmbiguityAffix
	 *
	 * \brief Possible keys describing range of ambiguous PTM assignment.
	 *
	 */
	class ProFormaAmbiguityAffix {
	public:
        /** \brief  Marks a possible site at which a modification may be localized. */
        static std::string PossibleSite() { return "#"; }

        /** \brief  Marks the left boundary of the range over which a modification may be localized. */
        static std::string LeftBoundary() { return "->"; }

        /** \brief  Marks the right boundary of the range over which a modification may be localized. */
        static std::string RightBoundary() { return "<-"; };
	};
}
