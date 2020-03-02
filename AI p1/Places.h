#ifndef PLACES_H
#define PLACES_H

#include <fstream>
#include <deque>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <cmath>

using namespace std;

struct Place
{
	int id;
	double xcoordinate;
	double ycoordinate;
};

class Places
{
	public:
		Places(string filename)
		{
			numPlaces = 0;
			if (!readInPlaces(filename))
			{
				exit(-1);
			}
		};

		~Places()
		{
			for (size_t i = 0; i < places.size(); ++i)
			{
				delete places[i];
			}
		};

		bool readInPlaces(string filename)
		{
			ifstream infile;

			infile.open(filename.c_str());
			Place* temp;

			if(!infile.good())
			{
				cout << "Error opening file\n";
				return false;
			}

			bool start = false;
			string line;

			while(!infile.eof() && start == false)
			{
				infile >> line;
				//cout << line << endl;

				if(line == "DIMENSION")
				{
					infile >> line;
					infile >> numPlaces;
					//cout << numPlaces << endl;
				}

				if(line == "NODE_COORD_SECTION")
				{
					start = true;
				}
			}

			if(start == false)
			{
				cout << "unable to read coordinates\n";
				return false;
			}

			while(!infile.eof())
			{
				temp = new Place();
				infile >> line;
				//cout << line << endl;
				if (line == "EOF")
				{
					break;
				}
				else if(line == "\n")
				{
					continue;
				}

				temp->id = atoi(line.c_str());
				//cout << temp->id << endl;

				infile >> line;
				//cout << line << endl;
				if (line == "EOF")
				{
					break;
				}
				else if(line == "\n")
				{
					continue;
				}

				temp->xcoordinate = strtod(line.c_str(), NULL);
				//cout << temp->xcoordinate << endl;

				infile >> line;
				//cout << line << endl;
				if (line == "EOF")
				{
					break;
				}
				else if(line == "\n")
				{
					continue;
				}

				temp->ycoordinate = strtod(line.c_str(), NULL);
				//cout << temp->ycoordinate << endl;

				places.push_back(temp);
			}

			infile.close();

			return true;
		};

		int getNumPlaces()
		{
			return numPlaces;
		};

		double euclideanDistance(Place* x, Place* y)
		{
			if( x!= NULL && y != NULL)
			{
				return sqrt(pow((x->xcoordinate - y->xcoordinate), 2) + pow((y->ycoordinate - x->ycoordinate), 2));
			}
			else
			{
				return 0;
			}
		};

		Place* findPlaceByID(int id)
		{
			for (size_t i = 0; i < places.size(); ++i)
			{
				if(places[i]->id == id)
				{
					return places[i];
				}
			}
			return NULL;
		};

		int numPlaces;
		deque<Place*> places;

};

#endif