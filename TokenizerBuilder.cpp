#include "TokenizerBuilder.h"
#include "Tokenizer.h"

TokenizerBuilder::TokenizerBuilder() {
	
}

TokenizerBuilder::~TokenizerBuilder() {
	
}

void TokenizerBuilder::addTokenType(int tokenId, const string& regularExpression) {
	if(tokenId < 0) {
		throw invalid_argument("tokenId cannot be negative");
	}
	nfa.insertRegex(regularExpression, tokenId);
}

Tokenizer TokenizerBuilder::build() {
	nfa.removeEpsilonTransitions();
	DFA dfa(nfa);
	dfa.minimizeStates();
	return Tokenizer(dfa);
}


