#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define N_JOGADORES 5
#define MAX_CAMINHO 5
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef struct {
    float x;
    float y;
} Jogador;

Jogador gol = {15.0, 5.5};

SDL_Texture* sprite_timeA = NULL;
SDL_Texture* sprite_timeB = NULL;
SDL_Texture* sprite_bola = NULL;

float melhor_score = -1;
int melhor_caminho[MAX_CAMINHO];
int caminho_atual[MAX_CAMINHO];

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

void dfs(float scores[N_JOGADORES][N_JOGADORES], Jogador time1[], int atual, int profundidade, float score_acumulado, int visitados[]) {
    if (profundidade >= MAX_CAMINHO) return;

    caminho_atual[profundidade] = atual;
    visitados[atual] = 1;

    if (atual == 4 && profundidade > 0) {
        float score_final = score_acumulado / profundidade;
        float d_gol = distancia(time1[caminho_atual[profundidade - 1]], gol);
        if (d_gol < 5.0) score_final += 3.0f;

        if (score_final > melhor_score) {
            melhor_score = score_final;
            for (int i = 0; i <= profundidade; i++) {
                melhor_caminho[i] = caminho_atual[i];
            }
        }
    } else {
        for (int prox = 0; prox < N_JOGADORES; prox++) {
            if (!visitados[prox] && scores[atual][prox] > 0) {
                dfs(scores, time1, prox, profundidade + 1, score_acumulado + scores[atual][prox], visitados);
            }
        }
    }

    visitados[atual] = 0;
}

void render_jogador(SDL_Renderer* renderer, SDL_Texture* sprite, Jogador j) {
    SDL_Rect dest;
    dest.w = 40;
    dest.h = 40;
    dest.x = (int)(j.x * 50);
    dest.y = (int)(j.y * 50);
    SDL_RenderCopy(renderer, sprite, NULL, &dest);
}

void render_bola(SDL_Renderer* renderer, SDL_Texture* bola, Jogador origem, Jogador destino, float t) {
    SDL_Rect dest = {0, 0, 20, 20};
    float x = origem.x + (destino.x - origem.x) * t;
    float y = origem.y + (destino.y - origem.y) * t;
    dest.x = (int)(x * 50);
    dest.y = (int)(y * 50);
    SDL_RenderCopy(renderer, bola, NULL, &dest);
}

void desenhar_campo(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, 50, 50, 750, 50);
    SDL_RenderDrawLine(renderer, 50, 550, 750, 550);
    SDL_RenderDrawLine(renderer, 50, 50, 50, 550);
    SDL_RenderDrawLine(renderer, 750, 50, 750, 550);
    SDL_RenderDrawLine(renderer, 400, 50, 400, 550);

    SDL_RenderDrawLine(renderer, 725, 250, 750, 250);
    SDL_RenderDrawLine(renderer, 725, 350, 750, 350);
    SDL_RenderDrawLine(renderer, 725, 250, 725, 350);
}

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
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erro SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("DFS Futebol", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    IMG_Init(IMG_INIT_PNG);

    sprite_timeA = IMG_LoadTexture(renderer, "timeA.png");
    sprite_timeB = IMG_LoadTexture(renderer, "timeB.png");
    sprite_bola  = IMG_LoadTexture(renderer, "bola.png");

    Jogador time1[N_JOGADORES];
    Jogador time2[N_JOGADORES];

    int jogada_atual = 1;
    carregar_jogada(jogada_atual, time1, time2);

    int jogador_inicial = 0;
    float menor_x = time1[0].x;
    for (int i = 1; i < N_JOGADORES - 1; i++) {
        if (time1[i].x < menor_x) {
            menor_x = time1[i].x;
            jogador_inicial = i;
        }
    }
    jogador_inicial = 0;

    float w1 = 2.0, w2 = 2.0, w3 = 1.0;
    float scores[N_JOGADORES][N_JOGADORES];

    int visitados[N_JOGADORES] = {0};
    int running = 1;
    SDL_Event e;
    float t = 0;
    int passo = 0;
    int jogada_iniciada = 0;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = 0;
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_SPACE && !jogada_iniciada) {
                    melhor_score = -1;
                    dfs(scores, time1, jogador_inicial, 0, 0, visitados);
                    jogada_iniciada = 1;
                    passo = 0;
                    t = 0;

                    printf("Melhor caminho: ");
                    for (int i = 0; i < MAX_CAMINHO; i++) {
                        if (melhor_caminho[i] == 4) {
                            printf(" → Gol\n");
                            break;
                        } else {
                            printf("%d ", melhor_caminho[i]);
                        }
                    }
                } else if (e.key.keysym.sym == SDLK_r) {
                    jogada_iniciada = 0;
                    passo = 0;
                    t = 0;
                } else if (e.key.keysym.sym == SDLK_1 || e.key.keysym.sym == SDLK_2 || e.key.keysym.sym == SDLK_3|| e.key.keysym.sym == SDLK_4|| e.key.keysym.sym == SDLK_5 ) {
                    jogada_atual = e.key.keysym.sym - SDLK_0;
                    carregar_jogada(jogada_atual, time1, time2);

                    jogador_inicial = 0;
                    passo = 0;
                    t = 0;
                }
            }
        }

        for (int i = 0; i < N_JOGADORES; i++) {
            for (int j = 0; j < N_JOGADORES; j++) {
                if (i != j)
                    scores[i][j] = calcular_score(time1[i], time1[j], time2, N_JOGADORES, w1, w2, w3);
                else
                    scores[i][j] = -1;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255);
        SDL_RenderClear(renderer);

        desenhar_campo(renderer);

        for (int i = 0; i < N_JOGADORES - 1; i++) render_jogador(renderer, sprite_timeA, time1[i]);
        for (int i = 0; i < N_JOGADORES; i++) render_jogador(renderer, sprite_timeB, time2[i]);

        if (jogada_iniciada && passo < MAX_CAMINHO - 1) {
            if (melhor_caminho[passo + 1] == 4) {
                render_bola(renderer, sprite_bola, time1[melhor_caminho[passo]], gol, t);
                t += 0.01f;
            } else {
                render_bola(renderer, sprite_bola, time1[melhor_caminho[passo]], time1[melhor_caminho[passo + 1]], t);
                t += 0.01f;
                if (t >= 1.0f) {
                    t = 0;
                    passo++;
                }
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(sprite_timeA);
    SDL_DestroyTexture(sprite_timeB);
    SDL_DestroyTexture(sprite_bola);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
