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
	int motor_derecho;
	int motor_izquierdo;

	//Variable que guarda el obstaculo detectado
	simxUChar obstaculo0, obstaculo1, obstaculo2, obstaculo3, obstaculo4, obstaculo5;
	simxUChar obstaculos[6];

	//Variable para guardar el sensor de distancia
	int sensor0, sensor1, sensor2, sensor3, sensor4, sensor5;
	int sensores[6];

	vector < double > inputs (6);
	vector < double > vel_motores (2);

	float punto[3];
	float distancias[6];

	//Vector con las capas y el numero de neuronas para construir la red neuronal
	vector< int > capas = {6,15,15,15,2};
	//vector< int > capas = {6,20, 50, 70, 100,100, 100, 200, 100, 50, 10 ,2};

	//Vectores de entrenamiento: input (entrenador1) y output deseado (entrenador2)
	//Orden de los sensores: {0,1,2,3,4,5}
	vector< vector< double > > entrenador1 = {
	{0.23,1,1,1,1,1},
	{0.23,0.23,1,1,1,1},
	{0.12,0.12,1,1,1,1},
	{1,1,1,1,1,1},
	{1,1,0.12,0.12,1,1},
	{1,1,0.23,0.23,1,1},
	{1,1,1,1,0.23,0.23},
	{1,1,1,1,1,0.23},
	{1,1,1,1,0.12,0.12},
	{1,1,1,0.23,1,1},
	{1,1,0.23,1,1,1},
	{1,1,1,0.12,0.12,1}};
	//Orden de los motores: {a,b}, a-> motor derecho, b->motor izquierdo
	vector< vector< double > > entrenador2 = {
	{1,0.5},
	{1,0.5},
	{1,0},
	{1,1},
	{0,0.75},
	{0.6,1},
	{0.5,1},
	{0.5,1},
	{0,1},
	{0,1},
	{0,1},
	{0,0.5}};


	//Creamos la red
	cout << "Creando red..." << endl;
	Network red(capas); 
	cout << "Red creada" << endl; 

	//Entrenamos la red con los ejemplos
	cout << "Entrenando red..." << endl;
	red.Aprendizaje_Prop_Atras(entrenador1, entrenador2);
	cout << "Red entrenada" << endl;

	red.Mostrar_Pesos(); //Mostramos los pesos definitivos

	//Mostrar los outputs:
	for(int i = 0; i < entrenador1.size(); i++)
	{
		red.Calcular_Output(entrenador1[i]);
		red.Mostrar_Output();
	}



	int clientID=simxStart("127.0.0.1",19999,true,true,100,5); //Conectar con la simulacion
	if(clientID == -1) //Si clientID vale -1, es que no se ha conectado con exito
	{
		cout << "ERROR: No se ha podido conectar\n";
		simxFinish(clientID);
		return 0;
	}

	//Aqui guardamos los dos motores...
	int valido = simxGetObjectHandle(clientID, "motor_derecho", &motor_derecho, simx_opmode_oneshot_wait);
	int valido2 = simxGetObjectHandle(clientID, "motor_izquierdo", &motor_izquierdo, simx_opmode_oneshot_wait);

	//...y los sensores
	simxGetObjectHandle(clientID, "sensor0", &sensores[0], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "sensor1", &sensores[1], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "sensor2", &sensores[2], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "sensor3", &sensores[3], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "sensor4", &sensores[4], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "sensor5", &sensores[5], simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "sensor6", &sensores[6], simx_opmode_oneshot_wait);

	//Inicializar los sensores
	cout << "Inicializando sensores..." << endl;
	for(int i = 0; i < 6; i++)
	{
		simxReadProximitySensor(clientID,sensores[i],&obstaculos[i],NULL,NULL,NULL,simx_opmode_streaming);
	}
	cout << "Inicializados" << endl;

	
	
	

	while (simxGetConnectionId(clientID)!=-1) //Este bucle funcionara hasta que se pare la simulacion
	{
			//Se lee cada uno de los sensores, y se busca la distancia
			for(int i = 0; i < 6; i++)
			{	
				simxReadProximitySensor(clientID,sensores[i],&obstaculos[i],punto,NULL,NULL,simx_opmode_buffer); //Leer el sensor
				if(obstaculos[i] != 0)
				{
					distancias[i] = punto[2];
				}
				else
				{
					distancias[i] = 1;
				}
			}

			//Generamos el vector de inputs
			for(int i = 0; i < 6; i++)
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
			simxSetJointTargetVelocity(clientID,motor_derecho,-vel_motores[0],simx_opmode_oneshot);
			simxSetJointTargetVelocity(clientID,motor_izquierdo,-vel_motores[1],simx_opmode_oneshot);


	}

	simxFinish(clientID); //Siempre hay que parar la simulación
	cout << "Simulacion finalizada" << endl;
	return 0;
}

