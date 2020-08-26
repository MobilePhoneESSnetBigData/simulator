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
 * IDGenerator.h
 *
 *  Created on: Apr 11, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef IDGENERATOR_H_
#define IDGENERATOR_H_

/**
 * This singleton class is used to generate unique identifiers for all agents in the simulation. The identifiers are unsigned long integers.
 */
class IDGenerator {
public:
	/**
	 * Returns an instance of this class.
	 * @return an instance of this class.
	 */
	static IDGenerator* instance() {
		if (!m_instance) {
			m_instance = new IDGenerator;
		}
		return (m_instance);
	}

	/**
	 * Generates the next unique identifier.
	 * @return a unique identifier.
	 */
	unsigned long next() {
		return (m_id++);
	}

private:
	IDGenerator() :
			m_id(0) {
	}
	static IDGenerator* m_instance;
	unsigned long m_id;

};

#endif /* IDGENERATOR_H_ */
