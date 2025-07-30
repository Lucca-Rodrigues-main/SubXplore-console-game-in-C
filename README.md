# SubXplore-console-game-in-C
SubXplore is a maritime adventure game where players navigate a submarine to reach the wrecked ship NTN, avoiding debris and collecting ETAs for points. Move with WASD keys, dodge obstacles, and survive the dynamic map. Win by reaching NTN or lose by collisions/out-of-bounds. Features: randomized map updates, score tracking, and creative UI.

## Sobre o Jogo

SubXplore é um jogo baseado em um mapa marítimo onde o jogador controla um submarino com o objetivo de alcançar o cargueiro naufragado Netuno (NTN), que contém equipamentos de alta tecnologia (ETAs). O jogo combina navegação estratégica com elementos de coleta e evitação de obstáculos.

## Como Funciona

- O jogo ocorre em um mapa 10x10 representado por uma matriz.
- O submarino é movimentado usando as teclas W (cima), S (baixo), A (esquerda) e D (direita).
- A cada movimento, o mapa é atualizado, fazendo com que os obstáculos (destroços), ETAs e o NTN se movimentem aleatoriamente.
- O jogador deve evitar colisões com destroços e sair dos limites do mapa.

## Regras do Jogo

1. **Objetivo Principal**: Chegar ao NTN para ganhar 50 pontos e vencer o jogo.
2. **Pontuação Adicional**: Resgatar ETAs (10 pontos cada), sem repetições.
3. **Perder o Jogo**: Colidir com destroços ou sair do mapa.
4. **Movimentação**: O submarino se move uma posição por tecla pressionada.
5. **Atualização do Mapa**: Após cada movimento, os elementos do mapa se reposicionam.

## Implementação

- Desenvolvido em C, utilizando funções como `geraMapa` e `atualizaMapa` para gerenciar o mapa.
- Validação de entradas do usuário e feedback claro durante o jogo.
- Interface criativa com cores e caracteres especiais para melhor visualização.

<img width="1185" height="911" alt="image" src="https://github.com/user-attachments/assets/b931915a-3497-4827-9be6-8f6b9e565d02" />
