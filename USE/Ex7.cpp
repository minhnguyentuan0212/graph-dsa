#include "UGraphModel.h"
#include <iostream>
#include <string>
using namespace std;
bool intEQ(int &lhs, int &rhs) {
    return lhs==rhs;
}
string vertex2str(int & item) {
    string s = "int";
    return s;
}
class Maze {
public:
    UGraphModel<int> *map;
public:
    Maze(UGraphModel<int> *map):map(map){}
    Maze(){
        map = new UGraphModel<int>(&intEQ, &vertex2str);
    }
    void DFS(UGraphModel<int> *map, DLinkedList<int>* vertexList, bool* visited, UGraphModel<int>::Iterator disIt){
        visited[vertexList->indexOf(*disIt)] = true;
        DLinkedList<int> inList = map->getInwardEdges(*disIt);
        DLinkedList<int>::Iterator listIt = inList.begin();
        while (listIt != inList.end()){
            if(visited[vertexList->indexOf(*listIt)] == false) {
                UGraphModel<int>::Iterator It = map->begin();
                while (It != map->end()){
                    if(*It == *listIt)
                        break;
                    It++;
                }
                DFS(map, vertexList, visited, It);
            }
            listIt++;     
        }
    }   
    bool isConnected(){
        bool *visited = new bool[map->size()];
        for (int i = 0; i < map->size(); i++) visited[i] = false;
        DLinkedList<int> *vertexList = new DLinkedList<int>(NULL, &intEQ);
        UGraphModel<int>::Iterator It = map->begin();
        while (It != map->end()) {
            vertexList->add((*It));
            It++;
        }
        It = map->begin();
        int i = 0;
        while (It != map->end()) {
            if (map->inDegree(*It) != 0) break;
            i++;
            It++;
        }
        
        if (i == map->size()) return true;
        
        DFS(map, vertexList, visited, It);
        It = map->begin();
        while (It != map->end()){
            if (visited[vertexList->indexOf(*It)] == false && map->inDegree(*It) > 0) 
                return false;
            It++;
        }
        return true;
    }
    bool canFinish(){
        if (isConnected() == false) return false;
        UGraphModel<int>::Iterator It = map->begin();
        while (It != map->end()){
            if (map->inDegree(*It)%2 != 0) return false; 
            It++;
        }
        return true;
    }
    void printTour(){
        
    }
};
int main(){
    UGraphModel<int> *model = new UGraphModel<int>(&intEQ, &vertex2str);
    model->add(1);
    model->add(2);
    model->add(3);
    model->add(4);
    model->add(5);
    model->connect(1,2);
    model->connect(1,3);
    model->connect(1,4);
    model->connect(1,5);
    model->connect(2,3);
    model->connect(4,5);
    model->connect(3,4);
    model->connect(2,4);
    model->connect(3,5);
    model->connect(2,5);

    Maze maze(model);
    cout<<maze.canFinish();
}