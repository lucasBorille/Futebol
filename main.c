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

Jogador gol = {15, 5.5}; // posição do gol

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

        
    return w1 * (1.0f / d * 5) + w2 * d_marcador + w3 * (1 + c); // Adiciona 1 ao cosseno para não ter valores negativos
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

void carregar_jogada(int jogada, Jogador time1[], Jogador time2[]) {
    if (jogada == 1) {
        time1[0] = (Jogador){2, 2}; time1[1] = (Jogador){4,5}; time1[2] = (Jogador){6,2}; time1[3] = (Jogador){8,5}; time1[4] = gol;
        time2[0] = (Jogador){1,3}; time2[1] = (Jogador){5,4}; time2[2] = (Jogador){7,2}; time2[3] = (Jogador){10,4}; time2[4] = (Jogador){13,6};
    } else if (jogada == 2) {
        time1[0] = (Jogador){3, 5}; time1[1] = (Jogador){5,2}; time1[2] = (Jogador){6,4.5}; time1[3] = (Jogador){8,2.5}; time1[4] = gol;
        time2[0] = (Jogador){3.5,3}; time2[1] = (Jogador){5,4.5}; time2[2] = (Jogador){7,3.5}; time2[3] = (Jogador){9.5,4}; time2[4] = (Jogador){13.5,6};
    } else if (jogada == 3) {
        time1[0] = (Jogador){2.5, 4}; time1[1] = (Jogador){5,2}; time1[2] = (Jogador){6.5,5}; time1[3] = (Jogador){8.5,3}; time1[4] = gol;
        time2[0] = (Jogador){4,2.5}; time2[1] = (Jogador){6,3}; time2[2] = (Jogador){7.5,2}; time2[3] = (Jogador){10,4}; time2[4] = (Jogador){13.8,6};
    }else if (jogada == 4){
        time1[0] = (Jogador){15, 0}; time1[1] = (Jogador){11,3}; time1[2] = (Jogador){13,7}; time1[3] = (Jogador){13,5}; time1[4] = gol;
        time2[0] = (Jogador){14,3.5}; time2[1] = (Jogador){12.3,5.5}; time2[2] = (Jogador){13,6.5}; time2[3] = (Jogador){10,4}; time2[4] = (Jogador){14.6,5.5};
    }else if (jogada == 5){
        time1[0] = (Jogador){10,6.5}; time1[1] = (Jogador){11,3}; time1[2] = (Jogador){10.3,7}; time1[3] = (Jogador){13,5}; time1[4] = gol;
        time2[0] = (Jogador){12.4,7}; time2[1] = (Jogador){12.4,6}; time2[2] = (Jogador){12.4,6.5}; time2[3] = (Jogador){10,4}; time2[4] = (Jogador){14.6,5.5};
    }
}

int main() {
    Jogador time1[N_JOGADORES];

    Jogador time2[N_JOGADORES];

    printf("Digite o numero da jogada desejada:");
    int jogada;
    scanf("%d", &jogada);

    carregar_jogada(jogada, time1, time2);

    float w1 = 2.0, w2 = 2.0, w3 = 1.0;

    // Criação do Grafo de Matriz de adjacencia
    float scores[N_JOGADORES][N_JOGADORES];
    for (int i = 0; i < N_JOGADORES; i++) { 
        for (int j = 0; j < N_JOGADORES; j++) {
            if (i != j)
            //Preenche o peso da aresta com a função calcular_score
                scores[i][j] = calcular_score(time1[i], time1[j], time2,N_JOGADORES, w1, w2, w3);
            else
            // O jogador não pode passar para si mesmo, então colocamos -1
                scores[i][j] = -1;
        }
    }



    int jogador_inicial = 0; // Jogador 1
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
