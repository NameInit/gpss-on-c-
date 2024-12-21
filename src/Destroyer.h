#ifndef DESTROER_H
#define DESTROER_H

#include "List.h"
#include "Tranzakt.h"

class Destroer{
    private:
        List<Tranzakt*> __list;
    public:
        Destroer() : __list() {}
        ~Destroer();

        void push(Tranzakt* tranzakt);
        friend std::ostream& operator<<(std::ostream& output, const Destroer& other);
        friend std::ofstream& operator<<(std::ofstream& output, const Destroer& other);
};

std::ostream& operator<<(std::ostream& output, const Destroer& other){
    output<<(const char*)"-----TRANZAKTS REACHED THE END-----"<<std::endl;
    output<<other.__list;
    return output;
}

std::ofstream& operator<<(std::ofstream& output, const Destroer& other){
    output<<other.__list;
    return output;
}

void Destroer::push(Tranzakt* tranzakt){
    __list.push_tail(tranzakt);
    ;
}

Destroer::~Destroer(){
    while (__list.get_size()!=0){
        Tranzakt* temp=__list.pop(0);
        delete temp;
    }
}

#endif