#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

class edge {
public:
    string u;
    int weight_cost;
    edge *next;

    edge(string v, int w) {
        this->u = v;
        this->weight_cost = w;
        this->next = NULL;
    }
};

class node {
public:
    string name;
    edge *head;
    node *next;

    node(string nameofroad) {
        this->name = nameofroad;
        head = NULL;
        next = NULL;
    }
};

class roadmap {
public:
    node *head_n = NULL;
   node* addnode(string name) {
    node *curr = head_n;
    if (curr == NULL) {
        head_n = new node(name);
        return head_n;
    }
    while (curr->next != NULL) {
        if (curr->name == name) {
            return curr; 
        }
        curr = curr->next;
    }
    if (curr->name == name) {
        return curr;
    }
    curr->next = new node(name);
    return curr->next;
}


    void addedge(string road1, string road2, int weight_cost) {
    node *road1node = addnode(road1);
    edge *curr = road1node->head;
    if (curr == NULL) {
        road1node->head = new edge(road2, weight_cost);
    } else {
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = new edge(road2, weight_cost);
    }
}


    void loadgraphfromcsv(string filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Could not open file: " << filename << endl;
            return;
        }
        string line;
        getline(file, line); 

        while (getline(file, line)) {
            stringstream ss(line);
            string road1, road2;
            string weight_str;
            int weight_cost;

            getline(ss, road1, ',');
            getline(ss, road2, ',');
            getline(ss, weight_str, ',');  // weight should be extracted fully as string first.
            weight_cost = stoi(weight_str);  // convert weight to int properly

            addedge(road1, road2, weight_cost);
        }
        file.close();
    }

    void displayroadnetwork() {
        node *curr = head_n;
        while (curr != NULL) {
            cout << curr->name << " --> ";
            edge *edge = curr->head;
            while (edge != NULL) {
                cout << "(" << edge->u << ", " << edge->weight_cost << ")";
                edge = edge->next;
            }
            cout << endl;
            curr = curr->next;
        }
    }
};

int main() {
    string filename = "road_network.csv";
    roadmap r;
    r.loadgraphfromcsv(filename);
    r.displayroadnetwork();
    return 0;
}
