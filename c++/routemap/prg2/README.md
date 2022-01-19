# File containing rationale for datastructures and other desicions made in Datastructures class

Datastructures
- Ways are stored separately to an unordered map to allow quick insert and search of a single way, and effective iteration in case all of them are listed
- Endpoints of ways are also stored to a separate datastructure as Crossroads. They are also stored in an unordered map to allow quick insert and search
- Way holds information of its length, which is calculated when ways are added, because it doesn't cost too much time and it is convenient to have it calculated, since it is needed in several occasions
- Crossroad has a list of ways leaving from it, and paired with the way is a pointer to the crossroad it leads to
- Crossroad has fields for different information to be used in algorithms, such as its 'color', pointer to the crossroad from where it was last arrived to from, ID of the way it was last arrived to via, and its distance from the start of the algorithm.


Algorithms
- Searching for any route between two coordinates is done with DFS-algorithm that stops when the goal is first found. This is done to keep it as time efficient as possible. The route is then parsed recursively starting from end and working towards beginning.
- Searching for routes with shortest distance or least crossroads is done using Dijkstras algorithm, with the difference that in the first case, distance from start is determined by actual distance from start, and in the latter, by number of crossroads from start. This distance also serves as the priority factor in Dijkstras algrotihm for the priority queue.
- This is achieved in the implementation of Dijkstra by checking which is used, and calculating distance from current node to the next either as one in the case of crossroads, or as the length of the way between the two in the case of actual distance.
- This allows using very similar implementation in both cases, and hence a method shortestRoute was added to handle the basic stuff, such as checking if both crossroads exist, and calling necessary methods for initializing crossroads and parsing the route.
