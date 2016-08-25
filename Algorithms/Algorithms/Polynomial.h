#ifndef  POLYNOMIAL_H
#define  POLYNOMIAL_H
#include <set>
#include <map>
#include <iostream>
#include <string>
#include <utility>
using namespace std;
#define TARGETSIZE 6
// This header file models polynomial and its items. 
// It's originally created for improving Strassen's algorithm (cf CLRS P80)

// An item can have 1 constant factor and 2 unknown factors: a*b. Each factor is represented by an id (must >0, < 2 bytes)
class Item
{
 public:
     typedef int FactorType;
     int c;
     FactorType f1, f2;
    static const int ABSENT = 0;
    Item(int s, int _1, int _2 = ABSENT) : c(s), f1(_1), f2(_2)
    {
        if (f2!=ABSENT && f1 > f2) swap(f1, f2);
    }
    
    // Two single items can multiply
    Item operator*(const Item & i2)const{
        if (f2 != ABSENT || i2.f2 != ABSENT)
            throw "Only two single items can multiply";
        return Item(c*i2.c, f1, i2.f1);
    }

    // only designed for keep an order
    bool operator < (const Item & i)const{
        return get_id() < i.get_id();
    }
    bool operator == (const Item & i)const{
        return c==i.c && f1==i.f1 && f2==i.f2;
    }

    int get_id()const 
    {
        //if (f1 > f2) swap(f1, f2);
        return (f1 << 16) + f2;
    }
};

// A polynomial contains a set of items
class Polynomial
{
public:
    set<Item> p;
    static map<const Item::FactorType, string> item_names;
    
    Polynomial(){};
    Polynomial(const Item& i1)
    {
        add_item(i1);
    }
    Polynomial(const Item& i1, const Item & i2)
    {
        add_item(i1);
        add_item(i2);
    }
    void add_item(const Item & i)
    {
        if (i.c == 0) return;
        auto res = p.find(i);
        if (res == p.end())
            p.insert(i);
        else {
            Item copy = *res;
            p.erase(res);
            copy.c += i.c;
            if (copy.c!=0) p.insert(copy);
        }
    }

    Polynomial operator * ( const Polynomial & p2)const{
        Polynomial res;
        for (auto& i : p)
            for (auto &j: p2.p)
            {
                res.add_item(i*j);
            }
        return res;
    }
    Polynomial operator * (const Item & i) const{
        return (*this)*Polynomial(i);
    }
    Polynomial operator * (int t) const{
        Polynomial res;
        if (t!=0)
            transform(p.begin(), p.end(), inserter(res.p,res.p.begin()), [t](Item i){ i.c *= t;  return i; });
        return res;
    }
    Polynomial operator+(const Polynomial & po)const {
        Polynomial res(*this);
        for (auto & i : po.p)
            res.add_item(i);
        return res;
    }
    Polynomial operator-(const Polynomial & po)const {
        return *this + (po*(-1));
    }
    bool operator ==(const Polynomial & po)const{
        return p == po.p;
    }

    void print( map<const Item::FactorType, string> & namemap = item_names)
    {
        for (auto & i : p)
        {
            cout << ((i.c > 0) ? "+" : "")
                << ((i.c == 1) ? "" : to_string(i.c))
                << namemap[i.f1]
                << ((i.f2 == Item::ABSENT) ? "" : namemap[i.f2]);
        }
        cout<<endl;
    }

    static vector<Polynomial> & S_strassen()
    {
        static vector<Polynomial> s={
            Polynomial(Item(1, 212), Item(-1, 222)),
            Polynomial(Item(1, 111), Item(1, 112)),
            Polynomial(Item(1, 121), Item(1, 122)),
            Polynomial(Item(1, 221), Item(-1, 211)),
            Polynomial(Item(1, 111), Item(1, 122)),
            Polynomial(Item(1, 211), Item(1, 222)),
            Polynomial(Item(1, 112), Item(-1, 122)),
            Polynomial(Item(1, 221), Item(1, 222)),
            Polynomial(Item(1, 111), Item(-1, 121)),
            Polynomial(Item(1, 211), Item(1, 212)),
        };
        return s;
    }

    static vector<Polynomial> & create_s()
    {
        // Create Ss
        int As[] = { 111, 112, 121, 122 };
        int Bs[] = { 211, 212, 221, 222 };
        static vector<Polynomial> Ss;
        for (int i = 0; i < 4; ++i)
            for (int j = i + 1; j < 4; ++j)
            {
                Polynomial p;
                p.add_item(Item(1, As[i]));
                p.add_item(Item(1, As[j]));
                Ss.push_back(p);
                p.add_item(Item(-2, As[j]));
                Ss.push_back(p);
                p.p.clear();
                p.add_item(Item(1, As[j]));
                p.add_item(Item(-1, As[i]));
                Ss.push_back(p);
                //B
                p.p.clear();
                p.add_item(Item(1, Bs[i]));
                p.add_item(Item(1, Bs[j]));
                Ss.push_back(p);
                p.add_item(Item(-2, Bs[j]));
                Ss.push_back(p);
                p.p.clear();
                p.add_item(Item(1, Bs[j]));
                p.add_item(Item(-1, Bs[i]));
                Ss.push_back(p);
            }
        //for_each(Ss.begin(), Ss.end(), [](Polynomial&p){p.print(); });
        return Ss;
    }

    static vector<Polynomial> & create_p(vector<Polynomial> s=S_strassen())
    {
        if(s.empty())s = create_s();
        int As[] = { 111, 112, 121, 122 };
        int Bs[] = { 211, 212, 221, 222 };
        for (auto i : As) s.push_back(Item(1, i));
        for (auto i : Bs) s.push_back(Item(1, i));
        
        static vector<Polynomial> p;
        // add all possible combination
        for (int i = 0; i < s.size(); ++i){
            bool isA = (s[i].p.begin()->f1 / 100 == 1);
            for (int j = i + 1; j < s.size(); ++j)
            {
                bool isAlsoA = (s[j].p.begin()->f1 / 100 == 1);
                if (isA+isAlsoA==1)
                    p.push_back(s[i] * s[j]);
            }
        }
        return p;
    }

    static vector<Polynomial> & P_strassen()
    {
        vector<Polynomial> & s = S_strassen();
        static vector<Polynomial> p = {
            s[0] * Item(1, 111),
            s[1] * Item(1, 222),
            s[2] * Item(1, 211),
            s[3] * Item(1, 122),
            s[4] * s[5],
            s[6] * s[7],
            s[8] * s[9]
        };
        return p;
    }
    static vector<Polynomial> & objs()
    {
        static vector<Polynomial> o = {
            Polynomial(Item(1, 111,211), Item(1, 112,221)),
            Polynomial(Item(1, 111, 212), Item(1, 112, 222)),
            Polynomial(Item(1, 121, 211), Item(1, 122, 221)),
            Polynomial(Item(1, 121, 212), Item(1, 122, 222))
        };
        return o;
    }

    static bool validate(const vector<Polynomial> & P, vector<Polynomial> objs)
    {
        Polynomial sum = accumulate(P.begin(), P.end(), Polynomial(), 
            [](Polynomial & r, const Polynomial & p){ r.p.insert(p.p.begin(), p.p.end()); return r; });
        // P should contain all obj items
        for (auto & obj : objs)
            for (auto & i : obj.p)
                if (sum.p.find(i) == sum.p.end())   // ! Here find uses < instead of ==
                    return false;
        //if (sum.p.size() < accumulate(objs.begin(), objs.end(), 0, [](int s, Polynomial & p){return s + p.p.size(); }))
        //return false;

        for (auto & obj : objs)
            if (!validate(P, P.size()-1,obj)){
                return false;
            }
            //else cout << "One more satified\n";
        return true;
    }

    static bool validate(const vector<Polynomial> & P, int lastInd, Polynomial obj)
    {
        if (obj.p.empty()) return true;
        if (lastInd == 0) return P.front() == obj;
        
        // Case 1, 0 current component
        bool res = validate(P, lastInd - 1, obj);
        if (res)return true;

        // Case 2, 1 current component
        res = validate(P, lastInd - 1, obj - P[lastInd]);
        if (res)return true;

        // Case 3, -1 current component
        res = validate(P, lastInd - 1, obj + P[lastInd]);

        return res;
    }

    static bool testAllSubset(const vector<Polynomial> & objs, const vector<Polynomial> & groundP, int currInd, vector<Polynomial> target = vector<Polynomial>())
    {

        if (target.size() == TARGETSIZE){
            static long long int counter = 0;
            ++counter;
            if (counter % 10000 == 0)
                cout << counter<<"\n";
            if (validate(target, objs)){
                for_each(target.begin(), target.end(), [](Polynomial& p){p.print(); });
                getchar();
            }
            else return false;
        }
        if (currInd == groundP.size())return false;// No enough elements
        // Case 1, not counting
        if (testAllSubset(objs, groundP, currInd + 1, target) ) return true;
        // Case 2, count the current poly
        target.push_back(groundP[currInd]);
        return testAllSubset(objs, groundP, currInd + 1, target);
    }

    static void main()
    {
        std::ios_base::sync_with_stdio(false);

        item_names.insert(make_pair(111, "A11"));
        item_names.insert(make_pair(112, "A12"));
        item_names.insert(make_pair(121, "A21"));
        item_names.insert(make_pair(122, "A22"));
        item_names.insert(make_pair(211, "B11"));
        item_names.insert(make_pair(212, "B12"));
        item_names.insert(make_pair(221, "B21"));
        item_names.insert(make_pair(222, "B22"));

        // Test
        //Polynomial s9, s10;
        //s9.add_item(Item(1, 111));
        //s9.add_item(Item(-1, 121));
        //s10.add_item(Item(1, 211));
        //s10.add_item(Item(1, 212));
        //(s9*s9).print(); // output: +A11A11-2A11A21+A21A21
        
        bool res = validate(P_strassen(), objs());
        assert(res);

        // Serious work
        cout << "begin:\n";
        cout << "Total candidates : " << create_p().size()<<endl;
        res = testAllSubset(objs(), create_p(), 0);
        cout << res;
    }
};
map<const Item::FactorType, string> Polynomial::item_names;
#endif