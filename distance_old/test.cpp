#include <iostream>
#include <vector>

using namespace std;


int main() {
    struct Label {
        int v;
        int t1;
        int t2;
        int d;
        Label(int _v, int _t1, int _t2, int _d): v(_v), t1(_t1), t2(_t2), d(_d) {};
        
        bool operator< (const Label &x) const {
            return v < x.v;
        }

        void print_label() {
            cout << "v = " << v << ", t1 = " << t1 << ", t2 = " << t2 << ", d = " << d << endl;
        }
    };

    vector<Label> a;

    for (int i = 0; i < 10; i++) {
        a.push_back(Label(i, i, i+1, i*10));
    }

    for (auto v : a) v.print_label();

    auto it = lower_bound(a.begin(), a.end(), Label(3, 0, 0, 0));
    cout << it - a.begin() << endl;
}