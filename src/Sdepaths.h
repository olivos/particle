/*
 * Sdepaths.h
 *
 *  Created on: Dec 30, 2018
 *      Author: oliv
 *
 *      N Paths for a SDE : dSt = a(t,X_t) dt + b(t,X_t) dBt where X_t = (S_u)_{u<=t}
 *
 */

#ifndef SDEPATHS_H_
#define SDEPATHS_H_

#include "topov2.h"


namespace vSpace {

class Sdepaths {
public:
	Sdepaths(const realSpace & T,const double & S0,
			double (&a) (const double & t, const arma::vec & Path,const int & end),
			double (&b) (const double & t, const arma::vec & Path,const int & end),
			const int & N);
	const arma::vec & getPath (const int & i) const;
	double E(const double & t, double (&f) (const double &) = Ide ) const;
	double V(const double & t) const;
	vfun pdf(const double & t, int n = 100) ;
	virtual ~Sdepaths();

	void setPaths(const std::vector<arma::vec>& paths) {
		Paths = paths;
	}

	const int getN() const {
		return N;
	}

private:
	const realSpace& T;
	double (&a) (const double & t, const arma::vec & Path,const int & end);
	double (&b) (const double & t, const arma::vec & Path,const int & end);
	const int N;
	std::vector<arma::vec> Paths;
	/* To spare computation time the two following are not implemented unless pdf is called !! */
	arma::vec proba;
	realSpace logprice;

	static double Ide(const double & x) {
		return x;
	}
	static double square(const double & x) {
		return pow(x,2);
	}


};

} /* namespace vSpace */

#endif /* SDEPATHS_H_ */
