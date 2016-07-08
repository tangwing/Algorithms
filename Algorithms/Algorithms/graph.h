#ifndef GRAPH_H
#define GRAPH_H
///////////////////
// This header defines Graph class and some commonly used graph algorithms
///////////////////
#include<vector>
#include <queue>
#include <iostream>
#include <stack>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <fstream>
#include <numeric>
using namespace std;
typedef pair<int, int> Edge;
//ostream& operator<<(ostream& o, const Edge &e){ o << e.first << "->" << e.second; return o; }
// Directed graph. Can be used as undirected graph by add double edges
class Graph
{
public:
    enum REP{ LIST, MATRIX };
    bool directed;
    int n, m;
    REP rep;
    vector<vector<int> > adj_l;
    vector<vector<short> > adj_m;
    vector<int> node_attr;
    map<Edge, int> edge_attr;   // Used when rep=LIST. e=(u,v), u<v for undirected graph

private:
    vector<int> topoOrder;

public:
    Graph(bool dire=true, REP r = REP::LIST) :directed(dire),rep(r){ n = m = 0; }
    Graph(int n, bool dire = true, REP r = REP::LIST) :n(n), directed(dire),rep(r)
    {
        if (r == LIST)
        {
            adj_l.resize(n);
        } else
        {
            for (int i = 0; i < n; i++) adj_m.push_back(vector<short>(n));    
        }
    }
    // To be implememnted by sub-class: undirected graph. NOT USED
    void hook_undirected_edge(int& src, int &dest){ if (!directed && src > dest) swap(src, dest); };
    // Assume src dest are valid, no edge is added twice and no self-cycle
    void addEdge(int src, int dest, int w=1)
    {
        hook_undirected_edge(src, dest);
        if (rep == LIST){
            adj_l[src].push_back(dest);
            setEdgeAttr(Edge(src, dest), w);
            if (!directed)adj_l[dest].push_back(src);
        }
        else
        {
            adj_m[src][dest] = w;
            if (!directed)adj_m[dest][src] = w;
        }
    }

    // Add several edges. No support for weight
    void addEdges(int src, vector<int> dests)
    {
        if (rep == LIST)
            for (auto d : dests){
                adj_l[src].push_back(d);
                if (!directed)adj_l[d].push_back(src);
            }
        else for (auto d : dests){
            adj_m[src][d] = 1;
            if (!directed)adj_m[d][src] = 1;
        }
    }

    void setNodeAttr(int idNode, int attr)
    {
        if (node_attr.empty()) node_attr.resize(n);
        node_attr[idNode] = attr;
    }

    void setNodeAttrs(int idNodeBeg, vector<int> attrs)
    {
        if (node_attr.empty()) node_attr.resize(n);
        for (int i = idNodeBeg; i < n; i++)
            node_attr[i] = attrs[i-idNodeBeg];
    }

    void setEdgeAttr(Edge e, int attr)
    {
        hook_undirected_edge(e.first, e.second);
        edge_attr.insert(make_pair(e, attr));
    }


    //////////////////////
    // Algorithms
    //////////////////////

    void printBFS(int s=0)
    {
        vector<bool> visited(n, false);
        queue<int> to_visit;
        to_visit.push(s);
        visited[s] = true;
        cout << "\nBFS: ";
explore:
        while (to_visit.empty() == false)
        {

            int curr = to_visit.front();
            if (rep == LIST){
                for (auto succ : adj_l[curr]) 
                    if (!visited[succ]) { to_visit.push(succ); visited[succ] = true; }
            } else
            {
                for (int succ = 0; succ < n; succ++) 
                    if (adj_m[curr][succ]!=0 && !(visited[succ])) {
                        to_visit.push(succ); visited[succ] = true;
                    }
            }
            to_visit.pop();
            cout << " "<< curr ;
        }
        for (int i = 0; i < n; i++)
            if (!visited[i])
            {

                to_visit.push(i);
                visited[i] = true;
                cout << "\nBFS: ";
                goto explore;
            }
        cout << endl;
    }

    // Assume graph connected
    bool hasCycle()
    {// Go dfs and if a node has child in stack -> cycle
        vector<short> state(n, -1);
        stack<int> to_visit;
        to_visit.push(0);
        state[0] = 0;       // 0 => in the stack: exploring
 component:
        while (!to_visit.empty())
        {
            int c = to_visit.top();
            bool explored = true;
            if (rep == LIST)
            {
                for (auto succ : adj_l[c])
                    if (state[succ] == 0){
                        topoOrder.clear();
                        return true;
                    }
                    else if (state[succ] == -1)
                    {
                        to_visit.push(succ);
                        state[succ] = 0;
                        explored = false;
                    }
            } else
            {
                for (int i = 0; i < n; i++)
                {
                    if (adj_m[c][i] !=0 )
                    {
                        if (state[i] == 0) {
                            topoOrder.clear();
                            return true;
                        }
                        else if (state[i] == -1)
                        {
                            to_visit.push(i);
                            state[i] = 0;
                            explored = false;
                        }
                    }
                }
            }
            if (explored)
            {
                state[c] = 1;
                to_visit.pop();
                topoOrder.insert(topoOrder.begin(), c);
            }
        }
        // Next connected component
        for (int i = 0; i < n; i++)
            if (state[i]==-1)
            {
                to_visit.push(i);
                state[i] = 0;
                cout << "\n New component: ";
                goto component;
            }
        return false;
    }

    // Topological sorting
    vector<int> topoSort()
    {
        if (hasCycle())
        {
            cout << "Graph has cycle. Return empty list.\n";
        }
        else cout << "Topo ordering : ";
        return topoOrder;
    }

    // Kruskal's algo for finding minimum spanning tree.
    vector<Edge> mstKruskal()
    {
        assert(!directed);          // Assume undirected graph, edge=(u, >u)
        assert(edge_attr.size()>0); // Assume weights are explicitly set
        vector <vector<Edge>> res;  // Each element is a connected subtree
        vector<Edge> sorted;
        // Get edges
        if (rep == LIST)
        {
            transform(edge_attr.begin(), edge_attr.end(), back_inserter(sorted), [&](pair<const Edge,int>& e)
            {
                return e.first;
            });

        } else
        {
            // Assuming undirected graph
            for (int i = 0; i < n; ++i)
                for (int j = i+1; j < n; ++j)
                    if (adj_m[i][j] != 0)
                        sorted.push_back(Edge(i, j));
        }
        // Sort edges by weight
        sort(sorted.begin(), sorted.end(), [&](Edge & e1, Edge & e2)->bool
        {
            return edge_attr[e1] < edge_attr[e2];
        });
        vector <set<int>> subtree_vs;   // vertex set corresponding to each subtree element in res. All sets are disjoint
        for(auto e : sorted)
        {
            int first_hit = -1;
            for (int i = 0; i < res.size(); ++i)
            {
                bool src_found = find(subtree_vs[i].begin(), subtree_vs[i].end(), e.first) != subtree_vs[i].end();
                bool dest_found = find(subtree_vs[i].begin(), subtree_vs[i].end(), e.second) != subtree_vs[i].end();
                if (src_found && dest_found){   // Both vertices are already hit
                    first_hit = i;
                    break;
                }
                if (src_found || dest_found)
                {
                    if (first_hit == -1)
                    {
                        first_hit = i;
                        // add the new edge to res[i]
                        res[i].push_back(e);
                        if (src_found)subtree_vs[i].insert(e.second);
                        else subtree_vs[i].insert(e.first);
                    }
                    else
                    {   // merging two sub-tree
                        res[first_hit].insert(res[first_hit].end(), res[i].begin(), res[i].end());
                        subtree_vs[first_hit].insert(subtree_vs[i].begin(), subtree_vs[i].end());
                        res.erase(res.begin() + i);
                        subtree_vs.erase(subtree_vs.begin() + i);
                        --i;
                    }
                }
            }
            if (first_hit == -1)
            {// New tree for e
                res.resize(res.size() + 1);
                res.back().push_back(e);
                subtree_vs.resize(subtree_vs.size() + 1);
                subtree_vs.back().insert(e.first);
                subtree_vs.back().insert(e.second);
            }
            if (res.size() == 1 && subtree_vs.front().size()==n)
                return res.front();
        }
        return res.front();
    }

    // Draw graph to a img file using GraphViz. Input: nodes/edges to be emphasised
    void drawGraph(set<int> nodes_em=set<int>(), set<Edge> edges_em=set<Edge>())
    {
        // Generate .gv file then call dot or neato
        string header = directed ? "digraph g{\n" : "graph g{\n";
        string edgeop = directed ? "->" : "--";
        auto label = [](int id){return char('a' + id); };
        auto edge_line = [&](Edge & e)
        {
            string res = string("") + label(e.first) + edgeop + label(e.second) + "[minlen=2";
            if (rep == LIST && edge_attr.empty() == false)
                res += ",label=" + to_string(edge_attr[e]) ;
            if (rep == MATRIX && adj_m[e.first][e.second]!=1)
                res += ",label=" + to_string(adj_m[e.first][e.second]) ;
            if (edges_em.find(e)!=edges_em.end())
                res += ",style=bold, color=red";
            return res + "];";
        };
        stringstream content;
        content << header;
        // Add edge
        if (rep == LIST)
        {
            for (int i = 0; i < n; ++i){
                if (adj_l[i].empty())continue;
                for (auto succ : adj_l[i])
                {
                    if (directed || i<succ)
                        content << edge_line(make_pair(i,succ))<<"\n";
                }
            }
        } else
        {
            for (int i = 0; i < n; ++i){
                int j = directed ? 0 : i + 1;
                for (; j < n; ++j)
                {
                    if (adj_m[i][j]!=0)
                    {
                        content << edge_line(make_pair(i,j))<<"\n";
                    }
                }
            }
        }
        // Add nodes with attr
        for (int i = 0; i < n; ++i)
        {
            content << label(i)<<"[fixedsize=true, width=0.3, height=0.3";
            if (nodes_em.find(i) != nodes_em.end())
                content << ",style=filled, fillcolor=gray";
            content << "];\n";
        }
        content << "\n}\n";
        
        // Save to file
        ofstream out("graph.gv");
        if (!out)
        {
            cerr << "Problem when writing to graph.gv !\n";
            return;
        }
        out << content.str();
        out.close();
        if (directed)
            system("dot -Tpng graph.gv -o graph.png && explorer graph.png");
        else system("neato -Tpng graph.gv -o graph.png && explorer graph.png");
    }

    // Single source shortest path by Bellman-Ford. O(VE)
    // Return: 1. true if no negative cycle detected. 2. The list of predecessors of each vertex
    pair<bool, vector<int>> bellmanFord(int s)
    {
        vector<int> prec(n, -1);
        vector<int> d(n, numeric_limits<int>::max());
        d[s] = 0;
        bool updating;
        for (int i = 0; i < n ; i++)
        {
            updating = false;
            // Relax each edge
            if (rep == LIST)
            {
                for (int j = 0; j < n; ++j)
                {
                    for (auto succ : adj_l[j])
                    {
                        int w = edge_attr.empty() ? 1 : edge_attr[make_pair(j, succ)];
                        if (d[j] + w<d[succ])
                        {
                            if (i == n - 1)return make_pair(false, prec);   // - cycle testing
                            d[succ] = d[j] + w;
                            prec[succ] = j;
                            updating = true;
                        }
                    }
                }
            } else
            {
                for (int j = 0; j < n; ++j)
                {
                    for (int k = 0; k < n; ++k)
                    {
                        if (adj_m[j][k]!=0 && d[j] + adj_m[j][k]<d[k])
                        {
                            if (i == n - 1)return make_pair(false, prec);  // - cycle testing
                            d[k] = d[j] + adj_m[j][k];
                            prec[k] = j;
                            updating = true;
                        }
                    }
                }
            }
            if (!updating) return make_pair(true, prec);
        }
        return make_pair(true, prec);
    }

    // Return the distance and path from s to t, computed on the list of precessors
    // Assuming the path can be found
    pair<int, vector<int> > getPathFromPrec(vector<int>& prec, int s, int t)
    {
        vector<int> res;
        int dist = 0;
        int c = t;
        res.push_back(c);
        while (c != s && res.size()<prec.size())
        {
            c = prec[c];
            res.push_back(c);
            int w;
            if (rep == LIST) w = (edge_attr.empty() ? 1: edge_attr[make_pair(c, res[res.size()-2])] );
            else w = adj_m[c][res[res.size() - 2]];
            dist += w;
        }
        assert(c == s); //Otherwise it means there is a cycle
        return make_pair(dist, vector<int>(res.rbegin(), res.rend()));
    }

    // Single source shortest path for DAG. O(V+E). Useful for PERT chart analysis for finding longest path: just negate weights
    // Return:1. true if no cycle detected. 2. The list of predecessors of each vertex
    pair<bool, vector<int>> dagShortestPath(int s)
    {
        vector<int> prec(n,-1);
        if (hasCycle()) return make_pair(false, prec);
        vector<int> d(n,numeric_limits<int>::max());
        d[s] = 0;
        for (auto it = find(topoOrder.begin(), topoOrder.end(), s); it != topoOrder.end(); ++it)
        {
            // For each edge leaving *it
            if (rep == LIST)
            {
                int j = *it;
                for (auto succ : adj_l[j])
                {
                    int w = edge_attr.empty() ? 1 : edge_attr[make_pair(j, succ)];
                    if (d[j] + w<d[succ])
                    {
                        d[succ] = d[j] + w;
                        prec[succ] = j;
                    }
                }
            }
            else
            {
                int j = *it;
                for (int k = 0; k < n; ++k)
                {
                    if (adj_m[j][k] != 0 && d[j] + adj_m[j][k]<d[k])
                    {
                        d[k] = d[j] + adj_m[j][k];
                        prec[k] = j;
                    }
                }
            }
        }
        return make_pair(true, prec);
    }

    // Assume all weights are non-negative. Using heap
    vector<int> dijkstra(int s)
    {
        vector<int> prec(n, -1);
        vector<int> dist(n, numeric_limits<int>::max());
        vector<int> q(n);
        iota(q.begin(), q.end(), 0);
        dist[s] = 0;
        int q_size = n;
        auto cmp = [&](int a, int b){ return dist[a] > dist[b]; };
        make_heap(q.begin(), q.begin() + q_size, cmp);
        while (q_size>0)
        {
            pop_heap(q.begin(), q.begin()+q_size, cmp);
            --q_size;
            // Current vertex is  q[q_size]: relax its edges
            if (rep == LIST)
            {
                int j = q[q_size];
                for (auto succ : adj_l[j])
                {
                    int w = edge_attr.empty() ? 1 : edge_attr[make_pair(j, succ)];
                    if (dist[j] + w<dist[succ])
                    {
                        dist[succ] = dist[j] + w;
                        prec[succ] = j;
                    }
                }
            }
            else
            {
                int j = q[q_size];
                for (int k = 0; k < n; ++k)
                {
                    if (adj_m[j][k] != 0 && dist[j] + adj_m[j][k]<dist[k])
                    {
                        dist[k] = dist[j] + adj_m[j][k];
                        prec[k] = j;
                    }
                }
            }
            make_heap(q.begin(), q.begin() + q_size, cmp);
        }
        return prec;
    }


    //////////////////////////////////////////////
    //
    // TESTS
    //
    //////////////////////////////////////////////
    static void test()
    {
        //testTopo();
        //testMST();
        //testBellmanFord();
        //testDAGshortestpath();
        testDijkstra();
    }
    static void testTopo()
    {
        Graph g(6);
        g.addEdges(0, { 1, 2 });
        g.addEdges(1, { 2, 3 });
        g.addEdges(3, { 4, 5 });
        g.printBFS();
        g.drawGraph();
        auto topo = g.topoSort();
        copy(topo.begin(), topo.end(), ostream_iterator<int>(cout, " "));
        cout << endl;
    }
    static void testMST()
    {
        // ItA figure 23.4
        Graph g(9, false);
        g.addEdge(0, 1, 4);
        g.addEdge(0, 7, 8);
        g.addEdge(1, 2, 8);
        g.addEdge(1, 7, 11);
        g.addEdge(2, 8, 2);
        g.addEdge(2, 3, 7);
        g.addEdge(2, 5, 4);
        g.addEdge(3, 4, 9);
        g.addEdge(3, 5, 14);
        g.addEdge(4, 5, 10);
        g.addEdge(5, 6, 2);
        g.addEdge(6, 7, 1);
        g.addEdge(6, 8, 6);
        g.addEdge(7, 8, 7);
        auto mst = g.mstKruskal();
        g.drawGraph(set<int>(), set<Edge>(mst.begin(), mst.end()));
        for_each(mst.begin(), mst.end(), [](Edge&e)
        {
            cout << char('a' + e.first) << "->" << char('a' + e.second) << endl;
        });
    }
    static void testBellmanFord()
    {
        // ItA Ex 24.1-1
        Graph g(5);
        g.addEdge(0, 1, 6);
        g.addEdge(0, 3, 7);
        g.addEdge(1, 2, 5);
        g.addEdge(1, 3, 8);
        g.addEdge(1, 4, -4);
        g.addEdge(2, 1, -2);
        g.addEdge(3, 2, -3);
        g.addEdge(4, 0, 2);
        g.addEdge(4, 2, 7); // Change the weight to 4 to have nega cycle
        g.drawGraph();
        auto bf = g.bellmanFord(0);
        assert(bf.first == true);
        auto wp = g.getPathFromPrec(bf.second, 0, 4);
        cout << "Shortest path (w="<<wp.first<<") : ";
        copy(wp.second.begin(), wp.second.end(), ostream_iterator<int>(cout, " "));
        cout << endl;
    }
    static void testDAGshortestpath()
    {
        // ItA Fig 24.5
        Graph g(6);
        g.addEdge(0, 1, 5);
        g.addEdge(0, 2, 3);
        g.addEdge(1, 2, 2);
        g.addEdge(1, 3, 6);
        g.addEdge(2, 3, 7);
        g.addEdge(2, 4, 4);
        g.addEdge(2, 5, 2);
        g.addEdge(3, 4, -1);
        g.addEdge(4, 5, -2);
        g.drawGraph();
        auto bf = g.dagShortestPath(1);
        assert(bf.first == true);
        auto wp = g.getPathFromPrec(bf.second, 1, 4);
        cout << "Shortest path (w=" << wp.first << ") : ";
        copy(wp.second.begin(), wp.second.end(), ostream_iterator<int>(cout, " "));
        cout << endl;
    }
    static void testDijkstra()
    {
        // ItA Fig 24.6
        Graph g(5);
        g.addEdge(0, 1, 10);
        g.addEdge(0, 3, 5);
        g.addEdge(1, 2, 1);
        g.addEdge(1, 3, 2);
        g.addEdge(2, 4, 4);
        g.addEdge(3, 1, 3);
        g.addEdge(3, 2, 9);
        g.addEdge(3, 4, 2);
        g.addEdge(4, 0, 7); 
        g.addEdge(4, 2, 6); 
        g.drawGraph();
        auto bf = g.dijkstra(0);
        auto wp = g.getPathFromPrec(bf, 0, 2);
        cout << "Shortest path (w=" << wp.first << ") : ";
        copy(wp.second.begin(), wp.second.end(), ostream_iterator<int>(cout, " "));
        cout << endl;
    }
};




// Store only left half of the matrix, query edge (a,b) with a>b
#endif