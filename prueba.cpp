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
    int cvc, limite_consumo=2000;
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
        string tarjeta_adicional = "";
        string cvc_adicional = "";
        string limite_adicional = "";
        string consumo_temp_principal = "0";
        string consumo_temp_adicional = "0";
        string deuda_total_principal = "0";
        string deuda_total_adicional = "0";
        archivo << nombres << ";" << apellidos << ";" << cedula << ";" 
                << numero_tarjeta << ";" << cvc << ";" << limite_consumo << ";"
                << tarjeta_adicional << ";" << cvc_adicional << ";" << limite_adicional << ";"
                << consumo_temp_principal << ";" << consumo_temp_adicional << ";"
                << deuda_total_principal << ";" << deuda_total_adicional << endl;
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
        cout << "\nArchivo abierto correctamente" << endl;
        archivo.close();
    }else{
        cout << "Error al crear el archivo" << endl;
        exit(1);
  }
}


//Cálculo de consumos
int calcular_consumos(int descuento, int limite_descuento, int limite_credito){
    int consumo;
    int meses;
    cout<<"Ingrese el monto del consumo: ";
    cin>>consumo;
    while(consumo<=0){
        cout<<"El monto del consumo debe ser mayor a 0, intentelo de nuevo: ";
        cin>>consumo;
    }
    if(consumo >= limite_descuento && consumo < 2000){
        consumo = consumo-consumo*(descuento);
        cout<<"A cuantos meses desea diferir? (1-3): ";
        cin>>meses;
        while(meses<1 || meses>3){
            cout<<"El número de meses debe ser entre 1 y 3, intentelo de nuevo: ";
            cin>>meses;
        }
        consumo = consumo/meses;
        cout<<"El monto a pagar por mes es de: $"<<consumo<<" (diferido a "<<meses<< " mes/es)"<<endl;
    }else{
        if (consumo >= limite_credito){
            cout<<"El monto excede el limite de credito ("<<limite_credito<<"), intentelo de nuevo"<<endl;
        }else{
            cout<<"No se aplica descuento"<<endl;
            cout<<"El monto minimo para aplicar descuento es de: $"<<limite_descuento<<endl;
            cout<<"El monto a pagar es de: $"<<consumo<<endl;
        }
    }
    return consumo;
}

//Tarjeta a usar
void usar_tarjeta() {
    cout << "\n**************" << endl;
    cout << "USAR TARJETA" << endl;
    cout << "**************" << endl;
    int cvc, consumo, consumo_temp = 0, resp;
    bool encontrado = false;
    cout << "Ingrese el CVC de la tarjeta a usar: ";
    cin >> cvc;
    while (cvc >= 1000 || cvc < 100) {
        cout << "El CVC debe tener al menos tres dígitos, intentelo de nuevo: ";
        cin >> cvc;
    }
    ifstream archivo("CRUD.csv");
    ofstream archivo_temp("temp.csv"); // abrir archivo temporal
    if (archivo.is_open() && archivo_temp.is_open()) {
        string linea;
        while (getline(archivo, linea)) {
            size_t start = 0, end = 0;
            string partes[13];
            int i = 0;
            while ((end = linea.find(';', start)) != string::npos && i < 13) {
                partes[i++] = linea.substr(start, end - start);
                start = end + 1;
            }
            if (i < 13) {
                partes[i] = linea.substr(start);
                while (++i < 13) {
                    partes[i] = "";
                }
            }
            if (partes[4] == to_string(cvc) || partes[7] == to_string(cvc)) {
                encontrado = true;
                cout << "Tarjeta encontrada: ";
                if (partes[4] == to_string(cvc)) {
                    cout << partes[3] << " (Limite de credito: $" << stoi(partes[5]) << ")" << endl; // pasar de un string a un entero
                    cout << "Fecha de corte: 15 de cada mes" << endl;
                    cout << "Fecha de pago: 30 de cada mes" << endl;
                    cout << "Beneficios: " << endl;
                    cout << "- 10% de descuento en Osaka, Casa Res, Bocatto da Fiorentino, Friday's" << endl;
                    cout << "- 15% de descuento en tu proximo vuelo con LATAM Airlines" << endl;
                    cout << "- 20% de descuento en consultas medicas en METRORED" << endl;
                    int n1;
                    do {
                        cout << "\nEscoja el tipo de consumo: " << endl;
                        cout << "1. Consumo en restaurantes" << endl;
                        cout << "2. Consumo en vuelos" << endl;
                        cout << "3. Consumo en consultas medicas" << endl;
                        cout << "4. Salir al menu principal" << endl;
                        cin >> n1;
                        switch (n1) {
                            case 1:
                                consumo = calcular_consumos(0.1, 50, stoi(partes[5]));
                                consumo_temp += consumo;
                                break;
                            case 2:
                                consumo = calcular_consumos(0.15, 500, stoi(partes[5]));
                                consumo_temp += consumo;
                                break;
                            case 3:
                                consumo = calcular_consumos(0.2, 40, stoi(partes[5]));
                                consumo_temp += consumo;
                                break;
                            case 4:
                                cout << "Regresando al menu principal";
                                for (int i = 0; i < 4; i++) {
                                    sleep(1.5);
                                    cout << ".";
                                }
                                system("cls");
                                break;
                            default:
                                cout << "Opcion no valida" << endl;
                                break;
                        }
                        cout << "Desea registrar otro consumo? (1. Si / 2. No): ";
                        cin >> resp;
                        while (resp != 1 && resp != 2) {
                            cout << "Opcion no valida, intentelo de nuevo: ";
                            cin >> resp;
                        }
                    } while (n1 != 4 && resp == 1);
                    if (consumo_temp > 0) {
                        int consumo_actual = stoi(partes[9]) + consumo_temp;
                        int deuda_actual = stoi(partes[11]) + consumo_temp;
                        int limite_actual = stoi(partes[5]) - consumo_temp;

                        partes[9] = to_string(consumo_actual);
                        partes[11] = to_string(deuda_actual);
                        partes[5] = to_string(limite_actual);
                    }
                } else {
                    cout << partes[6] << " (Limite de credito: $" << stoi(partes[8]) << ")" << endl;
                    cout << "Fecha de corte: 10 de cada mes" << endl;
                    cout << "Fecha de pago: 25 de cada mes" << endl;
                    cout << "Beneficios: " << endl;
                    cout << "- 10% de descuento en Chez Jerome, Fogo de Chao, Carmine" << endl;
                    cout << "- 15% de descuento en tu proximo vuelo con AVIANCA" << endl;
                    cout << "- 20% de descuento en consultas medicas en SIME USFQ" << endl;
                    int n2;
                    do {
                        cout << "\nEscoja el tipo de consumo: " << endl;
                        cout << "1. Consumo en restaurantes" << endl;
                        cout << "2. Consumo en vuelos" << endl;
                        cout << "3. Consumo en consultas medicas" << endl;
                        cout << "4. Salir al menu principal" << endl;
                        cin >> n2;
                        switch (n2) {
                            case 1:
                                consumo = calcular_consumos(0.1, 50, stoi(partes[8]));
                                consumo_temp += consumo;
                                break;
                            case 2:
                                consumo = calcular_consumos(0.15, 500, stoi(partes[8]));
                                consumo_temp += consumo;
                                break;
                            case 3:
                                consumo = calcular_consumos(0.2, 40, stoi(partes[8]));
                                consumo_temp += consumo;
                                break;
                            case 4:
                                cout << "Regresando al menu principal";
                                for (int i = 0; i < 4; i++) {
                                    sleep(1.5);
                                    cout << ".";
                                }
                                system("cls");
                                break;
                            default:
                                cout << "Opcion no valida" << endl;
                                break;
                        }
                        cout << "Desea registrar otro consumo? (1. Si / 2. No): ";
                        cin >> resp;
                        while (resp != 1 && resp != 2) {
                            cout << "Opcion no valida, intentelo de nuevo: ";
                            cin >> resp;
                        }
                    } while (n2 != 4 && resp == 1);
                    if (consumo_temp > 0) {
                        int consumo_actual = stoi(partes[10]) + consumo_temp;
                        int deuda_actual = stoi(partes[12]) + consumo_temp;
                        int limite_actual = stoi(partes[8]) - consumo_temp;

                        partes[10] = to_string(consumo_actual);
                        partes[12] = to_string(deuda_actual);
                        partes[8] = to_string(limite_actual);
                    }
                }
            }
            // Escribir en el archivo temporal
            archivo_temp << partes[0] << ";" << partes[1] << ";" << partes[2] << ";"
                         << partes[3] << ";" << partes[4] << ";" << partes[5] << ";"
                         << partes[6] << ";" << partes[7] << ";" << partes[8] << ";"
                         << partes[9] << ";" << partes[10] << ";" << partes[11] << ";"
                         << partes[12] << endl;
        }
        archivo.close();
        archivo_temp.close();
        remove("CRUD.csv");
        rename("temp.csv", "CRUD.csv");
    } else {
        cout << "Error al abrir el archivo" << endl;
        exit(1);
    }
    if (!encontrado) {
        cout << "Tarjeta no encontrada, regresando al menu principal";
        for (int i = 0; i < 4; i++) {
            sleep(1.5);
            cout << ".";
        }
        system("cls");
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
            string partes[13];
            int i = 0;
            while ((end = linea.find(';', start)) != string::npos && i<13) {
                partes[i++] = linea.substr(start, end - start);
                start = end + 1;
            }
            if(i<13){
                partes[i] = linea.substr(start);
                while(++i<13){
                    partes[i] = "";
                }
            }
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
                    cout<<"4. Salir al menu principal"<<endl;
                    cout<<"Ingrese la opcion deseada: ";
                    cin>>n;
                    switch(n){
                        case 1:
                            cout<<"Tarjetas registradas: "<<endl;
                            cout<<"Numero de tarjeta principal: "<<partes[3]<<endl;
                            cout<<"Deuda de tarjeta principal: $"<<partes[11]<<endl;
                            if(partes[6] != ""){
                                cout<<"Numero de tarjeta adicional: "<<partes[6]<<endl;
                                cout<<"Deuda de tarjeta adicional: $"<<partes[12]<<endl;
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
                            partes[6] = to_string(tarjeta_adicional);
                            partes[7] = to_string(cvc_adicional);
                            partes[8] = "2000";
                            cout<<"Tarjeta adicional ingresada con exito"<<endl;
                            break;
                        case 3:
                            usar_tarjeta();
                            break;
                        case 4:
                            cout<<"Regresando al menu principal";
                            for(int i=0;i<4;i++){
                                sleep(1.5);
                                cout<<".";
                            }
                            system("cls");
                            break;
                        default:
                            cout<<"Opcion no valida"<<endl;
                            break;
                    }
                } while (n!=4);
                archivo_temp<<partes[0]<<";"<<partes[1]<<";"<<partes[2]<<";"<<partes[3]<<";"<<partes[4]<<";"<<partes[5]<<";"<<partes[6]<<";"<<partes[7]<<";"<<partes[8]<<";"<<partes[9]<<";"<<partes[10]<<";"<<partes[11]<<";"<<partes[12]<<endl;
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

//Pagar deudas
void pagar_deudas(){
    cout<<"\n***********"<<endl;
    cout<<"PAGAR DEUDA"<<endl;
    cout<<"***********"<<endl;
    int cvc, opcion, pago_minimo, monto, adeudado, pagar;
    long long int cedula;
    bool encontrado = false;
    cout<<"Ingrese la cedula del cliente que va a pagar: ";
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
        while(getline(archivo, linea)){
            size_t start = 0, end = 0;
            string partes[13];
            int i = 0;
            while ((end = linea.find(';', start)) != string::npos && i<13) {
                partes[i++] = linea.substr(start, end - start);
                start = end + 1;
            }
            if(i<13){
                partes[i] = linea.substr(start);
                while(++i<13){
                    partes[i] = "";
                }
            }
            if(partes[2] == to_string(cedula)){
                encontrado = true;
                cout<<"Cedula encontrada "<<endl;
                cout<<"Tarjeta principal: "<<partes[3]<<endl;
                cout<<"Tarjeta adicional: "<<partes[6]<<endl;
                cout<<"Tutor de la tarjeta: "<<partes[0]<<" "<<partes[1]<<endl;
                cout<<"Ingrese el CVC de la tarjeta a pagar: ";
                cin>>cvc;
                if(partes[4] == to_string(cvc)){
                    cout<<"Este CVC corresponde a la tarjeta principal"<<endl;
                    do{
                    cout<<"\n*****************************************"<<endl;
                    cout<<"La deuda total es de: $"<<2000-stoi(partes[5])<<endl;
                    cout<<"*****************************************"<<endl;
                    cout<<"Como desea pagar la deuda?"<<endl;
                    cout<<"1. Pagar la totalidad"<<endl;
                    cout<<"2. Pagar el minimo"<<endl;
                    cout<<"3. Salir al menu principal"<<endl;
                    cout<<"Ingrese la opcion deseada: ";
                    cin>>opcion;
                        switch(opcion){
                        case 1:
                            cout<<"Pagando la totalidad de la deuda mensual";
                            for(int i=0;i<4;i++){
                                sleep(1.5);
                                cout<<".";
                            }
                            partes[11] = "";
                            partes[5] = "2000";
                            break;
                        case 2:
                            pago_minimo=stoi(partes[5])*0.05;
                            cout<<"El pago minimo de la deuda mensual es: $"<<pago_minimo;
                            cout<<"Ingrese el monto a pagar (Si el monto es menor al pago minimo entra a buro de credito): ";
                            cin>>monto;
                            if(monto<pago_minimo){
                                cout<<"El monto ingresado es menor al pago minimo, entra a buro de credito";
                                adeudado=stoi(partes[11])-monto;
                                pagar=adeudado+adeudado*0.025;
                                cout<<"Pagando monto ingresado"<<endl;
                                for(int i=0;i<4;i++){
                                    sleep(1.5);
                                    cout<<".";
                                }
                                cout<<"Pago realizado con exito"<<endl;
                                cout<<"Calculando nueva deuda con intereses"<<endl;
                                for(int i=0;i<4;i++){
                                    sleep(1.5);
                                    cout<<".";
                                }
                                cout<<"La nueva deuda es de: $"<<pagar<<endl;
                                cout<<"Recuerde que ha entrado a buro de credito, su historial crediticio se vera afectado"<<endl;
                                partes[11]=to_string(pagar);
                                partes[5]="2000";
                            }else{
                                cout<<"El monto ingresado es mayor o igual al pago minimo, NO entra a buro de credito";
                                adeudado=stoi(partes[11])-monto;
                                pagar=adeudado+adeudado*0.0232;
                                cout<<"Pagando monto ingresado"<<endl;
                                for(int i=0;i<4;i++){
                                    sleep(1.5);
                                    cout<<".";
                                }
                                cout<<"Pago realizado con exito"<<endl;
                                cout<<"Calculando nueva deuda con intereses"<<endl;
                                for(int i=0;i<4;i++){
                                    sleep(1.5);
                                    cout<<".";
                                }
                                cout<<"La nueva deuda es de: $"<<pagar<<endl;
                                cout<<"Recuerde pagar al banco puntualmente"<<endl;
                                partes[11]=to_string(pagar);
                                partes[5]="2000";
                            }
                            
                            break;
                        case 3:
                            cout<<"Regresando al menu principal";
                            for(int i=0;i<4;i++){
                                sleep(1.5);
                                cout<<".";
                            }
                            system("cls");
                            break;
                        default:
                            cout<<"Opcion no valida"<<endl;
                            break;
                    }
                    }while(opcion!=3);
                    
                    archivo_temp<<partes[0]<<";"<<partes[1]<<";"<<partes[2]<<";"<<partes[3]<<";"<<partes[4]<<";"<<partes[5]<<";"<<partes[6]<<";"<<partes[7]<<";"<<partes[8]<<";"<<partes[9]<<";"<<partes[10]<<";"<<partes[11]<<";"<<partes[12]<<endl;
                    system("cls");
                }else{
                    if(partes[7]==to_string(cvc)){
                        cout<<"Este CVC corresponde a la tarjeta adicional"<<endl;
                        do{
                    cout<<"\n*****************************************"<<endl;
                    cout<<"La deuda total es de: $"<<2000-stoi(partes[8])<<endl;
                    cout<<"*****************************************"<<endl;
                    cout<<"Como desea pagar la deuda?"<<endl;
                    cout<<"1. Pagar la totalidad"<<endl;
                    cout<<"2. Pagar el minimo"<<endl;
                    cout<<"3. Salir al menu principal"<<endl;
                    cout<<"Ingrese la opcion deseada: ";
                    cin>>opcion;
                        switch(opcion){
                        case 1:
                            cout<<"Pagando la totalidad de la deuda mensual";
                            for(int i=0;i<4;i++){
                                sleep(1.5);
                                cout<<".";
                            }
                            partes[12] = "";
                            partes[8] = "2000";
                            break;
                        case 2:
                            pago_minimo=stoi(partes[8])*0.05;
                            cout<<"El pago minimo de la deuda mensual es: $"<<pago_minimo;
                            cout<<"Ingrese el monto a pagar (Si el monto es menor al pago minimo entra a buro de credito): ";
                            cin>>monto;
                            if(monto<pago_minimo){
                                cout<<"El monto ingresado es menor al pago minimo, entra a buro de credito";
                                adeudado=stoi(partes[12])-monto;
                                pagar=adeudado+adeudado*0.025;
                                cout<<"Pagando monto ingresado"<<endl;
                                for(int i=0;i<4;i++){
                                    sleep(1.5);
                                    cout<<".";
                                }
                                cout<<"Pago realizado con exito"<<endl;
                                cout<<"Calculando nueva deuda con intereses"<<endl;
                                for(int i=0;i<4;i++){
                                    sleep(1.5);
                                    cout<<".";
                                }
                                cout<<"La nueva deuda es de: $"<<pagar<<endl;
                                cout<<"Recuerde que ha entrado a buro de credito, su historial crediticio se vera afectado"<<endl;
                                partes[12]=to_string(pagar);
                                partes[8]="2000";
                            }else{
                                cout<<"El monto ingresado es mayor o igual al pago minimo, NO entra a buro de credito";
                                adeudado=stoi(partes[12])-monto;
                                pagar=adeudado+adeudado*0.0232;
                                cout<<"Pagando monto ingresado"<<endl;
                                for(int i=0;i<4;i++){
                                    sleep(1.5);
                                    cout<<".";
                                }
                                cout<<"Pago realizado con exito"<<endl;
                                cout<<"Calculando nueva deuda con intereses"<<endl;
                                for(int i=0;i<4;i++){
                                    sleep(1.5);
                                    cout<<".";
                                }
                                cout<<"La nueva deuda es de: $"<<pagar<<endl;
                                cout<<"Recuerde pagar al banco puntualmente"<<endl;
                                partes[12]=to_string(pagar);
                                partes[8]="2000";
                            }
                            break;
                        case 3:
                            cout<<"Regresando al menu principal";
                            for(int i=0;i<4;i++){
                                sleep(1.5);
                                cout<<".";
                            }
                            system("cls");
                            break;
                        default:
                            cout<<"Opcion no valida"<<endl;
                            break;
                        }
                    }while(opcion!=3);
                    archivo_temp<<partes[0]<<";"<<partes[1]<<";"<<partes[2]<<";"<<partes[3]<<";"<<partes[4]<<";"<<partes[5]<<";"<<partes[6]<<";"<<partes[7]<<";"<<partes[8]<<";"<<partes[9]<<";"<<partes[10]<<";"<<partes[11]<<";"<<partes[12]<<endl; 
                    system("cls");
                    }else{
                        cout<<"Este CVC no corresponde a ninguna tarjeta registrada"<<endl;
                        cout<<"Regresando al menu principal";
                        for(int i=0;i<4;i++){
                            sleep(1.5);
                            cout<<".";
                        }
                        system("cls");
                    }
                }
            }else{
                archivo_temp<<linea<<endl;
            }
        }
        archivo.close(); 
        archivo_temp.close(); 
        remove("CRUD.csv");
        rename("temp.csv","CRUD.csv");
    }else{
        cout << "Error al abrir el archivo" << endl;
        exit(1);
    }
    if(!encontrado){
        cout<<"Cliente no encontrado, regresando al menu principal";
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
            string partes[13];
            size_t start = 0, end = 0;
            for (int i = 0; i < 13; i++) {
                end = linea.find(';', start);
                if (end == string::npos) {
                partes[i] = linea.substr(start);
                while (++i < 13) {
                    partes[i] = "";
                }
                break;
            }
            partes[i] = linea.substr(start, end - start);
            start = end + 1;
            }
            partes[campo_num] = reemplazar;
            linea = partes[0] + ";" + partes[1] + ";" + partes[2] + ";" + partes[3] + ";" + partes[4] + ";" + partes[5] + ";" + partes[6] + ";" + partes[7] + ";" + partes[8] + ";" + partes[9] + ";" + partes[10] + ";" + partes[11] + ";" + partes[12];
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

//Menu Principal
void menu(){
    int n;
    do
    {
        titulo();
        n = imprimir_opciones("Ingresar cliente nuevo", "Consultar cliente", "Pagar deudas", "Actualizar datos", "Eliminar cliente", "6. Salir del programa");
        switch (n)
        {
        case 1: //ingreso de clientes
            system("cls");
            ingreso_cliente();
            break;
        case 2:
            system("cls");
            consultar_cliente();
            break;
        case 3:
            system("cls");
            pagar_deudas();
            break;
        case 4:
            system("cls");
            switch_actualizar_datos();
            break;
        case 5:
            system("cls");
            eliminar_cliente();
            break;
        case 6:
            system("cls");
            cout<<"Gracias por usar el programa"<<endl;
            exit(1);
            break;
        default:
            cout<<"Opcion no valida"<<endl;
            break;
        }
    } while (n!=6);
}

//Main
int main(){
    menu();
    return 0;
}