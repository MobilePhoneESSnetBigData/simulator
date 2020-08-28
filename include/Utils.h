/*
 * Copyright (C) 2019  Bogdan Oancea

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version and under the EUPL free software license version 1.0 or later.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/> and
 * <https://ec.europa.eu/info/european-union-public-licence_en>
 *
 * A data simulator for mobile phone network events
 *
 * utils.h
 *
 *  Created on: Apr 11, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef UTILS_H
#define UTILS_H

#include <agent/AgentsCollection.h>
#include <agent/MobilePhone.h>
#include <AntennaInfo.h>
#include <geos/geom/Point.h>
#include <PriorType.h>
#include <cmath>
#include <utility>
#include <vector>


namespace tinyxml2 {
class XMLElement;
class XMLNode;
} /* namespace tinyxml2 */

using namespace geos;
using namespace geos::geom;
using namespace std;
using namespace tinyxml2;

/**
 * This namespace contains utility functions that don't belong to any class.
 */
namespace utils {

	/**
	 * Generates n random points on a map.
	 * @param m a pointer to a Map object where the points have to be located.
	 * @param n the number of points to generate.
	 * @param seed the seed of the random number generator used to generate the points.
	 * @param percentHome a percent of the total number of points considered to be "home locations", i.e. each time they
	 * have the same values. The rest of the points differ from a simulation to another.
	 * @return a vector of pointers to Point objects.
	 */
	vector<Point*> generatePoints(const Map* m, unsigned long n, double percentHome, unsigned seed);

	/**
	 * Generates n random points on a map. The points have the same locations in all simulations.
	 * @param m a pointer to a Map object where the points have to be located.
	 * @param n the number of points to generate.
	 * @param seed the seed of the random number generator used to generate the points.
	 * @return a vector of pointers to Point objects.
	 */
	vector<Point*> generateFixedPoints(const Map* m, unsigned long n, unsigned seed);

	/**
	 * Number PI.
	 */
	const double PI = std::atan(1.0) * 4;

	/**
	 * Transforms a number from radians to degrees.
	 * @param x the angle to be transformed from radians to degrees.
	 * @return the value of x in degrees.
	 */
	inline double r2d(double x) {
		return x * (180.0 / PI);
	}

	/**
	 * Transforms a number from degrees to radians.
	 * @param x the angle to be transformed from degrees to radians.
	 * @return the value of x in radians.
	 */
	inline double d2r(double x){
		return (PI / 180.0) * x;
	}


	/**
	 * Returns a pointer to an XMLNode with a specific name that belongs to an XMLElement. This function is used
	 * to parse the content of the configuration files.
	 * @param el a pointer to the the XMLElement where to search the XMLNode.
	 * @param name the name of the node.
	 * @return a pointer to an XMLNode with a specific name that belongs to an XMLElement.
	 */
	XMLNode* getNode(XMLElement* el, const char* name);

	/**
	 * Returns a pointer to an XMLElement which is the first child of another XMLElement. This function is used
	 * to parse the content of the configuration files.
	 * @param el a pointer to the parent XMLElement.
	 * @param name the name of the child XMLElement.
	 * @return a pointer to an XMLElement which is the first child of another XMLElement.
	 */
	XMLElement* getFirstChildElement(XMLElement* el, const char* name) noexcept(false);


	/**
	 * Returns a double value obtained by converting the text in an XMLNode to a double. In case
	 * the node does not exists, this function returns a default value passed as a parameter.
	 * @param el a pointer to the XMLElement where the XMLNode is located.
	 * @param name the name of the XMLNode.
	 * @param default_value is the value returned in case the function doesn'f find any
	 * XMLNode with the specified name under the XMLElement.
	 * @return a double value obtained by converting the text in an XMLNode to a double.
	 */
	double 	getValue(XMLElement* el, const char* name, double default_value);


	/**
	 * Returns an int value obtained by converting the text in an XMLNode to an int. In case
	 * the node does not exists, this function returns a default value passed as a parameter.
	 * @param el a pointer to the XMLElement where the XMLNode is located.
	 * @param name the name of the XMLNode.
	 * @param default_value is the value returned in case the function doesn'f find any
	 * XMLNode with the specified name under the XMLElement.
	 * @return an int value obtained by converting the text in an XMLNode to an int.
	 */
	int getValue(XMLElement* el, const char* name, int default_value);

	/**
	 * Returns an unsigned long value obtained by converting the text in an XMLNode to an unsigned long. In case
	 * the node does not exists, this function returns a default value passed as a parameter.
	 * @param el a pointer to the XMLElement where the XMLNode is located.
	 * @param name the name of the XMLNode.
	 * @param default_value is the value returned in case the function doesn'f find any
	 * XMLNode with the specified name under the XMLElement.
	 * @return an unsigned long value obtained by converting the text in an XMLNode to an unsigned long.
	 */
	unsigned long getValue(XMLElement* el, const char* name, unsigned long default_value);

	/**
	 * Returns a string (const char*) value obtained by converting the text in an XMLNode to a const char* pointer. In case
	 * the node does not exists, this function returns a default value passed as a parameter.
	 * @param el a pointer to the XMLElement where the XMLNode is located.
	 * @param name the name of the XMLNode.
	 * @param default_value is the value returned in case the function doesn'f find any
	 * XMLNode with the specified name under the XMLElement.
	 * @return a const char* value obtained by converting the text in an XMLNode to a const char*.
	 */
	const char* getValue(XMLElement* el, const char* name, const char* default_value);

	/**
	 * Returns a double value obtained by converting the text in an XMLNode to a double. In case the node does not exist
	 * this method throws an exection.
	 * @param el a pointer to the XMLElement where the XMLNode is located.
	 * @param name the name of the XMLNode.
	 * @return a double value obtained by converting the text in an XMLNode to a double
	 */
	double getValue(XMLElement* el, const char* name);

	/**
	 * Computes the inverse of the normal CDF.
	 * @param p the value of a probability.
	 * @param mu the mean for the normal distribution.
	 * @param sigma the standard deviation of the normal distribution.
	 * @return the value of the inverse normal CDF.
	 */
	double inverseNormalCDF(const double p, const double mu, const double sigma);
	/**
	 * Computes the posterior probability of a mobile device to be in a tile of the Grid according to the method
	 * described in he paper "Deriving geographic location of mobile devices from network data"
	 * by Martijn Tennekes, Yvonne A.P.M. Gootzen, Shan H. Shah.
	 * @param t the time instant when the posterior localization probability is computed.
	 * @param m a pointer to a MobilePhone object for which the posterior localization probability is computed.
	 * @param data a vector of AntennaInfo objects generated and recorded by each antenna during the simulation.
	 * It contains the events recorder by each antenna during the simulation.
	 * @param it an iterator to access all objects of type Antenna from the AgentsCollection container.
	 * @param prior is used to set the method of computing the prior probabilities. It could take 3 values:
	 * PriorType::UNIFORM, PriorType::NETWORK or PriorType::REGISTER. Currently only
	 * UNIFORM and NETWORK methods are implemented.
	 * @return a vector with the posterior probability of the mobile phone given by m to be localized in a tile. The index of a value
	 * in this vector indicates the corresponding tile index. The size of this vector is equal to the total number of tiles in the Grid.
	 */
}

#endif
