#ifndef _GRAPH_HPP
#define _GRAPH_HPP

#include <boost/graph/graph_traits.hpp>
#include <boost/counting_iterator.hpp>
#include "property.hpp"

struct VectorS {
  template <class T> struct container {
	typedef DynamicArray<T> type;
  }
};

struct ListS {
  template <class T> struct container {
	typedef List<T> type;
  }
};


template <class G,E,C,V>
struct SourceEdges;

template <class G,E,C,V>
struct graph_traits<SourceEdges<G,E,C,V> > : public graph_traits<G> {
  typedef SourceEdges<G,E,C,V> graph;
  typedef E edge_descriptor;  
  typedef boost::counting_iterator_generator<graph::edge_desciptor> out_edge_iterator;
    typedef std::pair<
    typename graph_traits<Treexdcr<R> >::out_edge_iterator,
    typename graph_traits<Treexdcr<R> >::out_edge_iterator> pair_out_edge_it;  
};


// for simplicity, requires vertex index ... could allow user to specify property map type instead, but would have to allocate it themself

template <class E,class G,class F>
void visit(G &g,F f);

/*
if graphs had iterator_range
graph traits had (edge|vertex)::descriptor, (edge|vertex)::iterator
template <class E,class G,class F>
void visit(G &g,F f) {
  typedef typename E::iterator ei;
  std::pair<ei,ei> eis=iterator_range<E>(g);
  for (ei i=eis.first;i!=eis.second;++i)
  	f(*i);
}
*/

template <class G,class F>
void visit<typename graph_traits<G>::edge_descriptor>(G &g,F f) {
  typedef typename graph_traits<G>::edge_iterator ei;
  std::pair<ei,ei> eis=edges(g);
  for (ei i=eis.first;i!=eis.second;++i)
  	f(*i);
}

template <class G,class F>
void visit<typename graph_traits<G>::vertex_descriptor>(G &g,F f) {
  typedef typename graph_traits<G>::vertex_iterator ei;
  std::pair<ei,ei> eis=vertices(g);
  for (ei i=eis.first;i!=eis.second;++i)
  	f(*i);
}

// see property.hpp for factory
// must define visit_edges (although default above should be ok)
template <class G,class E=typename graph_traits<G>::edge_descriptor,class ContS=VectorS,
class VertMapFactory=property_factory<G,typename graph_traits<G>::vertex_descriptor>
>
struct SourceEdges {
  typedef SourceEdges<G,E,ContS,VertexIndexer> Self;
  typedef ContS::container<E>::type Adj;
  //typedef FixedArray<Adj> Adjs;  
  typedef typename graph_traits<G>::vertex_descriptor vertex_descriptor;
  typedef typename E edge_descriptor;
  graph &g;
  
  typedef G graph;  
  operator graph &() { return g; }
  typedef VertMapFactory
  typedef typename VertMapFactory::rebind<Adj>::implementation Adjs;
  Adjs adj;
  SourceEdges(Graph &g_,VertMapFactory vert_fact=VertMapFactory(g_)) : 
  g(g_),adj(vert_fact)
  {
	visit_edges<E>(g,*this);
  }
  Adj &operator[](vertex_descriptor v) {
    return adj[v];
  }
  const Adj &operator[](vertex_descriptor v) const {
    return *(const_cast<Self *>(this))[v];
  }
/*  template <class I>
  populate(I beg,I end) {
	for (I i=beg;i!=end;++i)
	  (*this)(*i);
  }*/
  void operator()(E e) {	
	adj[source(e,g)].push(e);
  }
};

template <class G,E,C,V>
inline typename SourceEdges<G,E,C,V>::pair_out_edge_it
out_edges(	  
          typename graph_traits<G>::vertex_descriptor v,
	      const SourceEdges<G,E,C,V> &g
	   )
{
  typedef typename SourceEdges<G,E,C,V> Self;
  typename Self::Adj &adj=g[v];
  return typename Self::pair_out_edge_it(adj.begin(),adj.end());
}

template <class G,E,C,V>
unsigned out_degree(typename SourceEdges<G,E,C,V>::vertex_descriptor v,SourceEdges<G,E,C,V> &g) {
  return g[v].size();
}

template <class G,E,C,V,F>
inline void
visit<E>(	  
          typename graph_traits<G>::vertex_descriptor v,
	      const SourceEdges<G,E,C,V> &g, 
          F f
	   )
{
  typedef typename SourceEdges<G,E,C,V> Self;
  typename Self::Adj &adj=g[v];
  for (typename Self::out_edge_iterator i=adj.begin(),e=adj.end();i!=e;++i)
    f(*i);  
}


#ifdef TEST
#include "test.hpp"
#endif

#ifdef TEST
BOOST_AUTO_UNIT_TEST( GRAPH )
{
}
#endif

#endif
