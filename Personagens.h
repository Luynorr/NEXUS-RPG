#ifndef PERSONAGENS_H
#define PERSONAGENS_H

#include "Visual.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

// ============================================================================
// 1. ESTRUTURA DE DADOS: Item
// ============================================================================
struct Item {
    string nome;        // Nome de exibição do item (ex: "Poção de Cura")
    string descricao;   // Explicação breve do efeito para o menu da bolsa
    int tipo;           // 1 = Recupera PV, 2 = Restaura Mana/Energia, 3 = Dano Direto no Inimigo
    int valorEfeito;    // Quantidade numérica do impacto (ex: 24 para cura, 32 para dano)
};

// Declarações antecipadas (Forward Declarations)
class Personagem;
class Esqueleto;

// ============================================================================
// 2. CLASSE BASE ABSTRATA: Personagem
// ============================================================================
class Personagem {
protected:
    string nome;             // Nome da entidade
    int pvAtual;             // Pontos de Vida atuais
    int pvMaximo;            // Teto máximo de Pontos de Vida
    int defesa;              // Defesa Física (Reduz dano físico)
    int resistencia;         // Resistência Mágica / Dificuldade para testes mágicos
    int bonusAcerto;         // Bônus base de acerto somado ao d20 (Calibrado por classe)

    // Controle de Estados de Batalha (Buffs & Debuffs)
    int turnosAtordoado;               // Se > 0, perde a vez no turno
    int turnosArmaduraArcana;          // Se > 0, concede +8 de Defesa bônus
    bool estaEscondido;                // Se true, imunidade total por 1 turno
    double multiplicadorDanoRecebido;  // Multiplicador de dano sofrido (ex: 1.5)
    int modBonusAcertoTemp;            // Modificador temporário de acerto
    int turnosModAcerto;               // Duração do modificador temporário de acerto

    int gerarNumeroAleatorio(int min, int max) const;

public:
    Personagem(string nome, int pvMax, int def, int res, int bonusAcerto);
    virtual ~Personagem();

    // Métodos Virtuais Puros
    virtual void executarAcao(Personagem& alvo, int escolhaAcao, vector<Esqueleto*>* capangas = nullptr) = 0;
    virtual void exibirStatus() const = 0;

    // Métodos Concretos Compartilhados
    virtual void processarInicioTurno();
    virtual void receberDano(int danoBruto);
    virtual void curarPV(int quantidade);

    bool testarAcerto(int defesaOuResistenciaAlvo) const;

    // Aplicação e Manipulação de Estados
    void aplicarAtordoamento(int turnos = 1);
    void decrementarAtordoamento();
    void aplicarArmaduraArcana(int turnos = 3);
    void aplicarEscondido(bool estado = true);
    void aplicarMultiplicadorDano(double mult);
    void aplicarModificadorAcerto(int mod, int turnos);

    // Getters de Leitura
    bool estaAtordoado() const;
    bool getEstaEscondido() const;
    bool estaVivo() const;
    string getNome() const;
    int getPvAtual() const;
    int getPvMaximo() const;
    int getDefesaEfetiva() const;       // Retorna a defesa base + bônus de Armadura Arcana (+8)
    int getResistencia() const;
    int getBonusAcertoBase() const;
    int getBonusAcertoEfetivo() const;
};

// ============================================================================
// 3. CLASSE INTERMEDIÁRIA: Jogador
// ============================================================================
class Jogador : public Personagem {
protected:
    int recursoAtual;    // Pontos de Mana ou Energia atuais
    int recursoMaximo;   // Teto máximo de recurso da classe
    vector<Item> bolsa;  // Inventário dinâmico de consumíveis

public:
    Jogador(string nome, int pvMax, int def, int res, int bonusAcerto, int recursoMax);
    virtual ~Jogador();

    // Exibição personalizada do menu de habilidades da classe
    virtual void exibirMenuHabilidades() const = 0;

    void evoluirNoNexus();
    void inicializarBolsaInicial();
    void exibirExplicacaoItensIniciais() const;
    void exibirBolsa() const;
    bool usarItem(int indice, Personagem& alvo, vector<Esqueleto*>* capangas = nullptr);
    void adicionarItem(string nome, string desc, int tipo, int valor);

    void restaurarRecurso(int quantidade);
    int getRecursoAtual() const;
    int getRecursoMaximo() const;
    int getQtdItens() const;
};

// ============================================================================
// 4. SUBCLASSES CONCRETAS DE HERÓIS REBALANCEADOS
// ============================================================================

// Mago (PV: 85 | Mana: 25 | Def: 10 | Res: 12 | Acerto: +7)
class Mago : public Jogador {
public:
    Mago(string nome);
    virtual ~Mago();

    void executarAcao(Personagem& alvo, int escolhaAcao, vector<Esqueleto*>* capangas = nullptr) override;
    void exibirStatus() const override;
    void exibirMenuHabilidades() const override;
};

// Bárbaro (PV: 120 | Energia: 16 | Def: 17 | Res: 15 | Acerto: +7)
class Barbaro : public Jogador {
public:
    Barbaro(string nome);
    virtual ~Barbaro();

    void executarAcao(Personagem& alvo, int escolhaAcao, vector<Esqueleto*>* capangas = nullptr) override;
    void exibirStatus() const override;
    void exibirMenuHabilidades() const override;
};

// Arqueiro (PV: 95 | Energia: 18 | Def: 14 | Res: 18 | Acerto: +8)
class Arqueiro : public Jogador {
public:
    Arqueiro(string nome);
    virtual ~Arqueiro();

    void executarAcao(Personagem& alvo, int escolhaAcao, vector<Esqueleto*>* capangas = nullptr) override;
    void exibirStatus() const override;
    void exibirMenuHabilidades() const override;
};

#endif // PERSONAGENS_H
