#  Esse tutoral ensinar como monitorar a posição X e Y de uma cor com um vision sensor no V-rep

 Antes de começar, vale citar que esse tutorial é derivado de um vídeo onde ensina muito mais  
 do que eu irei ensinar  

 # [Vídeo](https://www.youtube.com/watch?v=kOjQRYmeX_o)  

 Ok, primeiro de tudo você precisa ter o V-rep instalado  
 [V-rep](http://www.coppeliarobotics.com/downloads.html)  

 Seria interessante você ter lido o [tutorial do papelão](https://github.com/orivaldosantana/GPRo/blob/master/V-rep.md), pois eu irei partir do principio que você leu e aprendeu `tudo` que fiz lá  

Ok vamos comeaçar, primeiro crie uma nova cena no V-rep:  

   File>>new scene  

Depois configure a cena para receber um remote API, pois vou ensinar também como pegar os dados em `C++`  

# Esse tutorial será divido em duas etapas  


### 1° etapa vou ensinar a montar e configurar a camera  no V-rep  
### 2° etapa vou ensinar como programar em lua e C++ para receber as posições x, y do objeto  


## 1° etapa:  

 * Adicionar o vision sensor  
 * Adicionar um objeto
 * Configurar o objeto
 * Configurar o vision sensor


### adicionando  um vision sensor:  
Cliquei  com o botão direito no mouse vá em `àdd`>>`Vision sensor`>>` Perspective type `  
![foto 1](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/RoboticArm/Tutorial/foto%201.png)
)


Existem dois tipos de vision Sensor o `Orthographic projection-type` e `Perspective projection-type` a diferença é bem simples,  
 um faz uma projeção em forma de retangulo e outro faz em forma de trapezio.  
 Como queremos uma projeção em forma de trapezio, escolhemos o Perspective type   

 Agora  clique de de novo no botão direito e vá em `add`>>`Floating view`  
 ![foto 2](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/RoboticArm/Tutorial/foto%202.png)


Vai aparecer um quadrado no canto da tela, selecione seu `vision sensor` e clique com o botão direito do mouse dentro do `Floating view`>>`view`>>`asoociate view with selected vision sensor`  
![foto 3](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/RoboticArm/Tutorial/foto%203.png)  

Ajuste o vision sensor suba ele, coloque ele mais ao lado da cena, faça da forma que lhe  covém...  
# adicioando um objeto


Adicione o objeto a ser visto, pode ser um cubo, esfera, cilindro,  de novo escolha o que  quiser, mas eu escolhi adicionar uma esfera e ajustei a  
sua cor como sendo vermelho. [(caso, não saiba fazer isso dê uma procurada no meu outro Tutorial clicando aqui)](https://github.com/orivaldosantana/GPRo/blob/master/V-rep.md)  

Se você da play na cena vera que a camera não esta detectando o objeto Adicionado  
![foto 4](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/RoboticArm/Tutorial/foto%204.png)

Isso é porque pro padrão nenhum objeto criado ou exportado para o v-rep está habilitado como renderizável  
# Configurando o objeto
para que ele seja renderizável, terá que selecionar o objeto, clicar na lupazinha ir na aba `Common`, na parte `Object special properties`,  
 marque: `RENDERABLE`

 ![foto 5](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/RoboticArm/Tutorial/foto%205.png)

Dica: muitas vezes é interessante que o tal objeto nao caia, ou seja que ela não tenha dinamica a ser simulada.  
Para desabilitar a dinamica de um objeto vá em `shape`>> `Show dynamic properties dialog`>> desmarque `Body is respondable` e `Body is dynamic`  
![foto 6](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/RoboticArm/Tutorial/foto%206.png)  



### Agora vamos entender o basico Vision sensor  
Caso queria saber mais sobre as suas propriedades  [clique aqui](http://www.coppeliarobotics.com/helpFiles/en/visionSensorPropertiesDialog.htm)  



![foto 7](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/RoboticArm/Tutorial/foto%207.png)


* `Enable all vision sensor` = Ativa todos os vision sensors (não mexa!)  
* `Perspective mode` = É o que faz o sensor ser Perspective type, caso desabilite, transforma o sensor em Orthographic type  
* `Perspective angle` = O angulo de abertuda da projeção (caso sinta a necesside mexa)
*  `Near / far clipping plane` = Esse concerteza vai querer alterar,  no 1° quadrado você coloca a menor distancia que  
 o sensor vai poder detectar  e no 2° quadrado a maior distancia

* `Resolution X / Y` = È a resolução da camera o V-rep só suporta resoluções de 2^n e  
quando maior a resolução mais lento é a resposta do V-rep  

* `Entity to detect ` = Quais objetos essa camera poderá captar  

* `object size X - Y - Z`= Tamanho da Camera  

* `SHOW FILTER DIALOG` = mostra o dialogo dos filtros, uma das na minha opnião propriedades mais  IMPORTANTES  


Dica: sobre as resoluções:  
 ![dica](https://upload.wikimedia.org/wikipedia/commons/thumb/a/a2/Common_Video_Resolutions_2.svg/800px-Common_Video_Resolutions_2.svg.png)

Lembre-se: alta resolução = baixo tempo de resposta


### entendendo o basico do dialgo de filtros  
![foto 8](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/RoboticArm/Tutorial/foto%208.png)

Como mostra a imagem a existe um filtro chamado `Original image to work image`e outro  
chamado `Work image to output image`, o que está acontecendo:  

`Original image to work image` == Imagem Original vai para work image  

`Work image to output image` == Work image vai para imagem de saida que no caso é o nosso Floating view

Então caso quisermos trabalhar com a imagem, temos que colocar um filtro entre os dois  

Caso queria saber mais sobre o funcionamento dos filtros: [aqui](http://www.coppeliarobotics.com/helpFiles/en/visionSensorFilterComposition.htm)  


# configrando vision sensor

Para obtermos a posição x e y do objeto temos que adicionar dois filtros:   
* `Selective color on work image`  

* `Binary work image and trigger`  

A 1° seleciona a cor que vamos utilizar no meu caos vai ser a vermelha  
A 2° calcula para a gente o centro de massa da cor selecionada  

Então ficará assim:  
![foto 9](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/RoboticArm/Tutorial/foto%209.png)   



Agora vamos configurar ambos os filtros:  
clique duas vezes no `Selective color on work image`  
deve aparecer isso:  
![foto 10](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/RoboticArm/Tutorial/foto%2010.png)  

No meu caso eu escolhi o vermelho então, eu coloco:  

`Red/hue`= 1  com o  `Tolerance` = 0.1    
`Gree/saturation`= 0.5 com o `Tolerance` = 0.5  
`Blue/luminosity`= 0.5 com o `Tolerance` = 0.5  

São valores quase que abtrario, foram conseguidos a partir  de tentativa e erro  

ficará assim:  
![foto 11](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/RoboticArm/Tutorial/foto%2011.png)

No segundo filtro: `Binary work image and trigger`  
ficará assim:  
![foto 12](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/RoboticArm/Tutorial/foto%2012%20.png)  

## Parabéns você terminou a 1° etapa !!    


# 2° etapa:  

* Adicionar o script lua para pegar as posições x e y

* Adicionar um script em c++ para pegar as posições em x e y  


### Em lua eu realmente  recomendo que assistam a esse pedaço do [vídeo](https://www.youtube.com/watch?v=kOjQRYmeX_o#)  
Lá ele vai fazer exatamente a mesma coisa do que eu vou explicar agora  


## Adicionando o script lua  

Selecione o vision sensor, clique no botão direito mouse  vai em:  
`Add`>> `Associated child script`>>`Threaded`  
![foto 13](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/RoboticArm/Tutorial/foto%2013%20.png)

Após adicionar o script, vai aparecer um papelzinho ao lado do video_sensor, clique que vai aparecer isso:  
![foto 14](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/RoboticArm/Tutorial/foto%2014.png)

Em baixo de `threadFunction=function()` escreva essa linha de código:  
```LUA
out=simAuxiliaryConsoleOpen("debug",8,1)  
```  
Ficando assim:
![foto 15](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/RoboticArm/Tutorial/foto%2015.png)

Caso queria maior detalhe sobre o código  veja o [vídeo](https://youtu.be/kOjQRYmeX_o?t=1367)  


descomente o `while simGetSimulationState()`  
![foto 16](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/RoboticArm/Tutorial/foto%2016.png)
Não esqueça do `end`    


Antes do `while` chame a peça, caso não saiba do que eu estou falando veja o [outro tutorial](https://github.com/orivaldosantana/GPRo/blob/master/V-rep.md)  
Em lua chamar a peça é assim:
```LUA  
o nome que ver quer dar =simGetObjectHandle("o nome da peça")
```

No meu caso por exemplo, o nome da peça é Vision_sensor e o nome que vou dar é tekpix  
```LUA  
tekpix=simGetObjectHandle("Vision_sensor")
```
![foto 17](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/RoboticArm/Tutorial/foto%2017.png)

Depois de chamar a peça, precisamos fazer o script que será executado durante a simulação  
ou seja,  `enquanto`= `while` a simulação estiver rolando, execute isso...  



Bem a um certo tempo, aplicamos um filtro que nos voltava informações interessantes....  
![foto 18](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/RoboticArm/Tutorial/foto%2018%20.png)

Em Lua quando aplicamos um filtro temos que adicionar uma variavel adicional  
`ao contrario de C++ que é tudo jogado no primeiro pacote`  

Estou falando da função `simReadVisionSensor` falei como ela funciona no [outro tutorial](https://github.com/orivaldosantana/GPRo/blob/master/V-rep.md)   na parte de código  

Então a sintaxe fica assim:  
```LUA  
result,pack1,pack2=simReadVisionSensor(tekpix)
```
Onde pack2  é aonde vai voltar as informações do filtro, ficando assim:  
 ![foto 19](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/RoboticArm/Tutorial/foto%2019.png)


 ### Explicando  o resto do código
 Esse `simAuxiliaryConsoleOpen` abre um terminal onde futuramente a posição x e y será escrita  

 Se prestou atenção, as posições dos vetoror pack2 que peguei foi a indicada pelo proprio filtro  
 e  esse %0.2f significa que apenas 2 numeros apos o zero será mostrado, se você alterar o valor  
verá que mais numeros seram mostrados e a medita que aumenta e menos numeros seram vistos  
 caso coloque um valor menor que 2  
 `result` é uma variavel de estado que volta 3 situações:  

 *  -1 == quer dizer que tem algo de errado  
 *   0 == quer dizer que não está pegando nada  
 *   1 ==  quer dizer que pegou algo 

 ### programando em C++  

 Não irei explicar a função `simReadVisionSensor`, pois já fiz isso  no [outro tutorial](https://github.com/orivaldosantana/GPRo/blob/master/V-rep.md)  

 Parto do principio que já leram e entenderam o o primeiro tutorial, então postarei o códgio explicarei a diferença entre LUA e C++  

 #### O código:  
 ```C++

        simxReadVisionSensor(clientID,Webcam,NULL,NULL,NULL,simx_opmode_streaming);

        simxReadVisionSensor(clientID,Webcam,&state,&auxValues,&auxValuesCount,simx_opmode_buffer);
        if(state>0)
        {
          for(int i=16; i<18;i++)
          {
            cout<<"auxValues["<<i<<"]: "<<auxValues[i]<<endl; // auxValues[16] = pos X auxValues[17] =pos Y
          }

        }
        else
        cout<<"deu ruim"<<endl;
```  
Essa é a sintaxe em C++ do simReadVisionSensor:  
![sintaxe](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/URA/How%20to%20fazer%20papel%C3%A3o%20azul/descri%C3%A7%C3%A3o.png)

Ao ler direito o parametro auxValues, ele vai dizer que todo filtro adicionado vai voltar para o pack1 então dei um `cout` nos 20 primeiros espaços do vetor auxValues e achei a posição x e y do objeto

# Fim da 2° etapa e desse tutorial Parabéns ! :)   
