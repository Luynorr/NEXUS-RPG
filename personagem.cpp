#include "personagem.h"


personagem::personagem(const std::string &name, int age) : nome(name), idade(age) {}
personagem::~personagem() {};

std::string personagem::getName() const {
    return nome;
}

int personagem::getAge() const {
    return idade;
}

void exibir(personagem &boneco) {
    std::cout << "Nome: " << boneco.getName() << std::endl;;
    std::cout << "Idade: " << boneco.getAge() << std::endl;
}



