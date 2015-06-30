/**
 * @author     krikit(krikit@naver.com)
 * @copyright  Copyright (C) 2014 - 2015,
  krikit. All rights reserved. BSD 2 - Clause License
 */


#ifndef HANAL_SEJONGTAG_HPP
#define HANAL_SEJONGTAG_HPP


namespace hanal {


/**
 * Part-of-speech tag set from Sejong corpus
 */
enum class SejongTag: int {
  EC = 0,    ///< verb ending - connective; 의존형태 - 어미 - 연결어미
  EF,        ///< verb ending - conclusive; 의존형태 - 어미 - 종결어미
  EP,        ///< verb ending - pre-final; 의존형태 - 어미 - 선어말어미
  ETM,       ///< verb ending - unconjugation adjective transitive; 의존형태 - 어미 - 관형형전성어미
  ETN,       ///< verb ending - noun transitive; 의존형태 - 어미 - 명사형전성어미
  IC,        ///< interjection; 독립언 - 감탄사
  JC,        ///< noun ending - conjunctional; 관계언 - 접속조사
  JKB,       ///< noun ending - adverbial; 관계언 - 격조사 - 부사격조사
  JKC,       ///< noun ending - case - complement; 관계언 - 격조사 - 보격조사
  JKG,       ///< noun ending - case - unconjugation adjective; 관계언 - 격조사 - 관형격조사
  JKO,       ///< noun ending - case - object; 관계언 - 격조사 - 목적격조사
  JKQ,       ///< noun ending - case - quotation; 관계언 - 격조사 - 인용격조사
  JKS,       ///< noun ending - case - subject; 관계언 - 격조사 - 주격조사
  JKV,       ///< noun ending - case - vocative; 관계언 - 격조사 - 호격조사
  JX,        ///< noun ending - auxiliary; 관계언 - 보조사
  MAG,       ///< adverb - general; 수식언 - 부사 - 일반부사
  MAJ,       ///< adverb - conjunctional; 수식언 - 부사 - 접속부사
  MM,        ///< unconjugation adjective; 수식언 - 관형사
  NA,        ///< symbol - not available; 기호 - 분석불능범주
  NF,        ///< symbol - noun estimation; 기호 - 명사추정범주
  NNB,       ///< noun - dependent; 체언 - 명사 - 의존명사
  NNG,       ///< noun - general; 체언 - 명사 - 일반명사
  NNP,       ///< noun - proper; 체언 - 명사 - 고유명사
  NP,        ///< pronoun; 체언 - 대명사
  NR,        ///< numeral; 체언 - 수사
  NV,        ///< symbol - verb estimation; 기호 - 동사추정범주
  SE,        ///< symbol - ellipsis; 기호 - 줄임표
  SF,        ///< symbol - period, question/exclamation mark; 기호 - 마침표, 물음표, 느낌표
  SH,        ///< symbol - Chinese; 기호 - 한자
  SL,        ///< symbol - foreign letter; 기호 - 외국어
  SN,        ///< symbol - number; 기호 - 숫자
  SO,        ///< symbol - tilde, O mark; 기호 - 붙임표(물결, 숨김, 빠짐)
  SP,        ///< symbol - comma, dot, colon, slash; 기호 - 쉼표, 가운뎃점, 콜론, 빗금
  SS,        ///< symbol - quote, parentheses, dash; 기호 - 따옴표, 괄호, 줄표
  SW,        ///< symbol - etc; 기호 - 기타 기호(수학 논리기호, 화폐 기호 등)
  VA,        ///< adjective; 용언 - 형용사
  VCN,       ///< copula - negative; 용언 - 지정사 - 부정지정사
  VCP,       ///< copula - positive; 용언 - 지정사 - 긍정지정사
  VV,        ///< verb - main; 용언 - 동사
  VX,        ///< verb - auxiliary; 용언 - 보조용언
  XPN,       ///< prefix - noun; 의존형태 - 접두사 - 체언접두사
  XR,        ///< word stem; 의존형태 - 어근
  XSA,       ///< suffix - adjective derivative; 의존형태 - 접미사 - 형용사파생접미사
  XSN,       ///< suffix - noun derivative; 의존형태 - 접미사 - 명사파생접미사
  XSV,       ///< suffix - verb derivative; 의존형태 - 접미사 - 동사파생접미사
  _SIZE
};


}    // namespace hanal


#endif  // HANAL_SEJONGTAG_HPP
