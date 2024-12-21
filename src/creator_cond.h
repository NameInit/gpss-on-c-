#ifndef CREATOR_COND_H
#define CREATOR_COND_H

#include <stdexcept>
#include <iostream>

#define TINE_IMITATION 500
#define SIZE_ROW_RGB 3
#define SIZE_COL_RGB 3
#define SIZE_BORN_TRAN 3
#define SIZE_ROW_PROC_TRAN 2
#define SIZE_COL_PROC_TRAN 3
#define COUNT_TR_TYPE 3
#define COUNT_TYPE_WORKER 2
#define COUNT_WORKER1 1
#define COUNT_WORKER2 1

int** create_rgb();
int* create_mean_born_tranzakt(int** rgb);
int** create_mean_processing_tranzakt(int** rgb);
int* create_mass_count_workers();
void show_mass_conditional(int** rgb, int* mean_born_tranzakt, int** mean_processing_tranzakt, int* create_mass_count_workers);
void clear_mass_conditional(int** rgb, int* mean_born_tranzakt, int** mean_processing_tranzakt, int* create_mass_count_workers);

#endif