/*   CODIGO DE ROBOT ESQUIVA-OBSTACULOS CON V-REP

     Escrito por Nano en beneficio de los seres humanos
     www.robologs.net
     feito por: https://robologs.net/2017/01/22/tutorial-de-redes-neuronales-con-vrep-c-y-linux/
     (levemente)atualizado por Samuel Cavalcanti
     
     
*/

#include <iostream>
#include <neuron.h>

extern "C" {
    #include <extApi.h>
}

using namespace std;

int main()
{

	//Variables para guardar motores
	int motor_direito;
	int motor_esquerdo;

	//Variable que guarda el obstaculo detectado

	simxUChar obstaculos[4];

	//Variable para guardar el sensor de distancia
	int sensores[4];

	vector < double > inputs (4);
	vector < double > vel_motores (2);

	float coord[3];
	float distancias[4];

	//Vector con las capas y el numero de neuronas para construir la red neuronal
	vector< int > capas = {4,15,15,15,2};
	//vector< int > capas = {6,20, 50, 70, 100,100, 100, 200, 100, 50, 10 ,2};

	//Vectores de entrenamiento: input (entrenador1) y output deseado (entrenador2)
	//Orden de los sensores: {0,1,2,3,4,5}
	vector< vector< double > > entrenador1 = {
        {0.0442136,0.0442136,0.0442136,0.0442136}, //1
	{0.0442136,0.0442136,0.0442136,0.247024}, //2
	{0.0442136,0.0442136,0.247024,0.0442136}, //3
	{0.0442136,0.0442136,0.247024,0.247024}, //4
	{0.0442136,0.247024,0.0442136,0.0442136},//5
	{0.0442136,0.247024,0.0442136,0.247024},//6
	{0.0442136,0.247024,0.247024,0.0442136},//7
	{0.0442136,0.247024,0.247024,0.247024},//8
	{0.247024,0.0442136,0.0442136,0.0442136},//9
	{0.247024,0.0442136,0.0442136,0.247024},//10
	{0.247024,0.0442136,0.247024,0.0442136},//11
	{0.247024,0.0442136,0.247024,0.247024}, //12
	{0.247024,0.247024,0.0442136,0.0442136}, //13
	{0.247024,0.247024,0.0442136,0.247024}, //14
	{0.247024,0.247024,0.247024,0.0442136}, //15
	{0.247024,0.0442136,0.247024,0.247024}, //16
        };
	//Orden de los motores: {a,b}, a-> motor derecho, b->motor izquierdo
	vector< vector< double > > entrenador2 = {
	{0,0}, //1
	{0.5,-0.5},//2
	{-0.5,0.5},//3
	{0.75,0},//4
	{-0.5,0.5},//5
	{-0.5,0.5},//6 */
	{1,1},//7
	{1,0.5},//8
	{0.5,-0.5},//9
	{-1,-1},//10/
	{0.75,-0.25},//11
	{0.5,-0.5},//12
        {-0.25,0.75},//13
        {-0.25,0.75},//14
        {-0.25,0.75},//15*/
        {-0.25,0.75}//16
        };
          


	//Creamos la red
	cout << "Creando red..." << endl;
	Network red(capas); 
	cout << "Red creada" << endl; 

	//Entrenamos la red con los ejemplos
	cout << "Entrenando red..." << endl;
	red.Aprendizaje_Prop_Atras(entrenador1, entrenador2);
	cout << "Red entrenada" << endl;

	//red.Mostrar_Pesos(); //Mostramos los pesos definitivos

	//Mostrar los outputs:
	for(int i = 0; i < entrenador1.size(); i++)
	{
		red.Calcular_Output(entrenador1[i]);
		red.Mostrar_Output();
	}



	int clientID=simxStart("127.0.0.1",19999,true,true,100,5); //Conectar con la simulacion
	if(clientID == -1) //Si clientID vale -1, es que no se ha conectado con exito
	{
		cout << "ERROR: Não se conectou com v-rep\n";
		simxFinish(clientID);
		return 0;
	}

	//Aqui guardamos los dos motores...
	simxGetObjectHandle(clientID, "DynamicRightJoint", &motor_direito, simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "DynamicLeftJoint", &motor_esquerdo, simx_opmode_oneshot_wait);
        
        
            
            
	//...y los sensores
	simxGetObjectHandle(clientID, "s1", &sensores[0], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "s2", &sensores[1], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "s3", &sensores[2], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "s4", &sensores[3], simx_opmode_oneshot_wait);


	//Inicializar los sensores
	cout << "Inicializando sensores..." << endl;
	for(int i = 0; i < 4; i++)
	{
		simxReadProximitySensor(clientID,sensores[i],&obstaculos[i],NULL,NULL,NULL,simx_opmode_streaming);
	}
	cout << "Inicializados" << endl;

	
	
	

	while (simxGetConnectionId(clientID)!=-1) //Este bucle funcionara hasta que se pare la simulacion
	{
			//Se lee cada uno de los sensores, y se busca la distancia
			for(int i = 0; i < 4; i++)
			{	
				simxReadProximitySensor(clientID,sensores[i],&obstaculos[i],coord,NULL,NULL,simx_opmode_buffer); //Leer el sensor
				if(obstaculos[i] != 0)
				{
					distancias[i] = coord[2];
                                     //   cout <<"distancia " <<  distancias[0] << endl;
				}
				else
				{
					distancias[i] = 1;
				}
			}

			//Generamos el vector de inputs
			for(int i = 0; i < 4; i++)
			{
				inputs[i] = distancias[i];
			}
			
			vel_motores = red.Calcular_Output(inputs); //Calcular el output de la red neuronal

			//El output de las redes neuronales es un valor entre 0 y 1. Queremos mapearlo para que vaya de
			// -2 (motores giran hacia atras) a 2 (motores giran hacia delante).
			vel_motores[0] -= 0.5;
			vel_motores[1] -= 0.5;
			vel_motores[0] *= 4;
			vel_motores[1] *= 4;
                       
                        cout << vel_motores[0] << " " << vel_motores[1] << endl;
			simxSetJointTargetVelocity(clientID,motor_direito,-vel_motores[0],simx_opmode_oneshot);
			simxSetJointTargetVelocity(clientID,motor_esquerdo,-vel_motores[1],simx_opmode_oneshot);
                        
                        extApi_sleepMs(100);

	}

	simxFinish(clientID); //Siempre hay que parar la simulación
	cout << "simulação finalizada" << endl;
	return 0;
}

