# Smart Traffic Management System Simulator
# Project Description
The Smart Traffic Management System Simulator is a console-based simulation designed to optimize urban traffic flow in a city using advanced data structures and algorithms. It replicates real-world traffic scenarios, including dynamic vehicle routing, traffic signal management, congestion monitoring, emergency handling, and road disruption simulations. The project focuses on real-time traffic control without predictive features.
# Team Members
Zain Shahid
M. Talha Arshad
M. Mudassir
# Features and Modules
# 1. City Traffic Network (Graph Representation)
Purpose: Represent the city as a weighted, directed graph.
Key Components:
Nodes: Represent intersections.
Edges: Represent roads between intersections, with weights representing travel time or congestion levels.
Features:
Dynamic addition and removal of roads or intersections.
Text-based or graphical visualization of the graph structure.
# 2. Vehicle Routing System
Purpose: Provide efficient routes for vehicles dynamically.
Key Features:
Use Dijkstra’s Algorithm for shortest-path calculation.
Dynamic route recalculation based on traffic conditions.
Vehicle tracking to monitor movement across the network.
# 3. Traffic Signal Management
Purpose: Control traffic lights at intersections to minimize congestion.
Key Features:
Priority Queue: Manage incoming roads based on vehicle density.
Dynamic green signal duration adjustment.
Emergency override system for critical situations.
# 4. Congestion Monitoring
Purpose: Continuously track and analyze road congestion levels.
Key Features:
Use a hash table to track vehicle counts for each road segment.
Identify congested roads and reroute traffic using BFS or DFS algorithms.
Visual representation of congestion levels.
# 5. Emergency Vehicle Handling
Purpose: Ensure minimal delays for emergency vehicles.
Key Features:
Override normal signal operations to clear a path.
Use A Search Algorithm* to calculate the fastest route.
Restore normal traffic flow after the emergency vehicle passes.
# 6. Accident and Road Closure Simulation
Purpose: Simulate real-world disruptions like accidents and road closures.
Key Features:
Block specific roads or intersections dynamically.
Recalculate routes and update the traffic network.
Monitor system performance under these conditions.
# 7. Simulation Dashboard
Purpose: Provide an interactive interface for simulation control and visualization.
Key Features:
Display traffic flow, congestion levels, and signal statuses.
Allow manual addition or removal of vehicles.
Generate logs of all system actions, including rerouting and signal adjustments.
Implementation Details

# Programming Language: C++
# Data Structures Used:

Graph: Represent traffic infrastructure.
Priority Queue: Manage traffic signal priorities.
Hash Table: Track vehicle counts and congestion.
Heap: Optimize route recalculations.
Algorithms:
Dijkstra’s Algorithm: Optimal route calculation.
A Search Algorithm*: Emergency vehicle routing.
BFS/DFS: Congestion-based rerouting.
Input Handling: File-based initialization for road networks, vehicles, and disruptions.

# Setup Instructions
Clone the project repository from GitHub.
bash
Copy code
git clone <https://github.com/zain31197/data-structures-project.git>
Compile the code using a C++ compiler.
bash
Copy code
g++ -o simulator main.cpp
Run the executable file.
bash
Copy code
./simulator
How to Use the Simulator
Graph Visualization: View the current traffic network structure.
Add/Remove Roads or Intersections: Modify the network dynamically.
Simulate Traffic: Add vehicles and track their movement across the network.
Emergency Handling: Test the emergency vehicle routing system.
Accident Simulation: Block roads and observe the system's response.
Monitor Congestion: Identify and analyze congested areas in real time.
Adjust Signals: View and modify traffic signal durations.
Logs and Reports
The simulator generates logs for:

Rerouting decisions.
Signal changes.
Emergency vehicle handling.
Road closure impacts.
# Acknowledgment
This project was completed as part of the Data Structures course under the guidance of our instructor. Special thanks to team members for their contributions and dedication.
Contact
For queries, feel free to contact us via GitHub or email:
# Zain Shahid: i232582@isb.nu.edu.pk
# M. Talha Arshad: i232548@isb.nu.edu.pk
# M. Mudassir: i232562@isb.nu.edu.pk
