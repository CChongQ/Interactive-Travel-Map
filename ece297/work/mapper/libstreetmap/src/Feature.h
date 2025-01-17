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
/*
 * Feature.h
 *
 *  Created on: Dec 30, 2015
 *      Author: jcassidy
 */

#ifndef FEATURE_HPP_
#define FEATURE_HPP_

#include <vector>
#include <utility>
#include <string>

#include "LatLon.h"
#include "OSMID.h"

// Feature types (string representation provided in Feature.h:asString should be kept in sync)
enum FeatureType {
    Unknown = 0,
    Park,
    Beach,
    Lake,
    River,
    Island,
    Shoreline,		// TODO: deprecated, no longer used & should be removed
    Building,
    Greenspace,
    Golfcourse,
    Stream
};

const std::string& asString(FeatureType t);


/** Defines feature, one of the principal data types in the Layer-2 database.
 * Features can be either linear, or closed polygons (polys have start and end point the same)
 */

class Feature {
public:
    Feature();
    Feature(TypedOSMID osmTID, FeatureType type, std::string&& name, std::vector<LatLon>&& pts, bool bounded = true);

    Feature(const Feature&) = default;
    Feature(Feature&&) = default;
    Feature& operator=(Feature&&) = default;
    Feature& operator=(const Feature&) = default;

    const std::string& name() const {
        return m_name;
    }

    FeatureType type() const {
        return m_type;
    }

    // true if boundary never intersects map bounds

    bool isBounded() const {
        return m_bounded;
    }

    /// Access to the boundary points defining the feature

    const std::vector<LatLon>& points() const {
        return m_points;
    }

    std::size_t pointCount() const {
        return m_points.size();
    }

    LatLon point(unsigned i) const {
        return m_points.at(i);
    }

    /// OSM type and ID for connecting to Layer 1/OSM data sources

    TypedOSMID id() const {
        return m_osmTID;
    }

private:
    TypedOSMID m_osmTID;
    std::string m_name = "";
    std::vector<LatLon> m_points;
    FeatureType m_type;

    bool m_bounded = true;

    friend class boost::serialization::access;
    template<class Archive>void serialize(Archive& ar, const unsigned)
    	{ ar & m_name & m_type & m_osmTID & m_bounded & m_points ; }
};

#endif /* FEATURE_HPP_ */