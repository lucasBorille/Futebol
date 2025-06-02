#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    float x;
    float y;
} Jogador;

float distancia(Jogador a, Jogador b) {
    return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

float distanciaMarcador(Jogador a, Jogador time2[]) {
    float menor_distancia = distancia(a, time2[0]);
    for (int i = 1; i < 5; i++) {
        float d = distancia(a, time2[i]);
        if (d < menor_distancia) {
            menor_distancia = d;
        }
    }
    return menor_distancia;
}

float cosseno_angulo(Jogador a, Jogador b, Jogador gol){
    float vx1 = b.x - a.x;
    float vy1 = b.y - a.y;
    float vx2 = gol.x - a.x;
    float vy2 = gol.y - a.y;

    float vx = vx1 * vx2 + vy1 * vy2; // Produto escalar
    float vy = sqrtf(vx1*vx1 + vy1*vy1) * sqrtf(vx2*vx2 + vy2*vy2); // Módulos dos vetores
    if (vy == 0) return 0; // Evita divisão por zero
    
    return vx / vy; // Cosseno do ângulo
}


float calcular_passe(Jogador a, Jogador b, float w1, float w2, float w3,Jogador gol, Jogador time2[]) {
    float d = distancia(a, b);
    float d_marcador = distanciaMarcador(b, time2);
    float c = cosseno_angulo(a, b, gol);

    float score =  w1*(1.0/d*10) + w2*d_marcador + w3*(1+c);
    return score;
}

int main() {
    Jogador time1[5] = {
        {2, 2},
        {5,5},
        {7,0},
        {9,4},
        {12,3} //Gol adversário
    };

    Jogador time2[5] = {
        {6,6},
        {10,4.8},
        {10,1},
        {4,2.5},
        {11.5,3} //Goleiro
    };

    float w1 = 5.0; // Peso da distância entre jogadores
    float w2 = 1.5; // Peso da distância do marcador
    float w3 = 1.5; // Peso do cosseno do ângulo
    
    Jogador jogadorPosse = time1[0]; // Jogador com a posse de bola

    float passes[5][5]; // Matriz para armazenar os scores de passe
    int i,j;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            if (i != j) { // Evita que o jogador passe para si mesmo
                passes[i][j] = calcular_passe(time1[i], time1[j], w1, w2, w3,time1[4], time2);
            } else {
                passes[i][j] = -1; // Indica que não é possível passar para si mesmo
            }
            //printf("Score de passe de Jogador %d para Jogador %d: %.2f\n", i + 1, j + 1, passes[i][j]);
        }
        //printf("\n");   
    }
    printf("          | Jogador 1 | Jogador 2 | Jogador 3 | Jogador 4 |    Gol    |\n");
    printf("----------|-----------|-----------|-----------|-----------|-----------|\n");
    for (i = 0; i < 4; i++)
    {  
        printf("Jogador %d | ", i + 1);
        for(j = 0; j < 5; j++)
        {
            printf("%9.2f | ", passes[i][j]);
        }
        printf("\n");
    }
    

}