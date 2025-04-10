#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 100
#define FILENAME "alunos.txt"

typedef struct {
    char nome[100];
    float valor;
    char turma[50];
    char data_pagamento[11]; // Formato DD/MM/AAAA
} Aluno;

// Limpa o buffer do teclado
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Pega o mes atual
int mesAtual() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_mon + 1;
}

// Pega o ano atual
int anoAtual() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_year + 1900;
}

// Adiciona um novo aluno ao arquivo
void adicionarAluno() {
    FILE *file = fopen(FILENAME, "a");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Aluno a;
    printf("Nome do aluno: ");
    limparBuffer();
    fgets(a.nome, 100, stdin);
    a.nome[strcspn(a.nome, "\n")] = 0;

    printf("Valor da mensalidade: ");
    scanf("%f", &a.valor);

    printf("Turma: ");
    limparBuffer();
    fgets(a.turma, 50, stdin);
    a.turma[strcspn(a.turma, "\n")] = 0;

    printf("Data do pagamento (DD/MM/AAAA): ");
    fgets(a.data_pagamento, 11, stdin);
    a.data_pagamento[strcspn(a.data_pagamento, "\n")] = 0;

    fprintf(file, "%s; %.2f; %s; Pago em %s\n", a.nome, a.valor, a.turma, a.data_pagamento);
    fclose(file);

    printf("Aluno adicionado com sucesso!\n");
}

// Carrega todos os alunos do arquivo para o vetor
int carregarAlunos(Aluno alunos[]) {
    FILE *file = fopen(FILENAME, "r");
    if (!file) return 0;

    char linha[256];
    int i = 0;

    while (fgets(linha, sizeof(linha), file) && i < MAX) {
        sscanf(linha, "%[^;]; %f; %[^;]; Pago em %[^\n]",
               alunos[i].nome, &alunos[i].valor, alunos[i].turma, alunos[i].data_pagamento);
        i++;
    }

    fclose(file);
    return i;
}

// Salva todos os alunos no arquivo
void salvarAlunos(Aluno alunos[], int count) {
    FILE *file = fopen(FILENAME, "w");
    if (!file) return;

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s; %.2f; %s; Pago em %s\n",
                alunos[i].nome, alunos[i].valor, alunos[i].turma, alunos[i].data_pagamento);
    }

    fclose(file);
}

// Lista todos os alunos e status de pagamento
void listarAlunos() {
    Aluno alunos[MAX];
    int count = carregarAlunos(alunos);

    if (count == 0) {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }

    printf("\nLista de Alunos:\n");
    printf("------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        int dia, mes, ano;
        sscanf(alunos[i].data_pagamento, "%d/%d/%d", &dia, &mes, &ano);

        printf("ID: %d\n", i + 1);
        printf("Nome: %s\n", alunos[i].nome);
        printf("Valor: R$ %.2f\n", alunos[i].valor);
        printf("Turma: %s\n", alunos[i].turma);
        printf("Pagamento: Pago em %s [%s]\n", alunos[i].data_pagamento,
               (mes == mesAtual() && ano == anoAtual()) ? "Em dia" : "Pendente");
        printf("------------------------------------------------------\n");
    }
}

// Edita dados de um aluno especifico
void editarAluno() {
    Aluno alunos[MAX];
    int count = carregarAlunos(alunos);
    if (count == 0) {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }

    listarAlunos();
    int id;
    printf("Digite o ID do aluno que deseja editar: ");
    scanf("%d", &id);
    if (id < 1 || id > count) {
        printf("ID invalido.\n");
        return;
    }

    id--; // Ajustar para índice do vetor

    int opcao;
    do {
        printf("\nEditar aluno %s:\n", alunos[id].nome);
        printf("1. Nome\n2. Valor\n3. Turma\n4. Data de pagamento\n5. Voltar\nEscolha: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                printf("Novo nome: ");
                fgets(alunos[id].nome, 100, stdin);
                alunos[id].nome[strcspn(alunos[id].nome, "\n")] = 0;
                break;
            case 2:
                printf("Novo valor: ");
                scanf("%f", &alunos[id].valor);
                break;
            case 3:
                printf("Nova turma: ");
                fgets(alunos[id].turma, 50, stdin);
                alunos[id].turma[strcspn(alunos[id].turma, "\n")] = 0;
                break;
            case 4:
                printf("Nova data de pagamento (DD/MM/AAAA): ");
                fgets(alunos[id].data_pagamento, 11, stdin);
                alunos[id].data_pagamento[strcspn(alunos[id].data_pagamento, "\n")] = 0;
                break;
            case 5:
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 5);

    salvarAlunos(alunos, count);
    printf("Aluno atualizado com sucesso!\n");
}

// Exclui um aluno
void excluirAluno() {
    Aluno alunos[MAX];
    int count = carregarAlunos(alunos);
    if (count == 0) {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }

    listarAlunos();
    int id;
    printf("Digite o ID do aluno que deseja excluir: ");
    scanf("%d", &id);
    if (id < 1 || id > count) {
        printf("ID invalido.\n");
        return;
    }

    for (int i = id - 1; i < count - 1; i++) {
        alunos[i] = alunos[i + 1];
    }

    salvarAlunos(alunos, count - 1);
    printf("Aluno excluido com sucesso!\n");
}

// Menu de opcoes de aluno
void menuAlunos() {
    int opcao;
    do {
        printf("\n--- Menu Alunos ---\n");
        printf("1. Adicionar novo aluno\n");
        printf("2. Listar alunos\n");
        printf("3. Editar aluno\n");
        printf("4. Excluir aluno\n");
        printf("5. Voltar\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: adicionarAluno(); break;
            case 2: listarAlunos(); break;
            case 3: editarAluno(); break;
            case 4: excluirAluno(); break;
            case 5: break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 5);
}

// Menu principal
int main() {
    int opcao;
    do {
        printf("\n--- Menu Principal ---\n");
        printf("1. Alunos\n");
        printf("2. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: menuAlunos(); break;
            case 2: printf("Saindo...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 2);

    return 0;
}