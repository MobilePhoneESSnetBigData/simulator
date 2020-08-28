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
 * World.cpp
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <agent/AgentsCollection.h>
#include <AntennaType.h>
#include <Clock.h>
#include <Constants.h>
#include <CSVparser.hpp>
#include <EMField.h>
#include <geos/geom/Point.h>
#include <IDGenerator.h>
#include <LevyFlightDisplacement.h>
#include <map/Map.h>
#include <NetPriorPostLocProb.h>
#include <RandomNumberGenerator.h>
#include <RandomWalkDisplacement.h>
#include <RandomWalkDriftDisplacement.h>
#include <TinyXML2.h>
#include <TruncatedNormalAgeDistribution.h>
#include <UniformAgeDistribution.h>
#include <UnifPriorPostLocProb.h>
#include <Utils.h>
#include <World.h>
#include <algorithm>
#include <cstring>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <typeinfo>
#include <unordered_map>


using namespace std;
using namespace utils;
using namespace tinyxml2;

//ctor
World::World(Map* map, int numPersons, int numAntennas, int numMobilePhones) :
		m_map { map } {
	m_agentsCollection = new AgentsCollection();
	m_clock = new Clock();
	m_mvType = MovementType::RANDOM_WALK_CLOSED_MAP;
	m_connType = HoldableAgent::CONNECTION_TYPE::USING_POWER;
	m_gridFilename = Constants::GRID_FILE_NAME;
	m_personsFilename = Constants::PERSONS_FILE_NAME;
	m_antennasFilename = Constants::ANTENNAS_FILE_NAME;
	m_probSecMobilePhone = 0.0;
	m_seed = -1;

	vector<Person*> persons = generatePopulation(numPersons, 0.8);
	for (unsigned long i = 0; i < persons.size(); i++) {
		m_agentsCollection->addAgent(persons[i]);
	}

	vector<Antenna*> antennas = generateAntennas(numAntennas);
	for (unsigned long i = 0; i < antennas.size(); i++) {
		m_agentsCollection->addAgent(antennas[i]);
		EMField::instance()->addAntenna(antennas[i]);
	}

	vector<MobilePhone*> phones = generateMobilePhones(numMobilePhones, m_connType);
	for (unsigned long i = 0; i < phones.size(); i++) {
		m_agentsCollection->addAgent(phones[i]);
	}
}

World::World(Map* mmap, const string& configPersonsFileName, const string& configAntennasFile, const string& configSimulationFileName,
		const string& probabilitiesFileName) :
		m_map { mmap } {

	m_probSecMobilePhone = 0.0;
	vector<MobileOperator*> mnos = parseSimulationFile(configSimulationFileName);
	m_agentsCollection = new AgentsCollection();
	m_clock = new Clock(m_startTime, m_endTime, m_timeIncrement);
	time_t tt = m_clock->realTime();
	cout << "Generating objects started at " << ctime(&tt) << endl;

	string probsPrefix;
	if (!probabilitiesFileName.empty())
		probsPrefix = parseProbabilities(probabilitiesFileName);

	for (unsigned long i = 0; i < mnos.size(); i++) {
		m_agentsCollection->addAgent(mnos[i]);
		if (!probabilitiesFileName.empty())
			m_probFilenames.insert(pair<const unsigned long, string>(mnos[i]->getId(), m_outputDir + "/" + probsPrefix + "_" + mnos[i]->getMNOName() + ".csv"));
	}

	vector<Antenna*> antennas = parseAntennas(configAntennasFile, mnos);
	for (unsigned long i = 0; i < antennas.size(); i++) {
		antennas[i]->setHandoverMechanism(getConnectionType());
		m_agentsCollection->addAgent(antennas[i]);
		EMField::instance()->addAntenna(antennas[i]);
	}

	mmap->addGrid(getGridDimTileX(), getGridDimTileY());
	vector<Person*> persons = parsePersons(configPersonsFileName, mnos);
	for (unsigned long i = 0; i < persons.size(); i++) {
		m_agentsCollection->addAgent(persons[i]);
	}

	if (m_prior == PriorType::UNIFORM) {
		auto postProb = std::make_shared<UnifPriorPostLocProb>(m_map, m_clock, m_agentsCollection, m_probFilenames);
		setPostProbMethod(postProb);
	} else if (m_prior == PriorType::NETWORK) {
		auto postProb = std::make_shared<NetPriorPostLocProb>(m_map, m_clock, m_agentsCollection, m_probFilenames);
		setPostProbMethod(postProb);
	}
	tt = m_clock->realTime();
	cout << "Generating objects ended at " << ctime(&tt) << endl;
}

World::~World() {
	delete m_clock;
	delete m_agentsCollection;
	cout << "End of simulation!" << endl;
}

void World::runSimulation() noexcept(false) {
	ofstream personsFile, antennaFile;
	char sep = Constants::sep;

	try {
		personsFile.open(m_outputDir + "/" + m_personsFilename, ios::out);
		antennaFile.open(m_outputDir + "/" + m_antennasFilename, ios::out);
		writeSignalAndCells(antennaFile);
		antennaFile.close();
	} catch (ofstream::failure& e) {
		cerr << "Error opening output files!" << endl;
		throw e;
	}
	time_t tt = m_clock->realTime();

	personsFile << "t" << sep << "Person ID" << sep << "x" << sep << "y" << sep << "Tile ID" << sep << "Mobile Phone(s) ID" << endl;
	//initial time
	unsigned long t = m_clock->getInitialTime();
	tt = m_clock->realTime();
	cout << "Simulation started at " << ctime(&tt) << endl;
	cout << "Current simulation step: " << m_clock->getCurrentTime() << ":" << ctime(&tt) << endl;
	auto itr = m_agentsCollection->getAgentListByType(typeid(Person).name());
	for (auto it = itr.first; it != itr.second; it++) {
		Person* p = static_cast<Person*>(it->second);
		Point* loc = p->getLocation();
		personsFile << p->dumpLocation() << sep << m_map->getTileNo(loc->getX(), loc->getY()) << p->dumpDevices() << endl;
	}
	//iterate over all persons and call move()
	t = m_clock->tick();
	for (; t < m_clock->getFinalTime(); t = m_clock->tick()) {
		tt = m_clock->realTime();
		cout << "Current simulation step: " << m_clock->getCurrentTime() << ":" << ctime(&tt) << endl;
		for (auto it = itr.first; it != itr.second; it++) {
			Person* p = static_cast<Person*>(it->second);
			p->move();
			Point* loc = p->getLocation();
			personsFile << p->dumpLocation() << sep << m_map->getTileNo(loc->getX(), loc->getY()) << p->dumpDevices() << endl;
		}
	}
	tt = m_clock->realTime();
	cout << "Simulation ended at " << ctime(&tt) << endl;

	try {
		personsFile.close();
	} catch (std::ofstream::failure& e) {
		cerr << "Error closing output files!" << endl;
		throw e;
	}
}

AgentsCollection* World::getAgents() const {
	return (m_agentsCollection);
}

const Map* World::getMap() const {
	return (m_map);
}

const string& World::getGridFilename() const {
	return (m_gridFilename);
}

vector<Person*> World::generatePopulation(unsigned long numPersons, double percentHome) {
	vector<Person*> result;
	unsigned long id;
	vector<Point*> positions = utils::generatePoints(getMap(), numPersons, percentHome, m_seed);
	RandomNumberGenerator* random_generator = RandomNumberGenerator::instance(m_seed);
	double* speeds = random_generator->generateNormal2Double(0.3, 0.1, 1.5, 0.1, numPersons);
	int* ages = random_generator->generateUniformInt(1, 100, numPersons);
	for (unsigned long i = 0; i < numPersons; i++) {
		id = IDGenerator::instance()->next();
		Person* p = new Person(getMap(), id, positions[i], m_clock, speeds[i], ages[i], Person::Gender::MALE, Constants::SIM_STAY_TIME,
				Constants::SIM_INTERVAL_BETWEEN_STAYS);
		result.push_back(p);
	}
	delete[] speeds;
	delete[] ages;

	return (result);
}

vector<Antenna*> World::generateAntennas(unsigned long numAntennas) {
	vector<Antenna*> result;
	unsigned long id;
	double power = Constants::ANTENNA_POWER;
	double attFactor = Constants::ATT_FACTOR;
	unsigned long maxConnections = Constants::ANTENNA_MAX_CONNECTIONS;
	double smid = Constants::ANTENNA_S_MID;
	double ssteep = Constants::ANTENNA_S_STEEP;

	vector<Point*> positions = utils::generateFixedPoints(getMap(), numAntennas, m_seed);
	for (unsigned long i = 0; i < numAntennas; i++) {
		id = IDGenerator::instance()->next();
		string outDir = Constants::OUTPUT_DIR;
		Antenna* p = new Antenna(getMap(), id, positions[i], m_clock, attFactor, power, maxConnections, smid, ssteep,
				AntennaType::OMNIDIRECTIONAL, outDir);
		result.push_back(p);
	}
	return (result);
}

vector<MobilePhone*> World::generateMobilePhones(int numMobilePhones, HoldableAgent::CONNECTION_TYPE connType) {
	vector<MobilePhone*> result;
	unsigned long id;
	for (auto i = 0; i < numMobilePhones; i++) {
		id = IDGenerator::instance()->next();
		MobilePhone* p = new MobilePhone(getMap(), id, nullptr, nullptr, m_clock, Constants::PHONE_CONNECTION_THRESHOLD, connType);
		result.push_back(p);
		m_agentsCollection->addAgent(p);
	}
	return (result);
}

vector<Antenna*> World::parseAntennas(const string& configAntennasFile, vector<MobileOperator*> mnos) noexcept(false) {
	vector<Antenna*> result;
	XMLDocument doc;
	XMLError err = doc.LoadFile(configAntennasFile.c_str());
	if (err != XML_SUCCESS)
		throw std::runtime_error("Error opening configuration file for antennas ");

	XMLElement* antennasNode = doc.FirstChildElement("antennas");
	if (!antennasNode)
		throw std::runtime_error("Syntax error in the configuration file for antennas ");
	else {
		XMLElement* antennaEl = utils::getFirstChildElement(antennasNode, "antenna");
		for (; antennaEl; antennaEl = antennaEl->NextSiblingElement()) {
			if (antennaEl && strcmp(antennaEl->Name(), "antenna")) {
				cout << "unknown element: " << antennaEl->Name() << " ignoring it" << endl;
				continue;
			}
			unsigned long id = IDGenerator::instance()->next();
			Antenna* a = new Antenna(getMap(), m_clock, id, antennaEl, mnos, m_outputDir);
			result.push_back(a);
		}
	}
	return (result);
}

vector<Person*> World::parsePersons(const string& personsFileName, vector<MobileOperator*> mnos) noexcept(false) {
	vector<Person*> result;
	XMLDocument doc;
	XMLError err = doc.LoadFile(personsFileName.c_str());
	if (err != XML_SUCCESS)
		throw std::runtime_error("Error opening configuration file for persons ");

	XMLElement* personsEl = doc.FirstChildElement("persons");
	if (!personsEl)
		throw std::runtime_error("Syntax error in the configuration file for persons ");
	else {
		unsigned long numPersons = getValue(personsEl, "num_persons", Constants::SIM_NO_PERSONS);
		unsigned long min_age = getValue(personsEl, "min_age");
		unsigned long max_age = getValue(personsEl, "max_age");

		XMLElement* ageDistribEl = getFirstChildElement(personsEl, "age_distribution");
		const char* distrib = getValue(ageDistribEl, "type", "UNKNOWN");
		if (strcmp(distrib, "normal") && strcmp(distrib, "uniform"))
			throw std::runtime_error("Unknown age distribution for population!");

		shared_ptr<AgeDistribution> ageDistr;
		if (!strcmp(distrib, "normal")) {
			double mean_age = getValue(ageDistribEl, "mean");
			double sd = getValue(ageDistribEl, "sd");
			ageDistr = make_shared<TruncatedNormalAgeDistribution>(TruncatedNormalAgeDistribution(min_age, max_age, mean_age, sd));
		} else if (!strcmp(distrib, "uniform")) {
			ageDistr = make_shared<UniformAgeDistribution>(UniformAgeDistribution(min_age, max_age));
		} else {
			ageDistr = make_shared<UniformAgeDistribution>(UniformAgeDistribution(min_age, max_age));
		}

		double male_share = getValue(personsEl, "male_share");
		double speed_walk = getValue(personsEl, "speed_walk");
		double speed_car = getValue(personsEl, "speed_car");
		double percentHome = getValue(personsEl, "percent_home");
		result = generatePopulation(numPersons, ageDistr, male_share, mnos, speed_walk, speed_car, percentHome);
	}
	return (result);
}

double World::getGridDimTileX() const {
	return (m_GridDimTileX);
}

double World::getGridDimTileY() const {
	return (m_GridDimTileY);
}

vector<MobileOperator*> World::parseSimulationFile(const string& configSimulationFileName) noexcept(false) {
	XMLDocument doc;
	vector<MobileOperator*> result;
	XMLError err = doc.LoadFile(configSimulationFileName.c_str());
	if (err != XML_SUCCESS)
		throw std::runtime_error("Error opening configuration file for simulation ");

	XMLElement* simEl = doc.FirstChildElement("simulation");
	if (!simEl)
		throw std::runtime_error("Syntax error in the configuration file for simulation ");
	else {
		m_outputDir = getValue(simEl, "output_dir", Constants::OUTPUT_DIR);
		#ifdef OSX
		try{
			std::__fs::filesystem::create_directory(std::__fs::filesystem::path(m_outputDir.c_str()));
		} catch(std::__fs::filesystem::filesystem_error& e) {
			cerr << "Cannot create output folder :" << m_outputDir;
			throw e;
		}
		else
			try{
				std::filesystem::create_directory(std::filesystem::path(m_outputDir.c_str()));
			} catch(std::filesystem::filesystem_error& e) {
				cerr << "Cannot create output folder :" << m_outputDir;
				throw e;
			}
		#endif
		m_startTime = getValue(simEl, "start_time", Constants::SIM_START_TIME);
		m_endTime = getValue(simEl, "end_time", Constants::SIM_END_TIME);
		m_timeIncrement = getValue(simEl, "time_increment", Constants::SIM_INCREMENT_TIME);
		m_stay = getValue(simEl, "time_stay", Constants::SIM_STAY_TIME);
		m_intevalBetweenStays = getValue(simEl, "interval_between_stays", Constants::SIM_INTERVAL_BETWEEN_STAYS);
		result = parseMNOs(simEl);
		m_probSecMobilePhone = getValue(simEl, "prob_sec_mobile_phone", Constants::SIM_PROB_SECOND_MOBILE_PHONE);
		m_mvType = parseMovement(simEl);
		m_connType = parseConnectionType(simEl);
		m_connThreshold = getValue(simEl, "conn_threshold", getDefaultConnectionThreshold(m_connType));
		m_gridFilename = getValue(simEl, "grid_file", Constants::GRID_FILE_NAME);
		m_personsFilename = getValue(simEl, "persons_file", Constants::PERSONS_FILE_NAME);
		m_antennasFilename = getValue(simEl, "antennas_file", Constants::ANTENNAS_FILE_NAME);
		m_GridDimTileX = getValue(simEl, "grid_dim_tile_x", Constants::GRID_DIM_TILE_X);
		m_GridDimTileY = getValue(simEl, "grid_dim_tile_y", Constants::GRID_DIM_TILE_Y);
		m_seed = getValue(simEl, "random_seed", Constants::RANDOM_SEED);
	}
	return (result);
}

double World::getDefaultConnectionThreshold(HoldableAgent::CONNECTION_TYPE connType) {
	double result = -1;
	if (connType == HoldableAgent::CONNECTION_TYPE::USING_POWER)
		result = Constants::PHONE_POWER_THRESHOLD;
	else if (connType == HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_QUALITY)
		result = Constants::PHONE_QUALITY_THRESHOLD;
	else if (connType == HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_STRENGTH)
		result = Constants::PHONE_STRENGTH_THRESHOLD;
	return (result);
}

vector<Person*> World::generatePopulation(unsigned long numPersons, shared_ptr<AgeDistribution> ageDistribution,
		double male_share, vector<MobileOperator*> mnos, double speed_walk, double speed_car, double percentHome) {

	vector<Person*> result;
	unsigned long id;
	RandomNumberGenerator* random_generator = RandomNumberGenerator::instance(m_seed);
	int *phone1 = nullptr, *phone2 = nullptr;
	setPhones(phone1, phone2, m_probSecMobilePhone, numPersons, random_generator, mnos);

	int* walk_car = random_generator->generateBernoulliInt(0.5, numPersons);
	int sum = 0;
	sum = accumulate(walk_car, walk_car + numPersons, sum);
	int* gender = random_generator->generateBinomialInt(1, male_share, numPersons);
	double* speeds_walk = random_generator->generateNormalDouble(speed_walk, 0.1 * speed_walk, numPersons - sum);
	double* speeds_car = random_generator->generateNormalDouble(speed_car, 0.1 * speed_car, sum);
	int* ages = generateAges(numPersons, ageDistribution, random_generator );
	unsigned long cars = 0, walks = 0;
	Person* p;
	vector<Point*> positions = utils::generatePoints(getMap(), numPersons, percentHome, m_seed);
	unsigned int numMno = mnos.size();
	for (unsigned long i = 0; i < numPersons; i++) {
		id = IDGenerator::instance()->next();
		unsigned long stay = (unsigned long) random_generator->generateNormalDouble(m_stay, 0.2 * m_stay);
		unsigned long interval = (unsigned long) random_generator->generateExponentialDouble(1.0 / m_intevalBetweenStays);
		if (walk_car[i]) {
			p = new Person(getMap(), id, positions[i], m_clock, speeds_car[cars++], ages[i],
					gender[i] ? Person::Gender::MALE : Person::Gender::FEMALE, stay, interval);
		} else {
			p = new Person(getMap(), id, positions[i], m_clock, speeds_walk[walks++], ages[i],
					gender[i] ? Person::Gender::MALE : Person::Gender::FEMALE, stay, interval);
		}
		int np1 = phone1[i];
		while (np1) {
			AddMobilePhoneToPerson(p, mnos[0], m_agentsCollection, getMap(), m_clock, m_connThreshold, m_connType );
			np1--;
		}
		if (numMno == 2) {
			int np2 = phone2[i];
			while (np2) {
				AddMobilePhoneToPerson(p, mnos[1], m_agentsCollection, getMap(), m_clock, m_connThreshold, m_connType );
				np2--;
			}
		}
		setPersonDisplacementPattern(p, m_mvType, m_map, m_clock);
		result.push_back(p);
	}
	delete[] walk_car;
	delete[] phone1;
	delete[] phone2;
	delete[] speeds_walk;
	delete[] speeds_car;
	delete[] ages;
	delete[] gender;
	return (result);
}

string World::parseProbabilities(const string& probabilitiesFileName) {
	XMLDocument doc;
	string probsFileNamePrefix;
	XMLError err = doc.LoadFile(probabilitiesFileName.c_str());
	if (err != XML_SUCCESS)
		throw std::runtime_error("Error opening configuration file for probabilities ");

	XMLElement* probEl = doc.FirstChildElement("probabilities");
	if (!probEl)
		throw std::runtime_error("Syntax error in the configuration file for probabilities ");
	else {
		const char* prior = getValue(probEl, "prior", "UNKNOWN");
		if (!strcmp(prior, "network"))
			m_prior = PriorType::NETWORK;
		else if (!strcmp(prior, "uniform"))
			m_prior = PriorType::UNIFORM;
		else if (!strcmp(prior, "register"))
			m_prior = PriorType::REGISTER;
		else
			m_prior = Constants::PRIOR_PROBABILITY;
		probsFileNamePrefix = getValue(probEl, "prob_file_name_prefix", Constants::PROB_FILE_NAME_PREFIX);
	}
	return probsFileNamePrefix;
}

map<const unsigned long, const string> World::getProbFilenames() const {
	return m_probFilenames;
}

const string& World::getOutputDir() const {
	return m_outputDir;
}

HoldableAgent::CONNECTION_TYPE World::getConnectionType() const {
	return (m_connType);
}

void World::setPostProbMethod(const std::shared_ptr<PostLocProb>& post) {
	m_postMethod = post;
}

PriorType World::getPrior() {
	return m_prior;
}

void World::computeProbabilities(std::map<unsigned long, vector<AntennaInfo>> data) {
	m_postMethod->computeProbabilities(data);
}

std::map<unsigned long, vector<AntennaInfo>> World::getEvents() {
	char sep = Constants::sep;
	std::map<unsigned long, vector<AntennaInfo>> data;
	auto itr_mno = m_agentsCollection->getAgentListByType(typeid(MobileOperator).name());
	auto itra = m_agentsCollection->getAgentListByType(typeid(Antenna).name());

		for (auto itmno = itr_mno.first; itmno != itr_mno.second; itmno++) {
		MobileOperator* mo = static_cast<MobileOperator*>(itmno->second);
		vector<AntennaInfo> tmp;
		for (auto it = itra.first; it != itra.second; it++) {
			Antenna* a = static_cast<Antenna*>(it->second);
			if (a->getMNO()->getId() == mo->getId()) {
				string fileName = mo->getOutputDir() + "/" + a->getAntennaOutputFileName();
				CSVParser file = CSVParser(fileName, DataType::eFILE, ',', true);
				for (unsigned long i = 0; i < file.rowCount(); i++) {
					Row s = file[i];
					AntennaInfo a(stoul(s[0]), stoul(s[1]), stoul(s[2]), stoul(s[3]), stod(s[4]), stod(s[5]));
					tmp.push_back(a);
				}
			}
			sort(tmp.begin(), tmp.end());
			ofstream antennaInfoFile;
			string name = mo->getOutputDir() + "/" + string("AntennaInfo_MNO_" + mo->getMNOName() + ".csv");
			antennaInfoFile.open(name, ios::out);
			antennaInfoFile << "t,Antenna ID,Event Code,Device ID,x,y, Tile ID" << endl;
			for (AntennaInfo& ai : tmp) {
				antennaInfoFile << ai.toString() << sep << m_map->getTileNo(ai.getX(), ai.getY()) << endl;
			}
			antennaInfoFile.close();
		}
	}
	return (data);
}

Clock* World::getClock() {
	return m_clock;
}

void World::setPersonDisplacementPattern(Person* p, MovementType type, Map* map, Clock* clk) {
	if (type == MovementType::RANDOM_WALK_CLOSED_MAP) {
		auto displace = std::make_shared<RandomWalkDisplacement>(map, clk, p->getSpeed());
		p->setDisplacementMethod(displace);
	} else if (type == MovementType::RANDOM_WALK_CLOSED_MAP_WITH_DRIFT) {
		auto displace = std::make_shared<RandomWalkDriftDisplacement>(map, clk, p->getSpeed());
		p->setDisplacementMethod(displace);
	} else if (type == MovementType::LEVY_FLIGHT) {
		auto displace = std::make_shared<LevyFlightDisplacement>(map, clk, p->getSpeed());
		p->setDisplacementMethod(displace);
	}
}

int* World::generateAges(int n, shared_ptr<AgeDistribution> distr, RandomNumberGenerator* rng) {
	int* ages = new int[n];

	if(shared_ptr<UniformAgeDistribution> ageDistr = dynamic_pointer_cast<UniformAgeDistribution>(distr)){
		ages = rng->generateUniformInt(ageDistr->getMinAge(),ageDistr->getMaxAge(), n);
	}
	else if (shared_ptr<TruncatedNormalAgeDistribution> ageDistr = dynamic_pointer_cast<TruncatedNormalAgeDistribution>(distr)) {
		ages = rng->generateTruncatedNormalInt(ageDistr->getMinAge(),ageDistr->getMaxAge(), ageDistr->getMean(),ageDistr->getSd(), n);
	} else {
		ages = rng->generateUniformInt(distr->getMinAge(),distr->getMaxAge(), n);
	}
	return (ages);
}

void World::setPhones(int* &ph1, int* &ph2, double probSecMobilePhone, double numPersons, RandomNumberGenerator* rng, vector<MobileOperator*> mnos ) {
	int numMno = mnos.size();
	double probIntersection = 1;
	for (auto& n : mnos) {
		probIntersection *= n->getProbMobilePhone();
	}
	if (numMno > 1 && probIntersection > probSecMobilePhone)
		throw std::runtime_error(
				"Error specifying probabilities of having mobile phones: the probability of having a second mobile phone should be greater than or equal to the product of the probabilities of having mobile phones at each MNO");

	if (numMno == 1) {
		ph1 = rng->generateBernoulliInt(mnos[0]->getProbMobilePhone(), numPersons);
		for (unsigned long j = 0; j < numPersons; j++) {
			if (ph1[j] == 1)
				ph1[j] += rng->generateBernoulliInt(probSecMobilePhone);
		}
	} else if (numMno == 2) {
		double pSecPhoneMNO1 = (probSecMobilePhone - probIntersection) / 2.0;
		double pSecPhoneMNO2 = pSecPhoneMNO1;

		double pOnePhoneMNO1 = mnos[0]->getProbMobilePhone() - pSecPhoneMNO1 ;
		double pOnePhoneMNO2 = mnos[1]->getProbMobilePhone() - pSecPhoneMNO2 ;

		ph1 = rng->generateBernoulliInt(pOnePhoneMNO1, numPersons);
		ph2 = rng->generateBernoulliInt(pOnePhoneMNO2, numPersons);
		for (unsigned long i = 0; i < numPersons; i++) {
			if(ph1[i] == 1 && ph2[i] == 1)
				continue;
			if(ph1[i] == 1)
				ph1[i] += rng->generateBernoulliInt(pSecPhoneMNO1/pOnePhoneMNO1);
			if(ph2[i] == 1)
				ph2[i] += rng->generateBernoulliInt(pSecPhoneMNO2/pOnePhoneMNO2);
		}
	} else {
		throw std::runtime_error("Number of MNOs supported should be 1 or 2!");
	}
}

void World::writeSignalAndCells(ostream& antennaFile) {
	char sep = Constants::sep;
	auto itr0 = m_agentsCollection->getAgentListByType(typeid(MobileOperator).name());
	for (auto it = itr0.first; it != itr0.second; it++) {
		MobileOperator* mo = static_cast<MobileOperator*>(it->second);
		mo->writeSignalFileHeader();
	}
	auto itr2 = m_agentsCollection->getAgentListByType(typeid(Antenna).name());
	antennaFile << "t" << sep << "Antenna ID" << sep << "x" << sep << "y" << sep << "MNO ID" << sep << "Tile ID" << endl;
	for (auto it = itr2.first; it != itr2.second; it++) {
		Antenna* a = static_cast<Antenna*>(it->second);
		antennaFile << a->dumpLocation() << sep << a->getMNO()->getId() << sep << m_map->getTileNo(a->getLocation()) << endl;
		ofstream& f = a->getMNO()->getAntennaCellsFile();
		f << a->getId() << sep << a->dumpCell();
		a->dumpSignal();
	}
}

void World::AddMobilePhoneToPerson(Person* p, MobileOperator* mno, AgentsCollection* ag, const Map* map, Clock* clock, double thres, HoldableAgent::CONNECTION_TYPE conn ) {
	unsigned long id = IDGenerator::instance()->next();
	MobilePhone* mp = new MobilePhone(map, id, nullptr, nullptr, clock, thres, conn);
	mp->setMobileOperator(mno);
	mp->setHolder(p);
	ag->addAgent(mp);
	p->addDevice(typeid(MobilePhone).name(), mp);
}

MovementType World::parseMovement(XMLElement* el) {
	MovementType result;
	const char* mvType = getValue(el, "movement_type", "UNKNOWN");
	if (!strcmp(mvType, "random_walk_closed_map"))
		result = MovementType::RANDOM_WALK_CLOSED_MAP;
	else if (!strcmp(mvType, "random_walk_closed_map_drift")) {
		result = MovementType::RANDOM_WALK_CLOSED_MAP_WITH_DRIFT;
	}
	else if (!strcmp(mvType, "levy_flight")) {
		result = MovementType::LEVY_FLIGHT;
	}
	else
		result = MovementType::UNKNOWN;
	return (result);
}

HoldableAgent::CONNECTION_TYPE World::parseConnectionType(XMLElement* el) {
	HoldableAgent::CONNECTION_TYPE result;
	const char* connType = getValue(el, "connection_type", "UNKNOWN");
	if (!strcmp(connType, "power"))
		result = HoldableAgent::CONNECTION_TYPE::USING_POWER;
	else if (!strcmp(connType, "quality"))
		result = HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_QUALITY;
	else if (!strcmp(connType, "strength"))
		result = HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_STRENGTH;
	else
		throw runtime_error("Unknown connection mechanism! Available values: power, quality, strength");
	return (result);
}


vector<MobileOperator*> World::parseMNOs(XMLElement* el) {
	vector<MobileOperator*> result;
	unsigned numMNO = 0;
	XMLElement* mnoEl = utils::getFirstChildElement(el, "mno");
	if (mnoEl) {
		for (; mnoEl; mnoEl = mnoEl->NextSiblingElement("mno")) {
			numMNO++;
			const char* name = getValue(mnoEl, "mno_name", "UNKNOWN");
			const double prob = getValue(mnoEl, "prob_mobile_phone");
			unsigned long id = IDGenerator::instance()->next();
			MobileOperator* mo = new MobileOperator(getMap(), id, m_clock, name, prob, m_outputDir);
			result.push_back(mo);
		}
	} else {
		throw std::runtime_error("No MNO defined! At least one MNO should be defined for a valid simulation");
	}
	if (numMNO > 2)
		throw std::runtime_error("Maximum 2 MNOs are supported!");
	return (result);
}
