// bibliotecas importadas
#include "Relogio.h"
#include "Buzzer.h"
#include <LiquidCrystal.h>

// componentes externos importados por referência
extern LiquidCrystal lcd;
extern Buzzer buzzer;

Relogio::Relogio(){}

void Relogio::contarTempo(void){
  // incrementa segundo
  segundo++;

  if(segundo == 60){
    // atualiza o minuto
    minuto++;
    segundo = 0;

    // a cada 60 minutos
    if(minuto == 60){
      // atualiza a hora
      hora++;
      minuto = 0;

      if(hora == 24){
          hora = 0;
      }
    }

  }
  
  // atualiza o horário e verifica se o relógio e o alarme coincidem
  atualizarHorario();
  buzzer.monitorarAlarme();
}

void Relogio::atualizarHorario(void){
  // limpa horário atual
  horario = "";

  // para todos os dígitos menores do que 10, adiciona 0 à esquerda e concatena com a nova string do horário, separando os componentes do relógio com ":"
  if(hora < 10){
    horario.concat('0');
  }

  horario.concat(String(hora) + ":");

  if(minuto < 10){
    horario.concat('0');
  }

  horario.concat(String(minuto) + ":");

  if(segundo < 10){
    horario.concat('0');
  }

  horario.concat(String(segundo));

}

void Relogio::exibirRelogio(){
  // exibe relógio com o tipo do objeto no canto da primeira linha e o valor do horário atual no meio da tela
  lcd.setCursor(0, 0);
  lcd.print("C:");
  lcd.setCursor(5, 0);
  lcd.print(horario);
}

void Relogio::converterHorarioString(String time){
  // ajusta as variáveis do relógio que são atualizadas quando o relógio é inicializado e/ou configurado com um novo horário
  horario = time;
  buffer = horario.substring(0, 2);
  hora = buffer.toInt();

  buffer = horario.substring(3, 5);
  minuto = buffer.toInt();

  buffer = horario.substring(6, 8);
  segundo = buffer.toInt();
}
