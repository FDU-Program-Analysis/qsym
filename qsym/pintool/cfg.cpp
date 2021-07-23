#include "cfg.h"

namespace qsym {

cfg::cfg(const std::string path)
  : path_(path) {

  if (path.empty()) {
    exit(-1);
  }

  std::ifstream ifs(path_, std::ios::in | std::ios::binary);
  if (ifs.fail()) {
    LOG_FATAL("cfg file is not available!\n");
    exit(-1);
  }

  bo::dynamic_properties dp(bo::ignore_other_properties);
  dp.property("node_id", get(&Vertex::name,    graph_));
  dp.property("label",   get(&Vertex::label,   graph_));
  dp.property("shape",   get(&Vertex::shape,   graph_));
  dp.property("comment", get(&Vertex::comment, graph_));
  dp.property("label",   get(&Edge::label,     graph_));

  if (!read_graphviz(ifs, graph_, dp)) {
    LOG_FATAL("cannot read cfg graph!\n");
    exit(-1);
  }

  LOG_INFO("Open CFG succeed!\n");
}


// cfg::~cfg() {}

} // namespace qsym