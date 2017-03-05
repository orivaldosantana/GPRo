# Início do meu estudo sobre  V-rep

_Por Samuel Cavalcanti_

Primeiro passo que fiz foi baixar/instalar o [V-rep](http://www.coppeliarobotics.com/downloads.html)

Após ter baixando, assistir as seguintes vídeos aulas:

*OBS*: as aulas 1 mostra como usar o v-rep no linux mas caso tenha entendido aqui vai  
extraia o arquivo .tar.gz e pelo terminal vá até a pasta e execute o arquivo V-rep.sh  
assim: ./vrep.sh


* Noções básicas do V-rep
[aula 1](https://www.youtube.com/watch?v=LGMYDb6IkPM)

* Programando em c++ no V-rep
[aula 2](https://www.youtube.com/watch?v=QQ3M55bXjQw)

## Meu primeiro desafio: papelão azul
O meu objetivo era fazer um seguidor de linha no V-rep utilizando um remote API para programa-lo em C++.

O trabalho foi divido em duas etapas:

### 1° etapa: moldar o papelão no V-rep
![Papelão](https://github.com/orivaldosantana/GPRo/blob/master/URA/How%20to%20fazer%20papel%C3%A3o%20azul/Papel%C3%A3o.png) *esse em toda a sua glória é pepelão azul*   

Para fazer um papelão azul é simples, pois a humildade está no sangue apesar dele ser azul  
*Primeiro* pegue um seguidor de linha padrão do v-rep
![seguidor de linha](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/URA/How%20to%20fazer%20papel%C3%A3o%20azul/seguidor%20de%20linha%20padr%C3%A3o.png)

Desmonte-o, pegue apenas apenas as partes importantes: rodas, a base e sensores (apenas dois sensores)

Para fazer isso você precisa ter um certo conhecimento sobre no v-rep:  

* O de separar e juntar peças:
![separar/juntar](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/URA/How%20to%20fazer%20papel%C3%A3o%20azul/juntar%20e%20separar.png) a função ungroup serve para seprar(desagrupar) e a que está mais em cima do mouse (group) serve para juntar

O Fato de que quase todas todas as peças que realmente influenciam na simulação estão *INVISÍVEIS por padrão* e para ver  
 tem que habilitar a opção: ![ver_o_nao_visto](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/URA/How%20to%20fazer%20papel%C3%A3o%20azul/ver%20o%20invisivel.png) Para ter acesso a essa opção, você precisa selecionar a peça em questão e clicar duas vezes cima  da peça ou clique na lupazinha no canto superior esquerdo e clique em _COMMON_

Pronto agora selecione o _line_tracer_ desagrupe as peças dele e comece a deletar tudo menos a base  
(Dica: a base se chama line tracer),as rodas (os dynamicJoints NADA DESSAS CADEIAS DEVERAM SER DELETADOS) e não delete também a rodinha da frente(_Force_sensor_ e sua cadeia) com tudo devidamente deletado  as coisas ficaram assim:  

![resto](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/URA/How%20to%20fazer%20papel%C3%A3o%20azul/resto.png)

OBS: deletei o sensor do meio porque eu quis. Sobre dois sensores delete o que lhe covém  
deixei também o graph porque ele deixa um rastro amarelo que eu acho legal  :)



 Se o seu ficou assim, então está quase _CLAN_!  


Agora é necessário mais um conhecimento para fazer o mito (papelao azul) é necessário criar uma peça  
do tipo cuboide, siga os passos:

Vá até a barra superior e clique em ADD >> PRIMITIVE SHAPE >> CUBOID
![papelao](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/URA/How%20to%20fazer%20papel%C3%A3o%20azul/corpo-papel%C3%A3o.png)

Depois crie um cuboide com essas dimensões:

* X = 1.5000e-01  
* Y = 1.1000e-01  
* Z = 5.5000e-02

Detalhe: esse e-01 significa 10 elevando a -1 , e-02 = 10^(-2) .....  
e o V-rep trabalha como medida padrão o metro ou seja:  
* 1.5000e-01 m é igual a 0.15 metros = 15 centímetros  
* 1.1000e-01 m é igual a 0.11 metros = 11 centímetros  
* 5.5000e-02 m é igual a 0.055 metros = 5.5 centímetros

Vai ficar assim:  
![dimensão papelão](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/URA/How%20to%20fazer%20papel%C3%A3o%20azul/tamanho%20papel%C3%A3o.png)

Depois de criado a peça, precisamos pinta-lo, pois Papelao Azul só é Azul se estiver azul

Selecione o cuboite criado e depois clique na lupazinha de novo e logo de cara vai está  
ADJUST COLOR, clique, depois vá em _Ambient/Diffuse component_ e por fim abaixe o vemelho  
e o verde para zero assim:
![cor](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/URA/How%20to%20fazer%20papel%C3%A3o%20azul/Cor.png)


Para terminarmos a primeira etapa junte tudo e modifique o tamanho x e y da base  :)  
assista essa [vídeo aula](https://www.youtube.com/watch?v=LGMYDb6IkPM) que mostra como mudar os objetos de lugar  
obs: FAÇA AS PAREDES IGUAIS A DO VÍDEO  

No caso você deve abaixar mais as rodas, colocar o sensores mais perto de papelão e abaixo dele  
no final vai ficar assim (ou proximo disso):  
![resultado1](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/URA/How%20to%20fazer%20papel%C3%A3o%20azul/resultado%201.png)

Para editar a base vai precisar de mais um conhecimento de v-rep.  

Selecione a base clique de novo na lupazinha depois em _View/modifify geometry_, ai vai aparecer a seguinte janela:  
![seguinte janela](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/URA/How%20to%20fazer%20papel%C3%A3o%20azul/seguinte%20Janela.png)


Desabilite a opção keep proportions, pois você não quer manter as proporções:  

* Coloque o X e o Y do papelão   ![sem keepar](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/URA/How%20to%20fazer%20papel%C3%A3o%20azul/sem%20keepar.png)  
 * X= 1.5000e-01  
 * Y = 1.1000e-01  

* Agora  é só AGRUPAR!
 para juntar o corpo de papelão com a base selecione as duas peças e depois vai em:  
 Edit >> Gruoping\Merging >> Group selected shapes   
 DICA: para selecionar mais de uma peça clique com o botão esquerdo do mouse precionando shift  
 Deve ficar assim:
 ![resultado2](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/URA/How%20to%20fazer%20papel%C3%A3o%20azul/resultado2.png)  


### PARABÉNS VOCÊ TERMINOU A 1° ETAPA :)    

Papelão é um seguidor de linha, logo ele precisa seguir  uma linha. Caso não saiba fazer uma linha linha [clique aqui](https://www.youtube.com/watch?v=lKHZFHUOng8)

Agora você sabe criar papelão e seguimentos de linha, só falta o código.

## Inicio da 2° etapa colocando o código seguidor de linha

você já deve ter visto esse [video](https://www.youtube.com/watch?v=QQ3M55bXjQw)  
nele  você aprende a configurar o cenario para um remoteAPI
como  vamos usar  
c++ como liguagem então >> CppRemoteAPI  
após ter configurado o cenario vamos mexer no código (main.cpp)  
basicamente vamos apagar tudo que não é necessário, vai ficar assim:  
[somenete o necessario](https://github.com/orivaldosantana/GPRo/blob/master/URA/How%20to%20fazer%20papel%C3%A3o%20azul/main.cpp)  
Dica: Ctrl + C e Ctrl + V  

Antes  de continuar vale dar uma resalva sobre o [manual-vep](http://www.coppeliarobotics.com/helpFiles/index.html)  
principalmente essa parte [aqui](http://www.coppeliarobotics.com/helpFiles/en/remoteApiFunctions.htm)  
que mostra  todas as funçoes do Remote API do  v-rep para c++  

as funções que vamos utilizar são essas:  
simxGetObjectHandle  
simxReadVisionSensor  
simxSetJointTargetVelocity  

mais um pouco de conhecimento de v-rep  
para todo script imputido  você tem que chamar a peça em questão  
indicando que é ela que você vai pegar  
OK, mais fotos:  
![mais fotos](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/URA/How%20to%20fazer%20papel%C3%A3o%20azul/maisfotos.png)  
eu sinalizei 4 peças que vamos usar: os meus dois sensores de visão e as duas juntas que no caso funciona como  
se fosse os motores  

no caso a função simxGetObjectHandle serve para isso, pegar um objeto da cena  



(OBS: existe uma serie de pre-configurações para estabelecer um remote API, MAS  
  como já pegamos os aquivos da aula 2 então não seŕa necessario :)

#### escrevendo funções no main.cpp  

tudo começa pegando as peças  
se você foi no site do v-rep e pesquisou sobre as funções que vamos usar  
algo que duvido muito que você tenha feito, você vai encontrar essas sintaxes:  

Para simGetObjectHandle
```C++
simxInt simxGetObjectHandle(simxInt clientID,const simxChar* objectName,simxInt* handle,simxInt operationMode)
```  

  Para simSetJointTargetVelocity
```C++
simxInt simxSetJointTargetVelocity(simxInt clientID,simxInt jointHandle,simxFloat targetVelocity,simxInt operationMode)
```

Para simxReadVisionSensor
```C++
simxInt simxReadVisionSensor(simxInt clientID,simxInt sensorHandle,simxUChar* detectionState,simxFloat** auxValues,simxInt** auxValuesCount,simxInt operationMode)

```  

mais ou menos é por aqui que você desiste da vida não é mesmo ? Não mais !!  
pois agora vou salvar a sua vida meu caro amigo  

Vamos lá, começando pelo começo:  
função simGetObjectHandle como relmente vai ficar:  
```C++
simxGetObjectHandle(clientID,(const simxChar*) "A",(simxInt*) & B, (simxInt) simx_opmode_oneshot_wait
```
A= a Exatamente o nome da peça no V-rep!  
B= é a varialvel que você declarou para representar a peça  
exemplo: mo meu caso o nome da minha peça que vou pegar é a LeftSensor (pontinho preto)  
e o nome que pretendo colocar é sensor_esquerdo como ficaria....  
```C++
simxGetObjectHandle(clientID,(const simxChar*) "LeftSensor",(simxInt*) &sensor_esquerdo, (simxInt) simx_opmode_oneshot_wait
```  
```C++
LEMBREM-SE DE DECLARAR A VARIAVEL QUE VAI RECERBER A PEÇA COMO TIPO  int  
 ex: int sensor_esquerdo;
```

Detalhe importante: esse simxInt clientID = clientID (PARA QUEM PEGOU OS ARQUVOS DA AULA 2!)  


Digamos que você foi um um bom garoto(a) e assim que leu isso  escreveu 4 simxGetObjectHandle  
pegando as 4 peças que vamos utilizar,então vamos prosseguir  


na aula 2 o nosso amigo cria uma ```if``` fazendo a seguinte pergunta:  
ao executar a função simxGetObjectHandle deu ruim ?  
      se sim, então escrever tal objeto não encotrado  
      se não, escrever objeto encontrado

vamos utilizar a mesma abordagem, damos um ctrl + c e ctrl + v nisso  
mudando é claro o nome para as nossas peças    


### parte final código que vai fazer o papelão seguir a linha :)
após aprender a  inicializar os motores(dynamileftjoint e dynamicrightjoint) e sensores com  
simxGetObjectHandle, temos que aprender a setar a velocidade do papelao e fazer ele identificar  
a linha  




###### como ensinar papelão a enxergar o preto da linha:  
Para fazer o papelão enxergar vamos usar a função simxReadVisionSensor  

mas antes precisamos de mais um conhecimento de v-rep, voltado a remotes APIs  


simxInt, simxChar, simxFloat são variaveis do proprio v-rep que muitas vezes precisamos declara-las  


ok, vamos continuar  ao ler a função  simxInt simxReadVisionSensor de novo:  
```C++  
simxInt simxReadVisionSensor(simxInt clientID,simxInt sensorHandle,simxUChar* detectionState,simxFloat** auxValues,simxInt** auxValuesCount,simxInt operationMode)  
```
e  se formos ao [site](http://www.coppeliarobotics.com/helpFiles/en/remoteApiFunctions.htm) e procuramos pela função ```simxReadVisionSensor``` vamos encontrar a seguinte descrição:  
![descrição](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/URA/How%20to%20fazer%20papel%C3%A3o%20azul/descri%C3%A7%C3%A3o.png)  
~~(SIM, irei faze-lo ir ao site de um jeito ou de outro....)~~

vemos que temos que declarar ao menos 3 variaveis tipo  simx são elas:  
```C++
//  é assim que declara elas  
      
simxUChar* detectionState;
simxFloat* auxValues;
simxInt* auxValuesCount;

```

se ler a descrição, vai dizer o seguinte:  
detectionState vai voltar:  
  -1, caso algo esteja errado  
  0, caso não esteja pegando nada  
  1, caso pegou algo  

auxValues volta um vetor de 15 espaços sendo os 5 primeiros espaços para o minimo de intensidade os proximos  
 5 para a maxima intensidade e os restantes 5 para as intensidade media  

 OBS: não se preocupe caso não tenha entendido como funciona pois nem eu entendi direito :)  

 ###### voltamos ao que importa o código, segundo o v-rep e seu tutorial sobre line following  


 para detectamos que aquela é a cor preta o ``` auxValues[11]``` tem que ser menor que 0.3  
 ou seja:  
 ```C++  
 if(auxValues[11]<0.3) == hora de virar  
 ```

 operationMode segundo o manual-vep e é verdade acredite  
 o recomendado é fazer duas chamas a primeira chamda usando o:  
 ```simx_opmode_streaming ```  
 e a segunda usando o: simx_opmode_buffer  


vai ficar assim:  
```C++  
 primeira chamada:  
  simxReadVisionSensor(clientID,o nome da sua variavel,NULL,NULL,NULL,simx_opmode_streaming);
  ```
  isso faz como que o sensor fique streamando  
  obs: NULL significa nulo, ou seja isso só prepara o sensor dizendo que ta ta na hora de streamar  


e a segunda chamada é a que importa ela vai ficar assim:  
```C++  
simxReadVisionSensor(clientID,sua_variavel,&detectionState,&auxValues,&auxValuesCount,simx_opmode_buffer);
    if(auxValues[11]<0.3)  

    {
      hora do show
    }  
```      


 #### ta quase clâ!!!

 agora só falta aprender a setar a velocidade do papelão :))

 vamos usar a simxSetJointTargetVelocity
 ![simxSetJointTargetVelocity](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/URA/How%20to%20fazer%20papel%C3%A3o%20azul/velocity.png)


 se olharmos pra sintaxe dela vemos que é algo bem simples, por isso vou acrescentar um algo a mais  (:

```C++  
imxSetJointTargetVelocity(simxInt clientID,simxInt jointHandle,simxFloat targetVelocity,simxInt operationMode)
```
vou traduzir uma coisa: simxInt jointHandle = a varialvel que você declarou  
no meu caso quando eu fiz meu ```simxGetObjectHandle```  
 eu colquei que meu int motor_direito = o DynamicRightJoint  
 logo o meu simxInt jointHandle é o motor_direito  


 então no meu caso o código ficou assim:  
 ```C++  
   simxSetJointTargetVelocity(clientID, motor_direito, (simxFloat) velocidade_motor_direito, simx_opmode_streaming);  
   ```
e esse ```(simxFloat) velocidade_motor_direito``` ?  

```velocidade_motor_direito``` foi uma variavel tipo float que criei  
e esse ```(simxFloat)``` é uma função serve para converter o float em simxFloat  





Agora juntando tudo escrito aqui você pode criar o papelão e executar o codigo  
#### PERA COMO EU COMPILO O CODIGO E EXECUTO NO LINUX ?!  
tem um video muito bom, vou passar o link >>[Video bom](https://www.youtube.com/watch?v=H1VnA1tVrjE)<<


### o código de Papelao para tomar como referencia [AQUI](https://github.com/orivaldosantana/GPRo/blob/master/URA/papelao_azul/CPPremoteAPI%20%20Papelao%20conhecimentos%20/main.cpp)
