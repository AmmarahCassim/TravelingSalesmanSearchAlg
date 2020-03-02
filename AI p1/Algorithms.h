#ifndef TSPALGORITHMS_H
#define TSPALGORITHMS_H

#include <iostream>
#include <string>
#include <deque>
#include <fstream>
#include <time.h>
#include "Node.h"
#include "Places.h"

using namespace std;

class TSPAlgorithms
{
	private:
		Node* start;
		int numPlaces;
		Places* places;
		string outfilename;

	public:
		TSPAlgorithms(string _infilename, string _outfilename)
		{
			outfilename = _outfilename;
			places = new Places(_infilename);
			start = new Node();
			start->visitedIDs.push_back(places->places.front()->id);
			cout << start->visitedIDs.front() << endl;
		}

		~TSPAlgorithms()
		{
			delete places;
		}

		void setUpNode(Node* n, int newPlace)
		{
			n->visitedIDs.push_back(newPlace);
		};

		deque<Node*> generateChildrenB(Node* n)
		{
			cout << "generating children\n";

			deque<Node*> holder;

			Node* temp;

			for (size_t i = 0; i < places->places.size(); ++i)
			{
				if(contains(n->visitedIDs, places->places[i]->id))
				{
					continue;
				}
				else
				{
					temp = new Node();
					temp->visitedIDs = n->visitedIDs;
					setUpNode(temp, places->places[i]->id);
					holder.push_back(temp);
				}
			}

			return holder;
		};

		bool contains(deque<int> n, int id)
		{
			for (size_t i = 0; i < n.size(); ++i)
			{
				if(n[i] == id)
				{
					return true;
				}
			}
			return false;
		};

		bool areDone(Node* n)
		{
			cout << "places visited in Node so far are: ";
			for (size_t i = 0; i < n->visitedIDs.size(); ++i)
			{
				cout << n->visitedIDs[i] << " , ";
			}
			cout << endl;

			return (n->visitedIDs.size() == places->places.size());
		};

		double breadthfirstsearch()
		{
			time_t t1 = time(NULL);
			cout << "performing breadthfirstsearch\n";
			deque<Node*> closed;
			deque<Node*> open;
			Node* x = start;
			Node* shortest = new Node();
			double retVal = 0;
			int numNodes = 0;
			
			open.push_back(start);

			while(!open.empty())
			{
				x = open.front();
				open.pop_front();
				open.shrink_to_fit();

				if(areDone(x))
				{
					double v = getDistanceTravelled(x) + getLastDistance(x);
					
					cout << "Found distance: " << v << endl;

					if(retVal == 0 || retVal > v)
					{
						retVal = v;
						shortest->visitedIDs = x->visitedIDs;
					}
					//break;
				}
				else
				{
					//only allow 30 nodes to sit on closed
					//checkfull(closed);
					if(closed.size() > 29)
					{
						//cout << "queue is full\n";
						Node* temp;
						//int i = 0;
						while(!closed.empty())
						{
							//cout << i << endl;
							//i++;
							temp = closed.front();
							delete temp;
							temp = NULL;
							closed.pop_front();
							//delete temp;
						}
						//q.clear();
						//cout << closed.size() << endl;
					}
					closed.push_back(x);
					numNodes++;
					//Don't need to check for repeats because using tree structure
					deque<Node*> holder = generateChildrenB(x);
					while(!holder.empty())
					{
						open.push_back(holder.front());
						holder.pop_front();
					}
				}
			}

			time_t t2 = time(NULL);

			double stime = difftime(t2, t1);
			
			writeResultsToFile(retVal, shortest, outfilename, numNodes);
			writeTimeToFile(stime, outfilename);

			Node* temp2;
			while(!closed.empty())
			{
				temp2 = closed.front();
				delete temp2;
				temp2 = NULL;
				closed.pop_front();
			}

			delete shortest;

			return retVal;
			
		};

		double depthfirstsearch(Node* startstate, int depth)
		{
			cout << "performing depthfirstsearch\n";
			deque<Node*> closed;
			deque<Node*> open;
			bool success = false;
			Node* x = startstate;
			Node* shortest = new Node();
			int visitedDepth = 0;
			double retVal = 0;
			int numNodes = 0;
			
			open.push_front(startstate);

			while(!open.empty() && success == false)
			{
				x = open.front();
				open.pop_front();
				open.shrink_to_fit();

				if(areDone(x))
				{
					success = true;
					double v = getDistanceTravelled(x) + getLastDistance(x);
					
					cout << "Found distance: " << v << endl;

					if(retVal == 0 || retVal > v)
					{
						retVal = v;
						shortest->visitedIDs = x->visitedIDs;
					}
					//break;
				}
				else if(visitedDepth > depth)
				{
					success = false;
					break;
				}
				else
				{
					//only allow 30 nodes to sit on closed
					if(closed.size() > 29)
					{
						Node* temp;
						//int i = 0;
						while(!closed.empty())
						{

							temp = closed.front();
							delete temp;
							temp = NULL;
							closed.pop_front();
						}
					}
					closed.push_back(x);
					numNodes++;
					//Don't need to check for repeats because using tree structure
					deque<Node*> holder = generateChildrenB(x);
					while(!holder.empty())
					{
						open.push_front(holder.front());
						holder.pop_front();
					}
				}
				visitedDepth++;
			}

			Node* temp2;
			while(!closed.empty())
			{
				temp2 = closed.front();
				delete temp2;
				temp2 = NULL;
				closed.pop_front();
			}

			while(!open.empty())
			{
				temp2 = open.front();
				delete temp2;
				temp2 = NULL;
				if(open.empty()){break;}
				open.pop_front();
			}

			if(success == true)
			{
				writeResultsToFile(retVal, shortest, outfilename, numNodes);
				delete shortest;
				return retVal;
			}
			else
			{
				delete shortest;
				return -1;
			}
		};

		double depthfirstwithiterativedeepening()
		{
			time_t t1;
			t1 = time(NULL);

			int depth = 1;
			double distance = -1;
			Node* startstate;

			while(distance == -1)
			{
				cout << depth << " iteration --------------------------\n";
				startstate = new Node;
				startstate->visitedIDs = start->visitedIDs;
				distance = depthfirstsearch(startstate, depth++);
			}

			time_t t2;
			t2 = time(NULL);

			double stime = difftime(t2, t1);
			writeTimeToFile(stime, outfilename);

			return distance;
		};

		double astar()
		{
			time_t t1;
			t1 = time(NULL);

			cout << "performing a star search\n";
			deque<Node*> closed;
			deque<Node*> open;
			bool success = false;
			Node* x = start;
			Node* shortest = new Node();
			double retVal = 0;
			int numNodes = 0;
			
			open.push_front(start);

			while(!open.empty() && success == false)
			{
				x = open.front();
				open.pop_front();
				open.shrink_to_fit();

				if(areDone(x))
				{
					success = true;
					double v = getDistanceTravelled(x) + getLastDistance(x);
					
					cout << "Found distance: " << v << endl;

					if(retVal == 0 || retVal > v)
					{
						retVal = v;
						shortest->visitedIDs = x->visitedIDs;
					}
					break;
				}
				else
				{
					//only allow 30 nodes to sit on closed
					if(closed.size() > 29)
					{
						Node* temp;
						while(!closed.empty())
						{
							temp = closed.front();
							delete temp;
							temp = NULL;
							closed.pop_front();
						}
					}
					closed.push_back(x);
					numNodes++;
					//Don't need to check for repeats because using tree structure
					deque<Node*> holder = generateChildrenA(x);
					while(!holder.empty())
					{
						open.push_front(holder.back());
						holder.pop_back();
					}
				}
			}

			time_t t2;
			t2 = time(NULL);

			double stime = difftime(t2, t1);
			writeResultsToFile(retVal, shortest, outfilename, numNodes);
			writeTimeToFile(stime, outfilename);

			Node* temp2;
			while(!closed.empty())
			{
				temp2 = closed.front();
				delete temp2;
				temp2 = NULL;
				closed.pop_front();
			}
			while(!open.empty())
			{
				temp2 = open.front();
				delete temp2;
				temp2 = NULL;
				open.pop_front();
			}

			delete shortest;

			return retVal;
		};

		double getDistanceTravelled(Node* n)
		{
			//cout << "entered getDistanceTravelled\n";
			double sum = 0;
			for (size_t i = 0; i < n->visitedIDs.size()-1; ++i)
			{
				sum += places->euclideanDistance(places->findPlaceByID(n->visitedIDs[i]), places->findPlaceByID(n->visitedIDs[i+1]));
			}
			return sum;
		};

		double getLastDistance(Node* n)
		{
			//cout << "entered getLastDistance\n";
			return places->euclideanDistance(places->findPlaceByID(n->visitedIDs[0]), places->findPlaceByID(n->visitedIDs[n->visitedIDs.size()-1]));
		};

		deque<Node*> generateChildrenA(Node* n)
		{
			cout << "entered generateChildrenA\n";

			deque<Node*> holder;

			Node* temp;

			for (size_t i = 0; i < places->places.size(); ++i)
			{
				if(contains(n->visitedIDs, places->places[i]->id))
				{
					continue;
				}
				else
				{
					temp = new Node();
					temp->visitedIDs = n->visitedIDs;
					setUpNode(temp, places->places[i]->id);
					temp->heuristic = getDistanceTravelled(temp);
					holder.push_back(temp);
				}
			}

			size_t m, j, min_idx;

			// One by one move boundary of unsorted subarray
		    for(m = 0; m < holder.size()-1; m++)
		    {
		        // Find the minimum element in unsorted array
		        min_idx = m;
		        for(j = m+1; j < holder.size(); j++)
		        {
		        	if(holder[j]->heuristic < holder[min_idx]->heuristic)
		        	{
		        		min_idx = j;
		        	}
		        	
		        }
		 
		        // Swap the found minimum element with the first element
		       // swap(&arr[min_idx], &arr[m]);
		        Node* temp2 = holder[min_idx];
		        holder[min_idx] = holder[m];
		        holder[m] = temp2;
		    }

		  /*  cout << "priorities of children: ";
		    for (size_t f = 0; f < holder.size(); ++f)
		    {
		    	cout << holder[f]->heuristic << " , ";
		    }
		    cout << endl;*/

			return holder;
		};

		void writeResultsToFile(double val, Node* n, string filename, int NumNodes)
		{
			ofstream outfile;
			outfile.open(filename.c_str());
			
			outfile << "Shortest distance found by algorithm: " << val << endl;
			outfile << "Number of Nodes expanded: " << NumNodes << endl;

			outfile << "Path: ";
			for (size_t i = 0; i < n->visitedIDs.size(); i++)
			{
				outfile << n->visitedIDs[i] << ", ";
			}
			outfile << n->visitedIDs[0] << endl;

			outfile.close();
		};

		void writeTimeToFile(double stime, string filename)
		{
			ofstream outfile;
			outfile.open(filename.c_str(), std::ios_base::app);

			outfile << "Number of seconds taken to search: " << stime << endl;

			outfile.close();
		};
	
};

#endif