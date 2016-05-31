#ifndef REGEXERROR_H_
#define REGEXERROR_H_

#include <stdexcept>
using namespace std;
class RegexError : public exception
{
	string errorMsg;
public:
	explicit RegexError(){}

	explicit RegexError(const string& errorMsg){
		this->errorMsg = errorMsg;
	}

	virtual ~RegexError() throw (){}

	virtual const char* what() const throw (){
		return errorMsg.c_str();
	}
};


#endif
