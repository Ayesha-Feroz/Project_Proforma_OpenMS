#pragma once


#include <fstream>
#include <iostream>
#include <cstdarg>
#include <string>

#include "PlatformHelper.h"

using namespace std;


#define LOGGER CLogger::GetLogger()

namespace ProForma {
    /**
     * \class ProFormaLogger
     *
     * \brief  Singleton Logger Class.
     *
     */

    class EXPORT ProFormaLogger {
    public:
        /** \brief Logs a message
        *   \param sMessage message to be logged.
        */
        void Log(const std::string& sMessage);

        /** \brief Variable Length Logger function
        *   \param format string for the message to be logged.
        */
        void Log(const char* format, ...);

        /** \brief  << overloaded function to Logs a message
        *   \param sMessage message to be logged.
        */
        ProFormaLogger& operator<<(const string& sMessage);

        /**\brief Funtion to create the instance of logger class.
        *  \return singleton object of Clogger class..
        */
        static ProFormaLogger* GetLogger();
    private:
        /**
        *    Default constructor for the Logger class.
        */
        ProFormaLogger();
        /**
        *   copy constructor for the Logger class.
        */
        ProFormaLogger(const ProFormaLogger&) {};             // copy constructor is private
        /**
        *   assignment operator for the Logger class.
        */
        ProFormaLogger& operator=(const ProFormaLogger&) { return *this; };  // assignment operator is private

        /* date & time helper */ 
        static const std::string CurrentDateTime();

        /**
        *   Log file name.
        **/
        static const std::string _sFileName;
        /**
        *   Singleton logger class object pointer.
        **/
        static ProFormaLogger* _pThis;
        /**
        *   Log file stream object.
        **/
        static ofstream _Logfile;
    };
}
