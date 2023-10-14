#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;

const int N=10000;
const int INF = 1024*1024*1024;

vector<int> g[N];

vector<int> inv_g[N];

vector<int> order;
bool used[N];

void topsort(int v)
{
	if (used[v]) return;
	used[v]=true;
	for (int u: g[v]) topsort(u);
	order.push_back(v);
}

void do_topsort(int n)
{
	for (int i=0;i<n;++i) used[i]=false;
	for (int v=0;v<n;++v)
		if (! used[v]) topsort(v);
	reverse(order.begin(), order.end());
}

void build_rev(int n)
{
	for (int v=0; v<n; ++v)
        for (int u: g[v]) inv_g[u].push_back(v);
}

int comp[N];

void dfs2(int v, int c)
{
	if (comp[v]!=-1) return;
	comp[v]=c;
	for (int u: inv_g[v]) dfs2(u, c);
}

void find_strong_comps(int n)
{
    do_topsort(n);

	reverse(order.begin(), order.end());
	for (int i=0; i<n;++i) comp[i]=-1;

	build_rev(n);

	int c=0;
	for (int v: order)
    {
        if (comp[v]==-1)
        {
            dfs2(v,c);
            c++;
        }
    }
    //for (int i=0; i<n; ++i)
    //    cout<<comp[i]<<" ";
    //cout<<"\n";

}

//2-SAT

int get_true(int x)
{
    return 2*x;
}

int get_false(int x)
{
    return 2*x+1;
}

int conj(int x)
{
    return x^1;
}

void add_or(int x, int y)
{
    g[conj(x)].push_back(y);
    g[conj(y)].push_back(x);
}


int moves=0;
bool how_to[N];

bool find_2_sat(int n)
{
    find_strong_comps(2*n);
    for (int x=0; x<n;++x)
    {
        if (comp[get_true(x)]==comp[get_false(x)]) return false;
        if (comp[get_true(x)]<comp[get_false(x)]) how_to[x]=false;
        else
        {
            moves++;
            how_to[x]=true;
        }
    }
}

void info(int n)
{
    for (int i=0;i<2*n;++i)
    {
        cout<<"____"<<i<<"\n";
        for (int u:g[i]) cout<<u<<" ";
        cout<<"\n";
    }
}

int main()
{
    int n,m;
    cin>>n>>m;
    for (int i=0; i<m; ++i)
    {
        int x,y,a;
        cin>>x>>y>>a;
        x--;
        y--;
        if (a)
        {
            add_or(get_false(x),get_true(y));
            add_or(get_true(x),get_false(y));
        } else
        {
            add_or(get_true(x),get_true(y));
            add_or(get_false(x),get_false(y));
        }
    }
    //info(n);
    if (find_2_sat(n))
    {
        cout<<moves<<"\n";
        for (int i=0; i<n;++i)
        {
            if (how_to[i]) cout<<i+1<<" ";
        }
    } else cout<<"Impossible";

	return 0;
}
