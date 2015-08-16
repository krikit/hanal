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

#include "boost/lexical_cast.hpp"
#include "boost/locale.hpp"
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
std::array<const wchar_t*, static_cast<size_t>(SejongTag::_SIZE)> Util::_from_sejong_arr = {
    L"EC", L"EF", L"EP", L"ETM", L"ETN", L"IC", L"JC", L"JKB", L"JKC", L"JKG",
    L"JKO", L"JKQ", L"JKS", L"JKV", L"JX", L"MAG", L"MAJ", L"MM", L"NA", L"NF",
    L"NNB", L"NNG", L"NNP", L"NP", L"NR", L"NV", L"SE", L"SF", L"SH", L"SL",
    L"SN", L"SO", L"SP", L"SS", L"SW", L"VA", L"VCN", L"VCP", L"VV", L"VX",
    L"XPN", L"XR", L"XSA", L"XSN", L"XSV"
};


/////////////
// methods //
/////////////
std::string Util::to_utf8(const wchar_t* wstr) {
  return boost::locale::conv::utf_to_utf<char>(wstr);
}


std::string Util::to_utf8(const std::wstring& wstr) {
  return to_utf8(wstr.c_str());
}


SejongTag Util::to_sejong(const wchar_t* tag) {
  auto found = _to_sejong_map.find(tag);
  HANAL_ASSERT(found != _to_sejong_map.end(), "Unknown tag: " + to_utf8(tag));
  return found->second;
}


const wchar_t* Util::from_sejong(SejongTag sejong) {
  HANAL_ASSERT(SejongTag(0) <= sejong && sejong < SejongTag::_SIZE,
               "Invalid sejong tag: " + boost::lexical_cast<std::string>(static_cast<int>(sejong)));
  return _from_sejong_arr[static_cast<size_t>(sejong)];
}


}    // namespace hanal
