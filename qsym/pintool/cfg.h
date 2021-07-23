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
  std::string name, label, shape, comment; // addres is stored in comment attribute
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

class cfg {

private:
  std::string path_;
  graph_t graph_;

public:
  cfg(const std::string path);
  //~cfg();
};

} // namespace qsym
#endif