#pragma once

#include <map>
#include <string>
#include <sstream>

#include "PlatformHelper.h"
#include "ProFormaTerm.h"
#include "ProFormaLogger.h"


namespace ProForma {
	/**
	 * \class ProFormaParser
	 *
	 * \brief Parser for the ProForma proteoform notation
	 *
	 */

	class EXPORT ProFormaParser {
	public:
		/** \brief  Empty constructor, creates a new instance of the parser
		  */
		ProFormaParser();

		/** \brief  Parses the ProForma string.
		  * \param  proFormaString The pro forma string to be parsed.
		  * \return ProFormaTerm object obtained after parsing.
		  */
		ProFormaTerm ParseString(const std::string& proFormaString);

		
	private:
		ProFormaLogger* _logger;

		// methods
		void HandleGlobalModification
		(
			std::map<std::string, ProFormaTagGroup*>& tagGroups,
			std::list<ProFormaGlobalModification>& globalModifications, 
			std::stringstream& sequence,
			size_t startRange,
			size_t endRange,
			std::string& tagText
		);

		static void SplitStr(std::string const &str, const char delim, std::vector<std::string>& out);
		std::list<ProFormaDescriptor> ProcessTag(std::string& tag, size_t startIndex, size_t index, std::map<std::string, ProFormaTagGroup*>& tagGroups);
		void ProcessTag(std::string& tag, size_t startIndex, size_t index, std::list<ProFormaTag>& tags, std::map<std::string, ProFormaTagGroup*>& tagGroups);
		std::tuple<ProFormaKey, ProFormaEvidenceType, std::string, std::string, double> ParseDescriptor(std::string& text);
		
		static ProFormaKey GetKey(bool isMass);
		static void ToLower(std::string& input);
		void ToUpper(std::string& input);
		static void Trim(std::string& input);
	};
}
