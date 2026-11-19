# Relógio digital com alarme

Projeto de sistemas embarcados que implementa um relógio digital configurável com alarme, usando um Arduino, um display LCD 16×2 e um joystick. A interface permite ajustar o horário e o alarme; quando os horários coincidem, um buzzer toca até que o botão do joystick seja pressionado.

O projeto usa FreeRTOS para dividir as tarefas de atualização do relógio, leitura do joystick e controle da interface.

## Funcionalidades

- Exibe o horário e o alarme no LCD.
- Permite configurar o horário e o alarme pelo joystick.
- Oferece opções para salvar ou descartar as alterações.
- Aciona um alarme sonoro e visual quando o horário configurado é atingido.
- Executa a atualização do tempo, leitura do joystick e interface em tarefas separadas com FreeRTOS.
- Protege o LCD, o estado da máquina e os movimentos do joystick com mutexes.

## Hardware

Use uma placa Arduino compatível com [Arduino_FreeRTOS](https://github.com/feilipu/Arduino_FreeRTOS_Library), como a **Arduino Mega 2560**. A configuração abaixo corresponde aos pinos definidos no código:

| Componente | Pino do Arduino |
| --- | --- |
| LCD 16×2 — RS | 12 |
| LCD 16×2 — Enable | 11 |
| LCD 16×2 — D4 | 5 |
| LCD 16×2 — D5 | 4 |
| LCD 16×2 — D6 | 3 |
| LCD 16×2 — D7 | 2 |
| Joystick — eixo X | A0 |
| Joystick — eixo Y | A1 |
| Joystick — botão | 7 |
| Buzzer | 6 |

O botão do joystick usa `INPUT_PULLUP`; conecte-o entre o pino 7 e o GND. Faça as demais conexões de alimentação e contraste do LCD conforme o módulo utilizado.

## Como começar

### Requisitos

- Arduino IDE.
- Placa Arduino Mega 2560 ou outra placa compatível com Arduino_FreeRTOS.
- Biblioteca **Arduino_FreeRTOS**.
- Biblioteca `LiquidCrystal` (normalmente incluída com o Arduino IDE).
- `EEPROM` e `Arduino.h`, fornecidos pelo core Arduino.

### Compilar e carregar

1. Instale a biblioteca Arduino_FreeRTOS na Arduino IDE.
2. Abra [`main.ino`](main.ino).
3. Selecione a placa compatível (por exemplo, **Arduino Mega or Mega 2560**) e a porta serial correta.
4. Confira as conexões da tabela acima.
5. Compile e carregue o sketch.

O relógio começa com o horário de compilação do programa. Depois de ligado, o joystick controla os menus:

- Na tela inicial, pressione o botão para abrir o menu.
- Use cima/baixo para escolher entre **Edit alarm** e **Edit clock**; pressione o botão para selecionar.
- Na configuração, use cima/baixo para alterar o dígito selecionado, esquerda/direita para mover o cursor e o botão para avançar.
- Escolha **Save** ou **Discard** no menu de confirmação.
- Quando o alarme tocar, pressione o botão para interrompê-lo.

## Estrutura do projeto

- [`main.ino`](main.ino): inicialização, criação das tarefas FreeRTOS e máquina de estados.
- [`Relogio.h`](Relogio.h) / [`Relogio.cpp`](Relogio.cpp): contagem e exibição do horário.
- [`Alarme.h`](Alarme.h) / [`Alarme.cpp`](Alarme.cpp): armazenamento do horário do alarme e exibição.
- [`JoyStick.h`](JoyStick.h) / [`JoyStick.cpp`](JoyStick.cpp): leitura dos eixos e do botão.
- [`Menu.h`](Menu.h) / [`Menu.cpp`](Menu.cpp): navegação e confirmação das opções.
- [`Configuracao.h`](Configuracao.h) / [`Configuracao.cpp`](Configuracao.cpp): edição dos dígitos de horário e alarme.
- [`Buzzer.h`](Buzzer.h) / [`Buzzer.cpp`](Buzzer.cpp): monitoramento do horário do alarme e sinalização sonora/visual.
- [`MaquinaEstados.h`](MaquinaEstados.h): estados da interface.

## Observações

- O projeto não usa atualmente um módulo RTC: o horário é inicializado com o momento em que o sketch foi compilado e é atualizado pela tarefa FreeRTOS a cada segundo.
- O horário e o alarme configurados ficam em memória durante a execução; as alterações não são persistidas após reiniciar ou desligar a placa.
- O texto dos menus exibido no LCD está em inglês.
