#include "Alarme.h"

#include <LiquidCrystal.h>

extern LiquidCrystal lcd;

Alarme::Alarme(){}

void Alarme::exibirAlarme(int linha){
  // exibe alarme com o tipo de objeto no início da linha e o valor do alarme atual
  lcd.setCursor(0, linha);
  lcd.print("A:");

  lcd.setCursor(5, linha);

  lcd.print(alarme);
}