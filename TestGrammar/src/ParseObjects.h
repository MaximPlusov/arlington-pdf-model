////////////////////////////////////////////////////////////////////////////////////////////////////
// ParseObjects.h
// 2020 Roman Toda, Normex
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
  Reading the whole PDF starting from specific object and validating against grammar provided via tsv file
*/

#pragma once

#include <string>
#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <memory>
#include <algorithm> 
#include <codecvt>

#include "GrammarFile.h"
#include "Pdfix.h"
#include "utils.h"

using namespace PDFixSDK;

#ifdef GetObject
#undef GetObject
#endif

class CParsePDF
{
  std::map<PdsObject*, std::string> mapped;
  std::map<std::string, std::unique_ptr<CGrammarReader>> grammar_map;

  std::string grammar_folder;
  PdfDoc* pdf_doc;
  std::ofstream &output;
  const std::vector<std::vector<std::string>>* get_grammar(const std::string& link);

public:
  CParsePDF(PdfDoc* doc, std::string tsv_folder, std::ofstream &ofs) :
    pdf_doc(doc), grammar_folder(tsv_folder), output(ofs)
  { }

  void parse_object(PdsObject *object, const std::string &link, std::string context);
  void parse_name_tree(PdsDictionary* obj, const std::string &links, std::string context);
  void parse_number_tree(PdsDictionary* obj, const std::string &links, std::string context);

  std::string select_one(PdsObject* obj, const std::string &links_string, std::string &obj_name);
  std::string get_link_for_type(PdsObject* obj, const std::string &types, const std::string &links);
  int get_type_index(PdsObject *obj, std::string types);
  std::string get_type_string(PdsObject *obj);
  void check_basics(PdsObject *object, const std::vector<std::string> &vec, const std::string &grammar_file);
  bool check_possible_values(PdsObject* object, const std::string& possible_value_str, int index, std::wstring& real_str_value);
};

