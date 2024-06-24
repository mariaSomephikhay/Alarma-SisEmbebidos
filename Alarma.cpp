#include <EEPROM.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

//------SENSOR-------//
const int sensorEntrada = A0;
const int sensorHabitacion = A1;
const int sensorCocina = A2;
//para detectar movimiento
boolean sH = false;
boolean sC = false;
boolean sE = false;

//------PIEZO--------//
const int pinPiezo = A3;
boolean piezoActivado;

//--------LED--------//
const int ledEntrada = 5;
const int ledHabitacion = 3;
const int ledCocina = 6;

//------TECLADO-------//
const byte FILAS = 4;
const byte COLS = 4;
char teclas[FILAS][COLS]=
{
	'1','2','3','A',
  	'4','5','6','B',
  	'7','8','9','C',
  	'*','0','#','D'
};
byte pinesFilas[FILAS] ={13,12,11,10};
byte pinesCols[COLS] = {9,8,7,4};
Keypad kp = Keypad(makeKeymap(teclas), pinesFilas, pinesCols, FILAS, COLS);

//--------LCD--------//
const byte MAX_CHARS = 16;
const byte MAX_ROWS = 2;
LiquidCrystal_I2C lcd(0x20, MAX_CHARS, MAX_ROWS);

//-------Variables globales------//
boolean banderaMenu = false;
boolean alarma;
int tiempoActH = 0;
int tiempoActE = 0;
int tiempoActC = 0;
int tiempoInActH = 0;
int tiempoInActE = 0;
int tiempoInActC = 0;
boolean timpoInicioH = false;
boolean timpoInicioE = false;
boolean timpoInicioC = false;

void setup() {
    Serial.begin(9600);
  
    //PINES SENSOR
    pinMode(sensorEntrada, OUTPUT);
    pinMode(sensorHabitacion, OUTPUT);
    pinMode(sensorCocina, OUTPUT);
    
    //PINES LED
    pinMode(ledCocina, OUTPUT);
    pinMode(ledHabitacion, OUTPUT);
    pinMode(ledEntrada, OUTPUT);
  
  	//PIN PIEZO
  	pinMode(pinPiezo, OUTPUT);
    
    //LCD
    lcd.init();
    lcd.backlight();
    lcd.clear();
    
    //descomentar para utilizarlo con una config predeterminada
    //cargarEepromPorDefault();
    
  	
  	cargarEepromManual();
    cargarConfiguracion();
  	
}

void loop() {
  	char tecla;

    tecla = kp.getKey();

    if(tecla != NO_KEY){
      if(tecla=='1'){
       	opcionConfig();
      }else if(tecla=='2'){
      	opcionAlarma();
        delay(2000);
      }else if(tecla=='3'){
      	mostrarLCD("Eventos");
        delay(2000);
        mostrarEventos();
      }
      	cargarConfiguracion();
        mostrarValoorEeprom();
    }else{
        leerSensorCocina();
        leerSensorEntrada();
        leerSensorHabitacion();
    }
}
void mostrarValoorEeprom(){
    char pass[5];
  	strcpy(pass, traerValorEepromPorDelimitador(';', 0));
  	char sensorHabitacion[2];
  	strcpy(sensorHabitacion, traerValorEepromPorDelimitador(';', 1));
  	char sensorCocina[2];
    strcpy(sensorCocina, traerValorEepromPorDelimitador(';', 2));
    char sensorEntrada[2];
    strcpy(sensorEntrada, traerValorEepromPorDelimitador(';', 3));
    char buffer[2];
    strcpy(buffer, traerValorEepromPorDelimitador(';', 4));
    char alarma[2];
    strcpy(alarma, traerValorEepromPorDelimitador(';', 5));
    char evento1[10];
    strcpy(evento1, traerValorEepromPorDelimitador(';', 6));
    char evento2[10];
    strcpy(evento2, traerValorEepromPorDelimitador(';', 7));
    char evento3[10];
    strcpy(evento3, traerValorEepromPorDelimitador(';', 8));
    char evento4[10];
    strcpy(evento4, traerValorEepromPorDelimitador(';', 9));
    char evento5[10];
    strcpy(evento5, traerValorEepromPorDelimitador(';', 10));
    char evento6[10];
    strcpy(evento6, traerValorEepromPorDelimitador(';', 11));
    char evento7[10];
    strcpy(evento7, traerValorEepromPorDelimitador(';', 12));
  	char evento8[10];
    strcpy(evento8, traerValorEepromPorDelimitador(';', 13));
  	char evento9[10];
    strcpy(evento9, traerValorEepromPorDelimitador(';', 14));
  	char evento10[10];
    strcpy(evento10, traerValorEepromPorDelimitador(';', 15));
	
  	Serial.println(pass);
    Serial.println(sensorHabitacion);
    Serial.println(sensorCocina);
    Serial.println(sensorEntrada);
    Serial.println(buffer);
    Serial.println(alarma);
    Serial.println(evento1);
    Serial.println(evento2);
    Serial.println(evento3);
    Serial.println(evento4);
    Serial.println(evento5);
    Serial.println(evento6);
    Serial.println(evento7);
    Serial.println(evento8);
    Serial.println(evento9);
    Serial.println(evento10);
}
void cargarEepromManual(){
	char pass[5]= "";
  	char sensorH[2];
  	char sensorC[2];
  	char sensorE[2];
  	char buzzer[2];
    char alarma[2];
    char evento1[]= "XXXX";
    char evento2[]= "XXXX";
    char evento3[]= "XXXX";
    char evento4[]= "XXXX";
    char evento5[]= "XXXX";
    char evento6[]= "XXXX";
    char evento7[]= "XXXX";
    char evento8[]= "XXXX";
    char evento9[]= "XXXX";
    char evento10[]= "XXXX";

    mostrarLCD("Ingrese pin");
    lcd.setCursor(0, 1);
    boolean banderaBucle = true;
    int i = 0;
    while (banderaBucle) {
        char tecla = kp.getKey();
        if (tecla != NO_KEY) {
            if(tecla != '#'){
                lcd.setCursor(i, 1);
                lcd.print(tecla);
                pass[i]=tecla;
                i++;
            }else{
                if (i == 4) {
                    banderaBucle = false;
                }else{
                    mostrarLCD("PIN debe tener 4");
                    strcpy(pass, "");
                    delay(1000);
                }
            }
        }
    }

    cargarLCD2Renglones("Sensor habitacion","1 act 2 desact");
  	strcpy(sensorH, cargarComponentes());

    cargarLCD2Renglones("Sensor cocina","1 act 2 desact");
  	strcpy(sensorC, cargarComponentes());

    cargarLCD2Renglones("Sensor entrada","1 act 2 desact");
  	strcpy(sensorE, cargarComponentes());

    mostrarLCD("Modo alarma");
    delay(1000);
    cargarLCD2Renglones("1 sonoro","1 silencioso");
  	strcpy(buzzer, cargarComponentes());

    cargarLCD2Renglones("Alarma","1 act 2 desact");
  	strcpy(alarma, cargarComponentes());

    escribirEeprom(pass,0,';');
  	escribirEeprom(sensorH,1,';');
  	escribirEeprom(sensorC,2,';');
    escribirEeprom(sensorE,3,';');
    escribirEeprom(buzzer,4,';');
    escribirEeprom(alarma,5,';');
    escribirEeprom(evento1,6,';');
    escribirEeprom(evento2,7,';');
    escribirEeprom(evento3,8,';');
    escribirEeprom(evento4,9,';');
    escribirEeprom(evento5,10,';');
    escribirEeprom(evento6,11,';');
    escribirEeprom(evento7,12,';');
    escribirEeprom(evento8,13,';');
    escribirEeprom(evento9,14,';');
    escribirEeprom(evento10,15,';');
}

char* cargarComponentes (){

static char estadoSensor[2];
memset(estadoSensor, 0, sizeof(estadoSensor));
boolean banderaBucle = true;

    while (banderaBucle) {
        char tecla = kp.getKey();
        if (tecla != NO_KEY) {
            if(tecla == '1'){
                strcpy(estadoSensor, "A");
                banderaBucle = false;
            }else if (tecla == '2'){
                strcpy(estadoSensor, "D");
                banderaBucle = false;
            }
        }
    }
return estadoSensor;
}

void cargarEepromPorDefault(){
	char pass[]= "1234";
  	char sensorH[]= "A";
  	char sensorC[]= "A";
  	char sensorE[]= "A";
  	char buzzer[]= "A";
    char alarma[]= "A";
    char evento1[]= "H3XX";
    char evento2[]= "C153";
    char evento3[]= "E455";
    char evento4[]= "C104";
    char evento5[]= "E216";
    char evento6[]= "E654";
    char evento7[]= "H418";
    char evento8[]= "E641";
    char evento9[]= "C647";
    char evento10[]= "C13X";

    escribirEeprom(pass,0,';');
  	escribirEeprom(sensorH,1,';');
  	escribirEeprom(sensorC,2,';');
    escribirEeprom(sensorE,3,';');
    escribirEeprom(buzzer,4,';');
    escribirEeprom(alarma,5,';');
    escribirEeprom(evento1,6,';');
    escribirEeprom(evento2,7,';');
    escribirEeprom(evento3,8,';');
    escribirEeprom(evento4,9,';');
    escribirEeprom(evento5,10,';');
    escribirEeprom(evento6,11,';');
    escribirEeprom(evento7,12,';');
    escribirEeprom(evento8,13,';');
    escribirEeprom(evento9,14,';');
    escribirEeprom(evento10,15,';');
}
void escribirEeprom(char cadena[], int pos, char delimitador){
    int epromPos;
    if(pos == 0){
        epromPos = 0;
    }else{
        epromPos = posEeprom(pos,delimitador)+1;
    }  
  
    for (int i=0; i<strlen(cadena); i++) {
       EEPROM.put(i+epromPos, cadena[i]);
    }
  	EEPROM.put(strlen(cadena)+epromPos, delimitador);
}

int posEeprom(int pos, char delimitador){
    boolean bandera = true;
    int i = 0;
    char aux;
    int posAux = 0,epromPos=0;

    while(bandera){
        //aux = EEPROM.read(i);
      EEPROM.get(i, aux);
        if(aux==delimitador){
          posAux++;
               	if(posAux>=pos){
                    bandera = false;
                    epromPos = i;
                }
        }
        i++;
    }
    return epromPos;
}
char* traerValorEepromPorDelimitador(char delimitador, int pos){
    static char cadena[50] = "";
  	memset(cadena, 0, sizeof(cadena));
  
    int epromPosComienzo = 0, epromPosFinal = 0;
    if(pos==0){
    epromPosFinal = posEeprom(pos,delimitador);
    }else {
    epromPosComienzo  = posEeprom(pos,delimitador)+1;
    epromPosFinal  = posEeprom(pos+1,delimitador);
    }
     
    for (epromPosComienzo; epromPosComienzo < epromPosFinal; epromPosComienzo++) {
      char c;
      EEPROM.get(epromPosComienzo,c);
      char str[2];
      str[0] = c;
      str[1] = '\0';
      strcat(cadena, str);
    }
    return cadena;
}
void cargarConfiguracion(){
  	char valorSH[2];
  	strcpy(valorSH, traerValorEepromPorDelimitador(';', 1));
  	char valorSC[2];
    strcpy(valorSC, traerValorEepromPorDelimitador(';', 2));
    char valorSE[2];
    strcpy(valorSE, traerValorEepromPorDelimitador(';', 3));
    char valorB[2];
    strcpy(valorB, traerValorEepromPorDelimitador(';', 4));
    char valorA[2];
  	strcpy(valorA, traerValorEepromPorDelimitador(';', 5));

    if(valorA[0]=='A'){
        alarma = true;
        if(valorSH[0]=='A' && sH==false){
        	encenderLed(ledHabitacion,128);
        }else{
        	encenderLed(ledHabitacion,0);
        }
        if(valorSC[0]=='A' && sC==false){
            encenderLed(ledCocina,128);
        }else{
        	encenderLed(ledCocina,0);
        }
        if(valorSE[0]=='A' && sE==false){
            encenderLed(ledEntrada,128);
        }else{
        	encenderLed(ledEntrada,0);
        }
        if(valorB[0]=='A'){
            piezoActivado = true;
        }
    }else{
        alarma = false;
        encenderLed(ledHabitacion,0);
        encenderLed(ledCocina,0);
        encenderLed(ledEntrada,0);
        piezoActivado = false;
      	noTone(pinPiezo);
        sH=false;
      	sC=false;
      	sE=false;
    }

    cargarLCD2Renglones("1 config","2 Alarma 3Evento");
}

void encenderLed (int pinLed, int intensidad){
	analogWrite(pinLed,intensidad);
}
void mostrarLCD(char cadena[]){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(cadena);
}
void cargarLCD2Renglones (char cadena1[], char candena2[]){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(cadena1);
    lcd.setCursor(0, 1);
    lcd.print(candena2);
}
void opcionConfig(){
boolean banderaBucle = true;
boolean banderaMenuPrincipal = false;

cargarLCD2Renglones("1 PIN 2 Sensor","3 Alarma");

    while (banderaBucle) {
        char tecla = kp.getKey();
        if (tecla != NO_KEY) {
            if (tecla != 'A' && tecla != 'C') {
                if(tecla == '1'){
                    banderaMenuPrincipal = cambiarPin();
                    if (banderaMenuPrincipal){
                        banderaBucle = false;
                    }else{
                        cargarLCD2Renglones("1 PIN 2 Sensor","3 Alarma");
                    }
                }else if(tecla == '2'){
                    banderaMenuPrincipal = estadoSensor();
                    if (banderaMenuPrincipal){
                        banderaBucle = false;
                    }else{
                        cargarLCD2Renglones("1 PIN 2 Sensor","3 Alarma");
                    }
                }else if(tecla == '3'){
                    banderaMenuPrincipal = estadoAlarma();
                    if (banderaMenuPrincipal){
                        banderaBucle = false;
                    }else{
                        cargarLCD2Renglones("1 PIN 2 Sensor","3 Alarma");
                    }
                }else{
                    mostrarLCD("Opción inválida");
                    delay(1000);
                }
                
            } else if (tecla == 'A') {
                banderaBucle = false;
            }else if (tecla == 'C') {
                banderaBucle = false;
            }
        }
    }
}
boolean cambiarPin() {
    boolean banderaMenuPrincipal = false;
    boolean banderaSalir = false;
    boolean banderaBucle = true;
    char pinAlarma[5], pinAlarmaNueva[5];
  	int cantidadCaracteres = 0;

    mostrarLCD("Ingrese pin viejo");
    lcd.setCursor(0, 1);
    while (banderaBucle) {
        char tecla = kp.getKey();
        if (tecla != NO_KEY) {
            if (numeroValido(tecla)) {
                lcd.print(tecla);
                char str[2];
                str[0] = tecla;
                str[1] = '\0';
                strcat(pinAlarma, str);
            } else if (tecla == 'A') {
                banderaSalir = true;
                banderaBucle = false;
            } else if (tecla == '#') {
                banderaBucle = false;
            }else if (tecla == 'C'){
                banderaBucle = false;
                banderaMenuPrincipal = true;
            }else if (tecla == 'D'){
                mostrarLCD("Ingrese pin nuevo");
                lcd.setCursor(0, 1);
                strcpy(pinAlarma, "");
            }
        }
    }

    if (!banderaSalir && banderaMenuPrincipal == false) {
        char valorPass[5];
  	    strcpy(valorPass, traerValorEepromPorDelimitador(';', 0));

        if (strcmp(valorPass, pinAlarma) == 0 ) {
            mostrarLCD("Ingrese pin nuevo");
            lcd.setCursor(0, 1);
            banderaBucle = true;

            while (banderaBucle) {
                char tecla = kp.getKey();
                if (tecla != NO_KEY) {
                    if (numeroValido(tecla)) {
                        lcd.print(tecla);
                        char str[2];
                        str[0] = tecla;
                        str[1] = '\0';
                        strcat(pinAlarmaNueva, str);
              			cantidadCaracteres++;
                    } else if (tecla == 'A' || tecla == 'C') {
                        banderaSalir = true;
                        banderaBucle = false;
                    } else if (tecla == '#') {
                        banderaBucle = false;
                        if (cantidadCaracteres == 4) {
                            escribirEeprom(pinAlarmaNueva, 0, ';');
                            mostrarLCD("PIN actualizado");
                            delay(1000);
                        } else{
                            mostrarLCD("PIN debe tener 4");
                            delay(1000);
                        }
                    }else if (tecla == 'D') {
                        mostrarLCD("Ingrese pin nuevo");
                        lcd.setCursor(0, 1);
                        strcpy(pinAlarmaNueva, "");
                      	cantidadCaracteres=0;
                    }
                }
            }

        } else {
            mostrarLCD("Pin incorrecto");
          	delay(1000);
        }
    }

    return banderaMenuPrincipal;
}
boolean estadoSensor(){
    boolean banderaMenuPrincipal = false;
    boolean banderaBucle = true;
    char valorSH[2];
  	strcpy(valorSH, traerValorEepromPorDelimitador(';', 1));
  	char valorSC[2];
    strcpy(valorSC, traerValorEepromPorDelimitador(';', 2));
    char valorSE[2];
    strcpy(valorSE, traerValorEepromPorDelimitador(';', 3));

    cargarLCD2Renglones("1SensH 2SensC","3SensE");
    
    while (banderaBucle) {
        char tecla = kp.getKey();
        if (tecla != NO_KEY) {
            if (tecla != 'A' && tecla != 'C' ) {
                if(tecla == '1'){
                    if(valorSH[0]=='A'){
                        cargarEstadoSensor("Habitacion","Activo");
                    }else{
                        cargarEstadoSensor("Habitacion","Inactivo");
                    }
                    delay(2000);
                    cambiarEstadoSensor('H');
                }else if(tecla == '2'){
                    if(valorSC[0]=='A'){
                        cargarEstadoSensor("Cocina","Activo");
                    }else{
                        cargarEstadoSensor("Cocina","Inactivo");
                    }
                    delay(2000);
                    cambiarEstadoSensor('C');
                }else if(tecla == '3'){
                    if(valorSE[0]=='A'){
                        cargarEstadoSensor("Entrada","Activo");
                    }else{
                        cargarEstadoSensor("Entrada","Inactivo");
                    }
                    delay(2000);
                    cambiarEstadoSensor('E');
                }else{
                    mostrarLCD("Opción inválida");
                    delay(1000);
                }

                cargarLCD2Renglones("1SensH 2SensC","3SensE");

            } else if (tecla == 'A') {
                banderaBucle = false;
            }else if (tecla == 'C') {
                banderaBucle = false;
                banderaMenuPrincipal = true;
            }
        }
        strcpy(valorSH, traerValorEepromPorDelimitador(';', 1));
  		strcpy(valorSC, traerValorEepromPorDelimitador(';', 2));
    	strcpy(valorSE, traerValorEepromPorDelimitador(';', 3));
    }

    return banderaMenuPrincipal;
}
void cargarEstadoSensor(char sensor[], char estado[]){
    char estadoAMostrar[20] = "";
    char sensorAMostrar[20] = "";

    strcat(estadoAMostrar, "Estado ");
    strcat(sensorAMostrar, "Sensor ");
    strcat(estadoAMostrar, estado);
    strcat(sensorAMostrar, sensor);
    cargarLCD2Renglones(sensorAMostrar,estadoAMostrar);
}
void cambiarEstadoSensorEeprom(char sensor, char estado){
    char estadoAux[2];
    char str[2];
    str[0] = estado;
    str[1] = '\0';
    //strcat(estadoAux, str);
    if(sensor== 'H'){
        escribirEeprom(str, 1, ';');
    }else if(sensor== 'C'){
        escribirEeprom(str, 2, ';');
    }else{
        escribirEeprom(str, 3, ';');
    }
}
void cambiarEstadoSensor(char sensor){
    boolean banderaBucle = true;
    cargarLCD2Renglones("1 Activar","2 Desactivar");

    while (banderaBucle) {
        char tecla = kp.getKey();
        if (tecla != NO_KEY) {
            if (tecla != 'A') {
              	cargarLCD2Renglones("1 Activar","2 Desactivar");
                if(tecla == '1'){
                    cambiarEstadoSensorEeprom(sensor,'A');
                    mostrarLCD("Sensor Activo");
                  	delay(1000);
                  	banderaBucle = false;
                }else if(tecla == '2'){
                    cambiarEstadoSensorEeprom(sensor,'D');
                    mostrarLCD("Sensor Inactivo");
                  	delay(1000);
                  	banderaBucle = false;
                }else{
                    mostrarLCD("Opción inválida");
                    delay(1000);
                }
            } else if (tecla == 'A') {
                banderaBucle = false;
            }
        }
    }
}
boolean estadoAlarma(){
    boolean banderaMenuPrincipal = false;
    char valorB[2];
    strcpy(valorB, traerValorEepromPorDelimitador(';', 4));
    boolean banderaBucle = true;

    if(valorB[0]=='A'){
        cargarLCD2Renglones("Alarma modo","sonoro");
    }else{
        cargarLCD2Renglones("Alarma modo","silencioso");
    }

    delay(2000);

    cargarLCD2Renglones("1 modo sonoro","2 modo silencioso");

    while (banderaBucle) {
        char tecla = kp.getKey();
        if (tecla != NO_KEY) {
            if (tecla != 'A' && tecla != 'C') {
                if(tecla == '1'){
                    char str[2];
                    str[0] = 'A';
                    str[1] = '\0';
                    escribirEeprom(str, 4, ';');
                    cargarLCD2Renglones("Alarma en","modo sonoro");
                    piezoActivado = true;
                  	delay(1000);
                }else if(tecla == '2'){
                    char str[2];
                    str[0] = 'D';
                    str[1] = '\0';
                    escribirEeprom(str, 4, ';');
                    piezoActivado = false;
                    cargarLCD2Renglones("Alarma en","modo silencioso");
                  	delay(1000);
                }else{
                    mostrarLCD("Opción inválida");
                    delay(1000);
                }
                cargarLCD2Renglones("1 modo sonoro","2 modo silencioso");
            } else if (tecla == 'A') {
                banderaBucle = false;
            }else if (tecla == 'C') {
                banderaBucle = false;
                banderaMenuPrincipal = true;
            }
        }
    }

    return banderaMenuPrincipal;
}
void opcionAlarma() {
  	char pinAlarma[5];
    char aux[2];
    boolean banderaSalir = false;
    boolean banderaBucle = true;
    int i = 0;
    mostrarLCD("Ingrese pin");
    lcd.setCursor(0, 1);
  
    while (banderaBucle) {
        char tecla = kp.getKey();
        if (tecla != NO_KEY) {
            if (tecla != 'A' && tecla != '#' && tecla != 'C') {
                lcd.setCursor(i, 1);
                lcd.print(tecla);
                pinAlarma[i]=tecla;
                i++;
            } else if (tecla == 'A') {
                banderaSalir = true;
                banderaBucle = false;
            } else if (tecla == 'C') {
                banderaSalir = true;
                banderaBucle = false;
            } else if (tecla == '#') {
                banderaBucle = false;
            }
        }
    }
    banderaBucle = true;
    if (!banderaSalir) {
        char valorPass[5];
  	    strcpy(valorPass, traerValorEepromPorDelimitador(';', 0));
        if (strcmp(valorPass, pinAlarma) == 0 ){
            char valorAlarma[2];
            strcpy(valorAlarma, traerValorEepromPorDelimitador(';', 5));
            if(valorAlarma[0] == 'A'){
                cargarLCD2Renglones("Alarma ","Activa");
            }else{
                cargarLCD2Renglones("Alarma ","Inactiva");
            }
            delay(1000);
            
            cargarLCD2Renglones("1 Activar","2 Desactivar");

            while (banderaBucle) {
                char tecla = kp.getKey();
                if (tecla != NO_KEY) {
                    if (tecla != 'A') {
                        if(tecla == '1'){
                            char str[2];
                            str[0] = 'A';
                            str[1] = '\0';
                            escribirEeprom(str, 5, ';');
                            mostrarLCD("Alarma Activa");
                            delay(1000);
                        }else if(tecla == '2'){
                            char str[2];
                            str[0] = 'D';
                            str[1] = '\0';
                            escribirEeprom(str, 5, ';');
                            mostrarLCD("Alarma Inactiva");
                            delay(1000);
                        }else{
                            mostrarLCD("Opción inválida");
                            delay(1000);
                        }
                        cargarLCD2Renglones("1 Activar","2 Desactivar");
                    } else if (tecla == 'A') {
                        banderaBucle = false;
                    }
                }
            }
        } else {
            cargarLCD2Renglones("PIN","incorrecto");
            if(sC==true){
                sC=false;
            }
            if(sE==true){
                sE=false;
            }
            if(sH==true){
                sH=false;
            }
        }
    }
}
void mostrarEventos(){
    int i = 15;
    boolean bandera = true;
    i = mostrarEvento(i);
  	if (i>6){
    while(bandera){
        char tecla = kp.getKey();
        if (tecla != NO_KEY) {
            if (tecla == '1') {
                i = i-1;
              	if(i==6){
                    bandera = false;
                    cargarLCD2Renglones("No hay mas","eventos");
                    delay(2000);
                }else{
                	mostrarEvento(i);
                }
            } else if (tecla == 'A') {
                bandera = false;
            }
            
        }
    }
    }
}
int mostrarEvento (int pos){
    int posAux = pos;
    char evento[10];
  	boolean bandera = true;
    strcpy(evento, traerValorEepromPorDelimitador(';', posAux));

    if(evento[0]=='X'){
        while (bandera){
            posAux = posAux - 1;
            strcpy(evento, traerValorEepromPorDelimitador(';', posAux));
          	if(evento[0]!='X'){
          		bandera = false;
          	}
            if(posAux==6){
				bandera = false;
            }
        }
    }
  	if(posAux!=6){
      mostrarLCD("Alarma ");
      for(int i = 0; i<10; i++){
          if(i==0){
              if(evento[i] == 'H'){
                  lcd.print("Habitacion");
              }else if (evento[i] == 'C'){
                  lcd.print("Cocina");
              }else{
                  lcd.print("Entrada");
              }
              lcd.setCursor(0, 1);
              lcd.print("tiempo act ");
          }else{
              if(numeroValido(evento[i])){
                  lcd.print(evento[i]);
              }else{
                  i=10;
              }
          }

      }
    	delay(2000);
    	cargarLCD2Renglones("1 siguiente evento","A salir");
    }else{
    	cargarLCD2Renglones("No hay eventos","cargados");
      	delay(2000);
    }

    return posAux;
}
boolean numeroValido(char num){
boolean bandera = false;
  
  	if(num=='0'){
  		bandera = true;
    }else if(num=='1'){
      bandera = true;
    }else if(num=='2'){
      bandera = true;
    }else if(num=='3'){
      bandera = true;
    }else if(num=='4'){
      bandera = true;
    }else if(num=='5'){
      bandera = true;
    }else if(num=='6'){
      bandera = true;
    }else if(num=='7'){
      bandera = true;
    }else if(num=='8'){
      bandera = true;
    }else if(num=='9'){
      bandera = true;
    }
  
return bandera;
}
void leerSensorCocina(){
char valorSensorC[2];
strcpy(valorSensorC, traerValorEepromPorDelimitador(';', 2));

    if(valorSensorC[0]=='A' && alarma){
        int sensorC = leerSensor(sensorCocina);
        if(sensorC<100){
            sC=true;
          	encenderLed(ledCocina,255);
            if(timpoInicioC == false){
                tiempoActC = millis();
                timpoInicioC = true;
            }
            tiempoInActC = millis();
            if(piezoActivado){
                tone(pinPiezo, 100);
            }else{
            	noTone(pinPiezo);
            }
        }else{
            if(alarma){
                encenderLed(ledCocina,128);
                noTone(pinPiezo);
            }
            if(timpoInicioC){
                int tiempo =  tiempoInActC - tiempoActC ;
                int min = tiempo / 60000;
                registrarEvento('C',min);
                timpoInicioC = false;
            }
        }
    }
}
int leerSensor(int sensor){
long duration, inches, cm;

    pinMode(sensor, OUTPUT);
    digitalWrite(sensor, LOW);
    delayMicroseconds(2);
    digitalWrite(sensor, HIGH);
    delayMicroseconds(5);
    digitalWrite(sensor, LOW);

    pinMode(sensor, INPUT);
    duration = pulseIn(sensor, HIGH);
    
    cm = microsecondsToCentimeters(duration);
    
    return cm;

}
long microsecondsToCentimeters(long microseconds) {
    return microseconds / 29 / 2;
}
void leerSensorHabitacion(){
char valorSensorH[2];
strcpy(valorSensorH, traerValorEepromPorDelimitador(';', 1));

    if(valorSensorH[0]=='A'){
        int sensorH = leerSensor(sensorHabitacion);
        if(sensorH<100 && alarma){
            sH=true;
          	encenderLed(ledHabitacion,255);
            if(timpoInicioH == false){
                tiempoActH = millis();
                timpoInicioH = true;
            }
            tiempoInActH = millis();
            if(piezoActivado){
                tone(pinPiezo, 100);
            }else{
            	noTone(pinPiezo);
            }
        }else{
            if(alarma){
                encenderLed(ledHabitacion,128);
                noTone(pinPiezo);
            }
            if(timpoInicioH){
                int tiempo = tiempoInActH - tiempoActH ;
                int seg = tiempo / 1000;
                registrarEvento('H',seg);
                timpoInicioH=false;
            }
        }
    }
}
void leerSensorEntrada(){
char valorSensorE[2];
strcpy(valorSensorE, traerValorEepromPorDelimitador(';', 3)); 

    if(valorSensorE[0]=='A'){
        int sensorE = leerSensor(sensorEntrada);
        if(sensorE<100 && alarma){
            sE=true;
          	encenderLed(ledEntrada,255);
            if(timpoInicioE == false){
                tiempoActE = millis();
                timpoInicioE = true;
            }
            tiempoInActE = millis();
            if(piezoActivado){
                tone(pinPiezo, 100);
            }else{
            	noTone(pinPiezo);
            }//
        }else{
            if(alarma){
                encenderLed(ledEntrada,128);
                noTone(pinPiezo);
            }
            if(timpoInicioE){
                int tiempo =tiempoInActE - tiempoActE ;
                int min = tiempo / 60000;
                registrarEvento('E',min);
                timpoInicioE = false;
            }
        }
    }
}
void registrarEvento(char sensor, int tiempo){
    int i = 6;
    boolean registrado = false;
    char evento[10];
    strcpy(evento, traerValorEepromPorDelimitador(';', i));
    
    while(i<15){
        i++;
        strcpy(evento, traerValorEepromPorDelimitador(';', i));
        if(evento[0]=='X'){
            escribirEventoEeprom(sensor,tiempo,i);
            registrado = true;
            i=15; //para salir del bucle
        }
    }
    if(registrado==false){
        moverEvento(sensor,tiempo);
    }
}
void escribirEventoEeprom(char sensor, int tiempo, int pos){
  	char cadena[10];
    cadena[0]= sensor;
  	cadena[1] = '\0';
    char buffer[10]; 
    itoa(tiempo, buffer, 10);
    strcat(cadena, buffer);
  	int cantCarc = strlen(cadena);
  
    for(int i=cantCarc; i<4; i++){
        cadena[i]='X';
    }
	cadena[4] = '\0';
    escribirEeprom(cadena,pos,';');
}
void moverEvento(char sensor, int tiempo){
    char evento[10];
    for(int i = 6; i<15; i++){
        strcpy(evento, traerValorEepromPorDelimitador(';', i+1));
        escribirEeprom(evento,i,';');
    }
    escribirEventoEeprom(sensor,tiempo,15);
}