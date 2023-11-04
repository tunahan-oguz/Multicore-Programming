#include "MatMul.hpp"
#include "../Report.hpp"

using namespace std;
bool verify(float** A, float** B, int x, int y);

int main(int argc, char const *argv[]){
    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
    int L = atoi(argv[3]);
    int THRDS = atoi(argv[4]);
    float ** m1 = new float*[N];
    float ** m2 = new float*[M];

    for (size_t i = 0; i < N; i++){
        m1[i] = new float[M];        
        for (size_t j = 0; j < M; j++){
            m1[i][j] = i * 2 + j;
        }
    }
    for (size_t i = 0; i < M; i++){
        m2[i] = new float[L];        
        for (size_t j = 0; j < L; j++){
            m2[i][j] = i * 2 + j;
        }
    }

    STMatMul* st = new STMatMul(m1, m2, N, M, L);
    float** st_out = (*st)();
    MTMatMul* mt = new MTMatMul(m1, m2, N, M, L, THRDS);
    float** mt_out = (*mt)();
    
    // cout<< verify(mt_out, st_out, N, L) << endl;
 
    measure_run_time(st);
    measure_run_time(mt);

    return 0;
}

bool verify(float** A, float** B, int x, int y){
    for (size_t i = 0; i < x; i++){
        for (size_t j = 0; j < y; j++){
            if(A[i][j] != B[i][j])
                return false;
        }
    }
    return true;
}
