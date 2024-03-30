#include "UGraphModel.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;
//for string
bool strEQ(string& lhs, string& rhs){
    return lhs.compare(rhs) == 0;
}
string vertex2str(string& item){
    return item;
}
//for Warehouse
class Warehouse {
    public:
        string name;
        int soldiers;
    public:
        Warehouse(string name = "", int soldiers = 0):name(name), soldiers(soldiers){}
        ~Warehouse(){}
        string println(){
            return "[" + name + ":" + to_string(soldiers) + "]";
        }
};
bool WHEQ(Warehouse* &lhs, Warehouse* &rhs){
    return lhs->name.compare(rhs->name)==0 && lhs->soldiers==rhs->soldiers;
}
string vertexWH2str(Warehouse* &item){
    return "[" + item->name + ":" + to_string(item->soldiers) + "]";
}
class Invasion {
public:
    DLinkedList<Warehouse*> *plan; 
    UGraphModel<Warehouse*> *map;
public:
    Invasion(DLinkedList<Warehouse*> *plan, UGraphModel<Warehouse*> *map):plan(plan), map(map) {}
    Invasion(){
        this->plan = NULL;
        this->map = new UGraphModel<Warehouse*>(&WHEQ, &vertexWH2str);
    }
    ~Invasion(){
        if (plan) delete plan;
        if (map) delete map;
    }
    void changeRepresent(UGraphModel<Warehouse*> *map, vector<int> adj[], DLinkedList<Warehouse*> *vertexList){
        UGraphModel<Warehouse*>::Iterator It = map->begin();
        while (It != map->end())
        {
           DLinkedList<Warehouse*> inList = map->getInwardEdges(*It);
           typename DLinkedList<Warehouse*>::Iterator listIt = inList.begin();
           while (listIt != inList.end()){
            adj[vertexList->indexOf(*It)].push_back(vertexList->indexOf(*listIt));
            listIt++;
           }
           It++;
        }
    }
    void APUtil(vector<int> adj[], int u, bool visited[], int disc[], int low[], int &time, int parent, bool isAP[]){
        int children = 0;
        visited[u] = true;
        disc[u] = low[u] = ++time;
        for (auto v : adj[u]) {
            if (!visited[v]) {
            children++;
            APUtil(adj, v, visited, disc, low, time, u, isAP);
            low[u] = min(low[u], low[v]);
            if (parent != -1 && low[v] >= disc[u])
                isAP[u] = true;
            }
            else if (v != parent)
                low[u] = min(low[u], disc[v]);
            }
        if (parent == -1 && children > 1)
            isAP[u] = true;
    }
    DLinkedList<string> criticalPoint(){
        DLinkedList<string> result(NULL, &strEQ);
        int V = map->size();
        //Preprocessing
        vector<int> adj[V];
        DLinkedList<Warehouse*> *vertexList = new DLinkedList<Warehouse*>(NULL, &WHEQ);
        UGraphModel<Warehouse*>::Iterator It = map->begin();
        while (It != map->end()) {
            vertexList->add((*It));
            It++;
        }
        changeRepresent(map, adj, vertexList);
        //finding points
        int disc[V];
        int low[V];
        bool visited[V] ;
        bool isAP[V];
        for (int i = 0; i < V; i++){
            disc[i] = 0;
            visited[i] = false;
            isAP[i] = false;
        }
        int time = 0, par = -1;

        for (int u = 0; u < V; u++)
        if (!visited[u])
            APUtil(adj, u, visited, disc, low,
                   time, par, isAP);
        for (int i = 0; i < V; i++){
            if (isAP[i] == 1)
                result.add(vertexList->get(i)->name);
        }
        delete vertexList;
        return result;
    }
    DLinkedList<Warehouse*> defense(){
        DLinkedList<Warehouse*>::Iterator it = this->plan->begin();
        DLinkedList<Warehouse*> result(NULL, &WHEQ);
        while (it != this->plan->end()) {
            UGraphModel<Warehouse*>::Iterator mapIt = this->map->begin();
            while(mapIt != this->map->end()){
                if ((*mapIt)->name == (*it)->name && (*mapIt)->soldiers <= (*it)->soldiers){
                    Warehouse* defWarehouse = new Warehouse((*it)->name, (*it)->soldiers-(*mapIt)->soldiers);
                    result.add(result.size(), defWarehouse);  
                }
                mapIt++; 
            }
            it++;
        }
        return result;
    }
};
void ctest(){
        DLinkedList<Warehouse*> list(NULL, &WHEQ);
        list.add(0, new Warehouse("A", 1000));
        list.add(0, new Warehouse("B", 1000));
        list.add(0, new Warehouse("C", 1000));
        list.add(0, new Warehouse("D", 1000));
        list.add(0, new Warehouse("F", 1000));
        UGraphModel<Warehouse*> model(&WHEQ, &vertexWH2str);
        model.add(new Warehouse("A", 100));
        model.add(new Warehouse("B", 600));
        model.add(new Warehouse("C", 400));
        model.add(new Warehouse("D", 100));
        model.add(new Warehouse("F", 200));
        Invasion* inv = new Invasion(&list, &model);
        DLinkedList<Warehouse*> res = inv->defense();
        DLinkedList<Warehouse*>::Iterator it = res.begin();
        while (it != res.end()){
            cout<<(*it)->println()<<endl;
            it++;
        }   
}
void btest(){
    DLinkedList<Warehouse*> list(NULL, &WHEQ);
        Warehouse* warehouseA = new Warehouse("A", 100);
        Warehouse* warehouseB = new Warehouse("B", 600);
        Warehouse* warehouseC = new Warehouse("C", 400);
        Warehouse* warehouseD = new Warehouse("D", 100);
        Warehouse* warehouseE = new Warehouse("E", 200);
        Warehouse* warehouseF = new Warehouse("F", 200);

        list.add(0, new Warehouse("A", 1000));
        list.add(0, new Warehouse("B", 1000));
        list.add(0, new Warehouse("C", 1000));
        list.add(0, new Warehouse("D", 1000));
        list.add(0, new Warehouse("F", 1000));
        UGraphModel<Warehouse*> model(&WHEQ, &vertexWH2str);
        model.add(warehouseA);
        model.add(warehouseB);
        model.add(warehouseC);
        model.add(warehouseD);
        model.add(warehouseF);
        model.add(warehouseE);
        model.connect(warehouseB, warehouseE);
        model.connect(warehouseA, warehouseF);
        model.connect(warehouseB, warehouseF);
        model.connect(warehouseC, warehouseF);
        model.connect(warehouseD, warehouseF);
        Invasion* inv = new Invasion(&list, &model);
        DLinkedList<string> Slist = inv->criticalPoint();
        for (int i = 0; i < Slist.size(); i++)
            cout<<Slist.get(i)<<" ";
}
int main(){
    btest();
}