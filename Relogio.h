#include <Arduino.h>

class Relogio{
  // atributos
  private:
    int hora;
    int minuto;
    int segundo;
    String buffer;

  public:
    String horario;
    int contador = 0;

    Relogio();
    
    // métodos
    void atualizarHorario(void);
    void contarTempo(void);
    void exibirRelogio(void);
    void converterHorarioString(String time);
};