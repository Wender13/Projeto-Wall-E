#include <Servo.h> //Manipular motor servo
#include <AFMotor.h> //Manipular motores DC 
#include <NewPing.h> //Sensor de distância

#define trigPin A0 //Pino TRIG do sensor no pino analógico A0
#define echoPin A1 //Pino ECHO do sensor no pino analógico A1
#define distMax 300 //Distância máxima para sensor

Servo myservo; //Declaração do motor servo

NewPing sensorDist(trigPin, echoPin, distMax);

AF_DCMotor motor1(1); //Motor DC na porta 1
AF_DCMotor motor2(2); //Motor DC na porta 2


//Variáveis de distância e número aleatório
float distancia = 0;
float distanciaDireita = 0;
float distanciaEsquerda = 0;
int aleatorio;

void setup() {
  Serial.begin(9600);
  myservo.attach(10); //Pino do motor servo 
  motor1.setSpeed(100); //Velocidade motor DC 1
  motor2.setSpeed(100); //Velocidade motor DC 2
  olharFrente(); //Ajusta posição do sensor ultrassônico
  delay(500);
  distFrente(); //Calcula distância inicial para a função principal
}

void loop() {
  pensar();//Função principal
}

void pensar(){
  //Inicialmente ele anda enquanto a distância a sua frente é maior do que o estabelelecido
  distFrente();
  while(distancia > 15 || distancia < 0.1){
    andar();
    distFrente();
  }
  //Caso a distância seja menor do que a estabelecida, ele para, vai um pouco para trás, analisa as distâncias à direita e esquerda e seleciona a que for mais conveniente
  parar();
  distFrente();
  Serial.println(distancia);
  re();
  distEsquerda();
  distDireita();
  delay(1000);
  olharFrente();
  delay(500);
  decidirDirecao();
}

void andar(){ //Função para andar para frente
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
}

void re(){ //Função para andar para trás
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  delay(770);
  parar();
}

void zerarDistancias(){
  float distancia = 0;
  float distanciaDireita = 0;
  float distanciaEsquerda = 0; 
}

void parar(){
  motor1.run(RELEASE); // Parar motor 1  
  motor2.run(RELEASE); // Parar motor 2 
}

void girarParaTras(){ //Função para dar um volta
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  delay(1400);
  parar();
}

void girarParaEsquerda(){ //Função para andar para girar para a esquerda
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  delay(1000);
  parar();
}

void girarParaDireita(){ //Função para andar para girar para a direita
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  delay(1000);
  parar();
}

void olharEsquerda(){
  myservo.write(180); //Ajustar motor servo para esquerda
}

void olharDireita(){
  myservo.write(40); //Ajustar motor servo para direita
}

void olharFrente(){
  myservo.write(110); //Ajustar posição inicial do motor servo;
}

void direcaoAleatoria(){ //Seleciona uma direção aleatória
  aleatorio = random(3);
  if (aleatorio == 0){
    girarParaEsquerda();
  } else if(aleatorio == 1) {
    girarParaDireita();
  } else {
    girarParaTras();
  }
}

void decidirDirecao(){ //Analisa para qual lado ir
    if (distanciaDireita < 15 && distanciaEsquerda < 15 && distancia < 15){
        girarParaTras();
    } else if (distanciaDireita > 15 && distanciaEsquerda > 15|| distanciaDireita < 1 && distanciaEsquerda < 1){
        direcaoAleatoria();
    } else if (distanciaDireita > 15 || distanciaDireita < 1){
        girarParaDireita();
    } else {
      girarParaEsquerda();
    }
    zerarDistancias();
}

void distFrente(){ //Analisa a distância a sua frente
  distancia = sensorDist.ping_cm();
}

void distDireita(){ //Analisa a distância a sua direita
  olharDireita();
  delay(500);
  distanciaDireita = sensorDist.ping_cm();
  delay(500);
}

void distEsquerda(){ //Analisa a distância a sua esquerda
  olharEsquerda();
  delay(500);
  distanciaEsquerda = sensorDist.ping_cm();
  delay(500);
}
