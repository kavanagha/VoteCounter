//Student Name: Ailish Kavanagh
//Student  Number: C00206130

#include "VoteCount.h"
using namespace std;


inline bool operator<(const Candidate& left, const Candidate& right)
{
	return std::tie(left.name, left.party) < std::tie(right.name, right.party);
	//need to make name and string public for Candidate object so the STL map can sort the
	//objects as keys. Maps are ordered by key.
}

int main()
{
	//create a new vote counter object and ebgin the election process
	VoteCounter example = VoteCounter();
	example.runElection();

	system("pause");
	return 0;
}