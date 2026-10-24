#include "Buzzer.h"

#include "Alarme.h"
#include "Relogio.h"
#include "MaquinaEstados.h"
#include "joystick.h"

#include <LiquidCrystal.h>

// componentes importados por referência
extern LiquidCrystal lcd;
extern Estados estadoAtual;
extern Estados proximoEstado;
extern Alarme alarme;
extern Relogio relogio;
extern JoyStick joystick;

Buzzer::Buzzer(){}

void Buzzer::monitorarAlarme(void){
  // se o horário do relógio for igual ao horário do alarme
  if(relogio.horario == alarme.alarme){
    // limpa a tela atual e muda para a tela de alarme
    estadoAtual = LIMPA_TELA;
    proximoEstado = ALARME;
  }
}

void Buzzer::tocarAlarme(void){
  // durante 300ms
  if(millis() - tempoSom < 300){
    // aciona o buzzer na frequência 500hz
    tone(pinBuzzer, 500);
  // durante 300ms
  } else if(millis() - tempoSom < 600){
    // mantenha o buzzer desligado
    noTone(pinBuzzer);
  } else {
    // atualiza o tempo depois de intercalar entre ligar e desligar o buzzer
    tempoSom = millis();
  }

}

void Buzzer::exibirMensagem(void){
  // durate 400ms
  if(millis() - tempoPiscaTela < 400){
    // mantenha o display desligado
    lcd.clear();

    // e depois por 400ms
  } else if(millis() -  tempoPiscaTela < 800){
    // exiba o horário atual do relógio
    lcd.setCursor(4, 0);
    lcd.print(relogio.horario);

  // após alternar entre exibir e desligar a tela
  } else {
    // atualiza o tempo que controla o efeito de piscar
    tempoPiscaTela = millis();
  }
}

void Buzzer::analisaMovimentos(void){
  // se o botão do joystick foi acionado enquanto o alarme está ligado
  if(joystick.movimentos[4]){
    // limpa a tela, volta para a tela inicial e desliga o alarme (buzzer)
    estadoAtual = LIMPA_TELA;
    proximoEstado = TELA_INICIAL;
    noTone(pinBuzzer);
  }

  // reseta todos os movimentos detectados do joystick para evitar interferência
  joystick.resetaMovimentos();
}

