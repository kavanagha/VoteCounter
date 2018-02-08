//Student Name: Ailish Kavanagh
//Student  Number: C00206130

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
using namespace std;

class Candidate
{
public:
	Candidate()
	{
		name = "";
		party = "";
	}
	Candidate(string n, string p)
	{
		setName(n);
		setParty(p);
	}
	void setName(string n) { name = n; }
	void setParty(string p) { party = p; }
	string getName() { return name; }
	string getParty() { return party; }
	void toString()
	{
		//output Candidate name and Candidate party details
		cout << "Candidate: " << name << endl;
		cout << "Party: " << party << endl;
	}
	bool identical(Candidate c)
	{
		//check to ee if one Candidate is equal to another
		if (name == c.getName())
		{
			if (party == c.getParty())
				return true;
		}
		return false;
	}
	string name; //should be private...
	string party;	//need to be public for setting up maps, see VoteCount.cpp
};
class BallotPaper
{
	map<int, Candidate> votes;
public:
	BallotPaper(string line, vector<Candidate>can)
	{
		//string line is a string of number - voter preferences
		//candidate order in vector can == candidate order on ballot paper
		//integer or preference at string[index] == can[index] preference
		for (int index = 0; index < line.length(); index++)
		{
			Candidate c = can[index];
			int n = (int)line[index] - 48; //ASCII value to integer value
			setPreference(n, c); //set integer n preference at index to candidate c at index
		}

	}
	void setPreference(int n, Candidate c)
	{
		votes[n] = c; //assign preference to candidate in BallotPaper votes map
	}
	Candidate getPreference(int n)
	{
		//return Candidate which has preference n
		Candidate c = votes[n];
		return c;
	}
};
class VoteCounter
{
	vector<Candidate>candidates, losers;
	vector<BallotPaper>ballots;
	const string delimiter = ", "; //for splitting string
	int countCand = 0, countBallots = 0, loserCount = 0;
	map <Candidate, int> count; //vote count

public:
	VoteCounter()
	{
		startVoteCounter(); //begin intialise vote counting process
	}
	void startVoteCounter()
	{
		vector<string>voteList;
		vector<string>candList;
		string input;

		//begin process inputted text file
		//must be named "input.txt"
		ifstream inputList("input.txt");

		if (inputList.is_open())
		{
			while (getline(inputList, input))
			{
				try
				{
					//check if line is numerical
					//i.e can it be converted to a number
					int ballot = stoi(input);
					//if numerical: add to voteList, increment count
					voteList.push_back(input);
					countBallots++;

				}
				catch (invalid_argument e)
				{
					//if line is not numerical it will throw an
					//invalid_argument exception
					//This line is taken as candidate data
					//push whole line to candList, increment count
					candList.push_back(input);
					countCand++;
				}
			}
		}

		//process candList strings into Candidate objects
		//and voteList strings into BallotPaper objects
		setCandidates(candList);
		setBallots(voteList);
	}
	void setCandidates(vector<string> candList)
	{
		for (int index = 0; index < countCand; index++)
		{
			//delimiter = seperator = ", " - set in global variables
			//Before ", " is Candidate name, after ", " is candidate party
			string name = candList[index].substr(0, candList[index].find(delimiter));
			string party = candList[index].substr(candList[index].find(delimiter) + 2, candList[index].length());

			Candidate x = Candidate(name, party);//create candidate  object
			candidates.push_back(x); //push to candidates vector
		}
	}
	void setBallots(vector<string> voteList)
	{
		for (int index = 0; index < countBallots; index++)
		{
			//create BallotPaper object for each string in voteList
			//push to ballots vector
			BallotPaper b = BallotPaper(voteList[index], candidates);
			ballots.push_back(b);
		}
	}
	void initialiseResultMap()
	{
		//initialise map
		//set value to 0, unless eliminated candidate (set to -1)
		for (int index = 0; index < countCand; index++)
		{
			Candidate c = candidates[index];
			bool loserFlag = false;
			for (int l = 0; l < loserCount; l++)
			{
				if (losers[l].identical(c) == true)
					loserFlag = true;
			}
			if (loserFlag == false)
				count[c] = 0;
			else
				count[c] = -1;
		}
	}
	void runElection()
	{
		//begin election
		//for n candidates, run loop for n-1 rounds
		for (int index = 0; index < countCand - 1; index++)
		{
			int round = index + 1;
			cout << "Round #" << round << "\n" << endl;

			countRound();	//count votes and output results
			Candidate c = eliminate();	//eliminate losing candidate
			redistributeVotes(c, round);	//redistribute loser votes
			cout << endl;
		}

		Candidate c = winner(); //find winning candidate and output
		cout << "The winning candidate is: " << endl;
		c.toString();
		cout << endl;
	}
	void countRound()
	{
		initialiseResultMap();
		//for each ballot paper,get preference 1
		//increment that candidates vote count
		for (int index = 0; index < countBallots; index++)
		{
			Candidate c = ballots[index].getPreference(1);
			int x = count[c];
			count[c] = x + 1;
		}
		outputRoundResults(); //output vote count
	}
	void outputRoundResults()
	{
		//output the total votes for each candidate that has not been previously eliminated
		for (int index = 0; index < countCand; index++)
		{
			Candidate c = candidates[index];
			int x = count[c];
			if (x>-1)
				cout << "Candidate: " << c.getName() << ", Votes: " << x << endl;
		}
	}
	Candidate eliminate()
	{
		Candidate c = winner(), loser;
		int min = count[c]; //max votes for this round
		for (int index = 0; index < countCand; index++)
		{
			//find candidate with min votes - set to loser
			//dont include candidates with -1 votes (already eliminated)
			c = candidates[index];
			int x = count[c];
			if (x < min && x > -1)
			{
				min = x;
				loser = c;
			}
		}
		
		losers.push_back(loser); //add losing candidate to losers vector
		loserCount++;	//increment loser count
		//output and return eliminated candidate
		cout << "\nCandidate " << loser.getName() << " has been eliminated." << endl;
		return loser;
	}
	Candidate winner()
	{
		//find Candidate with Max/highest number of votes
		Candidate win;
		int max = 0;
		for (int index = 0; index < countCand; index++)
		{
			Candidate c = candidates[index];
			int x = count[c];

			if (x > max)
			{
				max = x;
				win = c;
			}
		}
		return win; //return winning candidate
	}
	void redistributeVotes(Candidate loser, int round)
	{	
		for (int row = 0; row < countBallots; row++)
		{
			for (int index = 0; index < countCand; index++)
			{
				//for each row in ballots
				//check if the Candidate at prefrence index is equal to the eliminated candidate
				Candidate c = ballots[row].getPreference(index);
				if (loser.identical(c)) //if eliminated candidate
				{
					for (int j = index + 1; j <= countCand; j++)
					{
						//for every Candidate with a preference greater than losers preference
						//decrement that Candidates preference
						Candidate c = ballots[row].getPreference(j);
						ballots[row].setPreference(j - 1, c);
					}
				}
				//set losers preference to 0 for each row after redistribution 
				ballots[row].setPreference(0, loser);
			}
		}
	}
};