#ifndef INCLUDE_DAG_NETWORKS_HPP_
#define INCLUDE_DAG_NETWORKS_HPP_

#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "static_edges.hpp"
#include "temporal_edges.hpp"

namespace dag {
  /**
    Generic network class, storing a set of edges with fast access to (in- and
    out-) incident edges to each node, neighbours and degrees.

    @tparam EdgeT Edge type for this network. Different edge types encapsulate
    different behaviour in networks, e.g. a directed network is a network with
    directed edges.
   */
  template <class EdgeT>
  class network {
  public:
    /**
      Type used for labelling vertices, derived from EdgeType of network.
     */
    using VertexType = typename EdgeT::VertexType;

    /**
      Type of the edges in the network.
     */
    using EdgeType = EdgeT;

    network() = default;
    /**
      Create a network from a vector of edges.
     */
    explicit network(const std::vector<EdgeT>& edges);

    /**
      list of unique vertices participating at least in one event in the
      network sorted by operator<.
     */
    [[nodiscard]]
    std::vector<VertexType> vertices() const;

    /**
      List of unique edges in the network sorted by operator<.
     */
    [[nodiscard]]
    const std::vector<EdgeType>& edges() const;

    /**
      List of unique edges in the network sorted by operator<.
     */
    [[nodiscard]]
    const std::vector<EdgeType>& edges_cause() const;

    /**
      List of unique edges in the network sorted by effect_lt.
     */
    [[nodiscard]]
    const std::vector<EdgeType>& edges_effect() const;

    /**
      List of edges in network incident to `vert`, i.e. 'vert' is mutated by
      them. Edges are sorted by `effect_lt(e1, e2)`.
     */
    [[nodiscard]]
    std::vector<EdgeType> in_edges(const VertexType& vert) const;

    /**
      List of edges in network which `vert` is incident to, i.e. where 'vert' is
      a mutator of. Edges are sorted by `operator<(e1, e2)`.
     */
    [[nodiscard]]
    std::vector<EdgeType> out_edges(const VertexType& vert) const;

    /**
      Map of vertices and edges in network that are incident to them, i.e. 'vert' is mutated by
      them. Edges are sorted by `effect_lt(e1, e2)`.
     */
    [[nodiscard]]
    const std::unordered_map<VertexType, std::vector<EdgeType>>&
    in_edges() const;

    /**
      Map of vertices and edges in network which they are incident to, i.e. where 'vert' is
      a mutator of. Edges are sorted by `operator<(e1, e2)`.
     */
    [[nodiscard]]
    const std::unordered_map<VertexType, std::vector<EdgeType>>&
    out_edges() const;

    /**
      List of edges in network which `vert` is a participant, i.e. where 'vert'
      is a mutator of or is mutated by that edge. Edges are sorted by
      `operator<(e1, e2)`.
     */
    [[nodiscard]]
    std::vector<EdgeType> incident_edges(const VertexType& vert) const;

    /**
      Number of edges incident to `vert`. Similart to `in_edges(vert).size()`
     */
    [[nodiscard]]
    size_t in_degree(const VertexType& vert) const;

    /**
      Number of edges that `vert` is incident to. Similart to
      `out_edges(vert).size()`
     */
    [[nodiscard]]
    size_t out_degree(const VertexType& vert) const;

    /**
      Number of edges that `vert` participates in. Similart to
      `incident_edges(vert).size()`
     */
    [[nodiscard]]
    size_t degree(const VertexType& vert) const;



    /**
      List of vertices that are mutators in at least one edge where 'v' is
      mutated.
     */
    [[nodiscard]]
    std::vector<VertexType> predecessors(const VertexType& v) const;

    /**
      List of vertices that are mutated in at least one edge where 'v' is a
      mutator.
     */
    [[nodiscard]]
    std::vector<VertexType> successors(const VertexType& v) const;

    /**
      List of vertices that participate in at least one edge with 'v'.
     */
    [[nodiscard]]
    std::vector<VertexType> neighbours(const VertexType& v) const;

  private:
    std::vector<EdgeType> _edges_cause;
    std::vector<EdgeType> _edges_effect;
    std::unordered_map<VertexType, std::vector<EdgeType>> _in_edges;
    std::unordered_map<VertexType, std::vector<EdgeType>> _out_edges;

    static constexpr bool instantaneous_undirected =
      is_instantaneous_v<EdgeType> && is_undirected_v<EdgeType>;
  };

  /**
    Directed network class. network class with directed edges.
   */
  template <class VertT>
  using directed_network = network<directed_edge<VertT>>;

  /**
    Undirected network class. network class with undirected edges.
   */
  template <class VertT>
  using undirected_network = network<undirected_edge<VertT>>;

  /**
    Directed temporal network class. network class with directed temporal edges.
   */
  template <class VertT, class TimeT>
  using directed_temporal_network =
    network<directed_temporal_edge<VertT, TimeT>>;

  /**
    Directed delayed temporal network class. network class with directed delayed
    temporal edges.
   */
  template <class VertT, class TimeT>
  using directed_delayed_temporal_network =
    network<directed_delayed_temporal_edge<VertT, TimeT>>;

  /**
    Undirected temporal network class. network class with undirected temporal
    edges.
   */
  template <typename VertT, typename TimeT>
  using undirected_temporal_network =
    network<undirected_temporal_edge<VertT, TimeT>>;
}  // namespace dag

#include "../../src/networks.tpp"

#endif  // INCLUDE_DAG_NETWORKS_HPP_
