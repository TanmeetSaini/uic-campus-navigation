# Campus Pathfinding in C++

This project uses graph algorithms to find paths between campus locations using OpenStreetMap data from UIC.

It builds a graph where:

- buildings are vertices
- footway points are vertices
- edges connect nearby points and buildings
- edge weights are distances between locations

The shortest paths are found with Dijkstra's algorithm.

## Features

- Search for campus buildings by name or abbreviation
- Find the closest building to a point on the map
- Find nearby buildings
- Find a route between two campus locations
- Run the project as either a console app or a local web app

## Files

- [main.cpp](/Users/tanmeets/Downloads/proj6-osm/main.cpp): console program
- [server.cpp](/Users/tanmeets/Downloads/proj6-osm/server.cpp): web server
- [application.cpp](/Users/tanmeets/Downloads/proj6-osm/application.cpp): graph building and pathfinding logic
- [graph.h](/Users/tanmeets/Downloads/proj6-osm/graph.h): graph data structure
- [tests](/Users/tanmeets/Downloads/proj6-osm/tests): test files
- [data](/Users/tanmeets/Downloads/proj6-osm/data): map data and small test datasets

## Build and Run

Run the console version:

```bash
make run_osm
```

Run the web server:

```bash
make run_server
```

Then open `http://localhost:1251` in your browser.

## Tests

Run all tests:

```bash
make test_all
```

Run specific groups:

```bash
make test_graph
make test_build_graph
make test_dijkstra
```

## Notes

- The app can read settings from `.env`, including `SERVER_HOST`, `SERVER_PORT`, `OSM_DATA_FILE`, `MAPBOX_TOKEN`, and `MAPBOX_STYLE_URL`.
- The main dataset is `data/uic-fa25.osm.json`.
- The web app uses a map interface to display locations and routes.
- The current test suite passes.
