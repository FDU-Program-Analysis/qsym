#include "cfg.h"

#include <queue>

namespace qsym {

CFG::CFG(const std::string path)
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

// CFG::~CFG() {}

bool CFG::isStateNode(vertex_desc vd) {
  std::string label = graph_[vd].label;
  if ((label.find("tran:") != std::string::npos) || 
      (label.find("check:") != std::string::npos)) {
    return true;
  }
  return false;  
}

bool CFG::isStateRelated(ADDRINT addr) {
  vertex_desc node = findNode(addr);

  // The cache is used for repeated queries.
  static unordered_map<ADDRINT, bool> cache_map_;
  auto map_it = cache_map_.find(addr);
  if (map_it != cache_map_.end()) return map_it->second;

  if (node == (vertex_desc)-1) {
    cache_map_.emplace(addr, true);
    return true;
  }

  // BFS search, if there is at least one path lead to stateful node,
  // the node is considerated as state-related node.
  std::queue<vertex_desc> q;
  std::unordered_set<edge_desc, edge_desc_hash> edge_mem_set;
  bo::graph_traits<graph_t>::out_edge_iterator out_i, out_end;
  edge_desc e;
  q.push(node);
  while (!q.empty()) {
    node = q.front();
    q.pop();
    for (bo::tie(out_i, out_end) = out_edges(node, graph_);
        out_i != out_end;
        ++out_i) {
      
      e = *out_i;
      if (edge_mem_set.find(e) != edge_mem_set.end()) continue;
      edge_mem_set.insert(e);

      vertex_desc dest = target(e, graph_);
      if (isStateNode(dest)) {
        cache_map_.emplace(addr, true);
        return true;
      }

      q.push(dest);
    }
  }

  cache_map_.emplace(addr, false);
  return false;
}

vertex_desc CFG::findNode(ADDRINT addr) {

  std::string addr_str = hexstr(addr);
  unsigned i = 0;
  for (i = 2; i < addr_str.length(); ++i) {
    if (addr_str[i] != '0') break; // remove leading zero
  }
  addr_str = "0x" + addr_str.substr(i);
  LOG_INFO("find addr: " + addr_str + "\n");

  bo::graph_traits<graph_t>::vertex_iterator vi, vi_end;
  for (bo::tie(vi, vi_end) = vertices(graph_);
        vi != vi_end;
        ++vi) {
      
    std::string cmt = graph_[*vi].comment;
    if (cmt == addr_str) return *vi;
  }

  return -1;
}


} // namespace qsym