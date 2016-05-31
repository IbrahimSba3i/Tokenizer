#include "NFA.h"
#include <iostream>

NFA::NFA() {
	transitions.push_back(vector<set<int> >(ALPHABET_SIZE));
	startingState = 0;
}

NFA::~NFA() {
	
}


NFA::NFA(const NFA& other) {
	this->transitions = other.transitions;
	this->finalStates = other.finalStates;
	this->startingState = other.startingState;
}

const NFA& NFA::operator =(const NFA& other) {
	if(this != &other) {
		this->transitions = other.transitions;
		this->finalStates = other.finalStates;
		this->startingState = other.startingState;		
	}
	return *this;
}

void NFA::insertRegex(const string& reg, int id) {
	if(insertionOrder.count(id)) {
		stringstream ss;
		ss << "Token id: " << id << " already inserted";
		throw invalid_argument(ss.str());
	}
	
	insertionOrder[id] = insertionOrder.size() + 1;
	
	int index = 0;
	pair<int, int> a = parseExpression(reg, index);
	if(index != reg.size()) {
		throw RegexError("couldn't complete parsing");
	}
	
	transitions[startingState][EPSILON].insert(a.first);
	finalStates.insert(make_pair(a.second, id));
}



void NFA::epsilonClosure(set<int>& closure, int state) {
	if(!closure.count(state)) {
		closure.insert(state);
		const set<int>& s = delta(state, EPSILON);
		for(set<int>::iterator i = s.begin(); i != s.end(); i++) {
			epsilonClosure(closure, *i);
		}
	}
}

set<int> NFA::epsilonClosureUnion(const set<int>& S, const vector<set<int> >& epsClosures) {
	set<int> result;
	for(set<int>::iterator i = S.begin(); i != S.end(); i++) {
		for(set<int>::iterator j = epsClosures[*i].begin(); j != epsClosures[*i].end(); j++) {
			result.insert(*j);
		}
	}
	return result;
}

void NFA::print() {
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	cout << "states: " << nStates() << endl;
	cout << "finals: " << finalStates.size() << endl;
	cout << "start:  " << startingState << endl;
	for (int i = 0; i < nStates(); i++) {
		cout << "    " << i;
		if (finalStates.count(i))
			cout << "\tF\n";
		else
			cout << "\tN\n";
	}
}

void NFA::removeEpsilonTransitions() {
	vector<set<int> > epsClosures(nStates());
	
	for(int i = 0; i < epsClosures.size(); i++) {
		epsilonClosure(epsClosures[i], i);
	}
	
	vector<vector<set<int> > > deltaPrime(nStates(), vector<set<int> >(ALPHABET_SIZE));
	for(int i = 0; i < nStates(); i++) {
		for(int j = 0; j < ALPHABET_SIZE; j++) {
			if(j != EPSILON) {
				deltaPrime[i][j] = epsilonClosureUnion(deltabar(epsClosures[i], j), epsClosures);
			}
		}
	}
	transitions = deltaPrime;

	map<int, int> finalStatesPrime;
	const int oo = 0x7fffffff;
	for(int i = 0; i < epsClosures.size(); i++) {
		int bestId = -1;
		int bestIdOrder = oo;
		for(set<int>::iterator j = epsClosures[i].begin(); j != epsClosures[i].end(); j++) {
			int state = *j;
			if(finalStates.count(state)) {
				int id = finalStates[state];
				int idOrder = insertionOrder[id];
				if(idOrder < bestIdOrder) {
					bestIdOrder = idOrder;
					bestId = id;
				}
			}
		}
		if(bestIdOrder != oo) {
			finalStatesPrime[i] = bestId;
		}
	}
	finalStates = finalStatesPrime;
	//print();
}

const set<int>& NFA::delta(int state, int symbol) {
	return transitions[state][symbol];
}

set<int> NFA::deltabar(const set<int>& states, int symbol) {
	set<int> result;
	for(set<int>::iterator i = states.begin(); i != states.end(); i++) {
		const set<int>& x = delta(*i, symbol);
		for(set<int>::iterator j = x.begin(); j != x.end(); j++) {
			result.insert(*j);
		}
	}
	return result;
}

int NFA::nStates() const {
	return transitions.size();
}

const map<int, int>& NFA::getFinalStates() const {
	return finalStates;
}

const map<int, int>& NFA::getInsertionOrder() const {
	return insertionOrder;
}

int NFA::getStartingState() const {
	return startingState;
}

const vector<vector<set<int> > >& NFA::getTransitions() const {
	return transitions;
}



//	<exp>	-> <conc> { "|" <conc> }
pair<int, int> NFA::parseExpression(const string& reg, int& index) {
	vector<pair<int, int> > concatenations;
	pair<int, int> a = parseConcatenation(reg, index);
	concatenations.push_back(a);
	while(index < reg.size() && reg[index] == '|') {
		index++;
		a = parseConcatenation(reg, index);
		concatenations.push_back(a);
	}
	
	int startIndex = nStates();
	transitions.push_back(vector<set<int> >(ALPHABET_SIZE));
	int endIndex = nStates();
	transitions.push_back(vector<set<int> >(ALPHABET_SIZE));
	for(int i = 0; i < concatenations.size(); i++) {
		transitions[startIndex][EPSILON].insert(concatenations[i].first);
		transitions[concatenations[i].second][EPSILON].insert(endIndex);
	}
	return make_pair(startIndex, endIndex);
}

//	<conc>	-> <qelm> { <qelm> }
pair<int, int> NFA::parseConcatenation(const string& reg, int& index) {
	vector<pair<int, int> > elements;
	pair<int, int> a = parseQuantifiedElement(reg, index);
	elements.push_back(a);
	while(index < reg.size() && reg[index] != '|') {
		a = parseQuantifiedElement(reg, index);
		elements.push_back(a);
	}
	for(int i = 0; i < (int)elements.size() - 1; i++) {
		transitions[elements[i].second][EPSILON].insert(elements[i + 1].first);
	}
	return make_pair(elements[0].first, elements[elements.size() - 1].second);
}

//	<qelm>	-> <elem> | <elem> "*" | <elem> "+" | <elem> "?"
pair<int, int> NFA::parseQuantifiedElement(const string& reg, int& index) {
	pair<int, int> a = parseElement(reg, index);
	if(index < reg.size()) {
		if(reg[index] == '?') {
			transitions[a.first][EPSILON].insert(a.second);
			index++;
		} else if(reg[index] == '*') {
			int startIndex = nStates();
			transitions.push_back(vector<set<int> >(ALPHABET_SIZE));
			transitions[startIndex][EPSILON].insert(a.first);
			transitions[startIndex][EPSILON].insert(a.second);
			transitions[a.second][EPSILON].insert(a.first);
			a = make_pair(startIndex, a.second);
			index++;
		} else if(reg[index] == '+') {
			transitions[a.second][EPSILON].insert(a.first);
			index++;
		}
	}
	return a;
}

//	<elem>	-> <group> | <any> | <char> | <set>
pair<int, int> NFA::parseElement(const string& reg, int& index) {
	if(index < reg.size()) {
		
		if(reg[index] == '(') {
			return parseGroup(reg, index);
		} else if(reg[index] == '.') {
			return parseAny(reg, index);
		} else if(reg[index] == '[') {
			return parseSet(reg, index);
		} else {
			return parseChar(reg, index);
		}			
	} else {
		return make_pair(-1, -1);
	}
}

//	<set>	-> "[" <setitems> "]" | "[^" <setitems> "]"
pair<int, int> NFA::parseSet(const string& reg, int& index) {
	if(reg[index] != '[') throw RegexError(string("Expected: '[' found: \'") + reg[index] + "\'");
	index++;
	bool negative = false;
	if(index < reg.size() && reg[index] == '^') {
		index++;
		negative = true;
	}
	vector<int> a = parseSetItems(reg, index);
	int startIndex = nStates();
	transitions.push_back(vector<set<int> >(ALPHABET_SIZE));
	int endIndex = nStates();
	transitions.push_back(vector<set<int> >(ALPHABET_SIZE));
	
	if(negative) {
		for(int i = 0; i < ALPHABET_SIZE; i++) {
			transitions[startIndex][i].insert(endIndex);
		}
		transitions[startIndex][EPSILON].erase(endIndex);
		for(int i = 0; i < a.size(); i++) {
			transitions[startIndex][a[i]].erase(endIndex);
		}
	} else {
		for(int i = 0; i < a.size(); i++) {
			transitions[startIndex][a[i]].insert(endIndex);
		}
	}
	
	if(index >= reg.size()) throw RegexError("Unexpected end of regex encountered");
	if(reg[index] != ']') throw RegexError(string("Expected: ']' found: \'") + reg[index] + "\'");
	else index++;
	return make_pair(startIndex, endIndex);
}

//	<group>	-> "(" <exp> ")"
pair<int, int> NFA::parseGroup(const string& reg, int& index) {
	if(reg[index] != '(') throw RegexError(string("Expected: '(' found: \'") + reg[index] + "\'");
	index++;
	pair<int, int> a = parseExpression(reg, index);
	if(index >= reg.size()) throw RegexError("Unexpected end of regex encountered");
	if(reg[index] != ')') throw RegexError(string("Expected: ')' found: \'") + reg[index] + "\'");
	else index++;
	return a;
}

//	<any>	-> "."
pair<int, int> NFA::parseAny(const string& reg, int& index) {
	if(reg[index] != '.') throw RegexError(string("Expected: '.' found: \'") + reg[index] + "\'");
	index++;
	int startIndex = nStates();
	transitions.push_back(vector<set<int> >(ALPHABET_SIZE));
	int endIndex = nStates();
	transitions.push_back(vector<set<int> >(ALPHABET_SIZE));
	
	for(int i = 0; i < ALPHABET_SIZE; i++) {
		transitions[startIndex][i].insert(endIndex);
	}
	transitions[startIndex][EPSILON].erase(endIndex);
	return make_pair(startIndex, endIndex);
}

//	<setitems>	-> <setitem> { <setitem> }
vector<int> NFA::parseSetItems(const string& reg, int& index) {
	vector<int> values;
	vector<int> a = parseSetItem(reg, index);
	values.insert(values.end(), a.begin(), a.end());
	while(index < reg.size() && reg[index] != ']') {
		a = parseSetItem(reg, index);
		values.insert(values.end(), a.begin(), a.end());
	}
	return values;
}

//	<setitem>	-> <char> | "\" <char>
vector<int> NFA::parseSetItem(const string& reg, int& index) {
	vector<int> A;
	if(index >= reg.size()) throw RegexError("Unexpected end of regex encountered");
	char x = reg[index++];
	if(x == '\\') {
		if(index >= reg.size()) throw RegexError("Unexpected end of regex encountered");
		x = reg[index++];
		if(tolower(x) == 'd' || tolower(x) == 'w' || tolower(x) == 's') {
			return getPrebuildSet(x);
		}
		x = getEquivalentCharacter(x);
	}
	
	if(index < reg.size() && reg[index] == '-') {
		index++;
		if(index < reg.size()) {
			if(reg[index] == ']') {
				index--;
				A.push_back(x);
			} else {
				char y = reg[index++];
				if(y == '\\') {
					if(index >= reg.size()) throw RegexError("Unexpected end of regex encountered");
					y = reg[index++];
					if(tolower(y) == 'd' || tolower(y) == 'w' || tolower(y) == 's') {
						throw RegexError(string("Invalid range: ") + x + "-\\" + y);
					}
					y = getEquivalentCharacter(y);
				}
				if(y < x) {
					throw RegexError(string("Text range out of order: ") + x + "-" + y);
				} else {
					for(int i = x; i <= y; i++) {
						A.push_back(i);
					}
				}
			}
		} else {
			throw RegexError("Unexpected end of regex encountered");
		}
	} else {
		A.push_back(x);
	}
	return A;
}

char NFA::getEquivalentCharacter(char c) {
	map<char, char> getEquivalent;
	getEquivalent['n'] = '\n';
	getEquivalent['r'] = '\r';
	getEquivalent['t'] = '\t';
	getEquivalent['a'] = '\a';
	getEquivalent['e'] = '\e';
	getEquivalent['f'] = '\f';
	getEquivalent['v'] = '\v';
	getEquivalent['0'] = '\0';
	
	if (getEquivalent.count(c))
		return getEquivalent[c];
	else return c;
}

vector<int> NFA::getPrebuildSet(char c) {
	set<int> S;
	vector<int> A;
	switch(c) {
	case 'd':
		for(int i = '0'; i <= '9'; i++)
			S.insert(i);
		break;
	case 'D':
		for(int i = 0; i < ALPHABET_SIZE; i++)
			S.insert(i);
		for(int i = '0'; i <= '9'; i++)
			S.erase(i);
		S.erase(EPSILON);
		break;
	case 'w':
		for(int i = '0'; i <= '9'; i++)
			S.insert(i);
		for(int i = 'A'; i <= 'Z'; i++)
			S.insert(i);
		for(int i = 'a'; i <= 'z'; i++)
			S.insert(i);
		S.insert('_');
		break;
	case 'W':
		for(int i = 0; i < ALPHABET_SIZE; i++)
			S.insert(i);
		for(int i = '0'; i <= '9'; i++)
			S.erase(i);
		for(int i = 'A'; i <= 'Z'; i++)
			S.erase(i);
		for(int i = 'a'; i <= 'z'; i++)
			S.erase(i);
		S.erase('_');
		S.erase(EPSILON);
		break;
	case 's':
		S.insert('\r');
		S.insert('\n');
		S.insert('\t');
		S.insert('\f');
		S.insert(' ');
		break;
	case 'S':
		for(int i = 0; i < ALPHABET_SIZE; i++)
			S.insert(i);
		S.erase('\r');
		S.erase('\n');
		S.erase('\t');
		S.erase('\f');
		S.erase(' ');			
		S.erase(EPSILON);
		break;
	}
	
	for(set<int>::iterator i = S.begin(); i != S.end(); i++)
		A.push_back(*i);
	return A;
}

pair<int, int> NFA::parseChar(const string& reg, int& index) {
	if(index >= reg.size()) throw RegexError("Unexpected end of regex encountered");
	int startIndex = nStates();
	transitions.push_back(vector<set<int> >(ALPHABET_SIZE));
	int endIndex = nStates();
	transitions.push_back(vector<set<int> >(ALPHABET_SIZE));
	
	char x = reg[index++];
	if(x == '+' || x == '*' || x == '?') {
		throw RegexError(string("Unexpected: ") + x);
	}
	if(x == '\\') {
		if(index >= reg.size()) throw RegexError("Unexpected end of regex encountered");
		x = reg[index++];
		if(tolower(x) == 'd' || tolower(x) == 'w' || tolower(x) == 's') {
			vector<int> symbols = getPrebuildSet(x);
			for(int i = 0; i < symbols.size(); i++) {
				transitions[startIndex][symbols[i]].insert(endIndex);
			}
		}
		x = getEquivalentCharacter(x);
	}
	transitions[startIndex][x].insert(endIndex);
	return make_pair(startIndex, endIndex);
}
