// /*
//   * Autores:
//   * Estefane de Lima dos Santos
//   * Kauane dos Santos Vieira
//   * 
// */

// bibliotecas do FreeRTOS
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#include <Wire.h>
#include <RTClib.h> 

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

// variáveis de controle da máquina de estados
volatile Estados estadoAtual = TELA_INICIAL;
volatile Estados proximoEstado = TELA_INICIAL;

// configuração do display LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// instanciação dos componentes
Relogio relogio;
Alarme alarme;
JoyStick joystick;
Menu menu;
Buzzer buzzer;
Configuracao configuracao;

// mutexes
SemaphoreHandle_t xMutexLCD;
SemaphoreHandle_t xMutexEstado;
SemaphoreHandle_t xMutexMovimentos;

void TaskRelogio(void *pvParameters);
void TaskJoystick(void *pvParameters);
void TaskMaquinaEstados(void *pvParameters);

void setup() {
  Serial.begin(9600);

  int numeroDoLog = EEPROM.read(10); 
  numeroDoLog++; 
  EEPROM.write(10, numeroDoLog);

  relogio.converterHorarioString(__TIME__);

  pinMode(7, INPUT_PULLUP);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  lcd.begin(16, 2);

  xMutexLCD = xSemaphoreCreateMutex();
  xMutexEstado = xSemaphoreCreateMutex();
  xMutexMovimentos = xSemaphoreCreateMutex();

  if (xMutexLCD != NULL && xMutexEstado != NULL) {
    // criação das tarefas
    // relógio --> maior prioridade (atualiza tempo e monitora alarme)
    xTaskCreate(TaskRelogio, "Relogio", 128, NULL, 3, NULL);

    // joystick --> prioridade média (lê entradas do usuário)
    xTaskCreate(TaskJoystick, "Joystick", 128, NULL, 2, NULL);

    // maquina estados --> menor prioridade (exibe valores de acordo com o estado atual) e permite configuração de alarme/relógio
    xTaskCreate(TaskMaquinaEstados, "MaquinaEstados", 128, NULL, 1, NULL);
  }
}

void loop() {}

void TaskRelogio(void *pvParameters) {
  // ultimo instante em que a tarefa saiu do estado bloqueado
  TickType_t xLastWakeTime = xTaskGetTickCount();
  // frequencia, de ticks para ms, para a tarefa ser acordada
  const TickType_t xFrequencia = pdMS_TO_TICKS(1000); // 1s

  for (;;) {
    // Aguarda 1000ms e executa o método de contar tempo sempre com a mesma periodicidade
    vTaskDelayUntil(&xLastWakeTime, xFrequencia);

    relogio.contarTempo();
  }
}

void TaskJoystick(void *pvParameters) {
  const TickType_t xDelay = pdMS_TO_TICKS(30); // varre joystick a cada 30ms

  for (;;) {
    // solicita mutex para acessar o vetor de movimentos
    if (xSemaphoreTake(xMutexMovimentos, portMAX_DELAY) == pdTRUE) {
      joystick.lerBotaoJoyStick();
      joystick.lerJoyStickEixoY();
      joystick.lerJoyStickEixoX();

      // entrega mutex depois que varreu o joystick
      xSemaphoreGive(xMutexMovimentos);
    }

    // executa a task a cada 30ms, mas pode gerar atrasos
    vTaskDelay(xDelay);
  }
}

void TaskMaquinaEstados(void *pvParameters) {
  const TickType_t xDelay = pdMS_TO_TICKS(50); // Atualiza tela a cada 50ms

  for (;;) {
    // protege o acesso ao LCD e às transições de estado
    // solicita mutex, passa o tempo máximo de espera (portMAX_DELAY) e verifica se o recurso foi obtido (pdTrue)
    if (xSemaphoreTake(xMutexLCD, portMAX_DELAY) == pdTRUE) {
      // mesmo procedimento para MutexEstado
      if (xSemaphoreTake(xMutexEstado, portMAX_DELAY) == pdTRUE) {

        switch (estadoAtual) {
          case TELA_INICIAL:
            if (joystick.movimentos[4]) {
              proximoEstado = MENU;
              estadoAtual = LIMPA_TELA;
            }
            joystick.resetaMovimentos();

            relogio.exibirRelogio();
            alarme.exibirAlarme(1);
            break;

          case MENU:
            menu.analisaMovimentos(TELA_INICIAL, CONFIGURACAO, MENU);
            menu.exibirMenu("Edit alarm", "Edit clock");
            break;

          case CONFIGURACAO:
            configuracao.analisaMovimentos();
            configuracao.exibirConfiguracao(0);
            break;

          case CONFIRMAR_CONFIGURACAO:
            menu.analisaMovimentos(CONFIGURACAO, TELA_INICIAL, CONFIRMAR_CONFIGURACAO);
            menu.exibirMenu("Save", "Discard");
            break;

          case LIMPA_TELA:
            lcd.clear();
            estadoAtual = proximoEstado;
            break;

          case ALARME:
            buzzer.tocarAlarme();
            buzzer.exibirMensagem();
            buzzer.analisaMovimentos();
            break;
        }

        // libera mutex de estado ao sair do switch
        xSemaphoreGive(xMutexEstado);
      }

      // libera mutex do LCD
      xSemaphoreGive(xMutexLCD);
    }

    // começa a contar o tempo de bloqueio da tarefa
    vTaskDelay(xDelay);
  }
}

