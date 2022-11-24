# Trabalho 2: Avaliação de Polinômios (MPI)

O objetivo do trabalho é implementar, usando o padrão MPI (Message Passing Interface), uma versão paralela de um programa que obtém o resultado da avaliação de um conjunto de valores para um polinômio de grau elevado, comparando a execução dessa versão paralela com a execução da versão sequencial.

A versão sequencial, em C (usando MPI com um único processo), para realizar a avaliação do conjunto de valores para um polinômio de grau 400 (`GRAU`) é apresentada no final deste enunciado. Esse código avalia o polinômio para um conjunto de valores, sendo que as seguintes constantes definem a quantidade de valores que serão avaliados: `TAM_INI` (tamanho do conjunto inicial), `TAM_INC` (incremento até o próximo tamanho de conjunto) e `TAM_MAX` (tamanho do conjunto final). A versão sequencial usa chamadas MPI para obter o tempo de execução e primitivas OpenMP apenas para acelerar a inicialização e a verificação da correção do resultado, desta forma a versão sequencial deve ser compilada com:

`mpicc Sequencial.c -o Sequencial -fopenmp`

A versão paralela em MPI deve seguir o padrão **mestre-escravo**. O mestre tem o conjunto de valores para os quais se quer avaliar o polinômio e os coeficientes do polinômio, e, inicialmente, distribui os coeficientes para os escravos (por broadcast) e a seguir distribui conjuntos de valores a serem avaliados para os escravos. Os escravos, por sua vez, executam continuamente recebendo conjuntos de valores, avaliando-os para o polinômio e retornando uma resposta ao mestre, até receberem um aviso de que devem encerrar a sua execução.

Após paralelizar a versão sequencial com MPI, o programa deve ser executado no cluster `grad` com 1 nodo e 2 nodos. Com 1 nodo, deve-se executar a aplicação com 8 e 16 processos MPI. Com 2 nodos, deve-se executar a aplicação com 16 e 32 processos MPI, igualmente distribuídos entre os dois nodos. Em todas as execuções um dos processos será o mestre e os demais serão os escravos. Cada execução deve ser feita no mínimo 3 vezes, tomando-se os menores tempos para cada tamanho de array, de forma a minimizar interferências causadas por outros processos em execução no cluster.

Cada aluno ou grupo (de no máximo 2 integrantes) deve entregar um relatório em .PDF com: descrição dos testes realizados, descrição dos nodos de processamento, resultados obtidos (gráficos mostrando resultados para tempos, speed-up e eficiência), análise dos resultados e conclusões.

Os itens para avaliação são:

- informação ao professor de qualquer formação de novo grupo em até uma semana após a divulgação do enunciado do trabalho;
- execução da versão sequencial abaixo (para viabilizar o cálculo de speed-up e eficiência) no cluster `grad`;
- implementação da versão paralela do algoritmo em C, usando MPI e eventualmente OpenMP;
- execução da versão paralela conforme especificado acima (usando um e dois nodos do cluster), com no mínimo 3 repetições para cada execução, considerando-se, ao final, o menor tempo para cada tamanho de vetor;
- cálculo de speed up e de eficiência para os testes paralelos executados;
- apresentação de gráficos de tempo, speed-up e eficiência, variando o número de valores avaliados pelo polinômio de grau 400, mostrando as curvas para cada configuração;
- análise do balanceamento da carga na execução do programa paralelo;
- clareza do código (utilização de comentários e nomes de variáveis adequadas);
- relatório no formato .PDF com no mínimo 3 páginas e no máximo 4 páginas, usando os templates fornecidos no moodle, incluindo gráficos para apresentar os cálculos de desempenho realizados;
- arquivo compactado no formato ZIP incluindo artigo em PDF, código-fonte e demais arquivos necessários à compilação e execução (é obrigatória a entrega do código-fonte da versão paralela de forma que se possa testá-la, caso seja necessário);
- submissão através do moodle até o dia e o horário definidos na respectiva sala de entrega;
- análise do número de horas máquina usadas pelo grupo no LAD durante o desenvolvimento do trabalho.

No dia da entrega do trabalho, todos os componentes do grupo devem comparecer à aula para apresentação de seu trabalho e devem estar em condições de provar de forma argumentativa que participaram do desenvolvimento do trabalho. Esta apresentação poderá ocorrer em um ou mais dos seguintes formatos:

- apresentação oral para o professor, com os integrantes respondendo perguntas sobre o desenvolvimento do trabalho;
questionário escrito sobre o desenvolvimento do trabalho;
realização de uma execução no cluster grad para mostrar que os dados obtidos correspondem aos resultados citados no relatório.

NÃO serão avaliados trabalhos que:

- tiverem sido entregues por um grupo que NÃO foi definido dentro do prazo estipulado;
- NÃO forem entregues na respectiva opção de entrega no moodle da disciplina, até o dia e horário definidos;
- NÃO forem devidamente apresentados;
- NÃO incluírem o relatório em formato de artigo, conforme descrito acima;
- NÃO contiverem resultados de execução no cluster ou que NÃO tiverem sido executados no cluster do LAD;
- NÃO contiverem gráficos mostrando os resultados da execução;
- NÃO incluírem o código-fonte (compilando sem erros) no arquivo compactado entregue no moodle.