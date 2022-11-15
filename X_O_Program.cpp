// Class definition for XO_Board class
// Author:  Mohammad El-Ramly
// Date:    10/10/2022
// Version: 1
#include "X_O_Program.h"

// Return true  if move is valid and put it on board
// within board boundaries in empty cell
// Return false otherwise
bool Board::update_board (int x, int y, char mark){
    // Only update if move is valid
    if (!(x < 0 || x > 2 || y < 0 || y > 2) && (board[x][y] == 0)) {
        board[x][y] = toupper(mark);
        n_moves++;
        return true;
    }
    else
        return false;
}

// Display the board and the pieces on it
void Board::display_board() {
    for (int i: {0,1,2}) {
        cout << "\n| ";
        for (int j: {0,1,2}) {
            cout << "(" << i << "," << j << ")";
            cout << setw(2) << board [i][j] << " |";
        }
        cout << "\n-----------------------------";
    }
    cout << endl;
}

// Returns true if there is any winner
// either X or O
// Written in a complex way. DO NOT DO LIKE THIS.
bool Board::is_winner() {
    char row_win[3], col_win[3], diag_win[2];
    for (int i:{0,1,2}) {
        row_win[i] = board[i][0] & board[i][1] & board[i][2];
        col_win[i] = board[0][i] & board[1][i] & board[2][i];
    }
    diag_win[0] = board[0][0] & board[1][1] & board[2][2];
    diag_win[1] = board[2][0] & board[1][1] & board[0][2];

    // to check if any of two players is winner by his symbol in rows or columns of the board
    for (int i:{0,1,2}) {
        if ( (row_win[i] && (row_win[i] == board[i][0])) ||
             (col_win[i] && (col_win[i] == board[0][i])) )
        {return true;}
    }
    // to check if any of two players is winner by his symbol in diagonal of the board
    if ((diag_win[0] && diag_win[0] == board[1][1]) ||
        (diag_win[1] && diag_win[1] == board[1][1]))
    {return true;}
    return false;
}

// Return true if 9 moves are done and no winner
bool Board::is_draw() {
    return (n_moves == 9 && !is_winner());
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Give player a symbol to use in playing
// It can be X or O or others
// This is needed for computer players
// as it does not ask for a name
Player::Player(char symbol) {
    this->symbol = symbol;
}

// Optionally, you can give him ID or order
// Like Player 1 and Player 2
Player::Player (int order, char symbol) {
    cout << "Welcome player " << order << endl;
    cout << "Please enter your name: ";
    cin >> name;
    this->symbol = symbol;
}

// Get desired move: x y (each between 0 and 2)
// Virtual (can change for other player types)
void Player::get_move (int& x, int& y,Board b) {
    cout << "\nPlease enter your move x and y (0 to 2) separated by spaces: ";
    cin >> x >> y;
}

// Give player info as a string
string Player::to_string(){
    return "Player: " + name ;
}

// Get symbol used by player
char Player::get_symbol() {
    return symbol;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// this class to start the game
// and allows the players to play
// and determines if any player of them is winner or draw in the game end
void GameManager::run() {
    Board x_o;
    int x, y, choice;
    Player* players[2];
    players[0] = new Player (1, 'x');
    cout << "Press 1 if you want to play with computer: ";
    cin >> choice;
    if (choice != 1)
        players[1] = new Player (2, 'o');
    else
        //Player pointer points to child
        players[1] = new ComputerPlayer ('o');

    x_o.display_board();
    while (true) {
        for (int i:{0,1}) {
            players[i]->get_move(x, y,x_o);
            while (!x_o.update_board (x, y, players[i]->get_symbol())){
                players[i]->get_move(x, y,x_o);
            }
            x_o.display_board();
            // to check if any of two players is winner
            if (x_o.is_winner()){
                cout  << players[i]->to_string() << " wins\n";
                return;
            }
            // to check if the game is a draw
            if (x_o.is_draw()){
                cout << "Draw!\n";
                return;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// this constructor to give the player name
ComputerPlayer::ComputerPlayer (char symbol):Player(symbol)
{
    this->name="Computer Player";
    cout<<"My name is "<<name<< endl;
}
// this function to determine the winning, losing of player or drawing or two players
int iswindrewlose(Board b,char character)
{
    if(b.is_winner() && character=='O'){
        return 1;
    }
    else if(b.is_winner() && character=='X'){
        return -1;
    }
    else if(b.is_draw()){
        return 0;
    }
    else{
        return -2;
    }
}
// this struct to store two indecies with state of win or draw or lose
struct indexstate
{
    int index1,index2,state;
};
// Generate a specific valid move
indexstate specificvalidxy(Board b,char character)
{
    Board a=b;indexstate myresult={0,0,(character=='X'?2:-2)},myreplacedresult;int result=0;
    for(int i=0;i<=2;i++){
        for(int j=0;j<=2;j++){
            if(a.update_board(i,j,character)){
                result=iswindrewlose(a,character);
                if(result!=-2){
                    if(character=='X'){
                        if(result<myresult.state){
                            myresult={i,j,result};
                        }
                    }
                    else if(character=='O'){
                        if(result>myresult.state){
                            myresult={i,j,result};
                        }
                    }
                }
                else{
                    myreplacedresult=specificvalidxy(a,(character=='X'?'O':'X'));
                    if(character=='X'){
                        if(myreplacedresult.state<myresult.state){
                            myresult={i,j,myreplacedresult.state};
                        }
                    }
                    else if(character=='O'){
                        if(myreplacedresult.state>myresult.state){
                            myresult={i,j,myreplacedresult.state};
                        }
                    }
                }
                a=b;
            }
        }
    }
    return myresult;
}
void ComputerPlayer::get_move (int& x, int& y,Board b) {
    indexstate myresult;
    myresult=specificvalidxy(b,'O');
    x=myresult.index1;y=myresult.index2;
}