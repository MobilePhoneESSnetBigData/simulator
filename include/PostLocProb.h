/*
 * Copyright (C) 2020 Bogdan Oancea

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
 * PosteriorLocationProbabilities.h
 *
 *  Created on: Feb 06, 2020
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef INCLUDE_POSTLOCPROB_H_
#define INCLUDE_POSTLOCPROB_H_

#include <AntennaInfo.h>
#include <agent/AgentsCollection.h>
#include <map/Map.h>
#include <agent/MobilePhone.h>
#include <AntennaInfo.h>
#include <Clock.h>
#include <map>

using namespace std;
/**
 * This class implements a Strategy design pattern. It acts as an interface to different algorithms for posterior
 * location probabilities computation.
 */
class PostLocProb {
public:
	/**
	 * Constructor of the class. It initializes the members according to the values given as parameters.
	 * @param map a pointer to the Map object of the simulation.
	 * @param clk a pointer to the Clock object of the simulation.
	 * @param agents a pointer to the AgentsCollection object (it contains all the agents in a simulation).
	 * @param probFiles a map object containing the name of the files where the probabilities are saved, one file per MNO.
	 */
	PostLocProb(const Map* m, Clock* clk, AgentsCollection* agents, map<const unsigned long, const string> probFiles);

	/**
	 * Default destructor.
	 */
	virtual ~PostLocProb();

	/**
	 *  Computes the posterior location probabilities. It iterates over all time instants and mobile phone and calls the prob()
	 *  pure virtual function which implements the actual algorithm for the location probabilities.
	 * @param data a vector with the network events.
	 */
	virtual void computeProbabilities(std::map<unsigned long, vector<AntennaInfo>> data);

	/**
	 * A pure virtual function that is implemented in each of the concrete subclasses derived from \class PostLocProb.
	 * @param t the time instant for which the location probabilities are computed.
	 * @param m a pointer to a MobilePhone object for which the location probabilities are computed.
	 * @param data a vector with network events registered during the simulation.
	 * @param it iterator over the Antenna's objects.
	 * @return a vector with posterior location probabilities for a time instant and a mobile phone.
	 */
	virtual vector<double> prob(unsigned long t, MobilePhone* m, vector<AntennaInfo>& data, pair<um_iterator, um_iterator> it) = 0;

protected:
	const Map* m_map;
	Clock* m_clock;
	AgentsCollection* m_agents;
	map<const unsigned long, const string> m_probFilenames;


	void writeProbFileHeader(ofstream& f);

};

#endif /* SRC_POSTERIORLOCATIONPROBABILITIES_H_ */


/**
 * Constructor of the class. Initializes members.
 * @param map a pointer to the Map object of the simulation.
 * @param clk a pointer to the Clock object of the simulation.
 * @param speed the speed of displacement.
 */
