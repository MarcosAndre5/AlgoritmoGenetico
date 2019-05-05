#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#define NUMERO_ORGANISMOS 100
#define TAXA_MUTACAO 0.05
#define MAX_GERACOES 100
#define MAXIMO_FITNESS numGenes
#define FALSE 0
#define TRUE 1

int **geracaoAtual, **proximaGeracao;
int **organismoFitnesses;
int **organismoFitnessesProx;
int **itens;
int totalFitnesses;
int numGenes;
int tamMochila;

void alocarMemoria(void);
int fazerExecucao(void);
void iniciarOrganismos(void);
int avaliarOrganismos(void);
void produzirProximaGeracao(void);
void selecionarOrganismo(void);

void imprimirAtual(int tam) {
    int organismo, gene;

    for(organismo = 0; organismo < tam; organismo++) {
        for(gene = 0; gene < numGenes; gene++)
            printf("%d", geracaoAtual[organismo][gene]);
        printf(" P = %d V = %d\n", geracaoAtual[organismo][numGenes], geracaoAtual[organismo][numGenes + 1]);
    }
}

void imprimirProx(int tam) {
    int organismo, gene;

    for(organismo = 0; organismo < tam; organismo++) {
        for(gene = 0; gene < numGenes; gene++)
            printf("%d", proximaGeracao[organismo][gene]);
        printf(" P = %d V = %d\n", proximaGeracao[organismo][numGenes], proximaGeracao[organismo][numGenes + 1]);
    }
}

int main(){
    srand(time(0));
	alocarMemoria();
    fazerExecucao();
	return 0;
}

int compara(const void *p1, const void *p2) {
	int *e1 = *(int **)p1;
	int *e2 = *(int **)p2;

	if(e1[numGenes + 1] != e2[numGenes + 1])
		return e2[numGenes + 1] - e1[numGenes + 1];
	return e2[numGenes] - e1[numGenes];
}

/*	Primeira tarefa é alocar memória para armazenar os organismos. A única informação armazenada sobre cada organismo
é sua genética (ou seja, seu conjunto de genes). No caso aqui, 100 organismos, cada um com 20 genes. Armazenando os
genes de cada organismo como um array de variáveis ​​char.	*/
void alocarMemoria(void){
	int organismo, i;

	scanf("%d %d", &numGenes, &tamMochila);

	geracaoAtual = (int**)malloc(sizeof(int*) * NUMERO_ORGANISMOS);
	if(geracaoAtual == NULL) {
		printf("Erro de alocacao 1\n");
		exit(-1);
	}
	proximaGeracao = (int**)malloc(sizeof(int*) * NUMERO_ORGANISMOS);
	if(proximaGeracao == NULL) {
		printf("Erro de alocacao 2\n");
		exit(-1);
	}
	for(organismo = 0; organismo < NUMERO_ORGANISMOS; organismo++){
		geracaoAtual[organismo] = (int*)malloc(sizeof(int) * (numGenes + 2));
		if(geracaoAtual[organismo] == NULL) {
			printf("Erro de alocacao 4\n");
			exit(-1);
		}
		proximaGeracao[organismo] = (int*)malloc(sizeof(int) * (numGenes + 2));
		if(proximaGeracao[organismo] == NULL) {
			printf("Erro de alocacao 5\n");
			exit(-1);
		}
	}
	
	itens = (int **)malloc(sizeof(int *) * numGenes);
	if(itens == NULL) {
		printf("Erro de alocacao 7\n");
		exit(-1);
	}
	for(i = 0; i < numGenes; i++) {
		itens[i] = (int *)malloc(sizeof(int) * 2); // coluna 0 = peso do item | coluna 1 = valor do item
		if(itens == NULL) {
			printf("Erro de alocacao 8\n");
			exit(-1);
		}
		scanf("%d %d", &itens[i][1], &itens[i][0]);
	}
}

/*	Randomizamos os genes dos organismos. Cada gene pode ser um 0, 1, 2 e 3.	*/
void iniciarOrganismos(void){
	int organismo, gene;

	for(organismo = 0; organismo < NUMERO_ORGANISMOS; organismo++){
		for(gene = 0; gene < numGenes; gene++){
			geracaoAtual[organismo][gene] = rand() % 2;
			if(geracaoAtual[organismo][numGenes] + itens[gene][0] > tamMochila)
				geracaoAtual[organismo][gene] = 0;
			geracaoAtual[organismo][numGenes] += geracaoAtual[organismo][gene] * itens[gene][0];
			geracaoAtual[organismo][numGenes + 1] += geracaoAtual[organismo][gene] * itens[gene][1];
		}
	}
	qsort(geracaoAtual, NUMERO_ORGANISMOS, sizeof(int*), compara);
}

/*	Uma vez que é descoberto a adequação de todos os organismos em nossa geração atual, pode-se selecionar os
melhores organismos e reproduzi-los. Armazenando temporariamente cada organismo da nova geração, os filhos, na
estrutura de dados proximaGeracao. Depois de criarmos todos os filhos, copiá-los para a estrutura de dados
Generation atual e a fase de reprodução estará concluída.	*/
void produzirProximaGeracao(void){
	int organismo, gene, paiUm, paiDois, pontoDeCruzamento;

	for(organismo = 0; organismo < NUMERO_ORGANISMOS; organismo++){
		paiUm = rand() % NUMERO_ORGANISMOS;
        do {
		  paiDois = rand() % NUMERO_ORGANISMOS;
        } while(paiDois == paiUm);
		pontoDeCruzamento  =  (rand() % (numGenes - 30)) + 15;

		proximaGeracao[organismo][numGenes] = 0;
		proximaGeracao[organismo][numGenes + 1] = 0;
		for(gene = 0; gene < numGenes; gene++){
			if(gene < pontoDeCruzamento){
				proximaGeracao[organismo][gene] = geracaoAtual[paiUm][gene];
			} else {
				proximaGeracao[organismo][gene] = geracaoAtual[paiDois][gene];
			}
			proximaGeracao[organismo][numGenes] += proximaGeracao[organismo][gene] * itens[gene][0];
			proximaGeracao[organismo][numGenes + 1] += proximaGeracao[organismo][gene] * itens[gene][1];
		}
        gene = rand() % numGenes;
        if((rand() % (int)(1.0 / TAXA_MUTACAO)) == 0) {
            proximaGeracao[organismo][gene] = proximaGeracao[organismo][gene] ? 0: 1;
            proximaGeracao[organismo][numGenes] += proximaGeracao[organismo][gene] * itens[gene][0];
            proximaGeracao[organismo][numGenes + 1] += proximaGeracao[organismo][gene] * itens[gene][1];
        }
		if(proximaGeracao[organismo][numGenes] > tamMochila)
			proximaGeracao[organismo][numGenes + 1] = -1;
	}

	qsort(proximaGeracao, NUMERO_ORGANISMOS, sizeof(int*), compara);
}

/*	O método usado foi o Método da Roleta, que usaremos aqui. Cada organismo é "atribuído" a uma fatia da roleta.
O tamanho da fatia que cada organismo recebe é proporcional à sua adequação. Então, após girarmos a roda e qualquer
que seja a fatia em que pousamos, esse organismo é selecionado.	*/
void selecionarOrganismo(void){
	int i, j = 0, k;

	for(i = NUMERO_ORGANISMOS - 1; i >= 0; i--) {
		if(geracaoAtual[i][numGenes + 1] < proximaGeracao[j][numGenes + 1]) {
            int *tmp = geracaoAtual[i];
            int ok = 1;
            for(k = 0; k < NUMERO_ORGANISMOS; k++) {
                if(geracaoAtual[k][numGenes + 1] == proximaGeracao[j][numGenes + 1]
                    && geracaoAtual[k][numGenes] == proximaGeracao[j][numGenes]) {
                    ok = 0;
                    break;
                }
            }
            if(ok) {
                geracaoAtual[i] = proximaGeracao[j];
                proximaGeracao[j] = tmp;
                j++;
            }
		}
	}
    qsort(geracaoAtual, NUMERO_ORGANISMOS, sizeof(int*), compara);
}


/*	O estágio de avaliação tem dois propósitos. Primeiramente determinar a adequação de todos os organismos para
que mais tarde, em produzirProximaGeracao(), saiba quais eram os melhores organismos e, portanto, quais deveriam se
reproduzir com mais frequência. E o objetivo secundário é decidir se existe uma geração perfeita, uma com pelo menos
um organismo que tenha a mesma genética do modelo.	*/
int avaliarOrganismos(void){
	int organismo, gene, organismoFitnessAtuais;

	totalFitnesses = 0;
	
	for(organismo = 0; organismo < NUMERO_ORGANISMOS; organismo++){
		organismoFitnessAtuais = 0;
		for(gene = 0; gene < numGenes; gene++){
			// if(geracaoAtual[organismo][gene] == organismoModelo[gene]){
			// 	organismoFitnessAtuais++;
			// }
		}
		//organismoFitnesses[organismo] = organismoFitnessAtuais;
		totalFitnesses += organismoFitnessAtuais;
		if(organismoFitnessAtuais == MAXIMO_FITNESS){
			return TRUE;
		}
	}
	return FALSE;
}

/*  Primeiro é iniciados os organismos, então usamos um loop simples para ir de uma geração a outra até que uma
geração perfeita seja encontrada. Uma geração perfeita é aquela que tem pelo menos um organismo que possui exatamente
os mesmos genes que o organismo modelo. Quando chegamos a uma geração perfeita, retornamos o número da geração. */
int fazerExecucao(void){
    int geracoes = 1;

    iniciarOrganismos();
    imprimirAtual(NUMERO_ORGANISMOS);
    while(geracoes < MAX_GERACOES){
        produzirProximaGeracao();
        selecionarOrganismo();
        printf("================== GERACAO %d ==================\n", geracoes);
        imprimirAtual(1);
        imprimirProx(0);
        geracoes++;
        
    }
    return geracoes;
}

