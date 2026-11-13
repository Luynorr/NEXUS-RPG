#ifndef VISUAL_H
#define VISUAL_H

#include <iostream>
#include <string>
#include <cstdlib>

// Compatibilidade de pausa entre Windows (Sleep) e POSIX/Linux (sleep_for)
#ifdef _WIN32
    #include <windows.h>
#else
    #include <thread>
    #include <chrono>
#endif

using namespace std;

// ============================================================================
// NAMESPACE: Cor (Cores e Formatações ANSI para o Terminal)
// ============================================================================
namespace Cor {
    const string RESET       = "\033[0m";
    const string NEGRITO     = "\033[1m";
    const string SUBLINHADO  = "\033[4m";

    // Cores de Texto
    const string VERMELHO    = "\033[31m"; // Dano / Inimigos / Perigo
    const string VERDE       = "\033[32m"; // Cura / Sucesso / Vida Alta
    const string AMARELO     = "\033[33m"; // d20 / Alertas / Recursos / Vida Média
    const string AZUL        = "\033[34m"; // Mana / Mago
    const string MAGENTA     = "\033[35m"; // Bárbaro / Chefes
    const string CIANO       = "\033[36m"; // Nexus / Arqueiro / Informação
    const string BRANCO      = "\033[37m"; // Textos Principais
    const string CINZA       = "\033[90m"; // Bordas / Detalhes Secundários
}

// ============================================================================
// FUNÇÕES AUXILIARES DE ANIMAÇÃO E INTERFACE VISUAL
// ============================================================================

void pausarMs(int ms);
void digitarTexto(const string& texto, int msDelay = 20);
void limparTela();
void pausarParaContinuar();

void desenharBarraVida(const string& nome, int pvAtual, int pvMaximo, int largura = 15);
void desenharBarraRecurso(const string& nomeRecurso, int atual, int maximo, const string& corBarra, int largura = 12);

// ============================================================================
// ARTES ASCII DO JOGO
// ============================================================================
void desenharLogoNexus();
void desenharKaidoASCII();
void desenharLichASCII();
void desenharBaalASCII();
void desenharGotwellennASCII();
void desenharWellingtonASCII();
void desenharMagoASCII();
void desenharBarbaroASCII();
void desenharArqueiroASCII();
void desenharVitoriaASCII();
void desenharGameOverASCII();

#endif // VISUAL_H
