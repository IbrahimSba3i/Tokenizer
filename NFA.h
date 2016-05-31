#ifndef NFA_H_
#define NFA_H_
#include <string>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include "RegexError.h"

using namespace std;

class NFA {
private:
	const int ALPHABET_SIZE = 257;
	const int EPSILON = ALPHABET_SIZE - 1;

	vector<vector<set<int> > > transitions;
	map<int, int> finalStates;
	int startingState;
	
	map<int, int> insertionOrder;
	
	pair<int, int> parseExpression(const string& reg, int& index);
	pair<int, int> parseConcatenation(const string& reg, int& index);
	pair<int, int> parseQuantifiedElement(const string& reg, int& index);
	pair<int, int> parseElement(const string& reg, int& index);
	pair<int, int> parseSet(const string& reg, int& index);	
	pair<int, int> parseGroup(const string& reg, int& index);
	pair<int, int> parseAny(const string& reg, int& index);
	pair<int, int> parseChar(const string& reg, int& index);
	
	vector<int> parseSetItems(const string& reg, int& index);	
	vector<int> parseSetItem(const string& reg, int& index);
	vector<int> getPrebuildSet(char c);
	
	char getEquivalentCharacter(char c);
	
	void epsilonClosure(set<int>& closure, int state);
	set<int> deltabar(const set<int>& states, int symbol);
	set<int> epsilonClosureUnion(const set<int>& S, const vector<set<int> >& epsClosures);
public:
	NFA();
	NFA(const NFA& other);
	virtual ~NFA();
	
	const NFA& operator=(const NFA& other);
	
	void insertRegex(const string& reg, int id);
	void removeEpsilonTransitions();
	
	const set<int>& delta(int state, int symbol);
	int nStates() const;

	const map<int, int>& getFinalStates() const;
	const map<int, int>& getInsertionOrder() const;
	int getStartingState() const;
	const vector<vector<set<int> > >& getTransitions() const;
	void print();
};

#endif
