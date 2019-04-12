/*
 * Data simulator for mobile phone network events
 *
 * LocatableAgent.cpp
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */

#include "LocatableAgent.h"
#include "Agent.h"
#include <geos/geom/Point.h>

using namespace geos;
using namespace geos::geom;

LocatableAgent::LocatableAgent(Map* m, long id, Point* initLocation) :
		Agent(m, id) {
	if (initLocation != nullptr) {
		const Coordinate *c = initLocation->getCoordinate();
		m_location = this->getMap()->getGlobalFactory()->createPoint(*c);
	}
	else
		m_location = nullptr;

}

LocatableAgent::~LocatableAgent() {
}

Point& LocatableAgent::getLocation() const {
	return *m_location;
}

void LocatableAgent::setLocation(Point& location) {
	const Coordinate *c = location.getCoordinate();
	m_location = this->getMap()->getGlobalFactory()->createPoint(*c);
}
