# Campus Pathfinding in C++

This project is a C++ application that finds paths between campus locations using OpenStreetMap data from UIC. It builds a graph where buildings and footway points are represented as vertices, and edges connect nearby locations with weights based on distance. The program uses Dijkstra’s algorithm to compute the shortest path between two points on campus. It was developed using standard C++ along with custom graph data structures and JSON map data. The application can be run as either a console program or a local web server with a map interface.

Features
- Search for campus buildings by name or abbreviation
- Find the closest building to a point on the map
- Find nearby buildings
- Find a route between two campus locations
- Run the project as either a console app or a local web app
Files
- main.cpp: console program
- server.cpp: web server
- application.cpp: graph building and pathfinding logic
- graph.h: graph data structure
- tests: test files
- data: map data and small test datasets

Build and Run

Run the console version:

`make run_osm`

Run the web server:

`make run_server`

Then open http://localhost:1251
 in your browser.

Tests

Run all tests:

`make test_all`

Run specific groups:

`make test_graph
make test_build_graph
make test_dijkstra`

Notes
The app can read settings from .env, including SERVER_HOST, SERVER_PORT, OSM_DATA_FILE, MAPBOX_TOKEN, and MAPBOX_STYLE_URL.
The main dataset is data/uic-fa25.osm.json.
The web app uses a map interface to display locations and routes.
The current test suite passes.
