#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define QTD_TERRITORIOS 5

typedef struct {
    char nome[50];
    char corExercito[30];
    int tropas;
} Territorio;

typedef struct {
    int tipoMissao; // 1 = destruir exército Verde, 2 = conquistar 3 territórios
    int territoriosConquistados; // para missão 2
    int missaoConcluida;
} Missao;

// ===== Funções =====

// Inicializa os territórios automaticamente
void inicializarTerritorios(Territorio *territorios) {
    const char *nomes[QTD_TERRITORIOS] = {"Brasil", "Argentina", "Chile", "Peru", "Colombia"};
    const char *cores[QTD_TERRITORIOS] = {"Verde", "Azul", "Amarelo", "Vermelho", "Roxo"};
    const int tropas[QTD_TERRITORIOS] = {5, 5, 5, 5, 5};

    for (int i = 0; i < QTD_TERRITORIOS; i++) {
        strcpy(territorios[i].nome, nomes[i]);
        strcpy(territorios[i].corExercito, cores[i]);
        territorios[i].tropas = tropas[i];
    }
}

// Imprime o estado do mapa
void imprimirMapa(const Territorio *territorios) {
    printf("\n=== Estado Atual do Mapa ===\n");
    for (int i = 0; i < QTD_TERRITORIOS; i++) {
        printf("Territorio %d:\n", i + 1);
        printf("  Nome: %s\n", territorios[i].nome);
        printf("  Cor do Exercito: %s\n", territorios[i].corExercito);
        printf("  Tropas: %d\n", territorios[i].tropas);
        printf("-----------------------------\n");
    }
}

// Simula uma batalha entre dois territórios
void batalha(Territorio *atacante, Territorio *defensor) {
    int dadoAtaque = rand() % 6 + 1;
    int dadoDefesa = rand() % 6 + 1;

    printf("\n⚔️ Batalha: %s (Atacante) vs %s (Defensor)\n", atacante->nome, defensor->nome);
    printf("Dado Atacante: %d | Dado Defensor: %d\n", dadoAtaque, dadoDefesa);

    if (dadoAtaque >= dadoDefesa) {
        defensor->tropas--;
        printf("👉 Atacante vence a rodada! %s perde 1 tropa.\n", defensor->nome);

        if (defensor->tropas <= 0) {
            defensor->tropas = 0;
            printf("🎉 %s conquistou o território de %s!\n", atacante->nome, defensor->nome);
            strcpy(defensor->corExercito, atacante->corExercito);
        }
    } else {
        printf("🛡️ Defensor resistiu! Nada muda.\n");
    }
}

// Verifica se a missão foi cumprida
void verificarMissao(const Territorio *territorios, Missao *missao) {
    if (missao->tipoMissao == 1) { // destruir exército Verde
        int verdeVivo = 0;
        for (int i = 0; i < QTD_TERRITORIOS; i++) {
            if (strcmp(territorios[i].corExercito, "Verde") == 0 && territorios[i].tropas > 0)
                verdeVivo++;
        }
        if (verdeVivo == 0) {
            missao->missaoConcluida = 1;
            printf("\n✅ Missão Concluída! Todos os exércitos Verdes foram destruídos.\n");
        } else {
            printf("\n❌ Missão não concluída. Ainda restam %d territórios Verdes.\n", verdeVivo);
        }
    } else if (missao->tipoMissao == 2) { // conquistar 3 territórios
        int conquistados = 0;
        for (int i = 0; i < QTD_TERRITORIOS; i++) {
            if (strcmp(territorios[i].corExercito, "Azul") != 0) // supomos jogador é Azul
                conquistados++;
        }
        if (conquistados >= 3) {
            missao->missaoConcluida = 1;
            printf("\n✅ Missão Concluída! Você conquistou %d territórios!\n", conquistados);
        } else {
            printf("\n❌ Missão não concluída. Você conquistou %d territórios.\n", conquistados);
        }
    }
}

// Escolhe a missão aleatória
void gerarMissao(Missao *missao) {
    missao->tipoMissao = (rand() % 2) + 1;
    missao->missaoConcluida = 0;
    missao->territoriosConquistados = 0;

    printf("\n🎯 Sua missão é: ");
    if (missao->tipoMissao == 1) {
        printf("Destruir o exército Verde!\n");
    } else {
        printf("Conquistar 3 territórios!\n");
    }
}

// Menu principal
void menu(Territorio *territorios, Missao *missao) {
    int opcao;
    do {
        printf("\n=== MENU ===\n");
        printf("1 - Atacar\n");
        printf("2 - Verificar Missão\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                int atacante, defensor;
                imprimirMapa(territorios);
                printf("\nEscolha o territorio atacante (1-%d): ", QTD_TERRITORIOS);
                scanf("%d", &atacante);
                printf("Escolha o territorio defensor (1-%d): ", QTD_TERRITORIOS);
                scanf("%d", &defensor);

                if (atacante < 1 || atacante > QTD_TERRITORIOS ||
                    defensor < 1 || defensor > QTD_TERRITORIOS ||
                    atacante == defensor) {
                    printf("❌ Escolha inválida!\n");
                } else {
                    batalha(&territorios[atacante-1], &territorios[defensor-1]);
                }
                break;
            }
            case 2:
                verificarMissao(territorios, missao);
                break;
            case 0:
                printf("Saindo do jogo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0 && !missao->missaoConcluida);

    if (missao->missaoConcluida) {
        printf("\n🏆 Parabéns! Você cumpriu sua missão!\n");
    }
}

// ===== Main =====
int main() {
    srand(time(NULL));

    Territorio *territorios = (Territorio *)calloc(QTD_TERRITORIOS, sizeof(Territorio));
    if (!territorios) {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }

    Missao missao;
    inicializarTerritorios(territorios);
    gerarMissao(&missao);
    menu(territorios, &missao);

    free(territorios);
    return 0;
}
