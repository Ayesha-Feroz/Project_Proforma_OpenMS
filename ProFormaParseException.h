#pragma once

#include <exception>
#include <string>

#include "PlatformHelper.h"

/**
 * \class ProFormaParseException
 *
 * \brief Base ProForma parsing exception.
 *
 */

namespace ProForma {
	class ProFormaParseException : public std::exception  {
	public:

		/** \brief  Object constructor
		  * \param  format Exception using variable parameters format like printf
		  * \return void
		  */
		ProFormaParseException(const char* format, ...);

		/** \brief  Object constructor passing exception code and error
		  * \param  code Exception code
		  * \param  error Internal error code when it is an API error (0 otherwise)
		  * \param  Exception description with variable length format
		  * \return void
		  */
		ProFormaParseException(const std::string& message);

		/** \brief  Object destructor
		  * \param  None
		  * \return None
		  */
		virtual ~ProFormaParseException() {};

		/** \brief  Gets exception description
		  * \param  None
		  * \return Exception description
		  */
		virtual const char* what() { return _errorMsg; };

	private:
		char _errorMsg[EXCEPTION_MAX_ERROR_LEN];

	};
}
