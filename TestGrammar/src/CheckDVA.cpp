///////////////////////////////////////////////////////////////////////////////
// CheckDVA.cpp
// Copyright 2020 PDF Association, Inc. https://www.pdfa.org
//
// This material is based upon work supported by the Defense Advanced
// Research Projects Agency (DARPA) under Contract No. HR001119C0079.
// Any opinions, findings and conclusions or recommendations expressed
// in this material are those of the author(s) and do not necessarily
// reflect the views of the Defense Advanced Research Projects Agency
// (DARPA). Approved for public release.
//
// SPDX-License-Identifier: Apache-2.0
// Contributors: Roman Toda, Frantisek Forgac, Normex
///////////////////////////////////////////////////////////////////////////////

#include <exception>
#include <iostream>
#include <string>
#include <filesystem>
#include <queue>
#include <map>

#include "Pdfix.h"
#include "CheckGrammar.h"
#include "GrammarFile.h"
#include "TestGrammarVers.h"

using namespace PDFixSDK;

// simulating recursive processing of the PDObjects
struct to_process_elem {
  std::wstring dva_link;
  std::wstring dva_link2;
  std::string link;
  to_process_elem(const std::wstring dva_lnk, std::string our_lnk)
    : dva_link(dva_lnk), link(our_lnk),dva_link2(L"")
  {}
  to_process_elem(const std::wstring dva_lnk1, const std::wstring dva_lnk2, std::string our_lnk)
    : dva_link(dva_lnk2), link(our_lnk), dva_link2(dva_lnk1)
  {}
};
std::queue<to_process_elem> to_process_checks;
PdsDictionary* map_dict;
std::ofstream ofg;

void process_dict(std::ofstream& ofs) {
  int count = 0;
  std::map<std::string, std::wstring> mapped;
  ofg.open("w:\\global.txt");

  to_process_checks.emplace(L"Catalog", "Catalog");

  to_process_checks.emplace(L"Font", L"FontType1", "FontType1");
  to_process_checks.emplace(L"Font", L"FontTrueType", "FontTrueType");
  to_process_checks.emplace(L"Font", L"FontMMType1", "FontMultipleMaster");
  to_process_checks.emplace(L"Font", L"FontType3", "FontType3");
  to_process_checks.emplace(L"Font", L"FontType0", "FontType0");
  to_process_checks.emplace(L"Font", L"FontCIDFontType0or2", "FontCIDType0");
  to_process_checks.emplace(L"Font", L"FontCIDFontType0or2", "FontCIDType2");

  //OPIDict
  to_process_checks.emplace(L"OPI1.3", "OPIVersion13");
  to_process_checks.emplace(L"OPI2.0", "OPIVersion20");

  to_process_checks.emplace(L"PagesOrPage",L"Pages", "PageTreeNode");
  to_process_checks.emplace(L"PagesOrPage",L"Page","PageObject");

  //Bead----BeadFirst,Bead
  to_process_checks.emplace(L"Bead_First", "BeadFirst");
  to_process_checks.emplace(L"Bead", "Bead");

  //OCGorOCMD----,
  to_process_checks.emplace(L"OCGorOCMD",L"OCG", "OptContentGroup");
  to_process_checks.emplace(L"OCGorOCMD",L"OCMD", "OptContentMembership");

  //Outline----OutlineItem,Outline
  to_process_checks.emplace(L"Outline", "OutlineItem");
  to_process_checks.emplace(L"Outlines", "Outline");

  //Pattern----PatternType1,PatternType2
  to_process_checks.emplace(L"Pattern",L"PatternType1", "PatternType1");
  to_process_checks.emplace(L"Pattern",L"PatternType2", "PatternType2");

  //XObject----XObjectFormType1,XObjectImage
  to_process_checks.emplace(L"XObject",L"XObjectForm", "XObjectFormType1");
  to_process_checks.emplace(L"XObject",L"XObjectImageBase", "XObjectImage");

  //Rendition----RenditionMedia,RenditionSelector
  to_process_checks.emplace(L"Rendition",L"MediaRendition", "RenditionMedia");
  to_process_checks.emplace(L"Rendition",L"SelectorRendition", "RenditionSelector");

  //SigRef----SignatureReferenceDocMDP,SignatureReferenceFieldMDP,SignatureReferenceIdentity,SignatureReferenceUR
  to_process_checks.emplace(L"SigRef",L"SigRefDocMDP", "SignatureReferenceDocMDP");
  to_process_checks.emplace(L"SigRef",L"SigRefFieldMDP", "SignatureReferenceFieldMDP");
  to_process_checks.emplace(L"SigRef",L"SigRefIdentity", "SignatureReferenceIdentity");
  to_process_checks.emplace(L"SigRef",L"SigRefUR", "SignatureReferenceUR");

  //Action----ActionGoTo,ActionGoToR,ActionGoToE,ActionGoToDp,ActionLaunch,ActionThread,ActionURI,ActionSound,ActionMovie,ActionHide,
  //ActionNamed,ActionSubmitForm,ActionResetForm,ActionImportData,ActionSetOCGState,ActionRendition,ActionTransition,ActionGoTo3DView,
  //ActionECMAScript,ActionRichMediaExecute
  to_process_checks.emplace(L"Action",L"ActionGoTo", "ActionGoTo");
  to_process_checks.emplace(L"Action",L"ActionGoToR", "ActionGoToR");
  to_process_checks.emplace(L"Action",L"ActionGoToE", "ActionGoToE");
  to_process_checks.emplace(L"Action",L"ActionLaunch", "ActionLaunch");
  to_process_checks.emplace(L"Action",L"ActionThread", "ActionThread");
  to_process_checks.emplace(L"Action",L"ActionURI", "ActionURI");
  to_process_checks.emplace(L"Action",L"ActionSound", "ActionSound");
  to_process_checks.emplace(L"Action",L"ActionMovie", "ActionMovie");
  to_process_checks.emplace(L"Action",L"ActionHide", "ActionHide");
  to_process_checks.emplace(L"Action",L"ActionNamed", "ActionNamed");
  to_process_checks.emplace(L"Action",L"ActionSubmitForm", "ActionSubmitForm");
  to_process_checks.emplace(L"Action",L"ActionResetForm", "ActionResetForm");
  to_process_checks.emplace(L"Action",L"ActionImportData", "ActionImportData");
  to_process_checks.emplace(L"Action",L"ActionSetOCGState", "ActionSetOCGState");
  to_process_checks.emplace(L"Action",L"ActionRendition", "ActionRendition");
  to_process_checks.emplace(L"Action",L"ActionTrans", "ActionTransition");
  to_process_checks.emplace(L"Action",L"ActionGoTo3DView", "ActionGoTo3DView");
  to_process_checks.emplace(L"Action",L"ActionJavaScript", "ActionECMAScript");

  to_process_checks.emplace(L"Annot", L"AnnotText", "AnnotText");
  to_process_checks.emplace(L"Annot", L"AnnotLink", "AnnotLink");
  to_process_checks.emplace(L"Annot", L"AnnotFreeText", "AnnotFreeText");
  to_process_checks.emplace(L"Annot", L"AnnotLine", "AnnotLine");
  to_process_checks.emplace(L"Annot", L"AnnotSquare", "AnnotSquare");
  to_process_checks.emplace(L"Annot", L"AnnotCircle", "AnnotCircle");
  to_process_checks.emplace(L"Annot", L"AnnotPolygon", "AnnotPolygon");
  to_process_checks.emplace(L"Annot", L"AnnotPolyLine", "AnnotPolyLine");
  to_process_checks.emplace(L"Annot", L"AnnotHighlight", "AnnotHighlight");
  to_process_checks.emplace(L"Annot", L"AnnotUnderline", "AnnotUnderline");
  to_process_checks.emplace(L"Annot", L"AnnotSquiggly", "AnnotSquiggly");
  to_process_checks.emplace(L"Annot", L"AnnotStrikeOut", "AnnotStrikeOut");
  to_process_checks.emplace(L"Annot", L"AnnotCaret", "AnnotCaret");
  to_process_checks.emplace(L"Annot", L"AnnotStamp", "AnnotStamp");
  to_process_checks.emplace(L"Annot", L"AnnotInk", "AnnotInk");
  to_process_checks.emplace(L"Annot", L"AnnotPopup", "AnnotPopup");
  to_process_checks.emplace(L"Annot", L"AnnotFileAttachment", "AnnotFileAttachment");
  to_process_checks.emplace(L"Annot", L"AnnotSound", "AnnotSound");
  to_process_checks.emplace(L"Annot", L"AnnotMovie", "AnnotMovie");
  to_process_checks.emplace(L"Annot", L"AnnotScreen", "AnnotScreen");
  to_process_checks.emplace(L"Annot", L"AnnotWidget", "AnnotWidget");
  to_process_checks.emplace(L"Annot", L"AnnotPrinterMark", "AnnotPrinterMark");
  to_process_checks.emplace(L"Annot", L"AnnotTrapNet", "AnnotTrapNetwork");
  to_process_checks.emplace(L"Annot", L"AnnotWatermark", "AnnotWatermark");
  to_process_checks.emplace(L"Annot", L"Annot3D", "Annot3D");



  while (!to_process_checks.empty()) {
    to_process_elem elem = to_process_checks.front();
    to_process_checks.pop();
    if (elem.link == "")
      continue;

    std::vector<std::string> links = split(elem.link, ',');
    if (links.size() > 1) {
      // ofg << ToUtf8(elem.dva_link) << "----"<< elem.link << std::endl;
      //for (auto lnk : links)
      //  to_process_checks.emplace(elem.dva_link, lnk);
      continue;
    }
    std::string function;
    elem.link = extract_function(elem.link, function);


    auto found = mapped.find(elem.link);
    if (found != mapped.end()) {
      //    output << context << " already Processed" <<std::endl;
      if (found->second != elem.dva_link) {
        //rt ofs << "TODO: link validated in 2 different contexts: " << ToUtf8(found->second) << std::endl;
        //        output << " second: " << elem.link << " in: " << elem.context << std::endl;
      }
      continue;
    }
    mapped.insert(std::make_pair(elem.link, elem.dva_link));

    // locate dict in DVA
    PdsDictionary* dict = (PdsDictionary*)map_dict->Get(elem.dva_link.c_str());
    if (dict == nullptr) {
      ofs << "DVA problem: " << ToUtf8(elem.dva_link) << std::endl;
      return;
    }

    //load "dict" in OUR
    std::string grammar_file = "w:\\PDF20_Grammar\\tsv\\latest\\" + elem.link + ".tsv";
    std::unique_ptr<CGrammarReader> reader(new CGrammarReader(grammar_file));
    reader->load();
    const std::vector<std::vector<std::string>>* data_list = &reader->get_data();
    ofs << count++ << "..Comparing OUR:" << elem.link << " vs. DVA:" << ToUtf8(elem.dva_link) << std::endl;
    ofg << elem.link << std::endl;


    // what we have and DVA doesn't
    for (auto& vec : *data_list) {
      PdsDictionary* inner_obj = nullptr;
      if (vec[TSV_KEYNAME] == "*") {
        inner_obj = (PdsDictionary*)dict->Get(L"GenericKey");
        if (inner_obj == nullptr) {
          PdsArray* inner_array = (PdsArray*)dict->Get(L"Array");
          if (inner_array == nullptr || inner_array->GetNumObjects() > 1) {
           //rt  ofs << "TODO: Array - either not linked or multiple links: " << ToUtf8(elem.dva_link) << "::" << vec[TSV_KEYNAME] << std::endl;
          }
          else inner_obj = (PdsDictionary*)inner_array->Get(0);
        }
      }
      else inner_obj = (PdsDictionary*)dict->Get(utf8ToUtf16(vec[TSV_KEYNAME]).c_str());

      // could be in "ConcatWithFormalReps"
      if (inner_obj == nullptr) {
        PdsArray* inner_array = (PdsArray*)dict->Get(L"ConcatWithFormalReps");
        if (inner_array != nullptr) {
          std::wstring new_dva_value;
          new_dva_value.resize(inner_array->GetText(0, nullptr, 0));
          inner_array->GetText(0, (wchar_t*)new_dva_value.c_str(), (int)new_dva_value.size());
          inner_obj = (PdsDictionary*)((PdsDictionary*)map_dict->Get(new_dva_value.c_str()))->Get(utf8ToUtf16(vec[TSV_KEYNAME]).c_str());
        } else if (elem.dva_link2!=L"") inner_obj = (PdsDictionary*)((PdsDictionary*)map_dict->Get(elem.dva_link2.c_str()))->Get(utf8ToUtf16(vec[TSV_KEYNAME]).c_str());
      }

      if (inner_obj == nullptr) {
        ofs << "Missing in DVA: " << ToUtf8(elem.dva_link) << "::" << vec[TSV_KEYNAME] << std::endl;
        continue;
      }
      else {
        //check attributtes
        
//Check Inheritable, Required and SinceVersion
        std::string indirect = "FALSE";
        if (inner_obj->GetBoolean(L"MustBeIndirect", false)) indirect = "TRUE";
        std::string required = "FALSE";
        if (inner_obj->GetBoolean(L"Required", false)) required = "TRUE";
        if (vec[TSV_INDIRECTREF]!= indirect)
          ofs << "Indirect is different DVA:" << ToUtf8(elem.dva_link) << " OUR:" << elem.link <<"::" << vec[TSV_KEYNAME] << "==" << vec[TSV_INDIRECTREF] << std::endl;
        if (vec[TSV_REQUIRED] != required)
          ofs << "Required is different DVA:" << ToUtf8(elem.dva_link) << " OUR:" << elem.link << "::" << vec[TSV_KEYNAME] <<"=="<< vec[TSV_REQUIRED] << std::endl;

        //std::string ver = std::to_string(inner_obj->GetInteger(L"PDFMajorVersion", 0)) + "." + std::to_string(inner_obj->GetInteger(L"PDFMinorVersion", 0));
        //if (ver != vec[TSV_SINCEVERSION])
        //  ofs << "Since version is different DVA:" << ToUtf8(elem.dva_link) << " OUR:" << elem.link << "::" << vec[TSV_KEYNAME] << " " << ver << " vs. " << vec[TSV_SINCEVERSION] << std::endl;

//Check Allowed Types
        PdsArray* types_array = (PdsArray*)inner_obj->Get(L"ValueType");
        if (types_array==nullptr)
          ofs << "Types not defined in DVA:" << ToUtf8(elem.dva_link) << "::" << vec[TSV_KEYNAME] << std::endl;
        else {
          std::vector<std::string>types_our = split(vec[TSV_TYPE], ';');
          for (int i = 0; i < types_our.size(); i++) {
            if (types_our[i] == "BOOLEAN") types_our[i] = "CosBool";
            if (types_our[i] == "NAME") types_our[i] = "CosName";
            if (types_our[i] == "NUMBER") { types_our[i] = "CosFixed"; types_our.push_back("CosInteger"); }
            if (types_our[i] == "INTEGER") types_our[i] = "CosInteger";
            if (types_our[i] == "STREAM") types_our[i] = "CosStream";
            if (types_our[i] == "ARRAY" || types_our[i] == "RECTANGLE") types_our[i] = "CosArray";
            if (types_our[i] == "DICTIONARY" || types_our[i] == "NAME-TREE" || types_our[i] == "NUMBER-TREE") types_our[i] = "CosDict";
            if (types_our[i] == "STRING" || types_our[i] == "DATE" || types_our[i] == "STRING-BYTE"
              || types_our[i] == "STRING-TEXT" || types_our[i] == "STRING-ASCII") types_our[i] = "CosString";
          }
          std::vector<std::string> types_dva;
          for (int i = 0; i < types_array->GetNumObjects(); i++) {
            std::wstring new_dva_value;
            new_dva_value.resize(types_array->GetText(i, nullptr, 0));
            types_array->GetText(i, (wchar_t*)new_dva_value.c_str(), (int)new_dva_value.size());
            for (int i = 0; i < types_our.size(); i++) 
              if (types_our[i] == ToUtf8(new_dva_value)) {
                types_our[i] = "";
                new_dva_value = L"";
                break;
              }
            types_dva.push_back(ToUtf8(new_dva_value));
          }
          
          std::string head = "==Key " + ToUtf8(elem.dva_link) +" " + elem.link + "::" + vec[TSV_KEYNAME] + "\n";
          std::string our("");
          for (auto& tpe : types_our)
            if (tpe != "") our += tpe + ",";
          if (our != "") {
            if (head != "") {
              ofs << head; head = "";
            }
            ofs << "\tOUR:" << our << std::endl;
          }
          our = "";
          for (auto& tpe : types_dva)
            if (tpe != "") our += tpe + ",";
          if (our != "") {
            if (head != "") {
              ofs << head; head = "";
            }
            ofs << "\tDVA:" << our << std::endl;
          }
        }

//Check Possible Value
        PdsDictionary *bounds_dict = (PdsDictionary*)inner_obj->Get(L"Bounds");
        std::string possible = "";
        if (vec[TSV_POSSIBLEVALUES]!="")
          possible = vec[TSV_POSSIBLEVALUES].substr(1, vec[TSV_POSSIBLEVALUES].size() - 2);
        if (bounds_dict == nullptr ) {
          if (possible != "")ofs << "Possible not defined in DVA:" << ToUtf8(elem.dva_link) << "::" << vec[TSV_KEYNAME] << std::endl;
        }
        else {
          PdsArray* possible_array = (PdsArray*)bounds_dict->Get(L"Equals");
          std::vector<std::string>possible_our = split(possible, ',');
          if (possible_array == nullptr) {
            if (possible != "") ofs << "Possible not defined DVA:" << ToUtf8(elem.dva_link) << "::" << vec[TSV_KEYNAME] << std::endl;
          }
          else {
            std::vector<std::string> possible_dva;
            for (int i = 0; i < possible_array->GetNumObjects(); i++) {
              std::wstring new_dva_value;
              new_dva_value.resize(possible_array->GetText(i, nullptr, 0));
              possible_array->GetText(i, (wchar_t*)new_dva_value.c_str(), (int)new_dva_value.size());
              for (int i = 0; i < possible_our.size(); i++) {
                std::string function;
                possible_our[i] = extract_function(possible_our[i], function);
                if (possible_our[i] == ToUtf8(new_dva_value)) {
                  possible_our[i] = "";
                  new_dva_value = L"";
                  break;
                }
              }
              possible_dva.push_back(ToUtf8(new_dva_value));
            }

            std::string head = "==Possible " + ToUtf8(elem.dva_link) + " " + elem.link + "::" + vec[TSV_KEYNAME] + "\n";
            std::string our("");
            for (auto& tpe : possible_our)
              if (tpe != "") our += tpe + ",";
            if (our != "") {
              if (head != "") {
                ofs << head; head = "";
              }
              ofs << "\tOUR:" << our << std::endl;
            }
            our = "";
            for (auto& tpe : possible_dva)
              if (tpe != "") our += tpe + ",";
            if (our != "") {
              if (head != "") {
                ofs << head; head = "";
              }
              ofs << "\tDVA:" << our << std::endl;
            }
          }
        }
      }

      PdsObject* link_obj = inner_obj->Get(L"VerifyAtFormalRep"); //0-dict, 1-stream, 2-array
      PdsObject* recursion_obj = inner_obj->Get(L"RecursionParams");
      if (recursion_obj != nullptr) {
        // ofs << "TODO: RecursionParams - special validation required: " << elem.link << "::" << vec[TSV_KEYNAME] << std::endl;
        link_obj = ((PdsDictionary*)recursion_obj)->Get(L"VerifyAtFormalRep"); //0-dict, 1-stream, 2-array
      }

      if (link_obj != nullptr) {
        // could array or name
        if (link_obj->GetObjectType() == kPdsName) {
          std::wstring link_str_value;
          link_str_value.resize(((PdsName*)link_obj)->GetText(nullptr, 0));
          ((PdsName*)link_obj)->GetText((wchar_t*)link_str_value.c_str(), (int)link_str_value.size());
          std::vector<std::string> lnk = split(vec[TSV_LINK], ';');
          if (vec[TSV_LINK] == "" || lnk.size() != 1) {
            ofs << "Wrong link in OUR: " << elem.link << "::" << vec[TSV_KEYNAME] << std::endl;
          }
          else to_process_checks.emplace(link_str_value, lnk[0].substr(1, lnk[0].size() - 2));
        }
        else {
          std::wstring link_dict_value;
          link_dict_value.resize(((PdsArray*)link_obj)->GetText(0, nullptr, 0));
          ((PdsArray*)link_obj)->GetText(0, (wchar_t*)link_dict_value.c_str(), (int)link_dict_value.size());

          std::wstring link_stream_value;
          link_stream_value.resize(((PdsArray*)link_obj)->GetText(1, nullptr, 0));
          ((PdsArray*)link_obj)->GetText(1, (wchar_t*)link_stream_value.c_str(), (int)link_stream_value.size());

          std::wstring link_array_value;
          link_array_value.resize(((PdsArray*)link_obj)->GetText(2, nullptr, 0));
          ((PdsArray*)link_obj)->GetText(2, (wchar_t*)link_array_value.c_str(), (int)link_array_value.size());

          std::string lnk_dict = get_link_for_type("DICTIONARY", vec[TSV_TYPE], vec[TSV_LINK]);
          std::string lnk_stream = get_link_for_type("STREAM", vec[TSV_TYPE], vec[TSV_LINK]);
          std::string lnk_array = get_link_for_type("ARRAY", vec[TSV_TYPE], vec[TSV_LINK]);

          if (link_dict_value != L"" && lnk_dict != "[]")
            to_process_checks.emplace(link_dict_value, lnk_dict.substr(1, lnk_dict.size() - 2));
          else if (!(link_dict_value == L"" && lnk_dict == "[]")) {
            //rt ofs << "TODO: Check link dictionary: " << ToUtf8(elem.dva_link) << "::" << vec[TSV_KEYNAME] << std::endl;
          }

          if (link_stream_value != L"" && lnk_stream != "[]")
            to_process_checks.emplace(link_stream_value, lnk_stream.substr(1, lnk_stream.size() - 2));
          else if (!(link_stream_value == L"" && lnk_stream == "[]")) {
            //rt ofs << "TODO: Check link stream: " << ToUtf8(elem.dva_link) << "::" << vec[TSV_KEYNAME] << std::endl;
          }

          if (link_array_value != L"" && lnk_array != "[]")
            to_process_checks.emplace(link_array_value, lnk_array.substr(1, lnk_array.size() - 2));
          else if (!(link_array_value == L"" && lnk_array == "[]")) {
            //rt ofs << "TODO: Check link array: " << ToUtf8(elem.dva_link) << "::" << vec[TSV_KEYNAME] << std::endl;
          }
        }
      };
    }

    auto exits_in_our = [=](auto key) {
      for (auto& vec : *data_list)
        if (vec[TSV_KEYNAME] == ToUtf8(key))
          return true;
      return false;
    };

    auto check_dict = [=](PdsDictionary* a_dict, std::ofstream& in_ofs) {
      for (int i = 0; i < (a_dict->GetNumKeys()); i++) {
        std::wstring key;
        key.resize(a_dict->GetKey(i, nullptr, 0));
        a_dict->GetKey(i, (wchar_t*)key.c_str(), (int)key.size());
        if (!exits_in_our(key) && key != L"FormalRepOf" && key != L"Array"
          && key != L"ArrayStyle" && key != L"FormalRepOfArray" && key != L"OR"
          && key != L"GenericKey" && key != L"ConcatWithFormalReps") {

          in_ofs << "Missing in OUR: " << elem.link << "::" << ToUtf8(key) << std::endl;
        }
      }
    };

    check_dict(dict,ofs);

    PdsArray* inner_array = (PdsArray*)dict->Get(L"ConcatWithFormalReps");
    if (inner_array != nullptr) {
      std::wstring new_dva_value;
      new_dva_value.resize(inner_array->GetText(0, nullptr, 0));
      inner_array->GetText(0, (wchar_t*)new_dva_value.c_str(), (int)new_dva_value.size());
      check_dict((PdsDictionary*)map_dict->Get(new_dva_value.c_str()),ofs);
    }
  }
}

void CheckDVA(std::wstring& dva_file, std::string& grammar_folder, std::ofstream& ofs) {
  Pdfix* pdfix = GetPdfix();
  ofs << "DVA Report - TestGrammar ver." << TestGrammar_VERSION << std::endl;

  PdfDoc* doc = pdfix->OpenDoc(dva_file.c_str(), L"");
  PdsObject* root = doc->GetRootObject();
  map_dict = (PdsDictionary*)((PdsDictionary*)root)->Get(L"FormalRepTree");
  process_dict(ofs);

  ofs << "END" << std::endl;
  ofs.close();
  if (doc != nullptr)
    doc->Close();
}
