#pragma once

#include <string>
#include <list>
#include <tuple>

#include "PlatformHelper.h"

#include "ProFormaTerm.h"


namespace ProForma {
	/**
	 * \class ProFormaWriter
	 *
	 * \brief Generates string representation from ProFormaTerm objects using different formats
	 *
	 */

	class EXPORT ProFormaWriter {
	public:
		/** No constructors for this class, just helper methods */
		ProFormaWriter() = delete;

        /** \brief  Returns the string representation of the ProFormaTerm object
		  * \param  term The input term.
		  * \return string value
		  */
        static std::string TermToString(ProFormaTerm& term);

        /** \brief  Returns the JSON representation of the ProFormaTerm object
		  * \param  term The input term.
		  * \return string value
		  */
        static std::string TermToJson(ProFormaTerm& term);
    private:
		static std::string CreateDescriptorsText(const std::list<ProFormaDescriptor>& descriptors);
		static std::string CreateDescriptorText(IProFormaDescriptor& descriptor);
		static bool sortBySec(const std::tuple<void*, size_t, size_t, bool, double>& a, const std::tuple<void*, size_t, size_t, bool, double>& b);
	};
}
