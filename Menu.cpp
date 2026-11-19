// bibliotecas importadas
#include "Menu.h"
#include "JoyStick.h"
#include "Alarme.h"
#include "Relogio.h"
#include "Configuracao.h"

#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#include <LiquidCrystal.h>
#include "MaquinaEstados.h"

// componentes importados por referência
extern LiquidCrystal lcd;
extern Estados estadoAtual;
extern Estados proximoEstado;
extern JoyStick joystick;
extern Alarme alarme;
extern Relogio relogio;
extern Configuracao configuracao;

extern SemaphoreHandle_t xMutexMovimentos;

Menu::Menu(){}

void Menu::exibirMenu(String opcao1, String opcao2){
  // exibe a primeira opção do menu na primeira linha
  lcd.setCursor(4, 0);
  lcd.print(opcao1);

  // exibe a segunda opção do menu na segunda linha
  lcd.setCursor(4, 1);
  lcd.print(opcao2);

  // exibe seletor do menu (*) de acordo com a opção atual selecionada pelo usuário
  lcd.setCursor(2, opcaoAtual);
  lcd.print("*");
}

void Menu::analisaMovimentos(Estados estadoAnterior, Estados estadoPosterior, Estados estadoCorrente){
  if (xMutexMovimentos != NULL && xSemaphoreTake(xMutexMovimentos, portMAX_DELAY) == pdTRUE) {
    // se o botão do joystick foi acionado
    if(joystick.movimentos[4]){
      // limpa a tela e vai para o estado correspondente à posição selecionada
      proximoEstado = estadoPosterior;
      estadoAtual = LIMPA_TELA;

      // se o próximo estado for a configuração
      if(estadoPosterior == CONFIGURACAO){
        // identifica qual objeto foi escolhido para ser configurado e atualiza o componente de configuração
        if(opcaoAtual){
          configuracao.objetoModificado = relogio.horario;
          configuracao.tipoObjetoModificado = "C";
        } else {
          configuracao.objetoModificado = alarme.alarme;
          configuracao.tipoObjetoModificado = "A";
        }

        configuracao.posicaoCursor = 0;
      // se o próximo estado for a tela inicial e o menu atual for da confirmação da configuração realizada
      } else {
        // verifica se o usuário selecionou a opção salvar, altera o valor do objeto original com o valor do objeto modificado (relógio ou alarme)
        if(!opcaoAtual){
          if(configuracao.tipoObjetoModificado == "C"){
            relogio.converterHorarioString(configuracao.objetoModificado);
          } else {
            alarme.alarme = configuracao.objetoModificado;
          }
        }
      }

      // reseta a opção atual do menu ao trocar de estado
      opcaoAtual = 0;

    // se o joystick foi movimentado para cima
    } else if(joystick.movimentos[0]){
      // e a opção atual era 1
      if(opcaoAtual){
        // decrementa a opção atual, limpa a tela e permanece no mesmo estado
        opcaoAtual = 0;
        estadoAtual = LIMPA_TELA;
        proximoEstado = estadoCorrente;
      }

    // se o joystick foi movimentado para baixo
    } else if(joystick.movimentos[1]){
      // e a opção atual era 0
      if(!opcaoAtual){
        // incrementa a opção atual, limpa a tela e permanece no mesmo estado
        opcaoAtual = 1;
        estadoAtual = LIMPA_TELA;
        proximoEstado = estadoCorrente;
      }

    // se o joystick foi movimentado para a esquerda
    } else if(joystick.movimentos[2]){
      // limpa a tela e volta para o estado anterior
      estadoAtual = LIMPA_TELA;
      proximoEstado = estadoAnterior;

      // reseta a opção atual do menu ao trocar de estado
      opcaoAtual = 0;
    }

    // reseta os movimentos do joystick para evitar interferências
    joystick.resetaMovimentos();
    xSemaphoreGive(xMutexMovimentos);
  }
}

