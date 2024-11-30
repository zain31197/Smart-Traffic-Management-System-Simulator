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
    int gettraveltime(string road1,string road2)
    {
        node *road1node=addnode(road1);
        edge *curr=road1node->head;
        while(curr!=NULL)
        {
            if(curr->u==road2)
            {
                return curr->weight_cost;
            }
            curr=curr->next;
        }
        return -1;
    }
    void updatetrafficweights(string road1,string road2,int newcost)
    {
        node *road1node=addnode(road1);
        edge *curr=road1node->head;
        while(curr!=NULL)
        {
            if(curr->u==road2)
            {
                curr->weight_cost=newcost;
                return ;
            }
            curr=curr->next;
        }
    }
};

class vehicle
{
    public:
    string vehicleid;
    string road1;
    string road2;
    vehicle *next;
    vehicle(string id,string road1,string road2)
    {
        this->vehicleid=id;
        this->road1=road1;
        this->road2=road2;
        this->next=NULL;
    }
};
class vehiclelist
{
    public:
    vehicle *head=NULL;
    void addvehicle(string id,string road1,string road2)
    {
        vehicle *NV=new vehicle(id,road1,road2);
        if(head==NULL)
        {
            head=NV;
        }
        else{
            vehicle *curr=head;
            while(curr->next!=NULL)
            {
                curr=curr->next;
            }
            curr->next=NV;
        }
    }
    void loadvehiclefromcsv(string filename)
    {
        ifstream file(filename);
        string line;
        getline(file,line);
        while(getline(file,line))
        {
            stringstream ss(line);
            string vehicleid,road1,road2;
            getline(ss,vehicleid,',');
            getline(ss,road1,',');
            getline(ss,road2,',');
            addvehicle(vehicleid,road1,road2);
        }
        file.close();
    }
    void displayvehicle()
    {
        vehicle *curr=head;
        while(curr!=NULL)
        {
            cout<<"Vehicle ID : "<<curr->vehicleid<<" start : "<<curr->road1<<" End : "<<curr->road2<<endl;
            curr=curr->next;
        }
    }
};
class emergencyvehicle
{
    public:
    string vehicleid;
    string road1;
    string road2;
    string priority;
    emergencyvehicle *next;
    emergencyvehicle(string id,string road1,string road2,string priority)
    {
        this->vehicleid=id;
        this->road1=road1;
        this->road2=road2;
        this->priority=priority;
        this->next=NULL;
    }
};
class emergencyvehiclefromcsv
{
    public:
    emergencyvehicle *head;
    emergencyvehiclefromcsv()
    {
        this->head=NULL;
    }
     void addemergencyvehicle(string id,string road1,string road2,string priority)
    {
        emergencyvehicle *NV=new emergencyvehicle(id,road1,road2,priority);
        if(head==NULL)
        {
            head=NV;
            return;
        }
        else{
            emergencyvehicle *curr=head;
            while(curr->next!=NULL)
            {
                curr=curr->next;
            }
            curr->next=NV;
        }
    }
    void loademergencyvehicleformcsv(string filename)
    {
        ifstream file(filename);
        string line;
        getline(file,line);
        while(getline(file,line))
        {
            stringstream ss(line);
            string vehicleid,road1,road2,prioritylevel;
            getline(ss,vehicleid,',');
             getline(ss,road1,',');
              getline(ss,road2,',');
               getline(ss,prioritylevel,',');
               addemergencyvehicle(vehicleid,road1,road2,prioritylevel);
        }
        file.close();
    }
    void displayemergencyvehicle()
    {
        emergencyvehicle *curr=head;
        while(curr!=NULL)
        {
             cout<<"Vehicle ID : "<<curr->vehicleid<<" start : "<<curr->road1<<" End : "<<curr->road2<<" PriorityLevel : "<<curr->priority<<endl;
             curr=curr->next;
        }
    }
};
class traficsignal
{
    public:
    string road;
    int greentime;
    traficsignal *next;
    traficsignal(string road,int greentime)
    {
        this->road=road;
        this->greentime=greentime;
        this->next=NULL;
    }
};
class trafficsignalgreenlist
{
    public:
    traficsignal *head;
    trafficsignalgreenlist()
    {
        this->head=NULL;
    }
    void addsignal(string road,int greentime)
    {
        traficsignal *tf=new traficsignal(road,greentime);
        if(head==NULL)
        {
            head=tf;
            return;
        }
        else{
            traficsignal *curr=head;
            while(curr->next!=NULL)
            {
                curr=curr->next;
            }
            curr->next=tf;
            return ;
        }
    }
    void readtrafficgreentimefromcsv(string filename)
    {
        ifstream file(filename);
        string line;
        getline(file,line);
        while(getline(file,line))
        {
            stringstream ss(line);
            string road;
            string greentimeinstring;
            getline(ss,road,',');
            getline(ss,greentimeinstring,',');
            int greentime=stoi(greentimeinstring);
            addsignal(road,greentime);
        }
        file.close();
    }
    void displaytrafficsignalstatus()
    {
        traficsignal *temp=head;
        while(temp!=NULL)
        {
            cout<<"Intersection "<<temp->road<<" Green Time: "<<temp->greentime<<endl;
            temp=temp->next;
        }

    }
};
int main() {
    string filename = "road_network.csv";
    roadmap r;
    r.loadgraphfromcsv(filename);
    r.displayroadnetwork();
    vehiclelist v;
    string filenamevehicle="vehicles.csv";
    v.loadvehiclefromcsv(filenamevehicle);
    v.displayvehicle();
    cout<<endl;
    emergencyvehiclefromcsv em;
    em.loademergencyvehicleformcsv("emergency_vehicles.csv");
    em.displayemergencyvehicle();
    trafficsignalgreenlist tf;
    tf.readtrafficgreentimefromcsv("traffic_signals.csv");
    tf.displaytrafficsignalstatus();
    return 0;
}
