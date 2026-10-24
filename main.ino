/*
  * Autores:
  * Estefane de Lima dos Santos
  * Kauane dos Santos Vieira
  * 
*/

// bibliotecas internas
#include "Relogio.h"
#include "Alarme.h"
#include "JoyStick.h"
#include "Menu.h"
#include "MaquinaEstados.h"
#include "Buzzer.h"
#include "Configuracao.h"

// bibliotecas externas
#include <LiquidCrystal.h>
#include <EEPROM.h>

// variáveis de controle de estado da máquina de estados
Estados estadoAtual = TELA_INICIAL;
Estados proximoEstado = TELA_INICIAL;

// configuração do display LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// instanciação dos componentes utilizados
Relogio relogio;
Alarme alarme;
JoyStick joystick;
Menu menu;
Buzzer buzzer;
Configuracao configuracao;

void setup() {
  // versionamento do último horário cadastrado na memória do programa
  int numeroDoLog = EEPROM.read(10); 
  numeroDoLog++; 

  EEPROM.write(10, numeroDoLog);

  // inicialização do monitor serial
  Serial.begin(9600);

  // inicialização do horário do relógio
  relogio.converterHorarioString(__TIME__);

  // configuração do timer 1 para ser acionado a cada 1ms
  TCCR1A = 0;                         
  TCCR1B = 0;   
  TCCR1B |= (1<<CS10);   
  TCCR1B |= (1<<WGM12);     
  OCR1A = 0x3E80;
  TIMSK1 |= (1 << OCIE1A);  

  // configuração do timer 3 para ser acionado a cada 1ms
  TCCR3A = 0;                         
  TCCR3B = 0;   
  TCCR3B |= (1<<CS30);   
  TCCR3B |= (1<<WGM32);     
  OCR3A = 0x3E80;
  TIMSK3 |= (1 << OCIE3A);  

  // configuração dos pinos utilizados no joyStick
  pinMode(7, INPUT_PULLUP);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  // inicialização do display LCD
  lcd.begin(16, 2);
}

void loop() {

  // máquina de estados
  switch(estadoAtual){

    // exibe relógio e alarme
    case TELA_INICIAL:
      if(joystick.movimentos[4]){
          // troca de tela (estado) se o botão for pressionado
          proximoEstado = MENU;
          estadoAtual = LIMPA_TELA;
      }

      joystick.resetaMovimentos();

      relogio.exibirRelogio();
      alarme.exibirAlarme(1);

      break;

    // menu de configuração
    case MENU:
      menu.analisaMovimentos(TELA_INICIAL, CONFIGURACAO, MENU);
      menu.exibirMenu("Edit alarm", "Edit clock");
      break;

    // estado de configuração do alarme ou relógio
    case CONFIGURACAO:
      configuracao.analisaMovimentos();
      configuracao.exibirConfiguracao(0);
      break;

    // menu para confirmar/descartar a configuração realizada
    case CONFIRMAR_CONFIGURACAO:
      menu.analisaMovimentos(CONFIGURACAO, TELA_INICIAL, CONFIRMAR_CONFIGURACAO);
      menu.exibirMenu("Save", "Discard");
      break;
    
    // apaga todos os caracteres da tela atual antes de passar a próxima 
    case LIMPA_TELA:
      lcd.clear();
      estadoAtual = proximoEstado;

      break;

    // aciona alarme
    case ALARME:
      buzzer.tocarAlarme();
      buzzer.exibirMensagem();
      buzzer.analisaMovimentos();
      break;
  }

}

// procedimento de interrupção do timer 1
ISR(TIMER1_COMPA_vect) 
{
  // atualiza o valor do relógio a cada 1s
  relogio.contarTempo();
}

// procedimento de interrupção do timer 3 - identifica movimentos do joystick
ISR(TIMER3_COMPA_vect) 
{
  joystick.lerBotaoJoyStick();
  joystick.lerJoyStickEixoY();
  joystick.lerJoyStickEixoX();
}

