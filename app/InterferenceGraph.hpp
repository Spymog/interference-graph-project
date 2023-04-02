#ifndef __INTERFERENCE_GRAPH__HPP
#define __INTERFERENCE_GRAPH__HPP

#include <exception>
#include <string>
#include <unordered_set>
#include <unordered_map>


class UnknownVertexException : public std::runtime_error {
public:
  UnknownVertexException(const std::string &v)
      : std::runtime_error("Unknown vertex " + v) {}
};

class UnknownEdgeException : public std::runtime_error {
public:
  UnknownEdgeException(const std::string &v, const std::string &w)
      : std::runtime_error("Unknown edge " + v + " - " + w) {}
};

// InterferenceGraph
//
// This is a class representing an interference graph
// as described in "Part 1: Interference Graph" of the README.md
// file.
template <typename T> class InterferenceGraph {
public:
  // Custom type used to represent edges. This is mainly
  // used in the utility function for reading and writing
  // the graph structure to/from files. You don't need to use it.
  using EdgeTy = std::pair<T, T>;

  InterferenceGraph();

  ~InterferenceGraph();

  void addEdge(const T &v, const T &w);

  void addVertex(const T &vertex) noexcept;

  void removeEdge(const T &v, const T &w);

  void removeVertex(const T &vertex);

  std::unordered_set<T> vertices() const noexcept;

  std::unordered_set<T> neighbors(const T &vertex) const;

  unsigned numVertices() const noexcept;

  unsigned numEdges() const noexcept;

  bool interferes(const T &v, const T &w) const;

  unsigned degree(const T &v) const;

private:
  // Private member variables here.

  std::unordered_map<T, std::unordered_set<T>> adjList;
  std::unordered_set<T> vertexList;

  unsigned int edgeCount = 0;

};

template <typename T> InterferenceGraph<T>::InterferenceGraph() {}

template <typename T> InterferenceGraph<T>::~InterferenceGraph() {}

template <typename T>
std::unordered_set<T> InterferenceGraph<T>::neighbors(const T &vertex) const {
  if (adjList.find(vertex) == adjList.end())
  {
    throw UnknownVertexException{vertex};
  }

  std::unordered_set<T> tempSet = adjList.at(vertex);

  return tempSet;
}

template <typename T>
std::unordered_set<T> InterferenceGraph<T>::vertices() const noexcept {
  return vertexList;
}

template <typename T>
unsigned InterferenceGraph<T>::numVertices() const noexcept {
  return vertexList.size();
}

template <typename T> unsigned InterferenceGraph<T>::numEdges() const noexcept {
  return edgeCount;
}

template <typename T>
void InterferenceGraph<T>::addEdge(const T &v, const T &w) 
{
  if (adjList.find(v) == adjList.end())
  {
    throw UnknownVertexException{v};
  }
  else if (adjList.find(w) == adjList.end())
  {
    throw UnknownVertexException{w};
  }

  if (adjList[v].find(w) == adjList[v].end())
  {
    adjList[v].insert(w);

  }
  if (adjList[w].find(w) == adjList[w].end())
  {
    adjList[w].insert(v);

  }
  edgeCount++;
}

template <typename T>
void InterferenceGraph<T>::removeEdge(const T &v, const T &w) 
{
  if (adjList.find(v) == adjList.end())
  {
    throw UnknownVertexException{v};
  }
  else if (adjList.find(w) == adjList.end())
  {
    throw UnknownVertexException{w};
  }

  if (adjList[v].find(w) == adjList[v].end())
  {
    throw UnknownEdgeException{v, w};
  }
  else if (adjList[w].find(v) == adjList[w].end())
  {
    throw UnknownEdgeException{v, w};
  }

  adjList[v].erase(w);
  adjList[w].erase(v);

  edgeCount--;
}

template <typename T>
void InterferenceGraph<T>::addVertex(const T &vertex) noexcept 
{
  if (vertexList.find(vertex) != vertexList.end())
  {
    return;
  }

  vertexList.emplace(vertex);
  std::unordered_set<T> newSet;
  adjList.emplace(vertex, newSet);
}

template <typename T>
void InterferenceGraph<T>::removeVertex(const T &vertex) 
{
  if (vertexList.find(vertex) == vertexList.end())
  {
    throw UnknownVertexException{vertex};
  }

  vertexList.erase(vertex);
  adjList.erase(vertex);
  for (auto i = adjList.begin(); i != adjList.end(); i++)
  {
    if (i->second.find(vertex) != i->second.end())
    {
      i->second.erase(vertex);
      edgeCount--;
    }
  }
}

template <typename T>
bool InterferenceGraph<T>::interferes(const T &v, const T &w) const 
{ 
  if (adjList.find(v) == adjList.end())
  {
    throw UnknownVertexException{v};
  }
  else if (adjList.find(w) == adjList.end())
  {
    throw UnknownVertexException{w};
  }
  return adjList.at(v).find(w) != adjList.at(v).end(); 
}

template <typename T>
unsigned InterferenceGraph<T>::degree(const T &v) const 
{ 
  if (adjList.find(v) == adjList.end())
  {
    throw UnknownVertexException{v};
  }
  return adjList.at(v).size(); 
}

#endif
