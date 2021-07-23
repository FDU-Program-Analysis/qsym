#ifndef QSYM_CFG_H_
#define QSYM_CFG_H_

#include "common.h"

#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <string>

namespace qsym {

namespace bo = boost;

/* Definations needed in using boost graph library */
struct Vertex {
  std::string name, label, shape, comment; // block address is stored in comment attribute
};

struct Edge {
  std::string label;
};

typedef bo::property<bo::graph_name_t, std::string> graph_p;
typedef bo::adjacency_list<bo::vecS, bo::vecS, bo::directedS,
                           Vertex, Edge> graph_t;
typedef bo::graph_traits<graph_t>::vertex_descriptor vertex_desc;
typedef bo::graph_traits<graph_t>::edge_descriptor edge_desc;
/* End definitions */

class CFG {

private:
  std::string path_;
  graph_t graph_;

public:
  CFG(const std::string path);
  //~cfg();

  // Check whether the node is stateful node
  bool isStateNode(vertex_desc vd);

  // Check whether the node is state-related node,
  // (i.e. the node has at least one path to a stateful node)
  bool isStateRelated(ADDRINT addr);

  // Find a node by address, return the vertex_desc of the node in CFG.
  // If there is no such node, return -1 represent this node is not exist.
  vertex_desc findNode(ADDRINT addr);
};

} // namespace qsym
#endif