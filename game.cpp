#include <iostream>
#include <vector>
using namespace std;

class Game {
  public:

    vector<vector<char>> table = {{' ', ' ', ' '},{' ', ' ', ' '},{' ', ' ', ' '}};
    char turn = 'x';

    bool change(int x, int y) {
        if (x < 1 || x > 3 || y < 1 || y > 3 || table[y - 1][x - 1] != ' ') return false;

        table[y - 1][x - 1] = turn;

        return true;
    }

    void changeTurn() {
        switch (turn) {
            case 'x':
                turn = 'o';
              break;
            case 'o':
                turn = 'x';
              break;
        }
    }

    bool checkFinish() {
        if (checkWin() || checkDraw()) return true;
        return false;
    }

    bool checkWin() {
        for (int x = 0; x < 3; x++)
            if ((table[x][0] != ' ' && table[x][0] == table[x][1] && table[x][1] == table[x][2]) || (table[0][x] != ' ' && table[0][x] == table[1][x] && table[1][x] == table[2][x])) return true;

        if (table[1][1] != ' ' && ((table[0][0] == table[1][1] && table[1][1] == table[2][2]) || (table[0][2] == table[1][1] && table[1][1] == table[2][0]))) return true;

        return false;
    }

    bool checkDraw() {
        for (int x = 0; x < 3; x++)
            for (int y = 0; y < 3; y++)
                if (table[y][x] == ' ') return false;

        return true;
    }

};

class InputOutput {
  public:

    string initPrint() {
        cout << "Benvenuto su TRIS!\n\nInserendo 1) Player vs Computer\nInserendo 2) Player1 vs Player2" << endl;

        string mod;
        int attempts = 0;
        do {
            if (attempts > 0) cout << "Codice modalità non valido, scegline un altro..." << endl;
            attempts++;

            cout << "\nInserisci la modalità: "; cin >> mod;
        } while (mod != "1" && mod != "2");

        return mod;
    }

    void printTable(vector<vector<char>> table) {
        cout << "\n";

        cout << " " << table[0][0] << " | " << table[0][1] << " | " << table[0][2] << " " << endl;
        cout << "---|---|---" << endl;
        cout << " " << table[1][0] << " | " << table[1][1] << " | " << table[1][2] << " " << endl;
        cout << "---|---|---" << endl;
        cout << " " << table[2][0] << " | " << table[2][1] << " | " << table[2][2] << " " << endl;
    }

    void makeMove(Game& game) {
        string x, y;
        int attempts = 0;
        do {
            if (attempts > 0) cout << "Riga o colonna non selezionabile, scegline un altra..." << endl;
            attempts++;

            cout << "\nScegli la colonna: "; cin >> x;
            cout << "Scegli la riga: "; cin >> y;
        } while ((!isdigit(x[0]) || !isdigit(y[0])) || !game.change(stoi(x), stoi(y)));
    }

    void playerNum(char turn) {
        if (turn == 'x') cout << "Turno Player 2 (x)" << endl;
        else             cout << "Turno Player 1 (o)" << endl;
    }

    void clear() {
        cout << "\033c";
    }

    bool askToPlayAgain() {
        string res;
        cout << "Vuoi giocare ancora? (si/no) "; cin >> res;

        if (res == "si") return true;
        else return false;
    }

    void awardsVictory(char turn) {
        switch (turn) {
            case 'x':
                cout << "\nVince x (bot)!\n" << endl;
              break;
            case 'o':
                cout << "\nVince o (player)!\n" << endl;
              break;
            case 'd':
                cout << "\nPatta!\n" << endl;
              break;
        }
    }

    int lastPrint() {
        clear();
        cout << "Alla prossima!" << endl;
        return 0;
    }

};

class Bot {
  public:

    void makeMove(Game& game) {
        vector<int> move = minimaxRoot(game.table);
        game.change(++move[1], ++move[0]);
    }

  private:

    vector<int> minimaxRoot(vector<vector<char>> table, bool isMaximisingPlayer = true) {
        int depth = 0;
        vector<int> bestMove;
        int bestValue = -9999;
        
        const vector<vector<int>> freeSpaces = findFreeSpaces(table);
        
        for (int i = 0; i < freeSpaces.size(); i++) {
            
            const vector<vector<char>> tableBackup = table;
            
            table[freeSpaces[i][0]][freeSpaces[i][1]] = 'x';
            const int attualValue = minimax(table, depth + 1, -10000, 10000, !isMaximisingPlayer);
            
            table = tableBackup;
            
            if (attualValue >= bestValue) {
                bestValue = attualValue;
                bestMove = freeSpaces[i];
            }
            
        }
        
        return bestMove;
    }
    
    int minimax(vector<vector<char>> table, int depth, int alpha, int beta, bool isMaximisingPlayer){

        if (checkDraw(table)) {
            return (0 - depth);
        } else if (checkWin(table)) {
            if (!isMaximisingPlayer) return (100 - depth);
            else return (-100 - depth);
        }
        
        const vector<vector<int>> freeSpaces = findFreeSpaces(table);
        
        if (isMaximisingPlayer) {
        
            int bestValue = -9999;
            
            for (int i = 0; i < freeSpaces.size(); i++) {
                
                const vector<vector<char>> tableBackup = table;
            
                table[freeSpaces[i][0]][freeSpaces[i][1]] = 'x';
                bestValue = max(bestValue, minimax(table, depth + 1, alpha, beta, !isMaximisingPlayer));
            
                table = tableBackup;
                
                alpha = max(alpha, bestValue);
                if (beta <= alpha) {
                    return bestValue;
                }
                
            }
            
            return bestValue;
            
        } else {
            
            int bestValue = 9999;
            
            for (int i = 0; i < freeSpaces.size(); i++) {
       
                vector<vector<char>> tableBackup = table;
            
                table[freeSpaces[i][0]][freeSpaces[i][1]] = 'o';
                bestValue = min(bestValue, minimax(table, depth + 1, alpha, beta, !isMaximisingPlayer));
            
                table = tableBackup;
                
                beta = min(beta, bestValue);
                if (beta <= alpha) {
                    return bestValue;
                }
                
            }
    
            return bestValue;
    
        }
        
    }
    
    vector<vector<int>> findFreeSpaces(vector<vector<char>> table) {
        vector<vector<int>> freeSpaces;

        for (int x = 0; x < 3; x++)
          for (int y = 0; y < 3; y++)
            if (table[y][x] == ' ')
              freeSpaces.push_back({y, x});
        
        return freeSpaces;
    }

    bool checkWin(vector<vector<char>> table) {
        for (int x = 0; x < 3; x++)
            if ((table[x][0] != ' ' && table[x][0] == table[x][1] && table[x][1] == table[x][2]) || (table[0][x] != ' ' && table[0][x] == table[1][x] && table[1][x] == table[2][x])) return true;

        if (table[1][1] != ' ' && ((table[0][0] == table[1][1] && table[1][1] == table[2][2]) || (table[0][2] == table[1][1] && table[1][1] == table[2][0]))) return true;

        return false;
    }

    bool checkDraw(vector<vector<char>> table) {
        for (int x = 0; x < 3; x++)
            for (int y = 0; y < 3; y++)
                if (table[y][x] == ' ') return false;

        return true;
    }

};

int main() {
    InputOutput inOut;
    Bot bot;

    do {
        Game game;

        inOut.clear();

        string mod = inOut.initPrint();

        do {
          //player1 turn
            inOut.clear();
            game.changeTurn();

            if (mod == "2") inOut.playerNum(game.turn);

            inOut.printTable(game.table);

            inOut.makeMove(game);

            if (game.checkFinish()) break;

          //player2 or bot turn
            inOut.clear();
            game.changeTurn();

            if (mod == "2") inOut.playerNum(game.turn);

            inOut.printTable(game.table);

            if (mod == "1") bot.makeMove(game);
            else inOut.makeMove(game);

        } while (!game.checkFinish());

        inOut.clear();
        inOut.printTable(game.table);

        if (game.checkDraw()) inOut.awardsVictory('d');
        else inOut.awardsVictory(game.turn);

    } while (inOut.askToPlayAgain());

    return inOut.lastPrint();
}