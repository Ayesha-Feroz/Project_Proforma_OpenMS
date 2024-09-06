#include <OpenMS/CONCEPT/ClassTest.h>
#include <OpenMS/CHEMISTRY/ProForma.h>
#include <OpenMS/CHEMISTRY/AASequence.h>
#include <OpenMS/test_config.h>
#include <stdexcept>
#include <iostream>

using namespace OpenMS;
using namespace std;

START_TEST(ProForma, "$Id$")

/////////////////////////////////////////////////////////////
// Create a pointer to hold the object
        ProForma* ptr = nullptr;
        ProForma* nullPointer = nullptr;

        START_SECTION(ProForma(const AASequence& seq))
                AASequence seq = AASequence::fromString("ACDEFGHIK");
                ptr = new ProForma(seq);
                TEST_NOT_EQUAL(ptr, nullPointer);
        END_SECTION

        START_SECTION(~ProForma())
                delete ptr;
        END_SECTION

        START_SECTION((void test_parseSimpleModification()))
                {
                    AASequence seq = AASequence::fromString("ACDEFGHIK");
                    ProForma proforma(seq);

                    String proforma_str = "A[Phospho]CDEFGHIK";
                    proforma.fromProFormaString(proforma_str);
                    String output = proforma.toProFormaString();
                    TEST_STRING_EQUAL(output, proforma_str);
                }
        END_SECTION

        START_SECTION((void test_parseMassShiftModification()))
                {
                    AASequence seq = AASequence::fromString("ACDEFGHIK");
                    ProForma proforma(seq);

                    String proforma_str = "A[+15.99]CDEFGHIK";
                    proforma.fromProFormaString(proforma_str);
                    String output = proforma.toProFormaString();
                    TEST_STRING_EQUAL(output, proforma_str);
                }
        END_SECTION

        START_SECTION((void test_parseNTerminalModification()))
                {
                    AASequence seq = AASequence::fromString("ACDEFGHIK");
                    ProForma proforma(seq);

                    String proforma_str = "[Acetyl]-ACDEFGHIK";
                    proforma.fromProFormaString(proforma_str);
                    String output = proforma.toProFormaString();
                    TEST_STRING_EQUAL(output, proforma_str);
                }
        END_SECTION

        START_SECTION((void test_parseCTerminalModification()))
                {
                    AASequence seq = AASequence::fromString("ACDEFGHIK");
                    ProForma proforma(seq);

                    String proforma_str = "ACDEFGHIK-[Amidation]";
                    proforma.fromProFormaString(proforma_str);
                    String output = proforma.toProFormaString();
                    TEST_STRING_EQUAL(output, proforma_str);
                }
        END_SECTION

        START_SECTION((void test_parseAmbiguousModification()))
                {
                    AASequence seq = AASequence::fromString("ACDEFGHIK");
                    ProForma proforma(seq);

                    String proforma_str = "A[+15.99]?CDEFGHIK";
                    proforma.fromProFormaString(proforma_str);
                    String output = proforma.toProFormaString();
                    TEST_STRING_EQUAL(output, proforma_str);
                }
        END_SECTION

        START_SECTION((void test_invalidModificationFormat()))
                {
                    AASequence seq = AASequence::fromString("ACDEFGHIK");
                    ProForma proforma(seq);

                    String invalid_proforma_str = "A[PhosphoCDEFGHIK"; // Missing closing bracket
                    TEST_EXCEPTION(std::runtime_error, proforma.fromProFormaString(invalid_proforma_str));
                }
        END_SECTION

        START_SECTION((void test_unsupportedCV()))
                {
                    AASequence seq = AASequence::fromString("ACDEFGHIK");
                    ProForma proforma(seq);

                    String invalid_proforma_str = "A[XYZ:12345]CDEFGHIK"; // Unsupported CV
                    TEST_EXCEPTION(std::invalid_argument, proforma.fromProFormaString(invalid_proforma_str));
                }
        END_SECTION

        START_SECTION((void test_removeModification()))
                {
                    AASequence seq = AASequence::fromString("ACDEFGHIK");
                    ProForma proforma(seq);

                    String proforma_str = "A[Phospho]CDEFGHIK";
                    proforma.fromProFormaString(proforma_str);
                    proforma.removeModification(1);
                    String output = proforma.toProFormaString();
                    TEST_STRING_EQUAL(output, "ACDEFGHIK");
                }
        END_SECTION

        START_SECTION((void test_validProFormaPSIMOD()))
                {
                    AASequence seq = AASequence::fromString("EMEVEESPEK");
                    ProForma proforma(seq);

                    String proforma_str = "EM[MOD:00719]EVEES[MOD:00046]PEK";
                    proforma.fromProFormaString(proforma_str);
                    String output = proforma.toProFormaString();
                    TEST_STRING_EQUAL(output, proforma_str);
                }
        END_SECTION

        START_SECTION((void test_validProFormaUNIMOD()))
                {
                    AASequence seq = AASequence::fromString("EMEVEESPEK");
                    ProForma proforma(seq);

                    String proforma_str = "EM[UNIMOD:35]EVEES[UNIMOD:56]PEK";
                    proforma.fromProFormaString(proforma_str);
                    String output = proforma.toProFormaString();
                    TEST_STRING_EQUAL(output, proforma_str);
                }
        END_SECTION

        START_SECTION((void test_validProFormaRESID()))
                {
                    AASequence seq = AASequence::fromString("EMEVEESPEK");
                    ProForma proforma(seq);

                    String proforma_str = "EM[RESID:AA0581]EVEES[RESID:AA0037]PEK";
                    proforma.fromProFormaString(proforma_str);
                    String output = proforma.toProFormaString();
                    TEST_STRING_EQUAL(output, proforma_str);
                }
        END_SECTION

        START_SECTION((void test_invalidProFormaPSIMODSyntax()))
                {
                    AASequence seq = AASequence::fromString("EMEVEESPEK");
                    ProForma proforma(seq);

                    String invalid_proforma_str = "EM[M:00719]EVEES[M:00046]PEK";
                    TEST_EXCEPTION(std::invalid_argument, proforma.fromProFormaString(invalid_proforma_str));
                }
        END_SECTION

        START_SECTION((void test_invalidProFormaUNIMODSyntax()))
                {
                    AASequence seq = AASequence::fromString("EMEVEESPEK");
                    ProForma proforma(seq);

                    String invalid_proforma_str = "EM[U:35]EVEES[U:56]PEK";
                    TEST_EXCEPTION(std::invalid_argument, proforma.fromProFormaString(invalid_proforma_str));
                }
        END_SECTION

        START_SECTION((void test_invalidProFormaRESIDSyntax()))
                {
                    AASequence seq = AASequence::fromString("EMEVEESPEK");
                    ProForma proforma(seq);

                    String invalid_proforma_str = "EM[R:AA0581]EVEES[R:AA0037]PEK";
                    TEST_EXCEPTION(std::invalid_argument, proforma.fromProFormaString(invalid_proforma_str));
                }
        END_SECTION

        START_SECTION((void test_parseMultipleModifications()))
                {
                    // Example 1: Test with multiple modifications associated with different positions
                    AASequence seq1 = AASequence::fromString("EKSVEAAAELSAKDLKEKKEKVEEKASRKERKKEVVEEEENGA");
                    ProForma proforma1(seq1);

                    // This string contains multiple modifications with specific positions
                    String proforma_str1 = "EKSVEAAAELSAKDLKEKKEKVEEKASRKERKKEVVEEEENGA[Phosphogluconoylation:1344,GluGlu:451,probiotinhydrazide:357]EEEEEETAEDGEEEDEGEEEDEEEEEEDDEGPALKRAAEEEDEADPKRQKTENGASA";
                    proforma1.fromProFormaString(proforma_str1);
                    String output1 = proforma1.toProFormaString();
                    TEST_STRING_EQUAL(output1, proforma_str1);

                    // Example 2: Test with another complex modification scenario
                    AASequence seq2 = AASequence::fromString("EKSVEAAAELSAKDLKEKKEKVE");
                    ProForma proforma2(seq2);

                    String proforma_str2 = "EKSVEAAAELSAKDLKEKKEKVE[Glu:450,Nmethylmaleimide+water:500,Pro+O(2):2035,Gly->Trp:573,Acetylhypusine:1043]";
                    proforma2.fromProFormaString(proforma_str2);
                    String output2 = proforma2.toProFormaString();
                    TEST_STRING_EQUAL(output2, proforma_str2);

                    // Example 3: Test with ambiguous modifications and multiple positions
                    AASequence seq3 = AASequence::fromString("FSVSDILSPIEETYKKFSGAMDGAPPGLGAPLGAAAAYRAPPP");
                    ProForma proforma3(seq3);

                    String proforma_str3 = "FSVSDILSPIEETYKKFSGAMDGAPPGLGAPLGAAAAYRAPPP[Thr->Trp:1211,NEIAA:211,Xlink:BuUrBu[85]:1886]";
                    proforma3.fromProFormaString(proforma_str3);
                    String output3 = proforma3.toProFormaString();
                    TEST_STRING_EQUAL(output3, proforma_str3);
                }
        END_SECTION


/////////////////////////////////////////////////////////////
END_TEST
