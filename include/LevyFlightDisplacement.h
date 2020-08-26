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
 * LevyFlightDisplacement.h
 *
 *  Created on: Aug 13, 2020
 *      Author: Bogdan.Oancea
 */

#ifndef INCLUDE_LEVYFLIGHTDISPLACEMENT_H_
#define INCLUDE_LEVYFLIGHTDISPLACEMENT_H_

#include <Displace.h>

/**
 * This class is part of the Strategy design pattern used to implement the displacement of persons on the map. It implements
 * the Levy flights, overriding the generateNewLocation() method from its superclass, Displace.
 */
class LevyFlightDisplacement: public Displace {
public:
	/**
	 * Constructor of the class. It only passes the arguments to the superclass, Displace.
	 * @param map a pointer to the Map object of the simulation.
	 * @param clk a pointer to the Clock object of the simulation.
	 * @param speed the speed of displacement.
	*/
	LevyFlightDisplacement(Map* map, Clock* clk,  double speed);

	/**
	 * Implements the Levy flight behavior. It takes a pointer to the current location, generates a uniformly distributed value
	 * between 0 and 2*PI as the angle of displacement and computes the length of the step in this direction using the
	 * speed and the time duration of a simulation step. To simulate a Levy flight, the value of the speed is generated at each time
	 * instant from a Levy distribution.
	 * If the new location is outside the map, it tries 10 times to generate another location. If after 10 trials the position
	 * is still outside the map returns the current location, i.e. the object will stay in the same location until the next simulation step.
	 * @param p a pointer to the current location.
	 * @return the new location
	 */
	virtual Point* generateNewLocation(Point* p) override;

	/**
	 * Destructor. Does nothing.
	*/
	virtual ~LevyFlightDisplacement();
private:
	virtual Point* computeNewLocation(Point* initLocation, double theta) override;

};

#endif /* INCLUDE_LEVYFLIGHTDISPLACEMENT_H_ */


