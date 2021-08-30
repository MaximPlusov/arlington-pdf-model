///////////////////////////////////////////////////////////////////////////////
// ArlingtonTSVGrammarFile.cpp
// Copyright 2020-2021 PDF Association, Inc. https://www.pdfa.org
//
// This material is based upon work supported by the Defense Advanced
// Research Projects Agency (DARPA) under Contract No. HR001119C0079.
// Any opinions, findings and conclusions or recommendations expressed
// in this material are those of the author(s) and do not necessarily
// reflect the views of the Defense Advanced Research Projects Agency
// (DARPA). Approved for public release.
//
// SPDX-License-Identifier: Apache-2.0
// Contributors: Roman Toda, Frantisek Forgac, Normex; Peter Wyatt, PDF Association
//
///////////////////////////////////////////////////////////////////////////////

///
///  A class to read the Arlington TSV-based grammar data from a TSV file.
///

#include <iterator>

#include "ArlingtonTSVGrammarFile.h"

/// @brief All Arlington pre-defined types (alphabetically sorted)
const std::vector<std::string>  CArlingtonTSVGrammarFile::arl_all_types = {
        "array",
        "bitmask",
        "boolean",
        "date",
        "dictionary",
        "integer",
        "matrix",
        "name",
        "name-tree",
        "null",
        "number",
        "number-tree",
        "rectangle",
        "stream",
        "string",
        "string-ascii",
        "string-byte",
        "string-text"
};

/// @brief Arlingon pre-defined types which REQUIRE a Link - aka "Complex types" (alphabetically sorted)
const std::vector<std::string>  CArlingtonTSVGrammarFile::arl_complex_types = {
    "array",
    "dictionary",
    "name-tree",
    "number-tree",
    "stream"
};

/// @brief Arlington pre-defined types that must NOT have Links - aka "Non-complex types" (alphabetically sorted)
const std::vector<std::string>  CArlingtonTSVGrammarFile::arl_non_complex_types = {
    "bitmask",
    "boolean",
    "date",
    "integer",
    "matrix",
    "name",
    "null",
    "number",
    "rectangle",
    "string",
    "string-ascii",
    "string-byte",
    "string-text"
};


// @brief Arlington PDF versions
const std::vector<std::string>  CArlingtonTSVGrammarFile::arl_pdf_versions = {
    "1.0",
    "1.1",
    "1.2",
    "1.3",
    "1.4",
    "1.5",
    "1.6",
    "1.7",
    "2.0",
};


/// @brief  Parses through a TSV file line by line and loads TSV data into data_list
/// @return returns false if TSV data is malformed, else returns true
bool CArlingtonTSVGrammarFile::load()
{
    std::ifstream     file(tsv_file_name);
    std::string       line = "";

    // Iterate through each line (row) and split content using TAB delimiter
    while (getline(file, line))
    {
        std::vector<std::string>    vec;
        std::string::size_type      prev_pos = 0;
        std::string::size_type      pos = 0;

        while ((pos = line.find('\t', pos)) != std::string::npos) {
            std::string substring(line.substr(prev_pos, pos - prev_pos));
            vec.push_back(substring);
            prev_pos = ++pos;
        }

        vec.push_back(line.substr(prev_pos, pos - prev_pos)); // Last word

        // Check first header line - have to have 12 columns
        if (data_list.empty() && (vec.size() < TSV_NOTES)) {
            file.close();
            return false;
        }

        // Move header row separately, so data is pure
        if (header_list.empty())
            header_list = vec;
        else
            data_list.push_back(vec);
    } // while

    // Close the TSV file
    file.close();
    return true;
}

/// @brief  Returns the name of the TSV without folder or file extension
/// @return just the TSV filename (no folder, no extension) as a string
std::string CArlingtonTSVGrammarFile::get_tsv_name()
{
    return tsv_file_name.stem().string();
}


/// @brief  Returns the folder containing the current TSV file
/// @return Folder of the current TSV fie
fs::path CArlingtonTSVGrammarFile::get_tsv_dir()
{
    return tsv_file_name.parent_path();
}



/// @brief   Returns raw TSV data as a vector of vector of strings
/// @return  internal data_list (vector of vector of strings)
const ArlTSVmatrix& CArlingtonTSVGrammarFile::get_data()
{
    return data_list;
}
