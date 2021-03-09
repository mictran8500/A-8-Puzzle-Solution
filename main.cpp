#include "puzzle.h"
#include <vector>
#include <iostream>
#include <unordered_map>
#include <tuple>
#include <queue>

using namespace std;

//  'Compare' argument for priority queue; compares evaluation functions for insertion
struct comparePuzzle
{
    bool operator()(Puzzle* lhs, Puzzle* rhs)
    {
        return lhs->getEvaluation() > rhs->getEvaluation();
    }
};

bool aStar(Puzzle* initial, const vector<vector<int>> goal)
{
    //  Base case - checks if the initial puzzle state is solvable
    if(initial->checkSolvable())
        return false;

    //  Priority queue created to store the frontier
    priority_queue<Puzzle*, vector<Puzzle*>, comparePuzzle> frontier;
    //  Map created to keep track of puzzle states that have been explored already
    unordered_map<size_t, vector<vector<int>>> explored;

    initial->heuristicManhattan(goal);
    initial->calculateEvaluation();
    frontier.push(initial);

    while(!frontier.empty())
    {
        Puzzle* current = frontier.top();
        frontier.pop();
        size_t key = current->createHash();
        explored[key] = current->getState();

        if(current->getState() == goal)
        {
            current->createPath();
            return true;
        }

        // Gets current location of 'blank' to check for valid movement and child creation
        const auto[row, col] = current->getLocation();
        for(int i = 0; i < 4; ++i)
        {
            if(current->validMove(i, row, col))
            {
                Puzzle* temp = current->createChild(i, row, col);

                //  Checks if the new puzzle state has already been explored;
                //  If not explored, pushes it onto the pq, and updates the explored map
                if(size_t temp_key = temp->createHash(); explored.find(temp_key) == explored.end())
                {
                    temp->calculatePath(current);
                    temp->heuristicManhattan(goal);
                    temp->calculateEvaluation();
                    temp->setParent(current);
                    frontier.push(temp);
                    explored[temp_key] = temp->getState();
                }
                else
                    delete temp;
            }
        }
    }
    return false;
}

int main()
{
    vector<vector<int>> goal = {{1,2,3}, {4,5,6}, {7,8,0}};
    vector<vector<int>> trial1 = {{2,1,4}, {0,5,3}, {8,7,6}};
    vector<vector<int>> trial2 = {{0,1,3}, {4,2,5}, {7,8,6}};
    vector<vector<int>> trial3 = {{1,3,4}, {8,6,2}, {0,7,5}};   // Unsolvable puzzle state

    bool check = false;
    Puzzle* initial = new Puzzle(trial1);
    check = aStar(initial, goal);
    
    if(!check)
        cout << "Puzzle is not solvable." << endl;

    delete initial;

    return 0;
}
