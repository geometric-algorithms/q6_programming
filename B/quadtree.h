#include<bits/stdc++.h>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;
using namespace std;

using point = pair<double, double>;

struct Node{
    point* leaf_point;
    bool leaf;
    int level;
    point* rep;
    point center;
    double half_side;
    Node* child[4];

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
    point bottom_left, top_right;
    double s;
    vector<point> points;
    vector<point> centers;
    vector<point> cs;
    vector<double> r, hs;
    vector<pair<vector<point>, vector<point>>> wspd_pairs;
    vector<pair<point, point>> edges;
    

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

