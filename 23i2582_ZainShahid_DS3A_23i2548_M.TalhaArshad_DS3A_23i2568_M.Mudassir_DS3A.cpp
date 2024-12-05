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
<<<<<<< Updated upstream
};

<<<<<<< Updated upstream
=======
    ~roadmap() {
    while (head_n) {
        node* temp = head_n;
        head_n = head_n->next;

        edge* currEdge = temp->head;
        while (currEdge) {
            edge* tempEdge = currEdge;
            currEdge = currEdge->next;
            delete tempEdge;
        }
        delete temp;
    }
}
};

=======
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
<<<<<<< Updated upstream
=======

>>>>>>> Stashed changes
=======

>>>>>>> Stashed changes
class vehiclelist
{
    public:
    vehicle *head=NULL;
<<<<<<< Updated upstream
<<<<<<< Updated upstream
=======
  
>>>>>>> Stashed changes
=======
  
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
<<<<<<< Updated upstream
=======
       
>>>>>>> Stashed changes
=======
       
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
};
<<<<<<< Updated upstream
=======
    ~vehiclelist() {
    while (head) {
        vehicle* temp = head;
        head = head->next;
        delete temp;
    }
}
};

>>>>>>> Stashed changes
=======

>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
};
<<<<<<< Updated upstream
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
=======
    ~trafficsignalgreenlist() {
    while (head) {
        traficsignal* temp = head;
        head = head->next;
        delete temp;
    }
}
};

=======
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
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


// Existing classes from previous implementation remain the same...

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
    void rerouteCongestedRoads(vehiclelist& vehicles) {
        cout << "\n--- Congestion Rerouting Report ---" << endl;
        
        RoadSegment* curr = head;
        while (curr != NULL) {
            if (curr->congestionLevel >= 3) {
                cout << "High Congestion Detected: " 
                     << curr->road1 << " - " << curr->road2 
                     << " (Vehicles: " << curr->vehicleCount << ")" << endl;

                // Find alternative routes
                string altRoute1 = findAlternativeRoute(
                    curr->road1, 
                    curr->road2
                );

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

    // Find alternative route using simple path exploration
    string findAlternativeRoute(string start, string end) {
        node* startNode = roadNetwork.addnode(start);
        edge* connectedRoads = startNode->head;

        while (connectedRoads != NULL) {
            // Prefer roads with lower travel times
            if (connectedRoads->u != end) {
                return connectedRoads->u;
            }
            connectedRoads = connectedRoads->next;
        }
        return ""; // No alternative found
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

// Modify main function to include congestion monitoring


// Modified main function to demonstrate new functionality
int main() {
    roadmap roadNetwork;
    vehiclelist vehicleList;
    emergencyvehiclefromcsv emergencyVehicles;
    trafficsignalgreenlist signalList;
   
    
    // Load data
    roadNetwork.loadgraphfromcsv("road_network.csv");
    vehicleList.loadvehiclefromcsv("vehicles.csv");
    emergencyVehicles.loademergencyvehicleformcsv("emergency_vehicles.csv");
  signalList.readtrafficgreentimefromcsv("traffic_signals.csv");
    cout << "Road Network:\n";
    roadNetwork.displayroadnetwork();
    cout << "\nVehicles:\n";
    vehicleList.displayvehicle();
    cout << "\nTraffic Signal Status:\n";
    signalList.displaytrafficsignalstatus();
    dijkstra("A",roadNetwork,"D");
    // Create traffic signal manager
    TrafficSignalManager trafficManager(roadNetwork);
    trafficManager.loadTrafficSignals("traffic_signals.csv");

    // Demonstrate traffic signal optimization
    cout << "Initial Traffic Signals:" << endl;
    trafficManager.displayTrafficSignals();

    // Optimize traffic signals based on vehicle density
    trafficManager.optimizeTrafficSignals(vehicleList);

    // Handle emergency vehicles
    trafficManager.handleEmergencyVehicles(emergencyVehicles);

    cout << "\nOptimized Traffic Signals:" << endl;
    trafficManager.displayTrafficSignals();
     // Create Congestion Monitor
    CongestionMonitor congestionMonitor(roadNetwork);

    // Update and analyze congestion
    congestionMonitor.updateVehicleCount(vehicleList);
    congestionMonitor.displayCongestionReport();
    
    // Reroute congested roads
    congestionMonitor.rerouteCongestedRoads(vehicleList);

    // Display updated vehicle list after rerouting
    cout << "\nUpdated Vehicles after Rerouting:" << endl;
    vehicleList.displayvehicle();

>>>>>>> Stashed changes
    return 0;
}


=======
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


// Existing classes from previous implementation remain the same...

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
    void rerouteCongestedRoads(vehiclelist& vehicles) {
        cout << "\n--- Congestion Rerouting Report ---" << endl;
        
        RoadSegment* curr = head;
        while (curr != NULL) {
            if (curr->congestionLevel >= 3) {
                cout << "High Congestion Detected: " 
                     << curr->road1 << " - " << curr->road2 
                     << " (Vehicles: " << curr->vehicleCount << ")" << endl;

                // Find alternative routes
                string altRoute1 = findAlternativeRoute(
                    curr->road1, 
                    curr->road2
                );

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

    // Find alternative route using simple path exploration
    string findAlternativeRoute(string start, string end) {
        node* startNode = roadNetwork.addnode(start);
        edge* connectedRoads = startNode->head;

        while (connectedRoads != NULL) {
            // Prefer roads with lower travel times
            if (connectedRoads->u != end) {
                return connectedRoads->u;
            }
            connectedRoads = connectedRoads->next;
        }
        return ""; // No alternative found
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

int main() {
    roadmap roadNetwork;
    vehiclelist vehicleList;
    emergencyvehiclefromcsv emergencyVehicles;
    trafficsignalgreenlist signalList;
   
    
    // Load data
    roadNetwork.loadgraphfromcsv("road_network.csv");
    vehicleList.loadvehiclefromcsv("vehicles.csv");
    emergencyVehicles.loademergencyvehicleformcsv("emergency_vehicles.csv");
  signalList.readtrafficgreentimefromcsv("traffic_signals.csv");
    cout << "Road Network:\n";
    roadNetwork.displayroadnetwork();
    cout << "\nVehicles:\n";
    vehicleList.displayvehicle();
    cout << "\nTraffic Signal Status:\n";
    signalList.displaytrafficsignalstatus();
    dijkstra("A",roadNetwork,"D");
    // Create traffic signal manager
    TrafficSignalManager trafficManager(roadNetwork);
    trafficManager.loadTrafficSignals("traffic_signals.csv");

    // Demonstrate traffic signal optimization
    cout << "Initial Traffic Signals:" << endl;
    trafficManager.displayTrafficSignals();

    // Optimize traffic signals based on vehicle density
    trafficManager.optimizeTrafficSignals(vehicleList);

    // Handle emergency vehicles
    trafficManager.handleEmergencyVehicles(emergencyVehicles);

    cout << "\nOptimized Traffic Signals:" << endl;
    trafficManager.displayTrafficSignals();
     // Create Congestion Monitor
    CongestionMonitor congestionMonitor(roadNetwork);
    congestionMonitor.updateVehicleCount(vehicleList);
    congestionMonitor.displayCongestionReport();
    
    congestionMonitor.rerouteCongestedRoads(vehicleList);

    // Display updated vehicle list after rerouting
    cout << "\nUpdated Vehicles after Rerouting:" << endl;
    vehicleList.displayvehicle();

    return 0;
}
>>>>>>> Stashed changes
