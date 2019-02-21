/*
ID: yaagnto2
LANG: C++14
TASK: wormhole
*/
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

struct point_t {
    int64_t x, y;
};

bool operator==(const point_t &a, const point_t &b) {
    return a.x == b.x && a.y == b.y;
}

bool operator!=(const point_t &a, const point_t &b) {
    return !(a == b);
}

typedef tuple<point_t, point_t> pair_t;
int times = 0;


bool connected(const pair_t &a, const pair_t &b) {
    if (a == b)return false;
    auto a2 = get<1>(a);
    auto b1 = get<0>(b);
    return a2.y == b1.y && a2.x < b1.x;
}

bool pairs_check(const vector<pair_t> &pairs) {
    if (pairs.size() < 2)
        return true;

    auto prs{pairs};
    for (auto &pr  : pairs) {
        prs.emplace_back(get<1>(pr), get<0>(pr));
    }

    auto pr = prs.back();
    auto head{pr};
    prs.pop_back();

    while (!prs.empty()) {
        auto it = find_if(prs.begin(), prs.end(),
                          [&](pair_t &p) { return connected(pr, p); });
        if (it == prs.end()) {
            pr = prs.back();
            head = pr;
            prs.pop_back();
            continue;
        }
        pr = *it;
        if (connected(pr, head))
            return false;
        prs.erase(it);
    }
    return !connected(pr, head);
}

void rec_fun(vector<point_t> &points, vector<pair_t> &pairs) {
    if (!pairs_check(pairs)) return;
    else if (points.empty()) {
        times++;
        return;
    }

    auto p1 = points.back();
    points.pop_back();
    for (point_t &p2:points) {
        if (p2.y == p1.y) continue;
        pair_t _pair{p1, p2};

        vector<point_t> _points{};
        remove_copy(points.begin(), points.end(), back_inserter(_points), p2);

        vector<pair_t> _pairs{};
        copy(pairs.begin(), pairs.end(), back_inserter(_pairs));

        _pairs.push_back(_pair);
        rec_fun(_points, _pairs);
    }
}

int main() {
    ifstream fin("wormhole.in");
    ofstream fout("wormhole.out");
    size_t n;
    fin >> n;
    vector<point_t> points(n);
    for (size_t i = 0; i < n; i++)
        fin >> points[i].x >> points[i].y;

    vector<pair_t> _{};
    rec_fun(points, _);
    fout << times * (n / 2) << endl;
    return 0;
}
