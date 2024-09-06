**ProForma Support in OpenMS**
Overview
This module in OpenMS provides support for handling ProForma strings that represent modifications in peptide and protein sequences. The ProForma notation allows for precise representation of modifications, such as mass shifts, post-translational modifications (PTMs), and terminal modifications on amino acids.

**What is ProForma?**
ProForma is a standardized way to represent peptide modifications, including mass shifts and complex modifications, using string notation. This format is essential for handling modified sequences in proteomics pipelines.

This project implements parsing, validation, and handling of ProForma strings in OpenMS, enabling seamless integration of ProForma-modified sequences with the OpenMS platform.

**Key Features**

Parsing of ProForma strings into amino acid sequences with modifications.

Support for various modification types:
Standard modifications (e.g., Phospho, Acetyl)

Controlled vocabulary (CV) modifications (e.g., UNIMOD, MOD, RESID)

Mass shifts (e.g., +15.99)

N-terminal/C-terminal modifications

Ambiguous modifications

Reconstruction of ProForma strings from amino acid sequences with modifications for output or further use.

Modification validation: Ensures that all modifications are supported by OpenMS's ModificationsDB.

**Supported Modifications**

The following types of modifications are supported:

**UNIMOD**: Unified Modifications

**MOD**: PSI-MOD controlled vocabulary

**RESID**: Residue modifications

Standard Modifications: Common modifications such as Phospho, Acetyl, etc.

If a modification is not supported, the ProForma class will raise an exception and provide feedback to the user.

**Unit Testing**

Unit tests are provided to ensure the functionality of the ProForma class. Key tests include:

**Basic Modifications**: Parsing and validating simple modifications like Phospho, Acetyl.

**CV Modifications**: Testing modifications from controlled vocabularies such as UNIMOD, MOD, and RESID.

**Mass Shifts**: Verifying the parsing of mass shift modifications like +15.99.

**Validation**: Ensuring unsupported modifications are caught.

