#include <Arduino.h>
#include "MaquinaEstados.h"

class Menu{
  // atributo
  public:
    int opcaoAtual = 0;

    // métodos
    Menu();
    
    void exibirMenu(String opcao1, String opcao2);
    void analisaMovimentos(Estados estadoAnterior, Estados estadoPosterior, Estados estadoCorrente);
};