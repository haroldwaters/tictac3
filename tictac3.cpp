#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <iomanip>
#include <string>
using namespace std;


class TicTacToe{
	private: 
		HANDLE screen = GetStdHandle(STD_OUTPUT_HANDLE);//Handle for screen manipulation
		COORD position;
		string gameBoard = "0123456789";
		
		char checkMatch(int, int, int);
	public:
		char checkWin();//Looks for winning layouts
		char getSquare(int);
		string getBoard();
		void setSquare(int, char);
		void setBoard(),//Resets board to blank spaces
		     showBoard(),//Displays board
		     showScore(int, int, int);//Displays score
		bool makeMove(int, char);//User selection for board
		void auto_input(int&, int, char, string, int);
		int  auto_move_scan(char);
		void player_input(int&, int, char, string, int);
	};
char TicTacToe::checkMatch(int a, int b ,int c){
	if( gameBoard[a] == gameBoard[b] && gameBoard[b] == gameBoard[c]){
		return gameBoard[a];
	}
	return ' ';
}

char TicTacToe::checkWin(){
	char winMark = ' ';
	int startPos;
	for(int i = 1; i < 4; i++){
		startPos = i * 3;
		if( winMark == ' '){
			winMark = checkMatch(startPos, startPos - 1, startPos - 2);
		}
		if( winMark == ' '){
			winMark = checkMatch(i, i + 3, i + 6);
		}
	}
	
	if( winMark == ' '){
		winMark = checkMatch(1, 5, 9);
	}
	if( winMark == ' '){
		winMark = checkMatch(7, 5, 3);
	}
	return winMark;
}

char TicTacToe::getSquare(int square){
	return gameBoard[square];
}

string TicTacToe::getBoard(){
	return gameBoard;
}

void TicTacToe::setSquare(int square, char mark){
	string tempBoard;
	for(int i = 0; i < gameBoard.length(); i++){
		if( i == square){tempBoard += mark;}
		else{tempBoard += gameBoard[i];}
	}
	gameBoard = tempBoard;
}

void TicTacToe::setBoard(){
	gameBoard = "0123456789";
}


void TicTacToe::showBoard(){
	const int BOARD_START_X = 20,//Board start positions
			  BOARD_START_Y = 3,
			  PLAYER_ONE_COLOR = 9,//Player colors
			  PLAYER_TWO_COLOR = 13,
			  BACKGROUND_COLOR = 8;//Background number colors
	int boardRow = 0;//Will adjust output positioning downward as box is drawn
	int outputOrder[] = {0, 7, 8, 9, 4, 5, 6, 1, 2, 3};
	for (int row = 0; row < 3; row++){//Draw loop starts
		position.X = BOARD_START_X;//Position curser. As boardRow increments draws position goes downward
		position.Y = BOARD_START_Y + boardRow++;
		SetConsoleCursorPosition(screen, position);
		cout << "---------" << endl;
		
		position.Y = BOARD_START_Y + boardRow++;
		SetConsoleCursorPosition(screen, position);
		
		for(int column = 0; column < 3; column++){//Draw columns with numbers and color
			cout << '|';
			if(getSquare(outputOrder[(column + 1) + (row * 3)]) == 'X'){//Draw for "X", increment number
				SetConsoleTextAttribute(screen, PLAYER_ONE_COLOR);
				cout << getSquare(outputOrder[(column + 1) + (row * 3)]);
			}
			else if(getSquare(outputOrder[(column + 1) + (row * 3)]) == 'O'){//Draw for "O", increment number
				SetConsoleTextAttribute(screen, PLAYER_TWO_COLOR);
				cout << getSquare(outputOrder[(column + 1) + (row * 3)]);
			}
			else{
				SetConsoleTextAttribute(screen, BACKGROUND_COLOR);//Draw number, increment number
				cout << getSquare(outputOrder[(column + 1) + (row * 3)]);
			}
			SetConsoleTextAttribute(screen, BACKGROUND_COLOR);
			SetConsoleTextAttribute(screen, 7);//Reset console color
			cout<< '|';
		}
		cout << endl;
	}							
	position.Y = BOARD_START_Y + boardRow++;//Final draw for botton line of box
	SetConsoleCursorPosition(screen, position);
  	cout << "---------" << endl;
}
/*ShowScore
** Outputs scores to go alongside TicTacToe board
** Takes in two ints for player scores and a third for
** total games played
*/
void TicTacToe::showScore (int playerOne, int playerTwo, int gamesPlayed){
	const int PLAYER_ONE_XPOS = 30,//Start positions for scores
			  PLAYER_ONE_YPOS = 4,
			  PLAYER_TWO_XPOS = 30,
			  PLAYER_TWO_YPOS = 6,
			  TIE_GAME_XPOS = 30,
			  TIE_GAME_YPOS = 8,
			  GAME_COUNT_XPOS = 30,
			  GAME_COUNT_YPOS = 10;
	
	position.X = PLAYER_ONE_XPOS;//Display player 1 score
	position.Y = PLAYER_ONE_YPOS;
	SetConsoleCursorPosition(screen, position);	
	cout << "Player 1: " << playerOne << " " << fixed<< setprecision(2) << (playerOne/static_cast<double>(gamesPlayed)) * 100 << '%' << endl;
	
	position.X = PLAYER_TWO_XPOS;//Display player 2 score
	position.Y = PLAYER_TWO_YPOS;
	SetConsoleCursorPosition(screen, position);	
	cout << "Player 2: " << playerTwo << " " << fixed<< setprecision(2) << (playerTwo/static_cast<double>(gamesPlayed)) * 100 << '%' << endl;
	
	position.X = TIE_GAME_XPOS;//Display Tie games
	position.Y = TIE_GAME_YPOS;
	SetConsoleCursorPosition(screen, position);	
	cout << "Tie Games: " << gamesPlayed - (playerOne + playerTwo) << endl;
	
	position.X = GAME_COUNT_XPOS;//Display Tie games
	position.Y = GAME_COUNT_YPOS;
	SetConsoleCursorPosition(screen, position);	
	cout << "Games Played: " << gamesPlayed << endl;
}

bool TicTacToe::makeMove(int space, char mark){
	string checkList = "0123456789";
	if(getSquare(space) == checkList[space]){
		setSquare(space, mark);
		return true;
	}
	return false;
}

int  TicTacToe::auto_move_scan(char mark){
	
	int myCount = 0;
	int checkSpace = 0;
	string checkList = "0123456789";
	int vertMask[] = {0 ,1, 4, 7, 2, 5, 8, 3, 6, 9};
	int diagMask[] = {0, 1, 5, 9, 7, 5, 3};
	 
	 //check horizontal moves
	for(int i = 1; i < 10; i++){
		//count marks in current row
		if(getSquare(i) == mark){myCount++;}//tally myMarks
		else if(getSquare(i) == checkList[i]){//look for unclaimed squares
			checkSpace = i;
		}
		
		if(i % 3 == 0 && myCount == 2 && checkSpace  > 0) {//look back to see if
			return checkSpace;							   //move should be made
		}
		else if(i % 3 == 0){//reset row counters
			myCount = 0;
			checkSpace = 0;
		}
	}
	
	//check vertical moves
	for(int i = 1; i < 10; i++){
		//count marks in current row
		if(getSquare(vertMask[i]) == mark){myCount++;}//tally myMarks
		else if(getSquare(vertMask[i]) == checkList[vertMask[i]]){//look back to see if
			checkSpace = i;										  //move should be made
		}
		
		if(i % 3 == 0 && myCount == 2 && checkSpace  > 0) {
			return vertMask[checkSpace];
		}
		else if(i % 3 == 0){//reset row counters
			myCount = 0;
			checkSpace = 0;
		}
	}
	
	//check for diagonal moves
	for(int i = 1; i < 7; i++){
		//count marks in current row
		if(getSquare(diagMask[i]) == mark){myCount++;}//tally myMarks
		else if(getSquare(diagMask[i]) == checkList[diagMask[i]]){
			checkSpace = i;
		}
		
		if(i % 3 == 0 && myCount == 2 && checkSpace  > 0) {
			return diagMask[checkSpace];
		}
		else if(i % 3 == 0){
			myCount = 0;
			checkSpace = 0;
		}
	}
	return 0;
}

void TicTacToe::auto_input(int& move, int plc_hldr, char mark, string plc_hldr3 = " ", int plc_hldr2 = 0){
	 int checkSpace;
	 char myMark = 'X', opMark = 'O';
	 if(mark == 'O'){ myMark = 'O'; opMark = 'X';}
	 
	//Look for offensive moves
	move = auto_move_scan(myMark);
	//If no offensive moves, look for defensive moves
	if(move == 0){ 	move = auto_move_scan(opMark);}
	
	//select random empty space if center square is taken 
	checkSpace = 5;
	while(move == 0){
		if(getSquare(checkSpace) == ('0' + checkSpace)){
			move = checkSpace;
		}
		checkSpace = (rand() % 9) + 1;
	}
}

void TicTacToe::player_input(int& user_input, int min, char mrk, string prompt, int max = std::numeric_limits<int>::max()){
	do{
		cout << prompt;	
		if (!(cin >> user_input) || user_input < min || user_input > max){//Read input and validate
			cout << "Invalid input!\n";//Show error message
		}
		cin.clear();//Clear input buffer
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	while (!(user_input >= min) || user_input > max);
}

class Player{
	private:
		int score = 0;
		char mark;
	public:	
	void win(){
		++score;
	}
	int getScore(){
		return score;
	}
	char getMark(){
		return mark;
	}
	Player(char in_mark){
		mark = in_mark;
	}	
};

void player_input(int& user_input, int min, char mrk, string prompt, int max = std::numeric_limits<int>::max()){
	do{
		cout << prompt;	
		if (!(cin >> user_input) || user_input < min || user_input > max){//Read input and validate
			cout << "Invalid input!\n";//Show error message
		}
		cin.clear();//Clear input buffer
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	while (!(user_input >= min) || user_input > max);
}


void gameScene(int gameCount, char m1 = 'p', char m2 = 'p'){
	TicTacToe game;
	Player p1('X'), p2('O');
	
	//pointers to input methods
	///default to player input
	void (TicTacToe::*input1)(int&, int, char, string, int) = &TicTacToe::player_input;
	void (TicTacToe::*input2)(int&, int, char, string, int) = &TicTacToe::player_input;
	
	//assign input methods
	if(m1 == 'c'){ input1 = &TicTacToe::auto_input;}
	if(m2 == 'c'){ input2 = &TicTacToe::auto_input;}
	
	bool xTurn = true;
	bool goodMove;
	
	int countDown = 9;
	int move;
	for(int i = 0; i < gameCount; i++){
		
			game.showBoard();
			game.showScore(p1.getScore(), p2.getScore(), i);
		do{
			cout << "\nMoves Remaining: " << countDown << '\n';
			if(xTurn){
				do{
					(game.*input1)(move, 1, p1.getMark(), "Player 1: ", 9);
				}while(!game.makeMove(move, p1.getMark()));
				xTurn = false;
			}
			else{
				do{
					(game.*input2)(move, 1, p2.getMark(), "Player 2: ", 9);
				}while(!game.makeMove(move, p2.getMark()));
				xTurn = true;
			}
			system("cls");
			move = 0;
			game.showBoard();
			game.showScore(p1.getScore(), p2.getScore(), i + 1);
			if(m1 == 'c'){Sleep(250);}
			countDown--;
		}while(countDown > 0 && game.checkWin() ==  ' ');
		
		if(game.checkWin() == p1.getMark()){ p1.win();}
		else if (game.checkWin() == p2.getMark()){ p2.win();}
		
		game.showBoard();
		game.showScore(p1.getScore(), p2.getScore(), i);
		
		if(m1 == 'p'){system("pause");}
		system("cls");	
		
		countDown = 9;
		game.setBoard();
	}
	game.showBoard();
	game.showScore(p1.getScore(), p2.getScore(), gameCount);
	system("pause");
}


int main(){
	int choice;
	do{
		cout << "welcome to Tic-Tac-3, an interactive experience\n";
		cout << "What would you like to play?\n";
		cout << "\t 1 - Human v. Human\n";
		cout << "\t 2 - Human v. Computer\n";
		cout << "\t 3 - Computer - Computer\n";
		cout << "\t 4 - Quit\n";
		choice = 0;
		player_input(choice, 1, ' ',  "Input: ", 4);
		switch(choice){
			case 1:{
				player_input(choice, 0, ' ', "How many games? ");
				system("cls");
				gameScene(choice);
				break;
			}
			case 2:{
				player_input(choice, 0, ' ', "How many games? ");
				system("cls");
				gameScene(choice, 'p', 'c');
				break;
			}
			case 3:{
				player_input(choice, 0, ' ', "How many games? ");
				system("cls");
				gameScene(choice, 'c', 'c');
				break;
			}
		}
		system("cls");
	}
	while( choice != 4);
	cout << "\nGood bye!\n";
	return 0;
}
