//This program will use the Floyd-Warshall algorithm to determine the winner in a Schulze-method vote. 
//This is intended only for votes of three candidates
//Noah Wyborny
//10/20/2020

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "voting.hpp"

using namespace std;

//TODO




int main() {
	//Beginning election
	//Creates an election with multiple candidates
	Election mainElection;
	//Iterates over the list of candidates to get the names and number of candidates
	vector<string> candidates = mainElection.candidateNames();
	//Creates the list of candidate externalIDs
	vector<char> candidateIDs = mainElection.candidateIDs();
	//Deploys the candidates
	mainElection.deploysCandidates(candidates, candidateIDs);
	//Sets up all of the pairwise functions
	Matrix distance(candidateIDs);
	//Copies the basic outline of the matrix onto path
	Matrix path = distance;
	char moreVotes = 'Y';
	string userVote = "";
	cout << "Would someone like to vote? [Y/N]" << endl;
	cin >> moreVotes;
	if (moreVotes == 'Y') {
		//While there are still more people to vote
		do {
			//Gives instructions on how to vote
			for (auto i = 0; i < candidateNumber; i++) {
				cout << "Candidate " << candidates[i] << " is under ID: " << candidateIDs[i] << endl;
			}
			cout << "Enter the candidates you would like to win, in order of preference. Equals are allowed. You may leave off candidates." << endl;
			cout << "Use commas to divide the candidates. Do not use spaces. For example: \"A,B,C\", \"A=C\", and \"A\" are all valid." << endl;
			//Takes input from user
			cin >> userVote;
			//Puts the user input onto a separate file
			mainElection.addVote(userVote);
			//Clears the screen
			cout << string(50, '\n');
			cout << "Would more people like to vote? [Y/N]" << endl;
			cin >> moreVotes;
		} while (moreVotes == 'Y');
	}
	//Counts all the votes
	distance.countVotes(mainElection);
	//Determines strongest path/winner
	path.warshallAlgo(distance);
	//Displays winner
	mainElection.displayWinner(path.outputWinner());

	
}

Candidate::Candidate() { //Constructor, assigns name as "*******", exID at '*'
	name_ = "*******";
	externalID_ = '*';
}

void Candidate::setName(std::vector<std::string>& nameList, int i) { //Takes the last name in the list, and applies the name to a Candidate object, then gets rid of the last name
	name_ = nameList[i];
}

void Candidate::setExID(std::vector<char>& exIDList, int i) {
	externalID_ = exIDList[i];

}

std::string Candidate::getName() const {
	return name_;
}

char Candidate::getExID() const {
	return externalID_;
}

bool Candidate::compareIDTo(Candidate& cand1) { //Testing "this" keyword
	if (this->externalID_ == cand1.getExID()) {
		return true;
	}
	return false;
}



Election::Election(){
	winner_ = "";
}

vector<string> Election::candidateNames() { //Grabs the names of the candidates and stores them in a vector of strings
	ifstream candidates("candidates.txt");
	string name;
	vector<string> names;
	while (getline(candidates, name)) {
		names.push_back(name);
	}
	return names;
}

vector<char> Election::candidateIDs() { //Gets the names of the candidates
	vector<char> tempIDs;
	for (int i = 0; i < candidateNumber; i++) {
		char tempSingleID = 'A';
		tempSingleID += i;
		tempIDs.push_back(tempSingleID);
	}
	return tempIDs;
}

void Election::deploysCandidates(std::vector<std::string>& candidateList, std::vector<char>& externalIDList) { //Adds candidates to array. Takes the vector of candidate names in first arg, vector of candidate IDs in the second arg
	for (int i = 0; i < candidateNumber; i++) {
		cand_[i].setName(candidateList, i);
		cand_[i].setExID(externalIDList, i);
	}
}

char Election::oddOneOut(char candidateA, char candidateB) {
	char candidateRemaining;
	//Loops through the candidates
	for (auto i = 0; i < (candidateNumber-1); i++) {
		//Grab the candidate and puts them in a variable
		if (!(cand_[i].getExID() == (candidateA || candidateB))) {
			candidateRemaining = cand_[i].getExID();
			return candidateRemaining;
		}
	}
	throw 420;
}

string Election::allTheRest(char candidateA) {
	string candidatesRemaining;
	for (auto i = 0; i < (candidateNumber); i++) {
		if (cand_[i].getExID() != candidateA) {
			candidatesRemaining =+ cand_[i].getExID();
		}
	}
	return candidatesRemaining;
}

void Election::addVote(string userVote) {
	ofstream votes("votes.txt");
	while (userVote != "") {
		votes << userVote << endl;
		userVote.clear();
	}
}

void Election::displayWinner(string winners) {
	cout << "The winner(s) is: " << winners << endl;
}

Matrix::Matrix(vector<char> candidateIDs) {//Constructs the matrix of pairwise preferences, values set at INF
	string matrixKeyName;
	for (auto xAxis = 0; xAxis < (candidateNumber); xAxis++) {
		for (auto yAxis = 0; yAxis < (candidateNumber); yAxis++) {
			matrixKeyName.push_back(candidateIDs[xAxis]);
			matrixKeyName.push_back(candidateIDs[yAxis]);
			pairwisePreference_.emplace(matrixKeyName, INF);
			matrixKeyName = "";
		}
		winners_.emplace(candidateIDs[xAxis], true);
	}
}

Matrix::Matrix(const Matrix& copiedMatrix) {
	pairwisePreference_ = copiedMatrix.pairwisePreference_;
	winners_ = copiedMatrix.winners_;
}

int Matrix::victoryCount(string aVsB) {
	return pairwisePreference_[aVsB];
}

int Matrix::findFirstVote(vector<string> votes) {
	int position;
	if ((votes.front().find('A')) || (votes.front().find('B')) || (votes.front().find('C')) != string::npos) {
		position = ((votes.front().find('A')) || (votes.front().find('B')) || (votes.front().find('C')));
		return position;
	}
	else throw 69;
}

void Matrix::increaseValue(char candidateA, char candidateB) { //Takes two candidates and increases the value of A->B by one
	string matrixKeyName;
	matrixKeyName.push_back(candidateA);
	matrixKeyName.push_back(candidateB);
	//If the key exists on the matrix already, and hasnt been incremented, sets it equal to 1
	if (pairwisePreference_[matrixKeyName] == INF)
		pairwisePreference_[matrixKeyName] = 1;
	//Else just increments
	else
		pairwisePreference_[matrixKeyName]++;
}

void Matrix::countVotes(Election main) {//Counts the individual votes from the file 
	ifstream totalVotes("votes.txt");
	string vote;
	vector<string> votes;
	//Keeps grabbing votes while there are some
	while (getline(totalVotes, vote))
		//TODO Fix
		//Normalizes the votes for spaces
		/*while (vote.find(' ') != string::npos) {
			vote.erase(vote.find(' '));
		}*/
		votes.push_back(vote);
	//Sorts the votes
	sort(votes.begin(), votes.end());
	int numberOfVotes = votes.size();
	//Outputs all the votes
	/*
	for (auto x = 0; x < numberOfVotes; x++)
		cout << votes[x] << endl;
	*/
	//Counts the total number of votes and outputs for the user to see
	//cout << numberOfVotes << endl;
	//Breaks the votes into preferences
	int posOfVote;
	char singleCandidate;
	string otherIDs;
	//While there are still votes in the vector
	//Iterates through the vector of votes
		for (auto i : votes) {
			//While the individual vote is not empty
			while(!(votes.front().empty())){
				//While the vote is valid
				while ((findFirstVote(votes)) != string::npos) {
					posOfVote = findFirstVote(votes)-1;
					//If the first vote is A=B
					if (votes.front().size() > 2) {
						if (votes.front()[posOfVote + 1] == '=') {
						//Then checks to make sure the rest of the vote has a candidate
						//TODO fix out of string range
							if ((votes.front().find('A', 3)) || (votes.front().find('B', 3)) || (votes.front().find('C', 3))!= string::npos) {
								//If it is, then it increases the votes for the equal candidates relative to the one at the end
								increaseValue((votes.front()[posOfVote]), votes.front().back());
								increaseValue((votes.front()[posOfVote + 2]), votes.front().back());
								votes.front().clear();
								break;
							}
							//Edge case, "A=", should be treated the same as just "A"
								else if ((votes.front().find('A', 1)) || (votes.front().find('B', 1)) || (votes.front().find('C', 1)) == string::npos) {
									otherIDs = main.allTheRest(votes.front()[posOfVote]);
								int numberOfOtherIDs = otherIDs.size();
									for (int i = 0; i < (numberOfOtherIDs - 1); i++)
									increaseValue((votes.front()[posOfVote]), otherIDs[i]);
								votes.front().clear();
								break;
							}
							//If they only put "A=B"
							else {
								singleCandidate = main.oddOneOut((votes.front()[posOfVote]), votes.front()[posOfVote + 2]);
								increaseValue((votes.front()[posOfVote]), singleCandidate);
								increaseValue((votes.front()[posOfVote + 2]), singleCandidate);
								votes.front().clear();
								break;
							}
						}
					}
					//If the vote has at least two candidates
					if (votes.front().size() > 2) {
						if ((votes.front().find('A', 1)) || (votes.front().find('B', 1)) || (votes.front().find('C', 1)) != string::npos) {
							singleCandidate = main.oddOneOut((votes.front()[posOfVote]), votes.front()[posOfVote + 2]);
							increaseValue((votes.front()[posOfVote]), singleCandidate);
							increaseValue((votes.front()[posOfVote + 2]), singleCandidate);
							increaseValue((votes.front()[posOfVote]), votes.front()[posOfVote + 2]);
							votes.front().clear();
							break;
						}
					}
					//If only one candidate was voted on
					else {
						otherIDs = main.allTheRest(votes.front()[posOfVote]);
						int numberOfOtherIDs = otherIDs.size();
						for (int i = 0; i < (numberOfOtherIDs - 1); i++)
							increaseValue((votes.front()[posOfVote]), otherIDs[i]);
						votes.front().clear();
						break;
					}
					posOfVote = -1;
					otherIDs = "";
					singleCandidate = ' ';
			}
		}
	}
}

void Matrix::warshallAlgo(Matrix distance) {
	//Creating a quick map to easily transfer an integer in a for loop to a character
	map<int, char> intToID;
	for (int i = 0; i < candidateNumber; i++) {
		char IDAddition = 'A' + i;
		intToID.emplace(i, IDAddition);
	}
	//Finds compares each candidate to each other, to determine comparative wins
	//for i = 1 to C
	//Creating three blank strings to hold two characters, to use as map keys
	string mapKey = "";
	string mapKeyReverse = "";
	for (auto i = 0; i < candidateNumber; i++) {
		mapKey = "";
		mapKeyReverse = "";
		//for j : = 1 to C
		for (auto j = 0; j < candidateNumber; i++) {
			//if (i != j) then
			if (i != j) {
				mapKey = intToID[i] + intToID[j];
				mapKeyReverse = intToID[j] + intToID[i];
				//if (d[i,j]>d[j,i]) then
				if (distance.victoryCount(mapKey) > distance.victoryCount(mapKeyReverse)) {
					//p[i,j] : = d[i,j]
					pairwisePreference_[mapKey] = distance.victoryCount(mapKey);
				}
				else
					//p[i,j] : = 0
					pairwisePreference_[mapKey] = 0;
			}
		}
	}
	//Finds which paths are strongest
	//for i : = 1 to C
	for (auto i = 0; i < candidateNumber; i++) {
		//Making three blank strings to hold map keys
		mapKey = "";
		string mapKeyOther = "";
		string mapKeyOtherReverse = "";
		//for j : = 1 to C
		for (auto j = 0; j < candidateNumber; j++) {
			//if (i != j) then
			if (i != j) {
				//for k : = 1 to C
				for (auto k = 0; k < candidateNumber; k++) {
					//if (i != k) then
					if (i != k) {
						//if (j != k)  then
						if (j != k) {
							//p[j,k] : max { p[j,k]; min { p[j,i]; p[i,k] }}
							mapKey = intToID[j] + intToID[k];
							mapKeyOther = intToID[j] + intToID[i];
							mapKeyOtherReverse = intToID[i] + intToID[k];
							if (pairwisePreference_[mapKeyOther] < pairwisePreference_[mapKeyOtherReverse]) {
								if (pairwisePreference_[mapKey] < pairwisePreference_[mapKeyOther])
									pairwisePreference_[mapKey] = pairwisePreference_[mapKeyOther];
								else pairwisePreference_[mapKey] = pairwisePreference_[mapKey];
							}
							else if (pairwisePreference_[mapKeyOther] > pairwisePreference_[mapKeyOtherReverse]) {
								if (pairwisePreference_[mapKey] < pairwisePreference_[mapKeyOtherReverse])
									pairwisePreference_[mapKey] = pairwisePreference_[mapKeyOtherReverse];
								else pairwisePreference_[mapKey] = pairwisePreference_[mapKey];
							}
						}
					}
				}
			}
		}
	}
	//Determines the longest path
	//for i : = 1 to C
	for (auto i = 0; i < candidateNumber; i++) {
		//Same idea, different for loop
		mapKey = "";
		mapKeyReverse = "";
		//for j : = 1 to C
		for (auto j = 0; j < candidateNumber; j++) {
			//if (i != j) then
			if (i != j) {
				//if (p[j,i] > p[i,j]) then
				mapKey = intToID[i] + intToID[j];
				mapKeyReverse = intToID[j] + intToID[i];
				if (pairwisePreference_[mapKeyReverse] > pairwisePreference_[mapKey])
					winners_[intToID[i]] = false;
			}
		}
	}
}

string Matrix::outputWinner() {
	//Creating a quick map to easily transfer an integer in a for loop to a character
	map<int, char> intToID;
	for (auto i = 0; i < candidateNumber; i++) {
		char IDAddition = 'A' + i;
		intToID.emplace(i, IDAddition);
	}
	//Creating a string to hold all winners
	string winners = "";
	for (auto i = 0; i < candidateNumber; i++) {
		if (winners_[intToID[i]] == true)
			winners =+intToID[i] + ' ';
	}
	return winners;
}