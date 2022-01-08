#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <windows.h>

/*IDEAS: Win/Lose Sprüche random aus einem array. -> unterschiedlich bei PvP und PvC (PvC muss Spieler ansprechen):
 * "The mind is nothing more than an involuntary ejaculation of the unavoidable entropy of infinite space."
 * Random delay bei computer_turns.
*/
char board[3][3];
int statistics_p1[7] = {0, 0, 0, 0, 0, 0, 0}; // Games played, wins, losses, ties, X games, O games, signs placed
char player_one = 'X';
char player_one_name[21];
int statistics_p2[7] = {0, 0, 0, 0, 0, 0, 0}; // Games played, wins, losses, ties, X games, O games, signs placed
char player_two = 'O';
char player_two_name[21] = "COMPUTER";
int game_mode = 0;

void player_stats_load() {
    char player_one_file[255];
    strcpy(player_one_file, player_one_name);
    strcat(player_one_file, ".txt");

    FILE* fptr;
    int num;
    if ((fptr = fopen(player_one_file, "r+")) == NULL) {
        printf("You seem to be new here, %s, good luck!\n", player_one_name);
    }else {
        for (int i = 0; i < 7; i++) {
            fscanf(fptr, "%d", &num);
            statistics_p1[i] = num;
        }
        if (fclose(fptr) != 0) {
            printf("ERROR: Closing file was unsuccessful!\n");
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < 7; ++i) { //prints array for test
            printf("[%d]", statistics_p1[i]);
        }
    }
}

void player_stats_save() {
    char player_one_file[255];
    strcpy(player_one_file, player_one_name);
    strcat(player_one_file, ".txt");

    FILE* fptr;
    if ((fptr = fopen(player_one_file, "w")) == NULL) {
        fprintf(stderr, "ERROR: Cannot open file!\n");
        return;
    }
    for (int i = 0; i < 7; i++) {
        fprintf(fptr, "%d ", statistics_p1[i]);
    }
    if (game_mode != 3){
        printf("Statistics saved!\n");
    }

    if (fclose(fptr) != 0) {
        fprintf(stderr, "ERROR: Unable to close file!\n");
        return;
    }
}

void playeroptions() {
    char player_one_sign;

    printf("Player One - Enter your name (max. 20 characters):");
    fgets(player_one_name, 20, stdin);
    player_one_name[strlen(player_one_name) - 1] = '\0';

    player_stats_load();

    int tmpmode;
    printf("\nChoose a game mode!\n1: Human versus Human\n2: Human versus Machine\n3: BATTLE OF THE MACHINES!\nPress 4 to flee!\n");
    tmpmode = getchar();
    getchar();
    tmpmode = tmpmode -48; // 1-9 on ASCII TABLE
    switch (tmpmode) {
        case 1:
            game_mode = 1;
            break;
        case 2:
            game_mode = 2;
            break;
        case 3:
            game_mode = 3;
            break;
        case 4:
            exit(EXIT_SUCCESS);
        default:
            printf("Invalid choice! You will be forced to watch a battle of the machines!\n");
            game_mode = 3;
            break;
    }

    sign:
    if (game_mode != 3) {
        printf("%s, chose your sign: X or O\n", player_one_name);
        player_one_sign = getchar();
        getchar();
        if (player_one_sign == 'x' || player_one_sign == 'X') {
            player_one = 'X';
            player_two = 'O';
            statistics_p1[4]++;
            statistics_p2[5]++;
        } else if (player_one_sign == 'o' || player_one_sign == 'O') {
            player_one = 'O';
            player_two = 'X';
            statistics_p1[5]++;
            statistics_p2[4]++;
        } else {
            printf("Invalid Sign! Please choose X or O!\n");
            goto sign;
        }
    }
    if (game_mode == 1) {
        printf("Player Two - Enter your name (max. 20 characters):");
        fgets(player_two_name, 20, stdin);
        player_two_name[strlen(player_two_name) - 1] = '\0';
    }

    for (int i = 0; i < 7; ++i) { //TEST PRINTOUT
        printf("[%d]", statistics_p1[i]);
    }
    printf("\n");
}


char who_wins() {
    for (int i = 0; i < 3; i++) { //checks rows for matching signs
        if ((board[i][0] == board[i][1]) && (board[i][0] == board[i][2])) {
            return board[i][0];
        }
    }

    for (int i = 0; i < 3; i++) { //checks columns for matching signs
        if ((board[0][i] == board[1][i]) && (board[0][i] == board[2][i])) {
            return board[0][i];
        }
    }

    if ((board[0][0] == board[1][1]) && (board[0][0] == board[2][2])) {        //andere Lösung?
        return board[0][0];
    }
    if ((board[0][2] == board[1][1]) && (board[0][2] == board[2][0])) {
        return board[0][2];
    }
    return ' ';
}

void fillFreeSpace() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }
}

void printBoard() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("| %c |", board[i][j]);
        }
        if (i < 2) {
            printf("\n|---||---||---|\n");
        }
    }
    printf("\n");
}

int freeSpace() {       // return 1 if full, return 0 if not
    int spaces = 0;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != ' ') {
            spaces++;
            }
        }
    }
    return spaces;
}

void player_1_turn() {
    int x, y;

    do {
        if (game_mode != 3) {
            printf("Player 1: Enter row# and column# (1-3):");
            scanf("%d %d", &x, &y);
            x--, y--;
            //printf("%d - %d", x, y);

            if (board[x][y] != ' ') {
                printf("This field has already been used!\n");
            } else {
                board[x][y] = player_one;
                break;
            }
        }else {                                         //checks all empty field for number of adjecent (computer) player signs
            printf("COMPUTER'S TURN!\n");
            int computer_count[8] = {0, 0, 0, 0, 0, 0, 0, 0};      // counts fields the computer has already picked
            int computer_pick[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};                             // counts free fields
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    int pos = i + (j * 3);
                    if (board[j][i] != ' ') {
                        computer_pick[pos] = 1;
                    } else {
                        computer_pick[pos] = 0;
                    }
                }

            }
            for (int i = 0; i < 3; i++) {                       // checks rows and columns sum of own player signs
                for (int j = 0; j < 3; j++) {
                    if (board[i][j] == player_one) {
                        computer_count[i]++;
                    }
                    if (board[j][i] == player_one) {
                        computer_count[i + 3]++;
                    }
                }
            }
            for (int k = 0;
                 k < 3; k++) {                           //checks diagonals - relevant for positions 0/2/4/6/8
                if (board[k][k] == player_one) {
                    computer_count[6]++;
                }
                if (board[2 - k][k] == player_one) {
                    computer_count[7]++;
                }
            }
            // get position of best empty field
            int record[3] = {0, 0, 0};             //0 = rows, 1 = columns, 2 = diagonals
            int lastbest = rand() % 9;                      //start on random field OVERWRITES PLAYER TURN!!!
            for (int k = 0; k < 9; k++) {
                //int j = computer_pick[k] % 3;               // for diagonals
                int i = computer_pick[k] / 3;

                if (computer_pick[k] == 0) {                 // if field is empty
                    if (computer_count[i] > record[0]) {   //checks rows for best fields
                        record[0] = computer_count[i];
                        lastbest = k;
                    }
                    if (computer_count[i + 3] > record[1]) { //checks columns for best fields
                        record[1] = computer_count[i + 3];
                        lastbest = k;
                    }
                }
            }
            y = lastbest % 3;
            x = lastbest / 3;
            sleep(1);
            board[x][y] = player_one;
            break;
        }
    } while (board[x][y] != ' ');
}

void player_2_turn() {
    int x, y;
        do {
            if (game_mode == 1) {
                printf("Player 2: Enter row# and column# (1-3):");
                scanf("%d %d", &x, &y);
                x--, y--;
                //printf("%d - %d", x, y);

                if (board[x][y] != ' ') {
                    printf("This field has already been used!\n");
                } else {
                    board[x][y] = player_two;
                    break;
                }
            } else {                                         //checks all empty field for number of adjecent (computer) player signs
                printf("COMPUTER'S TURN!\n");
                int computer_count[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // counts fields the computer has already picked
                int computer_pick[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};   // counts free fields
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        int pos = i + (j * 3);
                        if (board[j][i] != ' ') {
                            computer_pick[pos] = 1;
                        } else {
                            computer_pick[pos] = 0;
                        }
                    }

                }
                for (int i = 0; i < 3; i++) {                       // checks rows and columns sum of own player signs
                    for (int j = 0; j < 3; j++) {
                        if (board[i][j] == player_two) {
                            computer_count[i]++;
                        }
                        if (board[j][i] == player_two) {
                            computer_count[i + 3]++;
                        }
                    }
                }
                for (int k = 0;
                     k < 3; k++) {                           //checks diagonals - relevant for positions 0/2/4/6/8
                    if (board[k][k] == player_two) {
                        computer_count[6]++;
                    }
                    if (board[2 - k][k] == player_two) {
                        computer_count[7]++;
                    }
                }
                // get position of best empty field
                int record[3] = {0, 0, 0};             //0 = rows, 1 = columns, 2 = diagonals
                int lastbest = rand() % 9;                      //start on random field - OVERWRITES PLAYER TURN!
                for (int k = 0; k < 9; k++) {
                    //int j = computer_pick[k] % 3;               // for diagonals
                    int i = computer_pick[k] / 3;

                    if (computer_pick[k] == 0) {                 // if field is empty
                        if (computer_count[i] > record[0]) {   //checks rows for best fields
                            record[0] = computer_count[i];
                            lastbest = k;
                        }
                        if (computer_count[i + 3] > record[1]) { //checks columns for best fields
                            record[1] = computer_count[i + 3];
                            lastbest = k;
                        }
                    }
                }
                y = lastbest % 3;
                x = lastbest / 3;
                sleep(1);
                board[x][y] = player_two;
                break;
            }
        } while (board[x][y] != ' ');
}


int main() {

    srand(time(NULL));
    char winner = ' ';
    int starting_player = (rand() % 2);

    winner = ' ';
    fillFreeSpace();
    playeroptions();

    while(winner == ' ' && freeSpace() != 9) {
        printBoard();
        if (starting_player == 0){
            player_1_turn();
        }else {
            player_2_turn();
        }
        winner = who_wins();
        freeSpace();
        starting_player++;
        starting_player = (starting_player % 2);

        if (winner != ' ' || freeSpace() == 9) {
            break;
        }
    }
    printBoard();

    if (winner != ' ' || freeSpace() == 9) {
        if(winner == player_one){
            if (game_mode == 3) {
                printf("The MACHINE wins!\n");
            }else {
                printf("%s WINS!\n", player_one_name);
            }
        }else if(winner == player_two) {
            printf("%s WINS!\n", player_two_name);
        }else {
            printf("IT'S A DRAW!\n");
        }
    }
    player_stats_save();


    return 0;
}