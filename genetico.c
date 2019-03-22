#include <stdio.h>
#include <locale.h>

#define ORGANISMOS 100
#define GENES 20

char **geracaoAtual, **proximaGeracao;
char *organismoModelo;
int *organismosFitnesses;

void alocarMemoria(void) {
	int organismo;
	geracaoAtual = (char**)malloc(sizeof(char*) * ORGANISMOS);
	proximaGeracao = (char**)malloc(sizeof(char*) * ORGANISMOS);
	organismoModelo = (char*)malloc(sizeof(char) * GENES);
	organismosFitnesses = (int*)malloc(sizeof(int) * ORGANISMOS);

	for(organismo=0; organismo<ORGANISMOS; ++organismo) {
		geracaoAtual[organism] = (char*)malloc(sizeof(char) * GENES);
		proximaGeracao[organism] = (char*)malloc(sizeof(char) * GENES);
	}
}

int main() {
	int geracaoFinal = 0;

	alocarMemoria();
	
	geracaoFinal = primeiraExecucao();

	setlocale(LC_ALL, "Portuguese");

	printf("A geração final foi: %d\n", geracaoFinal);
}

// http://www-cs-students.stanford.edu/~jl/Essays/ga.html