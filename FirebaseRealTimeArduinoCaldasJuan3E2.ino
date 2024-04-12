//Inclusión de librerias
#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

//Credenciales para la conexión WiFi
const char* WIFI_SSID = "Sandra_Munoz";
const char* WIFI_PASSWORD= "010275194.@";

//Credenciales del Firebase
const char* API_KEY= "AIzaSyA9AYnkKqzXIIryyTSrc9weIHgkxbq9syA";
const char* DATABASE_URL= "https://realtimebd---iot-default-rtdb.firebaseio.com";
const char* USER_EMAIL= "dogmotica@gmail.com";
const char* USER_PASSWORD= "123456789";

//Objetos necesarios para la comunicación con el Firebase
FirebaseData fbdo; //Objeto para manejar la comunicación con Firebase
FirebaseAuth auth; //Objeto para la autenticación en Firebase
FirebaseConfig config; //Objeto de configuración de Firebase

//Variables auxiliares para el manejo del tiempo de envío y un contador para el estado del botón
unsigned long sendDataPrevMillis = 0;
unsigned long count = 0;

// Pines de los switches
const int atico =15;
const int banio =16;
const int cocina =17;
const int dormitorio =18;
const int garaje =19;
const int sala =21;

//Estado de los switches
int buttonStateAtico = 0;
int buttonStateBanio = 0;
int buttonStateCocina = 0;
int buttonStateDormitorio = 0;
int buttonStateGaraje = 0;
int buttonStateSala = 0;

//Configura y establece la conexión Wi-Fi
void setup_WIFI(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  unsigned long ms = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

//Configura Firebase con las credenciales y la URL de la base de datos
void setupFirebase() {
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;
  config.signer.tokens.legacy_token = "<database secret>";
  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096, 1024);
  fbdo.setResponseSize(2048);
  Firebase.begin(&config, &auth);
  Firebase.setDoubleDigits(5);
  config.timeout.serverResponse = 10 * 1000;
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

}

void setup() {
    Serial.begin(115200); //Comunicación seria
    setup_WIFI(); //Conecta el dispositivo a la red WiFi especificada con las credenciales 
    setupFirebase(); //Conexión con la plataforma Firebase con las credenciales
    //Configuración el pines como una entrada.
    pinMode(atico,INPUT);
    pinMode(banio,INPUT);
    pinMode(cocina,INPUT);
    pinMode(dormitorio,INPUT);
    pinMode(garaje,INPUT);
    pinMode(sala,INPUT);
}

void loop() {
  //Verificar si el Firebase está listo y ha transcurrido suficiente tiempo desde el último envío de datos
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
  {
    //Lee el estado de los switches
    buttonStateAtico = digitalRead(atico);
    buttonStateBanio = digitalRead(banio);
    buttonStateCocina = digitalRead(cocina);
    buttonStateDormitorio = digitalRead(dormitorio);
    buttonStateGaraje = digitalRead(garaje);
    buttonStateSala = digitalRead(sala);
      
    //Actualiza los valores en la base de datos del Firebase  
    Serial.printf("Set int... %s\n", Firebase.RTDB.setInt(&fbdo, F("/casa/atico"), buttonStateAtico) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Set int... %s\n", Firebase.RTDB.setInt(&fbdo, F("/casa/banio"), buttonStateBanio) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Set int... %s\n", Firebase.RTDB.setInt(&fbdo, F("/casa/cocina"), buttonStateCocina) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Set int... %s\n", Firebase.RTDB.setInt(&fbdo, F("/casa/dormitorio"), buttonStateDormitorio) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Set int... %s\n", Firebase.RTDB.setInt(&fbdo, F("/casa/garaje"), buttonStateGaraje) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Set int... %s\n", Firebase.RTDB.setInt(&fbdo, F("/casa/sala"), buttonStateSala) ? "ok" : fbdo.errorReason().c_str());
  }
}
