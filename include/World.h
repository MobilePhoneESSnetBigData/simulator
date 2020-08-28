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
 * World.h
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef WORLD_H
#define WORLD_H

#include <agent/Antenna.h>
#include <agent/HoldableAgent.h>
#include <agent/MobileOperator.h>
#include <agent/MobilePhone.h>
#include <agent/Person.h>
#include <AgeDistribution.h>
#include <AntennaInfo.h>
#include <MovementType.h>
#include <PriorType.h>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class AgeDistribution;
class RandomNumberGenerator;

class AgentsCollection;
class PostLocProb;

using namespace std;
using namespace tinyxml2;

/**
 * This is the class where the simulation process takes place. A World object has a Map, a Clock, a set of Agents
 * than can be persons, mobile phones, antennas, mobile operators etc. After generating all the required objects
 * for simulation by reading the parameters from the input configuration files runSimulation() method is
 * called to perform the actual simulation. The output of the simulation process is written in several files.
 * Antenna objects output their registered events in a .csv file and after the simulation ends, these files are merged
 * in a single file that is used to compute the posterior localization probabilities for each mobile device. These probabilities
 * are computed using a Grid that is overlapped on the Map, i.e. we compute the probability of a mobile phone to be in a tile
 * of the grid. A finer the grid means more accurate localization but this come with an important computational cost.
 */
class World {
public:

	/**
	 * Builds a new World object, randomly generating a set of persons, antennas and mobile devices with the default parameters.
	 * This class is used only for testing and developing new features. For a real simulation the user should build the Wolrd
	 * object using the other constructor that takes the name of the input files as params.
	 * @param map a pointer to a Map object where the simulation takes place.
	 * @param numPersons the number of persons to be generated.
	 * @param numAntennas the number of antennas to be generated.
	 * @param numMobilePhones the number of mobile phones to be generated. These phones are randomly given to persons.
	 */
	World(Map* map, int numPersons, int numAntennas, int numMobilePhones);

	/**
	 * Builds a new World object, reading the parameters for the Persons, Antennas and Mobile Phones from configuration files.
	 * The configuration files are in XML format and they should be provided as command line parameters. The general parameters
	 * of the simulation (duration, how people move around the map, how mobile phone try to connect to antennas, etc. are also
	 * read from a configuration file:
	 * 	- the persons configuration file is provided through the -p parameter in the command line.
	 * 	- the antennas configuration file is provided through the -a parameter in the command line.
	 * 	- the simulation configuration file is provided through the -s parameter in the command line.
	 * 	- the posterior probabilities configuration file is provided through the -pb parameter in the command line.
	 *
	 * @param map a pointer to a Map object where the simulation takes place
	 * @param configPersonsFileName the configuration file name for the persons objects.
	 * @param configAntennasFileName the configuration file name for antenna objects.
	 * @param configSimulationFileName the general configuration file for a simulation.
	 * @param probabilitiesFileName the config file for the posterior location probabilites.
	 */
	World(Map* map, const string& configPersonsFileName, const string& configAntennasFileName, const string& configSimulationFileName,
			const string& probabilitiesFileName) noexcept(false);

	/**
	 * Destructor
	 * It releases the memory allocated for the agents collection and the Clock object.
	 */
	virtual ~World();

	/**
	 * This method is called to perform the actual simulation. During the simulation it outputs the exact positions
	 * of all persons in a .csv file and the positions of antennas at the starting time of the simulation. A simulation
	 * means a number of time steps, at each step every person move to another position and after arriving at their new
	 * positions the mobile phones that they carry try to connect to one of the available antennas. The antennas record
	 * these events and output them in a file.
	 */
	void runSimulation() noexcept(false);

	/**
	 * Returns the AgentsCollection used in simulation.
	 * @return a pointer to AgentsCollection object.
	 */
	AgentsCollection* getAgents() const;

	/**
	 * Returns a pointer to a Map object where the simulation takes place.
	 * @return a pointer to a Map object where the simulation takes place.
	 */
	const Map* getMap() const;

	/**
	 * Returns the file name where the grid parameters are saved. They are needed for the visualization software.
	 * @return the file name where the grid parameters are saved.
	 */
	const string& getGridFilename() const;

	Clock* getClock();

		void computeProbabilities(std::map<unsigned long, vector<AntennaInfo>> data);
	/**
	 * Returns the name of the file where the probabilities of mobile phones locations are saved.
	 * @return the name of the file where the probabilities of mobile phones locations are saved.
	 */
	map<const unsigned long, const string> getProbFilenames() const;

	/**
	 * Sets the posterior location probability method
	 * @param post a pointer to an object implementing the method to compute the posterior location probabilities.
	 */
	void setPostProbMethod(const std::shared_ptr<PostLocProb>& post);

	/**
	 * Returns the name of the output folder.
	 * @return the name of the output folder.
	 */
	const string& getOutputDir() const;

		/**
		 * At the end of a simulation this method merges all the events saved by individual antennas in a single data structure.
		 * @return a map of <MNO_ID, vector<AntennInfo>> where for each MNO identified by its ID has
		 * vector of all events saved by all antennas belonging to that MNO. This map is needed for computation of location probabilities.
		 */
		std::map<unsigned long, vector<AntennaInfo>> getEvents();

private:

	Map* m_map;

	AgentsCollection* m_agentsCollection;
	Clock* m_clock;
	unsigned long m_startTime;
	unsigned long m_endTime;
	unsigned long m_timeIncrement;
	double m_GridDimTileX;
	double m_GridDimTileY;
	PriorType m_prior;
	unsigned m_seed;
	unsigned long m_stay;
	unsigned m_intevalBetweenStays;
	double m_connThreshold;

	HoldableAgent::CONNECTION_TYPE m_connType;
	MovementType m_mvType;
	string m_gridFilename;
	map<const unsigned long, const string> m_probFilenames;
	string m_personsFilename;
	string m_antennasFilename;
	string m_outputDir;
	double m_probSecMobilePhone;
	shared_ptr<PostLocProb> m_postMethod;

	vector<Person*> generatePopulation(unsigned long numPersons, double percentHome);
	vector<Person*> generatePopulation(const unsigned long numPersons, shared_ptr<AgeDistribution> age_distribution,
			double male_share, vector<MobileOperator*> mnos, double speed_walk, double speed_car, double percentHome);

	vector<Antenna*> generateAntennas(unsigned long numAntennas);
	vector<Antenna*> parseAntennas(const string& configAntennasFile, vector<MobileOperator*> mnos) noexcept(false);
	vector<Person*> parsePersons(const string& personsFileName, vector<MobileOperator*> mnos) noexcept(false);
	vector<MobilePhone*> generateMobilePhones(int numMobilePhones, HoldableAgent::CONNECTION_TYPE connType);
	vector<MobileOperator*> parseSimulationFile(const string& configSimulationFileName) noexcept(false);
	int whichMNO(vector<pair<string, double>> probs, vector<MobileOperator*> mnos);
	string parseProbabilities(const string& probabilitiesFileName);
	double getDefaultConnectionThreshold(HoldableAgent::CONNECTION_TYPE connType);
	void setPersonDisplacementPattern(Person* p, MovementType type, Map* map, Clock* clk);
	void generatePhones(vector<MobileOperator*> mnos);
	int* generateAges(int n, shared_ptr<AgeDistribution> distr, RandomNumberGenerator* rng);
	void setPhones(int* &ph1, int* &ph2, double probSecMobilePhone, double numPersons, RandomNumberGenerator* rng, vector<MobileOperator*> mnos );
	void writeSignalAndCells(ostream& antennaFile);
	void AddMobilePhoneToPerson(Person* p, MobileOperator* mno, AgentsCollection* ag, const Map* map, Clock* clock, double thres, HoldableAgent::CONNECTION_TYPE conn );
	MovementType parseMovement(XMLElement* el);
	HoldableAgent::CONNECTION_TYPE parseConnectionType(XMLElement* el);
	vector<MobileOperator*> parseMNOs(XMLElement* el);
	double getGridDimTileX() const;
	double getGridDimTileY() const;
	HoldableAgent::CONNECTION_TYPE getConnectionType() const;
};

#endif // WORLD_H
