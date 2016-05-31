#ifndef DETERMINISTICFINITEAUTOMATA_H_
#define DETERMINISTICFINITEAUTOMATA_H_
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <stack>
#include "DFASet.h"
#include "NFA.h"

using namespace std;

class DFA {
private:
	const int NULL_STATE = 1;
	
	vector<vector<int> > transitions;
	map<int, int> finalStates;
	int startingState;
	
	const int ALPHABET_SIZE = 256;
	
public:
	DFA();
	DFA(const DFA& other);
	DFA(const NFA& nfa);
	virtual ~DFA();
	
	const DFA& operator=(const DFA& other);
	void minimizeStates();
	
	int delta(int state, int symbol) const;
	int getStartingState() const;
	int nStates() const;
	bool isFinalState(int state) const;
	int getFinalStateId(int state) const;
	bool isNullState(int state) const;
	void print();
};

#endif

