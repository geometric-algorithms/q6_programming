#include<quadtree.h>

vector<point> dirs = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

void quadTree::build_tree(Node* node, vector<point> p, point center, double half_side){
    hs.push_back(half_side);
    cs.push_back(center);
    if(p.empty())
        return;
    //assiging the first point as rep of node
    node->rep = new point(p[0]);
    int n = p.size();
    //checking leaf condition
    if(n == 1){
        node->leaf = true;
        node->leaf_point = node->rep;
        return;
    }

    //splitting the points into left half and right half
    vector<point> l, r;
    for(int i = 0;i < n;i++){
        if(p[i].first <= center.first)
            l.push_back(p[i]);
        else
            r.push_back(p[i]);
    }

    vector<vector<point>> new_points(4);
    int nl = l.size(), nr = r.size();
    //splitting the left half points into north and south
    for(int i = 0;i < nl;i++){
        if(l[i].second <= center.second)
            new_points[0].push_back(l[i]);
        else
            new_points[1].push_back(l[i]);
    }
    //splitting the right half points into north and south
    for(int i = 0;i < nr;i++){
        if(r[i].second <= center.second)
            new_points[2].push_back(r[i]);
        else
            new_points[3].push_back(r[i]);
    }

    //calculating the length of the side for next split in the square
    half_side /= 2; 

    //recursively calculating the children nodes
    for(int i = 0;i < 4;i++){
        point new_center = {center.first + (dirs[i].first*half_side), center.second + (dirs[i].second*half_side)};
        node->child[i] = new Node(new_center, half_side);
        node->child[i]->level = node->level + 1;
        if(new_points[i].empty())
            node->child[i]->leaf = true;
        build_tree(node->child[i], new_points[i], new_center, half_side);
    }   
}

//plotting a square given center and length of half_side
void draw_square(double cx, double cy, double half_side) {
    std::vector<double> x, y;

    x.push_back(cx - half_side); y.push_back(cy - half_side);
    x.push_back(cx + half_side); y.push_back(cy - half_side);
    x.push_back(cx + half_side); y.push_back(cy + half_side);
    x.push_back(cx - half_side); y.push_back(cy + half_side);
    x.push_back(cx - half_side); y.push_back(cy - half_side);

    plt::plot(x, y, "r-");
}

//generate a new color for a particular integer
string generate_rgb_color(int i) {
    int r = (i * 73) % 256;
    int g = (i * 151) % 256;
    int b = (i * 199) % 256;

    char hex_color[8];
    snprintf(hex_color, sizeof(hex_color), "#%02X%02X%02X", r, g, b);
    return string(hex_color);
}

//plotting a circle given center, radius and the color in which it has to be shown
void draw_circle(double cx, double cy, double radius, const std::string& color) {
    std::vector<double> xs, ys;
    const int segments = 100;

    for (int i = 0; i <= segments; ++i) {
        double theta = 2.0 * M_PI * i / segments;
        xs.push_back(cx + radius * cos(theta));
        ys.push_back(cy + radius * sin(theta));
    }

    plt::plot(xs, ys, color);
}

//plotting the points
void scatter_points(vector<point>& points){
    vector<double> x, y;
    for (const auto& p : points) {
        x.push_back(p.first);
        y.push_back(p.second);
    }
    plt::scatter(x, y, 40.0);
}

//plotting the quadtree
void plot_quadtree(vector<point>& cs, vector<double>& hs){
    int n = cs.size();
    for (int i = 0; i < n; ++i) {
        double cx = cs[i].first;
        double cy = cs[i].second;
        double h = hs[i];
        draw_square(cx, cy, h);
    }
}

//plotting the circles, a pair in a WSPD represented by the same color 
void plot_circles(vector<point>& centers, vector<double>& r){
    string color;
    int n = centers.size();
    for (int i = 0; i < n; ++i) {
        double cx = centers[i].first;
        double cy = centers[i].second;
        double h = r[i] + 0.1;
        if(i%2 == 0)
            color = generate_rgb_color(i);
        draw_circle(cx, cy, h, color);
    }
}

void quadTree::plot(){
    // cout << "found " << centers.size()/2 << " WSPD Pairs\n";
    scatter_points(points);
    plot_quadtree(cs, hs);
    plot_circles(centers, r);
    plt::axis("equal");
    plt::grid(false);
    plt::title("well separated Points");
    plt::save("wspd.png");
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

//checking if nodes are well separated using the centers of both u & v
bool wellSeparated(Node* u, Node* v, double s) {
    double max_radius = max(radius(u), radius(v));
    // double d = dist(u->center, v->center);
    double d = dist(u->rep, v->rep);
    return d >= s * max_radius;
}

//getting the points associated within the node i.e. leaves
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

//checking if nodes u and v are well separated
void quadTree::get_wspd(Node* u, Node* v) {
    
    if (u == NULL || v == NULL) return;
    if(u->rep == NULL || v->rep == NULL || (u->leaf && v->leaf && u == v))
        return;
    // check well separated condition and append them to the corresponding lists
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
        //adding edges of the corresponding reps
        edges.push_back({u_rep, v_rep});
    } else {
        //check which is the bigger square and split it and compute again
        if (u->level < v->level) {
            for (int i = 0; i < 4; i++) get_wspd(u->child[i], v);
        } else {
            for (int i = 0; i < 4; i++) get_wspd(u, v->child[i]);
        }
    }
}

//printing a pair in WSPD
void print_pairs(vector<point> a, vector<point> b){
    string s = "{";
    for(point x: a)
        s += " (" +to_string(x.first) + ", " + to_string(x.second) + "),";
    s.pop_back();
    s += "} ";
    s += "<====> {";
    for(point x: b)
        s += " (" +to_string(x.first) + ", " + to_string(x.second) + "),";
    s.pop_back();
    s += "} \n";
    cout << s;
}


void quadTree::get_wspd_pairs(){
    get_wspd(root, root);
    plot();
    int n = wspd_pairs.size();
    for(int i = 0;i < n;i++){
        print_pairs(wspd_pairs[i].first, wspd_pairs[i].second);
    }
}
