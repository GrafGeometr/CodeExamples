#include <iostream>
#include <vector>
#include <queue>


using namespace std;


struct edge
{
    int to, cap, flow;
    edge () {};
    edge (int to, int cap, int flow):
        to(to), cap(cap), flow(flow){};
};
const int N = 10000;

int n;

int flow;
vector<edge> edges;
vector<int> g[N];
int rem(int x)
{
    return edges[x].cap - edges[x].flow;
}

int conj(int x)
{
    return x^1;
}

void add_edge(int x, int y, int w)
{
    g[x].push_back(int(edges.size()));
    edges.push_back(edge(y,w,0));
    g[y].push_back(int(edges.size()));
    edges.push_back(edge(x,0,0));
}

void info()
{
    cout<<"-----\n";
    for (int i=0; i<n; ++i)
    {
        cout<<i<<"\n";
        for (int e_ind: g[i])
        {
            cout<<e_ind<<" "<<edges[e_ind].to<<" "<<edges[e_ind].cap<<" "<<edges[e_ind].flow<<"\n";
        }
        cout<<"--------\n";
    }
}


bool find_way(int min_flow)
{
    //cout<<min_flow<<"\n";
    //s is g[0]
    //t is g[N-1]
    bool used[n];
    int parent[n];
    int parent_edge[n];
    queue <int> q; // pairs of vertexes
    q.push(0);
    for (int i=0; i<n; ++i) {parent[i]=-1;parent_edge[i]=-1;used[i]=false;}
    while (q.size())
    {
        int v = q.front();
        //cout<<v<<" ";
        q.pop();
        if (used[v]) continue;

        used[v] = true;
        for (auto e_ind: g[v])
        {
            if (rem(e_ind) >= min_flow && parent[edges[e_ind].to] == -1) {
                q.push(edges[e_ind].to);
                parent[edges[e_ind].to] = v;
                parent_edge[edges[e_ind].to] = e_ind;
            }
        }
    }
    //cout<<"\n";
    if (parent[n-1]==-1) return false;
    int min_rem = 10e9;
    int v = n - 1;
    while (v != 0)
    {
        min_rem = min(min_rem, rem(parent_edge[v]));
        v = parent[v];
    }

    v = n - 1;
    while (v != 0)
    {
        edges[parent_edge[v]].flow += min_rem;
        edges[conj(parent_edge[v])].flow -= min_rem;
        v = parent[v];
    }
    return true;
}

void set_max_flow()
{
    int max_max_flow = 1024*1024*1024;
    while (max_max_flow)
    {
        while (find_way(max_max_flow)){}
        max_max_flow /= 2;
    }
}

int get_flow()
{
    int res = 0;
    for (auto e_ind: g[0]) res += edges[e_ind].flow;
    return res;
}


int main()
{
    cin>>n;
    for (int i=0; i<n; ++i)
        for (int j=0; j<n; ++j)
        {
            int w;
            cin>>w;
            if (i!=j) add_edge(i, j, w);
        }
    //info();
    set_max_flow();
    cout<<get_flow();
    return 0;
}
