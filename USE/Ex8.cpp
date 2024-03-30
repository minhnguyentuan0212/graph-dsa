#include "UGraphModel.h"
#include "XArrayList.h"
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


class District {
public:
    DLinkedList<string> *ward;
    string name;
public:
    District(string name = "", DLinkedList<string>* ward = NULL):name(name), ward(ward){}
    ~District(){
        if (ward) delete ward;
    }
};
bool DisEQ(District* &rhs, District* &lhs){
    return rhs->name.compare(lhs->name) == 0;
}
string Dis2str(District* &item) {
    return item->name;
}
class Country {
public:
    UGraphModel<District*> *map;
public:
    Country(UGraphModel<District*> *map): map(map){}
    Country(){
        map = new UGraphModel<District*>(&DisEQ, &Dis2str);
    }
    ~Country(){
        if (map) delete map;
    }
    int numberOfProvince(){
        DLinkedList<string> *vertexList = new DLinkedList<string>(NULL, &strEQ);
        UGraphModel<District*>::Iterator It = map->begin();
        while (It != map->end()) {
            vertexList->add((*It)->name);
            It++;
        }
        bool *visited = new bool[map->size()];
        int count = 0;
        for (int i = 0; i < map->size(); i++) visited[i] = false;
        UGraphModel<District*>::Iterator disIt = map->begin();
        while (disIt != map->end()) {
            
            if (visited[vertexList->indexOf((*disIt)->name)] == false) {
                DFS(map, vertexList, visited, disIt);
                count++;
            }
            disIt++;
        }
        return count;
    }
    void DFS(UGraphModel<District*> *map, DLinkedList<string>* vertexList, bool* visited, UGraphModel<District*>::Iterator disIt){
        visited[vertexList->indexOf((*disIt)->name)] = true;
        DLinkedList<District*> inList = map->getInwardEdges(*disIt);
        DLinkedList<District*>::Iterator listIt = inList.begin();
        while (listIt != inList.end()){
            if(visited[vertexList->indexOf((*listIt)->name)] == false) {
                UGraphModel<District*>::Iterator It = map->begin();
                while (It != map->end()){
                    if((*It)->name.compare((*listIt)->name) == 0) {
                        DFS(map, vertexList, visited, It);
                        break;
                    }
                    It++;
                }
                
            }
            listIt++;     
        }   
    }
    void DFS(UGraphModel<District*> *map, DLinkedList<string>* vertexList, bool* visited, UGraphModel<District*>::Iterator disIt, int &count){
        visited[vertexList->indexOf((*disIt)->name)] = true;
        count+= (*disIt)->ward->size(); 
        DLinkedList<District*> inList = map->getInwardEdges(*disIt);
        DLinkedList<District*>::Iterator listIt = inList.begin();
        while (listIt != inList.end()){
            if(visited[vertexList->indexOf((*listIt)->name)] == false) {
                UGraphModel<District*>::Iterator It = map->begin();
                while (It != map->end()){
                    if((*It)->name.compare((*listIt)->name) == 0)
                        DFS(map, vertexList, visited, It, count);
                    It++;
                }
                
            }
            listIt++;     
        }   
    }
    int numberOfWard(string name){
        DLinkedList<string> *vertexList = new DLinkedList<string>(NULL, &strEQ);
        UGraphModel<District*>::Iterator It = map->begin();
        while (It != map->end()) {
            vertexList->add((*It)->name);
            It++;
        }
        bool *visited = new bool[map->size()];
        int count = 0;
        for (int i = 0; i < map->size(); i++) visited[i] = false;
        It = map->begin();
            while (It != map->end()){
                if((*It)->name.compare(name) == 0) {
                    DFS(map, vertexList, visited, It, count);
                    break;
                }
                It++;   
            }
        return count;   
    }
};
int main(){
    DLinkedList<string>* wardList1 = new DLinkedList<string>(NULL, &strEQ);
    wardList1->add("w1");
    wardList1->add("w2");
    wardList1->add("w3");
    wardList1->add("w4");
    wardList1->add("w5");
    wardList1->add("w6");
    wardList1->add("w7");

    DLinkedList<string>* wardList2 = new DLinkedList<string>(NULL, &strEQ);
    wardList2->add("w1");
    wardList2->add("w2");
    wardList2->add("w3");
    wardList2->add("w4");
    wardList2->add("w5");
    DLinkedList<string>* wardList3 = new DLinkedList<string>(NULL, &strEQ);
    wardList3->add("w1");
    wardList3->add("w2");
    wardList3->add("w3");
    wardList3->add("w4");


    District* disA = new District("A", wardList1);
    District* disB = new District("B", wardList2);
    District* disC = new District("C", wardList3);
    District* disD = new District("D", wardList1);
    District* disE = new District("E", wardList2);
    District* disF = new District("F", wardList3);
    UGraphModel<District*>*model = new UGraphModel<District*>(&DisEQ, &Dis2str);
    model->add(disA);
    model->add(disB);
    model->add(disC);
    model->add(disD);
    model->add(disE);
    model->add(disF);
    model->connect(disA, disB);
    model->connect(disB, disC);
    model->println();
    Country c(model);
    cout<<c.numberOfWard("A");
    
}