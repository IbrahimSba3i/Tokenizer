#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include "Token.h"
#include "TokenizerBuilder.h"
#include "DFA.h"
#include <istream>
#include <iostream>
#include "StringStream.h"

class Tokenizer {
	DFA dfa;
	
	StringStream stream;
	
	bool prepared;
	bool finished;
	Token token;
	int lineNumbersCounter;
	int characterPositionCounter;
	
	set<pair<int,int> > hopeless;
	
	void prepareToken();
	void checkPrepared();
	
	Tokenizer(const DFA& dfa);
	friend class TokenizerBuilder;

public:
	virtual ~Tokenizer();
	
	void setInputStream(const string& s);
	
	bool hasNext();
	Token nextToken();
};

#endif
