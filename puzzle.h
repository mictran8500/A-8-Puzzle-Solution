#ifndef PUZZLE_H
#define PUZZLE_H

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

typedef struct comparePuzzle comparePuzzle;

class Puzzle
{
    
    public:
        Puzzle(vector<vector<int>> state);
        Puzzle(const Puzzle& rhs);

        size_t createHash();
        vector<vector<int>> getState();

        void calculatePath(const Puzzle* parent);
        void heuristicManhattan(const vector<vector<int>> goal); 
        void calculateEvaluation();
        int getEvaluation();
        void setParent(Puzzle* parent);

        int checkSolvable();
        bool checkGoal();
        void createPath();
        void printPuzzle();

        tuple<int, int> getLocation();
        bool validMove(const int direction, const int row, const int col);
        void swapTiles(const int row, const int col, const int new_row, const int new_col);
        Puzzle* createChild(const int direction, const int row, const int col);

    private:
        vector<vector<int>> state;
        int g_cost; //  Path cost
        int h_cost; //  Heuristic cost
        int f_cost; //  Evaluation cost

        //char move;  //  Movement of "blank" to get to the current state

        Puzzle* parent;
};

#endif
