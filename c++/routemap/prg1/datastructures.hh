// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <memory>
#include <unordered_map>
#include <map>
#include <math.h>

// Types for IDs
using PlaceID = long int;
using AreaID = long int;
using Name = std::string;
using WayID = std::string;

// Return values for cases where required thing was not found
PlaceID const NO_PLACE = -1;
AreaID const NO_AREA = -1;
WayID const NO_WAY = "!!No way!!";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Enumeration for different place types
// !!Note since this is a C++11 "scoped enumeration", you'll have to refer to
// individual values as PlaceType::SHELTER etc.
enum class PlaceType { OTHER=0, FIREPIT, SHELTER, PARKING, PEAK, BAY, AREA, NO_TYPE };

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    double dist1 = sqrt(pow(c1.x, 2) + pow(c1.y, 2));
    double dist2 = sqrt(pow(c2.x, 2) + pow(c2.y, 2));

    if ( dist1 < dist2 ) { return true; }
    else if ( dist2 < dist1 ) { return false; }
    else { return c1.y <= c2.y; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Duration is unknown
Distance const NO_DISTANCE = NO_VALUE;

//Types for a place and an area

struct Place
{
    PlaceType type;
    Name name;
    Coord coord;
};

struct Area
{
    AreaID id;
    Name name;
    std::vector<Coord> coords;
    std::shared_ptr<Area> parentArea = nullptr;
    std::vector<std::shared_ptr<Area>> subAreas = {};
};

struct DistanceData
{
    Distance distance;
    int y;
};

inline bool operator<(DistanceData d1, DistanceData d2)
{
    Distance distance1 = d1.distance;
    Distance distance2 = d2.distance;

    if( distance1 == distance2 ){
        return d1.y < d2.y;
    }

    return distance1 < distance2;
}

// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: constant
    // Short rationale for estimate: returns std::unordered_map.size(), which
    //     has constant complexity
    int place_count();

    // Estimate of performance: linear
    // Short rationale for estimate: performs four clear methods which are linear
    void clear_all();

    // Estimate of performance: linear in context of number of places
    // Short rationale for estimate: loops through places in data structure
    //     and pushes ID of each to a vector, which has constant complexity
    std::vector<PlaceID> all_places();

    // Estimate of performance: average O(log n), worst case O(places_.size())
    // Short rationale for estimate: performs new Place initialization and
    //     inserts that to an unordered map and two multimaps, where insertion
    //     to a multimap is logarithmic and insertion to unordered map
    //     is amortized constant on average and linear in worst case
    bool add_place(PlaceID id, Name const& name, PlaceType type, Coord xy);

    // Estimate of performance: average Theta(1), worst case O(n)
    // Short rationale for estimate: find-method for an unordered map has complexity
    //      of constant on average, and linear in the worst case
    std::pair<Name, PlaceType> get_place_name_type(PlaceID id);

    // Estimate of performance: average Theta(1), worst case O(n)
    // Short rationale for estimate: find-method for an unordered map has complexity
    //      of constant on average, and linear in the worst case
    Coord get_place_coord(PlaceID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: linear in context of number of places
    // Short rationale for estimate: loops through datastructure containing places
    //      in alphabetical order and pushes id of each to a vector
    std::vector<PlaceID> places_alphabetically();

    // Estimate of performance: linear in context of number of places
    // Short rationale for estimate: loops through datastructure containing places
    //      in coordinate order and pushes id of each to a vector
    std::vector<PlaceID> places_coord_order();

    // Estimate of performance: worst case linear, on average somewhere between
    //      logarithmic and linear, best case constant
    // Short rationale for estimate: calls equal_range for multimap which is
    //      logarithmic, and then loops through given range. In worst case
    //      the range will contain all places, but on average can be assumed to
    //      be less. In best case nothing is done if number of places is zero
    //
    //      This reasoning also stands for find_places_type below
    std::vector<PlaceID> find_places_name(Name const& name);

    // Estimate of performance: worst case linear, on average somewhere between
    //      logarithmic and linear, best case constant
    // Short rationale for estimate: same reasoning as for method above
    std::vector<PlaceID> find_places_type(PlaceType type);

    // Estimate of performance: average logarithmic, linear in worst case
    // Short rationale for estimate: find-method for an unordered_map has complexity
    //      of constant on average, linear in worst case. multimaps equal_range
    //      is logarithmic, and in the worst case it will return the complete
    //      range of datastructure, which will in worst case take linear time to
    //      iterate over to find the correct place. On average the range of places
    //      is smaller, and for one of the places in the range two operations
    //      are issued: one of amortized constant complexity and one logarithmic.
    //
    //      This reasoning also holds for the change_place_coord-method below,
    //      as it is effectively similar
    bool change_place_name(PlaceID id, Name const& newname);

    // Estimate of performance: average logarithmic, linear in worst case
    // Short rationale for estimate: same reasoning as for method above

    //      Because of problems with equal_range with coordinates as key, removing
    //      from datastructure with coordinates as keys is done by looping through
    //      the structure, which makes the method linear in complexity in worst case.
    bool change_place_coord(PlaceID id, Coord newcoord);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: average amortized constant, worst case linear
    // Short rationale for estimate: insertion to unordered map has this complexity,
    //      which is the only operation done
    bool add_area(AreaID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance: average amortized constant, worst case O(n)
    // Short rationale for estimate: find-method for an unordered map has complexity
    //      of constant on average, and linear in the worst case.
    //      Operation looks up the id and if it is found, name is returned
    Name get_area_name(AreaID id);

    // Estimate of performance: average amortized constant, worst case O(n)
    // Short rationale for estimate: find-method for an unordered map has complexity
    //      of constant on average, and linear in the worst case.
    //      Operation looks up the id and if it is found, coordinates are returned
    std::vector<Coord> get_area_coords(AreaID id);

    // Estimate of performance: linear in context of number of areas
    // Short rationale for estimate: loops through areas in data structure
    //     and pushes ID of each to a vector, which has constant complexity
    std::vector<AreaID> all_areas();

    // Estimate of performance: constant on average, linear in worst case
    // Short rationale for estimate: performs two find-methods for unordered_map,
    //      one placement and one insertion to end of a vector. First two have
    //      complexity stated, and the latter two are constant operations
    bool add_subarea_to_area(AreaID id, AreaID parentid);

    // Estimate of performance: linear in worst case, on average linear in
    //      context of area hierarchy depth, best case constant
    // Short rationale for estimate: Find from unordered_map is constant on average,
    //      linear in worst case. If area is found and it has a parent area,
    //      the hierarchy is looped through and a constant operation is performed
    //      for each. Hence, performance is not worse than linear, and in best
    //      case it is constant if area is not found / has no parents. On average,
    //      it is somewhere in between.
    std::vector<AreaID> subarea_in_areas(AreaID id);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    void creation_finished();

    // Estimate of performance: linear in context of subareas in area
    // Short rationale for estimate: all subareas in area are recursively visited
    //      which is linear in their amount, and their id is pushed into a vector,
    //      which is amortized constant in complexity.
    std::vector<AreaID> all_subareas_in_area(AreaID id);

    // Estimate of performance: O(n log n)
    // Short rationale for estimate: if no type is given, all places are looped
    //      through and their distance to given coordinate is calculated, and the
    //      value is inserted in a map to have the smallest distances in the
    //      beginning. Inserting is logarithmic, which gives the given performance.
    //      After mapping distances maximum of three values are pushed into a vector,
    //      so this for-loop has no practical effect on performance.
    std::vector<PlaceID> places_closest_to(Coord xy, PlaceType type);

    // Estimate of performance: average logarithmic, linear in worst case
    // Short rationale for estimate: find-method for an unordered_map has complexity
    //      of constant on average, linear in worst case. multimaps equal_range
    //      is logarithmic, and in the worst case it will return the complete
    //      range of datastructure, which will in worst case take linear to
    //      iterate over to find the correct place. On average the range of
    //      places is smaller, and for one of the places in the range one operation
    //      is issued, which has constant complexity. The set of operations for
    //      multimap is done three times for three separate datastructures, which makes
    //      average performance 3 log n, but the multiplier is irrelevant.

    //      Because of problems with equal_range with coordinates as key, removing
    //      from datastructure with coordinates as keys is done by looping through
    //      the structure, which makes the method linear in complexity in worst case.
    bool remove_place(PlaceID id);

    // Estimate of performance: worst case linear in context of deeper area hierarchy,
    //      best case constant, average somewhere in between
    // Short rationale for estimate: calls subarea_in_areas twice, which on average
    //      is somewhere between constant and linear, empirically logarithmic in
    //      most cases. Then iterates over area hierarchies simultaneously if both
    //      areas have same ancestor highest in hierarchy, until different ancestors
    //      are found. This operation in worst case is linear in context of lower
    //      hierarchy, if one's direct parent is also an ancestor of the other area.
    AreaID common_area_of_subareas(AreaID id1, AreaID id2);

private:

    void subareas(std::shared_ptr<Area> areaNode, std::vector<AreaID> &subareas);

    std::unordered_map<PlaceID, std::shared_ptr<Place>> places_;
    std::unordered_map<AreaID, std::shared_ptr<Area>> areas_;

    std::multimap<Name, PlaceID> placesAlphabetically_;
    std::multimap<Coord, PlaceID> placesCoordOrder_;
    std::multimap<PlaceType, PlaceID> placesTypes_;

};

#endif // DATASTRUCTURES_HH
