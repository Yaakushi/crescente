Paciência 'crescente'
Desenvolvido por Kaio Augusto de Camargo (GRR20143061).

Índice desse arquivo
--------------------------------------------
-- Estrutura do Projeto
-- Compilação
-- Como jogar?
-- Algoritmos e estruturas
-- Bugs conhecidos

Estrutura do pacote
--------------------------------------------
kac14: Pasta principal do projeto
|
|- src: Pasta com os códigos fontes do projeto
|  |
|  |- crescente.c: Arquivo fonte principal.
|  |
|  |- crescente.h: Arquivo fonte contendo as definições, estruturas de dados e prototipos do projeto.
|
|- bin: Pasta onde os binários são gerados por padrão
|  |
|  |- crescente: Arquivo binário gerado pela compilação por make.
|
|
|- makefile: Arquivo para compilação por make.
|
|- LEIAME: Este arquivo.


Compilação
------------------------------------------
Este arquivo pode ser compilado por um simples comando "make" na pasta principal do projeto (kac14). A saída do make será o binário "crescente" na pasta "bin". Alternativamente, a compilação do arquivo "crescente.c" no diretório "src" também gerara o binário desejado. O comando make também suporta o argumento "clean", removendo o executável presente na pasta bin.

Como jogar?
------------------------------------------
Antes de tudo, é necessário entender como cada bloco de cartas é identificado:

      [P05] [P06] [P07] [P08] [P09] [P10] [P11] [P12]
[P04]                                                 [P13]
[P03]             [D01] [D02] [D03] [D04]             [P14]
[P02]             [C01] [C02] [C03] [C04]             [P15]
[P01]                                                 [P16]

Temos 24 blocos em uma partida, dos quais:
- 16 são blocos periféricos [P01 ~ P16]
- 4 são blocos centrais crescentes (ás -> rei) [C01~C04]
- 4 são blocos centrais decrescentes (rei -> ás) [D01~D04]

Vale notar que utilizar notações como "D01" ou "P15" como parametros para a movimentação de cartas NÃO trará o resultado esperado. Deve-se apenas utilizar o índice do bloco (No caso dos exemplos: "1" e "15", respectivamente). O bloco de origem sempre será tratado como um bloco de periferia (visto que não há como movimentar uma carta de um bloco central). O programa perguntará qual o bloco de destino (periférico, central crescente ou central decrescente) e o tratará de acordo.

Cada um dos blocos centrais crescentes começará com uma carta "ás" (cada bloco terá seu naipe), da mesma forma que cada bloco decrescente começará com um "rei" (com naipes também diferentes entre si). Os blocos periféricos receberão as cartas embaralhadas (ou seja, distribuição aleatória!). O jogador deve, utilizando-se das cartas dos blocos periféricos, completar as sequências nos 8 blocos centrais (4 sequências crescentes e 4 decrescentes).

Para tal, o jogador conta com 3 jogadas:
[1] Movimento
O jogador pode mover qualquer a primeira carta de qualquer um dos blocos periféricos para um bloco central ou para outro bloco periférico. O naipe da carta a ser movida deve ser igual ao da primeira carta do bloco de destino.
No caso de movimento para outro bloco periférico, o valor da carta a ser movida deve ser o incremento ou decremento direto em relação ao da primeira carta do bloco de origem.
No caso de movimento para um bloco central, o valor deve ser o próximo valor na sequência do bloco (levando em conta se o bloco é crescente ou decrescente).
[2] Embaralhamento
O jogador pode embaralhar e redistribuir as cartas nos blocos periféricos até 5 vezes durante o decorrer de uma partida.
[3] Desistir
O jogador pode desistir da partida a qualquer momento.

O jogo apenas se encerra quando uma das condições abaixo forem atingidas:
- O jogador desistir (derrota)
- Não haver nenhuma carta nos 16 blocos periféricos (o que implica que o jogador conseguiu mover todas as cartas para o bloco central, ou seja, vitória).

Algoritmos e estruturas
------------------------------------------
-- Estruturas:
Para representar cada carta, foi-se utilizado um struct com dois integers: Um para o valor da carta (um int entre 1 e 13) e outro para o naipe (0-3).

-- Função principal:
A função 'main' consiste basicamente de:
- Determinar o modo de jogo a ser jogado. Caso seja contra o relógio, determinar o tempo máximo.
- Obter e guardar o tempo (em segundos) desde 1970 no momento antes do início da partida.
- Chamar a função "jogar" para iniciar a partida.
- Após o fim do jogo (fim de jogar()), calcular o tempo gasto na partida (em segundos).]
- Imprimir uma mensagem de acordo com o fato do jogador ter vencido ou não a partida.
- Chamar a função "isBestTime" para determinar se o tempo gasto é o melhor tempo da seção. Caso for, guardar esse tempo.
- Voltar para o passo 1 até o usuário desejar sair do programa.
- Antes de sair do programa, informar o melhor tempo da seção (caso haja um).

-- Função jogar:
No início de cada jogo (chamada da função "jogar"), são criados:
* Uma matriz da struct "carta", 16 por NCARTAS (NCARTAS é o o numero total de cartas do baralho), representando cada um dos blocos perifericos.
* Duas matrizes da struct "carta", 4 por NCARTAS cada uma, representando os 8 blocos centrais, separando-os em "decrescentes" e "crescentes".
* Três matrizes do tipo int de 16, 4 e 4 posições respectivamente, representando a quantidade de cartas presentes nos blocos perifericos, crescentes e decrescentes.
* Um inteiro "running", que é responsável por manter o loop até o jogo acabar ou o jogador desistir.
* Um inteiro "reshuffles", responsável por guardar a quantidade de embaralhamentos que o jogador ainda possui.
* Um inteiro "status", que é utilizado para determinar se o jogador venceu (define "WON") ou perdeu o jogo (define "LOST").

Play consiste basicamente das seguintes ações:
- Preenche um "baralho" com NCARTAS.
- Embaralha o "baralho". O método de embaralhamento é trocar duas cartas aleatórias 3.000 vezes.
- Distribuir as cartas no "baralho" para os blocos perifericos, separando um "ás" de cada naipe para os blocos centrais crescentes e um "rei" de cada naipe para os blocos decrescentes.
* Repetindo até o fim do jogo (variável "running"):
  * Imprime as cartas na mesa de uma maneira organizada (função printMesa).
  * Ler um comando (Movimentar, embaralhar ou desistir).
  * Ler argumentos adicionais, no caso do movimento (Bloco de origem, bloco de destino).
  * Realizar a ação.
  * Chamar a função "isGameRunning" para averiguar se o jogo acabou (atualizar "running" com o resultado).
- Retornar "status", informando se o jogador conseguiu ou não ganhar o jogo. (Nota: Esse resultado NÃO leva em conta o tempo do modo "contra o relógio". A checagem do tempo gasto é feito na função main).

Bugs conhecidos
-------------------------------------------
- Nenhum bug conhecido até o momento.
