#include "IntegralArray.hpp"

using namespace std;

// FIX THIS MESS

STIntegralArray::STIntegralArray(vector<int>* list){
    this->input = list;
    this->integral = new vector<long>(list->size());
    iota(this->integral->begin(), this->integral->end(), 0);
}
vector<long>* STIntegralArray::operator()(){
    this->integral->insert(this->integral->begin(), *(this->input->begin()));
    for (size_t i = 1; i < this->input->size(); i++){
        this->integral->insert(this->integral->begin() + i,(*(this->integral->begin() + i - 1) + *(this->input->begin() + i)));
    }
    return this->integral;
}

MTIntegralArray::MTIntegralArray(vector<int>* list, int n){
    this->input = list;
    this->integral = new vector<long>(list->size());
    iota(this->integral->begin(), this->integral->end(), 0);
    this->n = n;
    this->step = list->size() / n;
    critical_elements = new long[n - 1];
}
void MTIntegralArray::partial_count(int s, int e, int thrd_idx){
    (*integral)[s] = (*input)[s];
    for (size_t i = s + 1; i < e; i++){
        (*integral)[i] = (*integral)[i - 1] + (*input)[i];
    }
}
void MTIntegralArray::merge_partial(int e, int inc, int t_idx){
    int e2 = e + step;
    if (t_idx == this->n - 2)
        e2 = integral->size();
    for (size_t i = e; i < e2; i++){
        (*integral)[i] += inc;
    }
    
}

vector<long>* MTIntegralArray::operator()(){
    thread** thrds = new thread*[this->n];
    for (size_t i = 0; i < this->n; i++){
        int s = i * this->step ;
        int e = (i + 1) * this->step;
        if (i == this->n - 1)
            e = this->integral->size();
        *(thrds + i) = new thread(&MTIntegralArray::partial_count, this, s, e, i);
    }
    for (size_t i = 0; i < this->n; i++){
        thrds[i]->join();
    }
    delete[] thrds;
    for (size_t i = 0; i < n; i++){
        int e = (i + 1) * this->step;
        if (i == this->n - 1)
            continue;
        if(i < this->n - 1){
            critical_elements[i] = (*integral)[e - 1];
        if(i > 0)
            critical_elements[i] += critical_elements[i - 1];
        }
    }
    thrds = new thread*[this->n];
    for (size_t i = 0; i < this->n; i++){
        int e = (i + 1) * this->step;
        if (i == this->n - 1)
            e = integral->size();
        long inc = critical_elements[i];
        *(thrds + i) = new thread(&MTIntegralArray::merge_partial, this, e, inc, i);;
    }
    for (size_t i = 0; i < this->n; i++){
        thrds[i]->join();
    }
    delete[] thrds;
    return this->integral;
}