#ifndef GENERATOR_H
#define GENERATOR_H

#include "Tranzakt.h"
#include <random>

#define None 0

class Generator : public Tranzakt{
	private:
		int __mean_born_tranzakt;
		double __last_time_born;
		std::random_device __rd;
		std::mt19937 __generator;
	public:
		Generator() : __mean_born_tranzakt(None), __last_time_born(None), __rd(), __generator(__rd()) {}
		Generator(int mean_born_tranzakt) : __mean_born_tranzakt(mean_born_tranzakt), \
		__last_time_born(None), __rd(), __generator(__rd()) {}
		void set_generator(int mean_born_tranzakt, unsigned tranzakt_type);
		void update_tranzakt(unsigned new_id,bool shift);
		void reset_tranzakt();
		bool is_free();
		Tranzakt* pop_tranzakt();

		friend std::ostream& operator<<(std::ostream& output, const Generator& other);
		friend std::ofstream& operator<<(std::ofstream& output, const Generator& other);
};

void Generator::set_generator(int mean_born_tranzakt, unsigned tranzakt_type){
	__mean_born_tranzakt=mean_born_tranzakt;
	this->_tranzakt_type=tranzakt_type;
}

bool Generator::is_free(){
	return this->_id==None && this->_time==None;
}

void Generator::reset_tranzakt(){
	this->_id=None;
	this->_time=None;
}

std::ostream& operator<<(std::ostream& output, const Generator& other){
	output<<Tranzakt(other._id,other._time,other._tranzakt_type);
	return output;
}

std::ofstream& operator<<(std::ofstream& output, const Generator& other){
	output<<Tranzakt(other._id,other._time,other._tranzakt_type);
	return output;
}

Tranzakt* Generator::pop_tranzakt(){
	Tranzakt* temp=new Tranzakt(this->_id, this->_time, this->_tranzakt_type);
	reset_tranzakt();
	return temp;
}

void Generator::update_tranzakt(unsigned new_id,bool shift=true){
	this->_id=new_id;
	std::exponential_distribution<double> distr(1.0/__mean_born_tranzakt);
	this->_time=this->__last_time_born*shift+distr(__generator);
	__last_time_born=this->_time;
}

#endif