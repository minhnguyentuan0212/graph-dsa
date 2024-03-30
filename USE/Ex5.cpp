#include "UGraphModel.h"
#include <iostream>
#include <string>
#include <math.h>
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
class Bomb {
public:
    int x,y;
    int radius;
public:
    Bomb(int x = 0, int y = 0, int radius = 0):x(x), y(y), radius(radius){}
};
bool EQ2D(Bomb* &rhs, Bomb* &lhs) {
    return rhs->x==lhs->x && rhs->y==lhs->y && rhs->radius==lhs->radius; 
}
string vertex2str2D(Bomb* & point) {
    return "(" + to_string(point->x) + "," + to_string(point->y) + "R = " + to_string(point->radius) + ")";
}
class Game {
public:
    UGraphModel<Bomb*> *map;
public:
    Game(DLinkedList<Bomb*> *bombList){
        map = new UGraphModel<Bomb*>(&EQ2D, &vertex2str2D);
        DLinkedList<Bomb*>::Iterator It = bombList->begin();
        while (It != bombList->end()){
            map->add(*It);
            It++;
        }
    }
    ~Game(){
        if (map) delete map;
    }
    void recDetonate(UGraphModel<Bomb*>::Iterator It, bool *visited, int &count, DLinkedList<Bomb*>* vertexList) {
        count++;
        visited[vertexList->indexOf(*It)] = true;
        UGraphModel<Bomb*>::Iterator mapIt = map->begin();
        while (mapIt != map->end()){
            if (mapIt != It 
            && visited[vertexList->indexOf(*mapIt)] == false
            && (*It)->radius*(*It)->radius >= ((*It)->x - (*mapIt)->x)*((*It)->x - (*mapIt)->x) + ((*It)->y - (*mapIt)->y)*((*It)->y - (*mapIt)->y))
                recDetonate(mapIt, visited, count, vertexList);
            mapIt++;
        }
    }
    int detonatedBomb(int x, int y){
        UGraphModel<Bomb*>::Iterator It = map->begin();
        while (It != map->end()){
            if ((*It)->x == x && (*It)->y == y) {
                bool *visited = new bool[map->size()];
                for (int i = 0; i < map->size(); i++) visited[i] = false;
                int count = 0;
                DLinkedList<Bomb*> *vertexList = new DLinkedList<Bomb*>(NULL, &EQ2D);
                UGraphModel<Bomb*>::Iterator listIt = map->begin();
                while (listIt != map->end()) {
                    vertexList->add((*listIt));
                    listIt++;
                }
                recDetonate(It, visited, count, vertexList);
                return count;
            }
                
            It++;
        }
        return -1;
    }
    int maximumBomb() {
        UGraphModel<Bomb*>::Iterator It = map->begin();
        int max = 0;
        while (It != map->end()){
            int x = detonatedBomb((*It)->x, (*It)->y);
            if (x > max)
                max = x;
            It++;
        }
        return max;
    }
};
int main(){
    DLinkedList<Bomb*> *bombList = new DLinkedList<Bomb*>(NULL, &EQ2D);
    bombList->add(new Bomb(1, 2, 3));
    bombList->add(new Bomb(2, 3, 1));
    bombList->add(new Bomb(3, 4, 2));
    bombList->add(new Bomb(4, 5, 3));
    bombList->add(new Bomb(5, 6, 4));
    bombList->add(new Bomb(1, 0, 4));
    bombList->add(new Bomb(10, 11, 10));

    Game game(bombList);
    cout<<game.detonatedBomb(1, 2);
    cout<<game.maximumBomb();
}