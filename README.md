# ⚔️ Nexus da Compilação — RPG em C++

> **Nexus da Compilação** é um jogo de RPG em turnos via terminal desenvolvido inteiramente em **C++11**, focado na aplicação prática e rigorosa de todos os pilares da **Programação Orientada a Objetos (POO)**.

---

## 📜 Sumário
- [Sobre o Projeto](#-sobre-o-projeto)
- [🎮 Funcionalidades do Jogo](#-funcionalidades-do-jogo)
- [🏛️ Arquitetura e Engenharia de Software](#️-arquitetura-e-engenharia-de-software)
- [🎓 Conceitos de POO Aplicados](#-conceitos-de-poo-aplicados)
- [📁 Estrutura do Repositório](#-estrutura-do-repositório)
- [🚀 Como Compilar e Executar](#-como-compilar-e-executar)
- [👥 Créditos e Contexto Acadêmico](#-créditos-e-contexto-acadêmico)

---

## 📖 Sobre o Projeto

O projeto simula uma aventura interativa em terminal onde o jogador é transportado para o *Nexus da Compilação* — uma dimensão digital repleta de desafios e chefes icônicos. 

Para escapar, o jogador deve escolher uma classe de herói, gerenciar seus recursos (Vida, Mana/Energia e Inventário) e derrotar uma sequência de guardiões estratégicos em batalhas por turno.

---

## 🎮 Funcionalidades do Jogo

- **3 Classes de Heróis Jogáveis**:
  - 🧙‍♂️ **Mago**: Especialista em dano mágico em área (*Bola de Fogo*) e proteção arcana.
  - 🪓 **Bárbaro**: Tanque com alto valor de vida e acertos críticos devastadores.
  - 🏹 **Arqueiro**: Mestre da precisão e disparos múltiplos de alta agilidade.
- **Sistema de Combate em Turnos com Física D20**:
  - Testes de precisão baseados em rolagens de dados de 20 lados (`d20`) + bônus de classe contra a armadura/resistência do alvo.
  - Oscilação dinâmica de dano e cura (sem valores estáticos).
- **Inteligência Artificial Probabilística para Chefes**:
  - Tomada de decisão dos inimigos baseada em tabelas percentuais de 1 a 100% (Ataques normais, habilidades de controle, invocações e ataques supremos).
- **Mecanica de Invocação de Minions (STL & Heap)**:
  - Chefe *Necromante Lich* capaz de invocar dinamicamente esqueletos na arena durante a batalha.
- **Interface Rica em Terminal**:
  - Renderização de cores **ANSI**, animação de teletipo (`digitarTexto`), barras de vida dinâmicas (`█`) e ilustrações em **ASCII Art**.

---

## 🏛️ Arquitetura e Engenharia de Software

O projeto segue estritamente o princípio da **Separação de Responsabilidades (SoR)** e **Modularização em C++**, dividindo declarações (`.h`) de implementações (`.cpp`):

```text
Projeto RPG
 ├── main-v2.cpp        # Ponto de entrada (UTF-8, srand, loop principal)
 ├── Jogo.h / .cpp      # Game Engine: controle de arenas, menus e loop de combate
 ├── Personagens.h/.cpp # Hierarquia dos heróis (Personagem -> Jogador -> Subclasses)
 ├── Monstros.h / .cpp  # Hierarquia dos chefes (Monstro -> Chefes & Capangas)
 └── Visual.h / .cpp    # Camada gráfica: cores ANSI, artes ASCII e animações
```

---

## 🎓 Conceitos de POO Aplicados

| Conceito de POO | Aplicação Prática no Código |
| :--- | :--- |
| **Abstração** | Classe base `Personagem`, definindo atributos universais (PV, Defesa, Acerto). |
| **Encapsulamento** | Modificadores `protected` e `private` protegendo status contra acesso direto externo. |
| **Construtores & Lista de Inicialização** | Sintaxe de inicialização direta (`: Personagem(...)`) para otimização de memória. |
| **Destrutores (`~`)** | Limpeza automática de recursos e alocações (`Jogador::~Jogador()`, `NecromanteLich::~NecromanteLich()`). |
| **Composição ("TEM-UM")** | `vector<Item> bolsa` dentro do `Jogador` e `vector<Esqueleto*> capangas` no `Lich`. |
| **Associação ("USA-UM")** | Interação temporária em combate via passagem por referência (`Personagem& alvo`). |
| **Herança ("É-UM")** | Hierarquia de 3 níveis: `Mago : public Jogador` e `Jogador : public Personagem`. |
| **Polimorfismo Dinâmico** | Métodos virtuais puros (`= 0`), `override` (C++11) e consulta via `vtable` em tempo de execução. |
| **Gerenciamento de Heap & STL** | Uso de `std::vector` para ponteiros polimórficos com gerência estrita via `new` e `delete`. |
| **RNG (Aleatoriedade)** | Inicialização via `srand(static_cast<unsigned int>(time(NULL)))` e física D20. |

---

## 📁 Estrutura do Repositório

```text
.
├── main-v2.cpp       # Código de inicialização e entrada do programa
├── Jogo.h            # Declaração do motor de combate e hub de arenas
├── Jogo.cpp          # Implementação da lógica de gerenciamento de batalhas
├── Personagens.h     # Declarações da classe base e heróis jogáveis
├── Personagens.cpp   # Implementação dos construtores e habilidades dos heróis
├── Monstros.h        # Declarações dos chefes de fase e lacaios
├── Monstros.cpp      # Implementação da IA dos chefes e invocações
├── Visual.h          # Interface de funções utilitárias visuais e ANSI
├── Visual.cpp        # Implementação de animações, barras de HP e artes ASCII
└── README.md         # Documentação principal do projeto
```

---

## 🚀 Como Compilar e Executar

### 🛠️ Pré-requisitos
- Compilador C++ com suporte ao padrão **C++11** ou superior (`g++` / MinGW recomendados).
- Terminal com suporte a UTF-8 e sequências de escape ANSI (PowerShell no VS Code, Bash, Windows Terminal, Zsh, etc.).

---

### 💻 Compilação no Windows (PowerShell / VS Code)

Navegue até a pasta do projeto e execute o comando de compilação unificada:

```powershell
g++ -std=c++11 main-v2.cpp Jogo.cpp Personagens.cpp Monstros.cpp Visual.cpp -o main.exe ; if ($?) { .\main.exe }
```

> **Nota**: O operador `; if ($?) { .\main.exe }` garante que o jogo só será executado se a compilação de todos os arquivos for concluída com 100% de sucesso.

---

### 🐧 Compilação no Linux / macOS

```bash
g++ -std=c++11 main-v2.cpp Jogo.cpp Personagens.cpp Monstros.cpp Visual.cpp -o main && ./main
```

---

## 👥 Créditos e Contexto Acadêmico

- **Disciplina**: Programação Orientada a Objetos (POO)
- **Linguagem**: C++11
- **Paradigma**: Orientação a Objetos
- **Status do Projeto**: 🚀 Concluído & Compilando com Zero Warnings (`-Wall -Wextra`)

---
*Desenvolvido como projeto prático de consolidação em Engenharia de Software e POO.*
