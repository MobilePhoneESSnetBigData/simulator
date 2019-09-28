/*
 * Data simulator for mobile phone network events
 *
 * LocatableAgent.h
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */

#ifndef LOCATABLEAGENT_H_
#define LOCATABLEAGENT_H_

#include <Agent.h>
#include <Clock.h>
#include <Map.h>
#include <geos/geom/Point.h>

using namespace geos;
using namespace geos::geom;
using namespace std;

/**
 * This class extends the Agent class and defines an object with a location on a map.
 */
class LocatableAgent: public Agent {
public:

	/**
	 * Constructor of the class. Builds an object that has a location on the map of the simulation.
	 * @param m a pointer to a Map object used in this simulation.
	 * @param id the id of the object.
	 * @param initLocation the initial location of the object.
	 * @param clock a pointer to a Clock object used in this simulation.
	 */
	explicit LocatableAgent(const Map* m, const unsigned long id, Point* initLocation, const Clock* clock);

	/**
	 * Destructor
	 */
	virtual ~LocatableAgent();

	/**
	 *  Returns the name of this class.
	 * @return the name of this class.
	 */
	const string getName() const override;

	/**
	 * Builds a human readable string representation of this class useful to output it to a file or on the screen.
	 * @return a string representation of this class.
	 */
	const string toString() const override;

	/**
	 *
	 * @return the location on the map as a pointer to a Point object.
	 */
	virtual Point* getLocation() const;

	/**
	 * Sets the location of the agent on the map.
	 * @param location the location of the agent on the map passed as a pointer to a Point object.
	 */
	virtual void setLocation(Point* location);

	/**
	 * Builds a human readable string representation of the location
	 * @return a human readable string representation of the location
	 */
	const string dumpLocation();

private:

	Point* m_location;
};

#endif /* LOCATABLEAGENT_H_ */
