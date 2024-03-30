#include "UGraphModel.h"
#include <iostream>
#include <string>
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
class Network{
public:
    UGraphModel<string> *map;
public:
    Network(UGraphModel<string> *map):map(map){}
    Network(){
        map = new UGraphModel<string>(&strEQ, &vertex2str);
    }
    ~Network(){
        if (map) delete map;
    }
    bool hasCentralOffice(){
        UGraphModel<string>::Iterator It = map->begin();
        while (It != map->end()){
            if (map->inDegree(*It) == map->size()-1) return true;
            It++;
        }
        return false;
    }
    void changeRepresent(UGraphModel<string> *map, int** matrix, DLinkedList<string> *vertexList){
        UGraphModel<string>::Iterator It = map->begin();
        for (int i = 0 ; i < map->size() ; i++)
            for (int j = 0; j < map->size() ; j++)
                matrix[i][j] = 0;
        while (It != map->end())
        {
           DLinkedList<string> inList = map->getInwardEdges(*It);
           typename DLinkedList<string>::Iterator listIt = inList.begin();
           while (listIt != inList.end()){
            matrix[vertexList->indexOf(*It)][vertexList->indexOf(*listIt)] = (int)map->weight(*It, *listIt);
            listIt++;
           }
           It++;
        }
    }
    int minDistance(int dist[], bool sptSet[], int V) {
        int min = INT_MAX, min_index;
        for (int v = 0; v < V; v++)
            if (sptSet[v] == false && dist[v] <= min)
                min = dist[v], min_index = v;
        return min_index;
    }
    int minimumDelayTime(string name) {
        int V = map->size();
        //Preprocessing
        int **matrix = new int*[V];
        for (int i = 0; i < V; i++) 
            matrix[i] = new int[V];
        DLinkedList<string> *vertexList = new DLinkedList<string>(NULL, &strEQ);
        UGraphModel<string>::Iterator It = map->begin();
        while (It != map->end()) {
            vertexList->add((*It));
            It++;
        }
        changeRepresent(map, matrix, vertexList);

        //Dijkstra 
        int dist[V];
        bool sptSet[V];
        for (int i = 0; i < V; i++) {
            dist[i] = INT_MAX;
            sptSet[i] = false;
        }
        dist[vertexList->indexOf(name)] = 0;
        for (int i=0; i < V - 1; i++) {
            int u = minDistance(dist, sptSet, V);
            sptSet[u] = true;
            for (int v = 0; v < V; v++){
                if (!sptSet[v] && matrix[u][v] && dist[u] != INT_MAX && dist[u] + matrix[u][v] < dist[v])
                    dist[v] = dist[u] + matrix[u][v];
            }
        }

        //postprocessing
        int result = dist[0];
        for (int i = 1; i < V; i++) {
            if (dist[i] > result)
                result = dist[i];
        }
        delete vertexList;
        //return
        return result;
    }
};
void tc1(){
    UGraphModel<string> model(&strEQ, &vertex2str);
    model.add("A");
    model.add("B");
    model.add("C");
    model.add("D");
    model.add("E");
    model.add("F");
    model.connect("A", "B", 1);
    model.connect("A", "C", 2);
    model.connect("A", "D", 3);
    model.connect("A", "E", 4);
    model.connect("A", "F", 5);
    Network n(&model);
    if (n.hasCentralOffice()) cout<<"yes";
    cout<<n.minimumDelayTime("A");
}
int main(){
    
}