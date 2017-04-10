# Arduíno

## Compilando Códigos no Linux

Este tutorial foi Desenvolvimento por Rafael dos Prazeres funcionário de TI da ECT/UFRN. Ele descreve como compilar  códigos via linha de comando em sistemas operacionais linux, principalmentes os que rodam nos terminais leves dos laboratórios da ECT/UFRN.   

### Passo 1 - Gerando MakeFile

 Criar arquivo Makefile dentro do diretório do sketch contendo o conteúdo a seguir (Manual disponível em https://github.com/sudar/Arduino-Makefile):

```
#modelo da placa
BOARD_TAG = uno

# lista de bibliotecas utilizadas no sketch separadas por espaço
ARDUINO_LIBS =

#Caminho de instalação do Arduino-Makefile
include /usr/share/arduino/Arduino.mk

#Device para comunicação com a placa
ARDUINO_PORT = /dev/ttyACM0
```

### Passo 2 - Compilando

Compilar códigos:

- Executar Xterm (Open MPI) a partir do menu Aplicativos/Desenvolvimento;
- Acessar diretório do sketch a partir do Xterm e executar utilitário "make";

### Passo 3 - *Upload*

Fazer upload do programa:

*  Acessar diretório do sketch a partir do Xterm e executar "make upload".

## Observações

* CuteCom está disponível no menu Aplicativos/Desenvolvimento.


## Referências

* A Makefile for Arduino Sketches,  https://github.com/sudar/Arduino-Makefile
* Compilando Sketches Arduínos,
http://hardwarefun.com/tutorials/compiling-arduino-sketches-using-makefile
* Arduíno a partir de linha de comando, http://www.mjoldfield.com/atelier/2009/02/arduino-cli.html
* Arduíno a partir de linha de comando,
http://www.themgames.net/arduino-from-the-command-line/
* Comunicação serial entre computador e Arduíno,  http://playground.arduino.cc/Interfacing/LinuxTTY
* Exemplos de uso,  https://github.com/sudar/Arduino-Makefile/tree/master/examples
