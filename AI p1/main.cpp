#include <iostream>
#include <string>
#include "Algorithms.h"

using namespace std;

int main(int argc, char const *argv[])
{
	cout << "Traveling Salesman problem with Search\n";

	string infilename, outfilename;
	int input = 0;

	cout << "Please give the name of the TSP data file to run the search on: ";
	cin >> infilename;

	cout << "Please give the name of the TXT output file to write the results to: ";
	cin >> outfilename;

	while (input != 1 && input != 2 && input != 3)
	{
		cout << "Please input the algorithm number to select it to run: \n";
		cout << "1 Breadth first search\n";
		cout << "2 Depth first search with iterative deepening\n";
		cout << "3 A star search\n";
		cout << "Input: ";
		cin >> input;
	}

	TSPAlgorithms* al = new TSPAlgorithms(infilename, outfilename);

	double val;

	if (input == 1)
	{
		val = al->breadthfirstsearch();
	}
	else if (input == 2)
	{
		val = al->depthfirstwithiterativedeepening();
	}
	else if (input == 3)
	{
		val = al->astar();
	}
	else
	{
		cout << "An error occured, exiting program\n";
		delete al;
		return -1;
	}

	cout << "The distance returned by the algorithm is: " << val << endl;
	//cout << "Press q to exit\n";

	while (cin.get() != 'q')
	{
		cout << "Press q to exit\n";
	}
	
	delete al;
	return 0;
}