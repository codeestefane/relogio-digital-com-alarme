#include <Arduino.h>
// #include "Relogio.h"

class JoyStick{
  // atributos
  private:
    int pinX = A0;
    int pinY = A1;
    int pinZ = 7;

    int debouncing = 0;

  public:
    int valorXAtual;
    int valorYAtual;
    int valorZAtual = 0;

    int valorXAnterior = 433;
    int valorYAnterior = 433;
    int valorZAnterior = 0;

    int movimentos[5] = {0, 0, 0, 0, 0}; // cima, baixo, esquerda, direita, botao

    // métodos
    JoyStick();
    
    void lerBotaoJoyStick(void);
    void lerJoyStickEixoY(void);
    void lerJoyStickEixoX(void);
    void resetaMovimentos(void);
};