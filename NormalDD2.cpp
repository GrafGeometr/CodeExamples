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

void show(treap a)
{
    if (!a) cout<<"NULL\n";
    else
    {
        cout<<"{\n";
        cout<<a->x<<" "<<a->y<<" "<<getSize(a)<<" "<<getSum(a)<<"\n";
        show(a -> l);
        show(a -> r);
        cout<<"}\n";
    }
}

treap fix(treap a)
{
	if (!a) return a;
	a -> sz = getSize(a -> l) + getSize(a -> r) + 1;
	a -> sum = a -> x + getSum(a -> l) + getSum(a -> r);
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

ptt split(treap a, ll x)
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

treap add(treap a, ll x, int y)
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


treap del(treap a, ll x)
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

treap getKth(treap a, int k, ll &res)
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

treap getNotMoreSum(treap a, ll mx, ll &res)
{
    a = push(a);
    if (!a) return a;
    //cout<<a->x<<"-009-09\n";
    if (a -> x > mx)
    {
        a -> l = getNotMoreSum(a -> l, mx, res);
        return fix(a);
    }
    else
    {
        res += a -> x;
        res += getSum(a -> l);
        a -> r = getNotMoreSum(a -> r, mx, res);
        return fix(a);
    }
}

treap getRangeSum(treap a, ll mn, ll mx, ll &res)
{
    ll r1 = 0, r2 = 0;
    a = getNotMoreSum(a, mn-1, r1);
    a = getNotMoreSum(a, mx, r2);
    //cout<<r1<<" "<<r2<<"\n";
    res = r2-r1;
    return a;
}

int main()
{
    mt19937 rnd(time(NULL));
    set <ll> s;
    ll m=1000'000'000;
    treap t = NULL;
    int n;
    cin>>n;
    ll wtf;
    bool last_q = false;
    for (int _=0; _<n; ++_)
    {
        str act;
        cin>>act;
        if (act=="+")
        {
            ll i;
            cin>>i;
            if (last_q)
                i=(i+wtf)%m;
            if (s.find(i)==s.end())
            {
                s.insert(i);
                t = add(t, i, rnd());
            }
            last_q = false;
        }
        if (act=="?")
        {
            ll l,r;
            cin>>l>>r;
            ll res = 0;
            t = getRangeSum(t, l, r, res);
            wtf=res;
            last_q=true;
            cout<<res<<"\n";
        }
        //cout<<"---------------------\n";
        //show(t);
    }
}
