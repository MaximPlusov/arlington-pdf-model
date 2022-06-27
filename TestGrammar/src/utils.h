///////////////////////////////////////////////////////////////////////////////
/// @file 
/// @brief Utility function declarations
/// 
/// @copyright
/// Copyright 2020-2022 PDF Association, Inc. https://www.pdfa.org
/// SPDX-License-Identifier: Apache-2.0
/// 
/// @remark
/// This material is based upon work supported by the Defense Advanced
/// Research Projects Agency (DARPA) under Contract No. HR001119C0079.
/// Any opinions, findings and conclusions or recommendations expressed
/// in this material are those of the author(s) and do not necessarily
/// reflect the views of the Defense Advanced Research Projects Agency
/// (DARPA). Approved for public release.
///
/// @author Roman Toda, Normex
/// @author Frantisek Forgac, Normex
/// @author Peter Wyatt, PDF Association
/// 
///////////////////////////////////////////////////////////////////////////////

#ifndef Utils_h
#define Utils_h
#pragma once

#include <string>
#include <filesystem>

/// @brief ANSI code to reset all text formatting. Portable across *nix and Windows 10/11.
#define COLOR_RESET_ANSI    "\033[0m"

/// @brief ANSI code for red foreground text. Portable across *nix and Windows 10/11.
#define COLOR_ERROR_ANSI    "\033[1;31m" // Red foreground

/// @brief ANSI code for yellow foreground text. Portable across *nix and Windows 10/11.
#define COLOR_WARNING_ANSI  "\033[1;33m" // Yellow foreground

/// @brief ANSI code for cyan foreground text. Portable across *nix and Windows 10/11.
#define COLOR_INFO_ANSI  "\033[1;36m" // Cyan foreground

/// @brief Global flag from main, representing --no-color CLI option
extern bool no_color;

/// @brief Inline function to reset terminal colors for text outout if not disabled. Also outputs EOL.
inline std::ostream& COLOR_RESET(std::ostream& os)    { if (!no_color) { os << COLOR_RESET_ANSI; } os << std::endl; return os; }

/// @brief Inline function to set error color for text outout if not disabled
inline std::ostream& COLOR_ERROR(std::ostream& os) { if (!no_color) { os << COLOR_ERROR_ANSI; } os << "Error: "; return os; }

/// @brief Inline function to set warning color for text outout if not disabled
inline std::ostream& COLOR_WARNING(std::ostream & os) { if (!no_color) { os << COLOR_WARNING_ANSI; } os << "Warning: "; return os; }

/// @brief Inline function to set informative color for text outout if not disabled
inline std::ostream& COLOR_INFO(std::ostream& os) { if (!no_color) { os << COLOR_INFO_ANSI; } os << "Info: "; return os; }

/// @brief /dev/null equivalent for chars
extern std::ostream  cnull;

/// @brief /dev/null equivalent for wide chars
extern std::wostream wcnull;

/// @brief Convert from wide string to UTF-8
std::string  ToUtf8(const std::wstring& str);

/// @brief Convert from UTF-8 to UTF16
std::wstring utf8ToUtf16(const std::string& utf8Str);

/// @brief Convert from string to a wide string
std::wstring ToWString(const std::string& s);

/// @brief Check if path is a folder
bool is_folder(const std::filesystem::path& p);

/// @brief Check if path is a regular file
bool is_file(const std::filesystem::path& p);

/// @brief Split Arlington-style strings into vector based on separator character
std::vector<std::string> split(const std::string& s, char separator);

/// @brief Arlington predicate support
std::string remove_link_predicates(const std::string& link_in);
std::string remove_type_predicates(const std::string& types_in);

/// @brief Returns the index of a specific type in an Arlington type string or -1 if not present
int get_type_index(std::string single_type, std::string types);

std::string get_link_for_type(std::string single_type, const std::string& types, const std::string& links);

/// @brief Strip leading whitespace
std::string strip_leading_whitespace(std::string& str);

/// @brief Case insensitive comparison of two strings
bool iequals(const std::string& a, const std::string& b);

/// @brief Case insensitive substring match of s1 in s
bool icontains(const std::string& s, const std::string& s1);

/// @brief Finds a string in a vector of strings
bool FindInVector(const std::vector<std::string> list, const std::string v);

/// @brief Check if Arlington data represents an array
bool check_valid_array_definition(const std::string& fname, const std::vector<std::string>& keys, std::ostream& ofs, bool* wildcard_only);

// @brief Tests if a key is a valid PDF second class name according to Annex E of ISO 32000-2:2020
bool is_second_class_pdf_name(const std::string key);

// @brief Tests if a key is a valid PDF third class name according to Annex E of ISO 32000-2:2020
bool is_third_class_pdf_name(const std::string key);

#endif // Utils_h
