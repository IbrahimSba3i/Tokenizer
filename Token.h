#ifndef TOKENOBJECT_H_
#define TOKENOBJECT_H_
#include <string>

using namespace std;

class Token {
private:
	int tokenType;
	string lexeme;
	int lineNumber;
	int charPosition;
public:
	Token();
	Token(int tokenType);
	Token(int tokenType, const string& lexeme);
	Token(int tokenType, const string& lexeme, int lineNumber, int charPosition);
	virtual ~Token();

	int getCharPosition() const;
	void setCharPosition(int charPosition);	
	
	string getLexeme() const;
	void setLexeme(string lexeme);
	
	int getLineNumber() const;
	void setLineNumber(int lineNumber);

	int getTokenType() const;
	void setTokenType(int tokenType);
};

#endif
