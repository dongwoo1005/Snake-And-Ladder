/*
CS341 Assignment 3 Problem 1
By Dongwoo Son (20420487)
d3son@uwaterloo.ca
*/

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;
// using std::cin;
// using std::cout;

class Square{

	int s_id;
	Square* nextSquare;
	Square* ladderDest;
	Square* snakeDest;

public:
	Square(int s_id);
	void addNextSq(Square* sq);
	void addLadderDest(Square* sq);
	void addSnakeDest(Square* sq);

	int getSid();
	Square* getNextSquare();
	Square* getLadderDest();
	Square* getSnakeDest();
};

Square::Square(int s_id){
	
	this->s_id = s_id;
	this->nextSquare = NULL;
	this->ladderDest = NULL;
	this->snakeDest = NULL;
}

void Square::addNextSq(Square* sq){
	this->nextSquare = sq;
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

Square* Square::getNextSquare(){
	return nextSquare;
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

	void addLadder(int bot, int top);
	void addSnake(int head, int tail);


public:
	Board(int n, int l, int s);
	void getLadderInputs();
	void getSnakeInputs();
	void printBoard();
};

Board::Board(int n, int l, int s){

	if (n < 2 || n > 1000) throw string("out of range: 2 <= n <= 1000");
	if (l < 0 || l > 500000) throw string("out of range: 0 <= l <= 500000");
	if (s < 0 || s > 500000) throw string("out of range: 0 <= s <= 500000");

	this->n = n;
	this->numSquares = n*n;
	this->numLadders = l;
	this->numSnakes = s;
	this->squareList.reserve(numSquares+1);

	this->squareList[1] = new Square(1);
	for (int i=2; i<=numSquares; ++i){
		Square* square = new Square(i);
		squareList.push_back(square);
	}
	for (int i=1; i<=numSquares; ++i){
		
	}
}

void Board::addLadder(int bot, int top){
	if (bot < 2 || bot > numSquares - 1 ||
			top < 3 || top > numSquares     ||
			bot >= top) throw string("invalid addLadder: 2 <= b < u <= n^2");
	squareList[bot]->addLadderDest(squareList[top]);
}

void Board::addSnake(int head, int tail){
	if (tail < 1 || tail > numSquares - 2 ||
			head < 2 || head > numSquares - 1 ||
			tail >= head) throw string("invalid addSnake: n^2-1 >= h > t >= 1");
	squareList[head]->addSnakeDest(squareList[tail]);
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

void Board::printBoard(){
	
	cout << endl;
	cout << "PRINT BOARD:" << endl;
	cout << n << " " << numLadders << " " << numSnakes << endl;
	
	for (int i=1; i<=numSquares; ++i){
		cout << endl;
		cout << "i = " << i << endl;;
		Square* sq = squareList[i];
		cout << "sid = " << sq->getSid() << endl;

		int nextId = sq->getNextSquare() == NULL ? 0 : sq->getNextSquare()->getSid();
		int ladderDestId = sq->getLadderDest() == NULL ? 0 : sq->getLadderDest()->getSid();
		int snakeDestId = sq->getSnakeDest() == NULL ? 0 : sq->getSnakeDest()->getSid();

		if (nextId) cout << "nextId = " << nextId << endl;
		if (ladderDestId) cout << "ladderDestId = " << ladderDestId << endl;
		if (snakeDestId) cout << "snakeDestId = " << snakeDestId << endl;
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
		board.printBoard();
	} catch (const string& err){
		cerr << "ERROR: " << err << endl;
	}
}

