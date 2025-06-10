# 🔁 Projeto de Grafos: Melhor Passe em uma Partida de Futebol

Este projeto simula a tomada de decisão de um jogador de futebol utilizando **grafos ponderados** e o algoritmo de **busca em profundidade (DFS)**. Ele calcula a melhor rota até o gol com base em passes táticos, levando em conta a posição dos adversários e o alinhamento dos jogadores com o gol.

---

## 🎮 Funcionalidades

- Renderização gráfica do campo e dos jogadores com **SDL2**
- Animação da bola seguindo o melhor caminho encontrado
- Três jogadas pré-definidas que podem ser alternadas com as teclas `1`, `2` e `3`
- A bola **sempre começa com o jogador mais à esquerda**
- Pressione `Espaço` para simular a jogada
- Pressione `R` para resetar a jogada
- O **gol está centralizado visualmente** na área
- Log da melhor sequência de passes exibido no terminal

---

## 🎯 Objetivo

Aplicar a teoria de grafos em um contexto prático e visual, utilizando:

- **Matriz de adjacência ponderada**
- **Busca em profundidade (DFS)** com profundidade máxima
- Heurísticas baseadas em futebol para calcular o **score** de cada passe

---

## 📊 Fórmula de Score

Cada passe é avaliado com base na fórmula:

```
score = w1 * (1 / distância * 5) + w2 * distância_do_marcador + w3 * (1 + cosseno_do_ângulo_com_o_gol)
```

Você pode ajustar os pesos `w1`, `w2`, `w3` no código para testar diferentes estratégias táticas.

---

## 🧠 Como funciona

1. Cada jogador do time é um **nó** no grafo.
2. Cada passe possível entre jogadores é uma **aresta ponderada**.
3. O algoritmo de **DFS** explora as combinações de até 3 passes.
4. A cada jogada:
   - O score acumulado é dividido pelo número de passes
   - Bônus extra é aplicado se o último passe for para uma finalização próxima ao gol

---

## 🧪 Como compilar e executar

### ✅ Pré-requisitos

Instale as bibliotecas SDL2 e SDL2_image:

**macOS (Homebrew):**

```bash
brew install sdl2 sdl2_image
```

**Ubuntu/Debian:**

```bash
sudo apt-get install libsdl2-dev libsdl2-image-dev
```

---

### 🔧 Compilação

**macOS:**

```bash
gcc main.c -o futebol \
  -I/opt/homebrew/include/SDL2 \
  -L/opt/homebrew/lib \
  -lSDL2 -lSDL2_image -lm
```

**Linux (Ubuntu):**

```bash
gcc main.c -o futebol -lSDL2 -lSDL2_image -lm
```

---

### ▶️ Execução

```bash
./futebol
```

---

## 🏁 Exemplo de saída

No terminal, ao pressionar `Espaço` para iniciar a jogada, será exibido:

```
Melhor caminho até o gol (score final: 27.82):
Jogador 0 → Jogador 2 → Jogador 4 (Gol)
```

---

## 📁 Estrutura esperada

```
.
├── main.c
├── timeA.png
├── timeB.png
├── bola.png
└── README.md
```

---

## ✅ Requisitos Acadêmicos Atendidos

- [x] Modelagem de problema real com grafos
- [x] Implementação em linguagem C
- [x] Uso de matriz de adjacência
- [x] Algoritmo de busca (DFS)
- [x] Apresentação visual com SDL
- [x] Animação interativa e controle por teclado
- [x] Impressão da melhor jogada no terminal

---

## ✍️ Autores

Projeto desenvolvido por:

- Lucas Borille  
- Talysson Decol  
- Thiago Medici  
- Gabriel Riccetto  

📚 *Disciplina: Estrutura de Dados Aplicadas – 2025*

---

## 📄 Licença

Distribuído sob a licença MIT. Sinta-se à vontade para estudar, modificar e utilizar este projeto em seus próprios experimentos.
