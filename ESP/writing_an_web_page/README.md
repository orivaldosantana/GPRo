

## Exibindo Leituras de um Sesnor em uma Página Web

Teste de construção de uma página web para mostrar as leituras de um sensor. O sensor utilizado é um sensor de reflectância o mesmo do teste [Sensor de Reflectância](https://github.com/orivaldosantana/GPRo/blob/master/ESP/reflectance_sensor). Neste exemplo o ESP comporta-se como servidor web e realiza conexão com uma rede WiFi, para mais informações veja [este tutorial](http://www.instructables.com/id/Quick-Start-to-Nodemcu-ESP8266-on-Arduino-IDE/).

O objetivo deste exemplo é realizar algumas leituras no sensor de reflectância e registrar o tempo que estas leituras ocorreram. Toda vez que uma leitura ocorre é verificado se existe uma transição entre os estados do sensor desativado e ativado. Uma nova leitura é gravada apenas se esta condição for verdadeira. Assim, se o sensor estiver ativo por algum tempo novos registros de tempo não serão gravados desnecessariamente.


Código para registra os momentos que o sensor passa do estado desativado para ativado:

```c++
/////////////////////////////////////////////////////////////////////////
 // código para registra o tempo (milisegundos) que o sensor foi ativado
 if ( digitalRead(REFLECTANCE_SENSOR) == 0 ){
   currentTime = millis();
   sensorState = true;  
 }
 else {
   sensorState = false;
 }

 // só registra quando o sensor passa do estado desativado para ativado  
 if ( ! previusSensorState && sensorState ) {
    // não permite gravar em posições maiores que o tamanho do vetor
    if (timePos >= TAM_READS) {
       timePos = 0;
    }    
    readTimes[timePos] = currentTime;
    Serial.println("... reading the sensor ... \n Value (ms): ");
    Serial.println(currentTime);
    timePos++;  
 }
 previusSensorState = sensorState;
 /////////////////////////////////////////////////////////////////////////
```

Tela do computador exibindo a página web produzida pelo ESP e a saída da porta serial para acompanhar a execução do código:

![Captura](https://github.com/orivaldosantana/GPRo/blob/master/ESP/writing_an_web_page/captura_de_tela.png)

## Próximos Passos

* Fazer o botão '_Start experiment_' reiniciar a variável tempo inicial
* Registrar o tempo de ativação do sensor em relação ao tempo inicial
* Fazer o botão '_Update informations_' atualizar os dados apresentados na página web


## Referência

* Introdução ao ESP NodeMCU na IDE do Arduíno,  <http://www.instructables.com/id/Quick-Start-to-Nodemcu-ESP8266-on-Arduino-IDE/>.
