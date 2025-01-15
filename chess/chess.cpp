#include <iostream>
#include<fstream>
#include <cstdlib>

using namespace std;

void initializing_board(char**& board);
void clearConsole();
void print_board(char** board);
void deleting_board(char** board);
bool turn(bool player);
bool validsource(char** board, bool turn, int x, int y);
bool is_lower(char element);
bool is_upper(char element);
bool validdestination(char** board, bool turn, int s_x, int s_y, int d_x, int d_y);
bool Rookmove(int  s_x, int  s_y, int d_x, int  d_y, char** board, bool  turn);
int absolute(int number);
bool Bishopmove(int  s_x, int  s_y, int d_x, int  d_y, char** board, bool  turn);
bool Hoursemove(int  s_x, int  s_y, int d_x, int  d_y, char** board, bool  turn);
bool Queenmove(int  s_x, int  s_y, int d_x, int  d_y, char** board, bool  turn);
bool Kingmove(int  s_x, int  s_y, int d_x, int  d_y, char** board, bool  turn);
bool WhitePawnmove(int  s_x, int  s_y, int d_x, int  d_y, char** board, bool  turn);
bool BlackPawnmove(int  s_x, int  s_y, int d_x, int  d_y, char** board, bool  turn);
void make_move(char** board, bool turn, int s_x, int s_y, int d_x, int d_y);
void find_king(char** board, bool turn, int* king_x, int* king_y);
bool check(char** board, int* king_x, int* king_y, bool turn);
bool remove_check(char** board, int s_x, int s_y, int d_x, int d_y, int king_x, int king_y, bool player_turn);
bool checkmate(char** board, int king_x, int king_y, bool turn);
bool draw(char** board, bool turn);




int main() {
    char** board;
    bool player_turn = 0;
    initializing_board(board);
    int s_x, s_y, d_x, d_y, king_x, king_y;
    while (true)
    {
        print_board(board);
        if (player_turn) {
            cout << "-----------   white(small) turns ------------" << endl;
        }
        else
        {
            cout << "-----------   black(capital) turns ------------" << endl;
        }
        do {
            cout << "Enter the s_x,s_y: " << endl;
            cin >> s_x >> s_y;
        } while (!validsource(board, player_turn, s_x, s_y));
        do {
            cout << "Enter the d_x,d_y: " << endl;
            cin >> d_x >> d_y;
        } while (!validdestination(board, player_turn, s_x, s_y, d_x, d_y));
        make_move(board, player_turn, s_x, s_y, d_x, d_y);
        find_king(board, player_turn, &king_x, &king_y);
        if (check(board, &king_x, &king_y, player_turn)) {
            cout << "------  Check -------" << endl;
            cout << "-------------------------------------------" << endl;
            if (checkmate(board, king_x, king_y, player_turn)) {
                cout << "------  Checkmate  -------" << endl;
                if (player_turn) {
                    cout << "Black   (Uppercase)  win " << endl;;
                }
                else {
                    cout << "White   (Lowercase)  win " << endl;
                }
                break;

            }
            do {
                cout << endl << "Your are under check soo make move to get rid of that !" << endl;
                cout << "Enter the s_x, s_y: " << endl;
                cin >> s_x >> s_y;
                cout << "Enter the d_x, d_y: " << endl;
                cin >> d_x >> d_y;
            } while ((!remove_check(board, s_x, s_y, d_x, d_y, king_x, king_y, player_turn)) || (!validdestination(board, player_turn, s_x, s_y, d_x, d_y)));
            make_move(board, player_turn, s_x, s_y, d_x, d_y);
            player_turn = turn(player_turn);

        }
        else if (draw(board, !player_turn)) {
            cout << "------  draw -------" << endl;
            break;
        }
        player_turn = turn(player_turn);
    }
    deleting_board(board);

    return 0;
}


void initializing_board(char**& board) {
    board = new char* [8];
    for (int i = 0; i < 8; i++) {
        board[i] = new char[8];
    }
    ifstream file("myboard.txt");
    if (!file) {
        cout << "Error while opening the file " << endl;
        return;
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            file >> board[i][j];
        }
    }
    file.close();
}




void clearConsole() {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
}


void print_board(char** board) {
    clearConsole();
    cout << "   ";
    for (int i = 0; i < 8; i++) {
        cout << " " << i;
    }
    cout << "    # Y" << endl << endl;
    for (int i = 0; i < 8; i++) {
        cout << i << "  ";
        for (int j = 0; j < 8; j++) {
            cout << " " << board[i][j];
        }
        cout << endl;
    }

    cout << endl << "#" << endl << "X" << endl << endl;
}


void make_move(char** board, bool turn, int s_x, int s_y, int d_x, int d_y) {
    board[d_x][d_y] = board[s_x][s_y];
    board[s_x][s_y] = '.';
    print_board(board);

}


void deleting_board(char** board) {
    for (int i = 0; i < 8; i++) {
        delete[] board[i];
    }
    delete[] board;
}



bool turn(bool player) {
    return !player;
}


bool validsource(char** board, bool turn, int x, int y) {
    if (x < 0 || x >= 8 || y < 0 || y >= 8) {
        return false;
    }
    if (turn == 0 && !(is_upper(board[x][y]))) {
        return false;
    }
    if (turn == 1 && !(is_lower(board[x][y]))) {
        return false;
    }
    return true;
}



bool is_lower(char element) {
    if (element >= 'a' && element <= 'z') {
        return true;
    }
    else
        return false;
}


bool is_upper(char element) {
    if (element >= 'A' && element <= 'Z') {
        return true;
    }
    else
        return false;
}



bool validdestination(char** board, bool turn, int s_x, int s_y, int d_x, int d_y) {

    if (board[s_x][s_y] == 'R' || board[s_x][s_y] == 'r') {
        if (Rookmove(s_x, s_y, d_x, d_y, board, turn)) {
            return true;
        }
        else
            return false;
    }

    else if (board[s_x][s_y] == 'B' || board[s_x][s_y] == 'b') {
        if (Bishopmove(s_x, s_y, d_x, d_y, board, turn)) {
            return true;
        }
        else
            return false;
    }
    else if (board[s_x][s_y] == 'Q' || board[s_x][s_y] == 'q') {
        if (Queenmove(s_x, s_y, d_x, d_y, board, turn)) {
            return true;
        }
        else
            return false;
    }
    else if (board[s_x][s_y] == 'K' || board[s_x][s_y] == 'k') {
        if (Kingmove(s_x, s_y, d_x, d_y, board, turn)) {
            return true;

        }
    }
    else if (board[s_x][s_y] == 'N' || board[s_x][s_y] == 'n') {
        if (Hoursemove(s_x, s_y, d_x, d_y, board, turn)) {
            return true;
        }
        else
            return false;
    }
    else if (board[s_x][s_y] == 'p') {
        if (WhitePawnmove(s_x, s_y, d_x, d_y, board, turn)) {
            return true;
        }
    }
    else if (board[s_x][s_y] == 'P') {
        if (BlackPawnmove(s_x, s_y, d_x, d_y, board, turn)) {
            return true;
        }
    }


    return false;

}



bool Rookmove(int  s_x, int  s_y, int d_x, int  d_y, char** board, bool  turn) {
    if ((turn == 0 && (is_upper(board[d_x][d_y]))) || (turn == 1 && (is_lower(board[d_x][d_y])))) {
        return false;
    }
    if (s_x != d_x && s_y != d_y) {
        return false;
    }
    if (s_x == d_x) {
        if (s_y < d_y) {
            for (int col = s_y + 1; col < d_y; ++col) {
                if (board[s_x][col] != '.') {
                    return false;
                }
            }
        }
        else {
            for (int col = s_y - 1; col > d_y; --col) {
                if (board[s_x][col] != '.') {
                    return false;
                }
            }
        }
    }
    else if (s_y == d_y) {
        if (s_x < d_x) {
            for (int row = s_x + 1; row < d_x; ++row) {
                if (board[row][s_y] != '.') {
                    return false;
                }
            }
        }
        else {
            for (int row = s_x - 1; row > d_x; --row) {
                if (board[row][s_y] != '.') {
                    return false;
                }
            }
        }
    }
    return true;
}


bool Bishopmove(int  s_x, int  s_y, int d_x, int  d_y, char** board, bool  turn) {
    if (absolute(s_x - d_x) != absolute(s_y - d_y)) {
        return false;
    }
    if ((turn == 0 && (is_upper(board[d_x][d_y]))) || (turn == 1 && (is_lower(board[d_x][d_y])))) {
        return false;
    }
    if (s_x < d_x && s_y < d_y) {
        for (int row = s_x + 1, col = s_y + 1; row < d_x && col < d_y; row++, col++) {
            if (board[row][col] != '.') {
                return false;
            }
        }
    }
    else if (d_x > s_x && d_y < s_y) {
        for (int row = s_x + 1, col = s_y - 1; row < d_x && col > d_y; row++, col--) {
            if (board[row][col] != '.') {
                return false;
            }
        }
    }
    else if (d_x < s_x && d_y > s_y) {
        for (int row = s_x - 1, col = s_y + 1; row > d_x && col < d_y; row--, col++) {
            if (board[row][col] != '.') {
                return false;
            }
        }
    }
    else if (d_x < s_x&& d_y < s_y) {
        for (int row = s_x - 1, col = s_y - 1; row > d_x && col > d_y; row--, col--) {
            if (board[row][col] != '.') {
                return false;
            }
        }
    }
    return true;
}


int absolute(int number) {
    if (number < 0) {
        return -(number);
    }
    else
        return number;
}


bool Hoursemove(int  s_x, int  s_y, int d_x, int  d_y, char** board, bool  turn) {
    int dx = absolute(d_x - s_x);
    int dy = absolute(d_y - s_y);
    if (!((dx == 2 && dy == 1) || (dx == 1 && dy == 2))) {
        return false;
    }
    if ((turn == 0 && (is_upper(board[d_x][d_y]))) || (turn == 1 && (is_lower(board[d_x][d_y])))) {
        return false;
    }
    return true;
}


bool Queenmove(int  s_x, int  s_y, int d_x, int  d_y, char** board, bool  turn) {
    if ((Rookmove(s_x, s_y, d_x, d_y, board, turn)) || (Bishopmove(s_x, s_y, d_x, d_y, board, turn))) {
        return true;
    }
    else
    {
        return false;
    }
}


bool Kingmove(int s_x, int s_y, int d_x, int d_y, char** board, bool turn) {
    if ((turn == 0 && is_upper(board[d_x][d_y])) || (turn == 1 && is_lower(board[d_x][d_y]))) {
        return false;
    }
    if ((s_x + 1 == d_x && s_y == d_y) || (s_x - 1 == d_x && s_y == d_y) || (s_x == d_x && s_y + 1 == d_y) || (s_x == d_x && s_y - 1 == d_y) || (s_x + 1 == d_x && s_y + 1 == d_y) || (s_x - 1 == d_x && s_y - 1 == d_y) || (s_x + 1 == d_x && s_y - 1 == d_y) || (s_x - 1 == d_x && s_y + 1 == d_y)) {
        return true;
    }
    return false;
}


bool WhitePawnmove(int s_x, int s_y, int d_x, int d_y, char** board, bool turn) {
    if (turn == 1 && s_x == 1 && d_x == 0) {
        char promotion;
        do {
            cout << "Promote the pawn (q/r/b/n): ";
            cin >> promotion;
        } while (promotion != 'q' && promotion != 'r' && promotion != 'b' && promotion != 'n');
        board[d_x][d_y] = promotion;
    }
    if (turn == 1 && is_lower(board[d_x][d_y])) {
        return false;
    }
    if (s_x - 1 == d_x && s_y == d_y && board[d_x][d_y] == '.') {
        return true;
    }
    if (s_x - 1 == d_x && (s_y + 1 == d_y || s_y - 1 == d_y) && is_upper(board[d_x][d_y])) {
        return true;
    }
    return false;
}

bool BlackPawnmove(int s_x, int s_y, int d_x, int d_y, char** board, bool turn) {
    // Ensure it's Black's turn and the pawn is moving downward (from lower to higher rows)
    if (turn == 0 && s_x + 1 == d_x) {
        // Forward move
        if (s_y == d_y && board[d_x][d_y] == '.') {
            // Perform the move
            board[d_x][d_y] = board[s_x][s_y];
            board[s_x][s_y] = '.';

            // Promotion check
            if (d_x == 7) {
                char promotion;
                do {
                    cout << "Promote the pawn (Q/R/B/N): ";
                    cin >> promotion;
                } while (promotion != 'Q' && promotion != 'R' && promotion != 'B' && promotion != 'N');
                board[d_x][d_y] = promotion; // Replace pawn with promoted piece
            }
            return true;
        }

        // Capture move
        if ((s_y + 1 == d_y || s_y - 1 == d_y) && is_lower(board[d_x][d_y])) {
            // Perform the capture
            board[d_x][d_y] = board[s_x][s_y];
            board[s_x][s_y] = '.';

            // Promotion check
            if (d_x == 7) {
                char promotion;
                do {
                    cout << "Promote the pawn (Q/R/B/N): ";
                    cin >> promotion;
                } while (promotion != 'Q' && promotion != 'R' && promotion != 'B' && promotion != 'N');
                cout << "THis is the pieace going to be promoted " << promotion << endl;
                board[d_x][d_y] = promotion; // Replace pawn with promoted piece
            }
            return true;
        }
    }

    // Invalid move if none of the above conditions are satisfied
    return false;
}




void find_king(char** board, bool turn, int* king_x, int* king_y) {
    char king;
    if (turn) {
        king = 'K';

    }
    else
    {
        king = 'k';

    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] == king) {

                *king_x = i;
                *king_y = j;
            }
        }
    }
}



bool check(char** board, int* king_x, int* king_y, bool turn) {
    char King;
    char opponent_start;
    char opponent_end;
    if (turn) {
        King = 'Q';
        opponent_start = 'a';
        opponent_end = 'z';
    }
    else {
        King = 'q';
        opponent_start = 'A';
        opponent_end = 'Z';
    }
    int s_x, s_y;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] >= opponent_start && board[i][j] <= opponent_end) {
                s_x = i;
                s_y = j;
                if (validdestination(board, turn, s_x, s_y, *king_x, *king_y)) {
                    return true;
                }
            }
        }
    }
    return false;
}


bool remove_check(char** board, int s_x, int s_y, int d_x, int d_y, int king_x, int king_y, bool player_turn) {
    char** temp_board = new char* [8];
    for (int i = 0; i < 8; i++) {
        temp_board[i] = new char[8];
        for (int j = 0; j < 8; j++) {
            temp_board[i][j] = board[i][j];
        }
    }
    temp_board[d_x][d_y] = temp_board[s_x][s_y];
    temp_board[s_x][s_y] = '.';
    if (player_turn) {
        if (temp_board[d_x][d_y] == 'K') {
            king_x = d_x;
            king_y = d_y;
        }
    }
    else {
        if (temp_board[d_x][d_y] == 'k') {
            king_x = d_x;
            king_y = d_y;
        }
    }
    bool result = !check(temp_board, &king_x, &king_y, player_turn);
    for (int i = 0; i < 8; i++) {
        delete[] temp_board[i];
    }
    delete[] temp_board;

    return result;
}


bool checkmate(char** board, int king_x, int king_y, bool turn) {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            int newRow = king_x + i;
            int newCol = king_y + j;
            if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8 && board[newRow][newCol] == '.') {
                int temp_king_x = king_x;
                int temp_king_y = king_y;
                char temp = board[newRow][newCol];
                if (turn) {
                    board[newRow][newCol] = 'K';
                }
                else {
                    board[newRow][newCol] = 'k';
                }
                board[king_x][king_y] = '-';
                if (!check(board, &newRow, &newCol, !turn)) {
                    board[newRow][newCol] = temp;
                    if (turn) {
                        board[king_x][king_y] = 'K';
                    }
                    else {
                        board[king_x][king_y] = 'k';
                    }
                    return false;
                }
                board[newRow][newCol] = temp;
                if (turn) {
                    board[king_x][king_y] = 'K';
                }
                else {
                    board[king_x][king_y] = 'k';
                }
            }
        }
    }
    return true;
}


bool draw(char** board, bool turn) {
    int s_x, s_y;
    int d_x, d_y;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((turn && is_lower(board[i][j])) || (!turn && is_upper(board[i][j]))) {
                s_x = i;
                s_y = j;
                for (int r = 0; r < 8; r++) {
                    for (int c = 0; c < 8; c++) {
                        d_x = r;
                        d_y = c;
                        if (validdestination(board, turn, s_x, s_y, d_x, d_y)) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}