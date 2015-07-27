/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014-2015, krikit. All rights reserved. BSD 2-Clause License
 */


#include "hanal/Util.hpp"


//////////////
// includes //
//////////////
#include <map>
#include <string>

#include "hanal/Except.hpp"


namespace hanal {


////////////////////
// static members //
////////////////////
std::map<std::wstring, SejongTag> Util::_to_sejong_map = {
    {L"EC", SejongTag::EC}, {L"EF", SejongTag::EF}, {L"EP", SejongTag::EP}, {L"ETM", SejongTag::ETM},
    {L"ETN", SejongTag::ETN}, {L"IC", SejongTag::IC}, {L"JC", SejongTag::JC}, {L"JKB", SejongTag::JKB},
    {L"JKC", SejongTag::JKC}, {L"JKG", SejongTag::JKG}, {L"JKO", SejongTag::JKO}, {L"JKQ", SejongTag::JKQ},
    {L"JKS", SejongTag::JKS}, {L"JKV", SejongTag::JKV}, {L"JX", SejongTag::JX}, {L"MAG", SejongTag::MAG},
    {L"MAJ", SejongTag::MAJ}, {L"MM", SejongTag::MM}, {L"NA", SejongTag::NA}, {L"NF", SejongTag::NF},
    {L"NNB", SejongTag::NNB}, {L"NNG", SejongTag::NNG}, {L"NNP", SejongTag::NNP}, {L"NP", SejongTag::NP},
    {L"NR", SejongTag::NR}, {L"NV", SejongTag::NV}, {L"SE", SejongTag::SE}, {L"SF", SejongTag::SF},
    {L"SH", SejongTag::SH}, {L"SL", SejongTag::SL}, {L"SN", SejongTag::SN}, {L"SO", SejongTag::SO},
    {L"SP", SejongTag::SP}, {L"SS", SejongTag::SS}, {L"SW", SejongTag::SW}, {L"VA", SejongTag::VA},
    {L"VCN", SejongTag::VCN}, {L"VCP", SejongTag::VCP}, {L"VV", SejongTag::VV}, {L"VX", SejongTag::VX},
    {L"XPN", SejongTag::XPN}, {L"XR", SejongTag::XR}, {L"XSA", SejongTag::XSA}, {L"XSN", SejongTag::XSN},
    {L"XSV", SejongTag::XSV}
};


// methods //
/////////////
std::string Util::to_utf8(const wchar_t* wstr) {
  HANAL_THROW("Wide string to UTF-8 converting method is not implemented yet!");
  // TODO(krikit): implement converting from wide string to utf8 string
}


std::string Util::to_utf8(const std::wstring& wstr) {
  return to_utf8(wstr.c_str());
}


SejongTag Util::to_sejong(const wchar_t* tag) {
  auto found = _to_sejong_map.find(tag);
  HANAL_ASSERT(found != _to_sejong_map.end(), "Unknown tag: " + to_utf8(tag));
  return found->second;
}


}    // namespace hanal
