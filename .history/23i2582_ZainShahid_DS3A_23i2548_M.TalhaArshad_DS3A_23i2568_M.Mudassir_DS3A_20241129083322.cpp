#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

class PriorityQueue {
private:
    struct QueueNode {
        string roadName;
        int vehicleDensity;
        int waitTime;
        QueueNode* next;

        QueueNode(string road, int density, int wait = 0) : 
            roadName(road), 
            vehicleDensity(density), 
            waitTime(wait),
            next(NULL) {}
    };

    QueueNode* front;
    int size;

    void bubbleSort() {
        if (!front || !front->next) return;

        bool swapped;
        QueueNode* ptr1;
        QueueNode* lptr = NULL;

        do {
            swapped = false;
            ptr1 = front;

            while (ptr1->next != lptr) {
              
                if (ptr1->vehicleDensity < ptr1->next->vehicleDensity || 
                    (ptr1->vehicleDensity == ptr1->next->vehicleDensity && 
                     ptr1->waitTime < ptr1->next->waitTime)) {
                    
                  
                    swap(ptr1->roadName, ptr1->next->roadName);
                    swap(ptr1->vehicleDensity, ptr1->next->vehicleDensity);
                    swap(ptr1->waitTime, ptr1->next->waitTime);
                    
                    swapped = true;
                }
                ptr1 = ptr1->next;
            }
            lptr = ptr1;
        } while (swapped);
    }

public:
    PriorityQueue() : front(NULL), size(0) {}

    void push(string roadName, int vehicleDensity, int waitTime = 0) {
        QueueNode* newNode = new QueueNode(roadName, vehicleDensity, waitTime);
        
        if (!front || vehicleDensity > front->vehicleDensity) {
            newNode->next = front;
            front = newNode;
        } else {
            QueueNode* current = front;
            while (current->next && current->next->vehicleDensity >= vehicleDensity) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
        size++;
        bubbleSort(); 
    }

    void pop() {
        if (!front) return;
        QueueNode* temp = front;
        front = front->next;
        delete temp;
        size--;
    }

    string topRoad() {
        return front ? front->roadName : "";
    }

    int topDensity() {
        return front ? front->vehicleDensity : 0;
    }

    bool isEmpty() {
        return front == NULL;
    }

    int getSize() {
        return size;
    }
};

class TrafficSignal {
private:
    string intersection;
    int baseGreenDuration;
    int currentGreenDuration;
    int emergencyPriority;
    bool isEmergencyMode;
    int consecutiveOverrides;

public:
    TrafficSignal() : 
        intersection(""), 
        baseGreenDuration(30),
        currentGreenDuration(30),
        emergencyPriority(0),
        isEmergencyMode(false),
        consecutiveOverrides(0) {}

    TrafficSignal(string name, int initialDuration = 30) : 
        intersection(name),
        baseGreenDuration(initialDuration),
        currentGreenDuration(initialDuration),
        emergencyPriority(0),
        isEmergencyMode(false),
        consecutiveOverrides(0) {}

    void adjustSignalTiming(int congestionFactor) {
       
        int dynamicAdjustment = congestionFactor / 10;
        currentGreenDuration = baseGreenDuration + dynamicAdjustment;

        if (currentGreenDuration < 10) currentGreenDuration = 10;
        if (currentGreenDuration > 90) currentGreenDuration = 90;

        cout << "Signal at " << intersection 
             << " adjusted to " << currentGreenDuration 
             << " seconds (Congestion Factor: " << congestionFactor << ")" << endl;
    }

    void setEmergencyOverride(int priority) {
        if (priority > emergencyPriority) {
            isEmergencyMode = true;
            emergencyPriority = priority;
            currentGreenDuration = 5; 
            consecutiveOverrides++;

            cout << "EMERGENCY OVERRIDE: " << intersection 
                 << " - Priority " << priority 
                 << ", Consecutive Overrides: " << consecutiveOverrides << endl;
        }
    }

    void resetEmergencyMode() {
        isEmergencyMode = false;
        emergencyPriority = 0;
        currentGreenDuration = baseGreenDuration;
        consecutiveOverrides = 0;

        cout << "Normal traffic flow restored at " << intersection << endl;
    }

    string getIntersection() const { return intersection; }
    int getCurrentGreenDuration() const { return currentGreenDuration; }
    bool isInEmergencyMode() const { return isEmergencyMode; }
};

class TrafficManagementSystem {
private:
    PriorityQueue congestionQueue;
    TrafficSignal* intersections;
    int intersectionCount;
    int maxIntersections;

public:
    TrafficManagementSystem(int capacity = 50) {
        maxIntersections = capacity;
        intersections = new TrafficSignal[maxIntersections];
        intersectionCount = 0;
    }

    ~TrafficManagementSystem() {
        delete[] intersections;
    }

    void addIntersection(string name, int initialGreenTime = 30) {
        if (intersectionCount < maxIntersections) {
            intersections[intersectionCount++] = TrafficSignal(name, initialGreenTime);
        }
    }

    void reportTrafficDensity(string roadName, int vehicleCount, int waitTime = 0) {
        congestionQueue.push(roadName, vehicleCount, waitTime);
    }

    void optimizeTrafficFlow() {
       
        PriorityQueue tempQueue;
        
        while (!congestionQueue.isEmpty()) {
            string road = congestionQueue.topRoad();
            int density = congestionQueue.topDensity();

            for (int i = 0; i < intersectionCount; i++) {
                if (road + " Intersection" == intersections[i].getIntersection()) {
                    intersections[i].adjustSignalTiming(density);
                    break;
                }
            }

            tempQueue.push(road, density);
            congestionQueue.pop();
        }

        while (!tempQueue.isEmpty()) {
            string road = tempQueue.topRoad();
            int density = tempQueue.topDensity();
            congestionQueue.push(road, density);
            tempQueue.pop();
        }
    }

    void handleEmergencyVehicle(string intersection, int priority) {
        for (int i = 0; i < intersectionCount; i++) {
            if (intersections[i].getIntersection() == intersection) {
                intersections[i].setEmergencyOverride(priority);
                break;
            }
        }
    }

    void restoreNormalTraffic(string intersection) {
        for (int i = 0; i < intersectionCount; i++) {
            if (intersections[i].getIntersection() == intersection) {
                intersections[i].resetEmergencyMode();
                break;
            }
        }
    }
};

int main() {
    TrafficManagementSystem trafficSystem;

    trafficSystem.addIntersection("Main Street Intersection");
    trafficSystem.addIntersection("Broadway Intersection");
    trafficSystem.addIntersection("Oak Avenue Intersection");

    trafficSystem.reportTrafficDensity("Main Street", 180);
    trafficSystem.reportTrafficDensity("Broadway", 220);
    trafficSystem.reportTrafficDensity("Oak Avenue", 90);

    trafficSystem.optimizeTrafficFlow();

    trafficSystem.handleEmergencyVehicle("Broadway Intersection", 5);

    trafficSystem.restoreNormalTraffic("Broadway Intersection");

    return 0;
}