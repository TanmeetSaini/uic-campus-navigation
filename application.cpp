#include "application.h"

#include <iostream>
#include <limits>
#include <map>
#include <queue> // priority_queue
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "dist.h"
#include "graph.h"
#include "json.hpp"
using json = nlohmann::json;

using namespace std;

double INF = numeric_limits<double>::max();

void buildGraph(istream &input, graph<long long, double> &g,
                vector<BuildingInfo> &buildings,
                unordered_map<long long, Coordinates> &coords) {
  // TODO_STUDENT
  buildings.clear();
  coords.clear();

  json data;
  input>>data;
  for(const auto& b : data["buildings"]){
    long long id = b["id"];
    BuildingInfo bInfo;
    bInfo.name = b["name"];
    bInfo.id = id;
    bInfo.abbr = b["abbr"];
    bInfo.location.lat = b["lat"];
    bInfo.location.lon = b["lon"];
    buildings.push_back(bInfo);
    g.addVertex(id);
  }
  for (const auto& wp : data["waypoints"]) {
    long long id = wp["id"];
    double lat = wp["lat"];
    double lon = wp["lon"];
    coords[id] = Coordinates(lat, lon);
    g.addVertex(id);
  }
  for (const auto& footway : data["footways"]){
    for(size_t i = 0; i + 1 < footway.size(); i++){
      long long from  = footway[i];
      long long to = footway[i+1];
      double dist = distBetween2Points(coords[from], coords[to]);
      g.addEdge(from, to, dist);
      g.addEdge(to, from, dist);
    }
  }
  double MAX_DIST = 0.036;
  for (const auto& building : data["buildings"]){
    long long buildingId = building["id"];
    double buildingLat = building["lat"];
    double buildingLon = building["lon"];
    for (const auto& [wayPointId, wayPointCoord] : coords){
      double dist = distBetween2Points({buildingLat, buildingLon}, wayPointCoord);
      if(dist < MAX_DIST){
        g.addEdge(buildingId, wayPointId, dist);
        g.addEdge(wayPointId, buildingId, dist);
      }
    }
  }
}

BuildingInfo getBuildingInfo(const vector<BuildingInfo> &buildings,
                             const string &query) {
  for (const BuildingInfo &building : buildings) {
    if (building.abbr == query) {
      return building;
    } else if (building.name.find(query) != string::npos) {
      return building;
    }
  }
  BuildingInfo fail;
  fail.id = -1;
  return fail;
}

BuildingInfo getClosestBuilding(const vector<BuildingInfo> &buildings,
                                Coordinates c) {
  double minDestDist = INF;
  BuildingInfo ret = buildings.at(0);
  for (const BuildingInfo &building : buildings) {
    double dist = distBetween2Points(building.location, c);
    if (dist < minDestDist) {
      minDestDist = dist;
      ret = building;
    }
  }
  return ret;
}






class prioritize {
 public:
  bool operator()(const pair<long long, double>& p1,
                  const pair<long long, double>& p2) const {
    return p1.second > p2.second;
  }
};


vector<long long> dijkstra(const graph<long long, double> &G, long long start,
                           long long target,
                           const set<long long> &ignoreNodes) {

  unordered_map<long long,double> dist;
  priority_queue<pair<long long, double>,
               vector<pair<long long, double>>,
               prioritize>
    worklist;

  for (auto vert:G.getVertices()) {
    dist[vert] = INF;
  }
  dist[start] = 0;
  worklist.push({start,0});

  if (start == target) {
    return {start};
  }
  unordered_map<long long,long long> previous;
  while (worklist.empty() == false) {
    auto top = worklist.top();
    worklist.pop();
    auto curr = top.first;
    if (ignoreNodes.count(curr) == 1 && curr != start && curr != target) {
      continue;
    }
    for (auto vert:G.neighbors(curr)) {
      if (ignoreNodes.count(vert) == 1 && vert != target) {
        continue;
      }
      double weight;
      if (G.getWeight(curr,vert,weight) == true) {
        double newDistance = dist[curr] + weight;
        if (newDistance < dist[vert]) {
          dist[vert] = newDistance;
          previous[vert] = curr;
          worklist.push({vert,newDistance});
        }
      }
    }
  }
  if (dist[target] == INF) {
    return {};
  }
  vector<long long> path;
  while (target != start) {
    path.insert(path.begin(),target);
    target = previous[target];
  }
  path.insert(path.begin(),start);

  return path;
}










double pathLength(const graph<long long, double> &G,
                  const vector<long long> &path) {
  double length = 0.0;
  double weight;
  for (size_t i = 0; i + 1 < path.size(); i++) {
    bool res = G.getWeight(path.at(i), path.at(i + 1), weight);
    if (!res) {
      return -1;
    }
    length += weight;
  }
  return length;
}

void outputPath(const vector<long long> &path) {
  for (size_t i = 0; i < path.size(); i++) {
    cout << path.at(i);
    if (i != path.size() - 1) {
      cout << "->";
    }
  }
  cout << endl;
}

// Honestly this function is just a holdover from an old version of the project
void application(const vector<BuildingInfo> &buildings,
                 const graph<long long, double> &G) {
  string person1Building, person2Building;

  set<long long> buildingNodes;
  for (const auto &building : buildings) {
    buildingNodes.insert(building.id);
  }

  cout << endl;
  cout << "Enter person 1's building (partial name or abbreviation), or #> ";
  getline(cin, person1Building);

  while (person1Building != "#") {
    cout << "Enter person 2's building (partial name or abbreviation)> ";
    getline(cin, person2Building);

    // Look up buildings by query
    BuildingInfo p1 = getBuildingInfo(buildings, person1Building);
    BuildingInfo p2 = getBuildingInfo(buildings, person2Building);
    Coordinates P1Coords, P2Coords;
    string P1Name, P2Name;

    if (p1.id == -1) {
      cout << "Person 1's building not found" << endl;
    } else if (p2.id == -1) {
      cout << "Person 2's building not found" << endl;
    } else {
      cout << endl;
      cout << "Person 1's point:" << endl;
      cout << " " << p1.name << endl;
      cout << " " << p1.id << endl;
      cout << " (" << p1.location.lat << ", " << p1.location.lon << ")" << endl;
      cout << "Person 2's point:" << endl;
      cout << " " << p2.name << endl;
      cout << " " << p2.id << endl;
      cout << " (" << p2.location.lon << ", " << p2.location.lon << ")" << endl;

      Coordinates centerCoords = centerBetween2Points(p1.location, p2.location);
      BuildingInfo dest = getClosestBuilding(buildings, centerCoords);

      cout << "Destination Building:" << endl;
      cout << " " << dest.name << endl;
      cout << " " << dest.id << endl;
      cout << " (" << dest.location.lat << ", " << dest.location.lon << ")"
           << endl;

      vector<long long> P1Path = dijkstra(G, p1.id, dest.id, buildingNodes);
      vector<long long> P2Path = dijkstra(G, p2.id, dest.id, buildingNodes);

      // This should NEVER happen with how the graph is built
      if (P1Path.empty() || P2Path.empty()) {
        cout << endl;
        cout << "At least one person was unable to reach the destination "
                "building. Is an edge missing?"
             << endl;
        cout << endl;
      } else {
        cout << endl;
        cout << "Person 1's distance to dest: " << pathLength(G, P1Path);
        cout << " miles" << endl;
        cout << "Path: ";
        outputPath(P1Path);
        cout << endl;
        cout << "Person 2's distance to dest: " << pathLength(G, P2Path);
        cout << " miles" << endl;
        cout << "Path: ";
        outputPath(P2Path);
      }
    }

    //
    // another navigation?
    //
    cout << endl;
    cout << "Enter person 1's building (partial name or abbreviation), or #> ";
    getline(cin, person1Building);
  }
}
