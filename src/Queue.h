#ifndef QUEUE_H
#define QUEUE_H

#include "List.h"
#include "Tranzakt.h"
#include "Workers.h"
#include <fstream>

#define None 0

class Queue{
    private:
    	List<Tranzakt*> __queue;
        double __last_time_entry_or_exit;
        double __average_queue_length; //sum(Q(t))
        unsigned __max_len_queue;
    public:
        unsigned size() { return __queue.get_size(); }
        void show(std::ofstream* logger){
            if(__queue.get_size()!=0){
                for(int i=0; i<__queue.get_size(); i++)
                    (*logger)<<(const char*)"Tranzakt with id=" <<__queue[i]->id()
                    <<(const char*)" type="<<__queue[i]->type() << (const char*)" in queue position="<<i+1<<std::endl;
                (*logger)<<std::endl;
            }
            else
                (*logger)<<"<void>"<<std::endl<<std::endl;
        }
        Queue() : __queue(), __last_time_entry_or_exit(None), __average_queue_length(None), __max_len_queue(None) {}
        void push(Tranzakt* tranzakt);
        Tranzakt* pop_tranzakt(int n);
        Tranzakt* operator[](int n);
        unsigned get_max_len();
        void promote_tranzakts_to_workers(Workers& workers);
        void shift_time_all_tranzakts(double end_time);
        double get_average_queue_length(double time);
        friend std::ostream& operator<<(std::ostream& output, const Queue& other);
        friend std::ofstream& operator<<(std::ofstream& output, const Queue& other);
};

std::ofstream& operator<<(std::ofstream& output, const Queue& other){
    output<<other.__queue;
    return output;
}

unsigned Queue::get_max_len(){
    return __max_len_queue;
}

double Queue::get_average_queue_length(double end_time){
    __max_len_queue=(__max_len_queue<__queue.get_size()) ? __queue.get_size() : __max_len_queue;
    __average_queue_length+=(end_time-__last_time_entry_or_exit)*__queue.get_size();
    return __average_queue_length/end_time; 
}

void Queue::shift_time_all_tranzakts(double time){
    __max_len_queue=(__max_len_queue<__queue.get_size()) ? __queue.get_size() : __max_len_queue;
    __average_queue_length+=(time-__last_time_entry_or_exit)*((double)__queue.get_size());
    for(int i=0; i<__queue.get_size(); i++) __queue[i]->set_time(time);
}

void Queue::promote_tranzakts_to_workers(Workers& workers){
   while(workers.any_worker_free()&&__queue.get_size()!=0){
        Tranzakt* temp=__queue.pop(0);
        __last_time_entry_or_exit=temp->time();
        workers.push_tranzakt(temp);
    }
}

Tranzakt* Queue::operator[](int n){
    if(n<0 || n>=__queue.get_size()){
        throw std::logic_error("index queue out of range");
    }
    return __queue[n];
}

std::ostream& operator<<(std::ostream& output, const Queue& other){
    output<<other.__queue;
    return output;
}

void Queue::push(Tranzakt* tranzakt){
    __last_time_entry_or_exit=tranzakt->time();
    __queue.push_tail(tranzakt);
}

Tranzakt* Queue::pop_tranzakt(int n=0){
    if(n<0 || n>=__queue.get_size()){
        throw std::logic_error("index error");
    }
    Tranzakt* temp=__queue.pop(n);
    __last_time_entry_or_exit=temp->time();
    return temp;
}

#endif