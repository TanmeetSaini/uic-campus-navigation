#include <fstream>
#include <iomanip> /*setprecision*/
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "application.h"
#include "graph.h"

using namespace std;

string trim(const string &s) {
  const string whitespace = " \t\r\n";
  size_t start = s.find_first_not_of(whitespace);
  if (start == string::npos) {
    return "";
  }

  size_t end = s.find_last_not_of(whitespace);
  return s.substr(start, end - start + 1);
}

string loadEnvValue(const string &filename, const string &key) {
  ifstream input(filename);
  string line;

  while (getline(input, line)) {
    line = trim(line);
    if (line.empty() || line[0] == '#') {
      continue;
    }

    size_t equals = line.find('=');
    if (equals == string::npos) {
      continue;
    }

    string currentKey = trim(line.substr(0, equals));
    if (currentKey != key) {
      continue;
    }

    return trim(line.substr(equals + 1));
  }

  return "";
}

int main() {
  cout << "** Navigating UIC open street map **" << endl;
  cout << std::setprecision(8);

  string default_filename = loadEnvValue(".env", "OSM_DATA_FILE");
  if (default_filename.empty()) {
    default_filename = "data/uic-fa25.osm.json";
  }

  // Build graph from input data
  graph<long long, double> G;
  vector<BuildingInfo> buildings;
  unordered_map<long long, Coordinates> coords;
  ifstream input(default_filename);
  buildGraph(input, G, buildings, coords);

  cout << "# of buildings: " << buildings.size() << endl;

  cout << "# of vertices: " << G.numVertices() << endl;
  cout << "# of edges: " << G.numEdges() << endl;
  application(buildings, G);

  cout << "** Done **" << endl;
  return 0;
}
