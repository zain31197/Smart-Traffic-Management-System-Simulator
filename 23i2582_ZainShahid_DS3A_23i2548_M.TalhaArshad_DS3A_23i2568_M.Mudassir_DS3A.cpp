#include <iostream>
#include <fstream>
#include <limits.h>
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
struct roadStatus {
    string road1;
    string road2;
    string status;   // Status of the road: Blocked, Under Repair, Clear
    roadStatus *next;

    roadStatus(string r1, string r2, string s) : road1(r1), road2(r2), status(s), next(NULL) {}
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
struct PathNode {
    string road;
    PathNode *prev;
    int g_cost; // Cost from start to this node
    int f_cost; // Estimated total cost (g + heuristic)
    PathNode *next;

    PathNode(string r, int g, int f, PathNode *p = NULL) {
        road = r;
        g_cost = g;
        f_cost = f;
        prev = p;
        next = NULL;
    }
};
class roadmap {
public:
    node *head_n;
    roadStatus *statusHead;
    roadmap()
    {
        this->head_n=NULL;
        this->statusHead=NULL;
    }
public:
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
    node *newNode = new node(name);
    curr->next = newNode;
    return newNode;
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
   int heuristic(string, string) {
        return 0; // Placeholder heuristic function (Dijkstra-like)
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
        edge *edgePtr = curr->head;
        if (edgePtr != NULL) {
            cout << curr->name << " --> ";
            while (edgePtr != NULL) {
                cout << "(" << edgePtr->u << ", " << edgePtr->weight_cost << ")";
                edgePtr = edgePtr->next;
            }
            cout << endl;
        }
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
   
     PathNode* addPriorityNode(PathNode *head, string road, int g, int f, PathNode *prev) {
        PathNode *newNode = new PathNode(road, g, f, prev);
        if (head == NULL || head->f_cost > f) {
            newNode->next = head;
            return newNode;
        }
        PathNode *curr = head;
        while (curr->next != NULL && curr->next->f_cost <= f) {
            curr = curr->next;
        }
        newNode->next = curr->next;
        curr->next = newNode;
        return head;
    }

    bool contains(PathNode *head, string road) {
        PathNode *curr = head;
        while (curr != NULL) {
            if (curr->road == road) return true;
            curr = curr->next;
        }
        return false;
    }

    PathNode* findNode(PathNode *head, string road) {
        PathNode *curr = head;
        while (curr != NULL) {
            if (curr->road == road) return curr;
            curr = curr->next;
        }
        return NULL;
    }

    void aStarSearch(string start, string goal) {
        PathNode *openList = NULL;
        openList = addPriorityNode(openList, start, 0, heuristic(start, goal), NULL);

        PathNode *closedList = NULL;

        while (openList != NULL) {
            PathNode *current = openList;
            openList = openList->next;

            if (current->road == goal) {
                // Reconstruct path
                cout << "Path: ";
                while (current != NULL) {
                    cout << current->road;
                    if (current->prev != NULL) cout << " <- ";
                    current = current->prev;
                }
                cout << endl;
                return;
            }

            node *roadNode = addnode(current->road);
            edge *edgePtr = roadNode->head;
            while (edgePtr != NULL) {
                int tentative_g_cost = current->g_cost + edgePtr->weight_cost;
                int f_cost = tentative_g_cost + heuristic(edgePtr->u, goal);

                PathNode *existingNode = findNode(openList, edgePtr->u);
                if (existingNode == NULL || tentative_g_cost < existingNode->g_cost) {
                    if (existingNode == NULL) {
                        openList = addPriorityNode(openList, edgePtr->u, tentative_g_cost, f_cost, current);
                    } else {
                        existingNode->g_cost = tentative_g_cost;
                        existingNode->f_cost = f_cost;
                        existingNode->prev = current;
                    }
                }
                edgePtr = edgePtr->next;
            }
            current->next = closedList;
            closedList = current;
        }

        cout << "No path found from " << start << " to " << goal << endl;
    }
    ~roadmap() {
    node *curr = head_n;
    while (curr != NULL) {
        edge *e = curr->head;
        while (e != NULL) {
            edge *temp = e;
            e = e->next;
            delete temp;
        }
        node *temp = curr;
        curr = curr->next;
        delete temp;
    }
}

 void updatetrafficweights(string road1, string road2, int newcost) {
        node *road1node = addnode(road1);
        edge *curr = road1node->head;
        while (curr != NULL) {
            if (curr->u == road2) {
                curr->weight_cost = newcost;
                return;
            }
            curr = curr->next;
        }
    }
void addRoadStatus(string road1, string road2, string status) {
        roadStatus *newStatus = new roadStatus(road1, road2, status);
        if (statusHead == NULL) {
            statusHead = newStatus;
        } else {
            roadStatus *curr = statusHead;
            while (curr->next != NULL) {
                curr = curr->next;
            }
            curr->next = newStatus;
        }
    }

     void blockRoad(string road1, string road2) {
   
        string reasontoblockroad="";
        cout<<"---Enter the reason why you are blocking road---"<<endl;
        cin>>reasontoblockroad;
        addRoadStatus(road1,road2,reasontoblockroad);
        cout << "Roads between " << road1 << " and " << road2 << " have been blocked." << endl;
         removeSpecificRoad(road1,road2);
    }

    void applyRoadClosures(string filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Could not open file: " << filename << endl;
            return;
        }
        string line;
        getline(file, line); // Skip header line
        while (getline(file, line)) {
            stringstream ss(line);
            string road1, road2, status;
            getline(ss, road1, ',');
            getline(ss, road2, ',');
            getline(ss, status, ',');

            addRoadStatus(road1, road2, status); // Store the closure status
            
            if (status == "Blocked") {
                updatetrafficweights(road1, road2, INT_MAX); // Block the road
                removeSpecificRoad(road1,road2);
            } else if (status == "Under Repair") {
                updatetrafficweights(road1, road2, INT_MAX); // Delay the road
                removeSpecificRoad(road1,road2);
            }
            else{
                continue;
            }
            //  removeSpecificRoad(road1,road2);
        }
           
        file.close();
    }
    void removeSpecificRoad(string road1, string road2) {
    // Find the node corresponding to road1
    node* road1Node = head_n;
    while (road1Node != NULL && road1Node->name != road1) {
        road1Node = road1Node->next;
    }

    if (road1Node == NULL) {
        cout << "Road " << road1 << " not found!" << endl;
        return;
    }

    // Remove road2 from the adjacency list of road1
    edge* prevEdge = NULL;
    edge* currEdge = road1Node->head;
    while (currEdge != NULL) {
        if (currEdge->u == road2) {
            // Found the edge from road1 to road2, remove it
            if (prevEdge == NULL) {
                // The edge is the first one in the list
                road1Node->head = currEdge->next;
            } else {
                // The edge is somewhere in the middle or end
                prevEdge->next = currEdge->next;
            }
            // Free the memory used by the edge
            delete currEdge;
            // cout << "Removed road " << road1 << " to " << road2 << endl;
            return;
        }
        prevEdge = currEdge;
        currEdge = currEdge->next;
    }

    // cout << "Road " << road2 << " not found connected to " << road1 << endl;
}



    // Display the road statuses
    void displayRoadStatuses() {
        roadStatus *curr = statusHead;
        cout << "Road Closures:\n";
        while (curr != NULL) {
            cout << curr->road1 << " -> " << curr->road2 << ": " << curr->status << endl;
            curr = curr->next;
        }
    }

  void findPaths(node* currNode, string destination, string path[], int pathWeights[], int pathIndex, int weightIndex, string allPaths[][100], int allWeights[], int& pathCount) {
    // Base case: If the current node is the destination, add the path and its weight to the result
    if (currNode->name == destination) {
        // Store the path and its weight
        for (int i = 0; i < pathIndex; ++i) {
            allPaths[pathCount][i] = path[i];
        }
        allWeights[pathCount] = weightIndex;
        ++pathCount; // Increment the path count
        return;
    }

    // Mark the current node as visited
    edge* currEdge = currNode->head;
    while (currEdge != NULL) {
        // If the road is not blocked (weight is not INT_MAX), continue
        if (currEdge->weight_cost != INT_MAX) {
            // Add the connected road to the current path
            path[pathIndex] = currEdge->u;
            pathWeights[pathIndex] = currEdge->weight_cost;
            // Recursively find paths
            node* nextNode = addnode(currEdge->u);
            findPaths(nextNode, destination, path, pathWeights, pathIndex + 1, weightIndex + currEdge->weight_cost, allPaths, allWeights, pathCount);
        }
        currEdge = currEdge->next;
    }
}

void displayAllPaths(string source, string destination) {
    string path[100];
    int pathWeights[100];
    string allPaths[100][100];  
    int allWeights[100];  
    int pathCount = 0;  

    // Start DFS from the source node
    node* sourceNode = addnode(source);
    path[0] = source;  // Initialize the path with the source
    pathWeights[0] = 0;  // No weight at the source

    // Find all possible paths
    findPaths(sourceNode, destination, path, pathWeights, 1, 0, allPaths, allWeights, pathCount);

    // Display the paths and their total weight
    if (pathCount == 0) {
        cout << "No path found from " << source << " to " << destination << " without blocked roads." << endl;
    } else {
        cout << "All possible paths from " << source << " to " << destination << " excluding blocked roads are:" << endl;
        for (int i = 0; i < pathCount; ++i) {
            int totalWeight = 0;
            for (int j = 0; allPaths[i][j] != ""; ++j) {
                cout << allPaths[i][j] << " ";
                if (j > 0) totalWeight += pathWeights[j];
            }
            cout << " | Total Weight: " << totalWeight << endl;
        }
    }
}

    
};

class minheap {
public:
    struct nodeminheap {
        string road;
        int distance;
    } 
    heap[100]; 
    int size;
    minheap()
    {
        this->size=0;
    }

    void insert(string road, int distance) {
        heap[++size] = {road, distance};
        int i = size;
        while (i > 1 && heap[i / 2].distance > heap[i].distance) {
            swap(heap[i], heap[i / 2]);
            i /= 2;
        }
    }
    nodeminheap extractMin() {
        nodeminheap min = heap[1];
        heap[1] = heap[size--];
        int i = 1;
        while (2 * i <= size) {
            int smallest = i;
            if (heap[2 * i].distance < heap[smallest].distance) smallest = 2 * i;
            if (2 * i + 1 <= size && heap[2 * i + 1].distance < heap[smallest].distance) smallest = 2 * i + 1;
            if (smallest == i) break;
            swap(heap[i], heap[smallest]);
            i = smallest;
        }
        return min;
    }

    bool isEmpty() {
        return size == 0;
    }
};
void dijkstra(string source, roadmap &graph, string destination) {
    minheap pq;
    node *curr = graph.head_n;

    int distances[100]; 
    for (int i = 0; i < 100; i++) distances[i] = INT_MAX;
    distances[source[0] - 'A'] = 0;

    pq.insert(source, 0);

    while (!pq.isEmpty()) {
        minheap::nodeminheap minNode = pq.extractMin();
        string u = minNode.road;

        curr = graph.addnode(u);
        edge *e = curr->head;

        while (e != NULL) {
            string v = e->u;
            int weight = e->weight_cost;
            if (distances[u[0] - 'A'] + weight < distances[v[0] - 'A']) {
                distances[v[0] - 'A'] = distances[u[0] - 'A'] + weight;
                pq.insert(v, distances[v[0] - 'A']);
            }
            e = e->next;
        }
    }
    cout << "Shortest distance to " << destination << ": " << distances[destination[0] - 'A'] << endl;
}
void updateAndRecalculate(string road1, string road2, int newWeight, roadmap &graph, string source, string destination) {
    graph.updatetrafficweights(road1, road2, newWeight);
    dijkstra(source, graph, destination);
}
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
class TrafficSignalManager {

    trafficsignalgreenlist signalList;
    roadmap& roadNetwork;  // Reference to the road network

    // Custom priority queue for roads based on vehicle density
    class RoadPriorityQueue {
    public:
        struct RoadPriority {
            string road;
            int vehicleCount;
            int currentGreenTime;
        };
        RoadPriority heap[100];
        int size;

        RoadPriorityQueue() : size(0) {}

        void insert(string road, int vehicleCount, int currentGreenTime) {
            RoadPriority newEntry = {road, vehicleCount, currentGreenTime};
            heap[++size] = newEntry;
            
            // Bubble up based on vehicle count (higher count = higher priority)
            int i = size;
            while (i > 1 && heap[i/2].vehicleCount < heap[i].vehicleCount) {
                swap(heap[i], heap[i/2]);
                i /= 2;
            }
        }

        RoadPriority extractMax() {
            RoadPriority max = heap[1];
            heap[1] = heap[size--];
            
            // Heapify down
            int i = 1;
            while (2*i <= size) {
                int largest = i;
                if (2*i <= size && heap[2*i].vehicleCount > heap[largest].vehicleCount)
                    largest = 2*i;
                if (2*i+1 <= size && heap[2*i+1].vehicleCount > heap[largest].vehicleCount)
                    largest = 2*i+1;
                
                if (largest == i) break;
                
                swap(heap[i], heap[largest]);
                i = largest;
            }
            
            return max;
        }

              bool isEmpty() { return size == 0; }
    };

    // Count vehicles on a specific road
int countVehiclesOnRoad(string road1, string road2, vehiclelist& vehicles) {
    int count = 0;
    vehicle* curr = vehicles.head;

    while (curr != nullptr) {
        if ((curr->road1 == road1 && curr->road2 == road2) || 
            (curr->road1 == road2 && curr->road2 == road1)) {
            count++;
        }
        curr = curr->next;
    }
    return count;
}
public:
    TrafficSignalManager(roadmap& network) : roadNetwork(network) {}

    // Emergency vehicle override system
    void handleEmergencyVehicles(emergencyvehiclefromcsv& emergencyVehicles) {
        emergencyvehicle* curr = emergencyVehicles.head;
        while (curr != NULL) {
            if (curr->priority == "High") {
                // Override traffic signals for high-priority emergency vehicles
                roadNetwork.updatetrafficweights(curr->road1, curr->road2, 1);
                cout << "Emergency Override: Clearing path for vehicle " 
                     << curr->vehicleid << " from " << curr->road1 
                     << " to " << curr->road2 << endl;
            }
            curr = curr->next;
        }
    }

    // Dynamic traffic signal management
  void optimizeTrafficSignals(vehiclelist& vehicles) {
    cout << "\n--- Traffic Signal Optimization Report ---" << endl;
    RoadPriorityQueue priorityRoads;
    traficsignal* signal = signalList.head;

    // Detailed road and vehicle analysis
    while (signal != NULL) {
        node* currentNode = roadNetwork.addnode(signal->road);
        edge* connectedRoad = currentNode->head;
        int totalVehiclesAtIntersection = 0;

        cout << "Analyzing Intersection: " << signal->road 
             << " (Current Green Time: " << signal->greentime << " seconds)" << endl;

        while (connectedRoad != NULL) {
            // Count vehicles for the current connected road
            int vehicleCount = countVehiclesOnRoad(signal->road, connectedRoad->u, vehicles);
            totalVehiclesAtIntersection += vehicleCount;

            // Display details for each connected road
            cout << "  Connected Road: " << signal->road << " -> " << connectedRoad->u 
                 << ", Vehicle Count: " << vehicleCount 
                 << ", Travel Time: " << connectedRoad->weight_cost << " seconds" << endl;

            // Insert data into the priority queue for adjustment
            priorityRoads.insert(
                connectedRoad->u,
                vehicleCount,
                signal->greentime
            );

            connectedRoad = connectedRoad->next;
        }

        // Display total vehicle count for the intersection
        cout << "Total Vehicles at Intersection " << signal->road 
             << ": " << totalVehiclesAtIntersection << endl;

        signal = signal->next;
    }

    

    cout << "\n--- Traffic Signal Adjustments ---" << endl;
    // Adjust green times based on vehicle density
    while (!priorityRoads.isEmpty()) {
        RoadPriorityQueue::RoadPriority mostCongested = priorityRoads.extractMax();

        // Dynamically adjust green time based on vehicle density
        int adjustedGreenTime = mostCongested.currentGreenTime + 
                                (mostCongested.vehicleCount / 10); // Adjustment factor

        // Cap green time between minimum (15s) and maximum (60s)
        adjustedGreenTime = min(max(adjustedGreenTime, 15), 60);

        // Update traffic signal green time
        traficsignal* curr = signalList.head;
        while (curr != NULL) {
            if (curr->road == mostCongested.road) {
                int originalTime = curr->greentime;
                curr->greentime = adjustedGreenTime;

                // Display updated green time
                cout << "Intersection " << curr->road 
                     << ": Green Time Adjusted" << endl
                     << "  Original Time: " << originalTime << " seconds" << endl
                     << "  New Time: " << curr->greentime << " seconds" << endl;
                break;
            }
            curr = curr->next;
        }
    }
}

    // Load existing traffic signals
    void loadTrafficSignals(string filename) {
        signalList.readtrafficgreentimefromcsv(filename);
    }

    // Display current traffic signal status
    void displayTrafficSignals() {
        signalList.displaytrafficsignalstatus();
    }

void overrideSignalForRoad(string road, int overrideTime) {
        traficsignal* curr = signalList.head;
        while (curr != NULL) {
            if (curr->road == road) {
                int originalTime = curr->greentime;

                curr->greentime = overrideTime;

                cout << "\n--- Emergency Signal Override ---" << endl;
                cout << "Traffic signal for road " << road 
                     << " has been overridden for emergency use." << endl;
                cout << "  Original Green Time: " << originalTime << " seconds" << endl;
                cout << "  New Green Time: " << curr->greentime << " seconds" << endl;

                return;  
            }
            curr = curr->next;
        }

        // If the road was not found in the signal list
        cout << "Error: Road " << road << " not found in the traffic signal list." << endl;
    }

};



struct KeyValuePair {
    string key;
    int value;
    KeyValuePair* next;

    KeyValuePair(string k = "", int v = 0)
        : key(k), value(v), next(nullptr) {}
};

class Congestion {
private:
    static const int TABLE_SIZE = 100;

    KeyValuePair* table[TABLE_SIZE];

    unsigned int hash(const string& key) {
        unsigned int hashVal = 0;
        for (char ch : key) {
            hashVal = hashVal * 31 + ch;
        }
        return hashVal % TABLE_SIZE;
    }

public:
    // Trim function without using <algorithm>
    static string trim(const string& str) {
        size_t start = 0, end = str.length() - 1;

        // Remove leading spaces
        while (start < str.length() && (str[start] == ' ' || str[start] == '\t' || str[start] == '\n' || str[start] == '\r')) {
            ++start;
        }

        // Remove trailing spaces
        while (end > start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n' || str[end] == '\r')) {
            --end;
        }

        return str.substr(start, end - start + 1);
    }

    Congestion() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i] = nullptr;
        }
    }

    void insert(const string& key, int value = 1) {
        if (key.empty()) {
            cerr << "Attempted to insert an empty key!" << endl;
            return;
        }

        unsigned int index = hash(key);
        // cout << "Inserting key: '" << key << "' at index: " << index << endl;

        if (table[index] == nullptr) {
            table[index] = new KeyValuePair(key, value);
            // cout << "Inserted new key: '" << key << "' with value: " << value << endl;
            return;
        }

        KeyValuePair* current = table[index];
        while (current != nullptr) {
            if (current->key == key) {
                current->value += value;
                // cout << "Updated key: '" << key << "' to value: " << current->value << endl;
                return;
            }
            if (current->next == nullptr) break;
            current = current->next;
        }

        // Ensure proper memory allocation and check before dereferencing
        if (current == nullptr) {
            cerr << "Error: Unable to insert key due to null pointer." << endl;
            return;
        }

        current->next = new KeyValuePair(key, value);
        // cout << "Appended key: '" << key << "' with value: " << value << endl;
    }

    void displayEntries() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            KeyValuePair* current = table[i];
            while (current != nullptr) {
                if (!current->key.empty()) {
                    cout << current->key << ", Vehicles: " 
                         << current->value << endl;
                }
                current = current->next;
            }
        }
    }

    // Retrieve the congestion data of a specific road
    KeyValuePair* getRoadInfo(const string& road) {
        unsigned int index = hash(road);
        KeyValuePair* current = table[index];
        while (current != nullptr) {
            if (current->key == road) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }
    ~Congestion() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            KeyValuePair* current = table[i];
            while (current != nullptr) {
                KeyValuePair* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }
};

class CongestionMonitor {
private:
    Congestion roadCongestion;

public:
    void readTrafficData(const char* filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Unable to open file '" << filename << "'." << endl;
            return;
        }

        string line;
        getline(file, line); // Skip header

        while (getline(file, line)) {
            istringstream ss(line);
            string vehicleID, startIntersection, endIntersection;

            getline(ss, vehicleID, ',');
            getline(ss, startIntersection, ',');
            getline(ss, endIntersection, ',');

            // Trim spaces from intersections to avoid issues with leading/trailing spaces
            startIntersection = Congestion::trim(startIntersection);
            endIntersection = Congestion::trim(endIntersection);
            // cout << "Start Intersection: '" << startIntersection << "' , End Intersection: '" << endIntersection << "'" << endl;

            if (startIntersection.empty() || endIntersection.empty()) {
                // cout << "Invalid intersection data. Skipping line: " << line << endl;
                continue;
            }

            // Generate road key in the required format "start - end"
            string roadKey = startIntersection + " - " + endIntersection;
            // cout << "Generated RoadKey: '" << roadKey << "'" << endl;

            // Insert road key into congestion table
            roadCongestion.insert(roadKey);
        }
        

        file.close();
    }

       void addVehicleToRoad(const string& roadA, const string& roadB, int vehicleCount) {
    if (vehicleCount <= 0) {
        cout << "Error: Vehicle count must be greater than 0." << endl;
        return;
    }

    // Generate the road key in the "A - B" format
    string roadKey = Congestion::trim(roadA) + " - " + Congestion::trim(roadB);

    // Check if the road exists in the congestion data
    KeyValuePair* roadInfo =roadCongestion.getRoadInfo(roadKey);
    if (roadInfo != nullptr) {
        // Update the existing road's vehicle count
        roadCongestion.insert(roadKey, roadInfo->value + vehicleCount);
        cout << vehicleCount+1 << " vehicle(s) added to " << roadKey << "." << endl;
    } else {
        // If the road does not exist, add it with the specified vehicle count
        roadCongestion.insert(roadKey, vehicleCount);
        cout << "New road " << roadKey << " created with " << vehicleCount << " vehicle(s)." << endl;
    }
}

    void rerouteTraffic(const string& congestedRoad) {
        // Check congestion level
        KeyValuePair* roadInfo = roadCongestion.getRoadInfo(congestedRoad);
        if (roadInfo != nullptr && roadInfo->value >= 4) {
            cout << "Rerouting traffic from " << congestedRoad << " due to congestion." << endl;

            // Store the number of vehicles being rerouted
            int vehiclesToReroute = roadInfo->value;

            // Decrease vehicle count on the congested road by a professional rerouting amount (not zero)
            roadCongestion.insert(congestedRoad, -vehiclesToReroute);

            // Find an alternate road (dummy logic here)
            string alternateRoad = findAlternateRoad(congestedRoad);

            if (alternateRoad != "") {
                cout << "Traffic rerouted to: " << alternateRoad << endl;
                // Increase vehicle count on the alternate road
                roadCongestion.insert(alternateRoad, vehiclesToReroute);
            } else {
                cout << "No alternate road found for rerouting." << endl;
            }
        }
    }

    string findAlternateRoad(const string& congestedRoad) {
        // Here, you would implement logic to find an alternate route (for simplicity, just returning a dummy road)
        return "Alternate Road";
    }

    void analyzeCongestion() {
        cout << "\n----------------------------\n";
        cout << "Road Congestion Analysis\n";
        cout << "----------------------------\n";
        roadCongestion.displayEntries();
    }
};
int main() {
    roadmap roadNetwork;
    vehiclelist vehicleList;
    emergencyvehiclefromcsv emergencyVehicles;
    trafficsignalgreenlist signalList;
    TrafficSignalManager trafficSignalManager(roadNetwork);
    CongestionMonitor congestionMonitor;

    // Load initial data
    roadNetwork.loadgraphfromcsv("road_network.csv");
    // roadNetwork.applyRoadClosures("road_closures.csv");
    vehicleList.loadvehiclefromcsv("vehicles.csv");
    emergencyVehicles.loademergencyvehicleformcsv("emergency_vehicles.csv");
    signalList.readtrafficgreentimefromcsv("traffic_signals.csv");
    trafficSignalManager.loadTrafficSignals("traffic_signals.csv");
    congestionMonitor.readTrafficData("vehicles.csv");
    int mainChoice, subChoice;
    do {
        cout << "\n=== Smart Traffic Management System ===\n";
        cout << "1. Road Network Operations\n";
        cout << "2. Vehicle Management\n";
        cout << "3. Traffic Signal Management\n";
        cout << "4.Display Block Road Status\n";
        cout << "5. Emergency Vehicle Management\n";
        cout << "6. Congestion Management\n";
        cout << "7. Block a specific Road\n";
        cout << "8. Simulate Vehicle Routing\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
        cin >> mainChoice;

        switch(mainChoice) {
            case 1: // Road Network Operations
                do {
                    cout << "\n--- Road Network Operations ---\n";
                    cout << "1. Display Road Network\n";
                    cout << "2. Add Road\n";
                    cout << "3. Update Traffic Weights\n";
                    cout << "4. Block Road\n";
                    cout << "5. Display Road Statuses\n";
                    cout << "6. Find All Paths Between Intersections\n";
                    cout << "7. Shortest Path (Dijkstra)\n";
                    cout << "8. A* Search Path\n";
                    cout << "9. Return to Main Menu\n";
                    cout << "Enter your choice: ";
                    cin >> subChoice;

                    switch(subChoice) {
                        case 1: roadNetwork.displayroadnetwork(); break;
                        case 2: {
                            string road1, road2;
                            int weight;
                            cout << "Enter first intersection: ";
                            cin >> road1;
                            cout << "Enter second intersection: ";
                            cin >> road2;
                            cout << "Enter road weight: ";
                            cin >> weight;
                            roadNetwork.addedge(road1, road2, weight);
                            break;
                        }
                        case 3: {
                            string road1, road2;
                            int newWeight;
                            cout << "Enter first intersection: ";
                            cin >> road1;
                            cout << "Enter second intersection: ";
                            cin >> road2;
                            cout << "Enter new weight: ";
                            cin >> newWeight;
                            roadNetwork.updatetrafficweights(road1, road2, newWeight);
                            break;
                        }
                        
                        case 4: {
                            string road1, road2;
                            cout << "Enter first intersection: ";
                            cin >> road1;
                            cout << "Enter second intersection: ";
                            cin >> road2;
                            roadNetwork.blockRoad(road1, road2);
                            break;
                        }
                        case 5:
                         roadNetwork.applyRoadClosures("road_closures.csv");
                         roadNetwork.displayRoadStatuses(); 
                         break;
                        case 6: {
                            string start, end;
                            cout << "Enter start intersection: ";
                            cin >> start;
                            cout << "Enter end intersection: ";
                            cin >> end;
                            roadNetwork.displayAllPaths(start, end);
                            break;
                        }
                        case 7: {
                            string start, end;
                            cout << "Enter start intersection: ";
                            cin >> start;
                            cout << "Enter end intersection: ";
                            cin >> end;
                            dijkstra(start, roadNetwork, end);
                            break;
                        }
                        case 8: {
                            string start, end;
                            cout << "Enter start intersection: ";
                            cin >> start;
                            cout << "Enter end intersection: ";
                            cin >> end;
                            roadNetwork.aStarSearch(start, end);
                            break;
                        }
                    }
                } while (subChoice != 9);
                break;

            case 2: // Vehicle Management
                do {
                    cout << "\n--- Vehicle Management ---\n";
                    cout << "1. Display Vehicles\n";
                    cout << "2. Add Vehicle\n";
                    cout << "3. Load Vehicles from CSV\n";
                    cout << "4. Return to Main Menu\n";
                    cout << "Enter your choice: ";
                    cin >> subChoice;

                    switch(subChoice) {
                        case 1: vehicleList.displayvehicle(); break;
                        case 2: {
                            string id, road1, road2;
                            cout << "Enter Vehicle ID: ";
                            cin >> id;
                            cout << "Enter Start Road: ";
                            cin >> road1;
                            cout << "Enter End Road: ";
                            cin >> road2;
                            vehicleList.addvehicle(id, road1, road2);
                            break;
                        }
                        case 3: vehicleList.loadvehiclefromcsv("vehicles.csv"); break;
                    }
                } while (subChoice != 4);
                break;

            case 3: // Traffic Signal Management
                do {
                    cout << "\n--- Traffic Signal Management ---\n";
                    cout << "1. Display Traffic Signals\n";
                    cout << "2. Optimize Traffic Signals\n";
                    cout << "3. Load Traffic Signals from CSV\n";
                    cout << "4. Return to Main Menu\n";
                    cout << "Enter your choice: ";
                    cin >> subChoice;

                    switch(subChoice) {
                        case 1: trafficSignalManager.displayTrafficSignals(); break;
                        case 2: trafficSignalManager.optimizeTrafficSignals(vehicleList); break;
                        case 3: trafficSignalManager.loadTrafficSignals("traffic_signals.csv"); break;
                    }
                } while (subChoice != 4);
                break;
            case 4:
            {
                   do {
                    cout << "1. Display Block Road Status\n";
                    cout << "2. to return main menu\n";
                    cin >> subChoice;

                    switch(subChoice) {
                        case 1:
                         roadNetwork.applyRoadClosures("road_closures.csv");
                         roadNetwork.displayRoadStatuses(); 
                         break;
                    }
                } while (subChoice != 2);
                break;

            }
            case 5: // Emergency Vehicle Management
                do {
                    cout << "\n--- Emergency Vehicle Management ---\n";
                    cout << "1. Display Emergency Vehicles\n";
                    cout << "2. Add Emergency Vehicle\n";
                    cout << "3. Handle Emergency Vehicles\n";
                    cout << "4. Handle Emergency Vehicles Routing\n";
                    cout<<  "5. Emergency overirde signal for specific road"<<endl;
                    cout << "6. Return to Main Menu\n";
                    cout << "Enter your choice: ";
                    cin >> subChoice;

                    switch(subChoice) {
                        case 1: emergencyVehicles.displayemergencyvehicle(); break;
                        case 2: {
                            string id, road1, road2, priority;
                            cout << "Enter Vehicle ID: ";
                            cin >> id;
                            cout << "Enter Start Road: ";
                            cin >> road1;
                            cout << "Enter End Road: ";
                            cin >> road2;
                            cout << "Enter Priority (High/Medium/Low): ";
                            cin >> priority;
                            emergencyVehicles.addemergencyvehicle(id, road1, road2, priority);
                            break;
                        }
                        case 3: trafficSignalManager.handleEmergencyVehicles(emergencyVehicles); break;
                         case 4: {
                           string road1,road2;
                            cout<<"Enter the start and end intersection for emergency vehicle"<<endl;
                            cin>>road1;
                            cin>>road2;
                            roadNetwork.aStarSearch(road1,road2);
                            break;
                        }

                        case 5: {
                           string road1;
                           int overridetime;
                            cout<<"Enter the road where you want to override the signal for emergency purpose"<<endl;
                            cin>>road1;
                            cout<<"Enter the override time emergency purpose"<<endl;
                            cin>>overridetime;
                            
                            trafficSignalManager.overrideSignalForRoad(road1,overridetime);
                            break;
                        }
                    }
                } while (subChoice != 6);
                break;

            case 6: // Congestion Management
                do {
                    cout << "\n--- Congestion Management ---\n";
                    cout << "1. Display Congestion Report\n";
                    cout << "2. Reroute Congested Roads\n";
                    cout << "3. Add vehicle on Road\n";
                    cout << "4. Return to Main Menu\n";
                    cout << "Enter your choice: ";
                    cin >> subChoice;

                    switch(subChoice) {
                        case 1: 
                        {
                            congestionMonitor.analyzeCongestion();
                             break;
                        }
                            
                        case 2:
                        {
                        string road1, road2, key;
                        cout << "Enter Road1 to Road 2 where you want to reroute the traffic (format: B - D): " << endl;
                        cin >> road1;  // Get input for road1
                        cin.ignore();  // Ignore the space before the hyphen
                        cin >> ws;  // Skip any whitespace characters (including spaces, tabs, and newlines)
                        cin >> road2;  // Get input for road2
                        key = road1 + " - " + road2;  // Concatenate road1 and road2 with space around the hyphen
                         congestionMonitor.rerouteTraffic(key);
                         break;
                        }
                        case 3:
                        {
                             string road1, road2, key;
                            cout << "Enter Road1 to Road 2 where you want to add vehicles" << endl;
                            cin>>road1;
                            cin>>road2;
                            int vehiclecount=0;
                            cout<<"Enter the vehicle count"<<endl;
                            cin>>vehiclecount;
                            vehiclecount=vehiclecount-1;
                        congestionMonitor.addVehicleToRoad(road1,road2,vehiclecount);
                        break;
                        }
                    }
                } while (subChoice != 4);
                break;

                 case 7: // Congestion Management
                do {
                    cout << "1. Block a Road\n";
                    cout << "2. Return to Main Menu\n";
                    cout << "Enter your choice: ";
                    cin >> subChoice;

                    switch(subChoice) {
                        case 1: 
                        string road1,road2;
                        cout<<"Enter the road1 and road 2 name that will be blocked"<<endl;
                        cin>>road1;
                        cin>>road2;
                        roadNetwork.blockRoad(road1,road2);
                    }
                } while (subChoice != 2);
                break;

                case 8:
            {
                   do {
                    cout << "1. Simulate the vehicle Routing\n";
                    cout << "2. to return main menu\n";
                    cin >> subChoice;
                    switch(subChoice) {
                        case 1:
                          string road1,road2;
                     cout<<"Enter the road1 and road 2 from where you want to start routing and end "<<endl;
                        cin>>road1;
                        cin>>road2;
                        roadNetwork.displayAllPaths(road1,road2);
                         break;
                    }
                } while (subChoice != 2);
                break;
            }
                
        }
    } while (mainChoice != 9);

    return 0;
}
