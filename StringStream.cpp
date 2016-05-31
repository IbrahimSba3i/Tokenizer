#include "StringStream.h"

StringStream::StringStream() {
	cursor = 0;
}

StringStream::StringStream(const string& buffer) {
	cursor = 0;
	this->buffer = buffer;
}

void StringStream::reset() {
	cursor = 0;
}

void StringStream::setBuffer(const string& buffer) {
	this->buffer = buffer;
}

char StringStream::get() {
	return buffer[cursor++];
}

char StringStream::peek() {
	return buffer[cursor];
}

bool StringStream::isFinished() const {
	return (cursor == buffer.size());
}

void StringStream::unget() {
	if(cursor > 0) cursor--;
}

int StringStream::getCursor() const {
	return cursor;
}

