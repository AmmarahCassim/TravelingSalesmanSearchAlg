#ifndef Node_h
#define Node_h

#include <deque>

using namespace std;

class Node
{
	public:
		//int id;
		deque<int> visitedIDs;
		double heuristic;
	
};

#endif