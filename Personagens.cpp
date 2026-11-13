#include "Personagens.h"

// ============================================================================
// IMPLEMENTAÇÃO: Personagem (Classe Base Abstrata)
// ============================================================================

// ----------------------------------------------------------------------------
// Construtor de Personagem: Usa Lista de Inicialização (Member Initialization List)
// Sintaxe com ':': Inicializa os atributos diretamente no momento em que a memória
// é alocada na Stack, garantindo maior eficiência e prevenindo lixo de memória.
// ----------------------------------------------------------------------------
Personagem::Personagem(string nome, int pvMax, int def, int res, int bonusAcerto)
    : nome(nome), pvAtual(pvMax), pvMaximo(pvMax), defesa(def), resistencia(res), bonusAcerto(bonusAcerto),
      turnosAtordoado(0), turnosArmaduraArcana(0), estaEscondido(false), multiplicadorDanoRecebido(1.0),
      modBonusAcertoTemp(0), turnosModAcerto(0) {}

// Destrutor da base: Corpo vazio pois não alocamos ponteiros brutos diretamente na base.
Personagem::~Personagem() {}

// ----------------------------------------------------------------------------
// Método Auxiliar: gerarNumeroAleatorio
// Sorteia um número inteiro entre [min, max] inclusive.
// Utiliza a fórmula padrão do C++: min + (rand() % (intervalo + 1))
// ----------------------------------------------------------------------------
int Personagem::gerarNumeroAleatorio(int min, int max) const {
    return min + (rand() % (max - min + 1));
}

// ----------------------------------------------------------------------------
// Método: testarAcerto (Sistema d20 Balanceado)
// Executa a fórmula: Resultado = d20 (1 a 20) + bonusAcertoEfetivo
// Se Resultado >= defesaOuResistenciaAlvo -> Retorna true (Acertou o golpe)
// Se Resultado < defesaOuResistenciaAlvo  -> Retorna false (Errou o golpe)
// ----------------------------------------------------------------------------
bool Personagem::testarAcerto(int defesaOuResistenciaAlvo) const {
    int d20 = gerarNumeroAleatorio(1, 20);
    int bonusEfetivo = getBonusAcertoEfetivo();
    int total = d20 + bonusEfetivo;

    cout << "  [D20] " << nome << " rolou d20 (" << d20 << ") + Bônus (" << bonusEfetivo 
         << ") = " << total << " (vs Dificuldade " << defesaOuResistenciaAlvo << ")" << endl;

    return total >= defesaOuResistenciaAlvo;
}

// ----------------------------------------------------------------------------
// Método: processarInicioTurno
// Executado no início da rodada para decrementar contadores de buffs/debuffs temporários.
// ----------------------------------------------------------------------------
void Personagem::processarInicioTurno() {
    // Reduz o atordoamento se o personagem estiver paralisado
    if (turnosAtordoado > 0) {
        turnosAtordoado--;
    }

    // Reduz a duração da Armadura Arcana e notifica a dissipação
    if (turnosArmaduraArcana > 0) {
        turnosArmaduraArcana--;
        if (turnosArmaduraArcana == 0) {
            cout << "  -> A Armadura Arcana de " << nome << " se dissipou." << endl;
        }
    }

    // Reduz a duração de modificadores temporários de acerto (ex: Presença de Luz de Baal)
    if (turnosModAcerto > 0) {
        turnosModAcerto--;
        if (turnosModAcerto == 0) {
            cout << "  -> O modificador temporário de acerto de " << nome << " expirou." << endl;
            modBonusAcertoTemp = 0;
        }
    }

    // Imunidade da habilidade 'Esconder' expira no início do turno
    if (estaEscondido) {
        estaEscondido = false;
    }
}

// ----------------------------------------------------------------------------
// Método: receberDano
// Processa a aplicação do dano sofrido com regras de mitigação e vulnerabilidade:
// 1. Se estiver Escondido: esquiva total de dano.
// 2. Mitigação: Dano Efetivo = Dano Bruto - (Defesa Efetiva / 2).
// 3. Dano mínimo garantido = 1.
// 4. Vulnerabilidade (Paralisia): multiplica o dano final (ex: x1.5) e consome a marca.
// ----------------------------------------------------------------------------
void Personagem::receberDano(int danoBruto) {
    if (estaEscondido) {
        cout << "  -> " << nome << " estava ESCONDIDO e se esquivou completamente do ataque!" << endl;
        return;
    }

    int defEfetiva = getDefesaEfetiva();
    int danoEfetivo = danoBruto - (defEfetiva / 2);
    if (danoEfetivo < 1) danoEfetivo = 1; // Garante dano mínimo de 1

    // Aplica multiplicador se o alvo sofreu debuff de vulnerabilidade (ex: Paralisia)
    if (multiplicadorDanoRecebido > 1.0) {
        danoEfetivo = static_cast<int>(danoEfetivo * multiplicadorDanoRecebido);
        cout << "  -> [GOLPE VULNERÁVEL] Dano ampliado!" << endl;
        multiplicadorDanoRecebido = 1.0; // Consome o efeito de vulnerabilidade
    }

    pvAtual -= danoEfetivo;
    if (pvAtual < 0) pvAtual = 0; // Tranca a vida em zero (sem vida negativa)

    cout << "  -> " << nome << " sofreu " << danoEfetivo 
         << " de dano! (PV: " << pvAtual << "/" << pvMaximo << ")" << endl;
}

// ----------------------------------------------------------------------------
// Método: curarPV
// Restaura a vida da entidade sem ultrapassar o limite de pvMaximo.
// ----------------------------------------------------------------------------
void Personagem::curarPV(int quantidade) {
    pvAtual += quantidade;
    if (pvAtual > pvMaximo) pvAtual = pvMaximo;
}

// Setters para aplicação de efeitos de status
void Personagem::aplicarAtordoamento(int turnos) { turnosAtordoado = turnos; }
void Personagem::aplicarArmaduraArcana(int turnos) { turnosArmaduraArcana = turnos; }
void Personagem::aplicarEscondido(bool estado) { estaEscondido = estado; }
void Personagem::aplicarMultiplicadorDano(double mult) { multiplicadorDanoRecebido = mult; }

// Aplica modificadores temporários de acerto (ex: -3 por Presença de Luz ou +2 por Bater Escudo)
void Personagem::aplicarModificadorAcerto(int mod, int turnos) {
    modBonusAcertoTemp = mod;
    turnosModAcerto = turnos;
    if (mod < 0) {
        cout << "  -> " << nome << " sofreu penalidade de acerto (" << mod << ") por " << turnos << " turnos!" << endl;
    } else {
        cout << "  -> " << nome << " recebeu bônus de acerto (+" << mod << ") por " << turnos << " turnos!" << endl;
    }
}

// Consultores (Getters) de Leitura
bool Personagem::estaAtordoado() const { return turnosAtordoado > 0; }
bool Personagem::getEstaEscondido() const { return estaEscondido; }
bool Personagem::estaVivo() const { return pvAtual > 0; }
string Personagem::getNome() const { return nome; }
int Personagem::getPvAtual() const { return pvAtual; }
int Personagem::getPvMaximo() const { return pvMaximo; }

// Retorna a defesa calculando o bônus temporário da Armadura Arcana
int Personagem::getDefesaEfetiva() const { 
    return defesa + (turnosArmaduraArcana > 0 ? 5 : 0); 
}

int Personagem::getResistencia() const { return resistencia; }
int Personagem::getBonusAcertoBase() const { return bonusAcerto; }

// Retorna o bônus de acerto efetivo considerando eventuais modificadores temporários
int Personagem::getBonusAcertoEfetivo() const { 
    return bonusAcerto + (turnosModAcerto > 0 ? modBonusAcertoTemp : 0); 
}

// ============================================================================
// IMPLEMENTAÇÃO: Jogador (Subclasse Intermediária)
// ============================================================================

Jogador::Jogador(string nome, int pvMax, int def, int res, int bonusAcerto, int recursoMax)
    : Personagem(nome, pvMax, def, res, bonusAcerto), recursoAtual(recursoMax), recursoMaximo(recursoMax) {
    inicializarBolsaInicial();
}

Jogador::~Jogador() { bolsa.clear(); }

// ----------------------------------------------------------------------------
// Método: evoluirNoNexus
// Chamado ao derrotar qualquer um dos 3 chefes primários no Nexus da Compilação.
// Incrementa permanentemente o PV Máximo (+15) e o limite de recurso da classe (+3),
// restaurando 100% da saúde e do recurso do herói.
// ----------------------------------------------------------------------------
void Jogador::evoluirNoNexus() {
    pvMaximo += 15;
    recursoMaximo += 3; // Eleva o teto máximo de Mana ou Energia da classe do jogador
    pvAtual = pvMaximo;
    recursoAtual = recursoMaximo;

    cout << "\n==================================================" << endl;
    cout << "          EVOLUÇÃO CONCEDIDA NO NEXUS!            " << endl;
    cout << "==================================================" << endl;
    cout << "Sua vitória fortaleceu seu código digital:" << endl;
    cout << "  +15 PV Máximo   -> Teto de Vida: " << pvMaximo << endl;
    cout << "  +3 Recurso Máx  -> Teto de Recursos: " << recursoMaximo << endl;
    cout << "  (Vida e Recursos totalmente restaurados!)" << endl;
    cout << "==================================================\n" << endl;
}

void Jogador::inicializarBolsaInicial() {
    bolsa.clear();
    bolsa.push_back({"Poção de Cura", "Recupera 14 PVs do jogador.", 1, 14});
    bolsa.push_back({"Poção de Cura", "Recupera 14 PVs do jogador.", 1, 14});
    bolsa.push_back({"Poção de Mana", "Recupera 9 de Mana/Energia do jogador.", 2, 9});
    bolsa.push_back({"Poção de Mana", "Recupera 9 de Mana/Energia do jogador.", 2, 9});
    bolsa.push_back({"Bomba de Fogo", "Causa 28 de dano direto a um inimigo.", 3, 28});
}

void Jogador::exibirExplicacaoItensIniciais() const {
    cout << "\n==================================================" << endl;
    cout << "       SUPRIMENTOS DE COMBATE RECEBIDOS           " << endl;
    cout << "==================================================" << endl;
    cout << "Ao despertar no Nexus da Compilação, você encontra" << endl;
    cout << "uma bolsa contendo 5 itens fundamentais:" << endl;
    cout << "  1. [2x] POÇÃO DE CURA  -> Recupera 14 PVs." << endl;
    cout << "  2. [2x] POÇÃO DE MANA  -> Recupera 9 de Mana/Energia." << endl;
    cout << "  3. [1x] BOMBA DE FOGO  -> Infringe 28 de dano no alvo." << endl;
    cout << "==================================================\n" << endl;
}

void Jogador::exibirBolsa() const {
    cout << "\n--- BOLSA DE ITENS DE " << nome << " ---" << endl;
    if (bolsa.empty()) {
        cout << " (Sua bolsa está vazia!)" << endl;
        return;
    }
    for (size_t i = 0; i < bolsa.size(); i++) {
        cout << " " << (i + 1) << ". " << bolsa[i].nome 
             << " - " << bolsa[i].descricao << endl;
    }
}

bool Jogador::usarItem(int indice, Personagem& alvo) {
    if (indice < 0 || indice >= static_cast<int>(bolsa.size())) {
        cout << "\n[AVISO] Posição de item inválida!" << endl;
        return false;
    }

    Item itemUsado = bolsa[indice];

    if (itemUsado.tipo == 1) { // Poção de Cura (PV)
        curarPV(itemUsado.valorEfeito);
        cout << "\n[ITEM] " << nome << " usou " << itemUsado.nome 
             << " e recuperou " << itemUsado.valorEfeito << " PVs! (PV: " 
             << pvAtual << "/" << pvMaximo << ")" << endl;
    } 
    else if (itemUsado.tipo == 2) { // Poção de Mana/Energia
        restaurarRecurso(itemUsado.valorEfeito);
        cout << "\n[ITEM] " << nome << " usou " << itemUsado.nome 
             << " e recuperou " << itemUsado.valorEfeito << " de recurso! (Atual: " 
             << recursoAtual << "/" << recursoMaximo << ")" << endl;
    } 
    else if (itemUsado.tipo == 3) { // Bomba de Fogo
        cout << "\n[ITEM] " << nome << " arremessou uma " << itemUsado.nome 
             << " contra " << alvo.getNome() << "!" << endl;
        alvo.receberDano(itemUsado.valorEfeito);
    }

    bolsa.erase(bolsa.begin() + indice);
    return true;
}

void Jogador::adicionarItem(string nome, string desc, int tipo, int valor) {
    bolsa.push_back({nome, desc, tipo, valor});
    cout << "\n[INVENTÁRIO] Item '" << nome << "' adicionado à bolsa!" << endl;
}

void Jogador::restaurarRecurso(int quantidade) {
    recursoAtual += quantidade;
    if (recursoAtual > recursoMaximo) recursoAtual = recursoMaximo;
}

int Jogador::getRecursoAtual() const { return recursoAtual; }
int Jogador::getRecursoMaximo() const { return recursoMaximo; }
int Jogador::getQtdItens() const { return static_cast<int>(bolsa.size()); }

// ============================================================================
// IMPLEMENTAÇÃO: Mago (Herói Concreto)
// Atributos Calibrados: PV: 75 | Mana: 22 | Def: 8 | Res: 10 | Bônus Acerto: +7
// ============================================================================

Mago::Mago(string nome) : Jogador(nome, 75, 8, 10, 7, 22) {}
Mago::~Mago() {}

void Mago::exibirStatus() const {
    cout << "\n[MAGO DA COMPILAÇÃO] " << nome << " | PV: " << pvAtual << "/" << pvMaximo 
         << " | MANA: " << recursoAtual << "/" << recursoMaximo 
         << " | DEF: " << getDefesaEfetiva() 
         << " | BÔNUS ACERTO: +" << getBonusAcertoEfetivo() << endl;
}

// ----------------------------------------------------------------------------
// Método: executarAcao (Mago)
// Processa as habilidades balanceadas do Mago com o sistema de d20 (Bônus +7):
// 1. Super Soco: 1d20 + 7 contra Defesa do alvo (Custo: 0 | Dano: 2-5)
// 2. Bola de Fogo: 1d20 + 7 contra Resistência do alvo (Custo: 4 Mana | Dano: 18-30)
// 3. Curar Ferimentos: Acerto automático (Custo: 2 Mana | Cura: 12-20 PV)
// 4. Armadura Arcana: Acerto automático (Custo: 3 Mana | +5 Defesa por 3 turnos)
// 5. Paralisia: Chance fixa de 50% (Custo: 5 Mana | Atordoa 1 turno + 50% dano extra)
// ----------------------------------------------------------------------------
void Mago::executarAcao(Personagem& alvo, int escolhaAcao) {
    switch (escolhaAcao) {
        case 1: { // Super Soco (1d20 + 7 vs Defesa)
            if (testarAcerto(alvo.getDefesaEfetiva())) {
                int dano = gerarNumeroAleatorio(2, 5);
                cout << "\n[ATAQUE] " << nome << " acertou um SUPER SOCO!" << endl;
                alvo.receberDano(dano);
            } else cout << "\n[ERROU] " << nome << " errou o Super Soco!" << endl;
            break;
        }
        case 2: { // Bola de Fogo (1d20 + 7 vs Resistência)
            if (recursoAtual >= 4) {
                recursoAtual -= 4;
                if (testarAcerto(alvo.getResistencia())) {
                    int dano = gerarNumeroAleatorio(18, 30);
                    cout << "\n[MAGIA] " << nome << " lançou BOLA DE FOGO!" << endl;
                    alvo.receberDano(dano);
                } else cout << "\n[ERROU] Bola de Fogo passou de raspão!" << endl;
            } else cout << "\n[FALHA] Mana insuficiente!" << endl;
            break;
        }
        case 3: { // Curar Ferimentos (Uso pessoal / Acerto automático)
            if (recursoAtual >= 2) {
                recursoAtual -= 2;
                int cura = gerarNumeroAleatorio(12, 20);
                curarPV(cura);
                cout << "\n[MAGIA] " << nome << " usou CURAR FERIMENTOS (+ " << cura << " PVs)!" << endl;
            } else cout << "\n[FALHA] Mana insuficiente!" << endl;
            break;
        }
        case 4: { // Armadura Arcana (Buff pessoal / Acerto automático)
            if (recursoAtual >= 3) {
                recursoAtual -= 3;
                aplicarArmaduraArcana(3);
                cout << "\n[MAGIA] " << nome << " ergueu uma ARMADURA ARCANA (+5 Defesa por 3 turnos)!" << endl;
            } else cout << "\n[FALHA] Mana insuficiente!" << endl;
            break;
        }
        case 5: { // Paralisia (Porcentagem fixa: 50%)
            if (recursoAtual >= 5) {
                recursoAtual -= 5;
                if (gerarNumeroAleatorio(1, 100) <= 50) {
                    alvo.aplicarAtordoamento(1);
                    alvo.aplicarMultiplicadorDano(1.5);
                    cout << "\n[MAGIA] " << nome << " PARALISOU " << alvo.getNome() << "! (Perde 1 turno e sofre +50% dano!)" << endl;
                } else cout << "\n[FALHA] Paralisia resistida pelo alvo!" << endl;
            } else cout << "\n[FALHA] Mana insuficiente!" << endl;
            break;
        }
    }
}

// ============================================================================
// IMPLEMENTAÇÃO: Bárbaro (Herói Concreto)
// Atributos Calibrados: PV: 112 | Energia: 14 | Def: 16 | Res: 15 | Bônus Acerto: +7
// ============================================================================

Barbaro::Barbaro(string nome) : Jogador(nome, 112, 16, 15, 7, 14) {}
Barbaro::~Barbaro() {}

void Barbaro::exibirStatus() const {
    cout << "\n[BÁRBARO DO NEXUS] " << nome << " | PV: " << pvAtual << "/" << pvMaximo 
         << " | ENERGIA: " << recursoAtual << "/" << recursoMaximo 
         << " | DEF: " << getDefesaEfetiva() 
         << " | BÔNUS ACERTO: +" << getBonusAcertoEfetivo() << endl;
}

// ----------------------------------------------------------------------------
// Método: executarAcao (Bárbaro)
// Processa as habilidades balanceadas do Bárbaro com o sistema de d20 (Bônus +7):
// 1. Espada: 1d20 + 7 contra Defesa do alvo (Custo: 0 | Dano: 6-12)
// 2. Ataque Duplo: 1d20 + 7 contra Defesa do alvo (Custo: 2 Energia | Dano: 12-20)
// 3. Ataque Triplo: 1d20 + 7 contra Defesa do alvo (Custo: 4 Energia | Dano: 18-28)
// 4. Atordoar: Porcentagem fixa de 60% (Custo: 3 Energia | Atordoa por 1 turno)
// ----------------------------------------------------------------------------
void Barbaro::executarAcao(Personagem& alvo, int escolhaAcao) {
    switch (escolhaAcao) {
        case 1: { // Espada (1d20 + 7 vs Defesa)
            if (testarAcerto(alvo.getDefesaEfetiva())) {
                int dano = gerarNumeroAleatorio(6, 12);
                cout << "\n[ATAQUE] " << nome << " atacou com a ESPADA!" << endl;
                alvo.receberDano(dano);
            } else cout << "\n[ERROU] Ataque de espada bloqueado!" << endl;
            break;
        }
        case 2: { // Ataque Duplo (1d20 + 7 vs Defesa)
            if (recursoAtual >= 2) {
                recursoAtual -= 2;
                if (testarAcerto(alvo.getDefesaEfetiva())) {
                    int dano = gerarNumeroAleatorio(12, 20);
                    cout << "\n[HABILIDADE] " << nome << " executou ATAQUE DUPLO!" << endl;
                    alvo.receberDano(dano);
                } else cout << "\n[ERROU] Ataque Duplo errou o alvo!" << endl;
            } else cout << "\n[FALHA] Energia insuficiente!" << endl;
            break;
        }
        case 3: { // Ataque Triplo (1d20 + 7 vs Defesa)
            if (recursoAtual >= 4) {
                recursoAtual -= 4;
                if (testarAcerto(alvo.getDefesaEfetiva())) {
                    int dano = gerarNumeroAleatorio(18, 28);
                    cout << "\n[HABILIDADE] " << nome << " executou ATAQUE TRIPLO!" << endl;
                    alvo.receberDano(dano);
                } else cout << "\n[ERROU] Ataque Triplo errou!" << endl;
            } else cout << "\n[FALHA] Energia insuficiente!" << endl;
            break;
        }
        case 4: { // Atordoar (Porcentagem fixa: 60%)
            if (recursoAtual >= 3) {
                recursoAtual -= 3;
                if (gerarNumeroAleatorio(1, 100) <= 60) {
                    alvo.aplicarAtordoamento(1);
                    cout << "\n[HABILIDADE] " << nome << " ATORDOU " << alvo.getNome() << " por 1 turno!" << endl;
                } else cout << "\n[FALHA] Golpe de atordoamento esquivado!" << endl;
            } else cout << "\n[FALHA] Energia insuficiente!" << endl;
            break;
        }
    }
}

// ============================================================================
// IMPLEMENTAÇÃO: Arqueiro (Herói Concreto)
// Atributos Calibrados: PV: 88 | Energia: 16 | Def: 13 | Res: 17 | Bônus Acerto: +8
// ============================================================================

Arqueiro::Arqueiro(string nome) : Jogador(nome, 88, 13, 17, 8, 16) {}
Arqueiro::~Arqueiro() {}

void Arqueiro::exibirStatus() const {
    cout << "\n[ARQUEIRO DA MATRIZ] " << nome << " | PV: " << pvAtual << "/" << pvMaximo 
         << " | ENERGIA: " << recursoAtual << "/" << recursoMaximo 
         << " | DEF: " << getDefesaEfetiva() 
         << " | BÔNUS ACERTO: +" << getBonusAcertoEfetivo() << endl;
}

// ----------------------------------------------------------------------------
// Método: executarAcao (Arqueiro)
// Processa as habilidades balanceadas do Arqueiro com o sistema de d20 (Bônus +8):
// 1. Flechada: 1d20 + 8 contra Defesa do alvo (Custo: 0 | Dano: 7-10)
// 2. Flecha Explosiva: 1d20 + 8 contra Defesa do alvo (Custo: 2 Energia | Dano: 10-15)
// 3. Esconder: Porcentagem fixa de 60% (Custo: 3 Energia | Imunidade por 1 turno)
// 4. Ataque Furtivo: 1d20 + 8 contra Resistência do alvo (Custo: 5 Energia | Dano: 25-30)
// ----------------------------------------------------------------------------
void Arqueiro::executarAcao(Personagem& alvo, int escolhaAcao) {
    switch (escolhaAcao) {
        case 1: { // Flechada (1d20 + 8 vs Defesa)
            if (testarAcerto(alvo.getDefesaEfetiva())) {
                int dano = gerarNumeroAleatorio(7, 10);
                cout << "\n[ATAQUE] " << nome << " disparou uma FLECHADA!" << endl;
                alvo.receberDano(dano);
            } else cout << "\n[ERROU] Flecha passou longe!" << endl;
            break;
        }
        case 2: { // Flecha Explosiva (1d20 + 8 vs Defesa)
            if (recursoAtual >= 2) {
                recursoAtual -= 2;
                if (testarAcerto(alvo.getDefesaEfetiva())) {
                    int dano = gerarNumeroAleatorio(10, 15);
                    cout << "\n[HABILIDADE] " << nome << " disparou FLECHA EXPLOSIVA!" << endl;
                    alvo.receberDano(dano);
                } else cout << "\n[ERROU] Flecha explosiva errou o alvo!" << endl;
            } else cout << "\n[FALHA] Energia insuficiente!" << endl;
            break;
        }
        case 3: { // Esconder (Porcentagem fixa: 60%)
            if (recursoAtual >= 3) {
                recursoAtual -= 3;
                if (gerarNumeroAleatorio(1, 100) <= 60) {
                    aplicarEscondido(true);
                    cout << "\n[HABILIDADE] " << nome << " ESCONDEU-SE nas sombras (Imune a ataques por 1 turno)!" << endl;
                } else cout << "\n[FALHA] Tentativa de se esconder falhou!" << endl;
            } else cout << "\n[FALHA] Energia insuficiente!" << endl;
            break;
        }
        case 4: { // Ataque Furtivo (1d20 + 8 vs Resistência)
            if (recursoAtual >= 5) {
                recursoAtual -= 5;
                if (testarAcerto(alvo.getResistencia())) {
                    int dano = gerarNumeroAleatorio(25, 30);
                    cout << "\n[HABILIDADE] " << nome << " desferiu um ATAQUE FURTIVO!" << endl;
                    alvo.receberDano(dano);
                } else cout << "\n[ERROU] Ataque furtivo bloqueado!" << endl;
            } else cout << "\n[FALHA] Energia insuficiente!" << endl;
            break;
        }
    }
}
