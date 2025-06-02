# 🔁 Projeto de Grafos: Melhor Passe em uma Partida de Futebol

Este projeto simula a tomada de decisão de um jogador de futebol utilizando **grafos ponderados e algoritmo de busca em profundidade (DFS)**. A ideia é calcular o melhor caminho até o gol a partir da posição de um jogador com a posse da bola, considerando fatores táticos realistas como:

- Distância entre os jogadores
- Marcação dos adversários
- Direção do passe em relação ao gol
- Finalizações de curta distância (com bônus)

---

## 🎯 Objetivo

Aplicar a teoria de grafos em um contexto prático e realista, utilizando:

- **Matriz de adjacência ponderada**
- **Busca em profundidade (DFS) com profundidade máxima**
- Heurísticas baseadas em futebol para calcular o **score** de cada passe

---

## 🧠 Como funciona

1. Cada jogador é um **nó** do grafo.
2. Cada passe possível é uma **aresta dirigida e ponderada**.
3. O algoritmo de **DFS** explora todas as sequências de passes (limitadas a no máximo 3 passes).
4. A cada jogada, calcula-se o **score médio da sequência**, aplicando:
   - Bônus por chute próximo do gol
   - Penalidade implícita ao dividir o total pelo número de jogadas

---

## 📊 Fórmula de Score

Cada passe é avaliado com base na fórmula:

score = w1 * (1 / distância * 5) + w2 * distância_do_marcador + w3 * (1 + cosseno_do_ângulo_com_o_gol)

Você pode ajustar os pesos `w1`, `w2` e `w3` para testar diferentes comportamentos táticos.

---

## 🏁 Exemplo de saída
```
Melhor caminho até o gol (score final: 27.82):
Jogador 1 → Jogador 3 → Jogador 4 → Gol
```
---

## 📂 Estrutura do projeto

- `main.c` – Código-fonte principal com estrutura de grafo, cálculo de scores e DFS
- `README.md` – Este arquivo com instruções e explicações

---

## 🧪 Como compilar e executar

```bash
gcc main.c -o futebol -lm
./futebol
```
O uso de -lm é necessário para incluir a biblioteca matemática.

✅ Requisitos atendidos
Este projeto foi desenvolvido com base em uma proposta acadêmica que exigia:

Modelagem de um problema real com grafos

Implementação em linguagem C

Uso de matriz de adjacência

Algoritmo de busca (DFS)

Apresentação do melhor caminho com base em heurísticas

Todos os itens foram contemplados e testados.

✍️ Autor
Projeto desenvolvido por Lucas Borille disciplina de Estrutura de Dados Aplicadas – 2025.

📄 Licença
Este projeto está sob a licença MIT. Sinta-se à vontade para estudar, modificar e utilizar.
