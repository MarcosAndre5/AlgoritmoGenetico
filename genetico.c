#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#define NUMERO_ORGANISMOS 100
#define NUMERO_GENES 20
#define TODOS 4
#define TAXA_MUTACAO 0.001
#define MAXIMO_FITNESS NUMERO_GENES
#define FALSE 0
#define TRUE 1

char **geracaoAtual, **proximaGeracao;
char *organismoModelo;
int *organismoFitnesses;
int totalFitnesses;

void alocarMemoria(void);
int fazerExecucao(void);
void iniciarOrganismos(void);
int avaliarOrganismos(void);
void produzirProximaGeracao(void);
int selecionarOrganismo(void);

int main(){
	int geracaoFinal;

	alocarMemoria();
	geracaoFinal = fazerExecucao();
	printf("A geração final foi: %d\n", geracaoFinal);
}

/*	Primeira tarefa é alocar memória para armazenar os organismos. A única informação armazenada sobre cada organismo
é sua genética (ou seja, seu conjunto de genes). No caso aqui, 100 organismos, cada um com 20 genes. Armazenando os
genes de cada organismo como um array de variáveis ​​char.	*/
void alocarMemoria(void){
	int organismo;

	geracaoAtual = (char**)malloc(sizeof(char*) * NUMERO_ORGANISMOS);
	proximaGeracao = (char**)malloc(sizeof(char*) * NUMERO_ORGANISMOS);
	organismoModelo = (char*)malloc(sizeof(char) * NUMERO_GENES);
	organismoFitnesses = (int*)malloc(sizeof(int) * NUMERO_ORGANISMOS);

	for(organismo = 0; organismo < NUMERO_ORGANISMOS; organismo++){
		geracaoAtual[organismo] = (char*)malloc(sizeof(char) * NUMERO_GENES);
		proximaGeracao[organismo] = (char*)malloc(sizeof(char) * NUMERO_GENES);
	}
}

/*	Primeiro é iniciados os organismos, então usamos um loop simples para ir de uma geração a outra até que uma
geração perfeita seja encontrada. Uma geração perfeita é aquela que tem pelo menos um organismo que possui exatamente
os mesmos genes que o organismo modelo. Quando chegamos a uma geração perfeita, retornamos o número da geração.	*/
int fazerExecucao(void){
	int geracoes = 1, geracaoPerfeita = FALSE;

	iniciarOrganismos();

	while(TRUE){
		geracaoPerfeita = avaliarOrganismos();
		if(geracaoPerfeita == TRUE){
			return geracoes;
		}
		geracoes++;
		produzirProximaGeracao();
	}
}

/*	Randomizamos os genes dos organismos. Cada gene pode ser um 0, 1, 2 e 3.	*/
void iniciarOrganismos(void){
	int organismo, gene;

	printf("Organismos Criados:\n\n");
	for(organismo = 0; organismo < NUMERO_ORGANISMOS; organismo++){
		printf("%d - ", organismo + 1);
		for(gene = 0; gene < NUMERO_GENES; gene++){
			geracaoAtual[organismo][gene] = rand() % TODOS;
			printf("%d ", geracaoAtual[organismo][gene]);
		}
		printf("\n");
	}

	printf("\n\nOrganismos Modelo:\n\n");
	for(gene = 0; gene < NUMERO_GENES; gene++){
		organismoModelo[gene] = rand() % TODOS;
		printf("%d ", organismoModelo[gene]);
	}
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
		for(gene = 0; gene < NUMERO_GENES; gene++){
			if(geracaoAtual[organismo][gene] == organismoModelo[gene]){
				organismoFitnessAtuais++;
			}
		}
		organismoFitnesses[organismo] = organismoFitnessAtuais;
		totalFitnesses += organismoFitnessAtuais;
		if(organismoFitnessAtuais == MAXIMO_FITNESS){
			return TRUE;
		}
	}
	return FALSE;
}

/*	Uma vez que é descoberto a adequação de todos os organismos em nossa geração atual, pode-se selecionar os
melhores organismos e reproduzi-los. Armazenando temporariamente cada organismo da nova geração, os filhos, na
estrutura de dados proximaGeracao. Depois de criarmos todos os filhos, copiá-los para a estrutura de dados
Generation atual e a fase de reprodução estará concluída.	*/
void produzirProximaGeracao(void){
	int organismo, gene, paiUm, paiDois, pontoDeCruzamento, mudarEsseGene;

	for(organismo = 0; organismo < NUMERO_ORGANISMOS; organismo++){
		paiUm = selecionarOrganismo();
		paiDois = selecionarOrganismo();
		pontoDeCruzamento  =  rand() % NUMERO_GENES;

		for(gene = 0; gene < NUMERO_GENES; gene++){
			mudarEsseGene = rand() % (int)(1.0 / TAXA_MUTACAO);
			if(mudarEsseGene == 0){
				proximaGeracao[organismo][gene] = rand() % TODOS;
			} else {
				if(gene < pontoDeCruzamento){
					proximaGeracao[organismo][gene] = geracaoAtual[paiUm][gene];
				} else {
					proximaGeracao[organismo][gene] = geracaoAtual[paiDois][gene];
				}
			}
		}
	}

	for(organismo = 0; organismo < NUMERO_ORGANISMOS; organismo++){
		for(gene = 0; gene < NUMERO_GENES; gene++){
			geracaoAtual[organismo][gene] = proximaGeracao[organismo][gene];
		}
	}
}

/*	O método usado foi o Método da Roleta, que usaremos aqui. Cada organismo é "atribuído" a uma fatia da roleta.
O tamanho da fatia que cada organismo recebe é proporcional à sua adequação. Então, após girarmos a roda e qualquer
que seja a fatia em que pousamos, esse organismo é selecionado.	*/
int selecionarOrganismo(void){
	int organismo, execucaoTotal = 0, pontoDeSelecaoAleatoria;

	pontoDeSelecaoAleatoria = rand() % (totalFitnesses + 1);

	for(organismo = 0; organismo < NUMERO_ORGANISMOS; organismo++){
		execucaoTotal += organismoFitnesses[organismo];
		if(execucaoTotal >= pontoDeSelecaoAleatoria){
			return organismo;
		}
	}
}