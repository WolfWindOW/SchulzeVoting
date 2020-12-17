//Class Definitions, function prototypes for SchulzeVoting
//Noah Wyborny
//11/3/2020

#ifndef VOTING_HPP
#define VOTING_HPP

#include <vector>
#include <string>
#include <map>

//Constant declarations
const int INF = INT_MAX; //Infinity, just to see if anything has gone wrong
const int candidateNumber = 3;

//Class Defs

class Candidate {
public:
	Candidate(); //Constructor, assigns name as "*******", exID as '*'
	void setName(std::vector<std::string>&, int); //Mutator, sets the name for a candidate
	void setExID(std::vector<char>&, int); //Sets the External ID for a candidate
	std::string getName() const; //Accessor, returns the name of a Candidate
	char getExID() const; //Accessor, returns the exID of a Candidate
	bool compareIDTo(Candidate&); //Compares two candidates' IDs
private:
	std::string name_; //First and last name of the candidate
	char externalID_; //For voting purposes, used as a substitute for name
};

class Election {
public:
	Election();
	std::vector<std::string>  candidateNames(); //Gets the names of the candidates
	std::vector<char>  candidateIDs(); //Gets the IDs of the candidates
	char oddOneOut(char, char); //Input two candidate IDs, returns the last available ID
	std::string allTheRest(char); //Input one candidate ID, returns the rest of the available IDs
	void displayWinner(std::string); //Displays all potential winners
	void addVote(std::string); //Adds a vote to the file
	void deploysCandidates(std::vector<std::string>&, std::vector<char>&); //Adds candidates to array. Takes the vector of candidate names in first arg, vector of candidate IDs in the second arg
private:
	Candidate cand_[candidateNumber]; //Candidates of the election
	std::string winner_; //Displays which canidate wins
};

class Matrix { //Acts as the graph
public:
	Matrix(std::vector<char>);
	Matrix(const Matrix&); //Copy Constructor
	int victoryCount(std::string); //Returns the amount of wins for a specific pairwise preference
	int findFirstVote(std::vector<std::string>); //Finds if the first valid vote exists, and outputs the first element's index
	void countVotes(Election); //Counts the individual votes
	void increaseValue(char, char); //Increments the value of a pairwise preference
	void warshallAlgo(Matrix); //Calculates strongest path
	std::string outputWinner(); //Runs through winners_ and if true, adds to string. Outputs the string
private:
	std::map<const std::string, int> pairwisePreference_; //A series of these will make up the matrix
	std::map<const char, bool> winners_;

};
#endif