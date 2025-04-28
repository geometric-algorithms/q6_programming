#include<bits/stdc++.h>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;
using namespace std;

using point = pair<double, double>;

struct Node{
    point* leaf_point; // if leaf and has a point associated with it
    bool leaf;
    int level; // level of the node starting from the root

    point* rep; //representative of the subset A in that node which is the first point among the points
    point center; //center of the square also the circle while calculating wspd
    double half_side; //half of the length of the square contributing to this node
    Node* child[4]; //4 children => 0->SW, 1->NW, 2->SE, 3->NE

    Node(point c, double hs){
        leaf = false;
        rep = NULL;
        level = 0;
        leaf_point = NULL;
        center = c;
        half_side = hs;
        for(int i = 0;i < 4;i++)
            child[i] = NULL;
    }
};

struct quadTree{
    Node* root; 
    point bottom_left, top_right; //boundaries of the square
    double s; // s-WSPD
    vector<point> points; //points for which either WSPD or t-spanner is being computed
    vector<point> centers; // centers of circles in WSPD
    vector<point> cs; // centers of squares for visualisation of quadtree
    vector<double> r; // radius of circles in WSPD
    vector<double> hs; // half side lengths of squares of quatree corresponding to the node in tree
    vector<pair<vector<point>, vector<point>>> wspd_pairs; // set of the WSPD pairs
    vector<pair<point, point>> edges; // edges of the t-spanner
    

    quadTree(point bl, point tr, vector<point> p, double s){
        this->s = s;
        points = p;
        bottom_left = bl;
        top_right = tr;
        point center = {(bl.first + tr.first)/2, (bl.second + tr.second)/2};
        double half_side = max((tr.first - bl.first)/2, (tr.second - bl.second)/2);
        
        root = new Node(center, half_side);
    }


    void build_tree(Node* node, vector<point> points, point center, double half_side);
    void plot();
    void get_wspd(Node* u, Node* v);
    void get_wspd_pairs();
    void get_leaves(Node* node, vector<point>& leaves);
    void get_spanner(double t);
};

