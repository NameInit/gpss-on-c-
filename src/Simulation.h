#ifndef SIMULATION_H
#define SIMULATION_H

#include "Tranzakt.h"
#include "Generator.h"
#include "Workers.h"
#include "Queue.h"
#include "Destroyer.h"

#include <cstring>

#define None 0

class Simulation{
	private:
		unsigned __id;
		Generator* __generate;
		unsigned __count_type_worker;
		unsigned __count_tranzakt_type;
		Workers* __all_workers;
		Queue* __all_queue;
		Destroer __terminate;

		double __GetNextTime();

		void __CheckGenerator(std::ofstream *logger, double current_time);
		void __CheckQueues(std::ofstream *logger);
		void __CheckWorkers(std::ofstream *logger, double  current_time);

		void __SHIFT_TIME_IN_ALL_QUEUE(double time);

		unsigned __LogicDistributionTranzakts(Tranzakt* tranzakt, Workers* workers, Queue* queues);

		void __ShowStatistic(std::ofstream *logger,double end_time);

		void __Generate();
		void __GenerateToQueue(double time);
		void __QueueToWorkers(double time);
		void __WorkersToTerminate(double time);
	public:
		Simulation(int *mean_born_tranzakt,int **mean_processing_tranzakt, unsigned count_tranzakt_type, int* count_workers);
		~Simulation();

		void start_simulation(std::ofstream *logger, double time_simulation);
};

void Simulation::start_simulation(std::ofstream *logger, double time_simulation){
	double current_time=0;
	while((current_time=__GetNextTime())<time_simulation){
		__SHIFT_TIME_IN_ALL_QUEUE(current_time);

		(*logger)<<(const char*)"<<<<<<<<<<<<<<<<<<<<<<<CEC>>>>>>>>>>>>>>>>>>>>>>>"<<std::endl;
		
		(*logger)<<(const char*)"Model time: "<<current_time<<std::endl;

		__CheckGenerator(logger, current_time);
		__CheckQueues(logger);
		__CheckWorkers(logger,current_time);

		(*logger)<<(const char*)"###################################################"<<std::endl<<std::endl;
		
		__Generate();

		__WorkersToTerminate(current_time);
		__GenerateToQueue(current_time);
		__QueueToWorkers(current_time);

		__Generate();

		(*logger)<<(const char*)"<<<<<<<<<<<<<<<<<<<<<<<FEC>>>>>>>>>>>>>>>>>>>>>>>"<<std::endl;

		(*logger)<<(const char*)"Model time: "<<current_time<<std::endl;

		__CheckGenerator(logger, current_time);
		__CheckQueues(logger);
		__CheckWorkers(logger,current_time);

		(*logger)<<(const char*)"###################################################"<<std::endl<<std::endl;
	}
	__ShowStatistic(logger,time_simulation);
}

void Simulation::__Generate(){
	for(int i=0; i<__count_tranzakt_type; i++)
		if(__generate[i].is_free())
			__generate[i].update_tranzakt(++__id);
}

void Simulation::__CheckGenerator(std::ofstream *logger, double current_time){
	(*logger)<<(const char*)"-----GENERATOR-----"<<std::endl;
	for(int i=0; i<__count_tranzakt_type; i++){
		if(__generate[i].is_free()) (*logger)<<(const char*)"<void>";
		else if(__generate[i].time()==current_time)
			(*logger)<<"Tranzakt with id="<<__generate[i].id()<<(const char*)
			" type="<<__generate[i].type()<<" created in time="<<__generate[i].time();
		else (*logger)<<"Tranzakt with id="<<__generate[i].id()<<(const char*)
			" type="<<__generate[i].type()<<" will be created in time="<<__generate[i].time();
		
		(*logger)<<std::endl;
	}
}

unsigned Simulation::__LogicDistributionTranzakts(Tranzakt* tranzakt, Workers* workers, Queue* queues){ //надо доделать, не переводится в double
	if(tranzakt->type()<=__count_type_worker)
		return tranzakt->type();
	
	unsigned optimal_type_worker=1;
	double min_count_circle=((double)workers[0].count_busy_people()+(double)queues[0].size())/(double)workers[0].get_count_workers();
	for(int i=1; i<__count_type_worker; i++){
		if(min_count_circle >(((double)workers[i].count_busy_people()+(double)queues[i].size())/(double)workers[i].get_count_workers())){
			min_count_circle=(((double)workers[i].count_busy_people()+(double)queues[i].size())/(double)workers[i].get_count_workers());
			optimal_type_worker=i+1;
		}
	}
	return optimal_type_worker;
}

void Simulation::__GenerateToQueue(double time){
	for(int i=0; i<__count_tranzakt_type; i++)
		if(__generate[i].time()<=time && !__generate[i].is_free()){
			Tranzakt* temp=__generate[i].pop_tranzakt();
			__generate[i].reset_tranzakt();
			unsigned type=__LogicDistributionTranzakts(temp,__all_workers,__all_queue);
			__all_queue[type-1].push(temp);
		}
}

void Simulation::__QueueToWorkers(double time){
	for(int i=0; i<__count_type_worker; i++){
		if(__all_workers[i].any_worker_free())
			__all_queue[i].promote_tranzakts_to_workers(__all_workers[i]);
	}
}

void Simulation::__WorkersToTerminate(double time){
	for(int i=0; i<__count_type_worker; i++){
		__all_workers[i].promote_tranzakts_to_terminate(__terminate,time);
	}
}

void Simulation::__CheckQueues(std::ofstream *logger){
	(*logger)<<std::endl<<(const char*)"-----QUEUES IN SIMULATION-----"<<std::endl;
	for(int i=0; i<__count_type_worker;i++){
		(*logger)<<(const char*)"QUEUE"<<i+1<<':'<<std::endl;
		__all_queue[i].show(logger);
	}
}

void Simulation::__CheckWorkers(std::ofstream *logger, double current_time){
	(*logger)<<std::endl<<(const char*)"-----WORKERS IN SIMULATION-----"<<std::endl;
	for(int i=0; i<__count_type_worker; i++){
		for(int j=0; j<__all_workers[i].get_count_workers(); j++)
			if(__all_workers[i][j].get_ptr_tranzakt()==nullptr) (*logger)<<__all_workers[i][j]<<std::endl;
			else if(__all_workers[i][j].time_tranzakt()==current_time)
				(*logger)<<"Worker type="<<i+1<<" num="<<j+1<<" finished Tranzakt with id="
				<<__all_workers[i][j].get_ptr_tranzakt()->id()<<" type="<<__all_workers[i][j].get_ptr_tranzakt()->type()
				<<" in "<<__all_workers[i][j].time_tranzakt()<<std::endl;
			else (*logger)<<"Worker type="<<i+1<<" num="<<j+1<<" didn't finish Tranzakt with id="
				<<__all_workers[i][j].get_ptr_tranzakt()->id()<<" type="<<__all_workers[i][j].get_ptr_tranzakt()->type()
				<<" in "<<__all_workers[i][j].time_tranzakt()<<std::endl;
		(*logger)<<std::endl;
	}

	(*logger)<<std::endl;
}

double Simulation::__GetNextTime(){
	double min_time=None;

	for(int i=0; i<__count_tranzakt_type; i++)
		if(!__generate[i].is_free())
			if(min_time==None || min_time>__generate[i].time())
				min_time=__generate[i].time();

	for(int i=0; i<__count_type_worker; i++)
		if((__all_workers[i].get_min_time()<min_time && __all_workers[i].any_worker_not_free()) || min_time==None)
			min_time=__all_workers[i].get_min_time();

	return min_time;
}

Simulation::Simulation(int *mean_born_tranzakt,int **mean_processing_tranzakt, unsigned count_tranzakt_type, int* count_workers) :
	__id(0), __count_type_worker(sizeof(count_workers)/sizeof(int)), \
	__count_tranzakt_type(count_tranzakt_type), __terminate(){
	
	__generate=new Generator[count_tranzakt_type];
	for(int i=0; i<__count_tranzakt_type; i++)
		__generate[i].set_generator(mean_born_tranzakt[i], i+1);


	__all_workers=new Workers[__count_type_worker];
	for(int i=0; i<__count_type_worker; i++)
		__all_workers[i].set_workers(count_workers[i],i+1,mean_processing_tranzakt[i]);
	
	__all_queue=new Queue[__count_type_worker];
}

Simulation::~Simulation(){
	for(int i=0; i<__count_type_worker; i++){
		while(__all_workers[i].any_worker_not_free()) delete __all_workers[i].pop_tranzakt();
		while(__all_queue[i].size()) delete __all_queue[i].pop_tranzakt();
	}
	delete [] __generate;
	delete [] __all_workers;
	delete [] __all_queue;
}

void Simulation::__ShowStatistic(std::ofstream *logger,double end_time){
	(*logger)<<(const char*)"------COFFICIENT LOAD WORKERS------"<<std::endl;
	for(int i=0; i<__count_type_worker; i++)
		for(int j=0; j<__all_workers[i].get_count_workers(); j++)
			(*logger)<<(const char*)"Coefficient load workers type="<<i+1<<(const char*)" worker num="<<j+1<<": "<<__all_workers[i][j].get_coefficient_load(end_time)<<std::endl;

	(*logger)<<std::endl<<(const char*)"------AVERAGE QUEUE LENGTH------"<<std::endl;
	for(int i=0; i<__count_type_worker; i++)
		(*logger)<<(const char*)"Average length queue type="<<i+1<<(const char*)": "<<__all_queue[i].get_average_queue_length(end_time)<<(const char*)" (MAX_LEN="<<__all_queue[i].get_max_len()<<')'<<std::endl;
}

void Simulation::__SHIFT_TIME_IN_ALL_QUEUE(double time){
	for(int i=0; i<__count_type_worker; i++)
		__all_queue[i].shift_time_all_tranzakts(time);
}

#endif