# A* Search Algorithm

The algorithm implemented throughout Udacity C++ nanodegree.

A* is an informed search algorithm, or a best-first search,meaning that it is formulated in terms of weighted graphs: starting from a specific starting node of a graph, it aims to find a path to the given goal node having the smallest cost (least distance travelled, shortest time, etc.)

At each iteration of its main loop, A* needs to determine which of its paths to extend. It does so based on the cost of the path and an estimate of the cost required to extend the path all the way to the goal. Specifically, A* selects the path that minimizes
f(n)=g(n)+h(n)
where n is the next node on the path, g(n) is the cost of the path from the start node to n, and h(n) is a heuristic function that estimates the cost of the cheapest path from n to the goal.

For more details, please visit the following wikipedia link :
* [A * Search Algorithm](https://en.wikipedia.org/wiki/A*_search_algorithm)

For the given test file (test.txt), the following output has been produced.


🚦  ⛰️   0   0   0   0   
🚗  ⛰️   0   0   0   0   
🚗  ⛰️   0   0   0   0   
🚗  🚗  🚗   ⛰️   0   0  
0   ⛰️  🚗   🚗   🚗  🏁 

