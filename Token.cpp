#include "Token.h"

Token::Token() {
	this->tokenType = -1;
	this->lexeme = "";
	this->lineNumber = 0;
	this->charPosition = 0;
}

Token::Token(int tokenType) {
	this->tokenType = tokenType;
	this->lexeme = "";
	this->lineNumber = 0;
	this->charPosition = 0;
}

Token::Token(int tokenType, const string& lexeme) {
	this->tokenType = tokenType;
	this->lexeme = lexeme;
	this->lineNumber = 0;
	this->charPosition = 0;
}

Token::Token(int tokenType, const string& lexeme, int lineNumber, int charPosition) {
	this->tokenType = tokenType;
	this->lexeme = lexeme;
	this->lineNumber = lineNumber;
	this->charPosition = charPosition;
}

Token::~Token() {
	
}

int Token::getCharPosition() const {
	return charPosition;
}

void Token::setCharPosition(int charPosition) {
	this->charPosition = charPosition;
}

string Token::getLexeme() const {
	return lexeme;
}

void Token::setLexeme(string lexeme) {
	this->lexeme = lexeme;
}

int Token::getLineNumber() const {
	return lineNumber;
}

void Token::setLineNumber(int lineNumber) {
	this->lineNumber = lineNumber;
}

int Token::getTokenType() const {
	return tokenType;
}

void Token::setTokenType(int tokenType) {
	this->tokenType = tokenType;
}
