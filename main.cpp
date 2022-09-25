#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include<stdlib.h>  

// This file contains the code to solve the towers of hanoi puzzle.
// It will use states in order to solve the path to the solution.
// This program will only solve the solution using the second peg method.

//--------------------------------------------------
// YOUR NAME: Juan Solis Melgar
//--------------------------------------------------
using namespace std;

// State description class.
// Disk string will be made up S,M,L for SML
// S = small disk
// M = medium disk
// L = large disk
// e.g initial state will equal [SML] [] []
class state
{
public:
	string peg[3]; // Made up of S,M,L for storing SML
	string camefrom[3]; // Will be use to store the solution path
	int g; // cost so far
	int h; // estimated cost to the goal
	int f; // g+h
};

// data structures:
vector<state> frontier;  // Frontier nodes
vector<state> beenThere; // already expanded nodes


// to remove state f from the frontier
void removeFrontier(state f)
{
	vector<state> newfrontier;
	for (int k = 0; k < frontier.size(); k++)
		if (frontier[k].peg[0] != f.peg[0] && frontier[k].peg[1] != f.peg[1] && frontier[k].peg[2] != f.peg[2])
			newfrontier.push_back(frontier[k]);

	frontier = newfrontier;
}


//======= For Generating a new state  =============

// Trace and display the solution path from goal to initial.
// Using camefrom we will determine the solution path
void tracePath(state goal)
{
	//  display the path from goal to initial
	state path = goal;
	for (int k = beenThere.size() - 1; k >= 0; k--)
	{
		if (beenThere[k].peg[0] == path.camefrom[0] && beenThere[k].peg[1] == path.camefrom[1] && beenThere[k].peg[2] == path.camefrom[2])
		{
			cout << "Came from\n" << "[" << beenThere[k].peg[0] << "] [" << beenThere[k].peg[1] << "] ["<<beenThere[k].peg[2] << "]" << endl;
			path = beenThere[k];
		}
	}

}// end of tracePath 

// This function will check if the next frontier is better
bool inFrontier(state next)
{
	// loop through all current frontiers in the vector beenThere
	for (int k = 0; k < frontier.size(); k++)
		if (beenThere[k].peg[0] != next.peg[0] && beenThere[k].peg[1] != next.peg[1] && beenThere[k].peg[2] != next.peg[2])
		{
			cout << "Arealdy in frontier" << endl;
			if (frontier[k].f > next.f) // Found a better frontier
			{
				cout << "Updating frontier..." << endl;
				frontier[k] = next;
				return true;
			}
			return false; // There was no better frontier
		}
	cout << "Not in frontier yet" << endl;
	return false;

}// end of inFrontier

// Function that will check if the next frontier is a safe state
bool frontierIsSafe(state next)
{
	string unsafeStates[] = { "MS","LS","LM","LMS","LSM", "MSL"}; // unsafe states
	for (int k = 0; k < 6; k++)
		if (unsafeStates[k] == next.peg[0] || unsafeStates[k] == next.peg[1] || unsafeStates[k] == next.peg[2])
			return false; // unsafe state
	return true; // next state is safe
} // end of frontierIsSafe

// Function that will check if the next frontier has already been expanded
bool beenThereBefore(state next)
{
	for (int k = 0; k < beenThere.size(); k++)
	{
		if (beenThere[k].peg[0] == next.peg[0] && beenThere[k].peg[1] == next.peg[1] && beenThere[k].peg[2] == next.peg[2])
			return true; // not a new frontier
	}
	return false; // new frontier 
} // end of beenThereBefore

// Find the best f state out of the frontier and return it 
state bestofFrontier()
{
	state bestFrontier = frontier[0]; // will start with the first frontier as the best option
	for (int k = 1; k < frontier.size(); k++)
	{
		if (frontier[k].f < bestFrontier.f)
		{
			bestFrontier = frontier[k];
		}
	}
	return bestFrontier;
}//end of bestofFrontier

// Try to add next to frontier but stop if SML has been
// reached in the second peg 
void checkState(state next)
{
	cout << "Trying to add: " << "["<< next.peg[0] << "] [" << next.peg[1] << "] [" << next.peg[2] << "]";

	if (next.peg[1] == "SML")  // the goal is reached
	{
		cout << ">>Reached: " << "["<< next.peg[0] << "] [" << next.peg[1] << "] [" << next.peg[2] << "]" << endl;
		tracePath(next);  // display the solution path
		exit(0);
	}//done

// if been there before, do not add to frontier.
	if (beenThereBefore(next))
	{
		cout << " been there already" << endl;
		return;
	}
	// if unsafe state, do not add to frontier.
	if (!frontierIsSafe(next))
	{
		cout << " unsafe!" << endl;
		return;
	}
	// else compute h and then f for next
	int h = 0;// start point for h;
	//  update h and then f
	h = pow(2, 3) - 1 - next.g;
	next.h = h;
	next.f = next.g + h;	// new value of f
	cout << " safe!" << endl;
	if (!inFrontier(next))
		frontier.push_back(next); // add next to Frontier


}// end of checkState 

// Generate all possible states including legal/illegal states.
// The function will loop to the next possible state.
// This will repeat until a solution path is reached.
void generateAllStates(state current)
{
	state next; // generate a new state
	current.g = current.g + 1; // New move means update g by 1
	// To keep track of path
	current.camefrom[0] = current.peg[0];
	current.camefrom[1] = current.peg[1];
	current.camefrom[2] = current.peg[2];
	// All possible states

	// move S into second peg
	next = current; // starting point of next
	if (current.peg[0] == "SML" && current.peg[1] =="" && current.peg[2] == "")	// Starting point
	{
		next.peg[0] = "ML";
		next.peg[1] = "S";
		next.peg[2] = "";
		checkState(next);
	}

	// Move M into second peg
	next = current; // starting point of next
	if (current.peg[0] == "ML" && current.peg[1] == "S" && current.peg[2] == "")	// next point
	{
		next.peg[0] = "L";
		next.peg[1] = "MS";
		next.peg[2] = "";
		checkState(next);
	}
	// Move L into third peg
	next = current; // starting point of next
	if (current.peg[0] == "" && current.peg[1] == "L" && current.peg[2] == "SM")	// next point
	{
		next.peg[0] = "";
		next.peg[1] = "";
		next.peg[2] = "LSM";
		checkState(next);
	}

	// Move M into second peg
	next = current; // starting point of next
	if (current.peg[0] == "S" && current.peg[1] == "L" && current.peg[2] == "M")	// next point
	{
		next.peg[0] = "S";
		next.peg[1] = "ML";
		next.peg[2] = "";
		checkState(next);
	}

	// Move M into first peg
	next = current; // starting point of next
	if (current.peg[0] == "S" && current.peg[1] == "L" && current.peg[2] == "M")	// next point
	{
		next.peg[0] = "MS";
		next.peg[1] = "L";
		next.peg[2] = "";
		checkState(next);
	}

	// Move M into third peg
	next = current; // starting point of next
	if (current.peg[0] == "ML" && current.peg[1] == "S" && current.peg[2] == "")	// next point
	{
		next.peg[0] = "L";
		next.peg[1] = "S";
		next.peg[2] = "M";
		checkState(next);
	}
	
	// Move S into third peg
	next = current; // starting point of next
	if (current.peg[0] == "L" && current.peg[1] == "S" && current.peg[2] == "M")	// next point
	{
		next.peg[0] = "L";
		next.peg[1] = "";
		next.peg[2] = "SM";
		checkState(next);
	}

	// Move M into first peg
	next = current; // starting point of next
	if (current.peg[0] == "SL" && current.peg[1] == "" && current.peg[2] == "M")	// next point
	{
		next.peg[0] = "MSL";
		next.peg[1] = "";
		next.peg[2] = "";
		checkState(next);
	}

	// Move L into second peg
	next = current; // starting point of next
	if (current.peg[0] == "L" && current.peg[1] == "" && current.peg[2] == "SM")	// next point
	{
		next.peg[0] = "";
		next.peg[1] = "L";
		next.peg[2] = "SM";
		checkState(next);
	}

	// Move L into third peg
	next = current; // starting point of next
	if (current.peg[0] == "L" && current.peg[1] == "" && current.peg[2] == "SM")	// next point
	{
		next.peg[0] = "";
		next.peg[1] = "";
		next.peg[2] = "LSM";
		checkState(next);
	}

	// Move S into first peg
	next = current; // starting point of next
	if (current.peg[0] == "" && current.peg[1] == "L" && current.peg[2] == "SM")	// next point
	{
		next.peg[0] = "S";
		next.peg[1] = "L";
		next.peg[2] = "M";
		checkState(next);
	}

	// Move S into second peg
	next = current; // starting point of next
	if (current.peg[0] == "S" && current.peg[1] == "ML" && current.peg[2] == "")	// next point
	{
		next.peg[0] = "";
		next.peg[1] = "SML";
		next.peg[2] = "";
		checkState(next);
	}
} // end of generateAllStates

// Display the states in the frontier in a nice format
void displayFrontier()
{
	for (int k = 0; k < frontier.size(); k++)
	{
		cout << "[" << frontier[k].peg[0] << "] [" << frontier[k].peg[1] << "] [" << frontier[k].peg[2] << "]" << endl;
		cout << "g = " << frontier[k].g << " ";
		cout << "h = " << frontier[k].h << " ";
		cout << "f = " << frontier[k].f << endl;
	}
} // end of displayFrontier
int main()
{
	// initialize starting values
	state  current;
	current.peg[0] = "SML"; current.peg[1] = ""; current.peg[2] = "";
	current.camefrom[0] = "SML"; current.camefrom[1] = ""; current.camefrom[2] = "";
	current.f = 7;
	current.g = 0;
	current.h = 7;
	frontier.push_back(current); // first frontier

	char ans = ' ';
	while (ans != 'n')
	{
		removeFrontier(current); // Check if a better frontier exists
		beenThere.push_back(current); // update beenthere before with new state

		cout << ">>>Expand:" << "[" << current.peg[0] << "] [" << current.peg[1] << "] [" << current.peg[2] << "]"<< endl;
		generateAllStates(current);  // new states are added to frontier

		cout << "Frontier is: " << endl;
		displayFrontier(); //Display current frontier

		current = bestofFrontier(); // update current with best frontier state
		cout << "Best is: "  << "[" << current.peg[0] << "] [" << current.peg[1] << "] [" << current.peg[2] << "]" << endl;
		cin >> ans;

	}// end while

	return 0;
} // end of main