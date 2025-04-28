#include<quadtree.h>

int main(){
    //everything is same from the previous part except the plotting part
    double eps, s;
    s = 1;
    cin >> eps;
    int n;
    cin >> n;
    vector<point> points;
    while(n--){
        double x, y;
        cin >> x >> y;
        points.push_back({x, y});
    }
    n = points.size();
    auto comp = [](point a, point b){
        return a.first < b.first;
    };
    sort(points.begin(), points.end(), comp);
    double x_min = points[0].first;
    double x_max = points[n-1].first;
    double y_min = numeric_limits<double>::max();
    double y_max = numeric_limits<double>::lowest();
    for(int i = 0;i < n;i++){
        y_min = min(y_min, points[i].second);
        y_max = max(y_max, points[i].second);
    }
    x_min--;x_max++;
    y_min--;y_max++;
    point bl = {x_min, y_min};
    point tr = {x_max, y_max};

    quadTree qt(bl, tr, points, s);
    qt.build_tree(qt.root, points, qt.root->center, qt.root->half_side);
    // s = 4*(t+1)/(t-1) <==> s = 4*(eps + 2)/eps
    qt.get_spanner(eps);
}