#include "UGraphModel.h"
#include "DLinkedList.h"
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <stack>
#include <sstream>
using namespace std;
bool strEQ(string& lhs, string& rhs){
    return lhs.compare(rhs) == 0;
}
string vertex2str(string& item){
    return item;
}
class Transportation{
public:
    DLinkedList<vector<pair<string, string>>> *repairList;
    UGraphModel<string> *map;
public:
    Transportation(){
        repairList = new DLinkedList<vector<pair<string, string>>>(NULL, NULL);
        map = new UGraphModel<string>(&strEQ, &vertex2str);
    }
    Transportation(DLinkedList<vector<pair<string, string>>>* repairList, UGraphModel<string>* map):repairList(repairList), map(map){}
    ~Transportation() {
        if (repairList) {
            repairList->clear();
            delete repairList;
        }
        if (map) {
            map->clear();
            delete map;
        }
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
    int Dijkstra(string first, string second, UGraphModel<string> *map) {
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
        dist[vertexList->indexOf(first)] = 0;
        for (int i=0; i < V - 1; i++) {
            int u = minDistance(dist, sptSet, V);
            sptSet[u] = true;
            for (int v = 0; v < V; v++){
                if (!sptSet[v] && matrix[u][v] && dist[u] != INT_MAX && dist[u] + matrix[u][v] < dist[v])
                    dist[v] = dist[u] + matrix[u][v];
            }
        }

        //postprocessing
        int result = dist[vertexList->indexOf(second)];
        delete vertexList;
        //return
        return result;
    }
    int minimumTime(string first, string second, int month) {
        month--;
        vector<pair<string, string>>::iterator It = repairList->get(month).begin();
        DLinkedList<float> weightList;
        while (It != repairList->get(month).end()) {
            weightList.add(0, map->weight(It->first, It->second));
            map->disconnect(It->first, It->second);
            It++;
        }
        int result = Dijkstra(first, second, map);
        while (It != repairList->get(month).begin()) {
            It--;
            map->connect(It->first, It->second, weightList.get(0));
            weightList.removeAt(0);
        }
        return result;
    }
    void allShortestPath(){
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
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++)
                if (matrix[i][j] == 0 && i!=j) 
                    matrix[i][j] = INT_MAX;
        }
        for (int k = 0; k < V; k++)
            for (int i = 0; i < V; i++)
                for (int j = 0; j < V; j++)
                    if (matrix[i][j] > (matrix[i][k] + matrix[k][j]) && (matrix[k][j] != INT_MAX && matrix[i][k] != INT_MAX))
                    matrix[i][j] = matrix[i][k] + matrix[k][j];
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (matrix[i][j] == INT_MAX)
                    cout<<"INF";
                else
                    cout << matrix[i][j] << "   ";
        }
        cout << endl;
    }
    }
    void allShortestPath(int month){
        month--;
        vector<pair<string, string>>::iterator It = repairList->get(month).begin();
        DLinkedList<float> weightList;
        while (It != repairList->get(month).end()) {
            weightList.add(0, map->weight(It->first, It->second));
            map->disconnect(It->first, It->second);
            It++;
        }
        allShortestPath();
        while (It != repairList->get(month).begin()) {
            It--;
            map->connect(It->first, It->second, weightList.get(0));
            weightList.removeAt(0);
        }   
    }
};
int main(){
    UGraphModel<string> *graph = new UGraphModel<string>(&strEQ, &vertex2str);
    graph->add("HN");
    graph->add("TPHCM");
    graph->add("DN");
    graph->add("Hue");
    graph->add("VT");
    graph->add("DL");
    graph->add("CT");
    graph->add("PT");
    graph->connect("HN", "TPHCM", 1);
    graph->connect("HN", "DN", 2);
    graph->connect("DN", "TPHCM", 4);
    graph->connect("Hue", "TPHCM", 5);
    graph->connect("Hue", "DN", 7);
    graph->connect("VT", "TPHCM", 3);
    graph->connect("VT", "Hue", 9);
    graph->connect("DN", "CT", 6);
    graph->connect("Hue", "PT", 2);
    graph->connect("Hue", "CT", 4);
    graph->connect("VT", "DL", 5);
    graph->connect("DL", "PT", 10);
    graph->connect("DL", "CT", 6);
    graph->connect("PT", "CT", 3);
    DLinkedList<vector<pair<string,string>>> month;
    vector<pair<string,string>> t1;
    t1.push_back(make_pair("DN", "CT"));
    month.add(t1);
    Transportation ex1(&month, graph);
    ex1.allShortestPath();
}