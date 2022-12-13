// Helper header to adapt the library to different platforms and operating systems
#pragma once

// Includes specific for each platform
#ifdef WIN32
#include <direct.h>
#endif

#define EXCEPTION_MAX_ERROR_LEN  512

// Export C++ classes available under windows environment
#ifdef WIN32
#if defined (_MSC_VER)
#pragma warning(disable: 4251) // disable DLL warning displayed by Visual Studio
#endif
#if defined(TopDownProteomics_EXPORTS)
#define EXPORT   __declspec( dllexport )  
#else
#define EXPORT   __declspec( dllimport )  
#endif
#else
#define EXPORT   __attribute__ ((visibility ("default")))  
#endif

// Directory separators
#ifdef WIN32
#define DIR_SEP '\\'
#define NORMALIZE_PATH(s) std::replace(s.begin(), s.end(), '/', DIR_SEP); 
#else
#define DIR_SEP '/'
#define NORMALIZE_PATH(s) std::replace(s.begin(), s.end(), '\\', DIR_SEP);  
#endif // WIN32
