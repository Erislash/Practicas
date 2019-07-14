#include <Pagina.h>               //Libreria de las páginas {Documentos\Arduino\libraries}
#include <PubSubClient.h>         //Librería para uso del protocolo MQTT
#include <ESP8266WiFi.h>              
#include <ESP8266WebServer.h>         
#include <EEPROM.h>


#define led 2 //(PullUp)
#define p 0   //(PullUp)


Pagina pag;                       //INICIALIZACIÓN DE LA CLASE PÁGINA                    
ESP8266WebServer server(80);
WiFiClient clienteESP;
PubSubClient client(clienteESP);


String ssid = "";               //NOMBRE DE LA RED
String passRed  = "";           //CONTRASEÑA DE LA RED


String usuario;                 //USUARIO PARA ACCEDER A LA PÁGINA DE CONFIGURACIÓN
String passConfig;              //CONTRASEÑA PARA ACCEDER


//CONFIGURACIÓN MQTT
String servidorMQTT = "";
int puertoMQTT = 0;
String usuarioMQTT = "";
String contraMQTT = "";
String temaMQTT = "";


String mensaje = "";                  //MENSAJE A MOSTRAR LUEGO DE APLICAR CAMBIOS

void setup()
{
  EEPROM.end();
  EEPROM.begin(512);
  Serial.begin(115200);
  
  Serial.printf("\n\n\n");
  pinMode(p, INPUT);
  pinMode(led, OUTPUT);
  
  digitalWrite(led, HIGH);

//SI EL NODE ARRANCA POR PRIMERA VEZ O SE LO RESTABLECE DE FÁBRICA, SE ACCEDE CON EL USUARIO Y CONTRASEÑA POR DEFECTO (Admin, Admin)
  if(Leer(150, 10) != "Cambiado"){
    Grabar(120, "Admin", 15);
    Grabar(135, "Admin", 15);
  }
  
  usuario = Leer(120, 15);
  passConfig = Leer(135, 15);
  if(digitalRead(p) == LOW){
    Serial.println("INICIADO EN MODO SERVIDOR");
    digitalWrite(led, LOW);
    delay(100);
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    delay(100);
    digitalWrite(led, HIGH);
    
    WiFi.mode(WIFI_AP);
    boolean servidor = WiFi.softAP("Node", "esp82666");

//INICIO DEL SERVIDOR Y CONFIGURACIÓN DE SUS PARÁMETROS
    if(servidor == true){
      IPAddress ipLocal(192,168,4,108);
      IPAddress puertaAcceso(192,168,4,9);
      IPAddress mascaraSubnet(255,255,255,0);

      WiFi.softAPConfig(ipLocal, puertaAcceso, mascaraSubnet);
      Serial.println("\nEL SERVIDOR HA INICIADO");
      Serial.print("Direccion IP = ");
      Serial.println(WiFi.softAPIP());
    }
    else{
      Serial.println("\nEL SERVIDOR HA FALLADO");
    }
    server.on("/", HTTP_GET, Acceso);
    server.on("/acceder", HTTP_POST, Configuracion);
    server.on("/aplicar", HTTP_POST, Cambio);
    server.on("/reiniciar", HTTP_POST, Reinicio);
    server.on("/restablecer", HTTP_POST, ReinicioFabrica);
    server.onNotFound(NoEncontrado);
  
    server.begin();  

    while (true){
      server.handleClient();
    }
  }
  Serial.println("INICIADO EN MODO CLIENTE");
  ssid = Leer(0, 15);
  passRed = Leer(15, 15);

  WiFi.disconnect();
    
  if(Leer(110, 10) == "estatica"){
    Serial.println("RED ESTATICA CONFIGURADA");
      
    IPAddress ipEstatica(Leer(30, 5).toInt(), Leer(35, 5).toInt(), Leer(40, 5).toInt(), Leer(45, 5).toInt());
    IPAddress compuerta((Leer(50, 5).toInt()), (Leer(55, 5).toInt()), (Leer(60, 5).toInt()), (Leer(65, 5).toInt()));
    IPAddress mascara((Leer(70, 5).toInt()), (Leer(75, 5).toInt()), (Leer(80, 5).toInt()), (Leer(85, 5).toInt()));
    IPAddress dns((Leer(90, 5).toInt()), (Leer(95, 5).toInt()), (Leer(100, 5).toInt()), (Leer(105, 5).toInt()));
      
    Serial.println(ipEstatica);
    Serial.println(compuerta);
    Serial.println(mascara);
    Serial.println(dns);

    if(!WiFi.config(ipEstatica, compuerta, mascara, dns)){
      Serial.println("NO SE PUDO REALIZAR UNA CONFIGURACIÓN ESTÁTICA");
    }else{
      Serial.println("SE HIZO UNA CONFIGURACIÓN ESTÁTICA");
    }
  }else{
    Serial.println("RED DINAMICA CONFIGURADA");
  }
    
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, passRed);
    
  Serial.print("Intentando conectar a red: ");
  Serial.println(ssid);
  Serial.print("Contraseña: ");
  Serial.println(passRed);
          
  Serial.print("CONECTANDO");
  int intento = 0;
  while(WiFi.status() != WL_CONNECTED){
    if(intento < 50){
      Serial.print(".");
      digitalWrite(led, LOW);
      delay(250);
      digitalWrite(led, HIGH);
      delay(250);
    }else
      break;
    intento++;
  }
  if (intento >= 50){
    Serial.println("IMPOSIBLE CONECTAR");
    return;
  }else{
    Serial.print("\nConectado a NUEVA red: ");
    Serial.println(WiFi.SSID());
    Serial.print("NUEVA Direccion IP: ");
    Serial.println(WiFi.localIP());
    digitalWrite(led, LOW);
  }
  

//CONFIGURACIÓN DE DATOS MQTT
  servidorMQTT = Leer(160, 30);
  puertoMQTT = Leer(190, 30).toInt();
  usuarioMQTT = Leer(220, 30);
  contraMQTT = Leer(250, 30);
  temaMQTT = Leer(280, 30);
    
  Serial.println("CONFIGURACIÓN MQTT------------");
  Serial.println(servidorMQTT);
  Serial.println(puertoMQTT);
  Serial.println(usuarioMQTT);
  Serial.println(contraMQTT);
  Serial.println(temaMQTT);

  if(servidorMQTT == NULL || puertoMQTT == NULL || 
  usuarioMQTT == NULL || contraMQTT == NULL || temaMQTT == NULL)
  {
    Serial.println("NO HAY DATOS PARA MQTT");
    return;
  }
  client.setServer(servidorMQTT.c_str(), puertoMQTT);
  client.setCallback(Llamada);
  while (!client.connected()){
    Serial.println("Conectando a MQTT...");
    if (client.connect("ClienteESP8266", usuarioMQTT.c_str(), contraMQTT.c_str())){
      Serial.println("Conectado");  
      client.publish(temaMQTT.c_str(), "Hola desde el ESP8266");
      client.subscribe(temaMQTT.c_str());
    }else {
      Serial.print("Fallo en la conexión");
      Serial.print(client.state());
      delay(2000);
    }
  }
  
  client.setCallback(Llamada);
}


void Llamada(char* topic, byte* payload, unsigned int length) 
{
  String orden = "";
 
  Serial.print("Ha llegado un mensaje desde el tema: ");
  Serial.println(topic);
 
  Serial.print("Mensaje:");
  for (int i = 0; i < length; i++){
    Serial.print((char)payload[i]);
    orden += (char)payload[i];
  }
  Serial.println(orden);
  if(orden == "encender")
    digitalWrite(led, LOW);
  else
    digitalWrite(led, HIGH);
  
  Serial.println();
  Serial.println("-----------------------");
}


void loop()
{
  client.loop();
}


void Acceso()
{
   server.send(200, "text/html", pag.PInicio().c_str()); 
}


//VALIDACIÓN DE LOS DATOS DE ACCESO
void Configuracion()
{
  Serial.printf("Usuario de acceso: ");
  Serial.println(usuario);
  Serial.printf("Contraseña de acceso: ");
  Serial.println(passConfig);
  if(!server.hasArg("usuario") || !server.hasArg("password") || 
  server.arg("usuario") == NULL || server.arg("password") == NULL){
    server.send(400, "text/html", pag.PMensaje("ACCESO INCOMPLETO"));
    return;
  }
  if(server.arg("usuario") == usuario && server.arg("password") == passConfig){  
//ESCANEO DE LAS REDES Y LAS PASO COMO ARGUMENTO A LA FUNCIÓN DE LA CLASE PÁGINA
    int numeroRedes = WiFi.scanNetworks();
    String redes[numeroRedes];

    for(int i = 0; i < numeroRedes; i++){
      redes[i] = WiFi.SSID(i).c_str();
    }

    server.send(200, "text/html", pag.PConfiguracion(numeroRedes, redes).c_str());
  }else{
    server.send(401, "text/html", pag.PMensaje("USUARIO O CONTRASEÑA INVÁLIDOS"));
  }
}


//REALIZA LAS COMPROBACIONES DE LOS AJUSTES INTRODUCIDOS Y SI SON CORRECTOS APLICA LOS CAMBIOS
void Cambio()
{
  CambioRed();
  CambioAcceso();
  CambioMQTT();
  //LUEGO DE REALIZAR LAS COMPROBACIONES, ENVIÓ UNA PÁGINA INFORMANDO SI SE PUDIERON APLICAR LOS CAMBIOS
  server.send(200, "text/html", pag.PMensaje(mensaje).c_str());
  mensaje = "";   //LIMPIO LA VARIABLE CON LA INFORMACIÓN DE LOS CAMBIOS
}


void CambioRed()
{
    if (!server.hasArg("ssid_ap") || !server.hasArg("password_ap") || server.arg("ssid_ap") == NULL){
      mensaje += "<li>NO SE MODIFICÓ LA RED</li>";
      return;
    }
    else if(server.hasArg("ssid_ap") && server.hasArg("password_ap")){
      Serial.println(server.arg("ip"));
      
      String nSSID = server.arg("ssid_ap");
      String nPassWifi = server.arg("password_ap");
      
      Grabar(0, nSSID, 15);
      Grabar(15, nPassWifi, 15);
      
      if(server.arg("ip") == "dinamica"){
        Serial.println("SELECCIONADA RED DINAMICA");
        Grabar(110, "dinamica", 10);
      }
      else if(server.arg("ip") == "estatica"){
        Serial.println("SELECCIONADA RED ESTATICA");

        if(!server.hasArg("ip_est_1") || !server.hasArg("ip_est_2") || !server.hasArg("ip_est_3") || !server.hasArg("ip_est_4")
          || !server.hasArg("mascara_est_1") || !server.hasArg("mascara_est_2") || !server.hasArg("mascara_est_3") || !server.hasArg("mascara_est_4")
            || !server.hasArg("puerta_est_1") || !server.hasArg("puerta_est_2") || !server.hasArg("puerta_est_3") || !server.hasArg("puerta_est_4")
              || !server.hasArg("dns_est_1") || !server.hasArg("dns_est_2") || !server.hasArg("dns_est_3") || !server.hasArg("dns_est_4")
          || server.arg("ip_est_1") == NULL || server.arg("ip_est_2") == NULL || server.arg("ip_est_3") == NULL || server.arg("ip_est_4") == NULL
            ||  server.arg("mascara_est_1") == NULL || server.arg("mascara_est_2") == NULL || server.arg("mascara_est_3") == NULL || server.arg("mascara_est_4") == NULL
              || server.arg("puerta_est_1") == NULL || server.arg("puerta_est_2") == NULL || server.arg("puerta_est_3") == NULL || server.arg("puerta_est_4") == NULL
                || server.arg("dns_est_1") == NULL || server.arg("dns_est_2") == NULL || server.arg("dns_est_3") == NULL || server.arg("ip_est_4") == NULL
          ){
          mensaje += "<li>NO INGRESÓ TODOS LOS DATOS PARA LOGRAR UNA IP ESTÁTICA</li>";
          return;
        }
        else{
          String ipEstatica[4] = {server.arg("ip_est_1"), server.arg("ip_est_2"), server.arg("ip_est_3"), server.arg("ip_est_4")};
          String compuerta[4] = {server.arg("mascara_est_1"), server.arg("mascara_est_2"), server.arg("mascara_est_3"), server.arg("mascara_est_4")};
          String mascara[4] = {server.arg("puerta_est_1"), server.arg("puerta_est_2"), server.arg("puerta_est_3"), server.arg("puerta_est_4")};
          String dns[4] = {server.arg("dns_est_1"), server.arg("dns_est_2"), server.arg("dns_est_3"), server.arg("dns_est_4")};

          Grabar(30, ipEstatica[0], 5);
          Grabar(35, ipEstatica[1], 5);
          Grabar(40, ipEstatica[2], 5);
          Grabar(45, ipEstatica[3], 5);
          Grabar(50, compuerta[0], 5);
          Grabar(55, compuerta[1], 5);
          Grabar(60, compuerta[2], 5);
          Grabar(65, compuerta[3], 5);
          Grabar(70, mascara[0], 5);
          Grabar(75, mascara[1], 5);
          Grabar(80, mascara[2], 5);
          Grabar(85, mascara[3], 5);
          Grabar(90, dns[0], 5);
          Grabar(95, dns[1], 5);
          Grabar(100, dns[2], 5);
          Grabar(105, dns[3], 5);
          Grabar(110, "estatica", 10);
        }
      }
      else{
        Serial.println("NO RECIBO RED");
      }    
    }
}

void CambioAcceso()
{
  if (!server.hasArg("usuario_web") || !server.hasArg("password_web") || 
  server.arg("usuario_web") == NULL || server.arg("password_web") == NULL){
    mensaje += "<li>NO SE MODIFICO EL USUARIO NI CONTRASEÑA</li>";
    return;
  }else{
    usuario = server.arg("usuario_web");
    passConfig = server.arg("password_web");

    Grabar(120, usuario, 15);
    Grabar(135, passConfig, 15);
    Grabar(150, "Cambiado", 10);

    Serial.println(Leer(150, 10));
    Serial.println("USUARIO Y CONTRASEÑA MODIFICADOS");
    mensaje += "<li>USUARIO Y CONTRASEÑA MODIFICADOS</li>";
  }
}


void CambioMQTT()
{
  if(!server.hasArg("mqtt_server") || !server.hasArg("id_client") || !server.hasArg("usuario_mqtt") || !server.hasArg("password_mqtt") || !server.hasArg("prefijo_mqtt")
    || server.arg("mqtt_server") == NULL || server.arg("id_client") == NULL || server.arg("usuario_mqtt") == NULL || server.arg("password_mqtt") == NULL ||
    server.arg("prefijo_mqtt") == NULL){
      Serial.println("NO SE HA MODIFICADO EL BROKER MQTT");
      mensaje += "<li>NO SE MODIFICÓ EL BROKER MQTT</li>";
    }
    else{
      Grabar(160, server.arg("mqtt_server"), 30);
      Grabar(190, server.arg("id_client"), 30);
      Grabar(220, server.arg("usuario_mqtt"), 30);
      Grabar(250, server.arg("password_mqtt"), 30);
      Grabar(280, server.arg("prefijo_mqtt"), 30);
      
      Serial.println("SE HA MODIFICADO EL BROKER MQTT");
      mensaje += "<li>SE MODIFICÓ EL BROKER MQTT</li>";
    }
}


void Reinicio()
{
  Serial.println("REINICIANDO NODE MCU");
  server.send(200, "text/html", pag.PMensaje("REINICIANDO NODE MCU"));
  ESP.restart();
}


void ReinicioFabrica()
{
  Grabar(0, "0", 254);
  Serial.println("\n\n////////////////////\nREINICIO DE FÁBRICA\n////////////////////\n\n");
  ESP.restart();
}


void Grabar(int ph_dir, String ph_dato, int ph_uso)
{
  int espacio = ph_dato.length(); 
  char almac[ph_uso]; 
  ph_dato.toCharArray(almac, espacio+1);
  for (int i = 0; i < espacio; i++){
    EEPROM.write(ph_dir + i, almac[i]);
  }
  for (int i = espacio; i < ph_uso; i++){
    EEPROM.write(ph_dir + i, 255);
  }
  EEPROM.commit();
}


String Leer(int ph_dir, int ph_uso)
{
  byte dato;
  String datoString;
  for (int i = ph_dir; i < ph_dir + ph_uso; i++){
    dato = EEPROM.read(i);
    if (dato != 255)
      datoString += (char)dato;
  }
   return datoString;
}


void NoEncontrado()
{
  server.send(404, "text/html", pag.PMensaje("PÁGINA NO ENCONTRADA"));
}
