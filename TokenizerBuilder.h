#ifndef TOKENIZERBUILDER_H_
#define TOKENIZERBUILDER_H_

#include <string>
#include "NFA.h"
using namespace std;

class Tokenizer;
class TokenizerBuilder {
private:
	NFA nfa;
	
public:
	TokenizerBuilder();
	virtual ~TokenizerBuilder();
	
	void addTokenType(int tokenIdentifier, const string& regularExpression);
	Tokenizer build();
};

#endif
