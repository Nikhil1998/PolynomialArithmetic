#include <bits/stdc++.h>
using namespace std;

#define ff first
#define ss second
#define pb push_back
#define mp make_pair

typedef vector<pair<int, int> > vpi;         // pair<degree, coefficient> of every power of x
ifstream fin;

void print_poly(vpi v)
{
    sort(v.begin(), v.end(), greater<pair<int, int> >());

    if(v.size() == 0)
        return;

    for(int i = 0; i < v.size() - 1; i++)
    {
        if(v[i].ss > 1)
            cout << v[i].ss;
            
        cout << "x^" << v[i].ff << " + ";
    }

    int l = v.size() - 1;
    if(v[l].ff == 0)
        cout << v[l].ss;

    else
    {
        if(v[l].ss > 1)
            cout << v[l].ss;
        cout << "x^" << v[l].ff;
    }
        
    cout << endl;
}

void ip_poly(vpi &a)
{
    int t;
    fin >> t;

    while(t--)
    {
        pair<int, int> p;

    //    cout << "Enter degree: ";
        fin >> p.ff;

    //    cout << "Enter coefficient: ";
        fin >> p.ss;

        a.pb(p);
    }
}

vpi add_poly(vpi a, vpi b, int p)
{
    int l1 = a.size(), l2 = b.size();
    int i1 = 0, i2 = 0;

    sort(a.begin(), a.end());
    sort(b.begin(), b.end());

    vpi ans;

    while(i1 < l1 and i2 < l2)
    {
        // if degree not equal then copy to answer and move forward
        if(a[i1].ff < b[i2].ff)
        {
            ans.pb(mp(a[i1].ff, a[i1].ss));
            i1++;
        }
        else if(a[i1].ff > b[i2].ff)
        {
            ans.pb(mp(b[i2].ff, b[i2].ss));
            i2++;
        }
        // if degree equal then add with mod. If 0 then ignore else copy to answer
        else
        {
            int t = (a[i1].ss + b[i2].ss) % p;
            if(t)
                ans.pb(mp(a[i1].ff, t));

            i1++;
            i2++;
        }  
    }

    // copy remaining elements 
    while(i1 < l1)
    {
        ans.pb(mp(a[i1].ff, a[i1].ss));
        i1++;
    }

    while(i2 < l2)
    {
        ans.pb(mp(b[i2].ff, b[i2].ss));
        i2++;
    }

    return ans;
}

vpi sub_poly(vpi a, vpi b, int p)
{
    // a - b

    int l1 = a.size(), l2 = b.size();
    int i1 = 0, i2 = 0;

    sort(a.begin(), a.end());
    sort(b.begin(), b.end());

    vpi ans;

    while(i1 < l1 and i2 < l2)
    {
        // if degree not equal then copy to answer and move forward
        if(a[i1].ff < b[i2].ff)
        {
            ans.pb(mp(a[i1].ff, a[i1].ss));
            i1++;
        }
        else if(a[i1].ff > b[i2].ff)
        {
            ans.pb(mp(b[i2].ff, p - b[i2].ss));
            i2++;
        }
        // if degree equal then add with mod. If 0 then ignore else copy to answer
        else
        {
            int t = (a[i1].ss - b[i2].ss + p) % p;
            if(t)
                ans.pb(mp(a[i1].ff, t));

            i1++;
            i2++;
        }  
    }

    // copy remaining elements 
    while(i1 < l1)
    {
        ans.pb(mp(a[i1].ff, a[i1].ss));
        i1++;
    }

    while(i2 < l2)
    {
        ans.pb(mp(b[i2].ff, p - b[i2].ss));
        i2++;
    }

    return ans;
}

vpi mul_poly(vpi a, vpi b, int p)
{
    vpi ans, temp;

    for(int i = 0; i < a.size(); i++)
    {
        temp = b;
        for(int j = 0; j < temp.size(); j++)
        {
            temp[j].ff += a[i].ff;      // adding degree
            temp[j].ss *= a[i].ss;      // multiplying coeff
        }

        ans = add_poly(ans, temp, p);
    }
    return ans;
}

void div_poly(vpi dd, vpi ds, vpi &q, vpi &r, int p)
{
    /*
    dd: dividend
    ds: divisor
    q: quotient
    r: remainder
    dd = q * ds + r
    */

    sort(dd.begin(), dd.end(), greater<pair<int, int> >());
    sort(ds.begin(), ds.end(), greater<pair<int, int> >());

    vpi cur = dd;
    r = cur;

    while(true)
    {
        if(cur[0].ff < ds[0].ff)
            return;

        int ddiff = cur[0].ff - ds[0].ff;    // degree difference
        int coeff = cur[0].ss;               // coefficient
        q.pb(mp(ddiff, coeff));

        vpi sub;
        for(int i = 0; i < ds.size(); i++)
        {
            sub.pb(mp(ds[i].ff + ddiff, ds[i].ss * coeff));
        }

        cur = sub_poly(cur, sub, p);

        sort(cur.begin(), cur.end(), greater<pair<int, int> >());

        r = cur;
        if(r.size() == 0)
            return;
    }
}

vpi extendedGCD(vpi a, vpi b, vpi &s, vpi &t, int p) 
{
    vpi rprev = a, r = b;
    vpi sprev, tprev;
    
    sprev.pb(mp(0, 1));
    t.pb(mp(0, 1));

    vpi temp;

    vpi qtemp, rtemp;

    int i = 0;

    while(r.size() != 0) 
    {
        i++;
        if(i == 5)
            break;

    //    cout << "Dividing "; print_poly(rprev); cout << " by "; print_poly(r);
        qtemp.clear();
        rtemp.clear();
        div_poly(rprev, r, qtemp, rtemp, p);
    //    cout << "Quotient "; print_poly(qtemp); cout << "Remainder "; print_poly(rtemp);

        vpi q = qtemp;
        
        // ri = r(i - 2) - q * r(i - 1)
        temp = rprev;
        rprev = r;
        r = sub_poly(temp, mul_poly(q, rprev, p), p);
    //    cout << "r becomes "; print_poly(r);
        
        // si = s(i - 2) - q * s(i - 1)
    //    cout << "s was "; print_poly(s);
        temp = sprev;
        sprev = s;
        s = sub_poly(temp, mul_poly(q, sprev, p), p);
    //    cout << "s becomes "; print_poly(s);
        
        // ti = t(i - 2) - q * t(i - 1)
    //    cout << "t was "; print_poly(t);
        temp = tprev;
        tprev = t;
        t = sub_poly(temp, mul_poly(q, tprev, p), p);
    //    cout << "t becomes "; print_poly(t);
    }
    s = sprev;
    t = tprev;

    return rprev;
}

int main() 
{
    fin.open("ip.txt");

    int p; 

//    cout << "Enter p: ";    
    fin >> p;

    int n;  
//    cout << "Enter n: ";    
    fin >> n;

    vpi a, b, mod, ip, q, r, s, t;

//    cout << "Enter A: ";
    ip_poly(a);

//    cout << "\nEnter B: ";
    ip_poly(b);

//    cout << "\nEnter Modulo: ";
    ip_poly(mod);

    cout << "\nPolynomial A is: ";  print_poly(a);
    cout << "\nPolynomial B is: ";  print_poly(b);
    cout << "\nPolynomial modulo is: ";    print_poly(mod);

    vpi mul = mul_poly(a, b, p);
    div_poly(mul, mod, q, r, p);
    cout << "\nMultiplication of "; print_poly(a);
    cout << "And "; print_poly(b);
    cout << "Modulo "; print_poly(mod);
    cout << "Is "; print_poly(r);

    vpi gcd = extendedGCD(a, mod, s, t, p);

    cout << "\nInverse of "; print_poly(a);
    cout << "Modulo: "; print_poly(mod);
    cout << "is "; print_poly(s);

    return 0;
}