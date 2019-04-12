/*
 * Sdepaths.cpp
 *
 *  Created on: Dec 30, 2018
 *      Author: oliv
 */

#include "Sdepaths.h"
#include <random>
using namespace std;
using namespace arma;

namespace vSpace {


Sdepaths::Sdepaths(const realSpace& T,const double & S0,
		std::function<double  (const double & t, const arma::vec & Path,const int & end)> &a,
		std::function<double  (const double & t, const arma::vec & Path,const int & end)> &b,
		const int& N):T(T),a(a),b(b),N(N),proba(),logprice()
{
	Paths = vector<arma::vec>(N,vec(T.getNx()+1));
//	vector<mt19937> generators;
//	generators.reserve(N);
	mt19937_64 g;


	for(int i = 0 ; i != N ; ++i){
		Paths[i](0) = S0; /* Intialize */

	}

	std::normal_distribution<double> Z;
	for(int i = 0 ; i != N ; ++i){
		for(int j = 1 ; j != T.getNx()+1 ; ++j){
			Paths[i](j) = Paths[i](j-1) + a(T(j),Paths[i],j-1)*T.getHx() + b(T(j),Paths[i],j-1)*sqrt(T.getHx())*Z(g);
//			Paths[i](j) = (generators[i])();

		}
	}



}

Sdepaths::~Sdepaths() {
	// TODO Auto-generated destructor stub
}

const arma::vec& Sdepaths::getPath(const int& i) const {
	return Paths[i];
}

double Sdepaths::E(const double& t,
		std::function<double (const double&)> (&f) ) const {
	double res{0};
	int j = round( (t-T.getXi()) /T.getHx() );

	for(int i = 0 ; i != N ; ++i){
		res += f( Paths[i](j) );
	}
	return res/N;
}

//double Sdepaths::V(const double & t) const {
//	return this->E(t,square)- this->E(t);
//}

vfun Sdepaths::pdf(const double& t, int n) {
//	Find the max
	double maxi{0};

	int j = round( (t-T.getXi()) /T.getHx() );

	double mini{(Paths[0])(j)};
	double temp{0};
	for(int i = 0; i != N ; ++i){
		temp = (Paths[i])(j) ;
		maxi = temp < maxi ? maxi:temp;
		mini = temp > mini ? mini:temp;
	}
	double semirange = max(abs(log(mini)),abs(log(maxi)));
	if(semirange == 0 ){
		semirange = 0.1;
	}

	proba.reshape(n+1,1);


	double hx = 2*semirange/n;


	int c{0};
	for(int i = 0 ; i != N ; ++i){
		c = round( ( log( (Paths[i])(j) ) + semirange) / hx ) ;
		proba(c)++;
	}
	proba = proba/10000;
	return vfun(realSpace(-semirange,semirange,n),proba);
}

const arma::mat Sdepaths::getPathsMat() const {
	int p = T.getNx()+1;
	mat Paths_mat(N,p);
	for(int i = 0 ; i != N ; ++i){
		for(int j = 0 ; j != p ; ++j){
			Paths_mat(i,j) = Paths[i](j);
		}
	}
	return Paths_mat;
}

} /* namespace vSpace */
