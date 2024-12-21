#include <iostream>
#include <fstream>
#include "creator_cond.h"
#include "Generator.h"
#include "List.h"
#include "Tranzakt.h"
#include "Worker.h"
#include "Simulation.h"


int main(){
	std::ofstream logger;
    logger.open("logger");
    if(!logger.is_open()){
        throw std::logic_error("file is not open");
    }
    int **rgb=create_rgb();
    int *mean_born_tranzakt=create_mean_born_tranzakt(rgb); 				//size: COUNT_TYPE_TRANZAKT
    int **mean_processing_tranzakt=create_mean_processing_tranzakt(rgb); 	//size: COUNT_WORKERS x COUNT_TYPE_TRANZAKT
	int *mass_count_workers=create_mass_count_workers(); 					//size: COUNT_WORKERS
	unsigned count_tranzakt_type=COUNT_TR_TYPE;
	double time_imitation=TINE_IMITATION;
	
	Simulation thread1(mean_born_tranzakt, mean_processing_tranzakt, count_tranzakt_type, mass_count_workers);
	thread1.start_simulation(&logger, time_imitation);

	clear_mass_conditional(rgb,mean_born_tranzakt,mean_processing_tranzakt,mass_count_workers);
	return 0;
}