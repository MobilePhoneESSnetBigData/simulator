/*
 * Data simulator for mobile phone network events
 *
 * utils.cpp
 *
 *  Created on: Apr 11, 2019
 *      Author: Bogdan Oancea
 */

#ifndef UTILS_H
#define UTILS_H

#include <geos/geom/Point.h>
#include <TinyXML2.h>
#include <cmath>
#include <vector>

class Map;

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
	 * Prints out a header containing the names of the member variables from the Person class in a human readable format.
	 * It is used together with Person::toString() to output the Persons set on console
	 */
	void printPersonHeader();

	/**
	 * Prints out a header containing the names of the member variables from the Antenna class in a human readable format
	 * It is used together with Antenna::toString() to output the antennas set on console
	 */
	void printAntennaHeader();

	/**
	 * Prints out a header containing the names of the member variables from the MobilePhone class in a human readable format
	 * It is used together with MobilePhone::toString() to output the mobile phones set on console
	 */
	void printPhoneHeader();

	/**
	 * Prints out a header containing the names of the member variables from the MobileOperator class in a human readable format
	 * It is used together with MobileOperator::toString() to output the mobile network operators set on console
	 */
	void printMobileOperatorHeader();


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
}

#endif
