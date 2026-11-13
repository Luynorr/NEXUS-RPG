#ifndef MONSTROS_H
#define MONSTROS_H

#include "Personagens.h"
#include "Visual.h"
#include <vector>

// ============================================================================
// 1. CLASSE BASE INTERMEDIÁRIA: Monstro
// ============================================================================
class Monstro : public Personagem {
protected:
    string tituloArea;

public:
    Monstro(string nome, int pvMax, int def, int res, int bonusAcerto, string titulo);
    virtual ~Monstro();

    // Métodos Virtuais Puros e de Interface
    virtual void executarTurnoIA(Personagem& alvo) = 0;
    virtual void exibirEntradaCinematica() const = 0;
    virtual vector<Esqueleto*>* getCapangasPonteiro() { return nullptr; }

    void executarAcao(Personagem& alvo, int escolhaAcao, vector<Esqueleto*>* capangas = nullptr) override;
    void exibirStatus() const override;

    string getTituloArea() const;
};

// ============================================================================
// 2. SUBCLASSES CONCRETAS DE CHEFES E CAPANGAS
// ============================================================================

// CHEFE 1: Dragão Kaido (Fornalha Cinzenta - PV: 130 | Def: 16 | Res: 16 | Acerto: +9)
class DragaoKaido : public Monstro {
public:
    DragaoKaido();
    virtual ~DragaoKaido();

    void executarTurnoIA(Personagem& alvo) override;
    void exibirEntradaCinematica() const override;
};

// CAPANGA: Esqueleto (Invocado pelo Lich - PV: 10 | Def: 10 | Res: 8 | Acerto: +7)
class Esqueleto : public Monstro {
public:
    Esqueleto();
    virtual ~Esqueleto();

    void executarTurnoIA(Personagem& alvo) override;
    void exibirEntradaCinematica() const override;
};

// CHEFE 2: Necromante Lich (Cidade Sepulcral - PV: 90 | Def: 15 | Res: 15 | Acerto: +10)
class NecromanteLich : public Monstro {
private:
    vector<Esqueleto*> capangas;

public:
    NecromanteLich();
    virtual ~NecromanteLich();

    void executarTurnoIA(Personagem& alvo) override;
    void exibirEntradaCinematica() const override;
    void animarMortos();
    const vector<Esqueleto*>& getCapangas() const;
    vector<Esqueleto*>* getCapangasPonteiro() override { return &capangas; }
    void limparCapangas();
};

// CHEFE 3: Anjo Caído Baal (Catedral Herética - PV: 120 | Def: 16 | Res: 17 | Acerto: +11)
class AnjoCaidoBaal : public Monstro {
public:
    AnjoCaidoBaal();
    virtual ~AnjoCaidoBaal();

    void executarTurnoIA(Personagem& alvo) override;
    void exibirEntradaCinematica() const override;
};

// CHEFE FINAL (FASE 1): Gotwellenn (Matriz Congelada - PV: 70 | Def: 15 | Res: 15 | Acerto: +9)
class Gotwellenn : public Monstro {
public:
    Gotwellenn();
    virtual ~Gotwellenn();

    void executarTurnoIA(Personagem& alvo) override;
    void exibirEntradaCinematica() const override;
};

// CHEFE FINAL (FASE 2): Wellington Verdadeiro (Matriz Congelada - PV: 165 | Def: 17 | Res: 17 | Acerto: +12)
class WellingtonVerdadeiro : public Monstro {
private:
    int turnosAscensao;

public:
    WellingtonVerdadeiro();
    virtual ~WellingtonVerdadeiro();

    void executarTurnoIA(Personagem& alvo) override;
    void exibirEntradaCinematica() const override;
    void processarInicioTurno() override;

    int getDefesaEfetiva() const;
    int getResistencia() const;
    int getDanoBonusAscensao() const;
};

// CAPANGA: Kobold (Minion Genérico - PV: 8 | Def: 8 | Res: 6 | Acerto: +7)
class Kobold : public Monstro {
public:
    Kobold();
    virtual ~Kobold();

    void executarTurnoIA(Personagem& alvo) override;
    void exibirEntradaCinematica() const override;
};

#endif // MONSTROS_H
