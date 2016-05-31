#include "DFASet.h"


DFASet::DFASet() {
	
}

DFASet::DFASet(int val) {
	S.insert(val);
}

DFASet::DFASet(const set<int>& S) {
	this->S = S;
}

void DFASet::insert(int val) {
	S.insert(val);
}

void DFASet::erase(int val) {
	S.erase(val);
}

void DFASet::insert(const set<int>& A) {
	for(set<int>::iterator i = A.begin(); i != A.end(); i++) {
		S.insert(*i);
	}
}

set<int>::iterator DFASet::begin() const {
	return S.begin();
}

set<int>::iterator DFASet::end() const {
	return S.end();
}


bool DFASet::operator<(const DFASet& rhs) const {
	set<int>::iterator i = S.begin();
	set<int>::iterator j = rhs.S.begin();
	while(i != S.end() && j != rhs.S.end()) {
		if(*i < *j)
			return true;
		else if(*i > *j)
			return false;
		else {
			i++;
			j++;
		}
	}
	return (j != rhs.S.end());
}

bool DFASet::operator>(const DFASet& rhs) const {
	return (rhs < *this);
}

bool DFASet::operator<=(const DFASet& rhs) const {
	return !(*this > rhs);
}

bool DFASet::operator>=(const DFASet& rhs) const {
	return!(*this < rhs);
}

bool DFASet::operator==(const DFASet& rhs) const {
	return (*this >= rhs && *this <= rhs);
}

bool DFASet::operator!=(const DFASet& rhs) const {
	return !(*this == rhs);
}
