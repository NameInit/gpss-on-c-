#ifndef WORKER_H
#define WORKER_H

#include "Tranzakt.h"
#include <random>

#define None 0

class Worker{
	private:
		unsigned __num_worker;
		unsigned __type;
		int* __mean;
		Tranzakt *__processed_tranzakt;
		double __all_time_working;
		double __last_time_logged;
	public:
		Worker() : __num_worker(None),\
		__type(None), __mean(nullptr), __processed_tranzakt(nullptr), \
		__all_time_working(None), __last_time_logged(None) {}
		Worker(unsigned num_worker, unsigned type, int* mean) : __num_worker(num_worker),\
		__type(type), __mean(mean), __processed_tranzakt(nullptr), \
		__all_time_working(None), __last_time_logged(None) {}
		
		Worker& operator=(const Worker& other);

		double time_tranzakt();
		bool is_free();
		Tranzakt* get_ptr_tranzakt();
		Tranzakt* pop_tranzakt();
		void set_tranzakt(Tranzakt* tranzakt);
		void start_working();
		double get_all_time_working();
		double get_coefficient_load(double end_time);

		friend std::ostream& operator<<(std::ostream& output, const Worker& other);
		friend std::ofstream& operator<<(std::ofstream& output, const Worker& other);
};

Worker& Worker::operator=(const Worker& other){
	if(__processed_tranzakt!=other.__processed_tranzakt) {
		delete __processed_tranzakt;
		__processed_tranzakt=new Tranzakt(other.__processed_tranzakt->id(),other.__processed_tranzakt->time(),other.__processed_tranzakt->type());
	}
	__num_worker=other.__num_worker;
	__type=other.__type;
	__mean=other.__mean;
	__all_time_working=other.__all_time_working;
	return *this;
}

double Worker::get_coefficient_load(double end_time){
	if(__processed_tranzakt!=nullptr)
		__all_time_working+=end_time-__last_time_logged;
	return __all_time_working/end_time;
}

std::ostream& operator<<(std::ostream& output, const Worker& other){
	output<<(const char*)"Worker type: "<<other.__type<<(const char*)" Num: "<<other.__num_worker;
	if(other.__processed_tranzakt) output<<' '<<*other.__processed_tranzakt;
	else output << (const char*)" Tranzakt: <void>";
	return output;
}

std::ofstream& operator<<(std::ofstream& output, const Worker& other){
	output<<(const char*)"Worker type: "<<other.__type<<(const char*)" Num: "<<other.__num_worker;
	if(other.__processed_tranzakt) output<<*other.__processed_tranzakt;
	else output << (const char*)"Tranzakt: <void>";
	return output;
}

double Worker::time_tranzakt(){
	return __processed_tranzakt->time();
}

double Worker::get_all_time_working(){
	return __all_time_working;
}

Tranzakt* Worker::pop_tranzakt(){
	Tranzakt* temp=__processed_tranzakt;
	__all_time_working+=__processed_tranzakt->time()-__last_time_logged;
	__processed_tranzakt=nullptr;
	return temp;
}

bool Worker::is_free(){
	return __processed_tranzakt==nullptr;
}

void Worker::set_tranzakt(Tranzakt* tranzakt){
	if(tranzakt==nullptr){
		throw std::logic_error("the transaction was not transferred");
	}
	__processed_tranzakt=tranzakt;
	__last_time_logged=__processed_tranzakt->time();
}

Tranzakt* Worker::get_ptr_tranzakt(){
	return __processed_tranzakt;
}

void Worker::start_working(){
	if(__mean[__processed_tranzakt->type()-1]<0){
		throw std::logic_error("this worker cannot process this trazakt");
	}
	std::random_device _rd;
	std::mt19937 _generator;
	std::exponential_distribution<double> distr(1.0/__mean[__processed_tranzakt->type()-1]);
	double working_time=distr(_generator);
	__processed_tranzakt->set_time(__processed_tranzakt->time()+working_time);
}

#endif