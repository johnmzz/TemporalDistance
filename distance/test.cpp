#include <iostream>
#include <vector>
#include <map>
#include <typeinfo>

using namespace std;

int main() {
    struct Label {
        int t1;
        int t2;
        int d;
        Label(int _t1, int _t2, int _d): t1(_t1), t2(_t2), d(_d) {};
        
        bool operator< (const Label &x) const {
            return d < x.d;
        }

        void print_label() {
            cout << "d = " << d << ", t1 = " << t1 << ", t2 = " << t2 << endl;
        }
    };

    map<int,vector<Label>> a = map<int, vector<Label>>();

    a[0].push_back(Label(1,1,1));
    a[3].push_back(Label(2,2,2));
    a[3].push_back(Label(3,3,3));

    for (auto it = a.begin(); it != a.end(); it++) {
        cout << it->first << ": [";
        for (auto label : it->second) {
            cout << "(" << label.d << "," << label.t1 << "," << label.t2 << ")";
        }
        cout << "]" << endl;
    }
}