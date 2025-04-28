#include<quadtree.h>

vector<point> dirs = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

void quadTree::build_tree(Node* node, vector<point> p, point center, double half_side){
    hs.push_back(half_side);
    cs.push_back(center);
    int n = p.size();
    if(p.empty())
        return;
    node->rep = new point(p[0]);
    if(n == 1){
        node->leaf = true;
        node->leaf_point = node->rep;
        return;
    }

    vector<point> l, r;
    for(int i = 0;i < n;i++){
        if(p[i].first <= center.first)
            l.push_back(p[i]);
        else
            r.push_back(p[i]);
    }

    vector<vector<point>> new_points(4);
    int nl = l.size(), nr = r.size();
    for(int i = 0;i < nl;i++){
        if(l[i].second <= center.second)
            new_points[0].push_back(l[i]);
        else
            new_points[1].push_back(l[i]);
    }
    for(int i = 0;i < nr;i++){
        if(r[i].second <= center.second)
            new_points[2].push_back(r[i]);
        else
            new_points[3].push_back(r[i]);
    }

    half_side /= 2; 


    for(int i = 0;i < 4;i++){
        point new_center = {center.first + (dirs[i].first*half_side), center.second + (dirs[i].second*half_side)};
        node->child[i] = new Node(new_center, half_side);
        node->child[i]->level = node->level + 1;
        if(new_points[i].empty())
            node->child[i]->leaf = true;
        build_tree(node->child[i], new_points[i], new_center, half_side);
    }   
}

string generate_rgb_color(int i) {
    int r = (i * 73) % 256;
    int g = (i * 151) % 256;
    int b = (i * 199) % 256;

    char hex_color[8];
    snprintf(hex_color, sizeof(hex_color), "#%02X%02X%02X", r, g, b);
    return string(hex_color);
}


void draw_line(double x1, double y1, double x2, double y2, const std::string& color = "b-") {
    std::vector<double> xs = {x1, x2};
    std::vector<double> ys = {y1, y2};
    plt::plot(xs, ys, color);
}

void scatter_points(vector<point>& points){
    vector<double> x, y;
    for (const auto& p : points) {
        x.push_back(p.first);
        y.push_back(p.second);
    }
    plt::scatter(x, y, 40.0);
}

void plot_spanner(vector<pair<point, point>>& edges){
    string color;
    int i = 0;
    for (const auto& edge : edges) {
        auto [p1, p2] = edge;
        double x1 = p1.first, y1 = p1.second;
        double x2 = p2.first, y2 = p2.second;
        color = generate_rgb_color(i);
        i++;
        draw_line(x1, y1, x2, y2, color);
    }
}
void quadTree::plot(){
    // cout << "found " << edges.size() << " Edges\n";
    scatter_points(points);
    plot_spanner(edges);
    plt::axis("equal");
    plt::grid(false);
    plt::title("t-spanner");
    plt::save("spanner.png");
    plt::show();
}

double dist(point a, point b) {
    return sqrt((a.first - b.first)*(a.first - b.first) + (a.second - b.second)*(a.second - b.second));
}

double dist(point* a, point* b) {
    return sqrt((a->first - b->first)*(a->first - b->first) + (a->second - b->second)*(a->second - b->second));
}

double radius(Node* node) {
    if(node->leaf_point) return 0;
    return node->half_side * sqrt(2);
}

bool wellSeparated(Node* u, Node* v, double s) {
    double max_radius = max(radius(u), radius(v));
    double d = dist(u->center, v->center);
    return d >= s * max_radius;
}

void quadTree::get_leaves(Node* node, vector<point>& leaves){
    if(node == NULL)
        return;

    if(node->leaf){
        if(node->leaf_point)
            leaves.push_back({node->rep->first, node->rep->second});
    }

    for(int i = 0;i < 4;i++)
        get_leaves(node->child[i], leaves);

}

void quadTree::get_wspd(Node* u, Node* v) {
    
    if (u == NULL || v == NULL) return;
    if(u->rep == NULL || v->rep == NULL || (u->leaf && v->leaf && u == v))
        return;

    if (wellSeparated(u, v, s)) {
        centers.push_back(u->center);
        r.push_back(radius(u));
        centers.push_back(v->center);
        r.push_back(radius(v));
        vector<point> u_leaves, v_leaves;
        get_leaves(u, u_leaves);
        get_leaves(v, v_leaves);
        wspd_pairs.push_back({u_leaves, v_leaves});

        point u_rep = *(u->rep);
        point v_rep = *(v->rep);
        
        edges.push_back({u_rep, v_rep});
    } else {
        if (u->level < v->level) {
            for (int i = 0; i < 4; i++) get_wspd(u->child[i], v);
        } else {
            for (int i = 0; i < 4; i++) get_wspd(u, v->child[i]);
        }
    }
}


void quadTree::get_spanner(double eps){
    s = 4*(eps+2)/(eps);
    get_wspd(root, root);
    plot();
    for (const auto& edge : edges) {
        auto [p1, p2] = edge;
        double x1 = p1.first, y1 = p1.second;
        double x2 = p2.first, y2 = p2.second;
        cout << "(" << x1 << ", " << y1 << ") <----------> (" << x2 << ", " << y2 << ")\n";
    }
    
}