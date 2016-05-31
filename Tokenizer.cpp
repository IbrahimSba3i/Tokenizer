#include "Tokenizer.h"

Tokenizer::Tokenizer(const DFA& dfa) {
	this->dfa = dfa;
	lineNumbersCounter = 1;
	characterPositionCounter = 0;
	prepared = false;
	finished = false;
}

Tokenizer::~Tokenizer() {
	
}

void Tokenizer::setInputStream(const string& stream) {
	this->stream.setBuffer(stream);
	this->stream.reset();
	prepared = false;
}

bool Tokenizer::hasNext() {
	checkPrepared();
	return !finished;
}

void Tokenizer::prepareToken() {
	
	while(!finished) {
		string lexeme = "";
		stack<int> statesStack;
		int q = dfa.getStartingState();
		
		int characterPos = characterPositionCounter;
		int lineNumber = lineNumbersCounter;
		
		bool foundFinal = dfa.isFinalState(q);
		
		statesStack.push(q);
		while(!stream.isFinished() && !dfa.isNullState(q) && !hopeless.count(make_pair(q, stream.getCursor()))) {
			char x = stream.get();
			q = dfa.delta(q, x);
			if(dfa.isFinalState(q)) {
				foundFinal = true;
				statesStack = stack<int>();
			}
			statesStack.push(q);
			lexeme += x;
		}
		
		if(foundFinal) {
			q = statesStack.top();
			statesStack.pop();
			while(!dfa.isFinalState(q)) {
				hopeless.insert(make_pair(q, stream.getCursor()));
				stream.unget();
				lexeme.erase(lexeme.size() - 1);
				q = statesStack.top();
				statesStack.pop();				
			}
			
			int id = dfa.getFinalStateId(q);
			
			for(int i = 0; i < lexeme.length(); i++) {
				if(lexeme[i] == '\n') {
					lineNumbersCounter++;
					characterPositionCounter = 0;
				} else {
					characterPositionCounter++;
				}
			}
			
			token = Token(id, lexeme, lineNumber, characterPos);
			return;
		} else {
			if(stream.isFinished() && lexeme.length() == 0) {
				finished = true;
			} else {
				while(lexeme.size() != 0) {
					if(!statesStack.empty()) {
						q = statesStack.top();
						statesStack.pop();
						hopeless.insert(make_pair(q, stream.getCursor()));
					}
					stream.unget();
					lexeme.erase(lexeme.size() - 1);
				}
				char c = stream.get();
				if(c == '\n') {
					lineNumbersCounter++;
					characterPositionCounter = 0;
				} else {
					characterPositionCounter++;
				}
			}
		}
	}
	
	finished = true;
}

void Tokenizer::checkPrepared() {
	if(!prepared) {
		prepareToken();
		prepared = true;
	}
}

Token Tokenizer::nextToken() {
	checkPrepared();
	Token result = token;
	prepareToken();
	return result;
}


