#include "JoyStick.h"

JoyStick::JoyStick(){}

void JoyStick::lerJoyStickEixoY(){
  // lê o valor do pino conectado ao eixo y do joystick
  valorYAtual = analogRead(pinY);

  // se tiver acontecido movimento para a esquerda ou para a direita, atualiza vetor de movimentos
  if(valorYAtual > 800 && valorYAnterior < 800){
    movimentos[2] = 1;

  } else if(valorYAtual < 200 && valorYAnterior > 200){
    movimentos[3] = 1;

  }

  valorYAnterior = valorYAtual;
}

void JoyStick::resetaMovimentos(){
  // retorna todos os valores do vetor de movimentos para zero
  for(int i = 0; i < 5; i++){
    movimentos[i] = 0;
  }
}

void JoyStick::lerJoyStickEixoX(){
  // lê o valor do pino conectado ao eixo x do joystick
  valorXAtual = analogRead(pinX);

  // se tiver acontecido movimento para baixo ou para cima, atualiza vetor de movimentos
  if(valorXAtual > 800 && valorXAnterior < 800){
    movimentos[1] = 1;

  } else if(valorXAtual < 200 && valorXAnterior > 200){
    movimentos[0] = 1;

  }

  valorXAnterior = valorXAtual;
}

void JoyStick::lerBotaoJoyStick(){
    debouncing++;

    // a cada 50ms
    if(debouncing == 50){
      debouncing = 0;
      // lê o valor do pino conectado ao botão do joystick
      valorZAtual = !digitalRead(pinZ);

      // se o botão tiver sido pressionado
      if(valorZAnterior != valorZAtual && valorZAtual == 1){
        // atualiza vetor de movimentos
        movimentos[4] = 1;
      }

      valorZAnterior = valorZAtual;

    }

}


