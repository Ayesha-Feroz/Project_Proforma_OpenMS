#pragma once

namespace ProForma {
    /** @enum ProFormaKey
     *  @brief Possible keys for a descriptor
     */
    enum class ProFormaKey {
        /**<  No key provided. */
        None = 0,

        /**<  The exact modification name from an ontology. */
        Name = 1,

        /**< An identifier from an ontology. */
        Identifier = 2,

        /**< A delta mass of unknown annotation. */
        Mass = 3,

        /**< A chemical formula in our notation. */
        Formula = 4,

        /**< A glycan composition in our notation. */
        Glycan = 5,

        /**< The user defined extra information. */
        Info = 6,
    };

    /** @enum ProFormaEvidenceType
     *  @brief Evidence types to provide on a descriptor, typically an ontology identifier
     */
    enum class ProFormaEvidenceType {
        /**< No evidence provided */
        None = 0,

        /**< Experimentally observed evidence */
        Observed = 1,

        /**< The Unimod database identifier */
        Unimod = 2,

        /**< The UniProt database identifier */
        UniProt = 3,

        /**< The RESID database identifier */
        Resid = 4,

        /**< The PSI-MOD database identifier */
        PsiMod = 5,

        /**< The XL-MOD identifier */
        XlMod = 6,

        /**< The GNO identifier */
        Gno = 7,

        /**< The BRNO identifier (https://doi.org/10.1038/nsmb0205-110) */
        Brno = 8
    };
}
