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
 * TruncatedNormalAgeDistribution.h
 *
 *  Created on: Aug 14, 2020
 *      Author: Bogdan.Oancea
 */

#ifndef INCLUDE_TRUNCATEDNORMALAGEDISTRIBUTION_H_
#define INCLUDE_TRUNCATEDNORMALAGEDISTRIBUTION_H_

#include <AgeDistribution.h>

/**
 * Subclass of the \class AgeDistribution. It implements a truncated normal distribution.
 */
class TruncatedNormalAgeDistribution: public AgeDistribution {
public:
	/**
	 * Constructor of the class. It sets the members according to the values given as parameters.
	 * @param minAge the minimum value of the truncated normal age distribution.
	 * @param maxAge the maximum value of the truncated normal age distribution.
	 * @param meanAge the mean value of the truncated normal age distribution.
	 * @param sd the standard deviation value of the  standard deviation.
	 */
	TruncatedNormalAgeDistribution(int minAge, int maxAge, double meanAge, double sd);

	/**
	 * Default destructor.
	 */
	virtual ~TruncatedNormalAgeDistribution();

	/**
	 * Returns the mean of the age distribution.
	 * @return the mean value of the age distribution.
	 */
	double getMean() const;

	/**
	 * Sets the mean value of the age distribution.
	 * @param mean the mean value of the age distribution.
	 */
	void setMean(double mean);

	/**
	 * Returns the standard deviation of the age distribution.
	 * @return the standard deviation of the age distribution.
	 */
	double getSd() const;

	/**
	 * Sets the value of the standard deviation of the age distribution.
	 * @param sd the standard deviation of the age distribution.
	 */
	void setSd(double sd);

private:
	double m_mean;
	double m_sd;
};

#endif /* INCLUDE_TRUNCATEDNORMALAGEDISTRIBUTION_H_ */
