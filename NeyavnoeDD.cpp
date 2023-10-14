#include <iostream>
#include <random>
#include <string>
#include <set>
#include <time.h>

using namespace std;



typedef string str;
typedef long long ll;

struct node
{
	ll x, y;
	node* l;
	node* r;
	ll sz=0;
	ll sum=0;
	ll f=0;
	bool nrev=false;

	node (){};
};

typedef node* treap;
typedef pair<treap,treap> ptt;


node nodes[1000'000];

int cur=0;

treap newNode(ll x, int y, treap l, treap r)
{
	nodes[cur].x=x;
	nodes[cur].y=y;
	nodes[cur].l=l;
	nodes[cur].r=r;
	return &nodes[cur++];
}

int getSize(treap a)
{
    if (!a) return 0;
    return a -> sz;
}

ll getSum(treap a)
{
    if (!a) return 0;
    return a -> sum;
}

ll getVal(treap a)
{
    return a -> x + a -> f;
}

void show(treap a)
{
    if (!a) cout<<"NULL\n";
    else
    {
        cout<<"{\n";
        cout<<a->x<<" "<<a->y<<" "<<getSize(a)<<" "<<getSum(a)<<" "<<a -> f<<" "<<a -> nrev<<"\n";
        show(a -> l);
        show(a -> r);
        cout<<"}\n";
    }
}

treap fix(treap a)
{
	if (!a) return a;
	a -> sz = getSize(a -> l) + getSize(a -> r) + 1;
	a -> sum = getVal(a) + getSum(a -> l) + getSum(a -> r);
	return a;
}

treap push(treap a)
{
	if (!a) return a;
	if (a -> l) a -> l -> f += a -> f;
	if (a -> r) a -> r -> f += a -> f;
	a -> x += a -> f;
	a -> f = 0;
	if (a -> nrev)
    {
        swap(a -> l, a -> r);
        if (a -> l) a -> l -> nrev = !(a -> l -> nrev);
        if (a -> r) a -> r -> nrev = !(a -> r -> nrev);
        a -> nrev = false;
    }
	return a;
}

treap concat(treap a, treap b)
{
	a = push(a);
	b = push(b);
	if (!a) return b;
	if (!b) return a;
	if (a -> y > b -> y)
	{
		a -> r = concat(a -> r, b);
		return fix(a);
	}
	else
	{
		b -> l = concat(a, b -> l);
		return fix(b);
	}
}

ptt split(treap a, ll k) // takes k elems
{
	a = push(a);
	if (getSize(a) <= k) return make_pair(a, (treap)NULL);
	int lsz = getSize(a -> l);
	if (lsz < k)
	{
		ptt p = split(a -> r, k - lsz - 1);
		a -> r = p.first;
		return make_pair(fix(a), fix(p.second));
	}
	else
	{
		ptt p = split(a -> l, k);
		a -> l = p.second;
		return make_pair(fix(p.first), fix(a));
	}
}

treap add(treap a, ll x, int y, int k)
{
	a = push(a);
	if (!a) return fix(newNode(x, y, NULL, NULL));
	if (a -> y < y)
	{
		ptt p = split(a, k);
		return fix(newNode(x, y, p.first, p.second));
	}
	int lsz = getSize(a -> l);
	if (lsz < k)
	{
        a -> r = add(a -> r, x, y, k);
	}
	else
    {
        a -> l = add(a -> l, x, y, k);
    }
    return fix(a);
}


treap del(treap a, int k)
{
    a = push(a);
    if (!a) return a;
    int lsz = getSize(a -> l);
    if (lsz == k) return concat(a -> l, a -> r);
    if (lsz > k)
        a -> l = del(a -> l, k);
    else
        a -> r = del(a -> r, k);
    return fix(a);
}

treap getKth(treap a, int k, ll &res)
{
    a = push(a);
    if (getSize(a) < k)
    {
        res = -1;
        return a;
    }
    int lsz = getSize(a -> l);
    if (lsz == k){
        res = getVal(a);
        return fix(a);
    }
    if (lsz > k)
    {
        a -> l =  getKth(a -> l, k, res);
        return fix(a);
    }
    if (lsz < k)
    {
        a -> r = getKth(a -> r, k - lsz - 1, res);
        return fix(a);
    }
}



int main()
{
    ios_base :: sync_with_stdio(false);
    cin.tie(0);

    mt19937 rnd(time(NULL));
    //mt19937 rnd(16);
    treap t = NULL;
    int n, m;
    cin>>n>>m;
    for (int i=0; i<n ;++i)
    {
        int r = rnd();
        int x;
        cin>>x;
        t = add(t, x, r, i);
    }
    //show(t);
    for (int i=0; i<m; ++i)
    {
        int q,l,r;
        cin>>q>>l>>r;
        ptt p1 = split(t, l-1);
        ptt p2 = split(p1.second, r-l+1);
        treap mid = p2.first;
        if (q)
            mid -> nrev = true;
        else
        {
            cout << mid -> sum << "\n";
        }
        t = concat(p1.first, concat(mid, p2.second));
        //show(t);
    }
}
