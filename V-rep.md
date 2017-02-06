# Inicio do meu estudo sobre  V-rep
Primeira  que fiz foi  baixar/instalar o  [V-rep](http://www.coppeliarobotics.com/downloads.html)

após ter baixando, assistir as seguintes vídeos aulas

*OBS*: as aulas 1 mostra como usar o v-rep no linux mas caso tenha entendido aqui vai  
extraia o arquivo .tar.gz e pelo terminal vá até a pasta e execute o arquivo V-rep.sh  
assim: ./vrep.sh


###### noções básicas do V-rep
[aula 1](https://www.youtube.com/watch?v=LGMYDb6IkPM)

###### programando em c++ no V-rep
[aula 2](https://www.youtube.com/watch?v=QQ3M55bXjQw)

## meu primeiro desafio: papelão azul
O meu objetivo era fazer um seguidor de linha no V-rep utilizando um remote API  
para programa-lo em C++

 o trabalho foi divido em duas etapas:

 #### 1° etapa: moldar o papelão no V-rep
![papelão](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/URA/papelao_azul/Papel%C3%A3o.png)      *esse em tuda a sua glória é pepelão azul*   

para fazer um papelão azul é simples, pois a humildade está no sangue apesar dele ser azul  
*Primeiro* pegue um seguidor de linha padrão do v-rep
![seguidor de linha](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/URA/How%20to%20fazer%20papel%C3%A3o%20azul/seguidor%20de%20linha%20padr%C3%A3o.png)

desmonte ele, pegue apenas apenas as partes importantes: rodas, a base e sensores (apenas dois sensores)

para fazer isso você precisa ter um certo conhecimento de umas coisas no v-rep:  

 o de separar e juntar peças:
![separar/juntar](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/URA/How%20to%20fazer%20papel%C3%A3o%20azul/juntar%20e%20separar.png)
a função ungroup serve para seprar(desagrupar) e a que está mais em cima do mouse (group) serve para juntar

 o Fato de que quase todas  todas as peças que realmente influenciam na simulação estão *INVISÍVEIS por padrão* e para ver  
 tem que habilitar a opção:
 ![ver_o_nao_visto](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/URA/How%20to%20fazer%20papel%C3%A3o%20azul/ver%20o%20invisivel.png)





Para ter acesso a essa opção você precisa selecionar a peça em questão e clique duas vezes cima  
da peça ou clique na lupazinha no canto superior esquerdo e clique em  _COMMON_


Pronto agora selecione o line_tracer desgrupe as peças dele e comece a deletar tudo menos a base  
(Dica: a base se chama line tracer),as rodas (os dynamicJoints NADA DESSAS CADEIAS DEVERAM SER DELETADOS) e não delte também a rodinha da frente(Force_sensor e sua cadeia) com tudo devidamente deletado  as coisas ficaram assim:  

![resto](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/URA/How%20to%20fazer%20papel%C3%A3o%20azul/resto.png)

OBS: deletei o sensor do meio porque eu quis, desde que sobre dois sensores delete o que lhe covém  
deixei também o graph porque ele deixa um rastro amarelo que eu acho legal  :)



 Se o seu ficou assim então ta quase CLAN!  


agora  é necessário mais um conhecimento para fazer o mito (papelao azul) é necessário criar uma peça  
do tipo cuboide, siga os passos:

vá barra superior e clique em ADD >> PRIMITIVE SHAPE >> CUBOID
![papelao](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/URA/How%20to%20fazer%20papel%C3%A3o%20azul/corpo-papel%C3%A3o.png)

depois crie um cuboide com essas dimensões  

X = 1.5000e-01  
Y = 1.1000e-01  
Z = 5.5000e-02

Detalhe: esse e-01 significa 10 elevando a -1 , e-02 = 10^(-2) .....  
e o V-rep trabalha como medida padrão o metro ou seja:  
1.5000e-01 m é igual a 0.15 metros = 15 centímetros  
1.1000e-01 m é igual a 0.11 metros = 11 centímetros  
5.5000e-02 m é igual a 0.055 metros = 5.5 centímetros

vai ficar assim:  
![dimensão papelão](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/URA/How%20to%20fazer%20papel%C3%A3o%20azul/tamanho%20papel%C3%A3o.png)

depois de criado a peça, precisamos pinta-lo pois Papelao Azul só é Azul se estiver azul

selecione o cuboite criado e depois clique na lupazinha de novo e logo de cara vai está  
ADJUST COLOR, clique, depois vá em Ambient/diffuse component e por fim abaixe o vemelho  
e  o verde para zero assim:
![cor](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/URA/How%20to%20fazer%20papel%C3%A3o%20azul/Cor.png)


para terminarmos a primeira etapa junte tudo e modifique o tamanho x e y da base  :)  
assista essa [vídeo aula](https://www.youtube.com/watch?v=LGMYDb6IkPM) que mostra como mudar os objetos de lugar  
obs: FAÇA AS PAREDES IGUAIS A DO VÍDEO  

no caso você deve abaixar mais as rodas, colocar o sensores mais perto de papelão e abaixo dele  
no final vai ficar assim (ou proximo disso):  
![resultado1](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/URA/How%20to%20fazer%20papel%C3%A3o%20azul/resultado%201.png)

para editar a base vai precisar de mais um conhecimento de v-rep  

selecione a base clique de novo na lupazinha depois em View/modifify geometry  
ai vai aparecer a seguinte janela:  
![seguinte janela](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/URA/How%20to%20fazer%20papel%C3%A3o%20azul/seguinte%20Janela.png)


desabilite a opção keep proportions, pois você não quer manter as proporções:  

coloque o X e o Y do papelão   ![sem keepar](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/URA/How%20to%20fazer%20papel%C3%A3o%20azul/sem%20keepar.png)  
X= 1.5000e-01  
Y = 1.1000e-01  








  agora  é só AGRUPAR!

 para juntar o corpo de papelão com a base selecione as duas peças e depois vai em:  
 Edit >> Gruoping\Merging >> Group selected shapes   
 DICA: para selecionar mais de uma peça clique com o botão esquerdo do mouse precionando shift  
 Deve ficar assim:
 ![resultado2](https://raw.githubusercontent.com/orivaldosantana/GPRo/master/URA/How%20to%20fazer%20papel%C3%A3o%20azul/resultado2.png)  


 ## PARABÉNS VOCÊ TERMINOU A 1° ETAPA :)    



       papelão é um seguidor de linha, logo ele precisa seguir  uma linha  
caso não saiba fazer uma linha linha [clique aqui]()
