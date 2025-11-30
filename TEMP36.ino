// --- DEFINIÇÃO DOS PINOS ---
// Pinos digitais (saídas)
const int pinoMotor = 6;    // Motor do ventilador (D6)
const int pinoBuzina = 7;   // Buzina de emergência (D7)
const int pinoLED = 13;     // LED de emergência (LED na placa ou D13)

// Pino analógico (entrada)
const int pinoSensorTemp = A0; // Sensor de temperatura TMP36 (A0)

// --- DEFINIÇÃO DAS TEMPERATURAS DE ACIONAMENTO ---
const int TEMP_MOTOR_LIGAR = 30; // 30 °C
const int TEMP_EMERGENCIA = 50;  // 50 °C

void setup() {
  // 1. Configura as portas digitais como SAÍDAS
  pinMode(pinoMotor, OUTPUT);
  pinMode(pinoBuzina, OUTPUT);
  pinMode(pinoLED, OUTPUT);
  
  // 2. Garante que todos os dispositivos comecem DESLIGADOS
  digitalWrite(pinoMotor, LOW);
  digitalWrite(pinoBuzina, LOW);
  digitalWrite(pinoLED, LOW);
  
  // 3. Inicia a comunicação serial para monitoramento
  Serial.begin(9600);
  Serial.println("Simulador de Estufa\n");
}

void loop() {
  // 1. FAZER A LEITURA DA TEMPERATURA
  int valorAnalogico = analogRead(pinoSensorTemp);
  
  // Conversão para Tensão (em Volts)
  // 1023 (resolução de 10 bits) mapeado para 5.0V (tensão de referência do Arduino)
  float tensao_volts = valorAnalogico * (5.0 / 1023.0);
  
  // Conversão TENSÃO -> TEMPERATURA para o TMP36
  // Fórmula: Temperatura (°C) = (Tensão - 0.5V) * 100
  // O 0.5V (500mV) é o offset do sensor em 0°C.
  // O 100 vem de 10mV por grau Celsius (100 = 1 / 0.01V)
  float temperatura = (tensao_volts - 0.5) * 100.0; 
  
  // Exibe a leitura atual
  Serial.print("Tensao lida: ");
  Serial.print(tensao_volts, 3); // 3 casas decimais
  Serial.print(" V -> Temperatura Atual: ");
  Serial.print(temperatura, 1); // 1 casa decimal
  Serial.println(" °C");
  
  // --- LÓGICA DE CONTROLE ---
  
  // (c) CASO A TEMPERATURA ULTRAPASSE OS 50 °C (Emergência)
  if (temperatura > TEMP_EMERGENCIA) {
    Serial.println("!!! ALERTA DE EMERGÊNCIA: Temperatura > 50°C. !!!");
    
    // Aciona o LED (D13) e a Buzina (D7)
    digitalWrite(pinoLED, HIGH);
    digitalWrite(pinoBuzina, HIGH);
    
    // Motor permanece ligado
    digitalWrite(pinoMotor, HIGH);
  
  } 
  // (b) CASO A TEMPERATURA SEJA IGUAL OU MAIOR A 30 °C (Ventilação)
  else if (temperatura >= TEMP_MOTOR_LIGAR) {
    Serial.println("Ventilador Ligado (>= 30°C).");
    
    // Aciona o motor (D6)
    digitalWrite(pinoMotor, HIGH);
    
    // Garante que o LED e a Buzina estejam DESLIGADOS
    digitalWrite(pinoLED, LOW);
    digitalWrite(pinoBuzina, LOW);
  } 
  // Caso a temperatura esteja abaixo de 30 °C (Normal)
  else {
    Serial.println("Temperatura Normal (< 30°C).");
    
    // Desliga todos os dispositivos
    digitalWrite(pinoMotor, LOW);
    digitalWrite(pinoLED, LOW);
    digitalWrite(pinoBuzina, LOW);
  }

  // Espera um segundo para a próxima leitura
  delay(500); 
}