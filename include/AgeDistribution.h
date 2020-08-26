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
 * AgeDistribution.h
 *
 *  Created on: Aug 14, 2020
 *      Author: Bogdan.Oancea
 */

#ifndef INCLUDE_AGEDISTRIBUTION_H_
#define INCLUDE_AGEDISTRIBUTION_H_

/**
 * This is the base class for the age distribution among the individuals involved in a simulation.
 */
class AgeDistribution {
public:
	/**
	 * Constructor of the class. It only sets the minimum and maximum age.
	 * @param min_age the minimum age of a person.
	 * @param max_age the maximum age of a person.
	 */
	AgeDistribution(int min_age, int max_age);

	/**
	 * Destructor of the class.
	 */
	virtual ~AgeDistribution();

	/**
	 * Returns the value of the maximum age.
	 * @return the value of the maximum age.
	 */
	int getMaxAge() const;

	/**
	 * Sets the maximum age.
	 * @param maxAge the value of the maximum age.
	 */
	void setMaxAge(int maxAge);

	/**
	 * Returns the value of the minimum age of a person.
	 * @return the value of the minimum age of a person.
	 */
	int getMinAge() const;

	/**
	 * Sets the minimum age.
	 * @param minAge the value of the minimum age.
	 */
	void setMinAge(int minAge);

private:
	int m_minAge;
	int m_maxAge;
};

#endif /* INCLUDE_AGEDISTRIBUTION_H_ */
