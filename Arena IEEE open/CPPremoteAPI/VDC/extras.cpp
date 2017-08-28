
#include <extras.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>


using namespace std;

double Extras::pi() {
    return 3.141592653589;

}

double Extras::degree(double Degree) {
    return (Degree * 3.14159265359) / 180;
}
double Extras::radians(double radians){
    return (radians*180)/3.14159265359;
}

float Extras::ramdomValor() {
    // gerando um valor aleatório entre 0 e 1.0
    double xDiv = 10000000;
    double x = rand() % int(xDiv) + 1; // não pode ser 0
    x = x / xDiv;
    return x;

}
// Algortimos de valores aleatorios de para a coleta

void Extras::randomInLine(double &beta, double &gama) {
    float x = ramdomValor(); //valor aleatorio entre  0.0001 a 1

    // obtendo um valor entre 0 e 90 (pi/2)
    beta = x * 1.57075; // x*(pi/2)


    gama = 3.1415 - 2 * beta;

}

void Extras::randomInPlane(double &beta, double &gama) {
    double x;
    double limiteGama = 1.13446401; // 1.13446401 = 65°graus


    x = ramdomValor(); //valor aleatorio entre  0.0001 a 1
    // obtendo um valor entre 0 e 90 (pi/2)
    beta = x * 1.57079633; // x*(pi/2)

    x = ramdomValor();

    if (beta <= 0.698131701) // se beta for menor que 40° graus
        gama = x * 2.53072742; //2.53072742 = 145° graus
    else if (beta <= 0.785398163) //se  beta for menor que 45°, beta vai em 5, 40,45,50,55...
        gama = x * 1.91986218; //1.91986218 = 110° graus
    else if (beta <= 0.872664626) // beta = 50°
        gama = x * 1.74532925; // gama = 100° graus
    else if (beta <= 0.959931089) //beta =55° graus
        gama = x * 1.57079633; //gama = 90 °graus
    else if (beta <= 1.04719755) // beta = 60° graus
        gama = x * 1.30899694; // gama = 75 °graus
    else {
        for (double i = 1.13446401; i < 1.57079633; i += 0.0872664626) { // i vai somando 5° graus, ele está em 65° graus

            if (beta <= i) {
                gama = x*limiteGama;
                break;
            }

            limiteGama -= 0.174532925;


        }
    }



}

void Extras::ramdomInPlane2(double &beta, double &gama) {
    float x = ramdomValor(); //valor aleatorio entre  0.0001 a 1
    double gamaTemp;
    // obtendo um valor entre 0 e 90 (pi/2)
    beta = x * 1.5708; // x*(pi/2)

    x = ramdomValor();

    gama = x * 3.1415;

    gamaTemp = 3.1415 - 2 * beta;
    while (gama > gamaTemp) {
        x = ramdomValor();
        gama = x * 3.1415;
    }

}

void Extras::ramdomInPlane3(double &beta, double &gama) {
    double x;
    double antBraco = 0.14004;
    double braco = 0.14587;
    double h1, h2, h3, fi;

    x = ramdomValor(); //achando a altura da relativa beta

    beta = x * pi();

    h1 = sin(beta) * antBraco;

    x = ramdomValor();

    gama = x * pi(); // achando a altura da relativa gama

    fi = pi() - gama;

    h2 = sin(fi) * braco;



    h3 = h1 - h2;

    while (h3 < 0) {

        x = ramdomValor();

        beta = x * pi();

        h1 = sin(beta) * antBraco;


        x = ramdomValor();

        gama = x * pi();

        fi = pi() - gama;

        h2 = sin(fi) * braco;







        h3 = h1 - h2;



    }





}

void varredura(double &beta, double &gama) {

}

void Extras::ramdomInPlane4(double &beta, double &gama) {

    double x = ramdomValor();

    beta = x * degree(150);

    x = ramdomValor();

    gama = x * degree(170);



}

void Extras::ramdomInPlane5(double &alfa, double &beta, double &gama) {
    double x;

    x = ramdomValor();

    alfa = degree(90) * x;

    x = ramdomValor();

    beta = degree(180) * x;

    x = ramdomValor();

    gama = degree(180) * x;


}

// Algoritmos para gerenciamento de arquivos

void Extras::log(double beta, double gama, double relativeX, double relativeY, string filename) {
    ofstream myfile;
    string data;


    myfile.open(filename.c_str(), ios::app); // abrindo o log

    if (myfile.is_open() && myfile.good()) {
        data = to_string(beta) + "  " + to_string(gama) + "  " + to_string(relativeX) + "  " + to_string(relativeY);

        myfile << data << endl;
    }

    myfile.close();
}

void Extras::log3(std::string Data, std::string fileName) {
    ofstream myfile;

    myfile.open(fileName.c_str(), ios::app); // abrindo o log

    if (myfile.is_open() && myfile.good()) {
        myfile << Data << endl;
    }

    myfile.close();

}
void Extras::checkAndAddTypeCSV(std::string fileName){
    string typeFile = ".csv";
    for(int i=0; i< typeFile.size(); i++){
        if(typeFile[i] != fileName[fileName.size() -4 +i]){
           
            fileName+=typeFile;
            break;
        }
    }
 
}

void Extras::logCsv(std::string data, std::string filename, std::string header) {
    ofstream myfile;
    Extras::checkAndAddTypeCSV(filename.c_str());

    while (true) {
        myfile.open(filename.c_str(), ios::in);
        if (myfile.is_open() && myfile.good()) {
            myfile.close();
            myfile.open(filename.c_str(), ios::app);
            myfile << data <<endl;
            return;
        } else {
            myfile.open(filename.c_str(), ios::app);
            if (myfile.is_open() && myfile.good()) {
                myfile << header << endl;
                myfile.close();
            }

        }

    }


}

void Extras::logCsv_old(double beta, double gama, double relativeX, double relativeY) {
    ofstream myfile;
    string data;


    myfile.open("teste.csv", ios::in);
    if (myfile.is_open() && myfile.good()) {
        myfile.close();
        myfile.open("teste.csv", ios::app);
        data = to_string(beta) + ", " + to_string(gama) + ", " + to_string(relativeX) + ", " + to_string(relativeY);
        myfile << data << endl;
    } else {
        myfile.open("teste.csv", ios::app);
        if (myfile.is_open() && myfile.good()) {
            data = "beta, gamma, rx, ry";
            myfile << data << endl;
        }



    }



}

void Extras::log2(double alfa, double beta, double gama, double relativeXCam1, double relativeYCam1, double relativeXCam2, double relativeYCam2) {
    ofstream myfile;
    string data;

    myfile.open("amostrasArm3D", ios::app); // abrindo o log

    if (myfile.is_open() && myfile.good()) {
        data = to_string(alfa) + "  " + to_string(beta) + "  " + to_string(gama) + "  " + to_string(relativeXCam1) + "  " + to_string(relativeYCam1) + "  " + to_string(relativeXCam2) + " " + to_string(relativeYCam2);
        ;

        myfile << data << endl;
    }

    myfile.close();
}

void Extras::comecarDaOndeParou(string &filename, int &nColeta, int &nSamples, bool debug) {

    string aux;

    ifstream myfile;


    myfile.open(filename.c_str(), ios::in);

    while (myfile.is_open() && myfile.good()) {

        myfile.close();
        nColeta++;
        filename = "coletaEmLinha_" + to_string(nColeta);
        myfile.open(filename.c_str(), ios::in);

    }
    nColeta--;
    filename = "coletaEmLinha_" + to_string(nColeta);
    if (debug)
        cout << "filename: " << filename << endl;



    myfile.open(filename.c_str(), ios::in);

    while (getline(myfile, aux)) {
        nSamples--;
    }

    if (debug)
        cout << "nSamples: " << nSamples << endl;



    /*
    #include <dirent.h>
    
    if( opendir("teste1") )
  cout << "ok" <<endl;

  else
    system("mkdir \"teste1\"");


     */

}

