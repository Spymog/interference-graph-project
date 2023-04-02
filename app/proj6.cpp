#include "proj6.hpp"
#include "CSVReader.hpp"
#include "InterferenceGraph.hpp"
#include <utility>
#include <queue>
#include <vector>

using namespace proj6;

// assignRegisters
//
// This is where you implement the register allocation algorithm
// as mentioned in the README. Remember, you must allocate at MOST
// d(G) + 1 registers where d(G) is the maximum degree of the graph G.
// If num_registers is not enough registers to accomodate the passed in
// graph you should return an empty map. You MUST use registers in the
// range [1, num_registers] inclusive.

RegisterAssignment proj6::assignRegisters(const std::string &path_to_graph, int num_registers) noexcept 
{
  InterferenceGraph<Variable> ig = CSVReader::load(path_to_graph);

  RegisterAssignment assignments;
  std::priority_queue<std::pair<int, Variable>> wpVerticiesQueue;
  // unsigned int currentRegister = 1;


  std::unordered_set<Variable> tempVerticies = ig.vertices();

  for (auto i = tempVerticies.begin(); i != tempVerticies.end(); i++)
  {
    std::pair<int, Variable> newVertex (ig.degree(*i), *i);
    wpVerticiesQueue.push(newVertex);
  }

  int maxRegisters = wpVerticiesQueue.top().first + 1;


  // std::queue<int> availableRegisters;

  // for (int i = 1; i <= num_registers; i++)
  // {
  //   availableRegisters.push(i);
  // }


  // std::vector<Variable> wpVerticiesVector;
  // int i = 0;

  std::unordered_set<Variable> coloring;

  for (int i = 1; i <= maxRegisters; i++)
  {
    if (i > num_registers)
    {
      break;
    }
    std::priority_queue<std::pair<int, Variable>> tempQueue;
    while (!wpVerticiesQueue.empty())
    {
      std::pair<int, Variable> currentVertex = wpVerticiesQueue.top();
      wpVerticiesQueue.pop();
      std::unordered_set<Variable> adjacentVerticies = ig.neighbors(currentVertex.second);

      bool neighboring = false;

      for (auto vert = adjacentVerticies.begin(); vert != adjacentVerticies.end(); vert++)
      {
        if (coloring.find(*vert) != coloring.end())
        {
          neighboring = true;
          break;
        }
      }

      if (!neighboring)
      {
        coloring.emplace(currentVertex.second);
        assignments[currentVertex.second] = i;
      }
      else
      {
        tempQueue.push(currentVertex);
      }
      
     

    }
    wpVerticiesQueue = tempQueue;
    coloring.clear();
  }


  // while (currentRegister <= maxRegisters)
  // {
  //   Variable currentVertex = wpVerticiesQueue.top().second;
  //   // wpVerticiesVector.emplace(wpVerticiesVector.begin() + i, currentVertex);
  //   // i++;

  //   std::unordered_set<Variable> adjacentVerticies = ig.neighbors(currentVertex);
  //   for (auto i = adjacentVerticies.begin(); i != adjacentVerticies.end(); i++)
  //   {
  //     if (coloring.find(*i) != coloring.end())
  //     {
  //       continue;
  //     }
  //   }
    

  // }


  if (assignments.size() != tempVerticies.size())
  {
    return {};
  }
  return assignments;
}