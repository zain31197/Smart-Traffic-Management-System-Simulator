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
     updatetrafficweights(road1, road2, INT_MAX);

        string reasontoblockroad="";
        cout<<"---Enter the reason why you are blocking road---"<<endl;
        cin>>reasontoblockroad;
        addRoadStatus(road1,road2,reasontoblockroad);
        cout << "Roads between " << road1 << " and " << road2 << " have been blocked." << endl;
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
                updateTrafficWeights(road1, road2, INT_MAX); // Block the road
            } else if (status == "Under Repair") {
                updateTrafficWeights(road1, road2, INT_MAX); // Delay the road
            }
             removeSpecificRoad(road1,road2);
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
            cout << "Removed road " << road1 << " to " << road2 << endl;
            return;
        }
        prevEdge = currEdge;
        currEdge = currEdge->next;
    }

    cout << "Road " << road2 << " not found connected to " << road1 << endl;
}

 void updateTrafficWeights(string road1, string road2, int newCost) {
        node *road1Node = addnode(road1);
        edge *curr = road1Node->head;
        while (curr != NULL) {
            if (curr->u == road2) {
                curr->weight_cost = newCost;
                return;
            }
            curr = curr->next;
        }
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
    // Arrays to store the paths and their weights
    string path[100];  // Assuming a maximum of 100 roads in a path
    int pathWeights[100];
    string allPaths[100][100];  // Store up to 100 paths
    int allWeights[100];  // Store the total weight of each path
    int pathCount = 0;  // To track the number of found paths

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
private:
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
};


class CongestionMonitor {
private:
    // Custom hash table-like structure for vehicle counts
    struct RoadSegment {
        string road1;
        string road2;
        int vehicleCount;
        int congestionLevel;
        RoadSegment* next;

        RoadSegment(string start, string end) : 
            road1(start), road2(end), vehicleCount(0), 
            congestionLevel(0), next(NULL) {}
    };

    RoadSegment* head;
    roadmap& roadNetwork;

    // Hash function to distribute road segments
    int hash(string road1, string road2) {
        int hash = 0;
        for (char c : road1) hash += c;
        for (char c : road2) hash += c;
        return hash % 100;
    }

    // Find or create road segment entry
    RoadSegment* findOrCreateRoadSegment(string road1, string road2) {
        RoadSegment* curr = head;
        while (curr != NULL) {
            if ((curr->road1 == road1 && curr->road2 == road2) ||
                (curr->road1 == road2 && curr->road2 == road1)) {
                return curr;
            }
            curr = curr->next;
        }

        // Create new road segment if not found
        RoadSegment* newSegment = new RoadSegment(road1, road2);
        newSegment->next = head;
        head = newSegment;
        return newSegment;
    }

public:
    CongestionMonitor(roadmap& network) : roadNetwork(network), head(NULL) {}

    // Update vehicle count for a road segment
   void updateVehicleCount(vehiclelist& vehicles) {
    // Reset counts first
    RoadSegment* curr = head;
    while (curr != NULL) {
        curr->vehicleCount = 0;
        curr->congestionLevel = 0;
        curr = curr->next;
    }

    // Count vehicles on each road segment
    vehicle* vehiclePtr = vehicles.head;
    while (vehiclePtr != NULL) {
        RoadSegment* segment = findOrCreateRoadSegment(
            vehiclePtr->road1, 
            vehiclePtr->road2
        );
        segment->vehicleCount++;
        
        // Simple congestion level calculation
        int travelTime = roadNetwork.gettraveltime(
            vehiclePtr->road1, 
            vehiclePtr->road2
        );
        
        // Congestion levels: 
        // 0-2 vehicles: Low
        // 3-5 vehicles: Medium
        // 6+ vehicles: High
        if (segment->vehicleCount <= 2) 
            segment->congestionLevel = 1; // Low
        else if (segment->vehicleCount <= 5) 
            segment->congestionLevel = 2; // Medium
        else 
            segment->congestionLevel = 3; // High

        vehiclePtr = vehiclePtr->next;
    }
}

    // Reroute traffic from congested roads
    string findAlternativeRoute(string start, string end) {
    const int MAX_NODES = 100; // Adjust based on the size of the graph
    bool visited[MAX_NODES] = {false}; // Track visited nodes
    string nodeNames[MAX_NODES];       // Map indices to node names
    int nodeCount = 0;                 // Total number of nodes

    // Populate nodes and indices
    node* nodes[MAX_NODES];
    node* current = roadNetwork.head_n;  // Assuming roadNetwork has a head node
    while (current != NULL && nodeCount < MAX_NODES) {
        nodes[nodeCount] = current;
        nodeNames[nodeCount] = current->name;
        nodeCount++;
        current = current->next;
    }

    // Find the start node's index
    int startIndex = -1;
    for (int i = 0; i < nodeCount; i++) {
        if (nodeNames[i] == start) {
            startIndex = i;
            break;
        }
    }

    if (startIndex == -1) {
        return ""; // Start node not found
    }

    // Queue for BFS
    struct QueueNode {
        string nodeName;
        int nodeIndex;
    };

    QueueNode queue[MAX_NODES];
    int front = 0, rear = 0;

    // Enqueue the start node
    queue[rear++] = {start, startIndex};
    visited[startIndex] = true;

    // Perform BFS
    while (front < rear) {
        QueueNode currentNode = queue[front++];
        node* currentGraphNode = nodes[currentNode.nodeIndex];
        edge* connectedRoads = currentGraphNode->head;

        // Explore connected roads
        while (connectedRoads != NULL) {
            string neighbor = connectedRoads->u;

            // Find the neighbor index
            int neighborIndex = -1;
            for (int i = 0; i < nodeCount; i++) {
                if (nodeNames[i] == neighbor) {
                    neighborIndex = i;
                    break;
                }
            }

            if (neighborIndex == -1 || visited[neighborIndex]) {
                connectedRoads = connectedRoads->next;
                continue;
            }

            // Skip the original congested end node
            if (neighbor != end) {
                return neighbor; // Found a valid alternative route
            }

            // Mark as visited and enqueue
            visited[neighborIndex] = true;
            queue[rear++] = {neighbor, neighborIndex};

            connectedRoads = connectedRoads->next;
        }
    }

    return ""; // No alternative route found
}
void rerouteCongestedRoads(vehiclelist& vehicles) {
    cout << "\n--- Congestion Rerouting Report ---" << endl;

    RoadSegment* curr = head;
    while (curr != NULL) {
        if (curr->congestionLevel >= 3) {
            cout << "High Congestion Detected: " 
                 << curr->road1 << " - " << curr->road2 
                 << " (Vehicles: " << curr->vehicleCount << ")" << endl;

            // Find alternative routes using BFS
            string altRoute1 = findAlternativeRoute(curr->road1, curr->road2);

            if (!altRoute1.empty()) {
                // Update vehicle routes
                vehicle* vehiclePtr = vehicles.head;
                while (vehiclePtr != NULL) {
                    if ((vehiclePtr->road1 == curr->road1 && 
                         vehiclePtr->road2 == curr->road2) ||
                        (vehiclePtr->road1 == curr->road2 && 
                         vehiclePtr->road2 == curr->road1)) {
                        
                        // Reroute logic
                        vehiclePtr->road1 = curr->road1;
                        vehiclePtr->road2 = altRoute1;

                        cout << "  Rerouted Vehicle " 
                             << vehiclePtr->vehicleid 
                             << " via " << altRoute1 << endl;
                    }
                    vehiclePtr = vehiclePtr->next;
                }
            }
        }
        curr = curr->next;
    }
}


    // Display congestion report
void displayCongestionReport() {
    cout << "---------- Congestion Status ------" << endl;
    RoadSegment* curr = head;
    while (curr != nullptr) {
        cout << "-------------------------------------------------Segment: " << curr->road1 << " - " << curr->road2 
             << ", Vehicles: " << curr->vehicleCount 
             << ", Congestion Level: " << curr->congestionLevel << endl;
        curr = curr->next;
    }
}


    // Destructor to free memory
    ~CongestionMonitor() {
        while (head) {
            RoadSegment* temp = head;
            head = head->next;
            delete temp;
        }
    }
};



// int main() {
//     roadmap roadNetwork;
//     vehiclelist vehicleList;
//     emergencyvehiclefromcsv emergencyVehicles;
//     trafficsignalgreenlist signalList;
   
    
//     // Load data
//     roadNetwork.loadgraphfromcsv("road_network.csv");
//     vehicleList.loadvehiclefromcsv("vehicles.csv");
//     emergencyVehicles.loademergencyvehicleformcsv("emergency_vehicles.csv");
//   signalList.readtrafficgreentimefromcsv("traffic_signals.csv");
//     // cout << "Road Network:\n";
//     // roadNetwork.displayroadnetwork();
//     // cout << "\nVehicles:\n";
//     // vehicleList.displayvehicle();
//     // cout << "\nTraffic Signal Status:\n";
//     // signalList.displaytrafficsignalstatus();
//     // dijkstra("A",roadNetwork,"D");
//     // Create traffic signal manager
//     TrafficSignalManager trafficManager(roadNetwork);
//     trafficManager.loadTrafficSignals("traffic_signals.csv");

//     // Demonstrate traffic signal optimization
//     // cout << "Initial Traffic Signals:" << endl;
//     // trafficManager.displayTrafficSignals();

//     // Optimize traffic signals based on vehicle density
//     // trafficManager.optimizeTrafficSignals(vehicleList);

//     // Handle emergency vehicles
// //     trafficManager.handleEmergencyVehicles(emergencyVehicles);

// //     cout << "\nOptimized Traffic Signals:" << endl;
// //     trafficManager.displayTrafficSignals();
// //      // Create Congestion Monitor
// //     CongestionMonitor congestionMonitor(roadNetwork);
// //    // Make sure this is called before displaying the report
// // congestionMonitor.updateVehicleCount(vehicleList);
// // congestionMonitor.displayCongestionReport();

// //     congestionMonitor.rerouteCongestedRoads(vehicleList);
// //     // Display updated vehicle list after rerouting
// //     cout << "\nUpdated Vehicles after Rerouting:" << endl;
// //     vehicleList.displayvehicle();
// //      roadNetwork.aStarSearch("A", "Z");
//   roadNetwork.applyRoadClosures("road_closures.csv");
//   roadNetwork.displayRoadStatuses();
//   roadNetwork.displayroadnetwork();
//   roadNetwork.blockRoad("A","B");
//   roadNetwork.displayRoadStatuses();
//   roadNetwork.displayAllPaths("A","K");
//    dijkstra("A",roadNetwork,"K");
//   roadNetwork.aStarSearch("A","K");
//     return 0;
// }


void simulationDashboard(roadmap& roadNetwork, vehiclelist& vehicleList, trafficsignalgreenlist& signalList);
void cityTrafficNetwork(roadmap& roadNetwork);
void trafficSignalManagement(roadmap& roadNetwork, trafficsignalgreenlist& signalList);
void congestionStatus(roadmap& roadNetwork, vehiclelist& vehicleList);
void emergencyVehicleRouting(roadmap& roadNetwork, emergencyvehiclefromcsv& emergencyVehicles);
void blockRoad(roadmap& roadNetwork);
void simulateVehicleRouting(roadmap& roadNetwork);

int main() {
    roadmap roadNetwork;
    vehiclelist vehicleList;
    emergencyvehiclefromcsv emergencyVehicles;
    trafficsignalgreenlist signalList;

    // Load initial data
    roadNetwork.loadgraphfromcsv("road_network.csv");
    vehicleList.loadvehiclefromcsv("vehicles.csv");
    emergencyVehicles.loademergencyvehicleformcsv("emergency_vehicles.csv");
    signalList.readtrafficgreentimefromcsv("traffic_signals.csv");

    int choice;
    do {
        cout << "\n=== Smart Traffic Management System Simulator ===\n";
        cout << "1. Simulation Dashboard\n";
        cout << "2. City Traffic Network\n";
        cout << "3. Traffic Signal Management\n";
        cout << "4. Congestion Status\n";
        cout << "5. Emergency Vehicle Routing\n";
        cout << "6. Block a Road\n";
        cout << "7. Simulate Vehicle Routing (All Paths)\n";
        cout << "8. Exit Simulation\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                simulationDashboard(roadNetwork, vehicleList, signalList);
                break;
            case 2:
                cityTrafficNetwork(roadNetwork);
                break;
            case 3:
                trafficSignalManagement(roadNetwork, signalList);
                break;
            case 4:
                congestionStatus(roadNetwork, vehicleList);
                break;
            case 5:
                emergencyVehicleRouting(roadNetwork, emergencyVehicles);
                break;
            case 6:
                blockRoad(roadNetwork);
                break;
            case 7:
                simulateVehicleRouting(roadNetwork);
                break;
            case 8:
                cout << "Exiting simulation. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 8);

    return 0;
}

// Module implementations
void simulationDashboard(roadmap& roadNetwork, vehiclelist& vehicleList, trafficsignalgreenlist& signalList) {
    cout << "\n--- Simulation Dashboard ---\n";
    roadNetwork.displayroadnetwork();
    vehicleList.displayvehicle();
    signalList.displaytrafficsignalstatus();
}

void cityTrafficNetwork(roadmap& roadNetwork) {
    cout << "\n--- City Traffic Network ---\n";
    roadNetwork.displayroadnetwork();
}

void trafficSignalManagement(roadmap& roadNetwork, trafficsignalgreenlist& signalList) {
    cout << "\n--- Traffic Signal Management ---\n";
    signalList.displaytrafficsignalstatus();
    // Add features like optimization or updates here
}

void congestionStatus(roadmap& roadNetwork, vehiclelist& vehicleList) {
    cout << "\n--- Congestion Status ---\n";
    CongestionMonitor congestionMonitor(roadNetwork);
    congestionMonitor.updateVehicleCount(vehicleList);
    congestionMonitor.displayCongestionReport();
}

void emergencyVehicleRouting(roadmap& roadNetwork, emergencyvehiclefromcsv& emergencyVehicles) {
    cout << "\n--- Emergency Vehicle Routing ---\n";
    TrafficSignalManager trafficManager(roadNetwork);
    trafficManager.handleEmergencyVehicles(emergencyVehicles);
}

void blockRoad(roadmap& roadNetwork) {
    cout << "\n--- Block a Road ---\n";
    string start, end;
    cout << "Enter the road to block (start and end intersection): ";
    cin >> start >> end;
    roadNetwork.blockRoad(start, end);
    roadNetwork.displayRoadStatuses();
}

void simulateVehicleRouting(roadmap& roadNetwork) {
    cout << "\n--- Simulate Vehicle Routing (All Paths) ---\n";
    string start, end;
    cout << "Enter start intersection: ";
    cin >> start;
    cout << "Enter end intersection: ";
    cin >> end;
    roadNetwork.displayAllPaths(start,end);
    dijkstra(start, roadNetwork, end);
    roadNetwork.aStarSearch(start, end);
}
