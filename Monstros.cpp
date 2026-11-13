#include "Monstros.h"

// ============================================================================
// IMPLEMENTAÇÃO: Monstro (Classe Base de Inimigos)
// ============================================================================

Monstro::Monstro(string nome, int pvMax, int def, int res, int bonusAcerto, string titulo)
    : Personagem(nome, pvMax, def, res, bonusAcerto), tituloArea(titulo) {}

Monstro::~Monstro() {}

void Monstro::executarAcao(Personagem& alvo, int escolhaAcao, vector<Esqueleto*>* capangas) {
    (void)escolhaAcao;
    (void)capangas;
    executarTurnoIA(alvo);
}

void Monstro::exibirStatus() const {
    cout << Cor::VERMELHO << Cor::NEGRITO << "👾 [" << tituloArea << "] " << nome << Cor::RESET << " | ";
    desenharBarraVida("PV", pvAtual, pvMaximo, 10);
    cout << " | DEF: " << getDefesaEfetiva() 
         << " | RES: " << getResistencia() 
         << " | ACERTO: +" << getBonusAcertoEfetivo() << endl;
}

string Monstro::getTituloArea() const {
    return tituloArea;
}

// ============================================================================
// IMPLEMENTAÇÃO: Dragão Kaido (DANO AJUSTADO)
// Atributos: PV: 130 | Def: 16 | Res: 16 | Acerto: +9
// ============================================================================

DragaoKaido::DragaoKaido()
    : Monstro("Dragão Kaido", 130, 16, 16, 9, "CHEFE - FORNALHA CINZENTA") {}

DragaoKaido::~DragaoKaido() {}

void DragaoKaido::exibirEntradaCinematica() const {
    limparTela();
    desenharKaidoASCII();
    cout << Cor::VERMELHO << Cor::NEGRITO << "==================================================" << endl;
    cout << "        🔥 ENTRANDO NA FORNALHA CINZENTA 🔥       " << endl;
    cout << "==================================================" << Cor::RESET << endl;
    digitarTexto(Cor::AMARELO + "Seu corpo se desmaterializa e se materializa na cratera de um vulcão ativo.", 18);
    digitarTexto(Cor::AMARELO + "Rios de magma borbulhante cortam o chão de pedra negra ressecada.", 18);
    digitarTexto(Cor::AMARELO + "No topo de uma pilha de cinzas flamejantes, o colossal Dragão Kaido desperta!", 18);
    cout << endl;
    digitarTexto(Cor::VERMELHO + Cor::NEGRITO + "Kaido: "Inseto audacioso... ousais adentrar meu domínio de fogo?", 22);
    digitarTexto(Cor::VERMELHO + Cor::NEGRITO + "       Será reduzido à cinzas antes mesmo que tenhas a chance de suplicar por misericórdia, igual a muitos antes de ti!"", 22);
    pausarParaContinuar();
}

void DragaoKaido::executarTurnoIA(Personagem& alvo) {
    int chance = gerarNumeroAleatorio(1, 100);

    if (chance <= 40) { // 40% - Garra
        digitarTexto(Cor::VERMELHO + "Kaido ataca com suas Garras!" + Cor::RESET, 18);
        cout << Cor::VERMELHO << "[INIMIGO] Dragão Kaido avança e ataca com suas GARRAS!" << Cor::RESET << endl;
        if (testarAcerto(alvo.getDefesaEfetiva())) {
            int dano = gerarNumeroAleatorio(11, 16);
            alvo.receberDano(dano);
        }
    } 
    else if (chance <= 70) { // 30% - Mordida
        digitarTexto(Cor::VERMELHO + "Kaido avança com uma Mordida feroz!" + Cor::RESET, 18);
        cout << Cor::VERMELHO << "[INIMIGO] Dragão Kaido desfecha uma MORDIDA FEROZ!" << Cor::RESET << endl;
        if (testarAcerto(alvo.getDefesaEfetiva())) {
            int dano = gerarNumeroAleatorio(15, 20);
            alvo.receberDano(dano);
        }
    } 
    else if (chance <= 90) { // 20% - Presença Aterradora
        digitarTexto(Cor::VERMELHO + "Kaido: "Ajoelha-te perante o verdadeiro terror!"" + Cor::RESET, 18);
        cout << Cor::VERMELHO << "[O rugido do dragão paralisa teu sangue; teus músculos recusam-se a mover!]" << Cor::RESET << endl;
        if (testarAcerto(alvo.getResistencia())) {
            alvo.aplicarAtordoamento(1);
            cout << Cor::VERMELHO << "  -> " << alvo.getNome() << " ficou aterrorizado e PERDEU O PRÓXIMO TURNO!" << Cor::RESET << endl;
        }
    } 
    else { // 10% - Cuspir Fogo
        digitarTexto(Cor::VERMELHO + "Kaido: "Sinta o calor da aniquilação absoluta!"" + Cor::RESET, 18);
        cout << Cor::VERMELHO << "🔥 [Um jorro de chamas puras devora todo o campo de batalha!]" << Cor::RESET << endl;
        if (testarAcerto(alvo.getResistencia())) {
            int dano = gerarNumeroAleatorio(20, 28);
            alvo.receberDano(dano);
        }
    }
}

// ============================================================================
// IMPLEMENTAÇÃO: Esqueleto
// Atributos: PV: 10 | Def: 10 | Res: 8 | Acerto: +7
// ============================================================================

Esqueleto::Esqueleto()
    : Monstro("Esqueleto", 10, 10, 8, 7, "CAPANGA") {}

Esqueleto::~Esqueleto() {}

void Esqueleto::exibirEntradaCinematica() const {
    cout << Cor::MAGENTA << "  💀 Da terra seca, mãos esqueléticas emergem empunhando espadas enferrujadas!" << Cor::RESET << endl;
}

void Esqueleto::executarTurnoIA(Personagem& alvo) {
    int chance = gerarNumeroAleatorio(1, 100);

    if (chance <= 60) {
        cout << Cor::MAGENTA << "[CAPANGA] Esqueleto avança com sua ESPADA ENFERRUJADA!" << Cor::RESET << endl;
        if (testarAcerto(alvo.getDefesaEfetiva())) {
            int dano = gerarNumeroAleatorio(4, 8);
            alvo.receberDano(dano);
        }
    } 
    else {
        cout << Cor::MAGENTA << "[CAPANGA] Esqueleto BATE NO ESCUDO fortalecendo a precisão dos aliados (+2 acerto)!" << Cor::RESET << endl;
        aplicarModificadorAcerto(2, 1);
    }
}

// ============================================================================
// IMPLEMENTAÇÃO: Necromante Lich (DANO AJUSTADO)
// Atributos: PV: 90 | Def: 15 | Res: 15 | Acerto: +10
// ============================================================================

NecromanteLich::NecromanteLich()
    : Monstro("Necromante Lich", 90, 15, 15, 10, "CHEFE - CIDADE SEPULCRAL") {}

NecromanteLich::~NecromanteLich() {
    limparCapangas();
}

void NecromanteLich::exibirEntradaCinematica() const {
    limparTela();
    desenharLichASCII();
    cout << Cor::MAGENTA << Cor::NEGRITO << "==================================================" << endl;
    cout << "         ☠️ ENTRANDO NA CIDADE SEPULCRAL ☠️        " << endl;
    cout << "==================================================" << Cor::RESET << endl;
    digitarTexto(Cor::MAGENTA + "A umidade fria e o aroma de decomposição denunciam o sepulcro sob a terra.", 18);
    digitarTexto(Cor::MAGENTA + "Névoa esverdeada rasteja pelo solo coberto por milhares de crânios estalados.", 18);
    digitarTexto(Cor::MAGENTA + "No centro da mesa de sacrifício, o Lich ergue seu cajado necrótico!", 18);
    cout << endl;
    digitarTexto(Cor::MAGENTA + Cor::NEGRITO + "Lich: "Mais uma alma tola busca descansar nesta cidade decrépita?", 22);
    digitarTexto(Cor::MAGENTA + Cor::NEGRITO + "       Excelente... teus ossos servirão bem ao meu exército de mortos!"", 22);
    pausarParaContinuar();
}

void NecromanteLich::limparCapangas() {
    for (size_t i = 0; i < capangas.size(); i++) {
        delete capangas[i];
    }
    capangas.clear();
}

const vector<Esqueleto*>& NecromanteLich::getCapangas() const {
    return capangas;
}

void NecromanteLich::animarMortos() {
    digitarTexto(Cor::MAGENTA + "Lich: "Ergam-se, meus preciosos mortos! Protejam o vosso mestre!"" + Cor::RESET, 18);
    cout << Cor::MAGENTA << "[Da terra seca, mãos esqueléticas emergem empunhando espadas enferrujadas!]" << Cor::RESET << endl;
    if (capangas.size() >= 4) {
        cout << Cor::CINZA << "  -> A arena já está no limite máximo de 4 Esqueletos!" << Cor::RESET << endl;
        return;
    }

    int invocados = 0;
    while (capangas.size() < 4 && invocados < 2) {
        capangas.push_back(new Esqueleto());
        invocados++;
    }
    cout << Cor::MAGENTA << "  -> " << invocados << " Esqueleto(s) ergueram-se! (Total na arena: " << capangas.size() << ")" << Cor::RESET << endl;
}

void NecromanteLich::executarTurnoIA(Personagem& alvo) {
    int chance = gerarNumeroAleatorio(1, 100);

    if (chance <= 40) {
        digitarTexto(Cor::MAGENTA + "Lich ataca com Punho Pútrido!" + Cor::RESET, 18);
        cout << Cor::MAGENTA << "[INIMIGO] Necromante Lich ataca com PUNHO PÚTRIDO!" << Cor::RESET << endl;
        if (testarAcerto(alvo.getDefesaEfetiva())) {
            int dano = gerarNumeroAleatorio(11, 16);
            alvo.receberDano(dano);
        }
    } 
    else if (chance <= 70) {
        animarMortos();
    } 
    else if (chance <= 90) {
        digitarTexto(Cor::MAGENTA + "Lich dispara Raio da Bruxa!" + Cor::RESET, 18);
        cout << Cor::MAGENTA << "[INIMIGO] Necromante Lich dispara RAIO DA BRUXA!" << Cor::RESET << endl;
        if (testarAcerto(alvo.getDefesaEfetiva())) {
            int dano = gerarNumeroAleatorio(14, 22);
            alvo.receberDano(dano);
        }
    } 
    else {
        digitarTexto(Cor::MAGENTA + "Lich: "Todas as coisas precisam de um fim, e a morte leva tudo e todos!"" + Cor::RESET, 18);
        cout << Cor::MAGENTA << "☠️ [Uma onda de energia pútrida se expande de seu cajado, definhando a vida!]" << Cor::RESET << endl;
        if (testarAcerto(alvo.getResistencia())) {
            int dano = gerarNumeroAleatorio(20, 28);
            alvo.receberDano(dano);
        }
    }

    for (size_t i = 0; i < capangas.size(); i++) {
        if (capangas[i]->estaVivo() && alvo.estaVivo()) {
            cout << "
" << Cor::CINZA << "--- Turno do Capanga " << (i + 1) << " ---" << Cor::RESET << endl;
            capangas[i]->executarTurnoIA(alvo);
        }
    }
}

// ============================================================================
// IMPLEMENTAÇÃO: Anjo Caído Baal (DANO AJUSTADO)
// Atributos: PV: 120 | Def: 16 | Res: 17 | Acerto: +11
// ============================================================================

AnjoCaidoBaal::AnjoCaidoBaal()
    : Monstro("Anjo Caído Baal", 120, 16, 17, 11, "CHEFE - CATEDRAL HERÉTICA") {}

AnjoCaidoBaal::~AnjoCaidoBaal() {}

void AnjoCaidoBaal::exibirEntradaCinematica() const {
    limparTela();
    desenharBaalASCII();
    cout << Cor::AMARELO << Cor::NEGRITO << "==================================================" << endl;
    cout << "        ✨ ENTRANDO NA CATEDRAL HERÉTICA ✨       " << endl;
    cout << "==================================================" << Cor::RESET << endl;
    digitarTexto(Cor::AMARELO + "Vitrais quebrados projetam feixes de luz distorcida sobre o mármore partido.", 18);
    digitarTexto(Cor::AMARELO + "Penas negras flutuam lentamente pelo ar estático da catedral dilapidada.", 18);
    digitarTexto(Cor::AMARELO + "No altar em ruínas, uma figura de beleza aterradora e asas de sombra desce!", 18);
    cout << endl;
    digitarTexto(Cor::AMARELO + Cor::NEGRITO + "Baal: "Graça e julgamento são dois lados da mesma moeda.", 22);
    digitarTexto(Cor::AMARELO + Cor::NEGRITO + "       Pensas que estás pronto para encarar a queda dos céus?"", 22);
    pausarParaContinuar();
}

void AnjoCaidoBaal::executarTurnoIA(Personagem& alvo) {
    int chance = gerarNumeroAleatorio(1, 100);

    if (chance <= 40) {
        digitarTexto(Cor::AMARELO + "Baal ataca com a Espada Negra!" + Cor::RESET, 18);
        cout << Cor::AMARELO << "[INIMIGO] Anjo Caído Baal empunha uma lâmina que devora a própria luz!" << Cor::RESET << endl;
        if (testarAcerto(alvo.getDefesaEfetiva())) {
            int dano = gerarNumeroAleatorio(14, 22);
            alvo.receberDano(dano);
        }
    } 
    else if (chance <= 70) {
        int cura = gerarNumeroAleatorio(10, 18);
        curarPV(cura);
        cout << Cor::VERDE << "[INIMIGO] Anjo Caído Baal canaliza TOQUE CURATIVO (+ " << cura << " PVs)!" << Cor::RESET << endl;
    } 
    else if (chance <= 90) {
        digitarTexto(Cor::AMARELO + "Baal: "Contemple a imagem da qual foste privado..."" + Cor::RESET, 18);
        cout << Cor::AMARELO << "[A aura hipnótica do anjo perturba tua mente, enfraquecendo teus golpes!]" << Cor::RESET << endl;
        if (testarAcerto(alvo.getResistencia())) {
            alvo.aplicarModificadorAcerto(-3, 2);
            cout << Cor::AMARELO << "  -> " << alvo.getNome() << " foi ofuscado! (-3 de acerto nos próximos 2 turnos!)" << Cor::RESET << endl;
        }
    } 
    else {
        digitarTexto(Cor::AMARELO + "Baal: "Ficaste cego de tanto enxergar?"" + Cor::RESET, 18);
        cout << Cor::AMARELO << "✨ [Um feixe de luz ofuscante corta os céus, incendiando o solo aos teus pés!]" << Cor::RESET << endl;
        if (testarAcerto(alvo.getResistencia())) {
            int dano = gerarNumeroAleatorio(20, 30);
            alvo.receberDano(dano);
        }
    }
}

// ============================================================================
// IMPLEMENTAÇÃO: Gotwellenn (DANO AJUSTADO)
// Atributos: PV: 70 | Def: 15 | Res: 15 | Acerto: +9
// ============================================================================

Gotwellenn::Gotwellenn()
    : Monstro("Gotwellenn", 70, 15, 15, 9, "FASE 1 - MATRIZ CONGELADA") {}

Gotwellenn::~Gotwellenn() {}

void Gotwellenn::exibirEntradaCinematica() const {
    limparTela();
    desenharGotwellennASCII();
    cout << Cor::AZUL << Cor::NEGRITO << "==================================================" << endl;
    cout << "        ❄️ CONFRONTO NA MATRIZ CONGELADA ❄️        " << endl;
    cout << "==================================================" << Cor::RESET << endl;
    digitarTexto(Cor::AZUL + "A porta trancada se abre em um estrondo ressoante.", 18);
    digitarTexto(Cor::AZUL + "O chão é feito de linhas puras de código e telas de compilação.", 18);
    digitarTexto(Cor::AZUL + "No trono central, envolto em uma névoa glacial, aguarda Gotwellenn!", 18);
    cout << endl;
    digitarTexto(Cor::AZUL + Cor::NEGRITO + "Gotwellenn: "Chegaste longe, jovem aprendiz... mas a verdadeira prova final começa agora!"", 22);
    digitarTexto(Cor::VERDE + Cor::NEGRITO + "Jogador:    "Enfrentei monstros e deuses para chegar até aqui. Nada vai me parar!"", 22);
    pausarParaContinuar();
}

void Gotwellenn::executarTurnoIA(Personagem& alvo) {
    int chance = gerarNumeroAleatorio(1, 100);

    if (chance <= 70) {
        digitarTexto(Cor::AZUL + "Gotwellenn desfere um Gancho poderoso!" + Cor::RESET, 18);
        cout << Cor::AZUL << "[INIMIGO] Gotwellenn ataca com GANCHO!" << Cor::RESET << endl;
        if (testarAcerto(alvo.getDefesaEfetiva())) {
            int dano = gerarNumeroAleatorio(10, 16);
            alvo.receberDano(dano);
        }
    } 
    else {
        digitarTexto(Cor::AZUL + "Gotwellenn invoca uma tempestade de Nevasca!" + Cor::RESET, 18);
        cout << Cor::AZUL << "❄️ [INIMIGO] Uma névoa glacial congela o ar ao redor!" << Cor::RESET << endl;
        if (testarAcerto(alvo.getResistencia())) {
            int dano = gerarNumeroAleatorio(16, 24);
            alvo.receberDano(dano);
        }
    }
}

// ============================================================================
// IMPLEMENTAÇÃO: Wellington Verdadeiro (DESAFIO FINAL AUMENTADO!)
// Atributos: PV: 165 | Def: 17 | Res: 17 | Acerto: +12
// ============================================================================

WellingtonVerdadeiro::WellingtonVerdadeiro()
    : Monstro("Wellington Verdadeiro", 165, 17, 17, 12, "CHEFE FINAL - MATRIZ CONGELADA"), turnosAscensao(0) {}

WellingtonVerdadeiro::~WellingtonVerdadeiro() {}

void WellingtonVerdadeiro::exibirEntradaCinematica() const {
    limparTela();
    desenharWellingtonASCII();
    cout << Cor::VERMELHO << Cor::NEGRITO << "==================================================" << endl;
    cout << "   💥 A ILUSÃO SE DESFAZ! WELLINGTON VERDADEIRO! 💥 " << endl;
    cout << "==================================================" << Cor::RESET << endl;
    digitarTexto(Cor::VERMELHO + "A carcaça de Gotwellenn se desfaz em poeira digital.", 18);
    digitarTexto(Cor::VERMELHO + "A Cúpula treme enquanto o código do universo se reescreve!", 18);
    digitarTexto(Cor::VERMELHO + "Uma figura austera com o caderno de chamadas cintilando revela-se!", 18);
    cout << endl;
    digitarTexto(Cor::VERMELHO + Cor::NEGRITO + "Wellington Verdadeiro: "Impressionante.", 22);
    digitarTexto(Cor::VERMELHO + Cor::NEGRITO + "                       Mas será que seu código é limpo o suficiente para sobreviver ao meu compilador?!", 22);
    digitarTexto(Cor::VERMELHO + Cor::NEGRITO + "                       Sinta o peso da orientação a objetos pura!"", 22);
    pausarParaContinuar();
}

void WellingtonVerdadeiro::processarInicioTurno() {
    Monstro::processarInicioTurno();
    if (turnosAscensao > 0) {
        turnosAscensao--;
        if (turnosAscensao == 0) {
            cout << Cor::CINZA << "  -> O poder de Ascensão do Alpinista de Wellington dissipou-se." << Cor::RESET << endl;
        }
    }
}

int WellingtonVerdadeiro::getDefesaEfetiva() const {
    return defesa + (turnosAscensao > 0 ? 5 : 0);
}

int WellingtonVerdadeiro::getResistencia() const {
    return resistencia + (turnosAscensao > 0 ? 5 : 0);
}

int WellingtonVerdadeiro::getDanoBonusAscensao() const {
    return (turnosAscensao > 0 ? 5 : 0);
}

void WellingtonVerdadeiro::executarTurnoIA(Personagem& alvo) {
    int chance = gerarNumeroAleatorio(1, 100);

    if (chance <= 40) {
        digitarTexto(Cor::VERMELHO + "Wellington: "Sinta o impacto do Gancho Astral!"" + Cor::RESET, 18);
        cout << Cor::VERMELHO << "[CHEFE FINAL] Wellington Verdadeiro desfere GANCHO ASTRAL!" << Cor::RESET << endl;
        if (testarAcerto(alvo.getDefesaEfetiva())) {
            int dano = gerarNumeroAleatorio(16, 26) + getDanoBonusAscensao();
            alvo.receberDano(dano);
        }
    } 
    else if (chance <= 70) {
        digitarTexto(Cor::VERMELHO + "Wellington Verdadeiro: "Hora de checar as faltas! Aluno ausente não ganha nota!"" + Cor::RESET, 18);
        cout << Cor::VERMELHO << "(Uma lista arcana surge no ar, reduzindo a tua presença em combate!)" << Cor::RESET << endl;
        if (testarAcerto(alvo.getResistencia())) {
            int dano = gerarNumeroAleatorio(12, 18) + getDanoBonusAscensao();
            alvo.receberDano(dano);
            alvo.aplicarAtordoamento(1);
            cout << Cor::VERMELHO << "  -> " << alvo.getNome() << " levou falta pela Chamadinha e PERDEU O PRÓXIMO TURNO!" << Cor::RESET << endl;
        }
    } 
    else if (chance <= 90) {
        digitarTexto(Cor::VERMELHO + "Wellington Verdadeiro: "Seu programa falhou com o erro de Segment Fault!"" + Cor::RESET, 18);
        cout << Cor::VERMELHO << "🌋 (Uma cascata ininterrupta de gelo e dados colide contra sua estrutura!)" << Cor::RESET << endl;
        if (testarAcerto(alvo.getResistencia())) {
            int dano = gerarNumeroAleatorio(28, 40) + getDanoBonusAscensao();
            alvo.receberDano(dano);
        }
    } 
    else {
        turnosAscensao = 2;
        digitarTexto(Cor::AMARELO + "Wellington: "Ativando protocolo de Ascensão do Alpinista!"" + Cor::RESET, 18);
        cout << Cor::AMARELO << "🧗 [CHEFE FINAL] Wellington Verdadeiro ativa ASCENSÃO DO ALPINISTA!" << Cor::RESET << endl;
        cout << Cor::AMARELO << "  -> Wellington ganhou +5 em Defesa, Resistência e Dano pelos próximos 2 turnos!" << Cor::RESET << endl;
    }
}

// ============================================================================
// IMPLEMENTAÇÃO: Kobold
// ============================================================================

Kobold::Kobold()
    : Monstro("Kobold", 8, 8, 6, 7, "CAPANGA") {}

Kobold::~Kobold() {}

void Kobold::exibirEntradaCinematica() const {
    cout << Cor::CINZA << "  🗡️ Um Kobold salta dos arbustos!" << Cor::RESET << endl;
}

void Kobold::executarTurnoIA(Personagem& alvo) {
    int chance = gerarNumeroAleatorio(1, 100);

    if (chance <= 60) {
        cout << Cor::CINZA << "[CAPANGA] Kobold ataca com ADAGA!" << Cor::RESET << endl;
        if (testarAcerto(alvo.getDefesaEfetiva())) {
            int dano = gerarNumeroAleatorio(3, 6);
            alvo.receberDano(dano);
        }
    } 
    else {
        cout << Cor::CINZA << "[CAPANGA] Kobold dispara pedra com FUNDA!" << Cor::RESET << endl;
        if (testarAcerto(alvo.getDefesaEfetiva())) {
            int dano = gerarNumeroAleatorio(4, 7);
            alvo.receberDano(dano);
        }
    }
}
