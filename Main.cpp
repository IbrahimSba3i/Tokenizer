#include <iostream>
#include <string>
#include <sstream>
#include "TokenizerBuilder.h"
#include "Tokenizer.h"
using namespace std;

enum TokenType {
	NUM,
	ID,
	PLUS,
	MINUS,
	MUL,
	DIV
};

int main() {
	try {
		TokenizerBuilder builder;
		builder.addTokenType(NUM, "[1-9][0-9]*");
		builder.addTokenType(ID, "[a-zA-Z_]\\w*");
		builder.addTokenType(PLUS, "\\+");
		builder.addTokenType(MINUS, "\\-");
		builder.addTokenType(MUL, "\\*");
		builder.addTokenType(DIV, "\\/");
		
		Tokenizer tokenizer = builder.build();
		tokenizer.setInputStream("val21+3 - 25900/_x-MAX_SIZE");
		
		
		while(tokenizer.hasNext()) {
			Token token = tokenizer.nextToken();
			cout << token.getTokenType() << ": \"" << token.getLexeme() << "\"\n";
		}
		
	} catch(std::exception& e) {
		cout << e.what() << endl;
	}
	return 0;
}
