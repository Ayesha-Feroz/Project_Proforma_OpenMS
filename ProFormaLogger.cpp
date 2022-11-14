#include "ProFormaLogger.h"

using namespace ProForma;


const string ProFormaLogger::_sFileName = "Log.txt";
ProFormaLogger* ProFormaLogger::_pThis = NULL;
ofstream ProFormaLogger::_Logfile;


ProFormaLogger::ProFormaLogger()
{

}

ProFormaLogger* ProFormaLogger::GetLogger() {
    if (_pThis = NULL) {
        _pThis = new ProFormaLogger();
        //_Logfile.open(std::cout, ios::out | ios::app);
    }
    return _pThis;
}

void ProFormaLogger::Log(const char* format, ...)
{
#ifndef RELEASE
    char* sMessage = NULL;
    int nLength = 0;
    va_list args;
    va_start(args, format);
    //  Return the number of characters in the string referenced the list of arguments.
    // _vscprintf doesn't count terminating '\0' (that's why +1)
    nLength = _vscprintf(format, args) + 1;
    sMessage = new char[nLength];
    vsprintf_s(sMessage, nLength, format, args);
    //vsprintf(sMessage, format, args);
    //_Logfile << ProFormaLogger::CurrentDateTime() << ":\t";
    //_Logfile << sMessage << "\n";
    std::cout << ProFormaLogger::CurrentDateTime() << ":\t";
    std::cout << sMessage << "\n";
    va_end(args);

    delete[] sMessage;
#endif
}

void ProFormaLogger::Log(const string& sMessage)
{
#ifndef RELEASE
    //_Logfile << ProFormaLogger::CurrentDateTime() << ":\t";
    //_Logfile << sMessage << "\n";
    std::cout << ProFormaLogger::CurrentDateTime() << ":\t";
    std::cout << sMessage << "\n";
#endif
}

ProFormaLogger& ProFormaLogger::operator<<(const string& sMessage)
{
    //_Logfile << "\n" << ProFormaLogger::CurrentDateTime() << ":\t";
    //_Logfile << sMessage << "\n";
    std::cout << "\n" << ProFormaLogger::CurrentDateTime() << ":\t";
    std::cout << sMessage << "\n";
    return *this;
}

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string ProFormaLogger::CurrentDateTime()
{
    time_t     now = time(NULL);
    struct tm  tstruct;
    char       buf[80];
    localtime_s(&tstruct, &now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return buf;
}
