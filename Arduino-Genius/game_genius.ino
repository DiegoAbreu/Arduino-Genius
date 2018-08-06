// Definindo os componentes de acordo com as portas:
#define LED_VERDE 2
#define LED_VERMELHO 3
#define LED_AMARELO 4
#define LED_AZUL 5
#define BOTAO_VERDE 8
#define BOTAO_VERMELHO 9
#define BOTAO_AMARELO 10
#define BOTAO_AZUL 11
#define BUZZER 13

//Definindo variáveis 
#define INDEFINIDO -1
#define UM_SEGUNDO 1000
#define MEIO_SEGUNDO 500
#define UM_QUARTO_DE_SEGUNDO 250

//Aqui definimos a quantidade de sequências(rodadas) que o jogo terá
#define TAMANHO_SEQUENCIA 5

//Enumerando os estados do jogo
enum Estados {
  PRONTO_PARA_PROXIMA_RODADA,
  USUARIO_RESPONDENDO,
  JOGO_FINALIZADO_SUCESSO,
  JOGO_FINALIZADO_FALHA
};

//Iniciando váriaveis e Portas
int sequenciaLuzes[TAMANHO_SEQUENCIA];
int rodada = 0;
int ledsRespondidos = 0;

void setup() {
  Serial.begin(9600);
  iniciaPortas();
  iniciaJogo();
}

void iniciaPortas() {
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);

  pinMode(BOTAO_VERDE, INPUT_PULLUP);
  pinMode(BOTAO_AMARELO, INPUT_PULLUP);
  pinMode(BOTAO_VERMELHO, INPUT_PULLUP);
  pinMode(BOTAO_AZUL, INPUT_PULLUP);

  pinMode(BUZZER, OUTPUT);
}

//Pisca led
int piscaLed(int portaLed) {
  verificaSomDoLed(portaLed);
  digitalWrite(portaLed,HIGH);
  delay(UM_SEGUNDO);
  digitalWrite(portaLed,LOW);
  delay(UM_QUARTO_DE_SEGUNDO);
  return portaLed;
}

//Toca Som
void tocaSom(int frequencia) {
  tone(BUZZER,frequencia, 100);
}

//Associando som aos leds
void verificaSomDoLed(int portaLed) {
  switch (portaLed) {
    case LED_VERDE:
      tocaSom(2000);
      break;
    case LED_AMARELO:
      tocaSom(2200);
      break;
    case LED_VERMELHO:
      tocaSom(2400);
      break;
    case LED_AZUL:
      tocaSom(2500);
      break;
  }
}

//  --- O JOGO ---
//Função para iniciar o jogo
void iniciaJogo() {
  
  int jogo = analogRead(0);
  randomSeed(jogo);

  for (int i = 0; i < TAMANHO_SEQUENCIA; i++) {
    sequenciaLuzes[i] = sorteiaCor();
  }
}

int sorteiaCor() {
  return random(LED_VERDE, LED_AZUL + 1);
}

void loop() {
  switch (estadoAtual()) {
    case PRONTO_PARA_PROXIMA_RODADA:
      Serial.println("Pronto para a proxima rodada");
      preparaNovaRodada();
      break;
    case USUARIO_RESPONDENDO:
      Serial.println("Usuario respondendo");
      processaRespostaUsuario();
      break;
    case JOGO_FINALIZADO_SUCESSO:
      Serial.println("Jogo finalizado com sucesso");
      jogoFinalizadoSucesso();
      break;
    case JOGO_FINALIZADO_FALHA:
      Serial.println("Jogo finalizado com falha");
      jogoFinalizadoFalha();
      break;
  }
  delay(MEIO_SEGUNDO);
}

//Nova Rodada em caso de acerto
void preparaNovaRodada() {
  rodada++;
  ledsRespondidos = 0;
  if (rodada <= TAMANHO_SEQUENCIA) {
    tocaLedsRodada();
  }
}

void processaRespostaUsuario() {
  int resposta = checaRespostaJogador();
  if (resposta == INDEFINIDO) {
    return;
  }
  if (resposta == sequenciaLuzes[ledsRespondidos]) {
    ledsRespondidos++; 
  } else {
    Serial.println("Sequencia errada");
    rodada = TAMANHO_SEQUENCIA + 2;
  }
}

//Loop para iniciar uma nova rodada
int estadoAtual() {
  if (rodada <= TAMANHO_SEQUENCIA) {
    if (ledsRespondidos == rodada) {
      return PRONTO_PARA_PROXIMA_RODADA; 
    } else {
      return USUARIO_RESPONDENDO;
    }
  } else if (rodada == TAMANHO_SEQUENCIA + 1) {
    return JOGO_FINALIZADO_SUCESSO;
  } else {
    return JOGO_FINALIZADO_FALHA;
  }
}

void tocaLedsRodada() {
  for (int i = 0; i < rodada; i++) {
    piscaLed(sequenciaLuzes[i]);
  }
}

//Checagem de resposta
int checaRespostaJogador() {
  if (digitalRead(BOTAO_VERDE) == LOW) {
    return piscaLed(LED_VERDE);
  }
  if (digitalRead(BOTAO_AMARELO) == LOW) {
    return piscaLed(LED_AMARELO);
  }
  if (digitalRead(BOTAO_VERMELHO) == LOW) {
    return piscaLed(LED_VERMELHO);
  }
  if (digitalRead(BOTAO_AZUL) == LOW) {
    return piscaLed(LED_AZUL);
  }
  return INDEFINIDO;
}

//Fim de jogo - Vitória
void jogoFinalizadoSucesso() {
  delay(UM_QUARTO_DE_SEGUNDO);
  tocaSom(5000);
  piscaLed(LED_VERDE);
  tocaSom(5000);
  piscaLed(LED_VERMELHO);
  tocaSom(5000);
  piscaLed(LED_AMARELO);
  tocaSom(5000);
  piscaLed(LED_AZUL); 
}

//Fim de jogo - Derrota
void jogoFinalizadoFalha() {
  delay(MEIO_SEGUNDO);
  tocaSom(400);
  digitalWrite(LED_VERDE,HIGH);
  digitalWrite(LED_AMARELO,HIGH);
  digitalWrite(LED_VERMELHO,HIGH);
  digitalWrite(LED_AZUL,HIGH);
  delay(MEIO_SEGUNDO);
  digitalWrite(LED_VERDE,LOW);
  digitalWrite(LED_AMARELO,LOW);
  digitalWrite(LED_VERMELHO,LOW);
  digitalWrite(LED_AZUL,LOW);
  delay(MEIO_SEGUNDO);
}



