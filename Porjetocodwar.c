#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define QTD_TERRITORIOS 5

// Definição da struct
typedef struct {
    char nome[50];
    char corExercito[30];
    int tropas;
} Territorio;



void imprimirMapa(Territorio *territorios, int tamanho) {
    printf("\n=== Estado Atual do Mapa ===\n");
    for (int i = 0; i < tamanho; i++) {
        printf("Territorio %d:\n", i + 1);
        printf("  Nome: %s\n", territorios[i].nome);
        printf("  Cor do Exercito: %s\n", territorios[i].corExercito);
        printf("  Tropas: %d\n", territorios[i].tropas);
        printf("-----------------------------\n");
    }
}

void batalha(Territorio *atacante, Territorio *defensor) {
    int dadoAtaque = rand() % 6 + 1; // valores de 1 a 6
    int dadoDefesa = rand() % 6 + 1;

     printf("\n⚔️ Batalha entre %s (Atacante) e %s (Defensor)!\n", atacante->nome, defensor->nome);
    printf("Dado do atacante: %d | Dado do defensor: %d\n", dadoAtaque, dadoDefesa);

    if (dadoAtaque >= dadoDefesa) {
        defensor->tropas--;
        printf("👉 O atacante venceu a rodada! %s perde 1 tropa.\n", defensor->nome);

        if (defensor->tropas <= 0) {
            defensor->tropas = 0;
            printf("🎉 %s conquistou o território de %s!\n", atacante->nome, defensor->nome);
            strcpy(defensor->corExercito, atacante->corExercito); // muda de dono
        }
    } else {
        printf("🛡️ O defensor resistiu! Nada muda.\n");
    }
}



int main() {
    srand(time(NULL)); // inicializa gerador de números aleatórios

    // Alocação dinâmica
    Territorio *territorios = (Territorio *)calloc(QTD_TERRITORIOS, sizeof(Territorio));

    if (territorios == NULL) {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }

    printf("=== Cadastro de Territorios ===\n\n");
    for (int i = 0; i < QTD_TERRITORIOS; i++) {
        printf("Territorio %d:\n", i + 1);

        printf("Nome: ");
        fgets(territorios[i].nome, sizeof(territorios[i].nome), stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0';

        printf("Cor do exercito: ");
        fgets(territorios[i].corExercito, sizeof(territorios[i].corExercito), stdin);
        territorios[i].corExercito[strcspn(territorios[i].corExercito, "\n")] = '\0';

        printf("Numero de tropas: ");
        scanf("%d", &territorios[i].tropas);
        getchar(); // consome \n deixado pelo scanf

        printf("\n");
    }

    imprimirMapa(territorios, QTD_TERRITORIOS);

    // Loop de batalhas
    int atacante, defensor;
    while (1) {
        printf("\nEscolha o territorio atacante (1-%d, 0 para sair): ", QTD_TERRITORIOS);
        scanf("%d", &atacante);
        if (atacante == 0) break;

        printf("Escolha o territorio defensor (1-%d): ", QTD_TERRITORIOS);
        scanf("%d", &defensor);

        if (atacante < 1 || atacante > QTD_TERRITORIOS ||
            defensor < 1 || defensor > QTD_TERRITORIOS ||
            atacante == defensor) {
            printf("❌ Escolha invalida!\n");
            continue;
        }

        batalha(&territorios[atacante - 1], &territorios[defensor - 1]);
        imprimirMapa(territorios, QTD_TERRITORIOS);
    }

    free(territorios); // libera memória
    printf("\nJogo encerrado.\n");

    return 0;
}
