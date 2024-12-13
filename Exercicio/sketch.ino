 
#include <WiFi.h>
#include <HTTPClient.h>

// #define botao
#define verde 2 // Pino utilizado para controle do led verda
#define vermelho 40 // Pino utilizado para controle do led vermelho
#define amarelo 9 // Pino utilizado para controle do led amarelo

const int buttonPin = 18;  // constante que diz o pino do botao
int buttonState = 0;  // variavel que armazena o valor do estado do botao
const int ldrPin = 4;  // constante que diz qual o pino do sensor LDR
int threshold=600;

bool buttonLastState = LOW; // armazena o ultimo estado do botão em low

void setup() {

  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode (amarelo,OUTPUT);
  pinMode (verde,OUTPUT);
  pinMode (vermelho,OUTPUT);

  // Inicialização das entradas
  pinMode(buttonPin, INPUT_PULLUP); // inicializa o botão como um pino de entrada (input) com resitor pull down
  digitalWrite (amarelo, LOW);// inicializa o LED amarelo no estado LOW
  digitalWrite(vermelho, LOW); // inicializa o Led vermelho  no estado Low 
  digitalWrite(verde, LOW); // inicializa o Led verde  no estado Low

  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600

  WiFi.begin("Iphone de Bernardo", "123456789", 6); // Conexão à rede WiFi aberta com SSID 'Iphone de Bernardo'

 if (WiFi.status() != WL_CONNECTED) { // aqui verificaremos se há uma conexão esstabelecida retornando pra gente "wifi conectdo"
    delay(100);
    Serial.print(" Wifi conectado");
  }



  // Verifica estado do botão
  buttonState = digitalRead(buttonPin); //  cria a variavel buttonstate apartir da leitura do buttonpin registrado no pino 18

  if (buttonState == LOW) {
    Serial.println("Botão pressionado!");
  } 
  else {
    Serial.println("Botão não pressionado!");
  }

  if(WiFi.status() == WL_CONNECTED){ // Se o ESP32 estiver conectado à Internet
  
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET(); // Código do Resultado da Requisição HTTP

    if (httpResponseCode>0) {
      Serial.print("HTTP Codigo de resposta: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Erro no codigo: ");
      Serial.println(httpResponseCode);
      }
      http.end();
    }

  else {
    Serial.println("HTTP FALHOU");
  }
}

void loop() {

  // declara o botão como booleano e armazena a sua variavel como buttom state
  bool buttonState = digitalRead(buttonPin);

  int ldrstatus=analogRead(ldrPin);

  if(ldrstatus>=threshold){

    //4) Quando estiver escuro (segundo a leitura analógica do sensor LDR), o protótipo deve ativar o modo noturno e piscar o led amarelo a cada segundo (até 1,0 ponto);
    // Aqui é colocado o led amarelo pra piscar de 1 em 1 segundo
        digitalWrite(amarelo, HIGH);// liga o led
        delay(1000); // daley de 1s
        digitalWrite(amarelo, LOW);// desliga o led
        delay(1000);

      // print no console pra indicar qu está de noite
    Serial.print("Está de Noite Ligar Luz");
    Serial.println(ldrstatus);

  }
  else {
    // Caso acima não funcionar o significa que está de dia abaixo é 
    // mostrado o log através de um print no terminal
    Serial.print("Está de Dia desligar Luz");
    Serial.println(ldrstatus);

  // 3s no verde 
  digitalWrite(vermelho, LOW);
  digitalWrite(amarelo, LOW);
  digitalWrite(verde, HIGH);
  delay(3000); // 3 segundos

// 2s no amarelo
  digitalWrite(vermelho, LOW);
  digitalWrite(amarelo, HIGH);
  digitalWrite(verde, LOW);
  delay(2000); // 2 segundos

    // 5s no vermelho
     digitalWrite(vermelho, HIGH);
  digitalWrite(amarelo, LOW);
  digitalWrite(verde, LOW);
  delay(5000); // 5 segundos

  
  }
}