/* Sequencial.c (Roland Teodorowitsch) */

#include "mpi.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* CONTANTES */
#define GRAU         400
#define TAM_INI  1000000
#define TAM_INC  1000000
#define TAM_MAX 10000000


/* VARIAVEIS GLOBAIS */
double x[TAM_MAX], y[TAM_MAX], gabarito[TAM_MAX];


/* PROTOTIPOS */
void erro(char *msg_erro);
double polinomio(double v[], int grau, double x);


double polinomio(double a[], int grau, double x) {
    int i;
    double res = a[0], pot = x;
    for (i = 1; i <= grau; ++i) {
        res += a[i] * pot;
        pot = pot * x;
    }
    return res;
}


void erro(char *msg_erro) {
    fprintf(stderr, "ERRO: %s\n", msg_erro);
    MPI_Finalize();
    exit(1);
}


int main(int argc, char** argv) {
    int proc_id;      
    int num_procs;
    double *vet, valor, *vresp, resposta, tempo, coefs[GRAU + 1];
    int hostsize; 
    char hostname[MPI_MAX_PROCESSOR_NAME];
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Get_processor_name(hostname, &hostsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    
    if (proc_id == 0) { 
        // processo pai
        
        /* Gera os coeficientes do polinomio */
        #pragma omp parallel for
        for (int i = 0; i <= GRAU; ++i)
            coefs[i] = (i % 3 == 0) ? -1.0 : 1.0;

        /* Preenche vetores */
        #pragma omp parallel for
        for (int i = 0; i < TAM_MAX; ++i) {
            x[i] = 0.1 + 0.1 * (double) i / TAM_MAX;
            gabarito[i] = polinomio(coefs, GRAU, x[i]);
        }

        MPI_Barrier(MPI_COMM_WORLD);            
        MPI_Bcast(&coefs, GRAU, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        /* Gera tabela com tamanhos e tempos */
        for (int size = TAM_INI; size <= TAM_MAX; size += TAM_INC) {
            
            /* Calcula */
            tempo = -MPI_Wtime();
            for (int child_id = 1, stop_signal = 0; child_id < num_procs; child_id++) {
                // distribuo a mesma quantidade para cada filho => balanceamento de carga
                int lower_bound = (child_id - 1) * size / (num_procs - 1);
                int upper_bound = child_id * size / (num_procs - 1);
                int size = upper_bound - lower_bound;
                
                MPI_Send(&stop_signal, 1, MPI_INT, child_id, 1, MPI_COMM_WORLD);
                MPI_Send(&size, 1, MPI_INT, child_id, 1, MPI_COMM_WORLD);
                MPI_Send(&lower_bound, 1, MPI_INT, child_id, 1, MPI_COMM_WORLD);
                MPI_Send(&x[lower_bound], size, MPI_DOUBLE, child_id, 1, MPI_COMM_WORLD);
            }

            for (int child_id = 1, tam, index; child_id < num_procs; child_id++) {
                MPI_Recv(&tam, 1, MPI_INT, child_id, 1, MPI_COMM_WORLD, &status);
                MPI_Recv(&index, 1, MPI_INT, child_id, 1, MPI_COMM_WORLD, &status);
                MPI_Recv(&y[index], tam, MPI_DOUBLE, child_id, 1, MPI_COMM_WORLD, &status);
            }
            tempo += MPI_Wtime();

            /* Verificacao */
            for (int i = 0; i < size; i++)
                if (y[i] != gabarito[i])
                    erro("verificacao falhou!");
            
            /* Mostra tempo */
            printf("4,%d,%lf\n", size, tempo);
        }

        for(int child_id = 1, stop_signal = 1; child_id < num_procs; child_id++)
            MPI_Send(&stop_signal, 1, MPI_INT, child_id, 1, MPI_COMM_WORLD);

    } else {
        // processos filhos
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Bcast(&coefs, GRAU, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        
        while (1) {
            int stop_signal;
            MPI_Recv(&stop_signal, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
            if (stop_signal)
                break;

            int carga_balanceada, index;

            MPI_Recv(&carga_balanceada, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
            MPI_Recv(&index, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
            MPI_Recv(&x[0], carga_balanceada, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);

            for (int i = 0; i < carga_balanceada; i++) 
                y[i] = polinomio(coefs, GRAU, x[i]);
            
            MPI_Send(&carga_balanceada, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
            MPI_Send(&index, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
            MPI_Send(&y[0], carga_balanceada, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
        }
    }
    
    MPI_Finalize();
    return 0;
}