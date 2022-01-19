#Rationale for datastructures used in the Datastructures-class

- Places are stored in an unordered map, where key is the ID of the place and value is a shared pointer to a Struct called Place. This is natural as the places are often searched by ID, but their order is not relevant. Unordered map is also relatively quick for storing and searching for this data.
- Pointer to the data is used to enable using the data in other structures as well when necessary without havind the data in two separate places.
- Places are also stored in three other datastructures- all of which are multimaps. Each has PlaceID as their value, but their keys vary from coordinate to name to type of the place. This lowers performance when adding places to datastructure, but makes listing places alphabetically or in coordinate order, or searching for places with specific name or type, quicker.
- This means that removing a place requires removing it from in fact four different structures, but as maps are relatively quick to work with this should not create too big problems performance-wise.

- Unordered map is also used with Areas, but the type Area also holds information of an areas parent and sub areas. This creates a tree-like on the side of the map-structure to enable quick adding and searching for parent and/or subareas.

