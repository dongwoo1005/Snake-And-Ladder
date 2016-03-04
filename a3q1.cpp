/*
CS341 Assignment 3 Problem 1
By Dongwoo Son (20420487)
d3son@uwaterloo.ca
*/

#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <string>
#include <sstream>
#include <algorithm>
#include <limits.h>

#define NINF INT_MIN

using namespace std;

class Square{

	int s_id;
	list<Square*> next6Squares;
	Square* ladderDest;
	Square* snakeDest;

public:
	Square(int s_id);
	void addNextSq(Square* sq);
	void addLadderDest(Square* sq);
	void addSnakeDest(Square* sq);

	int getSid();
	list<Square*> const& getNext6Squares();
	Square* getLadderDest();
	Square* getSnakeDest();
};

Square::Square(int s_id){
	this->s_id = s_id;
	this->ladderDest = NULL;
	this->snakeDest = NULL;
}

void Square::addNextSq(Square* sq){
	this->next6Squares.push_back(sq);
}

void Square::addLadderDest(Square* sq){
	this->ladderDest = sq;
}

void Square::addSnakeDest(Square* sq){
	this->snakeDest = sq;
}

int Square::getSid(){
	return s_id;
}

list<Square*> const& Square::getNext6Squares(){
	return next6Squares;
}

Square* Square::getLadderDest(){
	return ladderDest;
}

Square* Square::getSnakeDest(){
	return snakeDest;
}

class Board{
	
	int n; // n x n grid
	int numSquares, numLadders, numSnakes;
	vector<Square*> squareList;
	int maxLadderDest;

	void addLadder(int bot, int top);
	void addSnake(int head, int tail);

	void runDFS(int s, bool visited[], stack<int> &Stack/*, int& time, int start[], int finish[]*/);
public:
	Board(int n, int l, int s);
	void getLadderInputs();
	void getSnakeInputs();
	// void printBoard();

	void runMain();
};

Board::Board(int n, int l, int s) : n(n), numSquares(n*n), numLadders(l), numSnakes(s), maxLadderDest(0){

	if (n < 2 || n > 1000) throw string("out of range: 2 <= n <= 1000");
	if (l < 0 || l > 500000) throw string("out of range: 0 <= l <= 500000");
	if (s < 0 || s > 500000) throw string("out of range: 0 <= s <= 500000");

	squareList.reserve(numSquares);
	for (int i=0; i<numSquares; ++i){
		Square* sq = new Square(i+1);
		for (int j=1; j<=6; ++j){
			if (i-j>=0){
				squareList[i-j]->addNextSq(sq);
			}
		}
		squareList.push_back(sq);
	}
}

void Board::addLadder(int bot, int top){
	if (bot < 2 || bot > numSquares - 1 ||
			top < 3 || top > numSquares     ||
			bot >= top) throw string("invalid addLadder: 2 <= b < u <= n^2");
	squareList[bot-1]->addLadderDest(squareList[top-1]);
	if (top > maxLadderDest) maxLadderDest = top;
}

void Board::addSnake(int head, int tail){
	if (tail < 1 || tail > numSquares - 2 ||
			head < 2 || head > numSquares - 1 ||
			tail >= head) throw string("invalid addSnake: n^2-1 >= h > t >= 1");
	squareList[head-1]->addSnakeDest(squareList[tail-1]);
}

void Board::getLadderInputs(){
	string line;
	int bot, top;

	for (int i=0; i<numLadders; ++i){
		getline(cin, line);
		stringstream ss(line);
		ss >> bot >> top;
		addLadder(bot, top);
	}
}

void Board::getSnakeInputs(){
	string line;
	int head, tail;

	for (int i=0; i<numSnakes; ++i){
		getline(cin, line);
		stringstream ss(line);
		ss >> head >> tail;
		addSnake(head, tail);
	}
}

// void Board::printBoard(){
	
// 	cout << endl;
// 	cout << "PRINT BOARD:" << endl;
// 	cout << n << " " << numLadders << " " << numSnakes << endl;
	
// 	for (int i=0; i<numSquares; ++i){
// 		cout << endl;
// 		cout << "i = " << i << endl;;
// 		Square* sq = squareList[i];
// 		cout << "sid = " << sq->getSid() << endl;

// 		cout << "next6Squares: ";
// 		list<Square*>::iterator it;
// 		list<Square*> sqList = sq->getNext6Squares();
// 		for (it = sqList.begin(); it != sqList.end(); ++it){
// 			Square* nextsq = *it;
// 			cout << nextsq->getSid() << " ";
// 		}
// 		cout << endl;

// 		int ladderDestId = sq->getLadderDest() == NULL ? 0 : sq->getLadderDest()->getSid();
// 		int snakeDestId = sq->getSnakeDest() == NULL ? 0 : sq->getSnakeDest()->getSid();

// 		if (ladderDestId) cout << "ladderDestId = " << ladderDestId << endl;
// 		if (snakeDestId) cout << "snakeDestId = " << snakeDestId << endl;
// 	}
// }

void Board::runDFS(int s, bool visited[], stack<int> &topologyOrderStack/*, int& time, int start[], int finish[]*/){

	// cout << "runDFS:" << endl;

	Square* nextSq;
	int nextSid;

	Square* sq = squareList[s];
	// start[s] = time;
	// time++;

	if (sq->getLadderDest() != NULL){
		nextSq = sq->getLadderDest();
		nextSid = nextSq->getSid();
		// cout << "On the Ladder:" << endl;
		// cout << "s: " << s+1 << ", nextSid: " << nextSid << endl;
		if (!visited[nextSid-1]){
			// cout << "nextSid was not visited" << endl;
			visited[nextSid-1] = true;
			runDFS(nextSid-1, visited, topologyOrderStack/*, time, start, finish*/);
		}
	} else if (sq->getSnakeDest() != NULL){
		nextSq = sq->getSnakeDest();
		nextSid = nextSq->getSid();
		// cout << "On the Snake:" << endl;
		// cout << "s: " << s+1 << ", nextSid: " << nextSid << endl;
		if (!visited[nextSid-1]){
			// cout << "nextSid was not visited" << endl;
			visited[nextSid-1] = true;
			runDFS(nextSid-1, visited, topologyOrderStack/*, time, start, finish*/);
		}		
	} else {
		list<Square*>::iterator it;
		list<Square*> next6Squares = sq->getNext6Squares();
		for (it = next6Squares.begin(); it != next6Squares.end(); ++it){
			nextSq = *it;
			nextSid = nextSq->getSid();
			// cout << "s: " << s+1 << ", nextSid: " << nextSid << endl;
			if (!visited[nextSid-1]){
				// cout << "nextSid was not visited" << endl;
				visited[nextSid-1] = true;
				runDFS(nextSid-1, visited, topologyOrderStack/*, time, start, finish*/);
			}
		}
	}

	// finish[s] = time; 
	// time++;
	topologyOrderStack.push(s);
}

void Board::runMain(){

	// Initialization
	int s = 0; // sid=1
	int time = 1;
	stack<int> DFSStack;
	stack<int> topologyOrderStack;
	int dist[numSquares];
	bool visited[numSquares];
	for (int i=0; i<numSquares; ++i){
		visited[i] = false;
		dist[i] = NINF;
	}
	dist[s] = 0;

	visited[s] = true;
	DFSStack.push(s);

	// Run DFS
	runDFS(s, visited, topologyOrderStack);
	// while (!DFSStack.empty()){

	// 	// prestate
	// 	Square* nextSq;
	// 	int nextSid;

	// 	int u = DFSStack.top();
	// 	// cout << "popped: " << u+1 << endl;


	// 	Square* sq = squareList[u];

	// 	// running
	// 	if (sq->getLadderDest() != NULL){
	// 		nextSq = sq->getLadderDest();
	// 		nextSid = nextSq->getSid();
	// 		cout << "On the Ladder:" << endl;
	// 		cout << "s: " << u+1 << ", nextSid: " << nextSid << endl;
	// 		if (!visited[nextSid-1]){
	// 			cout << "nextSid was not visited" << endl;
	// 			visited[nextSid-1] = true;
	// 			DFSStack.push(nextSid-1);
	// 		}
	// 	} else if (sq->getSnakeDest() != NULL){
	// 		nextSq = sq->getSnakeDest();
	// 		nextSid = nextSq->getSid();
	// 		cout << "On the Snake:" << endl;
	// 		cout << "s: " << u+1 << ", nextSid: " << nextSid << endl;
	// 		if (!visited[nextSid-1]){
	// 			cout << "nextSid was not visited" << endl;
	// 			visited[nextSid-1] = true;
	// 			DFSStack.push(nextSid-1);
	// 		}		
	// 	} else {
	// 		list<Square*>::iterator it;
	// 		list<Square*> next6Squares = sq->getNext6Squares();
	// 		for (it = next6Squares.begin(); it != next6Squares.end(); ++it){
	// 			nextSq = *it;
	// 			nextSid = nextSq->getSid();
	// 			cout << "s: " << u+1 << ", nextSid: " << nextSid << endl;
	// 			if (!visited[nextSid-1]){
	// 				cout << "nextSid was not visited" << endl;
	// 				visited[nextSid-1] = true;
	// 				DFSStack.push(nextSid-1);
	// 			}
	// 		}
	// 	}

	// 	// poststate
	// 	u = DFSStack.top();
	// 	DFSStack.pop();
	// 	cout << "popped and pushed to topOrder: " << u+1 << endl;
	// 	topologyOrderStack.push(u);
	// }

	// Visited Array Check
	// for (int i=0; i<numSquares; ++i){
	// 	cout << i+1 << " Visited: ";
	// 	string result = visited[i] == true ? "true" : "false";
	// 	cout << result << endl;
	// }

	int retval;
	int prevTopId = 0;
	int currTopId = 0;
	int stackSize = topologyOrderStack.size();
	bool isDAG = true;
	bool reachable = true;

	if (visited[numSquares-1] == false){
		// not reachable
		reachable = false; //"impossible";
	} else {
		while (!topologyOrderStack.empty()){

			prevTopId = currTopId;
			currTopId = topologyOrderStack.top() + 1;
			// cout << "Popped sid from Stack: " << currTopId << endl;
			topologyOrderStack.pop();

			if (dist[currTopId-1] != NINF){

				Square* nextSq;
				int nextSid;

				Square* sq = squareList[currTopId-1];

				if (sq->getLadderDest() != NULL){
					nextSq = sq->getLadderDest();
					nextSid = nextSq->getSid();
					if (dist[nextSid-1] < dist[currTopId-1]){
						// cout << "ON THE LADDER:" << endl;
						// cout << "dist[" << nextSid-1 << "] = " << dist[nextSid-1] << endl;
						// cout << "dist[" << currTopId-1 << "] = " << dist[currTopId-1]  << endl;
						// cout << "hence, dist[" << nextSid-1 << "] = " << dist[currTopId-1] << endl; 
						dist[nextSid-1] = dist[currTopId-1];
					}
				} else if (sq->getSnakeDest() != NULL){
					nextSq = sq->getSnakeDest();
					nextSid = nextSq->getSid();
					// if dist[tail] < dist[head]
					if (dist[nextSid-1] < dist[currTopId-1]){
						// cout << "ON THE SNAKE:" << endl;
						// cout << "dist[" << nextSid-1 << "] = " << dist[nextSid-1] << endl;
						// cout << "dist[" << currTopId-1 << "] = " << dist[currTopId-1] << endl;
						// cout << "hence, dist[" << nextSid-1 << "] = " << dist[currTopId-1] << endl; 
						dist[nextSid-1] = dist[currTopId-1];
						isDAG = false;
					}
				} else {
					list<Square*> next6Squares = sq->getNext6Squares();
					list<Square*>::iterator it;
					for (it = next6Squares.begin(); it != next6Squares.end(); ++it){
						nextSq = *it;
						nextSid = nextSq->getSid();
						if (dist[nextSid-1] < dist[currTopId-1] + 1){
							// cout << "dist[" << nextSid-1 << "] = " << dist[nextSid-1] << endl;
							// cout << "dist[" << currTopId-1 << "] + 1 = " << dist[currTopId-1] + 1 << endl;
							// cout << "hence, dist[" << nextSid-1 << "] = " << dist[currTopId-1] +1 << endl; 
							// cout << "dist[" << nextSid-1 << "] < dist[" << currTopId-1 << "] + 1" << endl; 
							dist[nextSid-1] = dist[currTopId-1] + 1;
							
						}
					}
				}
			}
		}
	}

	// Dist Array Check
	// for (int i=0; i<numSquares; ++i){
	// 	cout << "dist[" << i << "] = " << dist[i] << endl;
	// }

	if (!reachable){
		cout << "impossible" << endl;
	} else if (!isDAG){
		if (*std::max_element(dist,dist+numSquares) == dist[numSquares-1]){
			cout << "infinity" << endl;
		} else {
			cout << dist[numSquares-1] << endl;
		}
	} else {
		cout << dist[numSquares-1] << endl;
	}
}

int main(){

	int n, l, s;
	string line;
	getline(cin, line);
	stringstream ss(line);
	ss >> n >> l >> s;

	try {	
		Board board(n, l, s);
		board.getLadderInputs();
		board.getSnakeInputs();
		// board.printBoard();
		board.runMain();
	} catch (const string& err){
		cerr << "ERROR: " << err << endl;
	}
}

