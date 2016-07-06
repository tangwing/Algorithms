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

    /////////////////////////////////////////////////
    //
    // Minimum spanning tree
    //
    /////////////////////////////////////////////////

    // Kruskal's algo for finding minimum spanning tree.
    // Assume undirected graph
    // Assume edge=(u, >u)
    // Assume weights are explicitly set
    vector<Edge> mstKruskal()
    {
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
                    if (adj_m[i][j] != 0 || adj_m[j][i] != 0)
                        sorted.push_back(Edge(i, j));
        }
        // Sort edges by weight
        sort(sorted.begin(), sorted.end(), [&](Edge & e1, Edge & e2)->bool
        {
            return edge_attr[e1] < edge_attr[e2];
        });
        vector <set<int>> subtree_vs;   // vertices corresponds to each element in res
        for(auto e : sorted)
        {
            int first_hit = -1;
            for (int i = 0; i < res.size(); ++i)
            {
                bool src_found = find(subtree_vs[i].begin(), subtree_vs[i].end(), e.first) != subtree_vs[i].end();
                bool dest_found = find(subtree_vs[i].begin(), subtree_vs[i].end(), e.second) != subtree_vs[i].end();
                if (src_found && dest_found){
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
                    {// merge res[i] to res[first_hit]: merging two sub-tree
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

    static void test()
    {
        Graph g(6);
        g.addEdges(0, { 1, 2 });
        g.addEdges(1, { 2, 3 });
        g.addEdges(3, { 4, 5 });
        g.printBFS();
        auto topo = g.topoSort();
        copy(topo.begin(), topo.end(), ostream_iterator<int>(cout, " "));
        cout << endl;

        // ItA figure 23.4
        g=Graph(9, false);
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
        for_each(mst.begin(), mst.end(), [](Edge&e)
        {
            cout << char('a' + e.first) << "->" << char('a' + e.second) << endl;
        });

    }
};




// Store only left half of the matrix, query edge (a,b) with a>b
#endif