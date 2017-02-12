

## Sensor de Reflectância com o WeMos R1

Teste simples com um sensor de reflectância e um LED. O objetivo é avaliar a compatibilidade com sensores projetados para Arduíno. O WeMos R1 tem opção de 5V e 3.3V, o sensor de reflectância foi usado com estas duas voltagens e funcionou bem. Um LED foi acoplado no pino D15 e um código simples foi criado para validar a entrada de dados (sensor de reflectância), processamento (ESP) e saída (LED). O código foi desenvolvido com a IDE do Arduíno.

Montagem do circuíto:

![WeMos](https://github.com/orivaldosantana/GPRo/blob/master/ESP/reflectance_sensor/IMG_20170211_224446126_HDR.jpg)

Código:

```c++
#define REFLECTANCE_SENSOR D8
#define LED D15

void setup() {
  pinMode(REFLECTANCE_SENSOR,INPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  Serial.println("\n\nESP8266 ligado.\n\n");
}

void loop() {
 if ( digitalRead(REFLECTANCE_SENSOR) == 0 ) {
    digitalWrite(LED, HIGH);
    Serial.println("... HIGH ... ");
 }
 else {
    digitalWrite(LED, LOW);
    Serial.println("... LOW ...");
 }
 delay(100);
}
```

## Referência

* Placa ESP WeMos D1 R2 - Semelhante ao Arduíno UNO <http://pedrominatel.com.br/pt/esp32/wemos-d1-o-esp8266-com-cara-de-arduino/>.
