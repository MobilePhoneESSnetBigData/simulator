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
 * RandomWalkDriftDisplacement.h
 *
 *  Created on: Nov 21, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef INCLUDE_RANDOMWALKDRIFTDISPLACEMENT_H_
#define INCLUDE_RANDOMWALKDRIFTDISPLACEMENT_H_

#include <Displace.h>


/**
 * This class is part of the Strategy design pattern used to implement the displacement of persons on the map. It implements
 * the random walk with drift behaviour, overriding the generateNewLocation() method from its superclass, Displace.
 */
class RandomWalkDriftDisplacement: public Displace {
public:

	/**
	 * Constructor of the class. It only passes the arguments to the superclass, Displace.
	 * @param map a pointer to the Map object of the simulation.
	 * @param clk a pointer to the Clock object of the simulation.
	 * @param speed the speed of displacement.
	 */
	RandomWalkDriftDisplacement(Map* map, Clock* clk, double speed);

	/**
	 * Implements the random walk with drift behaviour. During the first half of the simulation it generates a displacement direction
	 * as a random value normally distributed with the mean equals to SIM_TREND_ANGLE_1 (a predefined constant) and sd equals
	 * to 0.1 and during the second half the mean of the distribution is changed to SIM_TREND_ANGLE_2. The step length of the displacement along
	 * this direction is computed using the speed and the time duration of a simulation step. If the new location is outside the map,
	 * it returns the current location, i.e. the object will stay in the same location until the next simulation step and raise a flag, so
	 * that at the simulation step the direction is changed with approximately 180 degrees (the actual value is a random value from an uniform
	 * distribution centered at PI value).
	 * @param p a pointer to the current location.
	 * @return the new location.
	 */
	virtual Point* generateNewLocation(Point* p) override;

	/**
	 * Destructor. It does nothing.
	 */
	virtual ~RandomWalkDriftDisplacement();

private:
	bool m_changeDirection;
};

#endif /* INCLUDE_RANDOMWALKDRIFTDISPLACEMENT_H_ */
