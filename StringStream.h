#ifndef STRINGSTREAM_H_
#define STRINGSTREAM_H_

#include <string>

using namespace std;

class StringStream {
private:
	int cursor;
	string buffer;
public:
	StringStream();
	StringStream(const string& buffer);
	
	void reset();
	void setBuffer(const string& buffer);
	
	char get();
	char peek();
	bool isFinished() const;
	void unget();
	
	int getCursor() const;
};

#endif /* STRINGSTREAM_H_ */
