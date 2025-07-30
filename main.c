// Defines das cores do sistema
#define preto_            0
#define azul_             1
#define verde_            2
#define ciano_            3
#define vermelho_         4
#define magenta_          5
#define marrom_           6
#define cinza_claro_      7
#define cinza_escuro_     8
#define azul_claro_       9
#define verde_claro_     10
#define ciano_claro_     11
#define vermelho_claro_  12
#define magenta_claro_   13
#define amarelo_         14
#define branco_          15

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

// Struct com as coordenadas x e y da matriz
typedef struct
{
    int lin;
    int col;
} Posicao;

// Struct com as coordenadas de cada objeto
typedef struct
{
    Posicao SUB;
    Posicao NTN;
    Posicao ETA[98];
    Posicao DTC[98];
} Locais;

// Matriz mapa
int mapa[10][10];

// Protitipos

void LimpaTela(void);
void corTexto(int texto, int fundo);

int procura(Locais loc, int obj, Posicao sub);
int movimenta(Posicao* pos);
int menu(int mapa[10][10], Locais* loc, int* pts,
         char* nome, int* cp, int* outcp);

int randomInteger (int low, int high);
void geraMapa(int mapa[10][10]);
void atualizaMapa(int mapa[10][10]);

void instrucoes(char* nome, int* cp, int* outcp);
void mostraMapa(int mapa[10][10], Locais* loc);

int main()
{
    // NTN = embarcacao cargueiro Netuno naufragado
    // ETA = equipamentos de alta tecnologia

    // Pontuacao
    int pts = 0;
    int ret;

    // Nome do jogador
    char nome[31];

    // Codepage
    int cp;
    int outcp;

    // Localidade dos objetos
    Locais locais;

    // Imprimindo as instrucoes
    instrucoes(nome, &cp, &outcp);
    LimpaTela();

    // Inicializando o mapa
    while(1)
    {
        // Iniciando ou reiniciando o jogo
        geraMapa(mapa);
        mostraMapa(mapa, &locais);
        while(1)
        {
            while((ret = menu(mapa, &locais, &pts, nome, &cp, &outcp)) == -1)
            {
                // retorno -1 significa uma escolha indevida
                LimpaTela();
                mostraMapa(mapa, &locais);
            }
            // retorno 1 ou 2 significia novo jogo ou fim de jogo
            if(ret) break;
            // Atualizacao
            //Sleep(1000);
            atualizaMapa(mapa);
            // Imprimindo novamente o mapa
            LimpaTela();
            mostraMapa(mapa, &locais);
        }
        // retorno 2 significa fim de jogo
        if(ret == 2) break;
        pts = 0;
    }

    return 0;
}

void LimpaTela(void)
{
    WORD wColor = ((preto_ & 0x0F) << 4) + (cinza_claro_ & 0x0F);
    // Obter o handle do buffer de saida
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    // Isso e usado para levar o cursor ao canto superior esquerdo
    COORD coord = {0, 0};
    // Um valor de retorno indicando quantos caracteres foram escritos e nao usados
    DWORD cont;

    // Estrutura que contem todas as informacoes da console
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    // Definindo a cor atual
    SetConsoleTextAttribute(hStdOut, wColor);
    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        // Preenchendo o buffer com espacos em branco
        FillConsoleOutputCharacter(hStdOut, (TCHAR) 32, csbi.dwSize.X * csbi.dwSize.Y, coord, &cont);

        FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord, &cont );
        // Definindo a posicao do cursor
        SetConsoleCursorPosition(hStdOut, coord);
    }
}


void corTexto(int texto, int fundo)
{
    // SetConsoleTextAttribute define os atributos de
    // caracteres gravados no buffer da tela do console
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), texto | (fundo << 4));
}

int procura(Locais loc, int obj, Posicao sub)
{
    int i;

    if(obj == 3)
    {
        // O objeto e um destroco
        for(i = 0; loc.DTC[i].lin > -1; i++)
        {
            // Verificando se a coordenada bate
            // com a coordenada do submarino
            if(loc.DTC[i].lin == sub.lin &&
               loc.DTC[i].col == sub.col)
            {
                // Retorna verdadeiro
                return 1;
            }
        }
    }
    else if(obj == 4)
    {
        // O objeto e um ETA
        for(i = 0; loc.ETA[i].lin > -1; i++)
        {
            // Verificando se a coordenada bate
            // com a coordenada do submarino
            if(loc.ETA[i].lin == sub.lin &&
               loc.ETA[i].col == sub.col)
            {
                // Retorna verdadeiro
                return 1;
            }
        }
    }

    // Retorna falso
    return 0;
}

int movimenta(Posicao* pos)
{
    int sair = 1;
    int tecla;

    printf("Teclas:\n"
           "[W] - cima\n"
           "[S] - baixo\n"
           "[A] - esquerda\n"
           "[D] - direita\n"
           "Escolha: ");

    while(sair)
    {
        tecla = _getch();

        // Atualizando pos
        switch(tecla)
        {
            case 'w':
            case 'W':
                {
                    (*pos).lin--;
                    sair = 0;
                }
            break;

            case 's':
            case 'S':
                {
                    (*pos).lin++;
                    sair = 0;
                }
            break;

            case 'a':
            case 'A':
                {
                    (*pos).col--;
                    sair = 0;
                }
            break;

            case 'd':
            case 'D':
                {
                    (*pos).col++;
                    sair = 0;
                }
        }
    }
    printf("%c", tecla);

    if((*pos).lin < 0 || (*pos).lin > 9 ||
       (*pos).col < 0 || (*pos).col > 9)
    {
        //printf("<%d %d>", (*pos).lin, (*pos).col);
        return -1;
    }

    return 0;
}

int menu(int mapa[10][10], Locais* loc, int* pts,
         char* nome, int* cp, int* outcp)
{
    int rele = 1;
    int tecla;
    int sair = 1;

    // Verificando se algo mudou desde a ultima chamada de atualizaMapa
    if(procura((*loc), 4, (*loc).SUB))
    {
        // ETA
        *pts += 10;
    }
    else if(procura((*loc), 3, (*loc).SUB))
    {
        // Destroco
        corTexto(vermelho_claro_, preto_);
        printf("\nO submarino colidiu com um destroco\nVoce perdeu!\n\n");
        corTexto(cinza_claro_, preto_);

        // Pergunta se quer um novo jogo ou sair
        while(rele)
        {
            printf("1. Novo jogo\n"
                   "2. Sair do SubXplore\n"
                   "Escolha: ");

            while((tecla = _getch() - 48) < 1 || tecla > 2);

            switch(tecla)
            {
                case 1:
                    {
                        // Novo jogo
                        rele = 0;
                        printf("%d", tecla);
                        printf("\nQuer mesmo iniciar um novo jogo? s/n\n");
                        tecla = _getche();
                        if(tecla == 's' || tecla == 'S')
                        {
                            LimpaTela();
                            return 1;
                        }
                        else
                        {
                            LimpaTela();
                            mostraMapa(mapa, loc);
                            rele = 1;
                        }
                    }
                break;
                case 2:
                    {
                        // Sair
                        rele = 0;
                        printf("%d", tecla);
                        printf("\nQuer mesmo sair? s/n\n");
                        tecla = _getche();
                        if(tecla == 's' || tecla == 'S')
                        {
                            LimpaTela();
                            corTexto(amarelo_, preto_);
                            printf("Obrigado por jogar SubXplore!\n\n");
                            corTexto(cinza_claro_, preto_);
                            // Restaurando a pagina de codigo
                            SetConsoleCP(*cp);
                            SetConsoleOutputCP(*outcp);
                            return 2;
                        }
                        else
                        {
                            LimpaTela();
                            mostraMapa(mapa, loc);
                            rele = 1;
                        }
                    }
            }
        }
    }
    else if((*loc).SUB.lin == (*loc).NTN.lin &&
       (*loc).SUB.col == (*loc).NTN.col)
    {
        // NTN
        *pts += 50;
        corTexto(amarelo_, preto_);
        printf("\nO submarino alcancou o NTN\n"
               "%s, voce venceu SubXplore!\n"
               "Pontuacao: %d\n\n", nome, *pts);
        corTexto(cinza_claro_, preto_);

        // Pergunta se quer um novo jogo ou sair
        while(rele)
        {
            printf("1. Novo jogo\n"
                   "2. Sair do SubXplore\n"
                   "Escolha: ");

            while((tecla = _getch() - 48) < 1 || tecla > 2);

            switch(tecla)
            {
                case 1:
                    {
                        // Novo jogo
                        rele = 0;
                        printf("%d", tecla);
                        printf("\nQuer mesmo iniciar um novo jogo? s/n\n");
                        tecla = _getche();
                        if(tecla == 's' || tecla == 'S')
                        {
                            LimpaTela();
                            return 1;
                        }
                        else
                        {
                            LimpaTela();
                            mostraMapa(mapa, loc);
                            rele = 1;
                        }
                    }
                break;
                case 2:
                    {
                        // Sair
                        rele = 0;
                        printf("%d", tecla);
                        printf("\nQuer mesmo sair? s/n\n");
                        tecla = _getche();
                        if(tecla == 's' || tecla == 'S')
                        {
                            LimpaTela();
                            corTexto(amarelo_, preto_);
                            printf("Obrigado por jogar SubXplore!\n\n");
                            corTexto(cinza_claro_, preto_);
                            // Restaurando a pagina de codigo
                            SetConsoleCP(*cp);
                            SetConsoleOutputCP(*outcp);
                            return 2;
                        }
                        else
                        {
                            LimpaTela();
                            mostraMapa(mapa, loc);
                            rele = 1;
                        }
                    }
            }
        }
    }
    else
    {
        // Imprime menu
        printf("1. Movimentar submarino\n"
               "2. Novo jogo\n"
               "3. Sair do SubXplore\n"
               "Escolha: ");
    }

    while(sair)
    {
        while((tecla = _getch() - 48) < 1 || tecla > 3);

        switch(tecla)
        {
            case 1:
                {
                    sair = 0;
                    printf("%d", tecla);
                    printf("\n\n");
                    // Atualizando atual do submarino vira mar
                    mapa[(*loc).SUB.lin][(*loc).SUB.col] = 0;
                    if(movimenta(&((*loc).SUB)) == -1)
                    {
                        // Retorno -1 significa que ultrapassou os limites da matriz
                        corTexto(vermelho_claro_, preto_);
                        printf("\n\nO submarino saiu do mapa\nVoce perdeu!\n\n");
                        corTexto(cinza_claro_, preto_);

                        // Pergunta se quer um novo jogo ou sair
                        while(rele)
                        {
                            printf("1. Novo jogo\n"
                                   "2. Sair do SubXplore\n"
                                   "Escolha: ");

                            while((tecla = _getch() - 48) < 1 || tecla > 2);

                            switch(tecla)
                            {
                                case 1:
                                    {
                                        // Novo jogo
                                        rele = 0;
                                        printf("%d", tecla);
                                        printf("\nQuer mesmo iniciar um novo jogo? s/n\n");
                                        tecla = _getche();
                                        if(tecla == 's' || tecla == 'S')
                                        {
                                            LimpaTela();
                                            return 1;
                                        }
                                        else
                                        {
                                            LimpaTela();
                                            mostraMapa(mapa, loc);
                                            rele = 1;
                                        }
                                    }
                                break;
                                case 2:
                                    {
                                        // Sair
                                        rele = 0;
                                        printf("%d", tecla);
                                        printf("\nQuer mesmo sair? s/n\n");
                                        tecla = _getche();
                                        if(tecla == 's' || tecla == 'S')
                                        {
                                            LimpaTela();
                                            corTexto(amarelo_, preto_);
                                            printf("Obrigado por jogar SubXplore!\n\n");
                                            corTexto(cinza_claro_, preto_);
                                            // Restaurando a pagina de codigo
                                            SetConsoleCP(*cp);
                                            SetConsoleOutputCP(*outcp);
                                            return 2;
                                        }
                                        else
                                        {
                                            LimpaTela();
                                            mostraMapa(mapa, loc);
                                            rele = 1;
                                        }
                                    }
                            }
                        }
                    }
                    // Atualizando a posicao do submarino
                    mapa[(*loc).SUB.lin][(*loc).SUB.col] = 1;
                    if(procura((*loc), 3, (*loc).SUB))
                    {
                        // Destroco
                        corTexto(vermelho_claro_, preto_);
                        printf("\n\nO submarino colidiu com um destroco\nVoce perdeu!\n\n");
                        corTexto(cinza_claro_, preto_);

                        // Pergunta se quer um novo jogo ou sair
                        while(rele)
                        {
                            printf("1. Novo jogo\n"
                                   "2. Sair do SubXplore\n"
                                   "Escolha: ");

                            while((tecla = _getch() - 48) < 1 || tecla > 2);

                            switch(tecla)
                            {
                                case 1:
                                    {
                                        // Novo jogo
                                        rele = 0;
                                        printf("%d", tecla);
                                        printf("\nQuer mesmo iniciar um novo jogo? s/n\n");
                                        tecla = _getche();
                                        if(tecla == 's' || tecla == 'S')
                                        {
                                            LimpaTela();
                                            return 1;
                                        }
                                        else
                                        {
                                            LimpaTela();
                                            mostraMapa(mapa, loc);
                                            rele = 1;
                                        }
                                    }
                                break;
                                case 2:
                                    {
                                        // Sair
                                        rele = 0;
                                        printf("%d", tecla);
                                        printf("\nQuer mesmo sair? s/n\n");
                                        tecla = _getche();
                                        if(tecla == 's' || tecla == 'S')
                                        {
                                            LimpaTela();
                                            corTexto(amarelo_, preto_);
                                            printf("Obrigado por jogar SubXplore!\n\n");
                                            corTexto(cinza_claro_, preto_);
                                            // Restaurando a pagina de codigo
                                            SetConsoleCP(*cp);
                                            SetConsoleOutputCP(*outcp);
                                            return 2;
                                        }
                                        else
                                        {
                                            LimpaTela();
                                            mostraMapa(mapa, loc);
                                            rele = 1;
                                        }
                                    }
                            }
                        }
                    }
                    if(procura((*loc), 4, (*loc).SUB))
                    {
                        // ETA
                        *pts += 10;
                    }
                    if((*loc).SUB.lin == (*loc).NTN.lin &&
                       (*loc).SUB.col == (*loc).NTN.col)
                    {
                        // NTN
                        *pts += 50;
                        corTexto(amarelo_, preto_);
                        printf("\n\nO submarino alcancou o NTN\n"
                               "%s, voce venceu SubXplore!\n"
                               "Pontuacao: %d\n\n", nome, *pts);
                        corTexto(cinza_claro_, preto_);

                        // Pergunta se quer um novo jogo ou sair
                        while(rele)
                        {
                            printf("1. Novo jogo\n"
                                   "2. Sair do SubXplore\n"
                                   "Escolha: ");

                            while((tecla = _getch() - 48) < 1 || tecla > 2);

                            switch(tecla)
                            {
                                case 1:
                                    {
                                        // Novo jogo
                                        rele = 0;
                                        printf("%d", tecla);
                                        printf("\nQuer mesmo iniciar um novo jogo? s/n\n");
                                        tecla = _getche();
                                        if(tecla == 's' || tecla == 'S')
                                        {
                                            LimpaTela();
                                            return 1;
                                        }
                                        else
                                        {
                                            LimpaTela();
                                            mostraMapa(mapa, loc);
                                            rele = 1;
                                        }
                                    }
                                break;
                                case 2:
                                    {
                                        // Sair
                                        rele = 0;
                                        printf("%d", tecla);
                                        printf("\nQuer mesmo sair? s/n\n");
                                        tecla = _getche();
                                        if(tecla == 's' || tecla == 'S')
                                        {
                                            LimpaTela();
                                            corTexto(amarelo_, preto_);
                                            printf("Obrigado por jogar SubXplore!\n\n");
                                            corTexto(cinza_claro_, preto_);
                                            // Restaurando a pagina de codigo
                                            SetConsoleCP(*cp);
                                            SetConsoleOutputCP(*outcp);
                                            return 2;
                                        }
                                        else
                                        {
                                            LimpaTela();
                                            mostraMapa(mapa, loc);
                                            rele = 1;
                                        }
                                    }
                            }
                        }
                    }
                }
            break;
            case 2:
                {
                    // Novo jogo
                    sair = 0;
                    printf("%d", tecla);
                    printf("\nQuer mesmo iniciar um novo jogo? s/n\n");
                    tecla = _getche();
                    if(tecla == 's' || tecla == 'S')
                    {
                        LimpaTela();
                        return 1;
                    }
                    else
                    {
                        return -1;
                    }
                }
            break;
            case 3:
                {
                    // Sair
                    sair = 0;
                    printf("%d", tecla);
                    printf("\nQuer mesmo sair? s/n\n");
                    tecla = _getche();
                    if(tecla == 's' || tecla == 'S')
                    {
                        LimpaTela();
                        corTexto(amarelo_, preto_);
                        printf("Obrigado por jogar SubXplore!\n\n");
                        corTexto(cinza_claro_, preto_);
                        // Restaurando a pagina de codigo
                        SetConsoleCP(*cp);
                        SetConsoleOutputCP(*outcp);
                        return 2;
                    }
                    else
                    {
                        return -1;
                    }
                }
        }
    }

    return 0;
}

int randomInteger (int low, int high) {
    int k;
    double d;

    d = (double) rand () / ((double) RAND_MAX + 1);
    k = d * (high - low + 1);

    return low + k;
}

void geraMapa(int mapa[10][10]) {
    int i, j, qtdO, qtdA, lin, col;

    srand((unsigned int)time(NULL));

    for (i=0; i<10; i++)
        for (j=0; j<10; j++)
            mapa[i][j] = 0;

    mapa[randomInteger(0,9)][0]= 1;
    mapa[randomInteger(0,9)][9]= 2;
    qtdO = randomInteger(5,25);
    qtdA = randomInteger(3,10);

    while (qtdO>0) {
        lin = randomInteger(0,9);
        col = randomInteger(1,8);
        if (mapa[lin][col] == 0) {
            mapa[lin][col] = 3;
            qtdO--;
        }
    }

    while (qtdA>0) {
        lin = randomInteger(0,9);
        col = randomInteger(1,8);
        if (mapa[lin][col] == 0) {
            mapa[lin][col] = 4;
            qtdA--;
        }
    }
}

void atualizaMapa(int mapa[10][10]) {
    int i, j, upd, atualizado[10][10];
    for (i=0; i<10; i++) for (j=0; j<10; j++) atualizado[i][j]=0;

    for (i=0; i<10; i++)
        for (j=0; j<10; j++) {
            if ((mapa[i][j] == 2 || mapa[i][j] == 3 || mapa[i][j] == 4) && atualizado[i][j] == 0) {
                upd = randomInteger(1,4);
                switch (upd) {
                    case 1:
                        if ((mapa[i-1][j] == 0 || (mapa[i-1][j] == 1 && mapa[i][j] == 3)) && i>0) {
                            mapa[i-1][j] = mapa[i][j];
                            mapa[i][j] = 0;
                            atualizado[i-1][j]=1;
                        }
                        break;
                    case 2:
                        if ((mapa[i+1][j] == 0 || (mapa[i+1][j] == 1 && mapa[i][j] == 3)) && i<9) {
                            mapa[i+1][j] = mapa[i][j];
                            mapa[i][j] = 0;
                            atualizado[i+1][j]=1;
                        }
                        break;
                    case 3:
                        if ((mapa[i][j+1] == 0 || (mapa[i][j+1] == 1 && mapa[i][j] == 3)) && j<9) {
                            mapa[i][j+1] = mapa[i][j];
                            mapa[i][j] = 0;
                            atualizado[i][j+1]=1;
                        }
                        break;
                    case 4:
                        if ((mapa[i][j-1] == 0 || (mapa[i][j-1] == 1 && mapa[i][j] == 3)) && j>0) {
                            mapa[i][j-1] = mapa[i][j];
                            mapa[i][j] = 0;
                            atualizado[i][j-1]=1;
                        }
                }
            }
       }
}

void instrucoes(char* nome, int* cp, int* outcp)
{
    // Obtendo a pagina de codigo e
    // a pagina de codigo de saida
    *cp = GetConsoleCP();
    *outcp = GetConsoleOutputCP();

    // Alterando a pagina de codigo
    SetConsoleCP(437);
    SetConsoleOutputCP(437);

    // Maximizando a janela
    ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

    corTexto(amarelo_, preto_);
    printf("Boas vindas ao SubXpore\n");
    corTexto(cinza_claro_, preto_);

    printf("\nInforme seu nome:\n");
    scanf("%30[^\n]%*c", nome);
    while(strlen(nome) < 3)
    {
        // Caso sejam menos de 3 caracteres para o nome
        printf("Informe um nome maior:\n");
        scanf("%30[^\n]%*c", nome);
    }

    // Imprimindo as instrucoes
    printf("\nSe atente para as instru%coes a seguir:\n"
           "SubXplore %c um jogo baseado em um mapa mar%ctimo em\n"
           "que voc%c deve guiar um submarino (representado pelo\n"
           "caractere %c) at%c a embarca%cao cargueiro Netuno (NTN,\n"
           "representado pelo caractere %c) naufragado, que estava\n"
           "carregado com diversos equipamentos de alta tecnologia\n"
           "(ETA, representados pelo caractere %c), passando por\n"
           "possiveis obst%cculos, os destro%cos da embarca%cao,\n"
           "(representados pelo caractere %c) espalhados pelo mar.\n"
           "Voc%c ganha pontos ao alcan%car o NTN e ao resgatar ETAs.",
           135, 130, 161, 136, 244, 130, 135, 245, 169, 160, 135, 135,
           207, 136, 135);

    corTexto(amarelo_, preto_);
    printf("\n\nPressione Enter para iniciar...");
    corTexto(cinza_claro_, preto_);
    getchar();
}

void mostraMapa(int mapa[10][10], Locais* loc)
{
    int i, j;
    int ETAaux = 0;
    int DTCaux = 0;
    putchar('\n');

    // Imprimindo a matriz mapa
    for(i = 0; i < 10; i++)
    {
        for(j = 0; j < 10; j++)
        {
            switch(mapa[i][j])
            {
                case 0:
                    {
                        corTexto(azul_claro_, preto_);
                        printf(" ~");
                    }
                break;
                case 1:
                    {
                        corTexto(branco_, preto_);
                        printf(" %c", 244);
                        // Posicao do submarino
                        (*loc).SUB.lin = i;
                        (*loc).SUB.col = j;
                        //printf("<%d %d>", i, j);
                    }
                break;
                case 2:
                    {
                        corTexto(amarelo_, preto_);
                        printf(" %c", 245);
                        // Posicao do NTN
                        (*loc).NTN.lin = i;
                        (*loc).NTN.col = j;
                    }
                break;
                case 3:
                    {
                        corTexto(vermelho_claro_, preto_);
                        printf(" %c", 207);
                        // Lista de posicoes dos destrocos
                        (*loc).DTC[DTCaux++].lin = i;
                        (*loc).DTC[DTCaux-1].col = j;
                        (*loc).DTC[DTCaux].lin = -1;
                        (*loc).DTC[DTCaux].col = -1;
                    }
                break;
                case 4:
                    {
                        corTexto(ciano_claro_, preto_);
                        printf(" %c", 169);
                        // Lista de posicoes dos ETA
                        (*loc).ETA[ETAaux++].lin = i;
                        (*loc).ETA[ETAaux-1].col = j;
                        (*loc).ETA[ETAaux].lin = -1;
                        (*loc).ETA[ETAaux].col = -1;
                    }
            }
            corTexto(cinza_claro_, preto_);
            if(j < 9) putchar(' ');
        }
        printf("\n\n");
    }
}
