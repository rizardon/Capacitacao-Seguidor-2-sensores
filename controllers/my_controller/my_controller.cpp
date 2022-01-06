// File:          my_controller.cpp
// Date:
// Description:
// Author:
// Modifications:

// You may need to add webots include files such as
// <webots/DistanceSensor.hpp>, <webots/Motor.hpp>, etc.
// and/or to add some other includes
#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/DistanceSensor.hpp>

#define TIME_STEP 64
#define MAX_SPEED 6.28 //Define a velocidade maxima que o motor consegue atingir
// All the webots classes are defined in the "webots" namespace
using namespace webots;


double reguladorVelocidade(double velocidade, double sensor_val, bool *primeira_vez){
  //se o sensor retornar valores menores que 5, considera-se que ele ta vendo branco
  if(sensor_val < 5.5){
    //a velocidade vira maxima ao ver branco, e a condicao de primeira vez volta a ser verdadeira
    velocidade = MAX_SPEED;
    *primeira_vez = true;
  }
  else if(sensor_val > 8){ //se o sensor retornar valores maiores que 8, considera-se que ele ta vendo preto
    if(*primeira_vez){
      //ao ver preto pela primeira vez, o motor ajusta levemente e muda a condicao de primeira vez pra falso
      velocidade *= 0.95;
      *primeira_vez = false;
    } else {  
      //ao ver preto pela segunda ou mais vezes seguidas, o motor reduz cada vez mais a velocidade
      velocidade *= 0.7;
    }
  }
  return velocidade;
}

// This is the main program of your controller.
// It creates an instance of your Robot instance, launches its
// function(s) and destroys it at the end of the execution.
// Note that only one instance of Robot should be created in
// a controller program.
// The arguments of the main function can be specified by the
// "controllerArgs" field of the Robot node
int main(int argc, char **argv) {
  // create the Robot instance.
  Robot *robot = new Robot();

  // You should insert a getDevice-like function in order to get the
  // instance of a device of the robot. Something like:
  //  Motor *motor = robot->getMotor("motorname");
  //  DistanceSensor *ds = robot->getDistanceSensor("dsname");
  //  ds->enable(timeStep);

  //cria o objeto do motor direito e esquerdo
  Motor *motor_esquerdo = robot->getMotor("left wheel motor");
  Motor *motor_direito = robot->getMotor("right wheel motor");

  //cria o objeto do sensor direito e esquerdo
  DistanceSensor *sensor_direito = robot->getDistanceSensor("sensor_left");
  DistanceSensor *sensor_esquerdo = robot->getDistanceSensor("sensor_right");

  //habilita os sensores direitos e esquerdos
  sensor_esquerdo->enable(TIME_STEP);
  sensor_direito->enable(TIME_STEP);

  //Define a posicao dos motores como infinita, necessario para conseguir controlar os motores
  //pela velocidade
  motor_esquerdo->setPosition(INFINITY);
  motor_direito->setPosition(INFINITY);

  //coloca a velocidade inicial dos motores como a maxima possivel
  motor_esquerdo->setVelocity(MAX_SPEED);
  motor_direito->setVelocity(MAX_SPEED);

  //cria a variavel responsavel por controlar as velocidades dos motores
  //O valor inicial delas eh o maximo possivel
  double velocidade_esq = MAX_SPEED;
  double velocidade_dir = MAX_SPEED;

  //define a variavel responsavel por indicar se eh a primeira vez que o sensor ve a linha preta(true), 
  //ou se ele viu 2 ou mais vezes seguidas(false)
  bool primeira_vez_dir = true;
  bool primeira_vez_esq = true;

  // Main loop:
  // - perform simulation steps until Webots is stopping the controller
  while (robot->step(TIME_STEP) != -1) {
    // Read the sensors:
    // Enter here functions to read sensor data, like:
    //  double val = ds->getValue();

    // Process sensor data here.

    // Enter here functions to send actuator commands, like:
    //  motor->setPosition(10.0);

    //le o valor dos sensores de distancia e atribui a eles uma variavel
    double sensor_esq_val = sensor_esquerdo->getValue();
    double sensor_dir_val = sensor_direito->getValue();

    //chama a funcao que regula a velocidade do robo de acordo com a leitura dos sensores
    velocidade_esq = reguladorVelocidade(velocidade_esq,sensor_esq_val,&primeira_vez_esq);
    velocidade_dir = reguladorVelocidade(velocidade_dir,sensor_dir_val,&primeira_vez_dir);
    
    //apos atualizar o valor da velocidade com a funcao assima, esta aplica esse valor aos motores
    motor_esquerdo->setVelocity(velocidade_esq);
    motor_direito->setVelocity(velocidade_dir);

    //prints para teste
    std::cout << "Valor do Sensor Esquerdo: " << sensor_esq_val << std::endl;
    std::cout << "Valor do Sensor Direito: " << sensor_dir_val << std::endl;
    std::cout << "Velocidade do Motor Direito: " << velocidade_dir << std::endl;
    std::cout << "Velocidade do Motor Esquerdo: " << velocidade_esq << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

    //para sensores em cima da linha preta: 8 pra cima
    //para sensores em cima do branco: 5.5 pra baixo
    //os sensores estao invertidos? --> tive que trocar na hora de criar os objetos

  };

  // Enter here exit cleanup code.

  delete robot;
  return 0;
}
