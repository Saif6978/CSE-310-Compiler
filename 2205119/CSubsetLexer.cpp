
    #pragma once
    #include <iostream>
    #include <fstream>
    #include <string>

    extern std::ofstream lexLogFile;


// Generated from CSubset.g4 by ANTLR 4.13.2


#include "CSubsetLexer.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct CSubsetLexerStaticData final {
  CSubsetLexerStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  CSubsetLexerStaticData(const CSubsetLexerStaticData&) = delete;
  CSubsetLexerStaticData(CSubsetLexerStaticData&&) = delete;
  CSubsetLexerStaticData& operator=(const CSubsetLexerStaticData&) = delete;
  CSubsetLexerStaticData& operator=(CSubsetLexerStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> channelNames;
  const std::vector<std::string> modeNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag csubsetlexerLexerOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<CSubsetLexerStaticData> csubsetlexerLexerStaticData = nullptr;

void csubsetlexerLexerInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (csubsetlexerLexerStaticData != nullptr) {
    return;
  }
#else
  assert(csubsetlexerLexerStaticData == nullptr);
#endif
  auto staticData = std::make_unique<CSubsetLexerStaticData>(
    std::vector<std::string>{
      "LINE_COMMENT", "BLOCK_COMMENT", "STRING", "WS", "IF", "ELSE", "FOR", 
      "WHILE", "PRINTLN", "RETURN", "INT", "FLOAT", "VOID", "LPAREN", "RPAREN", 
      "LCURL", "RCURL", "LTHIRD", "RTHIRD", "SEMICOLON", "COMMA", "ADDOP", 
      "MULOP", "INCOP", "DECOP", "NOT", "RELOP", "LOGICOP", "ASSIGNOP", 
      "ID", "CONST_INT", "CONST_FLOAT"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
    },
    std::vector<std::string>{
      "", "", "", "", "", "'if'", "'else'", "'for'", "'while'", "'println'", 
      "'return'", "'int'", "'float'", "'void'", "'('", "')'", "'{'", "'}'", 
      "'['", "']'", "';'", "','", "", "", "'++'", "'--'", "'!'", "", "", 
      "'='"
    },
    std::vector<std::string>{
      "", "LINE_COMMENT", "BLOCK_COMMENT", "STRING", "WS", "IF", "ELSE", 
      "FOR", "WHILE", "PRINTLN", "RETURN", "INT", "FLOAT", "VOID", "LPAREN", 
      "RPAREN", "LCURL", "RCURL", "LTHIRD", "RTHIRD", "SEMICOLON", "COMMA", 
      "ADDOP", "MULOP", "INCOP", "DECOP", "NOT", "RELOP", "LOGICOP", "ASSIGNOP", 
      "ID", "CONST_INT", "CONST_FLOAT"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,0,32,307,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
  	6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,
  	7,14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,
  	7,21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,
  	7,28,2,29,7,29,2,30,7,30,2,31,7,31,1,0,1,0,1,0,1,0,5,0,70,8,0,10,0,12,
  	0,73,9,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,5,1,84,8,1,10,1,12,1,87,
  	9,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,2,1,2,1,2,5,2,100,8,2,10,2,12,2,
  	103,9,2,1,2,1,2,1,2,1,2,1,2,1,3,4,3,111,8,3,11,3,12,3,112,1,3,1,3,1,4,
  	1,4,1,4,1,4,1,4,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,6,1,6,1,6,1,6,1,6,1,6,1,
  	7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,8,1,8,1,8,1,8,1,8,1,8,1,8,1,8,1,8,1,8,
  	1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,10,1,10,1,10,1,10,1,10,1,10,1,11,
  	1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,12,1,12,1,12,1,12,1,12,1,12,1,12,
  	1,13,1,13,1,13,1,14,1,14,1,14,1,15,1,15,1,15,1,16,1,16,1,16,1,17,1,17,
  	1,17,1,18,1,18,1,18,1,19,1,19,1,19,1,20,1,20,1,20,1,21,1,21,1,21,1,22,
  	1,22,1,22,1,23,1,23,1,23,1,23,1,23,1,24,1,24,1,24,1,24,1,24,1,25,1,25,
  	1,25,1,26,1,26,1,26,1,26,1,26,1,26,1,26,1,26,1,26,1,26,1,26,3,26,237,
  	8,26,1,27,1,27,1,27,1,27,1,27,1,27,3,27,245,8,27,1,28,1,28,1,28,1,29,
  	1,29,5,29,252,8,29,10,29,12,29,255,9,29,1,29,1,29,1,30,4,30,260,8,30,
  	11,30,12,30,261,1,30,1,30,1,31,4,31,267,8,31,11,31,12,31,268,1,31,1,31,
  	5,31,273,8,31,10,31,12,31,276,9,31,3,31,278,8,31,1,31,1,31,3,31,282,8,
  	31,1,31,4,31,285,8,31,11,31,12,31,286,3,31,289,8,31,1,31,1,31,1,31,4,
  	31,294,8,31,11,31,12,31,295,1,31,1,31,4,31,300,8,31,11,31,12,31,301,1,
  	31,1,31,3,31,306,8,31,1,85,0,32,1,1,3,2,5,3,7,4,9,5,11,6,13,7,15,8,17,
  	9,19,10,21,11,23,12,25,13,27,14,29,15,31,16,33,17,35,18,37,19,39,20,41,
  	21,43,22,45,23,47,24,49,25,51,26,53,27,55,28,57,29,59,30,61,31,63,32,
  	1,0,10,2,0,10,10,13,13,4,0,10,10,13,13,34,34,92,92,3,0,9,10,12,13,32,
  	32,2,0,43,43,45,45,3,0,37,37,42,42,47,47,2,0,60,60,62,62,3,0,65,90,95,
  	95,97,122,4,0,48,57,65,90,95,95,97,122,1,0,48,57,2,0,69,69,101,101,329,
  	0,1,1,0,0,0,0,3,1,0,0,0,0,5,1,0,0,0,0,7,1,0,0,0,0,9,1,0,0,0,0,11,1,0,
  	0,0,0,13,1,0,0,0,0,15,1,0,0,0,0,17,1,0,0,0,0,19,1,0,0,0,0,21,1,0,0,0,
  	0,23,1,0,0,0,0,25,1,0,0,0,0,27,1,0,0,0,0,29,1,0,0,0,0,31,1,0,0,0,0,33,
  	1,0,0,0,0,35,1,0,0,0,0,37,1,0,0,0,0,39,1,0,0,0,0,41,1,0,0,0,0,43,1,0,
  	0,0,0,45,1,0,0,0,0,47,1,0,0,0,0,49,1,0,0,0,0,51,1,0,0,0,0,53,1,0,0,0,
  	0,55,1,0,0,0,0,57,1,0,0,0,0,59,1,0,0,0,0,61,1,0,0,0,0,63,1,0,0,0,1,65,
  	1,0,0,0,3,78,1,0,0,0,5,95,1,0,0,0,7,110,1,0,0,0,9,116,1,0,0,0,11,121,
  	1,0,0,0,13,128,1,0,0,0,15,134,1,0,0,0,17,142,1,0,0,0,19,152,1,0,0,0,21,
  	161,1,0,0,0,23,167,1,0,0,0,25,175,1,0,0,0,27,182,1,0,0,0,29,185,1,0,0,
  	0,31,188,1,0,0,0,33,191,1,0,0,0,35,194,1,0,0,0,37,197,1,0,0,0,39,200,
  	1,0,0,0,41,203,1,0,0,0,43,206,1,0,0,0,45,209,1,0,0,0,47,212,1,0,0,0,49,
  	217,1,0,0,0,51,222,1,0,0,0,53,236,1,0,0,0,55,244,1,0,0,0,57,246,1,0,0,
  	0,59,249,1,0,0,0,61,259,1,0,0,0,63,305,1,0,0,0,65,66,5,47,0,0,66,67,5,
  	47,0,0,67,71,1,0,0,0,68,70,8,0,0,0,69,68,1,0,0,0,70,73,1,0,0,0,71,69,
  	1,0,0,0,71,72,1,0,0,0,72,74,1,0,0,0,73,71,1,0,0,0,74,75,6,0,0,0,75,76,
  	1,0,0,0,76,77,6,0,1,0,77,2,1,0,0,0,78,79,5,47,0,0,79,80,5,42,0,0,80,85,
  	1,0,0,0,81,84,9,0,0,0,82,84,7,0,0,0,83,81,1,0,0,0,83,82,1,0,0,0,84,87,
  	1,0,0,0,85,86,1,0,0,0,85,83,1,0,0,0,86,88,1,0,0,0,87,85,1,0,0,0,88,89,
  	5,42,0,0,89,90,5,47,0,0,90,91,1,0,0,0,91,92,6,1,2,0,92,93,1,0,0,0,93,
  	94,6,1,1,0,94,4,1,0,0,0,95,101,5,34,0,0,96,97,5,92,0,0,97,100,9,0,0,0,
  	98,100,8,1,0,0,99,96,1,0,0,0,99,98,1,0,0,0,100,103,1,0,0,0,101,99,1,0,
  	0,0,101,102,1,0,0,0,102,104,1,0,0,0,103,101,1,0,0,0,104,105,5,34,0,0,
  	105,106,6,2,3,0,106,107,1,0,0,0,107,108,6,2,1,0,108,6,1,0,0,0,109,111,
  	7,2,0,0,110,109,1,0,0,0,111,112,1,0,0,0,112,110,1,0,0,0,112,113,1,0,0,
  	0,113,114,1,0,0,0,114,115,6,3,1,0,115,8,1,0,0,0,116,117,5,105,0,0,117,
  	118,5,102,0,0,118,119,1,0,0,0,119,120,6,4,4,0,120,10,1,0,0,0,121,122,
  	5,101,0,0,122,123,5,108,0,0,123,124,5,115,0,0,124,125,5,101,0,0,125,126,
  	1,0,0,0,126,127,6,5,5,0,127,12,1,0,0,0,128,129,5,102,0,0,129,130,5,111,
  	0,0,130,131,5,114,0,0,131,132,1,0,0,0,132,133,6,6,6,0,133,14,1,0,0,0,
  	134,135,5,119,0,0,135,136,5,104,0,0,136,137,5,105,0,0,137,138,5,108,0,
  	0,138,139,5,101,0,0,139,140,1,0,0,0,140,141,6,7,7,0,141,16,1,0,0,0,142,
  	143,5,112,0,0,143,144,5,114,0,0,144,145,5,105,0,0,145,146,5,110,0,0,146,
  	147,5,116,0,0,147,148,5,108,0,0,148,149,5,110,0,0,149,150,1,0,0,0,150,
  	151,6,8,8,0,151,18,1,0,0,0,152,153,5,114,0,0,153,154,5,101,0,0,154,155,
  	5,116,0,0,155,156,5,117,0,0,156,157,5,114,0,0,157,158,5,110,0,0,158,159,
  	1,0,0,0,159,160,6,9,9,0,160,20,1,0,0,0,161,162,5,105,0,0,162,163,5,110,
  	0,0,163,164,5,116,0,0,164,165,1,0,0,0,165,166,6,10,10,0,166,22,1,0,0,
  	0,167,168,5,102,0,0,168,169,5,108,0,0,169,170,5,111,0,0,170,171,5,97,
  	0,0,171,172,5,116,0,0,172,173,1,0,0,0,173,174,6,11,11,0,174,24,1,0,0,
  	0,175,176,5,118,0,0,176,177,5,111,0,0,177,178,5,105,0,0,178,179,5,100,
  	0,0,179,180,1,0,0,0,180,181,6,12,12,0,181,26,1,0,0,0,182,183,5,40,0,0,
  	183,184,6,13,13,0,184,28,1,0,0,0,185,186,5,41,0,0,186,187,6,14,14,0,187,
  	30,1,0,0,0,188,189,5,123,0,0,189,190,6,15,15,0,190,32,1,0,0,0,191,192,
  	5,125,0,0,192,193,6,16,16,0,193,34,1,0,0,0,194,195,5,91,0,0,195,196,6,
  	17,17,0,196,36,1,0,0,0,197,198,5,93,0,0,198,199,6,18,18,0,199,38,1,0,
  	0,0,200,201,5,59,0,0,201,202,6,19,19,0,202,40,1,0,0,0,203,204,5,44,0,
  	0,204,205,6,20,20,0,205,42,1,0,0,0,206,207,7,3,0,0,207,208,6,21,21,0,
  	208,44,1,0,0,0,209,210,7,4,0,0,210,211,6,22,22,0,211,46,1,0,0,0,212,213,
  	5,43,0,0,213,214,5,43,0,0,214,215,1,0,0,0,215,216,6,23,23,0,216,48,1,
  	0,0,0,217,218,5,45,0,0,218,219,5,45,0,0,219,220,1,0,0,0,220,221,6,24,
  	24,0,221,50,1,0,0,0,222,223,5,33,0,0,223,224,6,25,25,0,224,52,1,0,0,0,
  	225,226,5,60,0,0,226,237,5,61,0,0,227,228,5,61,0,0,228,237,5,61,0,0,229,
  	230,5,62,0,0,230,237,5,61,0,0,231,237,7,5,0,0,232,233,5,33,0,0,233,234,
  	5,61,0,0,234,235,1,0,0,0,235,237,6,26,26,0,236,225,1,0,0,0,236,227,1,
  	0,0,0,236,229,1,0,0,0,236,231,1,0,0,0,236,232,1,0,0,0,237,54,1,0,0,0,
  	238,239,5,38,0,0,239,245,5,38,0,0,240,241,5,124,0,0,241,242,5,124,0,0,
  	242,243,1,0,0,0,243,245,6,27,27,0,244,238,1,0,0,0,244,240,1,0,0,0,245,
  	56,1,0,0,0,246,247,5,61,0,0,247,248,6,28,28,0,248,58,1,0,0,0,249,253,
  	7,6,0,0,250,252,7,7,0,0,251,250,1,0,0,0,252,255,1,0,0,0,253,251,1,0,0,
  	0,253,254,1,0,0,0,254,256,1,0,0,0,255,253,1,0,0,0,256,257,6,29,29,0,257,
  	60,1,0,0,0,258,260,7,8,0,0,259,258,1,0,0,0,260,261,1,0,0,0,261,259,1,
  	0,0,0,261,262,1,0,0,0,262,263,1,0,0,0,263,264,6,30,30,0,264,62,1,0,0,
  	0,265,267,7,8,0,0,266,265,1,0,0,0,267,268,1,0,0,0,268,266,1,0,0,0,268,
  	269,1,0,0,0,269,277,1,0,0,0,270,274,5,46,0,0,271,273,7,8,0,0,272,271,
  	1,0,0,0,273,276,1,0,0,0,274,272,1,0,0,0,274,275,1,0,0,0,275,278,1,0,0,
  	0,276,274,1,0,0,0,277,270,1,0,0,0,277,278,1,0,0,0,278,288,1,0,0,0,279,
  	281,7,9,0,0,280,282,7,3,0,0,281,280,1,0,0,0,281,282,1,0,0,0,282,284,1,
  	0,0,0,283,285,7,8,0,0,284,283,1,0,0,0,285,286,1,0,0,0,286,284,1,0,0,0,
  	286,287,1,0,0,0,287,289,1,0,0,0,288,279,1,0,0,0,288,289,1,0,0,0,289,290,
  	1,0,0,0,290,306,6,31,31,0,291,293,5,46,0,0,292,294,7,8,0,0,293,292,1,
  	0,0,0,294,295,1,0,0,0,295,293,1,0,0,0,295,296,1,0,0,0,296,297,1,0,0,0,
  	297,306,6,31,32,0,298,300,7,8,0,0,299,298,1,0,0,0,300,301,1,0,0,0,301,
  	299,1,0,0,0,301,302,1,0,0,0,302,303,1,0,0,0,303,304,5,46,0,0,304,306,
  	6,31,33,0,305,266,1,0,0,0,305,291,1,0,0,0,305,299,1,0,0,0,306,64,1,0,
  	0,0,20,0,71,83,85,99,101,112,236,244,253,261,268,274,277,281,286,288,
  	295,301,305,34,1,0,0,6,0,0,1,1,1,1,2,2,1,4,3,1,5,4,1,6,5,1,7,6,1,8,7,
  	1,9,8,1,10,9,1,11,10,1,12,11,1,13,12,1,14,13,1,15,14,1,16,15,1,17,16,
  	1,18,17,1,19,18,1,20,19,1,21,20,1,22,21,1,23,22,1,24,23,1,25,24,1,26,
  	25,1,27,26,1,28,27,1,29,28,1,30,29,1,31,30,1,31,31,1,31,32
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  csubsetlexerLexerStaticData = std::move(staticData);
}

}

CSubsetLexer::CSubsetLexer(CharStream *input) : Lexer(input) {
  CSubsetLexer::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *csubsetlexerLexerStaticData->atn, csubsetlexerLexerStaticData->decisionToDFA, csubsetlexerLexerStaticData->sharedContextCache);
}

CSubsetLexer::~CSubsetLexer() {
  delete _interpreter;
}

std::string CSubsetLexer::getGrammarFileName() const {
  return "CSubset.g4";
}

const std::vector<std::string>& CSubsetLexer::getRuleNames() const {
  return csubsetlexerLexerStaticData->ruleNames;
}

const std::vector<std::string>& CSubsetLexer::getChannelNames() const {
  return csubsetlexerLexerStaticData->channelNames;
}

const std::vector<std::string>& CSubsetLexer::getModeNames() const {
  return csubsetlexerLexerStaticData->modeNames;
}

const dfa::Vocabulary& CSubsetLexer::getVocabulary() const {
  return csubsetlexerLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView CSubsetLexer::getSerializedATN() const {
  return csubsetlexerLexerStaticData->serializedATN;
}

const atn::ATN& CSubsetLexer::getATN() const {
  return *csubsetlexerLexerStaticData->atn;
}


void CSubsetLexer::action(RuleContext *context, size_t ruleIndex, size_t actionIndex) {
  switch (ruleIndex) {
    case 0: LINE_COMMENTAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 1: BLOCK_COMMENTAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 2: STRINGAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 4: IFAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 5: ELSEAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 6: FORAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 7: WHILEAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 8: PRINTLNAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 9: RETURNAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 10: INTAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 11: FLOATAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 12: VOIDAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 13: LPARENAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 14: RPARENAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 15: LCURLAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 16: RCURLAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 17: LTHIRDAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 18: RTHIRDAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 19: SEMICOLONAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 20: COMMAAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 21: ADDOPAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 22: MULOPAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 23: INCOPAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 24: DECOPAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 25: NOTAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 26: RELOPAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 27: LOGICOPAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 28: ASSIGNOPAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 29: IDAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 30: CONST_INTAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;
    case 31: CONST_FLOATAction(antlrcpp::downCast<antlr4::RuleContext *>(context), actionIndex); break;

  default:
    break;
  }
}

void CSubsetLexer::LINE_COMMENTAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 0: 
            writeIntoLexLogFile(
              "Line# " + std::to_string(getLine())
              + ": Token <SINGLE LINE COMMENT> Lexeme "
              + getText()
            );
         break;

  default:
    break;
  }
}

void CSubsetLexer::BLOCK_COMMENTAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 1: 
          // extra braces create a new scope for your variables
          {
            std::string txt = getText();
            std::string content = txt.substr(2, txt.size() - 4);
            writeIntoLexLogFile(
              "Line# " + std::to_string(getLine())
              + ": Token <MULTI LINE COMMENT> Lexeme /*"
              + content + "*/"
            );
          }
         break;

  default:
    break;
  }
}

void CSubsetLexer::STRINGAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 2: 
            writeIntoLexLogFile(
              "Line# " + std::to_string(getLine())
              + ": Token <STRING> Lexeme " + getText()
            );
         break;

  default:
    break;
  }
}

void CSubsetLexer::IFAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 3:  writeIntoLexLogFile("Line# " + std::to_string(getLine()) + ": Token <IF> Lexeme " + getText());  break;

  default:
    break;
  }
}

void CSubsetLexer::ELSEAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 4:  writeIntoLexLogFile("Line# " + std::to_string(getLine()) + ": Token <ELSE> Lexeme " + getText());  break;

  default:
    break;
  }
}

void CSubsetLexer::FORAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 5:  writeIntoLexLogFile("Line# " + std::to_string(getLine()) + ": Token <FOR> Lexeme " + getText());  break;

  default:
    break;
  }
}

void CSubsetLexer::WHILEAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 6:  writeIntoLexLogFile("Line# " + std::to_string(getLine()) + ": Token <WHILE> Lexeme " + getText());  break;

  default:
    break;
  }
}

void CSubsetLexer::PRINTLNAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 7:  writeIntoLexLogFile("Line# " + std::to_string(getLine()) + ": Token <PRINTLN> Lexeme " + getText());  break;

  default:
    break;
  }
}

void CSubsetLexer::RETURNAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 8:  writeIntoLexLogFile("Line# " + std::to_string(getLine()) + ": Token <RETURN> Lexeme " + getText());  break;

  default:
    break;
  }
}

void CSubsetLexer::INTAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 9:  writeIntoLexLogFile("Line# " + std::to_string(getLine()) + ": Token <INT> Lexeme " + getText());  break;

  default:
    break;
  }
}

void CSubsetLexer::FLOATAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 10:  writeIntoLexLogFile("Line# " + std::to_string(getLine()) + ": Token <FLOAT> Lexeme " + getText());  break;

  default:
    break;
  }
}

void CSubsetLexer::VOIDAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 11:  writeIntoLexLogFile("Line# " + std::to_string(getLine()) + ": Token <VOID> Lexeme " + getText());  break;

  default:
    break;
  }
}

void CSubsetLexer::LPARENAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 12:  writeIntoLexLogFile("Line# " + std::to_string(getLine()) + ": Token <LPAREN> Lexeme " + getText());  break;

  default:
    break;
  }
}

void CSubsetLexer::RPARENAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 13:  writeIntoLexLogFile("Line# " + std::to_string(getLine()) + ": Token <RPAREN> Lexeme " + getText());  break;

  default:
    break;
  }
}

void CSubsetLexer::LCURLAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 14:  writeIntoLexLogFile("Line# " + std::to_string(getLine()) + ": Token <LCURL> Lexeme " + getText());  break;

  default:
    break;
  }
}

void CSubsetLexer::RCURLAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 15:  writeIntoLexLogFile("Line# " + std::to_string(getLine()) + ": Token <RCURL> Lexeme " + getText());  break;

  default:
    break;
  }
}

void CSubsetLexer::LTHIRDAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 16:  writeIntoLexLogFile("Line# " + std::to_string(getLine()) + ": Token <LTHIRD> Lexeme " + getText());  break;

  default:
    break;
  }
}

void CSubsetLexer::RTHIRDAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 17:  writeIntoLexLogFile("Line# " + std::to_string(getLine()) + ": Token <RTHIRD> Lexeme " + getText());  break;

  default:
    break;
  }
}

void CSubsetLexer::SEMICOLONAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 18:  writeIntoLexLogFile("Line# " + std::to_string(getLine()) + ": Token <SEMICOLON> Lexeme " + getText());  break;

  default:
    break;
  }
}

void CSubsetLexer::COMMAAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 19:  writeIntoLexLogFile("Line# " + std::to_string(getLine()) + ": Token <COMMA> Lexeme " + getText());  break;

  default:
    break;
  }
}

void CSubsetLexer::ADDOPAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 20:  writeIntoLexLogFile("Line# " + std::to_string(getLine()) + ": Token <ADDOP> Lexeme " + getText());  break;

  default:
    break;
  }
}

void CSubsetLexer::MULOPAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 21:  writeIntoLexLogFile("Line# " + std::to_string(getLine()) + ": Token <MULOP> Lexeme " + getText());  break;

  default:
    break;
  }
}

void CSubsetLexer::INCOPAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 22:  writeIntoLexLogFile("Line# " + std::to_string(getLine()) + ": Token <INCOP> Lexeme " + getText());  break;

  default:
    break;
  }
}

void CSubsetLexer::DECOPAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 23:  writeIntoLexLogFile("Line# " + std::to_string(getLine()) + ": Token <DECOP> Lexeme " + getText());  break;

  default:
    break;
  }
}

void CSubsetLexer::NOTAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 24:  writeIntoLexLogFile("Line# " + std::to_string(getLine()) + ": Token <NOT> Lexeme " + getText());  break;

  default:
    break;
  }
}

void CSubsetLexer::RELOPAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 25:  writeIntoLexLogFile("Line# " + std::to_string(getLine()) + ": Token <RELOP> Lexeme " + getText());  break;

  default:
    break;
  }
}

void CSubsetLexer::LOGICOPAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 26:  writeIntoLexLogFile("Line# " + std::to_string(getLine()) + ": Token <LOGICOP> Lexeme " + getText());  break;

  default:
    break;
  }
}

void CSubsetLexer::ASSIGNOPAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 27:  writeIntoLexLogFile("Line# " + std::to_string(getLine()) + ": Token <ASSIGNOP> Lexeme " + getText());  break;

  default:
    break;
  }
}

void CSubsetLexer::IDAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 28:  writeIntoLexLogFile("Line# " + std::to_string(getLine()) + ": Token <ID> Lexeme " + getText());  break;

  default:
    break;
  }
}

void CSubsetLexer::CONST_INTAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 29:  writeIntoLexLogFile("Line# " + std::to_string(getLine()) + ": Token <CONST_INT> Lexeme " + getText());  break;

  default:
    break;
  }
}

void CSubsetLexer::CONST_FLOATAction(antlr4::RuleContext *context, size_t actionIndex) {
  switch (actionIndex) {
    case 30: 
            writeIntoLexLogFile(
              "Line# " + std::to_string(getLine())
              + ": Token <CONST_FLOAT> Lexeme " + getText()
            );
         break;
    case 31: 
            writeIntoLexLogFile(
              "Line# " + std::to_string(getLine())
              + ": Token <CONST_FLOAT> Lexeme " + getText()
            );
         break;
    case 32: 
            writeIntoLexLogFile(
              "Line# " + std::to_string(getLine())
              + ": Token <CONST_FLOAT> Lexeme " + getText()
            );
         break;

  default:
    break;
  }
}



void CSubsetLexer::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  csubsetlexerLexerInitialize();
#else
  ::antlr4::internal::call_once(csubsetlexerLexerOnceFlag, csubsetlexerLexerInitialize);
#endif
}
