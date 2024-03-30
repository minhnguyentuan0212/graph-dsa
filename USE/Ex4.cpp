#include "UGraphModel.h"
#include <iostream>
#include <string>
using namespace std;
bool strEQ(string& lhs, string& rhs){
    return lhs.compare(rhs) == 0;
}
string vertex2str(string& item){
    return item;
}

class People {
public:
    string name;
    DLinkedList<string> *preferList;
public:
    People(string name, DLinkedList<string> *preferList):name(name), preferList(preferList){}   
    ~People(){
        if (preferList) delete preferList;
    }
};
bool PPEQ(People* &rhs, People* &lhs){
    return rhs->name.compare(lhs->name) == 0;
}
class StableMarriage{
public:
    int n;
    DLinkedList<People*> *men;
    DLinkedList<People*> *women;
public:
    StableMarriage(int n, DLinkedList<People*> *men, DLinkedList<People*> *women):n(n), men(men), women(women){}
    ~StableMarriage(){
        if (men) delete men;
        if (women) delete women;
    }
    bool wPrefersM1OverM(int **prefer, int w, int m, int m1) {
        for (int i = 0; i < n; i++){
            if (prefer[w][i] == m1)
                return true;
            if (prefer[w][i] == m)
                return false;
        }
    }
    string stableMarriage(int **prefer, string name, DLinkedList<string> *nameList)
{
    int wPartner[n];
    bool mFree[n];
    for (int i = 0; i < n; i++) {
        wPartner[i] = -1;
        mFree[i] = false;
    }
    
    int freeCount = n;

    while (freeCount > 0)
    {
        int m;
        for (m = 0; m < n; m++)
            if (mFree[m] == false)
                break;
        for (int i = 0; i < n && mFree[m] == false; i++)
        {
            int w = prefer[m][i];
            if (wPartner[w-n] == -1)
            {
                wPartner[w-n] = m;
                mFree[m] = true;
                freeCount--;
            }
 
            else
            {
                int m1 = wPartner[w-n];
                if (wPrefersM1OverM(prefer, w, m, m1) == false)
                {
                    wPartner[w-n] = m;
                    mFree[m] = true;
                    mFree[m1] = false;
                }
            }
        } 
    } 
    int i = 0;
    cout << "Woman   Man" << endl;
            for (int i = 0; i < n; i++)
            cout << " " << i + n << "\t" << wPartner[i] << endl;
    if (nameList->indexOf(name) < n) {
        for ( ; i < n; i++) {
            if (wPartner[i] == nameList->indexOf(name)) 
            {
                i += n;
                break;
            }
        }
    }
    else i = wPartner[i-n];
    return nameList->get(i);
}
    string fiancee(string name) {
        DLinkedList<string> *nameList = new DLinkedList<string>(NULL, &strEQ);
        DLinkedList<People*>::Iterator It = men->begin();
        while (It != men->end()){
            nameList->add((*It)->name);
            It++;
        }
        It = women->begin();
        while (It != women->end()){
            nameList->add((*It)->name);
            It++;
        }
        int **couple = new int*[2*n];
        for (int i = 0; i < 2*n; i++) 
            couple[i] = new int[n];
        int i = 0;
        It = men->begin();
        for (; i < n; i++){
            DLinkedList<string>::Iterator pIt = (*It)->preferList->begin();
            for (int j = 0; j < n; j++){
                couple[i][j] = nameList->indexOf(*pIt);
                pIt++;
            }
            It++;
        }
        It = women->begin();
        for (; i < 2*n; i++){
            DLinkedList<string>::Iterator pIt = (*It)->preferList->begin();
            for (int j = 0; j < n; j++){
                couple[i][j] = nameList->indexOf(*pIt);
                pIt++;
            }
            It++;
        }
        return stableMarriage(couple, name, nameList);
    }
};
int main(){
    DLinkedList<string> *men1 = new DLinkedList<string>(NULL, &strEQ);
    men1->add("XQ");
    men1->add("TN");
    men1->add("VA");
    men1->add("KH");

    DLinkedList<string> *men2 = new DLinkedList<string>(NULL, &strEQ);
    men2->add("TN");
    men2->add("KH");
    men2->add("VA");
    men2->add("XQ");

    DLinkedList<string> *men3 = new DLinkedList<string>(NULL, &strEQ);
    men3->add("KH");
    men3->add("TN");
    men3->add("VA");
    men3->add("XQ");

    DLinkedList<string> *men4 = new DLinkedList<string>(NULL, &strEQ);
    men4->add("KH");
    men4->add("TN");
    men4->add("VA");
    men4->add("XQ");

    DLinkedList<string> *women1 = new DLinkedList<string>(NULL, &strEQ);
    women1->add("Minh");
    women1->add("Dung");
    women1->add("Hao");
    women1->add("Duc");

    DLinkedList<string> *women2 = new DLinkedList<string>(NULL, &strEQ);
    women2->add("Minh");
    women2->add("Dung");
    women2->add("Hao");
    women2->add("Duc");

    DLinkedList<string> *women3 = new DLinkedList<string>(NULL, &strEQ);
    women3->add("Minh");
    women3->add("Dung");
    women3->add("Hao");
    women3->add("Duc");
    
    DLinkedList<string> *women4 = new DLinkedList<string>(NULL, &strEQ);
    women4->add("Minh");
    women4->add("Dung");
    women4->add("Hao");
    women4->add("Duc");

    People m1("Minh", men1);
    People m2("Dung", men2);
    People m3("Hao", men3);
    People m4("Duc", men4);
    People w1("KH", women1);
    People w2("TN", women2);
    People w3("VA", women3);
    People w4("XQ", women4);
    DLinkedList<People*>* menlist = new DLinkedList<People*>(NULL, &PPEQ);
    menlist->add(&m1);
    menlist->add(&m2);
    menlist->add(&m3);
    menlist->add(&m4);
    DLinkedList<People*>* womenlist = new DLinkedList<People*>(NULL, &PPEQ);
    womenlist->add(&w1);
    womenlist->add(&w2);
    womenlist->add(&w3);
    womenlist->add(&w4);
    StableMarriage model(4, menlist, womenlist);
    cout<<model.fiancee("Minh");

}