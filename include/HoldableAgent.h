/*
 * Data simulator for mobile phone network events
 *
 * HoldableAgent.h
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */

#ifndef HOLDABLEAGENT_H_
#define HOLDABLEAGENT_H_

#include <MovableAgent.h>

/*
 *
 */
class HoldableAgent: public MovableAgent {
	public:
		HoldableAgent(Map* m, long id, Point* initPosition, long idHolder);
		virtual ~HoldableAgent();
		long getIdHolder() const;
		void setIdHolder(long idHolder);

		virtual string getName() { return "HoldableAgent";}

	private:
		long m_idHolder;
};

#endif /* HOLDABLEAGENT_H_ */
