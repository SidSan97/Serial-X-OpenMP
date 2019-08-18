#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <locale.h>
#include <time.h>
#pragma warning(disable:4996)  //Pragma para o programa compilar no VS sem os erros de segurança

static void SERIAL(void)
{
	setlocale(LC_ALL, "Portuguese");
	clock_t c1, c2, c3, c4;
	float tempo, tempo1, tempo2;
	int LMA, CMA, LMX, CMX, LMB, CMB, aux, nLido; //LMA = LINHA MATRIZ A. CMA = COLUNA MATRIZ A. O MESMO VALE PARA MATRIZ B E X.
	int h, o, cont = 0;
	int matriz_A[5][5];
	int matriz_x[5][4];
	int matriz_B[5][4];

	FILE* matrizA;  //DANDO NOME AO PONTEIRO QUE IRÁ LER O ARQUIVO COM A MATRIZ

	printf(" Abaixo está a matriz esparssa contida no arquivo .txt A\n");

	matrizA = fopen("matriz_A.txt", "r"); //LENDO O ARQUIVO .TXT QUE CONTEM A MATRIZ
	if (matrizA == NULL)
	{
		printf("* não foi possivel abrir arquivo matriz_A.txt\n");
		exit(1);
	}

	c1 = clock();
	//PARTE DO CÓDIGO QUE IRÁ LER A MATRIZ NO ARQUIVO .TXT E IMPRIMI-LA NO TERMINAL
	for (LMA = 0; LMA < 5; LMA++)
	{
		for (CMA = 0; CMA < 5; CMA++)
		{
			nLido = fscanf(matrizA, "%d", &matriz_A[LMA][CMA]);
			if (nLido != 1)
			{
				printf(" erro na leitura de matriz_A[%d][%d]\n", LMA, CMA);
				exit(2);
			}
			if (matriz_A[LMA][CMA] != 0)  //FAZ A CONTAGEM DE QTS ELEMENOS DIFERENTES DE 0 A MATRIZ POSSUI
			{
				cont++;
			}
			printf("\t%02d ", matriz_A[LMA][CMA]);
		}
		printf("\n");
	}
	fclose(matrizA); //FINALIZA A LEITURA DO ARQUIVO DE TEXTO

	printf("\n Abaixo a matriz A esparssa em CSR \n");

	//ABAIXO PARTE DO CÓDIGO QUE CALCULA E IMPRIME A MATRIZ A ESPARSSA COMPACTADA
	int matrizCompc[3][10];
	int k = 0;

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (matriz_A[i][j] != 0)
			{
				matrizCompc[0][k] = i;
				matrizCompc[1][k] = j;
				matrizCompc[2][k] = matriz_A[i][j];
				k++;
			}
		}
	}

	for (int i = 0; i<3; i++)
	{
		for (int j = 0; j<10; j++)
		{
			printf(" %d ", matrizCompc[i][j]);
		}

		printf("\n");
	}
	c2 = clock();

	printf("\n");


	//PARTE DO CODIGO QUE FARÁ LEITURA DA MATRIZ DIGITADA PELO USUÁRIO
	printf("\nInforme 4 valores para primeira linha da matriz X\n");
	for (LMX = 0; LMX<5; LMX++)
	{
		for (CMX = 0; CMX<4; CMX++)
		{
			scanf("%d", &matriz_x[LMX][CMX]);
		}
		printf("\nInforme mais 4 valores para a proxima linha da matriz B \n");
	}

	c3 = clock();
	//Este laço for vai imprimir na tela a matriz 5x4 que contem os valores digitados pelo usuario
	for (LMX = 0; LMX<5; LMX++)
	{
		for (CMX = 0; CMX<4; CMX++)
		{
			printf(" %d ", matriz_x[LMX][CMX]);
		}
		printf("\n");
	}

	//Parte do código que faz a operação de multiplicação entre as matrizes
	for (int i = 0; i<LMA; i++)
	{
		for (int j = 0; j<CMX; j++)
		{
			matriz_B[i][j] = 0;
			for (int x = 0; x<LMX; x++)
			{
				aux += matriz_A[i][x] * matriz_x[x][j];
			}
			matriz_B[i][j] = aux;
			aux = 0;
		}
	}

	//Imprime os resultados da multiplicação
	printf("\nResultado \n");
	for (h = 0; h<5; h++)
	{
		for (o = 0; o<4; o++)
		{
			printf(" %d ", matriz_B[h][o]);
		}
		printf("\n");
	}


	//PARTE DO CÓDIGO QUE ESCREVE O PRODUTO DA MATRIZ EM UM ARQUIVO DE TEXTO CHAMADO 'MatrizC'.
	FILE *matrizB;
	matrizB = fopen("MatrizC.txt", "w");

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < o; j++)
		{
			fprintf(matrizB, " %d ", matriz_B[i][j]);
		}
		fprintf(matrizB, "\n", matriz_B[i][4]);
	}
	c4 = clock();

	//CALCULA O TEMPO QUE A CPU FEZ TODO O PROCESSO 
	tempo  =  (c2 - c1);
	tempo1 =  (c4 - c3);
	tempo2 =  (tempo1 - tempo);
	
	printf("\n Tempo total de execução do programa foi de %f milissegundos \n", tempo);

	system("pause");
	main(); //RETORNA A FUNÇÃO PRINCIPAL (MENU)

}

static void OPENMP()
{
	setlocale(LC_ALL, "Portuguese");
	clock_t c1, c2, c3, c4;
	float tempo, tempo1, tempo2;
	int LMA, CMA, LMX, CMX, LMB, CMB, aux, nLido; //LMA = LINHA MATRIZ A. CMA = COLUNA MATRIZ A. O MESMO VALE PARA MATRIZ B E X.
	int h, o, cont = 0;
	int tid, nthreads, chunk;
	int matriz_A[5][5];
	int matriz_x[5][4];
	int matriz_B[5][4];

	FILE* matrizA;  //DANDO NOME AO PONTEIRO QUE IRÁ LER O ARQUIVO COM A MATRIZ

	printf(" Abaixo está a matriz esparssa contida no arquivo .txt A\n");

	matrizA = fopen("matriz_A.txt", "r"); //LENDO O ARQUIVO .TXT QUE CONTEM A MATRIZ
	if (matrizA == NULL)
	{
		printf("* não foi possivel abrir arquivo matriz_A.txt\n");
		exit(1);
	}

		//PARTE DO CÓDIGO QUE IRÁ LER A MATRIZ NO ARQUIVO .TXT E IMPRIMI-LA NO TERMINAL
		for (LMA = 0; LMA < 5; LMA++)
		{
			for (CMA = 0; CMA < 5; CMA++)
			{
				nLido = fscanf(matrizA, "%d", &matriz_A[LMA][CMA]);
				if (nLido != 1)
				{
					printf(" erro na leitura de matriz_A[%d][%d]\n", LMA, CMA);
					exit(2);
				}
				if (matriz_A[LMA][CMA] != 0)  //FAZ A CONTAGEM DE QTS ELEMENOS DIFERENTES DE 0 A MATRIZ POSSUI
				{
					cont++;
				}
				printf("\t%02d ", matriz_A[LMA][CMA]);
			}
			printf("\n");
		}
		fclose(matrizA); //FINALIZA A LEITURA DO ARQUIVO DE TEXTO

		printf("\n Abaixo a matriz A esparssa em CSR \n");

		//ABAIXO PARTE DO CÓDIGO QUE CALCULA E IMPRIME A MATRIZ A ESPARSSA EM CSR
		int matrizCompc[3][10];
		int k = 0;

		#pragma omp parallel
		{
			tid = omp_get_thread_num();
			if (tid == 0){
				nthreads = omp_get_num_threads();  //INICIALIZA AS THREADS
				printf("Iniciando multiplicacao de matrizes com %d threads\n", nthreads);
			}
          #pragma omp for schedule(static, chunk)
			c1 = clock();
			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < 5; j++)
				{
					if (matriz_A[i][j] != 0)
					{
						matrizCompc[0][k] = i;
						matrizCompc[1][k] = j;
						matrizCompc[2][k] = matriz_A[i][j];
						k++;
					}
				}
			}

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					printf(" %d ", matrizCompc[i][j]);
				}

				printf("\n");
			}
		}
		c2 = clock();
		printf("\n");


		//PARTE DO CODIGO QUE FARÁ LEITURA DA MATRIZ DIGITADA PELO USUÁRIO
		printf("\nInforme 4 valores para primeira linha da matriz X\n");
		for (LMX = 0; LMX < 5; LMX++)
		{
			for (CMX = 0; CMX < 4; CMX++)
			{
				scanf("%d", &matriz_x[LMX][CMX]);
			}
			printf("\nInforme mais 4 valores para a proxima linha da matriz B \n");
		}

		//Este laço for vai imprimir na tela a matriz 5x4 que contem os valores digitados pelo usuario
		for (LMX = 0; LMX < 5; LMX++)
		{
			for (CMX = 0; CMX < 4; CMX++)
			{
				printf(" %d ", matriz_x[LMX][CMX]);
			}
			printf("\n");
		}

        #pragma omp parallel
		{
			tid = omp_get_thread_num();
			if (tid == 0)
			{
			  nthreads = omp_get_num_threads();  //INICIALIZA AS THREADS
			  printf("Iniciando exemplo de multiplicacao de matrizes com %d threads\n", nthreads);
			}

			//Parte do código que faz a operação de multiplicação entre as matrizes
            #pragma omp for schedule(static, chunk)
			c3 = clock();
			for (int i = 0; i < LMA; i++)
			{
				for (int j = 0; j < CMX; j++)
				{
					matriz_B[i][j] = 0;
					for (int x = 0; x < LMX; x++)
					{
						aux += matriz_A[i][x] * matriz_x[x][j];
					}
					matriz_B[i][j] = aux;
					aux = 0;
				}
			}
			c4 = clock();
		}
		//Imprime os resultados da multiplicação
		printf("\nResultado \n");
		for (h = 0; h < 5; h++)
		{
			for (o = 0; o < 4; o++)
			{
				printf(" %d ", matriz_B[h][o]);
			}
			printf("\n");
		}


		//PARTE DO CÓDIGO QUE ESCREVE O PRODUTO DA MATRIZ EM UM ARQUIVO DE TEXTO CHAMADO 'MatrizC'.
		FILE *matrizB;
		matrizB = fopen("MatrizC.txt", "w");

		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < o; j++)
			{
				fprintf(matrizB, " %d ", matriz_B[i][j]);
			}
			fprintf(matrizB, "\n", matriz_B[i][4]);
		}

		//CALCULA O TEMPO QUE A CPU FEZ TODO O PROCESSO 
		tempo = (c2 - c1);
		tempo1 = (c4 - c3);
		tempo2 = (tempo1 - tempo);

		printf("\n Tempo total de execução do programa foi de %f milissegundos \n", tempo);
	
	main();
}


int main(void)
{
	setlocale(LC_ALL, "Portuguese");
	char escolha = '0';

	printf("*\n");
	printf("* Em que modo você deseja executar o programa?\n");
	printf("* (1) SERIAL \n");
	printf("* (2) OPENMP \n");
	printf("* qualquer outra tecla fecha o programa\n");
	scanf(" %c%*[^\n]", &escolha);
	//escolha = getche();

	if (escolha == '1')
	{
		SERIAL();
	}
	else if (escolha == '2')
	{
		OPENMP();
	}
	else
	{
		printf("* escolha invalida, programa encerrado\n");
		exit(3);
	}
	system("pause");
	return;

}

// 0 1 0 0 3
// 4 0 0 0 6
// 0 5 0 3 0
// 8 0 9 0 0
// 0 4 0 0 1
