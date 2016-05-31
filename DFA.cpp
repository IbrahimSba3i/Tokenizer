#include "DFA.h"
#include <iostream>

DFA::DFA() {
	this->startingState = 0;
}

DFA::DFA(const DFA& other) {
	this->startingState = other.startingState;
	this->transitions = other.transitions;
	this->finalStates = other.finalStates;
}

DFA::DFA(const NFA& nfa) {
	map<DFASet, int> statesMap;
	startingState = 0;
	
	transitions.push_back(vector<int>(ALPHABET_SIZE, NULL_STATE));
	transitions.push_back(vector<int>(ALPHABET_SIZE, NULL_STATE));
	
	statesMap[DFASet(nfa.getStartingState())] = 0;
	statesMap[DFASet()] = NULL_STATE;
	
	const vector<vector<set<int> > >& nfaTransitions = nfa.getTransitions();
	const map<int, int>& nfaFinalStates = nfa.getFinalStates();
	const map<int, int>& nfaInsertionOrder = nfa.getInsertionOrder();
	int nfaStartingState = nfa.getStartingState();
	
	queue<DFASet> Q;
	Q.push(DFASet(nfaStartingState));
	while(!Q.empty()) {
		DFASet a = Q.front();
		int dfastate = statesMap[a];
		Q.pop();
		for(int symbol = 0; symbol < ALPHABET_SIZE; symbol++) {
			DFASet result;
			for(set<int>::iterator j = a.begin(); j != a.end(); j++) {
				result.insert(nfaTransitions[*j][symbol]);
			}
			
			int resultdfastate = NULL_STATE;
			if(statesMap.count(result)) {
				resultdfastate = statesMap[result];
			} else {
				resultdfastate = statesMap.size();
				statesMap[result] = resultdfastate;
				transitions.push_back(vector<int>(ALPHABET_SIZE, NULL_STATE));
				Q.push(result);
			}
			
			transitions[dfastate][symbol] = resultdfastate;
		}
	}

	const int oo = 0x7fffffff;
	for(map<DFASet, int>::iterator i = statesMap.begin(); i != statesMap.end(); i++) {
		int bestId = -1;
		int bestIdOrder = oo;
		for(set<int>::iterator j = i->first.begin(); j != i->first.end(); j++) {
			if(nfaFinalStates.count(*j)) {
				int id = nfaFinalStates.at(*j);
				int idOrder = nfaInsertionOrder.at(id);
				if(idOrder < bestIdOrder) {
					bestIdOrder = idOrder;
					bestId = id;
				}
			}
		}
		if(bestIdOrder != oo) {
			finalStates[i->second] = bestId;
		}
	}
}

DFA::~DFA() {
	
}

const DFA& DFA::operator =(const DFA& other) {
	if(this != &other) {
		this->startingState = other.startingState;
		this->transitions = other.transitions;
		this->finalStates = other.finalStates;
	}
	return *this;
}

void DFA::print() {
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
	cout << "\n\n";
	for(int i = 0; i < nStates(); i++) {
		for(int j = 0; j < ALPHABET_SIZE; j++) {
			cout << transitions[i][j];
		}
		cout << endl;
	}
}

void DFA::minimizeStates() {
	//print();
}

int DFA::delta(int state, int symbol) const {
	return transitions[state][symbol];
}

int DFA::getStartingState() const {
	return startingState;
}

bool DFA::isFinalState(int state) const {
	return finalStates.count(state);
}

int DFA::getFinalStateId(int state) const {
	if(finalStates.count(state)) {
		return finalStates.at(state);
	} else {
		return -1;
	}
}

int DFA::nStates() const {
	return transitions.size();
}

bool DFA::isNullState(int state) const {
	return (state == NULL_STATE);
}




