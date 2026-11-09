// esse é um arquivo cabeçalho onde só fica
// rosto da classe e funções

// Aqui eu preparo uma "trava" para impedir que o cabeçalho seja incluído mais de uma vez no projeto.
#include <string>
#include <iostream>

#ifndef PERSONAGEM_H
#define PERSONAGEM_H

class personagem{
    private:
        std::string nome;
        int idade;
    public:
    personagem(const std::string &name, int age);

    std::string getName() const;
    int getAge() const;

    ~personagem();
};

void exibir(personagem &boneco);

#endif

