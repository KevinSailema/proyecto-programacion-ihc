#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h> //Para usar la funcion sleep
#include <cstdlib> //Para usar la funcion cls en windows
using namespace std;
void titulo(){
    cout<<"\n*****************************************************************"<<endl;
    cout<<"*    Bienvenido al sistema de gestion de tarjetas de credito    *"<<endl;
    cout<<"*****************************************************************"<<endl;
}

int imprimir_opciones(string tipo1, string tipo2, string tipo3, string tipo4, string tipo5, string tipo6){
    int n;
    cout << "\n1. " << tipo1 << endl;
    cout << "2. " << tipo2 << endl;
    cout << "3. " << tipo3 <<endl;
    cout << "4. " << tipo4 << endl;
    cout << "5. " << tipo5 <<endl;
    cout<<tipo6<<endl;
    cout << "Ingrese la opcion deseada: ";
    cin>>n;
    return n;
}

//Ingreso de cliente
void ingreso_cliente(){
    cout<<"********************"<<endl;
    cout<<"INGRESO DE CLIENTES"<<endl;
    cout<<"********************"<<endl;
    string nombres, apellidos;
    long long int cedula, numero_tarjeta;
    int cvc;
    ofstream archivo("CRUD.csv",ios::app);
    if(archivo.is_open()){
        cin.ignore();
        cout<<"Ingrese sus nombres completos: ";
        getline(cin, nombres);
        cout<<"Ingrese sus apellidos completos: ";
        getline(cin, apellidos);
        cout<<"Ingrese su numero de cedula: ";
        cin>>cedula;
        while(cedula<=100000000||cedula>=2500000000){
            cout<<"Número de cedula incorrecto, ingrese nuevamente: ";
            cin>>cedula;
            cout<<endl;
        }
        cout<<"Ingrese el número de la tarjeta: ";
        cin>>numero_tarjeta;
        while(numero_tarjeta<=1000000000000000||numero_tarjeta>=10000000000000000){
            cout<<"El número de tarjeta debe tener 16 dígitos, intentelo de nuevo: ";
            cin>>numero_tarjeta;
        }
        cout<<"Ingrese el CVC de la tarjeta: ";
        cin>>cvc;
        while (cvc>=1000 || cvc<100){
            cout<<"El CVC debe tener al menos tres dígitos, intentelo de nuevo: ";
            cin>>cvc;
        }
        archivo<<nombres<<";"<<apellidos<<";"<<cedula<<";"<<numero_tarjeta<<";"<<cvc<<endl;
        archivo.close();
    }else{
        cout << "Error al abrir el archivo" << endl;
        exit(1);
    }
}

//Crear archivo
void crear_archivo(){
    ofstream archivo("CRUD.csv",ios::app);
    if(archivo.is_open()){
        cout << "\nArchivo creado correctamente" << endl;
        archivo.close();
    }else{
        cout << "Error al crear el archivo" << endl;
        exit(1);
  }
}

//Consultar cliente
void consultar_cliente(){
    cout<<"******************"<<endl;
    cout<<"CONSULTAR CLIENTE"<<endl;
    cout<<"******************"<<endl;
    long long int cedula, tarjeta_adicional = 0;
    int cvc_adicional, n;
    cout<<"Ingrese la cedula del cliente a consultar: ";
    cin>>cedula;
    while(cedula<=100000000||cedula>=2500000000){
        cout<<"El número de cedula debe tener diez dígitos, ingrese nuevamente: ";
        cin>>cedula;
        cout<<endl;
    }
    ifstream archivo("CRUD.csv");
    ofstream archivo_temp("temp.csv"); // abrir archivo temporal
    if(archivo.is_open() && archivo_temp.is_open()){
        string linea;
        bool encontrado = false;
        while(getline(archivo, linea)){
            size_t start = 0, end = 0;
            string partes[7];
            int i = 0;
            while ((end = linea.find(';', start)) != string::npos) {
                partes[i++] = linea.substr(start, end - start);
                start = end + 1;
            }
            partes[i] = linea.substr(start);

            if(partes[2] == to_string(cedula)){
                encontrado = true;
                cout<<"Cliente encontrado: ";
                cout<<partes[0]<<" "<<partes[1]<<" / "<<partes[2]<<endl;
                do{
                    cout<<endl;
                    cout<<"*************************"<<endl;
                    cout<<"Visualizacion de tarjetas"<<endl;
                    cout<<"*************************"<<endl;
                    cout<<"1. Ver tarjetas registradas"<<endl;
                    cout<<"2. Ingresar tarjeta adicional"<<endl;
                    cout<<"3. Ingresar pagos"<<endl;
                    cout<<"Ingrese la opcion deseada: ";
                    cin>>n;
                    switch(n){
                        case 1:
                            cout<<"Tarjetas registradas: "<<endl;
                            cout<<"Numero de tarjeta principal: "<<partes[3]<<endl;
                            if(partes[5] != ""){
                                cout<<"Numero de tarjeta adicional: "<<partes[5]<<endl;
                            }
                            break;
                        case 2:
                            cout<<"Ingrese el numero de la tarjeta adicional: ";
                            cin>>tarjeta_adicional;
                            while(tarjeta_adicional<=1000000000000000||tarjeta_adicional>=10000000000000000){
                                cout<<"El número de tarjeta debe tener 16 dígitos, intentelo de nuevo: ";
                                cin>>tarjeta_adicional;
                            }
                            cout<<"Ingrese el CVC de la tarjeta adicional: ";
                            cin>>cvc_adicional;    
                            while(cvc_adicional>=1000 || cvc_adicional<100){
                                cout<<"El CVC debe tener al menos tres dígitos, intentelo de nuevo: ";
                                cin>>cvc_adicional;
                            }
                            partes[5] = to_string(tarjeta_adicional);
                            partes[6] = to_string(cvc_adicional);
                            cout<<"Tarjeta adicional ingresada con exito"<<endl;
                            break;
                        case 3:
                            break;
                        default:
                            cout<<"Opcion no valida"<<endl;
                            break;
                    }
                } while (n!=3);

                archivo_temp << partes[0] << ";" << partes[1] << ";" << partes[2] << ";" << partes[3] << ";" << partes[4] << ";" << partes[5] << ";" << partes[6] << endl;
            } else {
                archivo_temp<<linea<<endl;
            }
        }
        if(!encontrado){ // Si no encuentra la cedula, imprime que no se encontró
            cout<<"Cliente no encontrado, regresando al menu principal";
            for(int i=0;i<4;i++){
                sleep(1.5);
                cout<<".";
            }
            system("cls");
        }
        archivo.close(); 
        archivo_temp.close(); 
        remove("CRUD.csv");
        rename("temp.csv","CRUD.csv");
    }else{
        cout << "Error al abrir el archivo" << endl;
        exit(1);
    }
}

//Tarjeta a usar
void usar_tarjeta(){
    cout<<"\n**************"<<endl;
    cout<<"USAR TARJETA"<<endl;
    cout<<"**************"<<endl;
    int cvc;
    bool encontrado = false;
    cout<<"Ingrese el CVC de la tarjeta a usar: ";
    cin>>cvc;
    while(cvc>=1000 || cvc<100){
        cout<<"El CVC debe tener al menos tres dígitos, intentelo de nuevo: ";
        cin>>cvc;
    }
    ifstream archivo("CRUD.csv");
    ofstream archivo_temp("temp.csv"); // abrir archivo temporal
    if(archivo.is_open() && archivo_temp.is_open()){
        string linea;
        while(getline(archivo, linea)){
            size_t start = 0, end = 0;
            string partes[9];
            int i = 0;
            while ((end = linea.find(';', start)) != string::npos) {
                partes[i++] = linea.substr(start, end - start);
                start = end + 1;
            }
            partes[i] = linea.substr(start);
            if(partes[4] == to_string(cvc) || partes[6] == to_string(cvc)){
                encontrado = true;
                cout<<"Tarjeta encontrada: ";
                if(partes[4] == to_string(cvc)){
                    cout<<partes[3]<< " (Limite de credito: $2000)"<<endl;
                    cout<<"Fecha de corte: 15 de cada mes"<<endl;
                    cout<<"Fecha de pago: 30 de cada mes"<<endl;
                    cout<<"Beneficios: "<<endl;
                    cout<<"- 10% de descuento en Osaka, Casa Res, Bocatto da Fiorentino, Friday's"<<endl;
                    cout<<"- 15% de descuento en tu proximo vuelo con LATAM Airlines"<<endl;
                    cout<<"- 20% de descuento en consultas medicas en METRORED"<<endl;
                    int n1;
                    cout<<"\nEscoja el tipo de consumo: "<<endl;
                    cout<<"1. Consumo en restaurantes"<<endl;
                    cout<<"2. Consumo en vuelos"<<endl;
                    cout<<"3. Consumo en consultas medicas"<<endl;
                    cin>>n1;
                    switch (n1)
                    {
                    case 1:

                        break;
                    case 2:

                        break;

                    case 3:

                        break;
                    default:
                        break;
                    }
                }else{
                    cout<<partes[5]<< " (Limite de credito: $2000)"<<endl;
                    cout<<"Fecha de corte: 10 de cada mes"<<endl;
                    cout<<"Fecha de pago: 25 de cada mes"<<endl;
                    cout<<"Beneficios: "<<endl;
                    cout<<"- 10% de descuento en Chez Jerome, Fogo de Chao, Carmine"<<endl;
                    cout<<"- 15% de descuento en tu proximo vuelo con AVIANCA"<<endl;
                    cout<<"- 20% de descuento en consultas medicas en SIME USFQ"<<endl;
                }
            }
        }
    }
    if(!encontrado){
        cout<<"Tarjeta no encontrada, regresando al menu principal";
        for(int i=0;i<4;i++){
            sleep(1.5);
            cout<<".";
        }
        system("cls");
    }
}

//Actualizar Datos
void actualizar_datos(string campo, int campo_num){
    bool encontrado = false;
    string linea, reemplazar;
    long long int cedula;
    cout << "Ingrese la cedula del cliente del cual se quieren actualizar los datos: ";
    cin >> cedula;
    while (cedula <= 100000000 || cedula >= 2500000000){
        cout << "El número de cedula debe tener diez dígitos, ingrese nuevamente: ";
        cin >> cedula;
        cout << endl;
    }

    ifstream archivo("CRUD.csv");
    ofstream archivo_temp("temp.csv");

    if (archivo.is_open() && archivo_temp.is_open()){
        while (getline(archivo, linea)){
            size_t pos = linea.find(to_string(cedula));
            if (pos != string::npos){
            cin.ignore();                
            cout << "Ingrese " << campo << " a actualizar: ";
            getline(cin, reemplazar);
            encontrado = true;
            string partes[5];
            size_t start = 0, end = 0;
            for (int i = 0; i < 5; i++) {
                end = linea.find(';', start);
                if (end == string::npos) end = linea.length();
                partes[i] = linea.substr(start, end - start);
                start = end + 1;
               }
                partes[campo_num] = reemplazar;
                linea = partes[0] + ";" + partes[1] + ";" + partes[2] + ";" + partes[3] + ";" + partes[4];
          }
          archivo_temp << linea << endl;
        }
        if (encontrado){
            cout << "El dato ha sido actualizado con exito" << endl;
        } else {
            cout << "No se encontro el cliente" << endl;
        }
    } else {
        cout << "Error al abrir el archivo" << endl;
        exit(1);
    }
    archivo.close();
    archivo_temp.close();
    remove("CRUD.csv");
    rename("temp.csv", "CRUD.csv");
}

// Switch Actualizar Datos
void switch_actualizar_datos(){
    cout << "**************************" << endl;
    cout << "INGRESO A ACTUALIZAR DATOS" << endl;
    cout << "**************************" << endl;
    cout << "Seleccione el dato que desea actualizar: " << endl;
    int n;
    n = imprimir_opciones("Actualizar nombres", "Actualizar apellidos", "Actualizar cedula", "Actualizar numero de tarjeta", "Actualizar CVC","6. Salir al menu principal");
    do{
        switch (n){
        case 1:
            actualizar_datos("el nombre", 0);
            system("cls");
            break;
        case 2:
            actualizar_datos("los apellidos", 1);
            system("cls");
            break;
        case 3:
            actualizar_datos("la cedula", 2);
            system("cls");
            break;
        case 4:
            actualizar_datos("el numero de tarjeta", 3);
            system("cls");
            break;
        case 5:
            actualizar_datos("el CVC (codigo de seguridad)", 4);
            system("cls");
            break;
        case 6:
            cout << "Regresando al menu principal";
            for(int i=0;i<4;i++){
                sleep(1.5);
                cout<<".";
            }
            system("cls");
            break;
        default:
            cout << "Opcion no valida" << endl;
            break;
    }
    }while(n!=6);
}

//Eliminar Datos
void eliminar_cliente(){
    cout<<"**************************"<<endl;
    cout<<"INGRESO A ELIMINAR CLIENTE"<<endl;
    cout<<"**************************"<<endl;
    long long int cedula;
    cout<<"Ingrese la cedula del cliente a eliminar: ";
    cout<<"(Se borraran todos los datos adjuntos al cliente)"<<endl;
    cin>>cedula;
    while(cedula<=100000000||cedula>=2500000000){
        cout<<"El número de cedula debe tener diez dígitos, ingrese nuevamente: ";
        cin>>cedula;
        cout<<endl;
    }
    ifstream archivo("CRUD.csv");
    ofstream archivo_temp("temp.csv");

    if(archivo.is_open() && archivo_temp.is_open()){
        string linea;
        while(getline(archivo, linea)){
            if(linea.find(to_string(cedula))==string::npos){ //si NO se encuentra la cedula en el archivo original esta NO se copia al archivo temporal que pasa a ser el original
                archivo_temp<<linea<<endl;
            }
        }
        archivo.close();
        archivo_temp.close();
        remove("CRUD.csv");
        rename("temp.csv","CRUD.csv");
        cout<<"Datos del cliente eliminados con exito "<<endl;
        cout << "Regresando al menu principal";
            for(int i=0;i<4;i++){
                sleep(1.5);
                cout<<".";
            }
            system("cls");
    }else{
        cout<<"Error al abrir los archivos"<<endl;
        exit(1);
    }
}

int main(){
    int n;
    crear_archivo();
    do
    {
        titulo();
        n = imprimir_opciones("Ingresar cliente nuevo", "Consultar cliente", "Actualizar cliente", "Eliminar cliente", "Salir del programa","");
        switch (n)
        {
        case 1: //ingreso de clientes
            system("cls");
            ingreso_cliente();
            break;
        case 2:
            system("cls");
            consultar_cliente();
            usar_tarjeta();
            break;
        case 3:
            system("cls");
            switch_actualizar_datos();
            break;
        case 4:
            system("cls");
            eliminar_cliente();
            break;
        case 5:
            system("cls");
            cout<<"Gracias por usar el programa"<<endl;
            exit(1);
            break;
        default:
            cout<<"Opcion no valida"<<endl;
            break;
        }
    } while (n <= 4 || n >= 1);
    return 0;
}