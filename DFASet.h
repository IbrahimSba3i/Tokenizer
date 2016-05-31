#ifndef DFASET_H_
#define DFASET_H_

#include <set>

using namespace std;

class DFASet {
	set<int> S;
	
public:
	DFASet();
	DFASet(int val);
	DFASet(const set<int>& S);
	
	void insert(int val);
	void insert(const set<int>& A);
	void erase(int val);
	set<int>::iterator begin() const;
	set<int>::iterator end() const;
	
	bool operator<(const DFASet& rhs) const;
	bool operator>(const DFASet& rhs) const;
	bool operator<=(const DFASet& rhs) const;
	bool operator>=(const DFASet& rhs) const;
	bool operator==(const DFASet& rhs) const;
	bool operator!=(const DFASet& rhs) const;
};


#endif
