#include "personagem.h" //arquivos do projeto ficam entre aspas duplas

// g++ *.cpp -o jogo && clear && ./jogo para compilar e executar o projeto


int main(void) {
    personagem demonio("Baphomet", 666);
    exibir(demonio);

    personagem humano("Arthur", 67);
    exibir(humano);
    return 0;
}