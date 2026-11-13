#include "Jogo.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

#ifdef _WIN32
    #include <windows.h>
#endif

using namespace std;

// ============================================================================
// PONTO DE ENTRADA PRINCIPAL (main-v2.cpp)
// ============================================================================

int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(65001);
    #endif

    srand(static_cast<unsigned int>(time(NULL)));

    Jogo jogo;
    jogo.iniciar();

    return 0;
}
