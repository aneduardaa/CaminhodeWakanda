#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cidades.h"

// Funcao para comparar e ordenar as cidades
int compare(const void *a, const void *b) {
    Cidade *cidadeA = (Cidade *)a;
    Cidade *cidadeB = (Cidade *)b;
    return cidadeA->Posicao - cidadeB->Posicao;
}
// Funcao para ler os dados da estrada a partir do arquivo.txt
Estrada *getEstrada(const char *nomeArquivo) { 
    FILE *file = fopen(nomeArquivo, "r");
    if (file == NULL) {
        return NULL;
    }
	
    Estrada *estrada = (Estrada *)malloc(sizeof(Estrada));
    if (estrada == NULL) {
        fclose(file);
        return NULL;
    }
    if (fscanf(file, "%d", &estrada->T) != 1 || estrada->T < 3 || estrada->T > 1000000) {
        fclose(file);
        free(estrada);
        return NULL;
    }
    if (fscanf(file, "%d", &estrada->N) != 1 || estrada->N < 2 || estrada->N > 10000) {
        fclose(file);
        free(estrada);
        return NULL;
    }
	 // alocaçao de memória para o vetor de cidades
    estrada->C = (Cidade *)malloc(estrada->N * sizeof(Cidade));
    if (estrada->C == NULL) {
        fclose(file);
        free(estrada);
        return NULL;
    }
    int c;
    while ((c = fgetc(file)) != EOF && c != '\n');
    // lendo as posições das cidades
	int i;
    for (i = 0; i < estrada->N; i++) {
        if (fscanf(file, "%d ", &estrada->C[i].Posicao) != 1) {
            fclose(file);
            free(estrada->C);
            free(estrada);
            return NULL;
        }

        char nomeCidade[256];
        if (fgets(nomeCidade, sizeof(nomeCidade), file) == NULL) {
            fclose(file);
            free(estrada->C);
            free(estrada);
            return NULL;
        }
		// Removendo o caractere de nova linha do nome da cidade
        nomeCidade[strcspn(nomeCidade, "\n")] = '\0';
        strcpy(estrada->C[i].Nome, nomeCidade);

		// Verificando se a posição da cidade está dentro dos limites da estrada
        if (estrada->C[i].Posicao <= 0 || estrada->C[i].Posicao >= estrada->T) {
            fclose(file);
            free(estrada->C);
            free(estrada);
            return NULL;
        }
    }

    fclose(file);
    return estrada;
}
// Funcao para calcular a menor vizinhanca entre as cidades.
double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    int numeroCidades = estrada->N;
    double menorVizinhaca = estrada->T;

    //organizando as cidades conforme as respectivas posicoes
    qsort(estrada->C, estrada->N, sizeof(Cidade), compare);
    
    //calculando o tamanho da menor vizinhan�a entre as cidades.
	int i;
    for (i = 0; i < numeroCidades; ++i) {
        double ladoEsquerdo, ladoDireito;

        if (i == 0) {
            ladoEsquerdo = 0;
            ladoDireito = (estrada->C[i].Posicao + estrada->C[i + 1].Posicao) / 2.0;
        } else if (i == numeroCidades - 1) {
            ladoEsquerdo = (estrada->C[i].Posicao + estrada->C[i - 1].Posicao) / 2.0;
            ladoDireito = estrada->T;
        } else {
            ladoEsquerdo = (estrada->C[i].Posicao + estrada->C[i - 1].Posicao) / 2.0;
            ladoDireito = (estrada->C[i].Posicao + estrada->C[i + 1].Posicao) / 2.0;
        }

        double tamanhoVizinhaca = ladoDireito - ladoEsquerdo;

        if (tamanhoVizinhaca < menorVizinhaca) {
            menorVizinhaca = tamanhoVizinhaca;
        }
    }
    free(estrada->C);
    free(estrada);

    return menorVizinhaca;
}
// Funcao para determinar a cidade com menor vizinhan�a
char *cidadeMenorVizinhanca(const char *nomeArquivo) { 
    Estrada *estrada = getEstrada(nomeArquivo);
    if (estrada == NULL) {
        return NULL;
    }

    int numeroCidades = estrada->N;
    int indexMenorVizinhanca = 0;
    double menorVizinhaca = estrada->T;

    qsort(estrada->C, estrada->N, sizeof(Cidade), compare);
    
	int i;
    for (i = 0; i < numeroCidades; ++i) {
        double leftBound, rightBound;

        if (i == 0) {
            leftBound = 0;
            rightBound = (estrada->C[i].Posicao + estrada->C[i + 1].Posicao) / 2.0;
        } else if (i == numeroCidades - 1) {
            leftBound = (estrada->C[i].Posicao + estrada->C[i - 1].Posicao) / 2.0;
            rightBound = estrada->T;
        } else {
            leftBound = (estrada->C[i].Posicao + estrada->C[i - 1].Posicao) / 2.0;
            rightBound = (estrada->C[i].Posicao + estrada->C[i + 1].Posicao) / 2.0;
        }

        double tamanhoVizinhaca = rightBound - leftBound;

        if (tamanhoVizinhaca < menorVizinhaca) {
            menorVizinhaca = tamanhoVizinhaca;
            indexMenorVizinhanca = i;
        }
    }
	// Alocaçao de memoria para a da cidade com menor vizinhanha
    char *cidade = (char *) malloc(strlen(estrada->C[indexMenorVizinhanca].Nome) + 1);
    if (cidade == NULL) {
        free(estrada->C);
        free(estrada);
        return NULL;
    }
    // Copiando o nome da cidade para a vari�vel alocada
    strcpy(cidade, estrada->C[indexMenorVizinhanca].Nome);

    free(estrada->C);
    free(estrada);
    return cidade;
}
//funcao principal
int main() {
    const char *nomeArquivo = "teste01.txt"; // Nome do arquivo de entrada

	// Calculando e exibindo a menor vizinhanca entre as cidades
    double menorVizinhanca = calcularMenorVizinhanca(nomeArquivo);
    if (menorVizinhanca < 0) {
        printf("Erro ao calcular a menor vizinhanca.\n");
        return 1;
    }

    printf("Menor vizinhanca encontrada: %.2f\n", menorVizinhanca);

	// Determinando e exibindo a cidade com menor vizinhanca
    char *cidadeMenor = cidadeMenorVizinhanca(nomeArquivo);
    if (cidadeMenor == NULL) {
        printf("Erro ao determinar a cidade com menor vizinhanca.\n");
        return 1;
    }

    printf("Cidade com menor vizinhanca: %s\n", cidadeMenor);
    free(cidadeMenor);

    return 0;
}
