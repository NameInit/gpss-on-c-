#ifndef WORKERS_H
#define WORKERS_H

#include "Worker.h"
#include "List.h"
#include "Destroyer.h"

#define None 0

class Workers{
	private:
		Worker* __workers;
		unsigned __type_workers;
		unsigned __count_workers;
		int* __mean_processing_tranzakt;
	public:
		Workers() : __workers(nullptr), __count_workers(0) {}
		Workers(int count_workers, int type, int* mean_processing_tranzakt);
		~Workers();
		
		Worker& operator[](int n);

		Worker* get_ptr_workers(){ return __workers; }

		void set_workers(unsigned count_workers, unsigned type_workers, int* mean_processing_tranzakt);

		unsigned count_busy_people();
		void promote_tranzakts_to_terminate(Destroer& terminate, double time);
		double get_min_time();
		unsigned get_count_free_workers();
		unsigned get_count_workers() { return __count_workers; }
		void push_tranzakt(Tranzakt* tranzakt);
		bool any_worker_free();
		bool any_worker_not_free();
		Tranzakt* pop_tranzakt(double time);
		friend std::ostream& operator<<(std::ostream& output, const Workers& other);
		friend std::ofstream& operator<<(std::ofstream& output, const Workers& other);
};

void Workers::set_workers(unsigned count_workers, unsigned type_workers, int* mean_processing_tranzakt){
	__type_workers=type_workers;
	__count_workers=count_workers;
	__mean_processing_tranzakt=mean_processing_tranzakt;
	__workers=new Worker[count_workers];
	for(int i=0; i<count_workers; i++)
		__workers[i] = Worker(i+1,type_workers,mean_processing_tranzakt);
}

std::ostream& operator<<(std::ostream& output, const Workers& other){
	if(!other.__count_workers)
		output<<(const char*)"<void>";
	for(int i=0; i<other.__count_workers; i++)
		output<<other.__workers[i]<<std::endl;
	return output;
}

std::ofstream& operator<<(std::ofstream& output, const Workers& other){
	if(!other.__count_workers)
		output<<(const char*)"<void>";
	for(int i=0; i<other.__count_workers; i++)
		output<<other.__workers[i]<<std::endl;
	return output;
}

Worker& Workers::operator[](int n){
	if(n<0 || n>=__count_workers){
		std::logic_error("index workers out of range");
	}
	return __workers[n];
}

unsigned Workers::count_busy_people(){
	unsigned counter=0;
	for(int i=0; i<__count_workers; i++)
		counter+=!__workers[i].is_free();
	return counter;
}

bool Workers::any_worker_not_free(){
	for(int i=0; i<__count_workers; i++)
		if(!__workers[i].is_free())
			return true;
	return false;
}

Tranzakt* Workers::pop_tranzakt(double time=None){
	for(int i=0; i<__count_workers; i++)
		if(!__workers[i].is_free())
			if(__workers[i].time_tranzakt()<=time || time==None)
				return __workers[i].pop_tranzakt();
	return nullptr;
}

void Workers::promote_tranzakts_to_terminate(Destroer& terminate, double time){
	for(int i=0; i<__count_workers; i++)
		if(!__workers[i].is_free())
			if(__workers[i].time_tranzakt()<=time){
				Tranzakt* temp=__workers[i].pop_tranzakt();
				terminate.push(temp);
		}
}

double Workers::get_min_time(){
	double min_time=None;
	for(int i=0; i<__count_workers; i++)
		if(!__workers[i].is_free()&&min_time==None) min_time=__workers[i].time_tranzakt();
		else if(!__workers[i].is_free())
			if(__workers[i].time_tranzakt()<min_time) min_time=__workers[i].time_tranzakt();
	return min_time;
}

unsigned Workers::get_count_free_workers(){
	unsigned counter=0;
	for(int i=0; i<__count_workers; i++)
		counter+=__workers[i].is_free();
	return counter;
}

bool Workers::any_worker_free(){
	for(int i=0; i<__count_workers; i++){
		if(__workers[i].is_free())
			return true;
	}
	return false;
}

void Workers::push_tranzakt(Tranzakt* tranzakt){
	if(!any_worker_free()){
		throw std::logic_error("can not push, because there are no places");
	}
	for(int i=0; i<__count_workers; i++)
		if(__workers[i].is_free()){
			__workers[i].set_tranzakt(tranzakt);
			__workers[i].start_working();
			break;
		}
}

Workers::Workers(int count_workers, int type, int* mean_processing_tranzakt) : __type_workers(type),\
__count_workers(count_workers), __mean_processing_tranzakt(mean_processing_tranzakt){
	__workers=new Worker[count_workers];
	for(int i=0; i<count_workers; i++) __workers[i]=Worker(i+1,type,mean_processing_tranzakt);
}

Workers::~Workers(){
	delete __workers;
}

#endif