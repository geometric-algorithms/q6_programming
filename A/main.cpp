#include<quadtree.h>

int main(){
    double s;
    cin >> s;
    int n;
    cin >> n;
    vector<point> points;
    // taking the input
    while(n--){
        double x, y;
        cin >> x >> y;
        points.push_back({x, y});
    }
    n = points.size();
    auto comp = [](point a, point b){
        return a.first < b.first;
    };
    //calculating the boundary of the points
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
    //initialize quadtree
    quadTree qt(bl, tr, points, s);
    //build quadtree
    qt.build_tree(qt.root, points, qt.root->center, qt.root->half_side);
    //getting wspd pairs
    qt.get_wspd_pairs();

}