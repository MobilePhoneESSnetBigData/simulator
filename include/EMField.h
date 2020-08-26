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
 * EMField.h
 *
 *  Created on: Apr 23, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef EMFIELD_H_
#define EMFIELD_H_

#include <agent/Antenna.h>
#include <Constants.h>
#include <map/Map.h>
#include <utility>
#include <vector>
#include <map>

using namespace std;
/**
 * This utility singleton class is used to compute different measures of the electromagnetic field radiated by an antenna (power, signal strength etc)
 * and it also provides methods needed to decide to which antenna a mobile device connects.
 */
class EMField {
public:
	/**
	 * Returns an instance of this class. This class is a singleton.
	 * @return an instance of this class.
	 */
	static EMField* instance() {
		if (m_instance == nullptr) {
			m_instance = new EMField();
		}
		return (m_instance);
	}

	/**
	 * Default destructor.
	 */
	virtual ~EMField();

	/**
	 * Add a pointer to an Antenna object to an internal collection needed for computations. Although these pointers are kept in
	 * an AgentCollection object they are also added to a local vector in this class for performance reasons.
	 * @param a a pointer to the Antenna object
	 */
	void addAntenna(Antenna* a);

	/**
	 * Returns a pair made of a pointer to an Antenna object and its power with the property that in the location specified by parameter p,
	 * the Antenna returned by this method provides the highest power (the power of the field is considered to decrease according a power-law).
	 * @param p the location where we want to find which Antenna provides the highest power the of electromagnetic field.
	 * @param mnoId the id of the MNO for which we compute the power. Only antennas belonging to this MNO will be considered during computations.
	 * @return a pair<Antenna*, double> containing a pointer to the Antenna object that provides the highest power of the field in the location
	 * specified by p.
	 */
	pair<Antenna*, double> computeMaxPower(const Point* p, const unsigned long mnoId);

	/**
	 * Returns a pair made of a pointer to an Antenna object and its signal quality with the property that in the
	 * location specified by  p, the Antenna returned by this method provides signal with the highest quality.
	 * The signal quality in this pair is the computed in location given by p.
	 * @param p indicates the location where we want to find which Antenna provides the highest quality of the signal.
	 * @param mnoId the id of the MNO for which we compute the signal quality. Only antennas belonging to this MNO will be considered during computations.
	 * @return a pair<Antenna*, double> containing a pointer to the Antenna object that provides a signal with the
	 * highest quality in location given by p.
	 */
	pair<Antenna*, double> computeMaxQuality(const Point* p, const unsigned long mnoId);


	/**
	 * Returns a pair made of a pointer to an Antenna object and its signal strength with the property that in the
	 * location specified by p, the Antenna returned by this method provides signal with the highest strength.
	 * The signal strength in this pair is the computed in location given by p.
	 * @param p indicates the location where we want to find which Antenna provides the highest strength of the signal.
	 * @param mnoId the id of the MNO for which we compute the signal strength. Only antennas belonging to this MNO will be considered during computations.
	 * @return a pair<Antenna*, double> containing a pointer to the Antenna object that provides a signal with the
	 * highest strength in location given by p.
	 */
	pair<Antenna*, double> computeMaxStrength(const Point* p, const unsigned long mnoId);


	/**
	 * Returns a vector of pairs made up of a pointer to an Antenna object and its power, signal quality or signal strength.
	 * All the antennas in this vector provides a signal with a power or signal quality greater than
	 * the threshold provided as threshold, i.e. this vector contains all antennas that have in
	 * their coverage area the location given by point p.
	 * @param p the location where we want to have the list with the all antennas that covers it.
	 * @param threshold the lowest limit of the power or signal quality below which the signal is considered to be only
	 * noise, i.e. it defines the limit of the coverage area.
	 * @param connType indicates the mechanism used to set up a connection between an antenna and a mobile phone
	 * @param mnoId the id of the MNO for which we build the resulting vector. Only antennas belonging to this MNO will be considered during computations.
	 * @return a vector of pairs made up of a pointer to an Antenna object and its power, signal quality or signal strength, according to the value
	 * of the connType.
	 * All the antennas in this vector provides a signal with a power, signal quality or signal strength greater than the threshold.
	 */
	vector<pair<Antenna*, double>> getInRangeAntennas(const Point* p, const double threshold, const HoldableAgent::CONNECTION_TYPE connType, unsigned long mnoId);

	/**
	 * Checks if p is in the coverage area of Antenna pointed out by a. The coverage area is considered the area
	 * where the signal quality or the power of the field is greater than the value of threshold.
	 * @param p the location that we want to check the power or the quality of the signal
	 * @param a pointer to an Antenna object for which we want to check if it covers the point p.
	 * @param threshold the lower limit of the power or signal quality below which the signal is considered only noise.
	 * @param connType indicates the mechanism used to set up a connection between an antenna and a mobile phone.
	 * @return true is the Antenna object provide enough power or signal quality in the location given as p.
	 */
	bool isAntennaInRange(const Point* p, Antenna* a, const double threshold, const HoldableAgent::CONNECTION_TYPE connType);

	/**
	 * Computes the connection likelihood for Antenna indicated by a in a certain location given by p.
	 * The connection likelihood is computed dividing the signal quality provided by Antenna indicated through p by the sum
	 * of the signal quality provided by all antennas of an MNO.
	 * @param a a pointer to an Antenna object.
	 * @param p a location in space.
	 * @return the connection likelihood for Antenna a in location  p.
	 */
	double connectionLikelihood(Antenna* a, const Point * p);

	/**
	 * Computes the sum of the signal quality given by all antennas belonging to an MNO for all tiles in the reference grid.
	 * The signal quality is computed in the center of each tile.
	 * @param map the of the simulation. It is needed to extract the grid of tiles where this method computes the sum of
	 * the signal quality. This grid is set at the beginning of the simulation and it overlaps the Map.
	 * @param mnoID the id of the MNO for which we want to compute this sum.
	 * @return a vector containing the sum of the signal quality given by all antennas of an MNO,
	 *  for all tiles in the reference grid. An element of the vector corresponds to a tile in the grid. The tiles
	 *  are linearized in a row-major order starting with the bottom-left corner.
	 */
	vector<double> sumSignalQuality(const Map* map, const unsigned long mnoID);

	/**
	 * Computes the connection likelihood for Antenna indicated by a in the center of the tile indicated by tileIndex
	 * @param a a pointer to an Antenna object.
	 * @param map a pointer to the map of the simulation.
	 * @param tileIndex the index of the tile where we want to compute the connection likelihood.
	 * @return the connection likelihood for Antenna  a in the center of the tile with the index tileIndex.
	 */
	double connectionLikelihoodGrid(Antenna* a, const Map* , unsigned long tileIndex);

	const double* getAntennaMin3DbArray() const;

	double* getSd() const;

private:
	EMField();

	EMField(const EMField&);
	EMField& operator=(const EMField&);

	static EMField* m_instance;
	vector<Antenna*> m_antennas;
	map<const unsigned long, vector<double>> m_sumQuality;

	double* m_antennaMin3DbArray;
	double* m_sd;
};

#endif /* EMFIELD_H_ */
