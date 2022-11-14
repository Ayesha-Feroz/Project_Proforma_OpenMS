#include <cstdarg>

#include "ProFormaParseException.h"

using namespace ProForma;

ProFormaParseException::ProFormaParseException(const char* format, ...)  : std::exception() {
    // format string description and store in error buffer 
    va_list args;
    va_start(args, format);
    vsnprintf(_errorMsg, EXCEPTION_MAX_ERROR_LEN, format, args);
    va_end(args);
}

ProFormaParseException::ProFormaParseException(const std::string& message)  : std::exception() {
    // format string description and store in error buffer 
    snprintf(_errorMsg, EXCEPTION_MAX_ERROR_LEN, "%s", message.c_str());
}
