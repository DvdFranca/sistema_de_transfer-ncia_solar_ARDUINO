//Controlador para transferência solar/rede automático
//funcionamento: acima de 27 volts(solar)abaixo de 23 volts(rede)
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

float tensaoA0;
float aRef=5;
float relacaoA0=11;
#define AMOSTRAS 12
int state = digitalRead(13);

void setup() { 
    Serial.begin(9600);
    pinMode(10, OUTPUT);//acende o led vermelho indicando "rede"
    pinMode(11, OUTPUT);//acende o led verde indicando "solar"
    pinMode(13, OUTPUT);//aciona o inversor 5 segundos antes do sistema transicionar 
    lcd.begin(); // inicializa o lcd
    lcd.backlight();
}

float lePorta(uint8_t portaAnalogica) {
  float total=0;  
  for (int i=0; i<AMOSTRAS; i++) {
    total += 1.0 * analogRead(portaAnalogica);
    delay(5);
  }
  return total / (float)AMOSTRAS;
}  

void mostrarTensoes() {
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.print("BATERIA");
  lcd.print("   ");
  lcd.print("STATUS");
  lcd.setCursor (0,1);
  lcd.print(tensaoA0 * relacaoA0);
  lcd.print(" ");
  lcd.print("V");
  lcd.print("   ");
  lcd.print(" ");
  
  if (state == HIGH) {
    lcd.print("SOLAR");
  } else {
    lcd.print("REDE"); 
  }

  lcd.print("   ");
  
  if (tensaoA0 * relacaoA0 > 27.00) {
    digitalWrite(13, HIGH);//liga o inversor
    delay(5000); //delay para dar tempo do inversor estabilizar a tensão
    digitalWrite(11, HIGH);//saída para a contatora(solar)
    digitalWrite(10, LOW);//saída para a contatora(rede)
    
  } else if (tensaoA0 * relacaoA0 < 23.00) {
    digitalWrite(13, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, HIGH);    
  }
}
 

void loop() {
  tensaoA0 = (lePorta(A0) * aRef) / 1024.0;
  mostrarTensoes();
  delay(100);
  state = digitalRead(13); // lê o estado do pino digital 13
  if (state == HIGH) {
      lcd.print("SOLAR");
      lcd.setCursor(1,1);
} else {
      lcd.print("REDE");
      lcd.setCursor(1,1);
  }
}


