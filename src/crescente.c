/*
 * crescente.c - Paciência 'crescente'
 *
 * IMPORTANTE! Este código fonte pode perder sua formatação caso não seja
 * aberto em um ambiente onde a largura da tabulação seja de 4 (espaços).
 *
 * Autor: kac14 - Kaio Augusto de Camargo (GRR20143061)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "crescente.h" //Biblioteca defines, estruturas de dados e prototipos.

//Preenche o baralho. Pode usar mais de um baralho (de 52 cartas). Ex: Um baralho de 104, 156, 208, ... cartas.
void fillBaralho(Carta baralho[], int size) {
    int i, j, numOfBaralhos = size/52;
    for(i = 0; i < 52; i++) {
        for(j = 0; j < size/52; j++) {
            baralho[numOfBaralhos*i + j].valor = i/4 + 1;
            baralho[numOfBaralhos*i + j].naipe = i%4;
        }
    }
}

//Embaralha o baralho (devidamente preenchido) aleatoriamente
void shuffleBaralho(Carta baralho[], int size) {
    int i, rand0, rand1;
	Carta tmp;
	for(i = 0; i < 3000; i++) {
		rand0 = rand() % size;
		do {
			rand1 = rand() % size;
		} while(rand0 == rand1);
		tmp = baralho[rand0];
		baralho[rand0] = baralho[rand1];
		baralho[rand1] = tmp;
	}
}

// Retorna a carta no topo do baralho (retirando-a do mesmo).
Carta drawCard(Carta baralho[], int *size) {
	int i, pSize;
	Carta sacada;
	if(*size > 0) {
		pSize = *size = *size - 1;
		sacada = baralho[0];
		for(i = 0; i < pSize; i++) {
			baralho[i] = baralho[i+1];
		}
	} else {
		fputs("Erro fatal: O programa tentou manipular um bloco de cartas de maneira ilegal na funcao drawCard.\n", stderr);
		exit(1);
	}
	return sacada;
}	

//Adiciona uma carta ao topo do baralho
void addCard(Carta baralho[], int *size, Carta card) {
	int i;
	*size = *size + 1;
	for(i = *size; i > 0; i--) {
		baralho[i] = baralho[i-1];
	}
	baralho[0] = card;
}

// Move a carta mais ao topo do bloco origem[] para o topo do bloco destino[], alterando os tamanhos automaticamente.
void moveCarta(Carta origem[], int *oSize, Carta destino[], int *dSize) {
	addCard(destino, dSize, drawCard(origem, oSize));
}

// Move cartas entre os blocos perifericos. averiguando se o movimento eh de fato valido.
void moveCartaBP(Carta origem[], int *oSize, Carta destino[], int *dSize) {
	if(*oSize < 1) {
		fputs("Erro: Não há carta para ser movida.\n", stderr);
		return;
	}
	if(origem[0].valor + 1 == destino[0].valor || origem[0].valor == destino[0].valor + 1) {
		if(origem[0].naipe == destino[0].naipe) {
			moveCarta(origem, oSize, destino, dSize);
			fputs("OK. Movimento válido.\n", stdout);
		} else {
			fputs("Operacao nao permitida: Os naipes nao coincidem.\n", stderr);
		}
	} else {
		fputs("Operacao nao permitida: As cartas nao estao em sequencia.\n", stderr);
	}
}

// Move cartas para os blocos centrais, averiguando se o movimento eh de fato valido.
// "crecesnte" eh um flag. Caso esteja em 1, o bloco de destino sera considerado como um bloco central crescente.
void moveCartaBC(Carta origem[], int *oSize, Carta destino[], int *dSize, int crescente) {
	if(*oSize < 1) {
		fputs("Erro: Não há carta para ser movida.\n", stderr);
		return;
	}
	if(origem[0].naipe == destino[0].naipe) {
		if((crescente && (origem[0].valor == destino[0].valor + 1)) || 
			(!crescente && (origem[0].valor + 1 == destino[0].valor))) 
		{
			moveCarta(origem, oSize, destino, dSize);
			fputs("OK: Movimento válido.\n", stdout);
		} else {
			fputs("Operacao invalida: As cartas nao estao em sequencia.\n", stderr);
		}
	} else {
		fputs("Operacao invalida: Os naipes nao coincidem.\n", stderr);
	}
}

/* 
 * Distribui as cartas do baralho para as suas respectivas estruturas.
 * bp = blocoPeriferico
 * bd = blocoDecrescente
 * bc = blocoCrescente
 */
void setCards(Carta baralho[], Carta bp[][NCARTAS], Carta bd[][NCARTAS], Carta bc[][NCARTAS], int *baralhoSize, int bpSize[], int bdSize[], int bcSize[]) {
	int i = 0;
    while(*baralhoSize > 0) {
		Carta card = drawCard(baralho, baralhoSize);
		if(card.valor == AS && bcSize[card.naipe] == 0) addCard(bc[card.naipe], &bcSize[card.naipe], card);
        else if(card.valor == REI && bdSize[card.naipe] == 0) addCard(bd[card.naipe], &bdSize[card.naipe], card);
		else {
			addCard(bp[i], &bpSize[i], card);
			if(++i >= 16) i = 0;
		}
	}	
}

// Redistribui as cartas dos blocos perifericos ("Re-embaralhamento")
void reshuffle(Carta blocos[][NCARTAS], int bsize[]) {
	Carta baralho[NCARTAS];
	int i, baralhoSize=0;
	for(i = 0; i < 16; i++) {
		while(bsize[i] > 0) {
			moveCarta(blocos[i], &bsize[i], baralho, &baralhoSize);
		}
	}
	shuffleBaralho(baralho, baralhoSize);
	i = 0;
	while(baralhoSize > 0) {
		moveCarta(baralho, &baralhoSize, blocos[i], &bsize[i]);
		if(++i >= 16) i = 0;
	}
}

//Retorna o caracter referente ao valor da carta
const char* getValor(int valor) {
	char *str = malloc(sizeof(char) * 2);
	switch(valor) {
		case AS:
			return "A"; //As
		case 10:
			return "10"; //10
		case VALETE:
			return "J"; //Valete
		case DAMA:
			return "Q"; //Dama
		case REI:
			return "K"; //Rei
		default:
			str[0] = valor + 0x30;
			str[1] = '\0';
			return str; 
	}
}



//Retorna o caracter referente ao naipe da carta
char getNaipe(int naipe) {
	switch(naipe) {
		case 0:
			return 'P'; //Paus
		case 1:
			return 'C'; //Copas
		case 2:
			return 'E'; //Espada
		case 3:
			return 'O'; //Ouro
		default:
			return '?'; //DEBUG: O programa nao deve chegar aqui
	}
}

// Imprime uma carta com a devida formatacao (Valor-Naipe).
void printCard(Carta card) {
	printf("%2s-%c", getValor(card.valor), getNaipe(card.naipe));
}

//Imprime a mesa como um todo
void printMesa(Carta bp[][NCARTAS], Carta bd[][NCARTAS], Carta bc[][NCARTAS], int bpSize[], int bdSize[], int bcSize[]) {
	int i, j;

    //Indices - Blocos perifericos superiores
	printf("%13s", ""); //Espacos vazios
    for(i = 4; i < 12; i++) {
        printf("[%2d]%2s", i+1, "");
    }

    //Cartas - Blocos perifericos superiores
    printf("\n%13s", "");
	for(i = 4; i < 12; i++) { //Blocos de indice 4 ate 11
        if(bpSize[i] > 0) printCard(bp[i][0]);
		else printf("%2s", "");
		printf("%2s", "");
	}
	printf("\n");

    // Parte inferior do tabuleiro
	for(i = 0; i < 4; i++) {
        // Cartas dos blocos perifericos (1-4)
		printf("[%2d] ", 4-i);
        printCard(bp[3-i][0]);
		printf("%4s", ""); // Espacos em branco
		switch(i) {
			case 0:
			case 3:
				printf("%48s", ""); //Espacos em branco
				break;
			case 1:
				for(j = 0; j < 4; j++) { //Imprimindo as cartas do bloco decrescente
					printf("[%2d]%2s", j+1, ""); 
                    printCard(bd[j][0]);
					printf("%2s", "");
				}
				break;
			case 2:
				for(j = 0; j < 4; j++) { //Imprimindo as cartas do bloco crescente
					printf("[%2d]%2s", j+1, ""); 
                    printCard(bc[j][0]);
					printf("%2s", "");
				}
		}
		printf("%4s", ""); //Espacos em branco
		printCard(bp[12+i][0]);
		printf(" [%2d]\n", i+13);	
	}
}

//Com base no numero de cartas nos blocos perifericos, diz se o jogo ja acabou.
int isGameRunning(int sizeBP[], int status) {
    int i;
    if(status == LOST) return 0;
    for(i = 0; i < 16; i++) {
        if(sizeBP[i] > 0) return 1;
    }
    return 0;
}

int jogar() {
	/*
	 * Variaveis:
	 * baralho - Estrutura temporaria utilizada pra embaralhar e distrubuir as cartas.
     * running - Informa se o jogo ainda esta correndo
     * reshuffles - Quantidade de embaralhamentos restantes
     * status - Status do jogo, por padrao, WON.
	 */
	Carta baralho[NCARTAS], blocoPeriferico[16][NCARTAS], blocoDecrescente[4][NCARTAS], blocoCrescente[4][NCARTAS];
	int sizeBaralho = NCARTAS, sizeBP[16], sizeBD[4], sizeBC[4], i, running = 1, reshuffles=5, status=WON;
	for(i = 0; i < 4; i++) {
		sizeBP[i] = 0;
		sizeBD[i] = 0;
		sizeBC[i] = 0;
	}
	for(i = 4; i < 16; i++) sizeBP[i] = 0;
	fillBaralho(baralho, sizeBaralho);
	shuffleBaralho(baralho, sizeBaralho);
	setCards(baralho, blocoPeriferico, blocoDecrescente, blocoCrescente, &sizeBaralho, sizeBP, sizeBD, sizeBC);
	do {
		int cmd, arg[3];
		printMesa(blocoPeriferico, blocoDecrescente, blocoCrescente, sizeBP, sizeBD, sizeBC);
		printf("\n* Embaralhamentos restantes: %d\nO que deseja fazer? 1 - Movimento | 2 - Embaralhar | 3 - Desistir\nOpcao: ", reshuffles);
		scanf("%d", &cmd);
		switch(cmd) {
			case 1: //Movimento
				printf("Insira o indice do bloco de origem [1-16]: ");
				scanf("%d", &arg[0]);
				if(arg[0] >= 1 && arg[0] <= 16) arg[0]--;
				else {
					fputs("Entrada invalida: Indice inexistente.\n", stderr);
					break;
				}
				printf("Qual o destino da carta? 1 - Bloco periferico | 2 - Bloco central crescente | 3 - Bloco central decrescente\nOpcao: ");
				scanf("%d", &arg[1]);
				printf("Insira o indice do bloco de destino ");
				if(arg[1] == 1) {
					printf("[1-16]: ");
					scanf("%d", &arg[2]);
					if(arg[2] >= 1 && arg[2] <= 16) arg[2]--;
					else {
						fputs("Entrada invalida: Indice inexistente.\n", stderr);
						break;
					}
					if(arg[2] == arg[0]) break; //Movimentando para o mesmo bloco.
					moveCartaBP(blocoPeriferico[arg[0]], &sizeBP[arg[0]], blocoPeriferico[arg[2]], &sizeBP[arg[2]]);
				} else if(arg[1] == 2) { //Crescente
					printf("[1-4]: ");
					scanf("%d", &arg[2]);
					if(arg[2] >= 1 && arg[2] <= 4) arg[2]--;
					else {
						fputs("Entrada invalida: Indice inexistente.\n", stderr);
						break;
					}
					moveCartaBC(blocoPeriferico[arg[0]], &sizeBP[arg[0]], blocoCrescente[arg[2]], &sizeBC[arg[2]],
									CRESCENTE);
				} else if(arg[1] == 3) { //Decrescente
					printf("[1-4]: ");
					scanf("%d", &arg[2]);
					if(arg[2] >= 1 && arg[2] <= 4) arg[2]--;
					else {
						fputs("Entrada invalida: Indice inexistente.\n", stderr);
						break;
					}
					moveCartaBC(blocoPeriferico[arg[0]], &sizeBP[arg[0]], blocoDecrescente[arg[2]], &sizeBD[arg[2]],
									DECRESCENTE);
				} else fputs("Entrada invalida: Opcao inexistente.\n", stderr);
				break;

			case 2: //Embaralha
				if(reshuffles > 0) { 
					reshuffle(blocoPeriferico, sizeBP);
					reshuffles--;
				}
				else fputs("Nao ha reembaralhamentos restantes.\n", stderr);
				break;

			case 3: //Desistencia
				running = 0;
				status = LOST;
				break;
			default:
				fputs("Entrada invalida: Opcao inexistente.\n", stderr);
		}
		// system("cls");
		printf("\n\n\n\n");
        running = isGameRunning(sizeBP, status);
	} while(running);
	return status;
}

int isBestTime(int actualTime, int candidate) {
    if(actualTime == -1 || candidate < actualTime) return 1;
    return 0;
}

int main () {
    unsigned int bestTime = 0, interval;
    char select;
    int maxTime, res;
	srand(time(NULL));
    do {
        printf("Selecione uma opção: [m] Melhor tempo [c] Contra o relogio [s] Sair.\n> Opção: ");
        scanf("%c", &select);
        printf("\n\n\n");
        switch (select) {
            case 'm': 
            case 'M':
                interval = time(NULL);
                res = jogar();
                interval = time(NULL) - interval;
                if(res == WON) {
                    printf("* Parabens, voce ganhou em %u minutos e %u segundos.\n", interval/60, interval%60);
                    if(isBestTime(bestTime, interval)) {
                        bestTime = interval;
                        printf("* Esse foi o melhor tempo dessa execução até o momento.\n");
                    }
                } else printf("* Voce perdeu. Tempo gasto: %u minutos e %u segundos.\n", interval/60, interval%60);
                break;
            case 'c':
            case 'C': 
                printf("Insira o tempo maximo do jogo, em minutos: ");
                scanf("%d", &maxTime);
                printf("\n\n\n\n");
                maxTime *= 60;
                interval = time(NULL);
                res = jogar();
                interval = time(NULL) - interval;
                if(res == WON && interval <= maxTime) {
                    printf("* Voce ganhou em %u minutos e %u segundos.\n", interval/60, interval%60);
                    if(isBestTime(bestTime, interval)) {
                        bestTime = interval;
                        printf("* Esse foi o melhor tempo dessa execução até o momento.\n");
                    }
                } else printf("* Voce perdeu. Tempo gasto: %u minutos e %u segundos.\n", interval/60, interval%60);
            default:
                break;
        }
        while(getchar() != '\n');
        printf("\n\n\n\n");
    } while (select != 's' || select != 'S');
	if(bestTime != 0) printf("* Melhor tempo da seção: %u minutos e %u segundos.\n", interval/60, interval%60);
    return 0;
}
