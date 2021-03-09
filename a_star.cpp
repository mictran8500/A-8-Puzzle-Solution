#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <stdlib.h>
#include "puzzle.h"

Puzzle::Puzzle(vector<vector<int>> state)
{
    this->state = state;
    g_cost = 0;
    h_cost = 0;
    f_cost = 0;
    parent = nullptr;
}

Puzzle::Puzzle(const Puzzle& rhs)
{
    this->state = rhs.state;
    g_cost = 0;
    h_cost = 0;
    f_cost = 0;
    parent = nullptr;
}

// Returns a hash key using the puzzle's current state
size_t Puzzle::createHash()
{
    string value;
    for(size_t i = 0; i < state.size(); ++i)
    {
        for(size_t j = 0; j < state[i].size(); ++j)
        {
            value += to_string(state[i][j]);
        }
    }
    
    hash<string> str_hash;
    return str_hash(value);

}

vector<vector<int>> Puzzle::getState()
{
    return this->state;
}

void Puzzle::calculatePath(const Puzzle* parent)
{
    this->g_cost = parent->g_cost +1;
}

// Heuristic function that calculates the total manhattan distance for all cells in the puzzle
void Puzzle::heuristicManhattan(const vector<vector<int>> goal)
{
    size_t i, j, k, l;
    int dist = 0;
    int total = 0;
    int flag;

    for(i = 0; i < state.size(); ++i)
    {
        for(j = 0; j < state[i].size(); ++j)
        {
            if(state[i][j] != goal[i][j])
            {
                int temp = state[i][j];
                for(k = 0; k < state.size(); ++k)
                {
                    for(l = 0; l < state[k].size(); ++l)
                    {
                        if(temp == goal[k][l])
                        {
                            flag = 1;
                            break;
                        }
                    }
                    if(flag == 1)
                        break;
                }

                //  Must type convert because abs() does not work with type_t
                int row_i = static_cast<int>(i);
                int row_j = static_cast<int>(j);
                int row_k = static_cast<int>(k);
                int row_l = static_cast<int>(l);

                dist = abs(row_i - row_k) + abs(row_j - row_l);
                total += dist;
            }
        }
    }

    this->h_cost = total;
}

void Puzzle::calculateEvaluation()
{
    this->f_cost = this->g_cost + this->h_cost;
}

int Puzzle::getEvaluation()
{
    return this->f_cost;
}

void Puzzle::setParent(Puzzle* parent)
{
    this->parent = parent;
}

//  Checks if the initial puzzle state is solvable using the inversion count of the puzzle
//  If the inversion count is of a different parity than the parity of the goal state,
//  then the puzzle is not solvable.
int Puzzle::checkSolvable()
{
    int count = 0;
    vector<int> temp;
    
    for(size_t i = 0; i < state.size(); ++ i)
    {
        for(size_t j = 0; j < state[i].size(); ++j)
        {
            temp.push_back(state[i][j]);
        }
    }

    for(size_t i = 0; i < temp.size(); ++i)
    {
        if(temp[i] != 0)
        {
            for(size_t j = i; j < temp.size(); ++j)
            {
                if(temp[j] < temp[i] && temp[j] != 0)
                    ++count;
            }
        }
    }

    return count%2;
}

void Puzzle::createPath()
{
    int size;
    vector<Puzzle*> goal_path;
    Puzzle* temp = this;

    while(temp != nullptr)
    {
        goal_path.push_back(temp);
        temp = temp->parent;
    }

    size = goal_path.size();
    cout << "Number of steps: " << size-1 << endl;
}

//  Finds and returns current location of 'blank' tile,
//  returns tuple with 404s if not found
tuple<int, int> Puzzle::getLocation()
{
    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            if(state[i][j] == 0)
                return make_tuple(i, j);
        }
    }

    return make_tuple(404, 404);
}

//  Checks that movement of 'blank' tile does not go out of bounds
bool Puzzle::validMove(int direction, int row, int col)
{
    bool flag = false;

    switch(direction)
    {
        case 0:
            flag = row - 1 >= 0;
            break;
        case 1:
            flag = row + 1 <= 2;
            break;
        case 2:
            flag = col - 1 >= 0;
            break;
        case 3:
            flag = col + 1 <= 2;
            break;
    }

    return flag;
}

void Puzzle::swapTiles(const int row, const int col, const int new_row, const int new_col)
{
    swap(state[row][col], state[new_row][new_col]);
}

//  Creates child; modifies the state to accurately represent the swap
Puzzle* Puzzle::createChild(const int direction, const int row, const int col)
{
    Puzzle* temp = new Puzzle(state);
    switch(direction)
    {
        // Up
        case 0:
            temp->swapTiles(row, col, row-1, col);
            break;
        // Down
        case 1:
            temp->swapTiles(row, col, row+1, col);
            break;
        // Left
        case 2:
            temp->swapTiles(row, col, row, col-1);
            break;
        // Right
        case 3:
            temp->swapTiles(row, col, row, col+1);
            break;
    }
    return temp;
}


