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
 * RandomNumberGenerator.h
 *
 *  Created on: Apr 18, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef RANDOMNUMBERGENERATOR_H_
#define RANDOMNUMBERGENERATOR_H_

#include <algorithm>
#include <random>

using namespace std;

/**
 * Utility singleton class to generate random numbers according to different distributions.
 */
class RandomNumberGenerator {
public:
	/**
	 * Returns an instance of this class.
	 * @return n instance of this class.
	 */
	static RandomNumberGenerator* instance() {
		if (m_instance == nullptr) {
			m_instance = new RandomNumberGenerator();
		}
		return m_instance;
	}

	/**
	 * Returns an instance of this class and also sets the seed of the random number generator.
	 * @return n instance of this class.
	 */
	static RandomNumberGenerator* instance(unsigned seed) {
		if (m_instance == nullptr) {
			m_instance = new RandomNumberGenerator(seed);
		}
		return m_instance;
	}

	/**
	 * Generates n random numbers with a normal distribution. Half of them are N(m1,sd1), the other half N(m2,sd2).
	 * @param m1 the mean of the first normal distribution.
	 * @param sd1 the standard deviation of the first normal distribution.
	 * @param m2 the mean of the second normal distribution.
	 * @param sd2 the standard deviation of the second normal distribution.
	 * @param n the total number of values to be generated.
	 * @return an array with random numbers according to two normal distributions.
	 */
	double* generateNormal2Double(const double m1, const double sd1, const double m2, const double sd2, int n);

	/**
	 * Generates a random value, normally distributed with mean m and standard distribution sd
	 * @param m the mean of the normal distribution.
	 * @param sd the standard deviation of the normal distribution.
	 * @return a random value, normally distributed with mean m and standard distribution sd.
	 */
	double generateNormalDouble(const double m, const double sd);

	/**
	 * Generates an array with n double values normally distributed with mean m and standard deviation sd.
	 * @param m the mean of the normal distribution.
	 * @param sd the standard deviation of the normal distribution.
	 * @param n the number of values to be generated.
	 * @return an array with n double values normally distributed with mean m and standard deviation sd.
	 */
	double* generateNormalDouble(const double m, const double sd, const int n);

	/**
	 * Generates n double values from a truncated normal distribution. All values will be in [a, b].
	 * @param a the inferior limit of the truncated normal distribution.
	 * @param b the superior limit of the truncated normal distribution.
	 * @param m the mean of the normal distribution.
	 * @param sd the standard deviation of the normal distribution.
	 * @param n the number of values to be generated.
	 * @return an array with n double values from a truncated normal distribution.
	 */
	double* generateTruncatedNormalDouble(const double a, const double b, const double m, const double sd, const unsigned long n);

	/**
	 * Generates n int values from a truncated normal distribution. All values will be in [a, b].
	 * @param a the inferior limit of the truncated normal distribution.
	 * @param b the superior limit of the truncated normal distribution.
	 * @param m the mean of the normal distribution.
	 * @param sd the standard deviation of the normal distribution.
	 * @param n the number of values to be generated.
	 * @return an array with n double values from a truncated normal distribution.
	 */
	int* generateTruncatedNormalInt(const int a, const int b, const double m, const double sd, const unsigned long n);

	/**
	 * Generates a random double value from a uniform distribution which lies inside [min, max].
	 * @param min the lower limit of the value.
	 * @param max the upper limit of the value.
	 * @return a double value, uniformly distributed in [min, max].
	 */
	double generateUniformDouble(const double min, const double max);

	/**
	 * Generates n uniform distributed random values which lie inside [min, max].
	 * @param min the lower limit of the values.
	 * @param max the upper limit of the values.
	 * @param n the number of values to be generated.
	 * @return n array with n double values from a uniform distribution.
	 */
	double* generateUniformDouble(const double min, const double max, const int n);


	/**
	 * Generates a random value distributed according to an exponential distribution with parameter lambda.
	 * @param lambda the parameter of the exponential distribution.
	 * @return a random value distributed according to an exponential distribution with parameter lambda.
	 */
	double generateExponentialDouble(const double lambda);

	/**
	 * Generates a random int value from a uniform distribution which lies inside [min, max].
	 * @param min the lower limit of the value.
	 * @param max the upper limit of the value.
	 * @return an int value, uniformly distributed in [min, max].
	 */
	int generateUniformInt(const int min, const int max);

	/**
	 * Generates n uniform distributed random values which lie inside [min, max].
	 * @param min the lower limit of the values.
	 * @param max the upper limit of the values.
	 * @param n the number of values to be generated.
	 * @return an array with n int values from a uniform distribution.
	 */
	int* generateUniformInt(const int min, const int max, const int n);

	/**
	 * Generates a int random value from a binomial distribution inside the interval [0, max].
	 * @param max the upper limit of the value.
	 * @param p the parameter of the binomial distribution.
	 * @return a int random value from a binomial distribution inside the interval [0, max].
	 */
	int generateBinomialInt(const int max, const double p);

	/**
	 * Generates an int random value from a Bernoulli distribution.
	 * @param p the parameter of the Bernoulli distribution.
	 * @return an int random value from a Bernoulii distribution.
	 */
	int generateBernoulliInt(const double p);

	/**
	 * Generates n int random values from a binomial distribution inside the interval [0, max].
	 * @param max the upper limit of the value.
	 * @param p the parameter of the binomial distribution.
	 * @param n the number of values to be generated.
	 * @return array with n int values from a binomial distribution.
	 */
	int* generateBinomialInt(const int max, const double p, const int n);

	/**
	 * Generates n int random values from a Bernoulli distribution.
	 * @param p the parameter of the Bernoulli distribution.
	 * @param n the number of values to be generated.
	 * @return array with n int values from a Bernoulli distribution.
	 */
	int* generateBernoulliInt(const double p, const int n);

	/**
	 * Generates n double random values from a Levy distribution
	 * @param mu the displacement of the Levy distribution
	 * @param c the scale parameter of the Levy distribution
	 * @param n the number of values to be generated
	 * @return an array with n double values from a Levy distribution
	 */
	double* generateLevy(const double mu, const double c, const int n);

	/**
	 * Generates a double random value from a Levy distribution
	 * @param mu the displacement of the Levy distribution
	 * @param c the scale parameter of the Levy distribution
	 * @return an array with n double values from a Levy distribution
	 */
	double generateLevy(const double mu, const double c);
	/**
	 * The value of the PDF of the normal distribution for x.
	 * @param x the value for which we need the PDF.
	 * @param m the mean of the normal distribution.
	 * @param s the standard deviation of the normal distribution.
	 * @return The value of the PDF of the normal distribution for x.
	 */
	double normal_pdf(double x, double m, double s);


	/**
	 * Sets the seed of the random number generator.
	 * @param seed
	 */
	void setSeed(unsigned seed);

private:
	RandomNumberGenerator();
	RandomNumberGenerator(unsigned seed);
	RandomNumberGenerator(const RandomNumberGenerator&);
	RandomNumberGenerator& operator=(const RandomNumberGenerator&);

	static RandomNumberGenerator* m_instance;
	uniform_int_distribution<int> m_unif_int_distribution;
	uniform_real_distribution<double> m_unif_double_distribution;
	normal_distribution<double> m_normal_double_distribution;
	exponential_distribution<double> m_exponential_double_distribution;
	binomial_distribution<int> m_binomial_distribution;
	bernoulli_distribution m_bernoulli_distribution;

	std::mt19937 m_generator;

};

#endif /* RANDOMNUMBERGENERATOR_H_ */
