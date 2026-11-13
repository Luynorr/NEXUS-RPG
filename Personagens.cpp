#include "Personagens.h"
#include "Monstros.h"

// ============================================================================
// IMPLEMENTAÇÃO: Personagem (Classe Base Abstrata)
// ============================================================================

Personagem::Personagem(string nome, int pvMax, int def, int res, int bonusAcerto)
    : nome(nome), pvAtual(pvMax), pvMaximo(pvMax), defesa(def), resistencia(res), bonusAcerto(bonusAcerto),
      turnosAtordoado(0), turnosArmaduraArcana(0), estaEscondido(false), multiplicadorDanoRecebido(1.0),
      modBonusAcertoTemp(0), turnosModAcerto(0) {}

Personagem::~Personagem() {}

int Personagem::gerarNumeroAleatorio(int min, int max) const {
    return min + (rand() % (max - min + 1));
}

bool Personagem::testarAcerto(int defesaOuResistenciaAlvo) const {
    int d20 = gerarNumeroAleatorio(1, 20);
    int bonusEfetivo = getBonusAcertoEfetivo();
    int total = d20 + bonusEfetivo;

    cout << Cor::AMARELO << "  🎲 [d20 TESTE] " << Cor::RESET << "Rolou " << Cor::NEGRITO << d20 << Cor::RESET 
         << " + Bônus (" << (bonusEfetivo >= 0 ? "+" : "") << bonusEfetivo << ") = " 
         << Cor::NEGRITO << total << Cor::RESET 
         << " vs Meta " << defesaOuResistenciaAlvo << " -> ";

    if (total >= defesaOuResistenciaAlvo) {
        cout << Cor::VERDE << Cor::NEGRITO << "[SUCESSO / ACERTOU!]" << Cor::RESET << endl;
        return true;
    } else {
        cout << Cor::VERMELHO << Cor::NEGRITO << "[FALHA / ERROU!]" << Cor::RESET << endl;
        return false;
    }
}

void Personagem::processarInicioTurno() {
    if (turnosArmaduraArcana > 0) {
        turnosArmaduraArcana--;
        if (turnosArmaduraArcana == 0) {
            cout << Cor::AZUL << "  🛡️ A Armadura Arcana de " << nome << " dissipou-se." << Cor::RESET << endl;
        }
    }
    if (estaEscondido) {
        estaEscondido = false;
    }
    if (turnosModAcerto > 0) {
        turnosModAcerto--;
        if (turnosModAcerto == 0) {
            modBonusAcertoTemp = 0;
            cout << Cor::CINZA << "  🎯 Modificador temporário de acerto de " << nome << " expirou." << Cor::RESET << endl;
        }
    }
}

void Personagem::receberDano(int danoBruto) {
    if (estaEscondido) {
        cout << Cor::CIANO << "  💨 " << nome << " estava ESCONDIDO e esquivou-se completamente do golpe!" << Cor::RESET << endl;
        return;
    }

    int defEfetiva = getDefesaEfetiva();
    int danoEfetivo = danoBruto - (defEfetiva / 2);
    if (danoEfetivo < 1) danoEfetivo = 1;

    if (multiplicadorDanoRecebido > 1.0) {
        danoEfetivo = static_cast<int>(danoEfetivo * multiplicadorDanoRecebido);
        cout << Cor::VERMELHO << Cor::NEGRITO << "  💥 [GOLPE VULNERÁVEL] Dano ampliado pela paralisia!" << Cor::RESET << endl;
        multiplicadorDanoRecebido = 1.0;
    }

    pvAtual -= danoEfetivo;
    if (pvAtual < 0) pvAtual = 0;

    cout << Cor::VERMELHO << "  🩸 " << nome << " sofreu " << Cor::NEGRITO << danoEfetivo 
         << Cor::RESET << Cor::VERMELHO << " de dano! " << Cor::RESET;
    desenharBarraVida(nome, pvAtual, pvMaximo, 10);
    cout << endl;
}

void Personagem::curarPV(int quantidade) {
    pvAtual += quantidade;
    if (pvAtual > pvMaximo) pvAtual = pvMaximo;
}

void Personagem::aplicarAtordoamento(int turnos) { turnosAtordoado = turnos; }
void Personagem::decrementarAtordoamento() { if (turnosAtordoado > 0) { turnosAtordoado--; } }
void Personagem::aplicarArmaduraArcana(int turnos) { turnosArmaduraArcana = turnos; }
void Personagem::aplicarEscondido(bool estado) { estaEscondido = estado; }
void Personagem::aplicarMultiplicadorDano(double mult) { multiplicadorDanoRecebido = mult; }
void Personagem::aplicarModificadorAcerto(int mod, int turnos) {
    modBonusAcertoTemp = mod;
    turnosModAcerto = turnos;
}

bool Personagem::estaAtordoado() const { return turnosAtordoado > 0; }
bool Personagem::getEstaEscondido() const { return estaEscondido; }
bool Personagem::estaVivo() const { return pvAtual > 0; }
string Personagem::getNome() const { return nome; }
int Personagem::getPvAtual() const { return pvAtual; }
int Personagem::getPvMaximo() const { return pvMaximo; }

int Personagem::getDefesaEfetiva() const { 
    return defesa + (turnosArmaduraArcana > 0 ? 8 : 0); 
}

int Personagem::getResistencia() const { return resistencia; }
int Personagem::getBonusAcertoBase() const { return bonusAcerto; }
int Personagem::getBonusAcertoEfetivo() const { return bonusAcerto + modBonusAcertoTemp; }

// ============================================================================
// IMPLEMENTAÇÃO: Jogador (Subclasse Intermediária)
// ============================================================================

Jogador::Jogador(string nome, int pvMax, int def, int res, int bonusAcerto, int recursoMax)
    : Personagem(nome, pvMax, def, res, bonusAcerto), recursoAtual(recursoMax), recursoMaximo(recursoMax) {
    inicializarBolsaInicial();
}

Jogador::~Jogador() { bolsa.clear(); }

void Jogador::evoluirNoNexus() {
    pvMaximo += 15;
    recursoMaximo += 3;
    pvAtual = pvMaximo;
    recursoAtual = recursoMaximo;

    cout << "\n" << Cor::VERDE << Cor::NEGRITO << "==================================================" << endl;
    cout << "       ✨ EVOLUÇÃO CONCEDIDA NO NEXUS! ✨         " << endl;
    cout << "==================================================" << Cor::RESET << endl;
    cout << "Sua vitória fortaleceu seu código digital:" << endl;
    cout << Cor::VERDE << "  +15 PV Máximo   -> Teto de Vida: " << pvMaximo << endl;
    cout << "  +3 Recurso Máx  -> Teto de Recursos: " << recursoMaximo << endl;
    cout << "  (Vida e Recursos totalmente restaurados!)" << Cor::RESET << endl;
    cout << Cor::VERDE << "==================================================\n" << Cor::RESET << endl;
}

void Jogador::inicializarBolsaInicial() {
    bolsa.clear();
    bolsa.push_back({"Poção de Cura", "Recupera 24 PVs do herói.", 1, 24});
    bolsa.push_back({"Poção de Cura", "Recupera 24 PVs do herói.", 1, 24});
    bolsa.push_back({"Poção de Mana", "Recupera 12 de Mana/Energia.", 2, 12});
    bolsa.push_back({"Poção de Mana", "Recupera 12 de Mana/Energia.", 2, 12});
    bolsa.push_back({"Bomba de Fogo", "Causa 32 de dano direto a um inimigo.", 3, 32});
}

void Jogador::exibirExplicacaoItensIniciais() const {
    cout << "\n" << Cor::CIANO << Cor::NEGRITO << "==================================================" << endl;
    cout << "       🎒 SUPRIMENTOS DE COMBATE RECEBIDOS 🎒      " << endl;
    cout << "==================================================" << Cor::RESET << endl;
    cout << "Ao despertar no Nexus da Compilação, você encontra" << endl;
    cout << "uma bolsa contendo 5 itens fundamentais:" << endl;
    cout << Cor::VERDE << "  1. [2x] POÇÃO DE CURA  -> Recupera 24 PVs." << Cor::RESET << endl;
    cout << Cor::AZUL << "  2. [2x] POÇÃO DE MANA  -> Recupera 12 de Mana/Energia." << Cor::RESET << endl;
    cout << Cor::VERMELHO << "  3. [1x] BOMBA DE FOGO  -> Infringe 32 de dano no alvo (18 AoE)." << Cor::RESET << endl;
    cout << Cor::CIANO << "==================================================\n" << Cor::RESET << endl;
}

void Jogador::exibirBolsa() const {
    cout << "\n" << Cor::AMARELO << "--- 🎒 BOLSA DE ITENS DE " << nome << " ---" << Cor::RESET << endl;
    if (bolsa.empty()) {
        cout << Cor::CINZA << "  (Sua bolsa está vazia!)" << Cor::RESET << endl;
        return;
    }
    for (size_t i = 0; i < bolsa.size(); i++) {
        cout << "  " << Cor::NEGRITO << (i + 1) << "." << Cor::RESET << " " 
             << Cor::BRANCO << bolsa[i].nome << Cor::RESET << " - " 
             << Cor::CINZA << bolsa[i].descricao << Cor::RESET << endl;
    }
}

bool Jogador::usarItem(int indice, Personagem& alvo, vector<Esqueleto*>* capangas) {
    if (indice < 0 || indice >= static_cast<int>(bolsa.size())) {
        cout << "\n" << Cor::VERMELHO << "[AVISO] Posição de item inválida!" << Cor::RESET << endl;
        return false;
    }

    Item itemUsado = bolsa[indice];

    if (itemUsado.tipo == 1) { // Poção de Cura
        curarPV(itemUsado.valorEfeito);
        cout << "\n" << Cor::VERDE << "🧪 [ITEM] " << nome << " usou " << itemUsado.nome 
             << " e recuperou " << itemUsado.valorEfeito << " PVs!" << Cor::RESET << endl;
    } 
    else if (itemUsado.tipo == 2) { // Poção de Mana/Energia
        restaurarRecurso(itemUsado.valorEfeito);
        cout << "\n" << Cor::AZUL << "💧 [ITEM] " << nome << " usou " << itemUsado.nome 
             << " e recuperou " << itemUsado.valorEfeito << " de recurso!" << Cor::RESET << endl;
    } 
    else if (itemUsado.tipo == 3) { // Bomba de Fogo
        cout << "\n" << Cor::VERMELHO << "💣 [ITEM] " << nome << " arremessou uma " << itemUsado.nome 
             << " contra " << alvo.getNome() << "!" << Cor::RESET << endl;
        alvo.receberDano(itemUsado.valorEfeito);

        // Dano em Área nos Capangas / Esqueletos da arena
        if (capangas != nullptr && !capangas->empty()) {
            cout << Cor::AMARELO << "  💥 [DANO EM ÁREA] A onda de choque da Bomba de Fogo atinge os minions!" << Cor::RESET << endl;
            for (size_t i = 0; i < capangas->size(); i++) {
                if ((*capangas)[i] != nullptr && (*capangas)[i]->estaVivo()) {
                    cout << "  -> " << (*capangas)[i]->getNome() << " " << (i + 1) << " sofreu dano da explosão!" << endl;
                    (*capangas)[i]->receberDano(18);
                }
            }
        }
    }

    bolsa.erase(bolsa.begin() + indice);
    return true;
}

void Jogador::adicionarItem(string nome, string desc, int tipo, int valor) {
    bolsa.push_back({nome, desc, tipo, valor});
    cout << "\n" << Cor::VERDE << "[INVENTÁRIO] Item '" << nome << "' adicionado à bolsa!" << Cor::RESET << endl;
}

void Jogador::restaurarRecurso(int quantidade) {
    recursoAtual += quantidade;
    if (recursoAtual > recursoMaximo) recursoAtual = recursoMaximo;
}

int Jogador::getRecursoAtual() const { return recursoAtual; }
int Jogador::getRecursoMaximo() const { return recursoMaximo; }
int Jogador::getQtdItens() const { return static_cast<int>(bolsa.size()); }

// ============================================================================
// IMPLEMENTAÇÃO: Mago (Herói Concreto REBALANCEADO)
// Atributos: PV: 85 | Mana: 25 | Def: 10 | Res: 12 | Acerto: +7
// ============================================================================

Mago::Mago(string nome) : Jogador(nome, 85, 10, 12, 7, 25) {}
Mago::~Mago() {}

void Mago::exibirStatus() const {
    cout << Cor::AZUL << Cor::NEGRITO << "🔮 [MAGO DA COMPILAÇÃO] " << nome << Cor::RESET << " | ";
    desenharBarraVida("PV", pvAtual, pvMaximo, 10);
    cout << " | ";
    desenharBarraRecurso("MANA", recursoAtual, recursoMaximo, Cor::AZUL, 8);
    cout << " | DEF: " << getDefesaEfetiva() 
         << " | ACERTO: +" << getBonusAcertoEfetivo() << endl;
}

void Mago::exibirMenuHabilidades() const {
    cout << "\n" << Cor::AZUL << Cor::NEGRITO << "==================================================" << endl;
    cout << "     🔮 HABILIDADES DO MAGO DA COMPILAÇÃO 🔮       " << endl;
    cout << "==================================================" << Cor::RESET << endl;
    cout << "1. Super Soco         [Custo: 0 Mana | Dano: 5-8 vs Defesa]" << endl;
    cout << "2. Bola de Fogo       [Custo: 4 Mana | Dano: 20-32 vs Resistência (" << Cor::VERMELHO << "💥 DANO EM ÁREA" << Cor::RESET << ")]" << endl;
    cout << "3. Curar Ferimentos   [Custo: 2 Mana | Cura: 16-24 PV]" << endl;
    cout << "4. Armadura Arcana    [Custo: 3 Mana | +8 Defesa por 3 turnos]" << endl;
    cout << "5. Paralisia          [Custo: 5 Mana | 50% Paralisar + 50% Dano Vulnerável]" << endl;
    cout << Cor::AZUL << "==================================================" << Cor::RESET << endl;
    cout << "Escolha a habilidade: ";
}

void Mago::executarAcao(Personagem& alvo, int escolhaAcao, vector<Esqueleto*>* capangas) {
    switch (escolhaAcao) {
        case 1: { // Super Soco
            cout << "\n" << Cor::AZUL << "[HABILIDADE: SUPER SOCO] " << nome << " avança para um soco energizado!" << Cor::RESET << endl;
            if (testarAcerto(alvo.getDefesaEfetiva())) {
                int dano = gerarNumeroAleatorio(5, 8);
                alvo.receberDano(dano);
            }
            break;
        }
        case 2: { // Bola de Fogo (Dano em Área)
            if (recursoAtual >= 4) {
                recursoAtual -= 4;
                cout << "\n" << Cor::VERMELHO << "🔥 [HABILIDADE: BOLA DE FOGO] " << nome << " canaliza um turbilhão chamejante!" << Cor::RESET << endl;
                if (testarAcerto(alvo.getResistencia())) {
                    int dano = gerarNumeroAleatorio(20, 32);
                    alvo.receberDano(dano);
                }

                if (capangas != nullptr && !capangas->empty()) {
                    cout << "\n" << Cor::AMARELO << "  💥 [DANO EM ÁREA] A onda de chamas atinge todos os esqueletos na arena!" << Cor::RESET << endl;
                    for (size_t i = 0; i < capangas->size(); i++) {
                        if ((*capangas)[i] != nullptr && (*capangas)[i]->estaVivo()) {
                            int danoAoE = gerarNumeroAleatorio(14, 22);
                            cout << "  -> Esqueleto " << (i + 1) << " foi queimado pela Bola de Fogo!" << endl;
                            (*capangas)[i]->receberDano(danoAoE);
                        }
                    }
                }
            } else cout << "\n" << Cor::VERMELHO << "[FALHA] Mana insuficiente! (Necessário: 4 Mana)" << Cor::RESET << endl;
            break;
        }
        case 3: { // Curar Ferimentos
            if (recursoAtual >= 2) {
                recursoAtual -= 2;
                int cura = gerarNumeroAleatorio(16, 24);
                curarPV(cura);
                cout << "\n" << Cor::VERDE << "✨ [HABILIDADE: CURAR FERIMENTOS] " << nome << " regenerou " << cura << " PVs!" << Cor::RESET << endl;
            } else cout << "\n" << Cor::VERMELHO << "[FALHA] Mana insuficiente! (Necessário: 2 Mana)" << Cor::RESET << endl;
            break;
        }
        case 4: { // Armadura Arcana
            if (recursoAtual >= 3) {
                recursoAtual -= 3;
                aplicarArmaduraArcana(3);
                cout << "\n" << Cor::AZUL << "🛡️ [HABILIDADE: ARMADURA ARCANA] " << nome << " ergueu um escudo místico (+8 Defesa por 3 turnos)!" << Cor::RESET << endl;
            } else cout << "\n" << Cor::VERMELHO << "[FALHA] Mana insuficiente! (Necessário: 3 Mana)" << Cor::RESET << endl;
            break;
        }
        case 5: { // Paralisia
            if (recursoAtual >= 5) {
                recursoAtual -= 5;
                cout << "\n" << Cor::AMARELO << "⚡ [HABILIDADE: PARALISIA] " << nome << " dispara relâmpagos imobilizadores contra " << alvo.getNome() << "!" << Cor::RESET << endl;
                if (gerarNumeroAleatorio(1, 100) <= 50) {
                    alvo.aplicarAtordoamento(1);
                    alvo.aplicarMultiplicadorDano(1.5);
                    cout << Cor::VERDE << "  -> SUCCESS! " << alvo.getNome() << " foi PARALISADO por 1 turno e sofrerá +50% de dano!" << Cor::RESET << endl;
                } else cout << Cor::VERMELHO << "  -> " << alvo.getNome() << " resistiu à Paralisia!" << Cor::RESET << endl;
            } else cout << "\n" << Cor::VERMELHO << "[FALHA] Mana insuficiente! (Necessário: 5 Mana)" << Cor::RESET << endl;
            break;
        }
        default:
            cout << "\n" << Cor::VERMELHO << "[AVISO] Número de habilidade inválido!" << Cor::RESET << endl;
            break;
    }
}

// ============================================================================
// IMPLEMENTAÇÃO: Bárbaro (Herói Concreto REBALANCEADO)
// Atributos: PV: 120 | Energia: 16 | Def: 17 | Res: 15 | Acerto: +7
// ============================================================================

Barbaro::Barbaro(string nome) : Jogador(nome, 120, 17, 15, 7, 16) {}
Barbaro::~Barbaro() {}

void Barbaro::exibirStatus() const {
    cout << Cor::MAGENTA << Cor::NEGRITO << "🪓 [BÁRBARO DO NEXUS] " << nome << Cor::RESET << " | ";
    desenharBarraVida("PV", pvAtual, pvMaximo, 10);
    cout << " | ";
    desenharBarraRecurso("ENERGIA", recursoAtual, recursoMaximo, Cor::MAGENTA, 8);
    cout << " | DEF: " << getDefesaEfetiva() 
         << " | ACERTO: +" << getBonusAcertoEfetivo() << endl;
}

void Barbaro::exibirMenuHabilidades() const {
    cout << "\n" << Cor::MAGENTA << Cor::NEGRITO << "==================================================" << endl;
    cout << "        🪓 HABILIDADES DO BÁRBARO DO NEXUS 🪓       " << endl;
    cout << "==================================================" << Cor::RESET << endl;
    cout << "1. Espada             [Custo: 0 Energia | Dano: 8-14 vs Defesa]" << endl;
    cout << "2. Ataque Duplo       [Custo: 2 Energia | Dano: 14-22 vs Defesa]" << endl;
    cout << "3. Ataque Giratório   [Custo: 4 Energia | Dano: 20-30 vs Defesa (" << Cor::VERMELHO << "💥 DANO EM ÁREA" << Cor::RESET << ")]" << endl;
    cout << "4. Atordoar           [Custo: 3 Energia | 60% Atordoar por 1 turno]" << endl;
    cout << Cor::MAGENTA << "==================================================" << Cor::RESET << endl;
    cout << "Escolha a habilidade: ";
}

void Barbaro::executarAcao(Personagem& alvo, int escolhaAcao, vector<Esqueleto*>* capangas) {
    switch (escolhaAcao) {
        case 1: { // Espada
            cout << "\n" << Cor::MAGENTA << "[HABILIDADE: ESPADA] " << nome << " desfechou um golpe pesado de espada!" << Cor::RESET << endl;
            if (testarAcerto(alvo.getDefesaEfetiva())) {
                int dano = gerarNumeroAleatorio(8, 14);
                alvo.receberDano(dano);
            }
            break;
        }
        case 2: { // Ataque Duplo
            if (recursoAtual >= 2) {
                recursoAtual -= 2;
                cout << "\n" << Cor::MAGENTA << "⚔️ [HABILIDADE: ATAQUE DUPLO] " << nome << " desfere uma sequência dupla de golpes!" << Cor::RESET << endl;
                if (testarAcerto(alvo.getDefesaEfetiva())) {
                    int dano = gerarNumeroAleatorio(14, 22);
                    alvo.receberDano(dano);
                }
            } else cout << "\n" << Cor::VERMELHO << "[FALHA] Energia insuficiente! (Necessário: 2 Energia)" << Cor::RESET << endl;
            break;
        }
        case 3: { // Ataque Giratório (Dano em Área)
            if (recursoAtual >= 4) {
                recursoAtual -= 4;
                cout << "\n" << Cor::VERMELHO << "🌀 [HABILIDADE: ATAQUE GIRATÓRIO] " << nome << " gira sua arma em um moinho de lâminas!" << Cor::RESET << endl;
                if (testarAcerto(alvo.getDefesaEfetiva())) {
                    int dano = gerarNumeroAleatorio(20, 30);
                    alvo.receberDano(dano);
                }

                if (capangas != nullptr && !capangas->empty()) {
                    cout << "\n" << Cor::AMARELO << "  💥 [DANO EM ÁREA] A lâmina giratória atinge os esqueletos ao redor!" << Cor::RESET << endl;
                    for (size_t i = 0; i < capangas->size(); i++) {
                        if ((*capangas)[i] != nullptr && (*capangas)[i]->estaVivo()) {
                            int danoAoE = gerarNumeroAleatorio(12, 20);
                            cout << "  -> Esqueleto " << (i + 1) << " sofreu o impacto do corte giratório!" << endl;
                            (*capangas)[i]->receberDano(danoAoE);
                        }
                    }
                }
            } else cout << "\n" << Cor::VERMELHO << "[FALHA] Energia insuficiente! (Necessário: 4 Energia)" << Cor::RESET << endl;
            break;
        }
        case 4: { // Atordoar
            if (recursoAtual >= 3) {
                recursoAtual -= 3;
                cout << "\n" << Cor::AMARELO << "🔨 [HABILIDADE: ATORDOAR] " << nome << " golpeia a cabeça de " << alvo.getNome() << "!" << Cor::RESET << endl;
                if (gerarNumeroAleatorio(1, 100) <= 60) {
                    alvo.aplicarAtordoamento(1);
                    cout << Cor::VERDE << "  -> SUCCESS! " << alvo.getNome() << " ficou ATORDOADO por 1 turno!" << Cor::RESET << endl;
                } else cout << Cor::VERMELHO << "  -> " << alvo.getNome() << " desviou do golpe de atordoamento!" << Cor::RESET << endl;
            } else cout << "\n" << Cor::VERMELHO << "[FALHA] Energia insuficiente! (Necessário: 3 Energia)" << Cor::RESET << endl;
            break;
        }
        default:
            cout << "\n" << Cor::VERMELHO << "[AVISO] Número de habilidade inválido!" << Cor::RESET << endl;
            break;
    }
}

// ============================================================================
// IMPLEMENTAÇÃO: Arqueiro (Herói Concreto REBALANCEADO)
// Atributos: PV: 95 | Energia: 18 | Def: 14 | Res: 18 | Acerto: +8
// ============================================================================

Arqueiro::Arqueiro(string nome) : Jogador(nome, 95, 14, 18, 8, 18) {}
Arqueiro::~Arqueiro() {}

void Arqueiro::exibirStatus() const {
    cout << Cor::CIANO << Cor::NEGRITO << "🏹 [ARQUEIRO DA MATRIZ] " << nome << Cor::RESET << " | ";
    desenharBarraVida("PV", pvAtual, pvMaximo, 10);
    cout << " | ";
    desenharBarraRecurso("ENERGIA", recursoAtual, recursoMaximo, Cor::CIANO, 8);
    cout << " | DEF: " << getDefesaEfetiva() 
         << " | ACERTO: +" << getBonusAcertoEfetivo() << endl;
}

void Arqueiro::exibirMenuHabilidades() const {
    cout << "\n" << Cor::CIANO << Cor::NEGRITO << "==================================================" << endl;
    cout << "       🏹 HABILIDADES DO ARQUEIRO DA MATRIZ 🏹     " << endl;
    cout << "==================================================" << Cor::RESET << endl;
    cout << "1. Flechada           [Custo: 0 Energia | Dano: 9-13 vs Defesa]" << endl;
    cout << "2. Flecha Explosiva   [Custo: 2 Energia | Dano: 14-20 vs Defesa (" << Cor::VERMELHO << "💥 DANO EM ÁREA" << Cor::RESET << ")]" << endl;
    cout << "3. Esconder           [Custo: 3 Energia | 75% Esquiva Total por 1 turno]" << endl;
    cout << "4. Ataque Furtivo     [Custo: 5 Energia | Dano: 25-30 vs Resistência]" << endl;
    cout << Cor::CIANO << "==================================================" << Cor::RESET << endl;
    cout << "Escolha a habilidade: ";
}

void Arqueiro::executarAcao(Personagem& alvo, int escolhaAcao, vector<Esqueleto*>* capangas) {
    switch (escolhaAcao) {
        case 1: { // Flechada
            cout << "\n" << Cor::CIANO << "[HABILIDADE: FLECHADA] " << nome << " puxa a corda e dispara com precisão!" << Cor::RESET << endl;
            if (testarAcerto(alvo.getDefesaEfetiva())) {
                int dano = gerarNumeroAleatorio(9, 13);
                alvo.receberDano(dano);
            }
            break;
        }
        case 2: { // Flecha Explosiva (Dano em Área)
            if (recursoAtual >= 2) {
                recursoAtual -= 2;
                cout << "\n" << Cor::VERMELHO << "💣 [HABILIDADE: FLECHA EXPLOSIVA] " << nome << " dispara uma flecha pontada com pólvora!" << Cor::RESET << endl;
                if (testarAcerto(alvo.getDefesaEfetiva())) {
                    int dano = gerarNumeroAleatorio(14, 20);
                    alvo.receberDano(dano);
                }

                if (capangas != nullptr && !capangas->empty()) {
                    cout << "\n" << Cor::AMARELO << "  💥 [DANO EM ÁREA] A onda de choque da Flecha Explosiva devasta os esqueletos!" << Cor::RESET << endl;
                    for (size_t i = 0; i < capangas->size(); i++) {
                        if ((*capangas)[i] != nullptr && (*capangas)[i]->estaVivo()) {
                            int danoAoE = gerarNumeroAleatorio(10, 16);
                            cout << "  -> Esqueleto " << (i + 1) << " foi atingido pela explosão!" << endl;
                            (*capangas)[i]->receberDano(danoAoE);
                        }
                    }
                }
            } else cout << "\n" << Cor::VERMELHO << "[FALHA] Energia insuficiente! (Necessário: 2 Energia)" << Cor::RESET << endl;
            break;
        }
        case 3: { // Esconder
            if (recursoAtual >= 3) {
                recursoAtual -= 3;
                cout << "\n" << Cor::CIANO << "🥷 [HABILIDADE: ESCONDER] " << nome << " tenta desaparecer nas sombras do terminal!" << Cor::RESET << endl;
                if (gerarNumeroAleatorio(1, 100) <= 75) {
                    aplicarEscondido(true);
                    cout << Cor::VERDE << "  -> SUCCESS! " << nome << " ficou ESCONDIDO (Imune a ataques neste turno)!" << Cor::RESET << endl;
                } else cout << Cor::VERMELHO << "  -> Tentativa de se esconder falhou!" << Cor::RESET << endl;
            } else cout << "\n" << Cor::VERMELHO << "[FALHA] Energia insuficiente! (Necessário: 3 Energia)" << Cor::RESET << endl;
            break;
        }
        case 4: { // Ataque Furtivo
            if (recursoAtual >= 5) {
                recursoAtual -= 5;
                cout << "\n" << Cor::AMARELO << "🎯 [HABILIDADE: ATAQUE FURTIVO] " << nome << " mira no ponto fraco vital de " << alvo.getNome() << "!" << Cor::RESET << endl;
                if (testarAcerto(alvo.getResistencia())) {
                    int dano = gerarNumeroAleatorio(25, 30);
                    alvo.receberDano(dano);
                }
            } else cout << "\n" << Cor::VERMELHO << "[FALHA] Energia insuficiente! (Necessário: 5 Energia)" << Cor::RESET << endl;
            break;
        }
        default:
            cout << "\n" << Cor::VERMELHO << "[AVISO] Número de habilidade inválido!" << Cor::RESET << endl;
            break;
    }
}
