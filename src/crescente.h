// Valor das cartas
#define AS 1
#define VALETE 11
#define DAMA 12
#define REI 13

//Numero maximo de cartas
#define NCARTAS 104

//Flag: crescente
#define CRESCENTE 1
#define DECRESCENTE 0

//Flag: Status
#define WON 1
#define LOST 2

//Estrutura para as cartas
typedef struct carta {
	int valor;
	int naipe;
} Carta;

//Prototipos
void fillBaralho(Carta baralho[], int size);

void shuffleBaralho(Carta baralho[], int size);

Carta drawCard(Carta baralho[], int *size);

void addCard(Carta baralho[], int *size, Carta card);

void moveCarta(Carta origem[], int *oSize, Carta destino[], int *dSize);

void moveCartaBP(Carta origem[], int *oSize, Carta destino[], int *dSize);

void moveCartaBC(Carta origem[], int *oSize, Carta destino[], int *dSize, int crescente);

void setCards(Carta baralho[], Carta bp[][NCARTAS], Carta bd[][NCARTAS], Carta bc[][NCARTAS], int *baralhoSize, int bpSize[], int bdSize[], int bcSize[]);

void reshuffle(Carta blocos[][NCARTAS], int bsize[]);

const char* getValor(int valor);

char getNaipe(int naipe);

void printCard(Carta card);

void printMesa(Carta bp[][NCARTAS], Carta bd[][NCARTAS], Carta bc[][NCARTAS], int bpSize[], int bdSize[], int bcSize[]);

int isGameRunning(int sizeBP[], int status);

int jogar();

int isBestTime(int actualTime, int candidate);
