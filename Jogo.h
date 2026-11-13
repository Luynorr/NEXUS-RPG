#ifndef JOGO_H
#define JOGO_H

#include "Personagens.h"
#include "Monstros.h"
#include "Visual.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

// ============================================================================
// CLASSE GERENCIADORA: Jogo (Engine / Loop Principal do RPG)
// ============================================================================
class Jogo {
private:
    Jogador* heroi;               // Ponteiro para a classe concreta do herói
    int cadeadosRestantes;        // Inicia em 3. Cada chefe reduz 1 cadeado.
    
    bool chefeKaidoDerrotado;     // Status da Fornalha Cinzenta
    bool chefeLichDerrotado;      // Status da Cidade Sepulcral
    bool chefeBaalDerrotado;      // Status da Catedral Herética
    
    bool jogoEmAndamento;         // Controla o loop principal do Nexus

    void exibirCabecalhoNexus() const;
    void escolherClasse();
    void menuNexus();
    
    void iniciarBatalha(Monstro* chefe, bool ehBossFinal = false);
    bool executarTurnoJogador(Personagem* inimigo);
    void processarVitoriaBoss(const string& nomeBoss, bool& flagBoss);
    
    void executarBatalhaFinal();
    
    void exibirVitoriaJogo() const;
    void exibirGameOver() const;

public:
    Jogo();
    ~Jogo();

    void iniciar();
};

#endif // JOGO_H
