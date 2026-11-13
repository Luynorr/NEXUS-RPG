#ifndef PERSONAGENS_H
#define PERSONAGENS_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
using namespace std;

// ============================================================================
// 1. ESTRUTURA DE DADOS: Item
// ============================================================================
// Estrutura leve para representar os consumíveis que o jogador carrega na bolsa.
// Não necessita de uma classe complexa separada, atendendo à mecânica intrínseca.
struct Item {
    string nome;        // Nome de exibição do item (ex: "Poção de Cura")
    string descricao;   // Explicação breve do efeito para o menu da bolsa
    int tipo;           // 1 = Recupera PV (Vida), 2 = Restaura Mana/Energia, 3 = Dano Direto no Inimigo
    int valorEfeito;    // Quantidade numérica do impacto (ex: 14 para cura, 28 para dano)
};

// Declaração antecipada (Forward Declaration) para permitir referências cruzadas
class Personagem;

// ============================================================================
// 2. CLASSE BASE ABSTRATA: Personagem
// ============================================================================
// Esta classe é a espinha dorsal do sistema de POO (Polimorfismo e Herança).
// Contém atributos e comportamentos comuns a TODAS as entidades (Jogadores e Inimigos).
// A presença de métodos virtuais puros (= 0) impede a instanciação direta desta classe.
class Personagem {
protected:
    // Atributos protegidos (protected): Acessíveis apenas por esta classe e suas subclasses derivadas.
    string nome;             // Nome da entidade
    int pvAtual;             // Pontos de Vida atuais no combate
    int pvMaximo;            // Teto máximo de Pontos de Vida
    int defesa;              // Defesa Física (Reduz dano físico recebido: Dano = Bruto - Defesa/2)
    int resistencia;         // Resistência Mágica / Dificuldade para testes de acerto mágico
    int bonusAcerto;         // Bônus base de acerto somado ao d20 (Calibrado por classe)

    // Controle de Estados de Batalha (Efeitos Temporários / Buffs & Debuffs)
    int turnosAtordoado;               // Se > 0, a entidade perde a vez no turno atual
    int turnosArmaduraArcana;          // Se > 0, concede +5 de Defesa bônus
    bool estaEscondido;                // Se true, concede imunidade total a ataques por 1 turno
    double multiplicadorDanoRecebido;  // Multiplicador de dano sofrido (ex: 1.5 = +50% de dano após Paralisia)
    int modBonusAcertoTemp;            // Modificador temporário de acerto (ex: -3 por Presença de Luz, +2 por Bater Escudo)
    int turnosModAcerto;               // Duração do modificador temporário de acerto

    // Métodos utilitários internos (Auxiliares de Mecânica)
    int gerarNumeroAleatorio(int min, int max) const;            // Sorteia um número no intervalo [min, max]

public:
    // Construtor parametrizado: Inicializa os atributos fundamentais da base
    Personagem(string nome, int pvMax, int def, int res, int bonusAcerto);

    // Destrutor Virtual Obrigatório: Garante desalocação segura da memória Heap ao deletar por ponteiro da base
    virtual ~Personagem();

    // ------------------------------------------------------------------------
    // MÉTODOS VIRTUAIS PUROS (= 0) - CONTRATO POLIMÓRFICO
    // Toda subclasse concreta É OBRIGADA a implementar estes métodos.
    // ------------------------------------------------------------------------
    virtual void executarAcao(Personagem& alvo, int escolhaAcao) = 0; // Executa um ataque ou habilidade
    virtual void exibirStatus() const = 0;                           // Exibe a ficha atualizada da entidade

    // ------------------------------------------------------------------------
    // MÉTODOS CONCRETOS COMPARTILHADOS (Herança direta)
    // ------------------------------------------------------------------------
    virtual void processarInicioTurno();    // Atualiza a duração dos efeitos de status no início da jogada
    virtual void receberDano(int danoBruto); // Aplica cálculo de dano com mitigação de defesa e vulnerabilidade
    virtual void curarPV(int quantidade);    // Adiciona vida sem ultrapassar o pvMaximo

    // Sistema de Teste de Acerto d20 Balanceado
    bool testarAcerto(int defesaOuResistenciaAlvo) const; // Executa: d20 (1 a 20) + bonusAcertoEfetivo >= Alvo

    // Aplicação e Manipulação de Efeitos de Status
    void aplicarAtordoamento(int turnos = 1);
    void aplicarArmaduraArcana(int turnos = 3);
    void aplicarEscondido(bool estado = true);
    void aplicarMultiplicadorDano(double mult);
    void aplicarModificadorAcerto(int mod, int turnos); // Aplica buff/debuff temporário de acerto

    // Métodos Getters (Consultas de Estado de Leitura)
    bool estaAtordoado() const;
    bool getEstaEscondido() const;
    bool estaVivo() const;
    string getNome() const;
    int getPvAtual() const;
    int getPvMaximo() const;
    int getDefesaEfetiva() const;       // Retorna a defesa base + bônus de Armadura Arcana (se ativa)
    int getResistencia() const;
    int getBonusAcertoBase() const;     // Retorna o bônus de acerto nativo da classe
    int getBonusAcertoEfetivo() const;  // Retorna bonusAcerto + modBonusAcertoTemp
};

// ============================================================================
// 3. CLASSE INTERMEDIÁRIA: Jogador (Especialização com Inventário e Recursos)
// ============================================================================
// Herda de Personagem e adiciona o gerenciamento de recursos (Mana/Energia)
// e o contêiner STL std::vector<Item> para a Bolsa de Suprimentos.
class Jogador : public Personagem {
protected:
    int recursoAtual;    // Pontos de Mana (Mago) ou Energia (Bárbaro/Arqueiro) atuais
    int recursoMaximo;   // Teto máximo de recurso da classe
    vector<Item> bolsa;  // Contêiner dinâmico (STL Vector) que guarda os itens consumíveis

public:
    // Construtor da subclasse intermediária
    Jogador(string nome, int pvMax, int def, int res, int bonusAcerto, int recursoMax);
    virtual ~Jogador();

    // Mecânica de Evolução no Nexus (Concedida ao derrotar cada chefe)
    void evoluirNoNexus(); // Soma +15 ao PV Máximo e +3 ao limite de recurso da classe

    // Gestão e Utilização da Bolsa de Itens (Uso de std::vector)
    void inicializarBolsaInicial();       // Preenche a bolsa com os 5 itens padrões da Lore
    void exibirExplicacaoItensIniciais() const; // Exibe o briefing explicativo dos itens
    void exibirBolsa() const;            // Lista os itens disponíveis com seus índices
    bool usarItem(int indice, Personagem& alvo); // Executa o item e remove do vetor com erase()
    void adicionarItem(string nome, string desc, int tipo, int valor); // Insere um novo item no vetor

    // Gestão do Recurso de Classe (Mana/Energia)
    void restaurarRecurso(int quantidade); // Recupera recurso sem ultrapassar recursoMaximo
    int getRecursoAtual() const;
    int getRecursoMaximo() const;
    int getQtdItens() const;
};

// ============================================================================
// 4. SUBCLASSES CONCRETAS DE HERÓIS REBALANCEADOS COM BÔNUS D20
// ============================================================================
// Cada classe representa um arquétipo jogável com bônus de acerto balanceado (+7 ou +8).

// ----------------------------------------------------------------------------
// CLASSE CONCRETA: Mago (Especialista em magia de área, cura e controle)
// Atributos Base: PV: 75 | Mana: 22 | Defesa: 8 | Resistência: 10 | Bônus Acerto: +7
// ----------------------------------------------------------------------------
class Mago : public Jogador {
public:
    Mago(string nome);
    virtual ~Mago();

    // Sobrescrita dos métodos abstratos com o qualificador 'override'
    void executarAcao(Personagem& alvo, int escolhaAcao) override;
    void exibirStatus() const override;
};

// ----------------------------------------------------------------------------
// CLASSE CONCRETA: Bárbaro (Especialista em dano físico direto e alta vitalidade)
// Atributos Base: PV: 112 | Energia: 14 | Defesa: 16 | Resistência: 15 | Bônus Acerto: +7
// ----------------------------------------------------------------------------
class Barbaro : public Jogador {
public:
    Barbaro(string nome);
    virtual ~Barbaro();

    void executarAcao(Personagem& alvo, int escolhaAcao) override;
    void exibirStatus() const override;
};

// ----------------------------------------------------------------------------
// CLASSE CONCRETA: Arqueiro (Especialista em precisão, furtividade e rajadas)
// Atributos Base: PV: 88 | Energia: 16 | Defesa: 13 | Resistência: 17 | Bônus Acerto: +8
// ----------------------------------------------------------------------------
class Arqueiro : public Jogador {
public:
    Arqueiro(string nome);
    virtual ~Arqueiro();

    void executarAcao(Personagem& alvo, int escolhaAcao) override;
    void exibirStatus() const override;
};

#endif // PERSONAGENS_H
