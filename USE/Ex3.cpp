#include "UGraphModel.h"
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <algorithm>
using namespace std;
bool strEQ(string& lhs, string& rhs){
    return lhs.compare(rhs) == 0;
}
bool intEQ(int &lhs, int &rhs) {
    return lhs==rhs;
}
string vertex2str(int & item) {
    return to_string(item);
}
string vertex2str(string& item){
    return item;
}
class Point2D {
public:
    int x,y;
public:
    Point2D(int x = 0, int y = 0):x(x), y(y){}
};
bool EQ2D(Point2D* &rhs, Point2D* &lhs) {
    return rhs->x==lhs->x && rhs->y==lhs->y; 
}
string vertex2str2D(Point2D* & point) {
    return "(" + to_string(point->x) + "," + to_string(point->y) + ")";
}
class DSU {
    int* parent;
    int* rank;
public:
    DSU(int n)
    {
        parent = new int[n];
        rank = new int[n];
 
        for (int i = 0; i < n; i++) {
            parent[i] = -1;
            rank[i] = 1;
        }
    }
    int find(int i)
    {
        if (parent[i] == -1)
            return i;
 
        return parent[i] = find(parent[i]);
    }
    void unite(int x, int y)
    {
        int s1 = find(x);
        int s2 = find(y);
        if (s1 != s2) {
            if (rank[s1] < rank[s2]) {
                parent[s1] = s2;
            }
            else if (rank[s1] > rank[s2]) {
                parent[s2] = s1;
            }
            else {
                parent[s2] = s1;
                rank[s1] += 1;
            }
        }
    }
};
class Game {
public:
    UGraphModel<Point2D*> *map;
public:
    Game(UGraphModel<Point2D*> *map):map(map){}
    ~Game(){
        if (map) delete map;
    }
    void changeRepresent(UGraphModel<Point2D*> *map, vector<vector<int>> &edgelist, DLinkedList<Point2D*> *vertexList){
        UGraphModel<Point2D*>::Iterator It = map->begin();
        int i = 0;
        while (It != map->end())
        {
           UGraphModel<Point2D*>::Iterator listIt = map->begin();
           int j = 0;
           while (j <= i) {
                j++;
                listIt++;
           }
           while (listIt != map->end()){
            //processing
            edgelist.push_back({abs((*It)->x - (*listIt)->x) + abs((*listIt)->y - (*It)->y), vertexList->indexOf(*It), vertexList->indexOf(*listIt)});
            listIt++;
           }
           It++;
           i++;
        }
    }
    int hasMST(int cost) {
        //preprocessing
        DLinkedList<Point2D*> *vertexList = new DLinkedList<Point2D*>(NULL, &EQ2D);
        UGraphModel<Point2D*>::Iterator It = map->begin();
        while (It != map->end()) {
            vertexList->add((*It));
            It++;
        }
        vector<vector<int>> edgelist;
        changeRepresent(map, edgelist, vertexList);
        //Kruskal
        
        sort(edgelist.begin(), edgelist.end());
        DSU s(map->size());
        int result = 0;
        for (auto edge : edgelist) {
            int w = edge[0];
            int x = edge[1];
            int y = edge[2];
            if (s.find(x) != s.find(y)) {
                s.unite(x, y);
                result += w;
            }
        }
        result = cost - result; 
        return (result > 0)?result:-1;
    }
    int maximumVertex(int x, int y, int rest) {
        UGraphModel<Point2D*>::Iterator It = map->begin();
        int count = 0;
        while(It != map->end()){
            if (abs(x - (*It)->x) + abs(y - (*It)->y) <= rest) count++;
            It++;
        }
        return count;
    }
};


int main(){
    Point2D s1(1,1);
    Point2D s2(5,9);
    Point2D s3(2,3);
    Point2D s4(7,8);
    Point2D s5(2,11);
    UGraphModel<Point2D*> *model = new UGraphModel<Point2D*>(&EQ2D, &vertex2str2D);
    model->add(&s1);
    model->add(&s2);
    model->add(&s3);
    model->add(&s4);
    model->add(&s5);
    Game game(model);
    cout<<game.hasMST(30);

}