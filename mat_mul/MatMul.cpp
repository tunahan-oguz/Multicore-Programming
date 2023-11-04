#include "MatMul.hpp"

STMatMul::STMatMul(float** A, float** B, int n, int m, int l){
    this->A = A;
    this->B = B; // B could be hold as B^T to optimize cache usage
    this->n = n;
    this->m = m;
    this->l = l;
    this->C = new float*[n];
    for (size_t i = 0; i < n; i++){
        C[i] = new float[l];
        for (size_t j = 0; j < l; j++){
            C[i][j] = 0;
        }
    }
    
}

float** STMatMul::operator()(){
    for (size_t i = 0; i < n; i++){
        for (size_t j = 0; j < l; j++){
            for (size_t k = 0; k < m; k++){
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

MTMatMul::MTMatMul(float** A, float** B, int n, int m, int l, int n_threads){
    this->n_threads = n_threads;
    this->A = A;
    this->B = B;
    this->n = n;
    this->m = m;
    this->l = l;
    this->C = new float*[n];
    for (size_t i = 0; i < n; i++){
        C[i] = new float[l];
        for (size_t j = 0; j < l; j++){
            C[i][j] = 0;
        }
    }
    
}
void MTMatMul::partial_mul(int t_idx){
    int step = this->n / n_threads;
    int row_start = t_idx * step;
    int row_end = row_start + step;
    if(t_idx == n_threads - 1)
        row_end = n;
    
    for (size_t i = row_start; i < row_end; i++){
        for (size_t j = 0; j < l; j++){
            for (size_t k = 0; k < m; k++){
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}


float** MTMatMul::operator()(){
    thread** threads = new thread*[n_threads];
    for (size_t i = 0; i < n_threads; i++){
        threads[i] = new thread(&MTMatMul::partial_mul, this, i);    
    }
    
    for (size_t i = 0; i < n_threads; i++){
        threads[i]->join();
    }
    return C;
}