#include <iostream>

#include "ProFormaParser.h"
#include "ProFormaParseException.h"
#include "ProFormaWriter.h"

using namespace ProForma;

int main(int argc, char** argv) {
    std::string proformaString = "EM[+15.9949]EVEES[-79.9663]PEK";

    // Define a parser and parse the string received as argv[1], use default one otherwise
    ProFormaParser parser;

    if(argc > 1) {
        proformaString = argv[1];
    }

    ProFormaLogger::GetLogger()->Log("Parsing proforma string: %s", proformaString.c_str());

    try {
        ProFormaTerm term = parser.ParseString(proformaString);
        //ProFormaLogger::GetLogger()->Log("TERM obtained => %s",ProFormaWriter::TermToString(term).c_str());
        std::cout << ProFormaWriter::TermToJson(term).c_str() << std::endl;
    } catch (ProForma::ProFormaParseException* e) {
        std::cout << "ERROR: Parsing exception => " << e->what() << std::endl;
    } catch(...) {
        std::cout << "ERROR: Unexpected parsing error!" << std::endl;
    }

    return 0;
}
