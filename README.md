# Project_Proforma_OpenMS
Protein and peptide sequences are usually represented using a string of amino acids using a well-known one letter code endorsed by the IUPAC. However, there is still no clear consensus about how to represent ‘proteoforms’ and ‘peptidoforms’, meaning all possible variations of a protein/peptide sequence, including protein modifications, both artefactual and post-translational modifications (PTMs). There are indeed multiple ways of encoding mass modifications and extended discussion has taken place to achieve a consensus. A standard notation for proteoforms and peptidoforms is then required for the community, so that it can be embedded in many relevant PSI (and potentially other) file formats.
#Use cases supported (with examples)
The ProForma notation is a string of characters that represent linearly one or more peptidoform/proteoform primary structures with possibilities to link peptidic chains together. It is not meant to represent secondary or tertiary structures.

## Canonical IUPAC amino acids
EMEVEESPEK
#PTMs using common ontologies or controlled vocabularies (e.g. Unimod, PSI-MOD, and RESID)
EM[UNIMOD:35]EVEES[UNIMOD:21]PEK
EM[L-methionine sulfoxide]EVEES[O-phospho-L-serine]PEK
## Cross-linkers using the XL-MOD ontology
EMEVTK[XLMOD:02001#XL1]SESPEK[#XL1]
EVTSEKC[L-cystine (cross-link)#XL1]LEMSC[#XL1]EFD
## Glycans using the GNO (Glycan Naming Ontology) ontology
YPVLN[GNO:G62765YT]VTMPN[GNO:G02815KT]NSNGKFDK
# Arbitrary mass shifts and unknown mass gaps
EM[+15.9949]EVEES[-79.9663]PEK
RTAAX[+367.0537]WT
## Elemental formulas and Glycan compositions
SEQUEN[Formula:C12H20O2]CE
SEQUEN[Glycan:HexNAc1Hex 2]CE
## Terminal and Labile Modifications
[iTRAQ4plex]-EMEVNESPEK-[Methyl]
{Glycan:Hex}EMEVNESPEK
## Ambiguity of modification position (completely unlocalised, n possible sites, or a range of sites)
[Phospho]?EMEVTSESPEK
EMEVT[#g1]S[#g1]ES[Phospho#g1]PEK
PROT(EOSFORMS)[+19.0523]ISK
## Global modifications (e.g. isotopic labeling or fixed protein modifications)
<13C>ATPEILTVNSIGQLK
<[S-carboxamidomethyl-L-cysteine]@C>ATPEILTCNSIGCLK
## Additional user-supplied textual information
ELV[info:AnyString]IS
