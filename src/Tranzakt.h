#ifndef TRANZAKT_H
#define TRANZAKT_H

#include <iostream>
#include <stdexcept>

#define None 0

class Tranzakt{
	protected:
		unsigned _id;
		double _time; // TIME LOCATION
		unsigned _tranzakt_type; //1,2, ... MAX_COUNT_tranzakt_type
	public:
		Tranzakt() : _id(None), _time(None), _tranzakt_type(None) {}
		Tranzakt(unsigned id, double time, unsigned type);
		unsigned id();
		double time();
		void set_time(double n);
		unsigned type();

		friend std::ostream& operator<<(std::ostream& output, const Tranzakt& tranzakt);
		friend std::ofstream& operator<<(std::ofstream& output, const Tranzakt& tranzakt);
};

std::ostream& operator<<(std::ostream& output, const Tranzakt& tranzakt){
	output<<(const char*)"Tranzakt type "<< tranzakt._tranzakt_type; 
	output<<(const char*)" with id="<<tranzakt._id;
	output<<(const char*)" located in "<<tranzakt._time<<(const char*)"sec";
	return output;
}

std::ofstream& operator<<(std::ofstream& output, const Tranzakt& tranzakt){
	output<<(const char*)"Tranzakt type "<< tranzakt._tranzakt_type; 
	output<<(const char*)" with id="<<tranzakt._id;
	output<<(const char*)" located in "<<tranzakt._time<<(const char*)"sec";
	return output;
}

Tranzakt::Tranzakt(unsigned id, double time, unsigned type) : _id(id), _time(time), _tranzakt_type(type) {
	if(time<0){
		throw std::logic_error("time cannot be negative");
	}
}

unsigned Tranzakt::id(){
	return _id;
}

double Tranzakt::time(){
	return _time;
}

void Tranzakt::set_time(double new_time){
	if(new_time<0){
		throw std::logic_error("time cannot be negative");
	}
	_time=new_time;
}

unsigned Tranzakt::type(){
	return _tranzakt_type;
}

#endif