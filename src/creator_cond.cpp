#include "creator_cond.h"

int** create_rgb(){
    int** rgb = new int*[SIZE_ROW_RGB];
    for(int i=0; i<SIZE_ROW_RGB ;i++)
        rgb[i]=new int[SIZE_COL_RGB];
    // [0][0] - R1, [0][1] - G1, [0][2] - B1
    // [1][0] - R2, [1][1] - G2, [1][2] - B2
    // [2][0] - R3, [2][1] - G3, [2][2] - B3

    #ifdef INIT_CONDITIONAL
    rgb[0][0] = 7; rgb[0][1] = 9; rgb[0][2] = 7;     // R1, G1, B1
    rgb[1][0] = 9; rgb[1][1] = 11; rgb[1][2] = 9;    // R2, G2, B2
    rgb[2][0] = 11; rgb[2][1] = 7; rgb[2][2] = 5;    // R3, G3, B3
    #else
    for(int i=0; i<SIZE_ROW_RGB; i++){
        for(int j=0; j<SIZE_COL_RGB; j++){
            const char* RGB="RGB";
            std::cout<<(const char*)"Write "<<RGB[j]<<i+1<<(const char*)">>";
            std::cin>>rgb[i][j];
            if(!std::cin.good()||rgb[i][j]<=0){
                throw std::invalid_argument("error input");
            }
        }
    }
    #endif
    return rgb;
}

int* create_mean_born_tranzakt(int** rgb){
    int* mean_born_tranzakt=new int[SIZE_BORN_TRAN];
    for(int i=0; i<SIZE_BORN_TRAN; i++)
        mean_born_tranzakt[i]=rgb[0][i];
    return mean_born_tranzakt;
}

int** create_mean_processing_tranzakt(int** rgb){
    int **mean_processing_tranzakt = new int*[SIZE_ROW_PROC_TRAN];
    for(int i=0; i<SIZE_ROW_PROC_TRAN; i++)
        mean_processing_tranzakt[i]=new int[SIZE_COL_PROC_TRAN];
    //WORKER 1
    mean_processing_tranzakt[0][0]=rgb[0][0]+rgb[0][1]+rgb[0][2]; //TRANZAKT TYPE 1
    mean_processing_tranzakt[0][1]=-1; //TRANZAKT TYPE 2 don't processing
    mean_processing_tranzakt[0][2]=rgb[2][0]+rgb[2][1]+rgb[2][2]+rgb[0][1]; //TRANZAKT TYPE 3
    //WORKER 2
    mean_processing_tranzakt[1][0]=-1; //TRANZAKT TYPE 1 don't processing
    mean_processing_tranzakt[1][1]=rgb[1][0]+rgb[1][1]+rgb[1][2]; //TRANZAKT TYPE 2
    mean_processing_tranzakt[1][2]=rgb[2][0]+rgb[2][1]+rgb[2][2]+rgb[0][2]; //TRANZAKT TYPE 3
    return mean_processing_tranzakt;
}

int* create_mass_count_workers(){
    int* mass_count_workers=new int[COUNT_TYPE_WORKER];
    mass_count_workers[0]=COUNT_WORKER1;
    mass_count_workers[1]=COUNT_WORKER2;
    return mass_count_workers;
}

void show_mass_conditional(int** rgb, int* mean_born_tranzakt, int** mean_processing_tranzakt, int* mass_count_workers){
    std::cout<<(const char*)"-----MASS_RGB-----"<<std::endl;
    for(int i=0;i<SIZE_ROW_RGB;i++){
        for(int j=0;j<SIZE_COL_RGB;j++){
            std::cout.width(3);
            std::cout << rgb[i][j];
        }
        std::cout<<std::endl;
    }

    std::cout<<std::endl<<(const char*)"-----MASS_MEAN_BORN_TRANZAKT-----"<<std::endl;
    for(int i=0;i<SIZE_BORN_TRAN;i++){
        std::cout.width(3);
        std::cout<<mean_born_tranzakt[i];
    }
    std::cout<<std::endl;

    std::cout<<std::endl<<(const char*)"-----MASS_MEAN_PROCESSING_TRANZAKT-----"<<std::endl;
    for(int i=0; i<SIZE_ROW_PROC_TRAN; i++){
        for(int j=0;j<SIZE_COL_PROC_TRAN;j++){
            std::cout.width(3);
            std::cout<<mean_processing_tranzakt[i][j];
        }
        std::cout<<std::endl;
    }

    std::cout<<std::endl<<(const char*)"-----COUNT_WORKERS-----"<<std::endl;
    for(int i=0; i<COUNT_TYPE_WORKER; i++)
        std::cout<<(const char*)"Worker"<<i+1<<(const char*)": "<<mass_count_workers[i]<<std::endl;
}

void clear_mass_conditional(int** rgb, int* mean_born_tranzakt, int** mean_processing_tranzakt, int* mass_count_workers){
    for(int i=0; i<SIZE_ROW_RGB; i++) delete [] rgb[i];
    delete [] rgb;

    delete [] mean_born_tranzakt;

    for(int i=0; i<SIZE_ROW_PROC_TRAN; i++) delete [] mean_processing_tranzakt[i];
    delete [] mean_processing_tranzakt;

    delete [] mass_count_workers;
}