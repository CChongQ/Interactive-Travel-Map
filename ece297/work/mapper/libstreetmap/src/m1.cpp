/* 
 * Copyright 2018 University of Toronto
 *
 * Permission is hereby granted, to use this software and associated 
 * documentation files (the "Software") in course work at the University 
 * of Toronto, or for personal use. Other uses are prohibited, in 
 * particular the distribution of the Software either publicly or to third 
 * parties.
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "m1.h"
#include "newmap.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"


#include <map>
#include <tuple>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <unordered_map>
#include <vector>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <limits>
namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

std::unordered_map<std::string, std::vector<unsigned>> closest_intersection_vectorsUOM;

// since the unit of LatLon is degree, use spherical coordinate system 
//in the following code 
typedef bg::model::point<double, 2, bg::cs::spherical_equatorial<bg::degree>> apoint;
//make position and ids (of intersections or point of interests) a pair
typedef std::pair<apoint, unsigned> apair;

newmap *mymap = NULL;

bool load_map(std::string map_name) {

    //We are going to use unordered_map structure here
    if (!loadStreetsDatabaseBIN(map_name)) {
        return false;
    }
    std::string replacedPath = "streets.bin";
    std::string osmPath = map_name.replace(map_name.find(replacedPath), replacedPath.length(), "osm.bin");

    if (!loadOSMDatabaseBIN(osmPath)) {
        return false;
    }

    mymap = new newmap;

    for (unsigned interestID = 0; interestID < getNumberOfPointsOfInterest(); interestID++) {
        std::string interest_name1 = getPointOfInterestName(interestID);
        LatLon interest_position = getPointOfInterestPosition(interestID);
        unsigned closest_intersection_of_interest = find_closest_intersection(interest_position);
        closest_intersection_vectorsUOM[interest_name1].push_back(closest_intersection_of_interest);

    }

    
    ////////////
        //get max and min lat
    double max_lat = getIntersectionPosition(0).lat();
    double min_lat = max_lat;

    //get max and min lon
    double max_lon = getIntersectionPosition(0).lon();
    double min_lon = max_lon;
        for (unsigned id = 0; id < getNumberOfIntersections(); id++) {
        // intersections[id].position = getIntersectionPosition(id);
        //  intersections[id].name = getIntersectionName(id);

        max_lat = std::max(max_lat, (mymap->intersectionUOM[id]).intersectionPosition1.lat()); // compares two num and finds max between the 2 and then put it into max_lat
        min_lat = std::min(min_lat, (mymap->intersectionUOM[id]).intersectionPosition1.lat());

        max_lon = std::max(max_lon, (mymap->intersectionUOM[id]).intersectionPosition1.lon());
        min_lon = std::min(min_lon, (mymap->intersectionUOM[id]).intersectionPosition1.lon());
    }
    double averagelat = 0.5 * (max_lat + min_lat);
    ////////////////
    unsigned num_Relation = getNumberOfRelations();


    for (unsigned i = 0; i < num_Relation; i++) {

        for (unsigned j = 0; j < getTagCount(getRelationByIndex(i)); j++) {



            if ((getTagPair(getRelationByIndex(i), j)).second == "subway") {

                unsigned subway_line = mymap->subway_Coordinate.size();

                mymap->subway_Coordinate.resize(subway_line + 1);

                std::vector<OSMRelation::Member> subway_member = getRelationByIndex(i)->members();

                unsigned member_size = subway_member.size();

                for (unsigned k = 0; k < member_size; k++) {
                    if (subway_member[k].tid.type() == 2) {

                        unsigned way_index = mymap->subway_Coordinate[subway_line].size();

                        mymap->subway_Coordinate[subway_line].resize(way_index + 1);

                        unsigned wayidx = mymap->OSM_Way[subway_member[k].tid];

                        //Generate the coordinates in the wayOSMDID

                        const std::vector<OSMID> node_coord = getWayByIndex(wayidx)-> ndrefs();

                                //continue Loop through the nodes

                        for (unsigned p = 0; p < node_coord.size(); p++) {

                            unsigned nodeix = mymap->OSM_Node[node_coord[p]];

                                    double latAvg = averagelat;

                                    LatLon Node_Coordinate = getNodeByIndex(nodeix)-> coords();

                                    t_point coordinate_cartesian;


                                    coordinate_cartesian.x = Node_Coordinate.lon() * cos(latAvg * DEG_TO_RAD) * DEG_TO_RAD;



                                    coordinate_cartesian.y = Node_Coordinate.lat() * DEG_TO_RAD;
                                    mymap->subway_Coordinate[subway_line][way_index].push_back(coordinate_cartesian);
                        }
                    }
                }
            }
        }
    }




    /////////////////////




    if (mymap == NULL) {

        return false;
    }
    return true;
}

void close_map() {

    //Clean-up your map related data structures here

    delete mymap;

            //call the function to free the memory
            closeStreetDatabase();
            closeOSMDatabase();

}

//Returns street id(s) for the given street name
//If no street with this name exists, returns a 0-length vector.

std::vector<unsigned> find_street_ids_from_name(std::string street_name) {

    return mymap->streetUOM[street_name]; //using unordered map
}

//Returns the street segments for the given intersection

std::vector<unsigned> find_intersection_street_segments(unsigned intersection_id) {

    return mymap->getssIndexfromintersectionIndex[intersection_id];
}

//Returns the street names at the given intersection (includes duplicate street 
//names in returned vector)

std::vector<std::string> find_intersection_street_names(unsigned intersection_id) {

    std::vector<std::string> intstreetVector;
            StreetSegmentInfo a;


    for (unsigned int i = 0; i < mymap->countstreetsegment[intersection_id]; i++) {

        unsigned int num = (mymap->getssIndexfromintersectionIndex[intersection_id])[i];
                a = mymap->streetSegmentUOM[num];
                intstreetVector.push_back(getStreetName(a.streetID));
    }
    return intstreetVector;
}

//Returns true if you can get from intersection1 to intersection2 using a single 
//street segment (hint: check for 1-way streets too)
//corner case: an intersection is considered to be connected to itself

bool are_directly_connected(unsigned intersection_id1, unsigned intersection_id2) {

    unsigned int streetSegmentSize;
            unsigned int streetSegmentIndex;
            streetSegmentSize = find_intersection_street_segments(intersection_id1).size();

    for (unsigned int i = 0; i < streetSegmentSize; i++) {

        streetSegmentIndex = find_intersection_street_segments(intersection_id1)[i];
                StreetSegmentInfo ssinfo = mymap->streetSegmentUOM[streetSegmentIndex];

                // if the condition is satisfied
        if ((ssinfo.from == intersection_id1
                && ssinfo.to == intersection_id2)
                || (ssinfo.from == intersection_id2
                && !ssinfo.oneWay
                && ssinfo.to == intersection_id1)) {
            return true;
        }

    }
    //corner case: an intersection is considered to be connected to itself
    if (intersection_id1 == intersection_id2) {

        return true;
    }
    //for other situations they are not directly connected
    return false;

}

//extern newmap *mymap;

//Returns all intersections reachable by traveling down one street segment 
//from given intersection (hint: you can't travel the wrong way on a 1-way street)

std::vector<unsigned> find_adjacent_intersections(unsigned intersection_id) {

    std::vector<unsigned> adjacentIntersectionIDs;

            //how many segments does this intersection connect to
            unsigned streetSegmentCount = mymap->countstreetsegment[intersection_id];

            StreetSegmentInfo a;

    for (unsigned int i = 0; i < streetSegmentCount; i++) {

        //boolean to check if the id is already existed
        bool shouldadd = true;

                unsigned num = (mymap->getssIndexfromintersectionIndex[intersection_id])[i];

                a = mymap->streetSegmentUOM[num];


                //if the intersection connect to itself
        if (a.from == a.to) {
            //do nothing
        }            //if it is one-way and order is correct
        else if (a.oneWay == true && a.from == intersection_id) {



            //the for loop is used for repeat index checking
            for (unsigned int j = 0; j < adjacentIntersectionIDs.size(); j++) {



                if (adjacentIntersectionIDs[j] == a.to) {
                    shouldadd = false;
                }
            }
            if (shouldadd == true) {
                adjacentIntersectionIDs.push_back(a.to);
            }
        }            //if it's not one-way order does not matter(so is the next else if)
        else if (!a.oneWay == true && a.from == intersection_id) {
            for (unsigned int j = 0; j < adjacentIntersectionIDs.size(); j++) {
                if (adjacentIntersectionIDs[j] == a.to) {
                    shouldadd = false;
                }
            }
            if (shouldadd == true) {
                adjacentIntersectionIDs.push_back(a.to);
            }
        } else if (!a.oneWay == true && a.to == intersection_id) {
            for (unsigned int j = 0; j < adjacentIntersectionIDs.size(); j++) {
                if (adjacentIntersectionIDs[j] == a.from) {
                    shouldadd = false;
                }
            }
            if (shouldadd == true) {

                adjacentIntersectionIDs.push_back(a.from);
            }
        }
        //for other situation, pushback nothing
    }
    return adjacentIntersectionIDs;
}

//Returns all street segments for the given street

std::vector<unsigned> find_street_street_segments(unsigned street_id) {

    return mymap->streetToSegID[street_id];

}
//Returns all intersections along the a given street

std::vector<unsigned> find_all_street_intersections(unsigned street_id) {

    std::vector<unsigned> allStInter;

            //holds st seg IDs associated with this street ID
            std::vector<unsigned> stSegs = find_street_street_segments(street_id);



    for (unsigned i = 0; i < stSegs.size(); i++) {

        StreetSegmentInfo a = mymap->streetSegmentUOM[stSegs[i]];
                IntersectionIndex fromID = a.from;
                IntersectionIndex toID = a.to;
                bool existTo = false;
                bool existFrom = false;



                //check if fromID, toID were previously inserted
        if (std::find(allStInter.begin(), allStInter.end(), fromID)
                != allStInter.end()) {

            existFrom = true;

        } else if (std::find(allStInter.begin(), allStInter.end(), toID)
                != allStInter.end()) {

            existTo = true;

        }


        // if the to or from index didn't exist in the list, add it to the 
        //allStInter vector
        if (!existFrom) {
            allStInter.push_back(fromID);
        }
        if (!existTo) {
            allStInter.push_back(toID);
        }
    }

    //erase duplicates
    std::sort(allStInter.begin(), allStInter.end());
            allStInter.erase(std::unique(allStInter.begin(), allStInter.end()),
            allStInter.end());

    return allStInter;


}


//Return all intersection ids for two intersecting streets
//This function will typically return one intersection id.
//However street names are not guarenteed to be unique, so more than 1 intersection id
//may exist

std::vector<unsigned> find_intersection_ids_from_street_names(
        std::string street_name1, std::string street_name2) {

    //basic idea: firstly, get intersectionIDs  the two given street have 
    //respectively.
    //secondly, compare these intersectionIds to see if there exists two street 
    //have same intersectionId
    std::vector<IntersectionIndex> interIds;

            //all intersections all along street_name1
            std::vector<StreetIndex> streetOne = find_street_ids_from_name(street_name1);

            // all intersection all along street_name2    
            std::vector<StreetIndex> streetTwo = find_street_ids_from_name(street_name2);


    if (streetOne.size() == 0 || streetTwo.size() == 0) {
        return interIds;
    } else {
        for (unsigned i = 0; i < streetOne.size(); i++) {
            for (unsigned j = 0; j < streetTwo.size(); j++) {

                std::vector<IntersectionIndex> street_interx_one =
                        find_all_street_intersections(streetOne[i]);
                        std::vector<IntersectionIndex> street_interx_two =
                        find_all_street_intersections(streetTwo[j]);

                        std::set_intersection(street_interx_one.begin(),
                        street_interx_one.end(),
                        street_interx_two.begin(),
                        street_interx_two.end(),
                        std::back_inserter(interIds));
            }

        }
        return interIds;
    }

}



//Returns the distance between two coordinates in meters

double find_distance_between_two_points(LatLon point1, LatLon point2) {

    //lon and lat in LatLon are in degree, need to be convert to radius to 
    //compute distance  
    //rad = degree * DEG_TO_RAD 

    //lat_avg = (lat1+lat2)*DEG_TO_RAD /2) in rad
    double lat_avg = (point1.lat() + point2.lat()) * DEG_TO_RAD / 2;

            //x = lon * cos(lat_avg)
            double x1 = (point1.lon() * DEG_TO_RAD) * cos(lat_avg);

            // y = lat
            double y1 = point1.lat() * DEG_TO_RAD;
            double x2 = (point2.lon() * DEG_TO_RAD) * cos(lat_avg);
            double y2 = point2.lat() * DEG_TO_RAD;
            double distance = 0;

            //d = R *sqrt((y2-y1)^2 + (x2-x1)^2 ))
            double temp = pow((y2 - y1), 2) + pow((x2 - x1), 2);
            distance = EARTH_RADIUS_IN_METERS * sqrt(temp);

    return distance;

}



//Returns the length of the given street segment in meters

double find_street_segment_length(unsigned street_segment_id) {

    double value = mymap->segmentLength[street_segment_id];

    return value;

}



//Returns the length of the specified street in meters

double find_street_length(unsigned street_id) {

    // a vector contains all street segments of the given street
    std::vector<unsigned> allSeg = find_street_street_segments(street_id);
            double length = 0;
            std::vector<unsigned>::iterator it;


    for (it = allSeg.begin(); it != allSeg.end(); ++it) {
        //iterator through the whole list , 
        //get length of different segments, then add them up

        length = length + find_street_segment_length(*it);
    }


    return length;

}



//Returns the travel time to drive a street segment in seconds 
//(time = distance/speed_limit)

double find_street_segment_travel_time(unsigned street_segment_id) {

    double time = mymap->travel_time[street_segment_id];

    return time;

}



//Returns the nearest point of interest to the given position

unsigned find_closest_point_of_interest(LatLon my_position) {

    double x = my_position.lat();
            double y = my_position.lon();
            std::vector<apair> result1;
            mymap->pointofInterestRTree.query(bgi::nearest(apoint(y, x), 1),
            std::back_inserter(result1));

    return result1[0].second;

}



//Returns the the nearest intersection to the given position

unsigned find_closest_intersection(LatLon my_position) {

    double a = my_position.lat();
            double b = my_position.lon();
            std::vector<apair> result2;
            mymap->intersectionRTree.query(bgi::nearest(apoint(b, a), 1),
            std::back_inserter(result2));


    return result2[0].second;

}
