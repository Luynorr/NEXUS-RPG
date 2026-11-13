#include "Jogo.h"

// ============================================================================
// IMPLEMENTAÇÃO DA CLASSE: Jogo
// ============================================================================

Jogo::Jogo() 
    : heroi(nullptr), cadeadosRestantes(3), 
      chefeKaidoDerrotado(false), chefeLichDerrotado(false), chefeBaalDerrotado(false),
      jogoEmAndamento(true) {}

Jogo::~Jogo() {
    if (heroi != nullptr) {
        delete heroi;
        heroi = nullptr;
    }
}

void Jogo::iniciar() {
    limparTela();
    exibirCabecalhoNexus();
    escolherClasse();

    while (jogoEmAndamento && heroi != nullptr && heroi->estaVivo()) {
        menuNexus();
    }
}

void Jogo::exibirCabecalhoNexus() const {
    desenharLogoNexus();
    cout << Cor::CIANO << Cor::NEGRITO << "==================================================" << endl;
    cout << "          ⚔️ BEM-VINDO AO NEXUS DO RPG ⚔️         " << endl;
    cout << "==================================================" << Cor::RESET << endl << endl;

    digitarTexto(Cor::BRANCO + "O prazo final do trabalho de Programação Orientada a Objetos se aproxima." + Cor::RESET, 20);
    digitarTexto(Cor::BRANCO + "Assolado por dúvidas sobre a estrutura de seu código em C++, você decide" + Cor::RESET, 20);
    digitarTexto(Cor::BRANCO + "buscar ajuda na sala do lendário Professor Wellington." + Cor::RESET, 20);
    pausarMs(300);
    digitarTexto(Cor::BRANCO + "Ao bater à porta, não encontra resposta. A sala está silenciosa, vazia," + Cor::RESET, 20);
    digitarTexto(Cor::BRANCO + "a não ser por um notebook aberto sobre a mesa." + Cor::RESET, 20);
    pausarMs(300);
    digitarTexto(Cor::AMARELO + "Curioso, você se aproxima da tela e vê um menu inicial de um jogo de RPG misterioso." + Cor::RESET, 20);
    digitarTexto(Cor::AMARELO + "Cedendo à tentação, você pressiona o botão START!" + Cor::RESET, 20);
    pausarMs(400);

    digitarTexto(Cor::VERMELHO + Cor::NEGRITO + "Instantaneamente, o ar ao redor se torna pesado, a luz se distorce" + Cor::RESET, 22);
    digitarTexto(Cor::VERMELHO + Cor::NEGRITO + "e a realidade se desfaz em linhas de código. Você é puxado para dentro da máquina!" + Cor::RESET, 22);
    pausarMs(400);

    cout << endl;
    digitarTexto(Cor::CIANO + "Ao abrir os olhos, você se encontra em uma cidadela em forma de abóbada" + Cor::RESET, 20);
    digitarTexto(Cor::CIANO + "flutuando no espaço: bem-vindo ao Nexus da Compilação!" + Cor::RESET, 20);
    pausarMs(300);
    digitarTexto(Cor::AMARELO + "Três reinos mantêm o selo da Matriz Congelada fechado por 3 cadeados." + Cor::RESET, 20);
    digitarTexto(Cor::AMARELO + "Derrote os 3 chefes para romper os selos e encarar a batalha final!" + Cor::RESET, 20);

    cout << Cor::CIANO << "\n==================================================\n" << Cor::RESET << endl;
    pausarParaContinuar();
}

void Jogo::escolherClasse() {
    limparTela();
    cout << Cor::AMARELO << Cor::NEGRITO << "Digite o nome do seu Herói: " << Cor::RESET;
    string nomeJogador;
    cin >> nomeJogador;

    limparTela();
    cout << Cor::CIANO << Cor::NEGRITO << "==================================================" << endl;
    cout << "       --- ESCOLHA A SUA CLASSE DE HERÓI ---       " << endl;
    cout << "==================================================" << Cor::RESET << endl;
    cout << Cor::AZUL << "1. MAGO DA COMPILAÇÃO    (PV: 85  | Mana: 25    | Def: 10 | Res: 12 | Acerto: +7)" << Cor::RESET << endl;
    cout << Cor::MAGENTA << "2. BÁRBARO DO NEXUS      (PV: 120 | Energia: 16 | Def: 17 | Res: 15 | Acerto: +7)" << Cor::RESET << endl;
    cout << Cor::CIANO << "3. ARQUEIRO DA MATRIZ    (PV: 95  | Energia: 18 | Def: 14 | Res: 18 | Acerto: +8)" << Cor::RESET << endl;
    cout << Cor::CIANO << "==================================================" << Cor::RESET << endl;
    cout << "Opção: ";

    int opcao;
    cin >> opcao;

    limparTela();
    switch (opcao) {
        case 1:
            heroi = new Mago(nomeJogador);
            desenharMagoASCII();
            digitarTexto(Cor::AZUL + Cor::NEGRITO + "\n✨ [CLASSE SELADA] Você escolheu o MAGO DA COMPILAÇÃO!" + Cor::RESET, 20);
            break;
        case 2:
            heroi = new Barbaro(nomeJogador);
            desenharBarbaroASCII();
            digitarTexto(Cor::MAGENTA + Cor::NEGRITO + "\n🪓 [CLASSE SELADA] Você escolheu o BÁRBARO DO NEXUS!" + Cor::RESET, 20);
            break;
        case 3:
            heroi = new Arqueiro(nomeJogador);
            desenharArqueiroASCII();
            digitarTexto(Cor::CIANO + Cor::NEGRITO + "\n🏹 [CLASSE SELADA] Você escolheu o ARQUEIRO DA MATRIZ!" + Cor::RESET, 20);
            break;
        default:
            cout << Cor::AMARELO << "\n[OPÇÃO INVÁLIDA] Selecionando Mago por padrão." << Cor::RESET << endl;
            heroi = new Mago(nomeJogador);
            desenharMagoASCII();
            break;
    }

    heroi->exibirExplicacaoItensIniciais();
    pausarParaContinuar();
}

void Jogo::menuNexus() {
    limparTela();
    cout << Cor::CIANO << Cor::NEGRITO << "==================================================" << endl;
    cout << "             HUB DO NEXUS DA COMPILAÇÃO          " << endl;
    cout << "==================================================" << Cor::RESET << endl;
    cout << "1. Fornalha Cinzenta  " << (chefeKaidoDerrotado ? Cor::VERDE + "[CONCLUÍDA]" : Cor::VERMELHO + "[CHEFE: Dragão Kaido]") << Cor::RESET << endl;
    cout << "2. Cidade Sepulcral   " << (chefeLichDerrotado ? Cor::VERDE + "[CONCLUÍDA]" : Cor::MAGENTA + "[CHEFE: Necromante Lich]") << Cor::RESET << endl;
    cout << "3. Catedral Herética  " << (chefeBaalDerrotado ? Cor::VERDE + "[CONCLUÍDA]" : Cor::AMARELO + "[CHEFE: Anjo Caído Baal]") << Cor::RESET << endl;
    
    if (cadeadosRestantes > 0) {
        cout << Cor::CINZA << "4. Matriz Congelada   [TRANCADA - " << cadeadosRestantes << " CADEADO(S) RESTANTE(S)]" << Cor::RESET << endl;
    } else {
        cout << Cor::VERDE << Cor::NEGRITO << "4. Matriz Congelada   [DESBLOQUEADA - CONFRONTO FINAL!]" << Cor::RESET << endl;
    }
    cout << Cor::BRANCO << "5. Sair do Jogo" << Cor::RESET << endl;
    cout << Cor::CIANO << "==================================================" << Cor::RESET << endl;
    cout << "Escolha o seu destino: ";

    int opcao;
    cin >> opcao;

    switch (opcao) {
        case 1: {
            if (chefeKaidoDerrotado) {
                cout << Cor::VERDE << "\n[SALA LIMPA] O Dragão Kaido já foi derrotado! Escolha outra sala." << Cor::RESET << endl;
                pausarParaContinuar();
            } else {
                DragaoKaido kaido;
                iniciarBatalha(&kaido);
                if (heroi->estaVivo() && !kaido.estaVivo()) {
                    processarVitoriaBoss("Dragão Kaido", chefeKaidoDerrotado);
                }
            }
            break;
        }
        case 2: {
            if (chefeLichDerrotado) {
                cout << Cor::VERDE << "\n[SALA LIMPA] O Necromante Lich já foi derrotado! Escolha outra sala." << Cor::RESET << endl;
                pausarParaContinuar();
            } else {
                NecromanteLich lich;
                iniciarBatalha(&lich);
                if (heroi->estaVivo() && !lich.estaVivo()) {
                    processarVitoriaBoss("Necromante Lich", chefeLichDerrotado);
                }
            }
            break;
        }
        case 3: {
            if (chefeBaalDerrotado) {
                cout << Cor::VERDE << "\n[SALA LIMPA] O Anjo Caído Baal já foi derrotado! Escolha outra sala." << Cor::RESET << endl;
                pausarParaContinuar();
            } else {
                AnjoCaidoBaal baal;
                iniciarBatalha(&baal);
                if (heroi->estaVivo() && !baal.estaVivo()) {
                    processarVitoriaBoss("Anjo Caído Baal", chefeBaalDerrotado);
                }
            }
            break;
        }
        case 4: {
            if (cadeadosRestantes > 0) {
                cout << Cor::VERMELHO << "\n[BLOQUEADO] A Matriz Congelada está selada por " << cadeadosRestantes 
                     << " cadeado(s)! Derrote todos os 3 chefes das salas anteriores." << Cor::RESET << endl;
                pausarParaContinuar();
            } else {
                executarBatalhaFinal();
            }
            break;
        }
        case 5:
            digitarTexto(Cor::AMARELO + "\n[ENCERRANDO] Saindo do Nexus da Compilação..." + Cor::RESET, 20);
            jogoEmAndamento = false;
            break;
        default:
            cout << Cor::VERMELHO << "\n[AVISO] Opção de sala inválida!" << Cor::RESET << endl;
            pausarParaContinuar();
            break;
    }
}

void Jogo::processarVitoriaBoss(const string& nomeBoss, bool& flagBoss) {
    flagBoss = true;
    cadeadosRestantes--;
    digitarTexto(Cor::VERDE + Cor::NEGRITO + "\n>>> 🏆 VITÓRIA CONQUISTADA! " + nomeBoss + " FOI DERROTADO! <<<" + Cor::RESET, 20);
    digitarTexto(Cor::AMARELO + "Um dos cadeados da Matriz Congelada se destruiu! (Restam: " + to_string(cadeadosRestantes) + ")" + Cor::RESET, 20);
    
    heroi->evoluirNoNexus();
    pausarParaContinuar();
}

void Jogo::iniciarBatalha(Monstro* chefe, bool ehBossFinal) {
    (void)ehBossFinal;
    
    // Entrada cinematográfica do chefe
    chefe->exibirEntradaCinematica();

    int rodada = 1;

    while (heroi->estaVivo() && chefe->estaVivo()) {
        cout << "\n" << Cor::CIANO << Cor::NEGRITO << "==================================================" << endl;
        cout << "                 --- RODADA " << rodada << " ---" << endl;
        cout << "==================================================" << Cor::RESET << endl;

        // Processa início do turno (Decrementa buffs/debuffs)
        heroi->processarInicioTurno();
        chefe->processarInicioTurno();

        // Regeneração passiva de recursos do jogador (+3 por rodada rebalanceado)
        heroi->restaurarRecurso(3);

        // Turno do Jogador
        bool turnoConcluido = false;
        while (!turnoConcluido && heroi->estaVivo()) {
            turnoConcluido = executarTurnoJogador(chefe);
        }

        if (!chefe->estaVivo()) {
            digitarTexto(Cor::VERDE + Cor::NEGRITO + "\n[COMBATE] " + chefe->getNome() + " sucumbiu aos seus ataques!" + Cor::RESET, 20);
            pausarMs(300);
            break;
        }

        pausarMs(200);

        // Turno do Inimigo (IA Automatizada)
        if (chefe->estaAtordoado()) {
            digitarTexto(Cor::AMARELO + "\n[STATUS] " + chefe->getNome() + " está ATORDOADO/PARALISADO e perdeu o turno!" + Cor::RESET, 20);
            chefe->decrementarAtordoamento();
        } else {
            cout << "\n" << Cor::VERMELHO << "--------------------------------------------------" << endl;
            cout << "[TURNO DO INIMIGO] " << chefe->getNome() << " prepara sua ação..." << endl;
            cout << "--------------------------------------------------" << Cor::RESET << endl;
            pausarMs(200);
            chefe->executarTurnoIA(*heroi);
        }

        if (!heroi->estaVivo()) {
            exibirGameOver();
            jogoEmAndamento = false;
            break;
        }

        pausarMs(300);
        rodada++;
    }
}

bool Jogo::executarTurnoJogador(Personagem* inimigo) {
    cout << "\n" << Cor::NEGRITO << "--- STATUS ATUAL DA ARENA ---" << Cor::RESET << endl;
    heroi->exibirStatus();
    inimigo->exibirStatus();

    // Obtém ponteiro de capangas da arena
    Monstro* monstroInimigo = dynamic_cast<Monstro*>(inimigo);
    vector<Esqueleto*>* capangasPonteiro = (monstroInimigo != nullptr) ? monstroInimigo->getCapangasPonteiro() : nullptr;

    if (capangasPonteiro != nullptr && !capangasPonteiro->empty()) {
        int vivos = 0;
        for (auto c : *capangasPonteiro) {
            if (c != nullptr && c->estaVivo()) vivos++;
        }
        if (vivos > 0) {
            cout << Cor::MAGENTA << " 💀 [MINIONS NA ARENA]: " << vivos << " Esqueleto(s) ativo(s)!" << Cor::RESET << endl;
        }
    }

    if (heroi->estaAtordoado()) {
        digitarTexto(Cor::VERMELHO + "\n[STATUS] Você está ATORDOADO/PARALISADO e não pode agir neste turno!" + Cor::RESET, 20);
        heroi->decrementarAtordoamento();
        return true;
    }

    cout << "\n" << Cor::AMARELO << Cor::NEGRITO << "--- SUA VEZ DE AGIR ---" << Cor::RESET << endl;
    cout << "1. Usar Habilidade / Ataque" << endl;
    cout << "2. Usar Item da Bolsa" << endl;
    cout << "Escolha sua ação: ";

    int tipoAcao;
    cin >> tipoAcao;

    if (tipoAcao == 1) {
        heroi->exibirMenuHabilidades();
        
        int escHabilidade;
        cin >> escHabilidade;
        heroi->executarAcao(*inimigo, escHabilidade, capangasPonteiro);
        return true;
    } 
    else if (tipoAcao == 2) {
        heroi->exibirBolsa();
        if (heroi->getQtdItens() == 0) {
            return false;
        }
        cout << "Digite o número do item que deseja usar (ou 0 para voltar): ";
        int itemEsc;
        cin >> itemEsc;
        if (itemEsc == 0) return false;
        return heroi->usarItem(itemEsc - 1, *inimigo, capangasPonteiro);
    }

    cout << "\n" << Cor::VERMELHO << "[AVISO] Opção de ação inválida!" << Cor::RESET << endl;
    return false;
}

void Jogo::executarBatalhaFinal() {
    limparTela();
    cout << Cor::VERMELHO << Cor::NEGRITO << "==================================================" << endl;
    cout << "     OS 3 CADEADOS SE ROMPERAM! MATRIZ CONGELADA  " << endl;
    cout << "==================================================" << Cor::RESET << endl;
    digitarTexto(Cor::AZUL + "A porta trancada se abre em um estrondo ressoante." + Cor::RESET, 20);
    digitarTexto(Cor::AZUL + "O chão é feito de linhas puras de código e telas de compilação..." + Cor::RESET, 20);

    // FASE 1: Gotwellenn (55 PV)
    Gotwellenn gotwellenn;
    iniciarBatalha(&gotwellenn, true);

    if (!heroi->estaVivo()) return;

    // FASE 2: Wellington Verdadeiro (115 PV)
    WellingtonVerdadeiro wellington;
    iniciarBatalha(&wellington, true);

    if (heroi->estaVivo() && !wellington.estaVivo()) {
        exibirVitoriaJogo();
        jogoEmAndamento = false;
    }
}

void Jogo::exibirVitoriaJogo() const {
    desenharVitoriaASCII();
    digitarTexto(Cor::VERDE + Cor::NEGRITO + "Wellington Verdadeiro foi derrotado!" + Cor::RESET, 20);
    digitarTexto(Cor::VERDE + "A Matriz Congelada se dissolve e seu código compila sem erros ou Warnings!" + Cor::RESET, 20);
    digitarTexto(Cor::VERDE + "Você escapou do terminal e conquistou a nota máxima no TP de POO!" + Cor::RESET, 20);
    pausarParaContinuar();
}

void Jogo::exibirGameOver() const {
    desenharGameOverASCII();
    digitarTexto(Cor::VERMELHO + Cor::NEGRITO + "Seu herói sucumbiu em combate." + Cor::RESET, 20);
    digitarTexto(Cor::VERMELHO + "Seu programa encerrou com erro de compilação..." + Cor::RESET, 20);
    pausarParaContinuar();
}
