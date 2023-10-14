#include <iostream>
#include <random>
#include <string>
#include <time.h>

using namespace std;

typedef string str;

struct node
{
	int x, y;
	node* l;
	node* r;
	int sz=0;

	node (){};
};

typedef node* treap;
typedef pair<treap,treap> ptt;


node nodes[1000'000];

int cur=0;

treap newNode(int x, int y, treap l, treap r)
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


void show(treap a)
{
    if (!a) cout<<"NULL\n";
    else
    {
        cout<<"{\n";
        cout<<a->x<<" "<<a->y<<" "<<getSize(a)<<"\n";
        show(a -> l);
        show(a -> r);
        cout<<"}\n";
    }
}

treap fix(treap a)
{
	if (!a) return a;
	a -> sz = getSize(a ->l) + getSize(a -> r) + 1;
	return a;
}

treap push(treap a)
{
	if (!a) return a;
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

ptt split(treap a, int x)
{
    //cout<<"--------"<<x<<"\n";
    //show(a);
	a = push(a);
	if (!a) return make_pair(a, a);
	if (a -> x < x)
	{
		ptt p = split(a -> r, x);
		a -> r = p.first;
		return make_pair(fix(a),fix(p.second));
	}
	else
	{
		ptt p = split(a -> l, x);
		a -> l = p.second;
		return make_pair(fix(p.first),fix(a));
	}
}

treap add(treap a, int x, int y)
{
    //cout<<"--------"<<x<<" "<<y<<"\n";
    //show(a);
	a = push(a);
	//cout<<"1\n";
	if (!a) return fix(newNode(x, y, NULL, NULL));
	//cout<<"1\n";
	if (a -> y < y)
	{
	    //cout<<"1\n";
		ptt p = split(a, x);
		//cout<<"1\n";
		return fix(newNode(x, y, p.first, p.second));
	}
	if (a -> x < x)
	{
        a -> r = add(a -> r, x, y);
	}
	else
    {
        a -> l = add(a -> l, x, y);
    }
    return fix(a);
}


treap del(treap a, int x)
{
    a = push(a);
    if (!a) return a;
    if (a -> x == x) return concat(a -> l, a -> r);
    if (a -> x > x)
    {
        a -> l = del(a -> l, x);
        return fix(a);
    }
    else
    {
        a -> r = del(a -> r, x);
        return fix(a);
    }
}

treap getKth(treap a, int k, int &res)
{
    //cout<<"????\n";
    //cout<<k<<"\n";
    //show(a);
    a = push(a);
    if (getSize(a) < k)
    {
        res = -1;
        return a;
    }
    int lsz = getSize(a -> l);
    if (lsz == k){
        res = a -> x;
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
    mt19937 rnd(time(NULL));
    treap t = NULL;
    int n;
    cin>>n;
    for (int _=0; _<n; ++_)
    {
        str act;
        int x;
        cin>>act>>x;
        if (act=="+1")
            t = add(t, x, rnd());
        if (act=="-1")
            t = del(t, x);
        if (act=="0")
        {
            int res;
            //cout<<getSize(t)<<"\n";
            t = getKth(t, getSize(t)-x, res);
            cout<<res<<"\n";
        }
        //cout<<"------------------------------\n";
        //show(t);
    }
}
