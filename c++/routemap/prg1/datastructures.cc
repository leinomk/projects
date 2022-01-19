// Datastructures.cc

#include "datastructures.hh"

#include <random>

#include <cmath>

#include <iostream>

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
