#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct {
    int eBomba;
    int estaAberta;
    int vizinhos;
} Celula;

Celula jogo[10][10];
int tam = 10;

void inicializarJogo() {
    int linha, coluna;
    for (linha = 0; linha < tam; linha++) {
        for (coluna = 0; coluna < tam; coluna++) {
            jogo[linha][coluna].eBomba = 0;
            jogo[linha][coluna].estaAberta = 0;
            jogo[linha][coluna].vizinhos = 0;
        }
    }
}

void sortearBombas(int n) {
    int i;
    srand(time(NULL));
    for (i = 1; i <= n; i++) {
        int linha = rand() % tam;
        int coluna = rand() % tam;
        if (jogo[linha][coluna].eBomba == 0)
            jogo[linha][coluna].eBomba = 1;
        else
            i--;
    }
}

int coordenadaEhValida(int linha, int coluna) {
    return (linha >= 0 && linha < tam && coluna >= 0 && coluna < tam);
}

int quantidadeBombasVizinhas(int linha, int coluna) {
    int quantidade = 0;
    int direcoes[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},         {0, 1},
        {1, -1}, {1, 0}, {1, 1}
    };
    for (int i = 0; i < 8; i++) {
        int novaLinha = linha + direcoes[i][0];
        int novaColuna = coluna + direcoes[i][1];
        if (coordenadaEhValida(novaLinha, novaColuna) && jogo[novaLinha][novaColuna].eBomba) {
            quantidade++;
        }
    }

    return quantidade;
}

void contarBombas() {
    int linha, coluna;
    for (linha = 0; linha < tam; linha++) {
        for (coluna = 0; coluna < tam; coluna++)
            jogo[linha][coluna].vizinhos = quantidadeBombasVizinhas(linha, coluna);
    }
}

void imprimir() {
    printf("\n\n\t   ");
    int linha, coluna;
    for (linha = 0; linha < tam; linha++)
        printf("  %d ", linha);
    printf("\n\n\t   -----------------------------------------\n");

    for (linha = 0; linha < tam; linha++) {
        printf("\t%d  |", linha);
        for (coluna = 0; coluna < tam; coluna++) {
            if (jogo[linha][coluna].estaAberta) {
                if (jogo[linha][coluna].eBomba)
                    printf(" * ");
                else
                    printf(" %d ", jogo[linha][coluna].vizinhos);
            } else
                printf("   ");
            printf("|");
        }
        printf("\n\t   -----------------------------------------\n");
    }
}

void abrirCelula(int linha, int coluna) {
    if (coordenadaEhValida(linha, coluna) && !jogo[linha][coluna].estaAberta) {
        jogo[linha][coluna].estaAberta = 1;
        if (jogo[linha][coluna].vizinhos == 0) {
            abrirCelula(linha - 1, coluna);
            abrirCelula(linha + 1, coluna);
            abrirCelula(linha, coluna + 1);
            abrirCelula(linha, coluna - 1);
            abrirCelula(linha - 1, coluna - 1);
            abrirCelula(linha - 1, coluna + 1);
            abrirCelula(linha + 1, coluna - 1);
            abrirCelula(linha + 1, coluna + 1);
        }
    }
}

int ganhou() {
    int quantidade = 0;
    int linha, coluna;
    for (linha = 0; linha < tam; linha++) {
        for (coluna = 0; coluna < tam; coluna++) {
            if (!jogo[linha][coluna].estaAberta && !jogo[linha][coluna].eBomba)
                quantidade++;
        }
    }
    return quantidade;
}

int pontuacao(int dificuldade) {
    int pontos = 0;
    int linha, coluna;
    for (linha = 0; linha < tam; linha++) {
        for (coluna = 0; coluna < tam; coluna++) {
            if (jogo[linha][coluna].estaAberta && !jogo[linha][coluna].eBomba)
                pontos++;
        }
    }
    return pontos * dificuldade * 100;
}



void exibirLista(int *lista, int tamanho) {
    int melhorPontuacao = 0;
    for (int i = 0; i < tamanho; i++) {
        printf("%d\n", lista[i]);
        if (lista[i] > melhorPontuacao)
            melhorPontuacao = lista[i];
    }
    if (melhorPontuacao != 0)
        printf("\n\n\t\tMELHOR PONTUACAO: %d\n", melhorPontuacao);
}

int* salvarPontuacao(int pontuacao) {
    static int ultimosPontos[10];
    static int indicePontos = 0;

    if (indicePontos < 10 && pontuacao != 0) {
        ultimosPontos[indicePontos++] = pontuacao;
    }

    return ultimosPontos;
}

void lerFase(int fase, int *dificuldade, int *numBombas) {
    char nomeArquivo[50];
    sprintf(nomeArquivo, "Desktop/jogoAA/fases/fase%d.txt", fase); // Corrigir o caminho do arquivo
    //printf("Tentando abrir o arquivo: %s\n", nomeArquivo);
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo != NULL) {
        //printf("Arquivo aberto com sucesso!\n"); 
        fscanf(arquivo, "dificuldade=%d\n", dificuldade);
        //printf("Dificuldade lida do arquivo: %d\n", *dificuldade); 
        fscanf(arquivo, "numBombas=%d\n", numBombas);
        fclose(arquivo);
    } else {
        printf("\n\n\t\t\tFase nao encontrada!\n");
    }
}

void jogar(int nivel) {
    int linhas, colunas;
    int dificuldade;
    int numBombas;

    if (nivel <= 5) {
        lerFase(nivel, &dificuldade, &numBombas); // Valores lidos do arquivo são armazenados em dificuldade e numBombas
    } else {
        dificuldade = nivel * 0.5;
        numBombas = 10 + (nivel - 1) * 2;
    }

    inicializarJogo();
    sortearBombas(numBombas); // Usa o valor de numBombas lido do arquivo
    contarBombas();

    do {
        printf("\n\n\t\t\tPONTUACAO: %d\n", pontuacao(dificuldade)); // Usa o valor de dificuldade lido do arquivo
        imprimir();
        do {
            printf("\nDigite as coordenadas de linha e coluna: ");
            scanf("%d%d", &linhas, &colunas);
            if (!coordenadaEhValida(linhas, colunas))
                printf("\nCoordenada invalida!");
        } while (!coordenadaEhValida(linhas, colunas) || jogo[linhas][colunas].estaAberta);

        abrirCelula(linhas, colunas);
    } while (ganhou() != 0 && !jogo[linhas][colunas].eBomba);

    if (jogo[linhas][colunas].eBomba) {
        printf("\n\tQue pena! Voce perdeu!\n");
    } else {
        printf("\n\tPARABENS VOCE GANHOU!\n");
    }
    printf("\n\n\t\t\tPONTUACAO FINAL: %d\n", pontuacao(dificuldade)); // Usa o valor de dificuldade lido do arquivo
    salvarPontuacao(pontuacao(dificuldade)); // Usa o valor de dificuldade lido do arquivo
}

int main() {
    int numBombas;
    int opcao;
    int iniciarJogo;
    int nivel;
    int dificuldade;

    do {
        printf("\n\n\n        BEM VINDO AO CAMPO MINADO!\n\n\n");
        printf("      1.INICIAR JOGO          2.ESTATÍSTICAS          3.SAIR\n\n");
        printf("Digite 1 ou 2 ou 3: ");
        scanf("%d", &iniciarJogo);

        if (iniciarJogo == 1) {
            nivel = 1; // Inicializa o nível aqui
            do {
                if (nivel <= 5) {
                    lerFase(nivel, &dificuldade, &numBombas); // Valores lidos do arquivo são armazenados em dificuldade e numBombas
                } else {
                    dificuldade = nivel * 0.5;
                    numBombas = 10 + (nivel - 1) * 2;
                }
                inicializarJogo();
                sortearBombas(numBombas); // Usa o valor de numBombas lido do arquivo
                contarBombas();
                printf("\n\t\t\tCAMPO MINADO\n");
                jogar(nivel); // Usa os valores de dificuldade e numBombas lidos do arquivo

                printf("\nDigite 1 para continuar jogando e 2 para voltar ao menu principal: ");
                scanf("%d", &opcao);
                if (opcao == 1 && ganhou() == 0) {
                    nivel++;
                    printf("\n\n\t\t\tPROXIMA FASE\n");
                }
            } while (opcao == 1);
        } else if (iniciarJogo == 2) {
            printf("\n\n\t\t\tESTATISTICAS\n");
            printf("\n\n\t\t\tPONTUACOES\n");
            exibirLista(salvarPontuacao(0), 10);
        }
    } while (iniciarJogo != 3);

    return 0;
}