#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALUNOS 100
#define TAM_NOME 100
#define TAM_TURMA 50
#define TAM_STATUS 20

typedef struct {
    char nome[TAM_NOME];
    float valor;
    char turma[TAM_TURMA];
    char status_pagamento[TAM_STATUS];
} Aluno;

Aluno alunos[MAX_ALUNOS];
int total_alunos = 0;

void salvarAlunos() {
    FILE *file = fopen("alunos.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    for (int i = 0; i < total_alunos; i++) {
        fprintf(file, "%s;%0.2f;%s;%s\n", alunos[i].nome, alunos[i].valor, alunos[i].turma, alunos[i].status_pagamento);
    }

    fclose(file);
}

void carregarAlunos() {
    FILE *file = fopen("alunos.txt", "r");
    if (file == NULL) return;

    while (fscanf(file, "%99[^;];%f;%49[^;];%19[^\n]\n",
                  alunos[total_alunos].nome,
                  &alunos[total_alunos].valor,
                  alunos[total_alunos].turma,
                  alunos[total_alunos].status_pagamento) != EOF) {
        total_alunos++;
    }

    fclose(file);
}

void listarAlunosPorTurma() {
    if (total_alunos == 0) {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }

    char turmas_encontradas[MAX_ALUNOS][TAM_TURMA];
    int total_turmas = 0;

    // Identificar turmas únicas
    for (int i = 0; i < total_alunos; i++) {
        int encontrada = 0;
        for (int j = 0; j < total_turmas; j++) {
            if (strcmp(alunos[i].turma, turmas_encontradas[j]) == 0) {
                encontrada = 1;
                break;
            }
        }
        if (!encontrada) {
            strcpy(turmas_encontradas[total_turmas], alunos[i].turma);
            total_turmas++;
        }
    }

    // Exibir alunos agrupados por turma
    for (int i = 0; i < total_turmas; i++) {
        printf("\n===== %s =====\n", turmas_encontradas[i]);
        for (int j = 0; j < total_alunos; j++) {
            if (strcmp(alunos[j].turma, turmas_encontradas[i]) == 0) {
                printf("- %s; R$%.2f; %s\n", alunos[j].nome, alunos[j].valor, alunos[j].status_pagamento);
            }
        }
    }
}

void adicionarAluno() {
    if (total_alunos >= MAX_ALUNOS) {
        printf("Limite de alunos atingido.\n");
        return;
    }

    Aluno novo;
    printf("Nome do aluno: ");
    getchar();
    fgets(novo.nome, TAM_NOME, stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0';

    printf("Valor a pagar: ");
    scanf("%f", &novo.valor);

    printf("Turma: ");
    getchar();
    fgets(novo.turma, TAM_TURMA, stdin);
    novo.turma[strcspn(novo.turma, "\n")] = '\0';

    printf("O aluno pagou? (1 - Sim, 2 - Nao): ");
    int pago;
    scanf("%d", &pago);
    strcpy(novo.status_pagamento, (pago == 1) ? "Pago" : "Nao pago");

    alunos[total_alunos++] = novo;
    salvarAlunos();
    printf("Aluno adicionado com sucesso!\n");
}

void editarAluno() {
    listarAlunosPorTurma();
    if (total_alunos == 0) return;

    int id;
    printf("\nDigite o numero do aluno que deseja editar (1 a %d): ", total_alunos);
    scanf("%d", &id);

    if (id < 1 || id > total_alunos) {
        printf("Aluno invalido.\n");
        return;
    }

    printf("O aluno pagou? (1 - Sim, 2 - Nao): ");
    int pago;
    scanf("%d", &pago);
    strcpy(alunos[id - 1].status_pagamento, (pago == 1) ? "Pago" : "Nao pago");

    salvarAlunos();
    printf("Status de pagamento atualizado com sucesso!\n");
}

void excluirAluno() {
    listarAlunosPorTurma();
    if (total_alunos == 0) return;

    int id;
    printf("\nDigite o numero do aluno que deseja excluir (1 a %d): ", total_alunos);
    scanf("%d", &id);

    if (id < 1 || id > total_alunos) {
        printf("Aluno invalido.\n");
        return;
    }

    for (int i = id - 1; i < total_alunos - 1; i++) {
        alunos[i] = alunos[i + 1];
    }

    total_alunos--;
    salvarAlunos();
    printf("Aluno excluido com sucesso!\n");
}

void menu() {
    int opcao;

    do {
        system("cls");
        printf("====== MENU ======\n");
        printf("1. Listar alunos por turma\n");
        printf("2. Adicionar aluno\n");
        printf("3. Editar pagamento de aluno\n");
        printf("4. Excluir aluno\n");
        printf("5. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        system("cls");

        switch (opcao) {
            case 1:
                listarAlunosPorTurma();
                break;
            case 2:
                adicionarAluno();
                break;
            case 3:
                editarAluno();
                break;
            case 4:
                excluirAluno();
                break;
            case 5:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }

        if (opcao != 5) {
            printf("\nPressione ENTER para continuar...");
            getchar(); getchar();
        }

    } while (opcao != 5);
}

int main() {
    carregarAlunos();
    menu();
    return 0;
}