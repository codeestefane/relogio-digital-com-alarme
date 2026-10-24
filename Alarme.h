#include <Arduino.h>

class Alarme{
  // atributos
  private:
    int bufferDigito;
    
  public:
    String alarme = "00:00:00";

    // métodos
    Alarme();
    
    void exibirAlarme(int linha);
};