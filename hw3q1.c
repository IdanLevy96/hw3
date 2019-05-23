/*-------------------------------------------------------------------------
	Include files:
-------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdbool.h>

/*-------------------------------------------------------------------------
	Constants and definitions:
-------------------------------------------------------------------------*/
#define N 11
#define MATRIX 2
#define SWITCH_PLAYER 3 - *player_index_ptr
#define PLAYER_ONE 'X'
#define PLAYER_TWO 'O'
#define INIT '_'
#define UNDO i < 0 && (-i % 2) == 1 && -i < *turn_ptr
#define INVALID_UNDO (i < 0 && -i % 2 == 0) || -i >= *turn_ptr
#define VALID i + 1 > n || i < 0 || j + 1 > n || j < 0 || board[i][j] != INIT
#define SECOND_PLAYER player_index == 2


/*-------------------------------------------------------------------------
	Function declaration
-------------------------------------------------------------------------*/
void print_welcome();
void print_enter_board_size();
void print_board(char board[N][N], int n);
void print_player_turn(int player_index);
void print_error();
void print_winner(int player_index);
void print_tie();
void init_board(char board[][N], int n);
int scan_board_size();
int game(char board[][N], int n, int moves[][N * N], int* turn_ptr, int* player_index_ptr);
bool scan_move(int* turn_ptr, int moves[][N * N], int n, char board[][N], int* player_index_ptr, int i, int j);
bool is_undo(int i, int* turn);
void undo(int i, int* turn, char board[][N], int moves[][N * N]);
void invalid_move(int* turn_ptr, int* player_ptr);
void update_board(char board[][N], int moves[][N * N], int turn, int player_index);
bool is_valid(int i, int j, int n, char board[][N]);
bool check_loser(int player_index, char board[][N], int n);
bool check_row(int player_index, char board[][N], int n);
bool check_col(int player_index, char board[][N], int n);
bool check_main_diag(int player_index, char board[][N], int n);
bool check_sec_diag(int player_index, char board[][N], int n);

/*-------------------------------------------------------------------------
	Implementation
-------------------------------------------------------------------------*/

//Initialize the board, start the game and print the winner
//10 lines
int main() {
	char board[N][N];
	int moves[MATRIX][N * N], n, turn = 1, player_index = 1, * turn_ptr = &turn, * player_index_ptr = &player_index;
	n = scan_board_size();
	if (n > N || n < 1) return 0;
	init_board(board, n);
	player_index = game(board, n, moves, turn_ptr, player_index_ptr);
	if (player_index == 0) {
		print_tie();
	}
	else print_winner(player_index);
	return 0;
}

//Show welcome message and ask user for board size
//5 lines
int scan_board_size() {
	int n;
	print_welcome();
	print_enter_board_size();
	if (scanf(" %d", &n) != 1) return 0;
	return n;
}

//Main game function - get moves from players and checks if the game is finished
//12 lines
int game(char board[][N], int n, int moves[][N * N], int* turn_ptr, int* player_index_ptr) {
	bool is_winner = false;
	for (print_player_turn(*player_index_ptr); *turn_ptr <= n * n; ++ * turn_ptr, *player_index_ptr = SWITCH_PLAYER) {
		if (scan_move(turn_ptr, moves, n, board, player_index_ptr, 0, 0) == false) {
			invalid_move(turn_ptr, player_index_ptr);
			continue;
		}
		print_board(board, n);
		if ((is_winner = (check_loser(*player_index_ptr, board, n)) == true) || *turn_ptr == n * n) {
			break;
		}
		print_player_turn(SWITCH_PLAYER);
	}
	if (is_winner == true)
		return (SWITCH_PLAYER);
	else
		return 0;
}

//Get move from player
//13 lines
bool scan_move(int* turn_ptr, int moves[][N * N], int n, char board[][N], int* player_index_ptr, int i, int j) {
	scanf(" %d", &i);
	if (UNDO) {
		undo(i, turn_ptr, board, moves);
		return true;
	}
	if (INVALID_UNDO)
		return false;
	else
		moves[0][*turn_ptr] = i - 1;
	scanf(" %d", &j);
	if (is_valid(i - 1, j - 1, n, board) == false)
		return false;
	else
		moves[1][*turn_ptr] = j - 1;
	update_board(board, moves, *turn_ptr, *player_index_ptr);
	return true;
}

//Check if player wants to undo
//3 lines
bool is_undo(int i, int* turn_ptr) {
	if (UNDO)
		return true;
	else
		return false;
}

//Reverts the board back to what the user asks and passes the turn to the other player
//6 lines
void undo(int i, int* turn_ptr, char board[][N], int moves[][N * N]) {
	for (int counter = -i; counter > 0; --counter) {
		board[moves[0][*turn_ptr - counter]][moves[1][*turn_ptr - counter]] = INIT;
	}
	*turn_ptr = *turn_ptr + i - 1;
}

//Check if the player's move is valid
//3 lines
bool is_valid(int i, int j, int n, char board[][N]) {
	if (VALID)
		return false;
	else
		return true;
}

//Function that handles a case in which user entered an invalid move
//3 lines
void invalid_move(int* turn_ptr, int* player_index_ptr) {
	--* turn_ptr;
	*player_index_ptr = SWITCH_PLAYER;
	print_error();
}

//Update the board after each move
//4 lines
void update_board(char board[][N], int moves[][N * N], int turn, int player_index) {
	char letter = PLAYER_ONE;
	if (SECOND_PLAYER) letter = PLAYER_TWO;
	board[moves[0][turn]][moves[1][turn]] = letter;
}

//Initialize board with '_' in each cell
//4 lines
void init_board(char board[][N], int n) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			board[i][j] = INIT;
		}
	}
	print_board(board, n);
}

//Check if the current player lost
//9 lines
bool check_loser(int player_index, char board[][N], int n) {
	if (check_row(player_index, board, n) == true)
		return true;
	if (check_col(player_index, board, n) == true)
		return true;
	if (check_main_diag(player_index, board, n) == true)
		return true;
	if (check_sec_diag(player_index, board, n) == true)
		return true;
	return false;
}

//Check if an entire row has player_index's letter
//10 lines
bool check_row(int player_index, char board[][N], int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0, count = 0; j < n; j++) {
			if (board[i][j] != PLAYER_ONE && player_index == 1)
				break;
			if (board[i][j] != PLAYER_TWO && SECOND_PLAYER)
				break;
			count++;
			if (count == n)
				return true;
		}
	}
	return false;
}

//Check if an entire coloumn has player_index's letter
//10 lines
bool check_col(int player_index, char board[][N], int n) {
	for (int j = 0; j < n; j++) {
		for (int i = 0, count = 0; i < n; i++) {
			if (board[i][j] != PLAYER_ONE && player_index == 1)
				break;
			if (board[i][j] != PLAYER_TWO && SECOND_PLAYER)
				break;
			count++;
			if (count == n)
				return true;
		}
	}
	return false;
}

//Check if the entire main diagonal has player_index's letter
//8 lines
bool check_main_diag(int player_index, char board[][N], int n) {
	for (int i = 0, count = 0; i < n; i++) {
		if (board[i][i] != PLAYER_ONE && player_index == 1)
			break;
		if (board[i][i] != PLAYER_TWO && SECOND_PLAYER)
			break;
		count++;
		if (count == n)
			return true;
	}
	return false;
}

//Check if the entire secondary diagonal has player_index's letter
//9 lines
bool check_sec_diag(int player_index, char board[][N], int n) {
	for (int i = 0, j = n - 1, count = 0; i < n; i++, j--) {
		if (board[i][j] != PLAYER_ONE && player_index == 1)
			break;
		if (board[i][j] != PLAYER_TWO && SECOND_PLAYER)
			break;
		count++;
		if (count == n)
			return true;
	}
	return false;
}

//print welcome message
//1 lines
void print_welcome()
{
	printf("*** Welcome to AVOIDANCE TIC-TAC-TOE game ***\n\n");
}

//print message to enter board size
//1 lines
void print_enter_board_size()
{
	printf("Please enter board size (1 to %d):\n", N);
}

//print the board
//7 lines
void print_board(char board[N][N], int n)
{
	printf("\nCurrent board:\n");
	for (int i = 0; i < n; i++)
	{
		printf("|");
		for (int j = 0; j < n; j++)
		{
			printf("%c|", board[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

//print a request for player with index "player_index" to move
//1 lines
void print_player_turn(int player_index)
{
	printf("\nPlayer ** %d **, enter next move:\n", player_index);
}

//print error message
//1 lines
void print_error()
{
	printf("Illegal move!!!, please try again:\n");
}

//print the winner
//1 lines
void print_winner(int player_index)
{
	printf("Player %d Wins! Hooray!\n", player_index);
}

//print message to announce there is a tie (no one wins)
//1 lines
void print_tie()
{
	printf("It's a tie!\n");
}
