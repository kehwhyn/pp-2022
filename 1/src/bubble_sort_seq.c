// bubble_sort.c (Roland Teodorowitsch; 15 set. 2022)

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_ARRAYS 500
#define INI_ARRAY_SIZE 2500
#define INC_ARRAY_SIZE 2500
#define MAX_ARRAY_SIZE 25000

// ESTRUTURA DE DADOS COMPARTILHADA
int arrays[NUM_ARRAYS][MAX_ARRAY_SIZE];

// BUBBLE SORT
void BubbleSort(int n, int *vetor)
{
    int c = 0, troca, trocou = 1;

    while (c < (n - 1) && trocou)
    {
        trocou = 0;
        for (int d = 0; d < n - c - 1; d++)
            if (vetor[d] > vetor[d + 1])
            {
                troca = vetor[d];
                vetor[d] = vetor[d + 1];
                vetor[d + 1] = troca;
                trocou = 1;
            }
        c++;
    }
}

int main(int const argc, char const *argv[])
{
    int i, j, array_size;
    double tempo;

    int const ARRAY_SIZE = atoi(argv[1]);
    char const *FILE_NAME = argv[2];
    printf("EXECUTANDO SEQ ARRAY DE TAMANHO => %d\n", ARRAY_SIZE);

    FILE *const file = fopen(FILE_NAME, "w");

    if (file == NULL)
        printf("Couldn't open file.\n");

    fprintf(file, "iter,array_size,time\n");

    for (int k = 0; k < 5; k++)
    {
        for (array_size = INI_ARRAY_SIZE; array_size <= MAX_ARRAY_SIZE; array_size += INC_ARRAY_SIZE)
        {
            // INICIALIZA OS ARRAYS A SEREM ORDENADOS
            #pragma omp parallel for private(j) num_threads(omp_get_num_procs())
            for (i = 0; i < ARRAY_SIZE; i++)
                for (j = 0; j < array_size; j++)
                    if (i % 5 == 0)
                        arrays[i][j] = array_size - j;
                    else
                        arrays[i][j] = j + 1;

            // REALIZA A ORDENACAO
            tempo = -omp_get_wtime();
            for (i = 0; i < ARRAY_SIZE; i++)
                BubbleSort(array_size, &arrays[i][0]);
            tempo += omp_get_wtime();

            // VERIFICA SE OS ARRAYS ESTAO ORDENADOS
            for (i = 0; i < ARRAY_SIZE; i++)
                for (j = 0; j < array_size - 1; j++)
                    if (arrays[i][j] > arrays[i][j + 1])
                        return 1;

            // SALVA DADOS COLETADOS NUM ARQUIVO
            fprintf(file, "%d,%d,%lf\n", k, array_size, tempo);
        }
    }

    fclose(file);

    return 0;
}