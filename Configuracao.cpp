// importa bibliotecas utilizadas
#include "Configuracao.h"
#include "MaquinaEstados.h";
#include "JoyStick.h";
#include <LiquidCrystal.h>

// importa componentes utilizados por referência
extern LiquidCrystal lcd;
extern Estados estadoAtual;
extern Estados proximoEstado;
extern JoyStick joystick;

Configuracao::Configuracao(){}

void Configuracao::exibirConfiguracao(int linha){
  // exibe o tipo do objeto que está sendo modificado (C ou A)
  lcd.setCursor(0, linha);
  lcd.print(tipoObjetoModificado + ": ");

  lcd.setCursor(5, linha);

  // exibe dígitos antes da posição atual do cursor
  lcd.print(objetoModificado.substring(0, posicaoCursor));

  // o dígito correspondente à posição atual do cursor é exibido com sublinhado (em configuração)
  lcd.createChar(0, (uint8_t*)numerosSublinhados[(objetoModificado[posicaoCursor] - '0')]);
  lcd.setCursor(posicaoCursor + 5, linha);
  lcd.print((char)0);

  // exibe dígitos após a posição atual do cursor
  lcd.setCursor(5 + posicaoCursor + 1, linha);
  lcd.print(objetoModificado.substring(posicaoCursor + 1, objetoModificado.length()));
}

void Configuracao::analisaMovimentos(void){
    // se o botão do joystick foi pressionado
    if(joystick.movimentos[4]){
        // limpa a tela e muda para a tela de confirmação
        proximoEstado = CONFIRMAR_CONFIGURACAO;
        estadoAtual = LIMPA_TELA;

    // se o joystick foi movimentado para baixo
    } else if(joystick.movimentos[1]){
        // salva dígito em configuração em hexadecimal
        bufferDigito = (int) objetoModificado[posicaoCursor];

        // se ele for maior do que 48 (0)
        if(bufferDigito > 48){
          // decrementa o valor
          bufferDigito--;

          // atualiza o objeto modificado
          objetoModificado[posicaoCursor] = (char) bufferDigito;

        }

        // limpa a tela e permanece no modo configuração
        estadoAtual = LIMPA_TELA;
        proximoEstado = CONFIGURACAO;

    // se o joystick foi movimentado para cima
    } else if(joystick.movimentos[0]){
        // salva dígito em configuração em hexadecimal
        bufferDigito = (int) objetoModificado[posicaoCursor];

        // se ele for menor do que 57 (9)
        if(bufferDigito < 57){
          // incrementa
          bufferDigito++;

          // atualiza o objeto modificado
          objetoModificado[posicaoCursor] = (char) bufferDigito;

          // se o dígito atualizado for a dezena dos minutos ou dos segundos e o valor atual for maior do que 53 (5)
          if((posicaoCursor == 3 || posicaoCursor == 6) && (int) objetoModificado[posicaoCursor] > 53){
            // o valor do dígito não pode ultrapassar 5
            objetoModificado[posicaoCursor] = '5'; 

          // se o dígito atualizado for a dezena das horas
          } else if(posicaoCursor == 0){
            // se ele for maior do que 50 (2)
            if((int) objetoModificado[posicaoCursor] > 50) {
              // o valor do dígito não pode ultrapassar 2
              objetoModificado[posicaoCursor] = '2';
              // se ele for maior do que 49 (1) e o dígito das unidades for maior do que 51 (3)
            } else if((int) objetoModificado[posicaoCursor] > 49 && (int) objetoModificado[1] > 51){
              // o valor do dígito não pode ultrapassar 1
              objetoModificado[posicaoCursor] = '1';
            }

          // se o dígito atual for a unidade das horas, o valor da dezena é igual a 50 (2) e o valor do dígito é maior do que 51 (3)
          } else if(posicaoCursor == 1 && (int) objetoModificado[0] == 50 && (int) objetoModificado[posicaoCursor] > 51){ 
            // o valor do dígito não pode ultrapassar 3
            objetoModificado[posicaoCursor] = '3';
          }

        }

        // limpa a tela e permanece no estado de configuração
        estadoAtual = LIMPA_TELA;
        proximoEstado = CONFIGURACAO;

    // se o joystick foi movimentado para a esquerda
    } else if(joystick.movimentos[2]){
        // decremneta posição do cursor
        posicaoCursor--;

        // se a posição d cursor coincidir com o caracter :, decrementa
        if(objetoModificado[posicaoCursor] == ':'){
          posicaoCursor--;
        // se for menor do que zero, mantém em 0
        } else if(posicaoCursor < 0){
          posicaoCursor = 0;
        }

        // limpa a tela e permanece no estado de configuração
        estadoAtual = LIMPA_TELA;
        proximoEstado = CONFIGURACAO;

    // se o joystick foi movimentado para a direita
    } else if(joystick.movimentos[3]){
      
        posicaoCursor++;

        // se a posição d cursor coincidir com o caracter :, incrementa
        if(objetoModificado[posicaoCursor] == ':'){
          posicaoCursor++;
        // se for maior do que o tamanho do objeto, permanece na última posição válida
        } else if(posicaoCursor > 7){
          posicaoCursor = 7;
        }

        // limpa a tela e permanece no estado de configuração
        estadoAtual = LIMPA_TELA;
        proximoEstado = CONFIGURACAO;
    }
  
  // limpa todos os movimentos do joystick para evitar interferências
  joystick.resetaMovimentos();

}
