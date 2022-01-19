// Datastructures.cc

#include "datastructures.hh"

#include <random>

#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    // Replace this comment with your implementation
}

Datastructures::~Datastructures()
{
    // Replace this comment with your implementation
}

int Datastructures::place_count()
{
    return places_.size();
}

void Datastructures::clear_all()
{
    placesAlphabetically_.clear();
    placesCoordOrder_.clear();
    placesTypes_.clear();
    places_.clear();
    areas_.clear();
}

std::vector<PlaceID> Datastructures::all_places()
{
    std::vector<PlaceID> allPlaces = {};
    for( const auto&[ placeID, place ] : places_ ){
        allPlaces.push_back(placeID);
    }
    return allPlaces;
}

bool Datastructures::add_place(PlaceID id, const Name& name, PlaceType type, Coord xy)
{
    std::shared_ptr<Place> newPlace( new Place{type, name, xy});
    bool inserted = places_.insert({id, newPlace}).second;
    if( inserted ){
        placesAlphabetically_.insert({name, id});
        placesCoordOrder_.insert({xy, id});
        placesTypes_.insert({type, id});
    }
    return inserted;
}

std::pair<Name, PlaceType> Datastructures::get_place_name_type(PlaceID id)
{
    auto search = places_.find(id);
    if( search != places_.end() ){
        return std::make_pair(search->second->name, search->second->type);
    }
    return {NO_NAME, PlaceType::NO_TYPE};
}

Coord Datastructures::get_place_coord(PlaceID id)
{
    auto search = places_.find(id);
    if( search != places_.end() ){
        return search->second->coord;
    }
    return NO_COORD;
}

bool Datastructures::add_area(AreaID id, const Name &name, std::vector<Coord> coords)
{
    std::shared_ptr<Area> newArea( new Area{id, name, coords});
    bool inserted = areas_.insert({id, newArea}).second;
    return inserted;
}

Name Datastructures::get_area_name(AreaID id)
{
    auto search = areas_.find(id);
    if( search != areas_.end() ){
        return search->second->name;
    }
    return NO_NAME;
}

std::vector<Coord> Datastructures::get_area_coords(AreaID id)
{
    auto search = areas_.find(id);
    if( search != areas_.end() ){
        return search->second->coords;
    }
    return {NO_COORD};
}

void Datastructures::creation_finished()
{
    // Replace this comment with your implementation
    // NOTE!! It's quite ok to leave this empty, if you don't need operations
    // that are performed after all additions have been done.
}


std::vector<PlaceID> Datastructures::places_alphabetically()
{
    if( placesAlphabetically_.size() > 0 ){
        std::vector<PlaceID> placesAlphabetical = {};
        for( const auto &[ placeName, placeID ] : placesAlphabetically_ ){
            placesAlphabetical.push_back(placeID);
        }
        return placesAlphabetical;
    };
    return {};
}

std::vector<PlaceID> Datastructures::places_coord_order()
{
    if( placesCoordOrder_.size() > 0 ){
        std::vector<PlaceID> placesCoord = {};
        for( const auto &[ placeCoord, placeID ] : placesCoordOrder_ ){
            placesCoord.push_back(placeID);
        }
        return placesCoord;
    };
    return {};
}

std::vector<PlaceID> Datastructures::find_places_name(Name const& name)
{
    if( places_.size() > 0 ){
        std::vector<PlaceID> placesWithName;
        auto range = placesAlphabetically_.equal_range(name); // logarithmic
        for( auto place = range.first; place != range.second; ++place ){ //linear in number of places with type
            placesWithName.push_back(place->second);
        }
        return placesWithName;
    }
    return {};
}

std::vector<PlaceID> Datastructures::find_places_type(PlaceType type)
{
    if( places_.size() > 0 ){
        std::vector<PlaceID> placesWithType;
        auto range = placesTypes_.equal_range(type);
        for( auto place = range.first; place != range.second; ++place ){
            placesWithType.push_back(place->second);
        }
        return placesWithType;
    }
    return {};
}

bool Datastructures::change_place_name(PlaceID id, const Name& newname)
{
    auto search = places_.find(id); // constant on average, worst case linear
    if( search != places_.end() ){
        Name nameToChange = search->second->name;
        search->second->name = newname;
        auto range = placesAlphabetically_.equal_range(nameToChange); // logarithmic
        for( auto i = range.first; i != range.second; ++i){ // linear in the number of places with name
            if( i->second == id){
                placesAlphabetically_.erase(i); //amortized constant
                placesAlphabetically_.insert({newname, id}); //logarithmic
                break;
            }
        }
        return true;
    }
    return false;
}

bool Datastructures::change_place_coord(PlaceID id, Coord newcoord)
{
    auto search = places_.find(id);
    if( search != places_.end() ){
        search->second->coord = newcoord;
        /*
        auto range = placesCoordOrder_.equal_range(coordToChange);
        for( auto i = range.first; i != range.second; ++i){
            if( i->second == id){
                placesCoordOrder_.erase(i);
                placesCoordOrder_.insert({newcoord, id});
                break;
            }
        }
        */
        for( std::multimap<Coord, PlaceID>::iterator place = placesCoordOrder_.begin();
             place != placesCoordOrder_.end(); ++place ){
            if( place->second == id ){
                placesCoordOrder_.erase(place);
                placesCoordOrder_.insert({newcoord, id});
                break;
            }
        }
        return true;
    }
    return false;
}

std::vector<AreaID> Datastructures::all_areas()
{
    if( areas_.size() > 0 ){
        std::vector<AreaID> areas;
        for( const auto &[ areaID, area ] : areas_ ){
            areas.push_back(areaID);
        }
        return areas;
    }
    return {};
}

bool Datastructures::add_subarea_to_area(AreaID id, AreaID parentid)
{
    auto subareaIt = areas_.find(id);
    if ( subareaIt != areas_.end() && subareaIt->second->parentArea == nullptr ){
        auto parentIt = areas_.find(parentid);
        if ( parentIt != areas_.end()){
            subareaIt->second->parentArea = parentIt->second;
            parentIt->second->subAreas.push_back(subareaIt->second);
            return true;
        }
    }
    return false;
}

std::vector<AreaID> Datastructures::subarea_in_areas(AreaID id)
{
    auto areaIt = areas_.find(id); // constant on average, linear worst case
    if ( areaIt != areas_.end() ){
        auto area = areaIt->second;
        if ( area->parentArea == nullptr ){
            return {};
        }
        std::vector<AreaID> subareaInAreas;
        while ( area->parentArea != nullptr ){ // linear in context of area-hierarchy depth
            subareaInAreas.push_back(area->parentArea->id);
            area = area->parentArea;
        }
        return subareaInAreas;
    }

    return {NO_AREA};
}

std::vector<PlaceID> Datastructures::places_closest_to(Coord xy, PlaceType type)
{
    if( places_.size() > 0 ){
        std::map<DistanceData, PlaceID> distances;
        std::vector<PlaceID> places;

        bool searchForType = true;
        if( type == PlaceType::NO_TYPE ){
            searchForType = false;
        }

        for( const auto &[ placeID, place ] : places_ ){
            if( !searchForType || place->type == type ){
                Coord coords = place->coord;
                int distance = sqrt(pow(coords.x - xy.x, 2) + pow(coords.y - xy.y, 2));
                DistanceData distData = {distance, coords.y};

                distances.insert({ distData, placeID });
            }
        }

        for( const auto &[distance, placeID ] : distances ){
            if( places.size() < 3 ){
                places.push_back(placeID);
            } else { break; }
        }

        return places;
    }
    return {};
}

bool Datastructures::remove_place(PlaceID id)
{
    auto search = places_.find(id); // constant on average, linear worst case
    if( search != places_.end() ){
        Name nameToRm = search->second->name;
        PlaceType typeToRm = search->second->type;

        /*
        auto coordRange = placesCoordOrder_.equal_range(coordToRm); // logarithmic
        for( auto i = coordRange.first; i != coordRange.second; ++i){ // linear in number of places with same coord
            if( i->second == id){
                placesCoordOrder_.erase(i); // amortized constant
                break;
            }
        }
        */

        for( std::multimap<Coord, PlaceID>::iterator place = placesCoordOrder_.begin();
             place != placesCoordOrder_.end(); ++place ){
            if( place->second == id ){
                placesCoordOrder_.erase(place);
                break;
            }
        }

        auto nameRange = placesAlphabetically_.equal_range(nameToRm); //logarithmic
        for( auto i = nameRange.first; i != nameRange.second; ++i){ // linear in number of places with same name
            if( i->second == id){
                placesAlphabetically_.erase(i); // amortized constant
                break;
            }
        }

        auto typeRange = placesTypes_.equal_range(typeToRm); //logarithmic
        for( auto i = typeRange.first; i != typeRange.second; ++i){ // linear in number of places with same name
            if( i->second == id){
                placesTypes_.erase(i); // amortized constant
                break;
            }
        }

        places_.erase(search); // amortized constant
        return true;
    }
    return false;
}

std::vector<AreaID> Datastructures::all_subareas_in_area(AreaID id)
{
    auto areaIt = areas_.find(id);
    if( areaIt != areas_.end() ){
        auto area = areaIt->second;
        if( !(area->subAreas.empty()) ){
            std::vector<AreaID> subAreas;
            for( const auto &subArea : area->subAreas ){
                subareas(subArea, subAreas);
            }
            return subAreas;
        }
        return {};
    }
    return {NO_AREA};
}

AreaID Datastructures::common_area_of_subareas(AreaID id1, AreaID id2)
{
    std::vector<AreaID> area1Parents = subarea_in_areas(id1); //linear in area hierarchy depth
    std::vector<AreaID> area2Parents = subarea_in_areas(id2); //linear in area hierarchy depth
    // check that both areas exist and each is a subarea of any areas
    if( !( area1Parents.empty() || area2Parents.empty() ||
           area1Parents.front() == NO_AREA || area2Parents.front() == NO_AREA) ){
        std::vector<AreaID>::iterator area1ParentsIt = area1Parents.end() - 1;
        std::vector<AreaID>::iterator area2ParentsIt = area2Parents.end() - 1;
        // areas that have common parent do also have the same area on
        // top of the hierarchy, i.e. last element of each vector
        if( *area1ParentsIt != *area2ParentsIt ){
            return NO_AREA;
        }
        // iterate vectors from the end towards beginning, on each round check
        // if values one level 'deeper' differ, in which case return one of current values
        while( area1ParentsIt > area1Parents.begin() && area2ParentsIt > area2Parents.begin() ){
            if( *(area1ParentsIt - 1) != *(area2ParentsIt - 1) ){
                return *area1ParentsIt;
            }
            area1ParentsIt--;
            area2ParentsIt--;
        }
        // at least one iterator points to beginning of vector and values on
        // that level in hierarchy are same
        return *area1ParentsIt;
    }
    return NO_AREA;
}

void Datastructures::subareas(std::shared_ptr<Area> area, std::vector<AreaID> &subAreas)
{
    subAreas.push_back(area->id);
    for( const auto &subArea : area->subAreas ){
        subareas(subArea, subAreas);
    }
}


/*
 *
 *
 *  PHASE 2 OPERATIONS
 *
 *
 */

std::vector<WayID> Datastructures::all_ways()
{
    std::vector<WayID> allWays = {};
    for( const auto&[ wayID, way ] : ways_ ){
        allWays.push_back(wayID);
    }
    return allWays;
}

bool Datastructures::add_way(WayID id, std::vector<Coord> coords)
{
    std::shared_ptr<Way> newWay( new Way{id, coords});
    auto insert = ways_.insert({id, newWay}); // average constant, linear worst case
    if( insert.second ){
        Distance length = wayLength(coords); // linear in number of coords
        insert.first->second->length = length;
        Coord startCoord = coords.front();
        Coord endCoord = coords.back();
        auto beginCrossroadIter = crossroads_.find(startCoord); // average constant, worst case linear
        auto endCrossroadIter = crossroads_.find(endCoord); // average constant, worst case linear

        std::shared_ptr<Crossroad> startCrossroad;
        std::shared_ptr<Crossroad> endCrossroad;

        if (beginCrossroadIter == crossroads_.end()){
            std::shared_ptr<Crossroad> newCrossroad ( new  Crossroad{startCoord});
            crossroads_.insert({startCoord, newCrossroad}); // average constant, linear worst case
            startCrossroad = newCrossroad;
        } else { startCrossroad = beginCrossroadIter->second; }

        if (endCrossroadIter == crossroads_.end()){
            std::shared_ptr<Crossroad> newCrossroad ( new  Crossroad{endCoord});
            crossroads_.insert({endCoord, newCrossroad}); // average constant, linear worst case
            endCrossroad = newCrossroad;
        } else { endCrossroad = endCrossroadIter->second; }

        startCrossroad->pathsFrom.push_back({id, endCrossroad}); // constant
        endCrossroad->pathsFrom.push_back({id, startCrossroad}); // constant

    }
    return insert.second;
}

std::vector<std::pair<WayID, Coord>> Datastructures::ways_from(Coord xy)
{
    auto search = crossroads_.find(xy); // average constant, linear worst case
    std::vector<std::pair<WayID, Coord>> waysFrom;
    if( search != crossroads_.end() ){
        std::pair<WayID, Coord> newWay;
        for (const auto& [wayID, target] : search->second->pathsFrom ){ // linear
            newWay = {wayID, target->location};
            waysFrom.push_back(newWay); // amortized constant
        }
    }
    return waysFrom;
}

std::vector<Coord> Datastructures::get_way_coords(WayID id)
{
    auto search = ways_.find(id); // average constant, linear worst case
    if( search != ways_.end() ){
        return search->second->coords;
    }
    return {NO_COORD};
}

void Datastructures::clear_ways()
{
    ways_.clear();
    crossroads_.clear();
}

std::vector<std::tuple<Coord, WayID, Distance> > Datastructures::route_any(Coord fromxy, Coord toxy)
{
    auto startpointSearch = crossroads_.find(fromxy);
    auto endpointSearch = crossroads_.find(toxy);
    if( startpointSearch != crossroads_.end() && endpointSearch != crossroads_.end() ){
        std::vector<std::tuple<Coord, WayID, Distance>> route = {};
        initCrossroads();
        DFS(startpointSearch->second, endpointSearch->second); // linear in V + E
        parseRoute(startpointSearch->second, endpointSearch->second, route); //linear in number of crossroads on route
        return route;
    }
    return {{NO_COORD, NO_WAY, NO_DISTANCE}};
}

bool Datastructures::remove_way(WayID id)
{
    auto waySearch = ways_.find(id); // constant on average, worst case linear
    if( waySearch != ways_.end() ){
        Coord beginCoord = waySearch->second->coords.front();
        Coord endCoord = waySearch->second->coords.back();

        // both constant on average, linear in worst case
        auto beginCrossroad = crossroads_.find(beginCoord);
        auto endCrossroad = crossroads_.find(endCoord);

        // if crossroad has only one way leaving from it, it can be erased completely
        if( beginCrossroad->second->pathsFrom.size() == 1 ){
            crossroads_.erase(beginCrossroad);
        } else {
            // linear iteration over paths from crossroad to remove desired way
            std::list<std::pair<WayID, std::shared_ptr<Crossroad>>>::iterator pathIt;
            for( pathIt = beginCrossroad->second->pathsFrom.begin();
                 pathIt != beginCrossroad->second->pathsFrom.end();
                 ++pathIt){
                if( pathIt->first == id ){
                    // constant complexity removal from list
                    beginCrossroad->second->pathsFrom.erase(pathIt);
                    break;
                }
            }
        }

        if( endCrossroad->second->pathsFrom.size() == 1 ){
            crossroads_.erase(endCrossroad);
        } else {
            std::list<std::pair<WayID, std::shared_ptr<Crossroad>>>::iterator pathIt;
            for( pathIt = endCrossroad->second->pathsFrom.begin();
                 pathIt != endCrossroad->second->pathsFrom.end();
                 ++pathIt ){
                if( pathIt->first == id ){
                    endCrossroad->second->pathsFrom.erase(pathIt);
                    break;
                }
            }
        }

        // constant on average, worst case linear
        ways_.erase(waySearch);

        return true;

    }
    return false;
}

std::vector<std::tuple<Coord, WayID, Distance> > Datastructures::route_least_crossroads(Coord fromxy, Coord toxy)
{
    return shortestRoute(fromxy, toxy, true);
}

std::vector<std::tuple<Coord, WayID> > Datastructures::route_with_cycle(Coord fromxy)
{
    // Replace this comment with your implementation
    return {{NO_COORD, NO_WAY}};
}

std::vector<std::tuple<Coord, WayID, Distance> > Datastructures::route_shortest_distance(Coord fromxy, Coord toxy)
{
    return shortestRoute(fromxy, toxy, false);
}

Distance Datastructures::trim_ways()
{
    // Replace this comment with your implementation
    return NO_DISTANCE;
}

Distance Datastructures::wayLength(std::vector<Coord> coords)
{
    Distance totalDistance = 0;
    std::vector<Coord>::size_type firstInd;
    Coord first, second;
    for( firstInd = 0; firstInd < coords.size() - 1; ++firstInd ){ // linear in number of coords
        first = coords[firstInd];
        second = coords[firstInd + 1];
        Distance legDistance = floor(sqrt(pow(first.x - second.x, 2) +
                                          pow(first.y - second.y, 2)));
        totalDistance += legDistance;
    }
    return totalDistance;
}

void Datastructures::initCrossroads()
{
    for( auto& [coord, crossroad] : crossroads_){ // linear in size of crossroads
        crossroad->visited = WHITE;
        crossroad->fromStart = std::numeric_limits<int>::max();
        crossroad->arriveVia = NO_WAY;
        crossroad->lastArrive = nullptr;
    }
}

std::vector<std::tuple<Coord, WayID, Distance>> Datastructures::shortestRoute(
        Coord fromxy, Coord toxy, bool countCrossroads)
{
    auto startpointSearch = crossroads_.find(fromxy);
    auto endpointSearch = crossroads_.find(toxy);
    if( startpointSearch != crossroads_.end() && endpointSearch != crossroads_.end() ){
        std::vector<std::tuple<Coord, WayID, Distance>> route = {};
        initCrossroads(); // linear
        Dijkstra(startpointSearch->second, endpointSearch->second, countCrossroads); // n*log(n)
        parseRoute(startpointSearch->second, endpointSearch->second, route); // linear
        return route;
    }
    return {{NO_COORD, NO_WAY, NO_DISTANCE}};
}

void Datastructures::DFS(std::shared_ptr<Crossroad> start, std::shared_ptr<Crossroad> end)
{
    std::stack<std::shared_ptr<Crossroad>> S;
    S.push(start);
    start->fromStart = 0;
    while( !S.empty() ){ // loop is run at most once per crossroad -> O( V )
        std::shared_ptr<Crossroad> current = S.top();
        S.pop();
        if( current->visited == WHITE ){
            if( current == end ){
                return;
            }
            current->visited = GRAY;
            for( std::pair<WayID, std::shared_ptr<Crossroad>>& way : current->pathsFrom ){ // executed at most twice per edge (way)
                std::shared_ptr<Crossroad> adjacent = way.second;
                if( adjacent->visited == WHITE ){
                    adjacent->arriveVia = way.first;
                    adjacent->lastArrive = current;
                    adjacent->fromStart = current->fromStart +
                            ways_.find(way.first)->second->length;
                    S.push(adjacent);
                }
            }
        } else {
            current->visited = BLACK;
        }
    }
}

void Datastructures::Dijkstra(std::shared_ptr<Crossroad> start, std::shared_ptr<Crossroad> end, bool countCrossroads)
{
    auto cmp = [](std::shared_ptr<Crossroad> left, std::shared_ptr<Crossroad> right){
        return left->fromStart >= right->fromStart;
    };
    std::priority_queue<std::shared_ptr<Crossroad>, std::vector<std::shared_ptr<Crossroad>>, decltype(cmp)> Q(cmp);
    Q.push(start);
    start->fromStart = 0;
    while( !Q.empty() ){
        std::shared_ptr<Crossroad> current = Q.top();
        Q.pop();
        if( current == end ){
            return;
        }
        for( std::pair<WayID, std::shared_ptr<Crossroad>>& way : current->pathsFrom ){
            std::shared_ptr<Crossroad> adjacent = way.second;
            if( adjacent->visited == WHITE ){
                adjacent->visited = GRAY;
                Q.push(adjacent);
            }
            Distance dist;
            if( countCrossroads ){
                dist = current->fromStart + 1;
            } else {
                dist = current->fromStart + ways_.find(way.first)->second->length;
            }
            if( adjacent->fromStart > dist ){
                adjacent->fromStart = dist;
                adjacent->arriveVia = way.first;
                adjacent->lastArrive = current;
                Q.push(adjacent);
            }
        }
        current->visited = BLACK;
    }
}

Distance Datastructures::parseRoute(std::shared_ptr<Crossroad> start,
                                    std::shared_ptr<Crossroad> current,
                                    std::vector<std::tuple<Coord, WayID, Distance>> &route,
                                    WayID continueVia)
{
    if( current == start ){
        std::tuple<Coord, WayID, Distance> nodeData =
            {current->location, continueVia, current->fromStart};
        route.push_back(nodeData);
        return ways_.find(continueVia)->second->length;
    } else if ( current->lastArrive == nullptr ){
        return NO_DISTANCE;
    } else {
        Distance fromStart = parseRoute(start, current->lastArrive, route, current->arriveVia);
        std::tuple<Coord, WayID, Distance> nodeData =
            {current->location, continueVia, fromStart};
        route.push_back(nodeData);
        if( continueVia != NO_WAY ){
            return fromStart + ways_.find(continueVia)->second->length;
        } else { return 0; }
    }
}

