#include <Arduino.h>

class Buzzer{
  // atributos
  private:
    int pinBuzzer = 6;
    unsigned long tempoSom = 0;
    unsigned long tempoPiscaTela = 0;
    int contador = 0;

  public:

    // métodos
    Buzzer();
    
    void monitorarAlarme(void);
    void tocarAlarme(void);
    void exibirMensagem(void);
    void analisaMovimentos(void);
};