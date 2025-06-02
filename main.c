#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define N_JOGADORES 5 // 4 jogadores + gol
#define MAX_CAMINHO 5

typedef struct {
    float x;
    float y;
} Jogador;

Jogador gol = {12, 3}; // posição do gol

float distancia(Jogador a, Jogador b) {
    return sqrtf((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

float distanciaMarcador(Jogador jogador, Jogador marcadores[], int n) {
    float menor = FLT_MAX;
    for (int i = 0; i < n; i++) {
        float d = distancia(jogador, marcadores[i]);
        if (d < menor) menor = d;
    }
    return menor;
}

float cosseno_angulo(Jogador a, Jogador b, Jogador gol) {
    float vx1 = b.x - a.x;
    float vy1 = b.y - a.y;
    float vx2 = gol.x - a.x;
    float vy2 = gol.y - a.y;

    float dot = vx1 * vx2 + vy1 * vy2;
    float mag1 = sqrtf(vx1*vx1 + vy1*vy1);
    float mag2 = sqrtf(vx2*vx2 + vy2*vy2);

    if (mag1 == 0 || mag2 == 0) return 0;
    return dot / (mag1 * mag2);
}

float calcular_score(Jogador a, Jogador b, Jogador time2[], int n_adv, float w1, float w2, float w3) {
    float d = distancia(a, b);
    if (d < 0.01f) d = 0.01f;

    float d_marcador = distanciaMarcador(b, time2, n_adv);
    float c = cosseno_angulo(a, b, gol);
    return w1 * (1.0f / d * 5) + w2 * d_marcador + w3 * (1 + c);
}

// ------------------------ DFS ---------------------------

float melhor_score = -1;
int melhor_caminho[MAX_CAMINHO]; // no máx 4 jogadas (3 passes + gol)
int caminho_atual[MAX_CAMINHO];

void dfs(float scores[N_JOGADORES][N_JOGADORES], Jogador time1[], int atual, int profundidade, float score_acumulado, int visitados[]) {
    if (profundidade >= MAX_CAMINHO)
        return;

    caminho_atual[profundidade] = atual;
    visitados[atual] = 1;

    if (atual == 4 && profundidade > 0) { // chegou ao gol
        float score_final = score_acumulado / profundidade;

        float d_gol = distancia(time1[caminho_atual[profundidade - 1]], gol);// distância do último jogador ao gol
        if (d_gol < 5.0) { // se o último jogador está a menos de 5 metros do gol
            score_final += 3.0f; // bônus por chute de curta distância
        }

        if (score_final > melhor_score) {
            melhor_score = score_final;
            for (int i = 0; i <= profundidade; i++) {
                melhor_caminho[i] = caminho_atual[i];
            }
        }
    } else {
        for (int prox = 0; prox < N_JOGADORES; prox++) {
            if (!visitados[prox]  && scores[atual][prox] > 0) { // se o jogador não foi visitado e há um score positivo
                // Chama recursivamente para o próximo jogador
                dfs(scores, time1, prox, profundidade + 1, score_acumulado + scores[atual][prox], visitados);
            }
        }
    }

    visitados[atual] = 0;
}

// --------------------------------------------------------

int main() {
    Jogador time1[N_JOGADORES] = {
        {2, 2}, {3.5,4}, {5,0}, {10,4}, {12,3}
    };

    Jogador time2[N_JOGADORES] = {
        {4,4}, {5,1}, {8,5}, {3,2}, {11.5,3}
    };

    float w1 = 1.0, w2 = 2.0, w3 = 1.0;

    float scores[N_JOGADORES][N_JOGADORES];
    for (int i = 0; i < N_JOGADORES; i++) { 
        for (int j = 0; j < N_JOGADORES; j++) {
            if (i != j)
                scores[i][j] = calcular_score(time1[i], time1[j], time2,N_JOGADORES, w1, w2, w3);
            else
                scores[i][j] = -1;
        }
    }



    int jogador_inicial = 2; // Jogador 1
    int visitados[N_JOGADORES] = {0};

    dfs(scores, time1, jogador_inicial, 0, 0, visitados);

    // Imprime o melhor caminho
    printf("Melhor caminho até o gol (score final: %.2f):\n", melhor_score);
    for (int i = 0; i < MAX_CAMINHO; i++) {
        if (melhor_caminho[i] == 4) {
            printf("→ Gol\n");
            break;
        } else if (i == 0) {
            printf("Jogador %d", melhor_caminho[i] + 1);
        } else {
            printf(" → Jogador %d", melhor_caminho[i] + 1);
        }
    }

    return 0;
}
