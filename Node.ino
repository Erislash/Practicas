#include <PubSubClient.h>             //LIBRERIA PARA COMUNICACION MQTT
#include <Pagina.h>                   //LIBRERIA DE LAS PAGINAS WEB
#include <ESP8266WiFi.h>              //LIBRERIAS DEL NODE
#include <ESP8266WebServer.h>         //LIBRERIA PARA SERVIDOR WIFI
#include <EEPROM.h>

struct Acceso
{
   char Usuario;
   char Contrasena;
};

struct Wifi
{
   const char* nombre;
   const char* contrasena;
   int ip1;
   int ip2;
   int ip3;
   int ip4;
   int mascara1;
   int mascara2;
   int mascara3;
   int mascara4;
   int puerta1;
   int puerta2;
   int puerta3;
   int puerta4;
   int dns1;
   int dns2;
   int dns3;
   int dns4;
};

struct Broker
{
   char ip;
   char idCliente;
   char usuario;
   char contrasena;
   char prefijo;
};








Pagina pag;                           //INICIALIZACIÓN DEL OBJETO PAGINA


String ssid = "";
String contra  = "";

ESP8266WebServer server(80);
WiFiClient clienteESP; 
         
String usuario = "Usuario";           //USUARIO PARA ACCEDER A LA PÁGINA DE CONFIGURACIÓN
String pass = "Usuario";              //CONTRASEÑA PARA ACCEDER

String mensaje = "";                  //MENSAJE A MOSTRAR LUEGO DE APLICAR CAMBIOS



void setup()
{
  EEPROM.begin(512);
  Serial.begin(115200);
  pinMode(0, INPUT);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  
  if(digitalRead(0) == LOW)
  {
    Serial.println("INICIADO EN MODO SERVIDOR");
    digitalWrite(2, LOW);
    delay(100);
    digitalWrite(2, HIGH);
    delay(100);
    digitalWrite(2, LOW);
    delay(100);
    digitalWrite(2, HIGH);



       
    WiFi.mode(WIFI_AP);                 //CONFIGURO EL NODE COMO ACCESS POINT(SERVIDOR)
    boolean servidor = WiFi.softAP("Node", "esp82666");


   if(servidor == true)
   {
    IPAddress ipLocal(192,168,4,108);
    IPAddress puertaAcceso(192,168,4,9);
    IPAddress mascaraSubnet(255,255,255,0);

    WiFi.softAPConfig(ipLocal, puertaAcceso, mascaraSubnet);
    Serial.println("\nEL SERVIDOR HA INICIADO");
    Serial.print("Direccion IP = ");
    Serial.println(WiFi.softAPIP());
  }
  else
  {
    Serial.println("\nEL SERVIDOR HA FALLADO");
  }
  server.on("/", HTTP_GET, Acceso);
  server.on("/acceder", HTTP_POST, Congifuracion);
  server.on("/aplicar", HTTP_POST, Cambio);
  server.on("/reset", HTTP_POST, Reinicio);
  server.onNotFound(NoEncontrado);
  
  server.begin();  

  while (true) 
  {
      server.handleClient();
  }
}
    Serial.println("INICIADO EN MODO CLIENTE");
    ssid = Leer(0);
    contra = Leer(50);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, contra);
    

    Serial.print("Intentando conectar a red: ");
    Serial.println(ssid);
    Serial.print("Contraseña: ");
    Serial.println(contra);
          
    Serial.print("CONECTANDO");
    int intento = 0;
    while(WiFi.status() != WL_CONNECTED)
    {
      if(intento < 50)
      {
        Serial.print(".");
        digitalWrite(2, LOW);
        delay(250);
        digitalWrite(2, HIGH);
        delay(250);
      }else
        break;
       intento++;
     }
     if (intento >= 50){
      Serial.println("IMPOSIBLE CONECTAR");
     }
     else
     {
      Serial.print("\nConectado a NUEVA red: ");
      Serial.println(WiFi.SSID());
      Serial.print("NUEVA Direccion IP: ");
      Serial.println(WiFi.localIP());
      digitalWrite(2, LOW);
     }
     

                     
}


void loop(){}


void Acceso()
{
   server.send(200, "text/html", pag.PInicio().c_str()); 
}

void Congifuracion()
{
  //COMPRUEBO SI EL ACCESO ES CORRECTO
  
  if(!server.hasArg("usuario") || !server.hasArg("password") || server.arg("usuario") == NULL || server.arg("password") == NULL)
  {
    server.send(400, "text/html", pag.PMensaje("ACCESO INCOMPLETO"));
    return;
  }
  if(server.arg("usuario") == usuario && server.arg("password") == pass)
  {
    //SI USUARIO Y CONTRASEÑA SON CORRECTOS, ENVÍO LA PÁGINA DE CONFIGURACIÓN
    server.send(200, "text/html", pag.PConfiguracion().c_str());
  }
  else
  {
    server.send(401, "text/html", pag.PMensaje("USUARIO O CONTRASEÑA INVÁLIDOS"));
  }
}


//REALIZA LAS COMPROBACIONES DE LOS AJUSTES INTRODUCIDOS Y SI SON CORRECTOS APLICA LOS CAMBIOS
void Cambio()
{
  CambioRed();
  CambioAcceso();
  CambioMQTT();
  server.send(200, "text/html", pag.PMensaje(mensaje).c_str());
  mensaje = "";
}


void CambioAcceso()
{
  if (!server.hasArg("usuario_web") || !server.hasArg("password_web") || server.arg("usuario_web") == NULL || server.arg("password_web") == NULL)
    {
      mensaje += "<li>NO SE MODIFICO EL USUARIO NI CONTRASEÑA<li>";
      return;
    }
    else
    {
      usuario = server.arg("usuario_web");
      pass = server.arg("password_web");
      Serial.println("USUARIO Y CONTRASEÑA MODIFICADOS");
      mensaje += "<li>USUARIO Y CONTRASEÑA MODIFICADOS</li>";
    }
}

void CambioRed()
{
    if (!server.hasArg("ssid_ap") || !server.hasArg("password_ap") || server.arg("ssid_ap") == NULL)
    {
      return;
    }
    else if(server.hasArg("ssid_ap") && server.hasArg("password_ap"))
    {
      Serial.println(server.arg("ip"));
      
      String nSSID = server.arg("ssid_ap");
      String ncontra = server.arg("password_ap");
      
      
      Grabar(0, nSSID);
      Grabar(50, ncontra);
      
      
      
      if(server.arg("ip") == "dinamica")
      {
        Serial.println("SELECCIONADA RED DINAMICA");
        WiFi.disconnect();
      }
      else if(server.arg("ip") == "estatica")
      {
        Serial.println("SELECCIONADA RED ESTATICA");
        if(!server.hasArg("ip_est_1") || !server.hasArg("ip_est_2") || !server.hasArg("ip_est_3") || !server.hasArg("ip_est_4")
          || !server.hasArg("mascara_est_1") || !server.hasArg("mascara_est_2") || !server.hasArg("mascara_est_3") || !server.hasArg("mascara_est_4")
            || !server.hasArg("puerta_est_1") || !server.hasArg("puerta_est_2") || !server.hasArg("puerta_est_3") || !server.hasArg("puerta_est_4")
              || !server.hasArg("dns_est_1") || !server.hasArg("dns_est_2") || !server.hasArg("dns_est_3") || !server.hasArg("dns_est_4")
          || server.arg("ip_est_1") == NULL || server.arg("ip_est_2") == NULL || server.arg("ip_est_3") == NULL || server.arg("ip_est_4") == NULL
            ||  server.arg("mascara_est_1") == NULL || server.arg("mascara_est_2") == NULL || server.arg("mascara_est_3") == NULL || server.arg("mascara_est_4") == NULL
              || server.arg("puerta_est_1") == NULL || server.arg("puerta_est_2") == NULL || server.arg("puerta_est_3") == NULL || server.arg("puerta_est_4") == NULL
                || server.arg("dns_est_1") == NULL || server.arg("dns_est_2") == NULL || server.arg("dns_est_3") == NULL || server.arg("ip_est_4") == NULL
          )
        {
          mensaje += "<li>NO INGRESÓ TODOS LOS DATOS PARA LOGRAR UNA IP ESTÁTICA</li>";
          return;
        }
        else
        {
          IPAddress ipEstatica(server.arg("ip_est_1").toInt(), server.arg("ip_est_2").toInt(), server.arg("ip_est_3").toInt(), server.arg("ip_est_4").toInt()); //ESP static ip
          IPAddress compuerta(server.arg("puerta_est_1").toInt(), server.arg("puerta_est_2").toInt(), server.arg("puerta_est_3").toInt(), server.arg("puerta_est_4").toInt());   //IP Address of your WiFi Router (Gateway)
          IPAddress mascara(server.arg("mascara_est_1").toInt(), server.arg("mascara_est_2").toInt(), server.arg("mascara_est_3").toInt(), server.arg("mascara_est_4").toInt());  //Subnet mask
          IPAddress dns(server.arg("dns_est_1").toInt(), server.arg("dns_est_2").toInt(), server.arg("dns_est_3").toInt(), server.arg("dns_est_4").toInt());

          Serial.println(ipEstatica);
          Serial.println(compuerta);
          Serial.println(mascara);
          Serial.println(dns);
          WiFi.disconnect();
          if(!WiFi.config(ipEstatica, compuerta, mascara, dns))
          {
            Serial.println("NO SE PUDO REALIZAR UNA CONFIGURACIÓN ESTÁTICA");
            mensaje += "<li>NO SE PUDO REALIZAR UNA CONFIGURACIÓN ESTÁTICA</li>";
          }
          WiFi.mode(WIFI_AP);
          WiFi.begin(ssid, contra);
        }
      }
      else
      {
        Serial.println("NO RECIBO RED");
      }

      //Serial.print("\nRE-CONECTANDO");
      //int intento = 0;
      //while(WiFi.status() != WL_CONNECTED)
      //{
      //  if((intento > 50) || WiFi.status() == WL_NO_SSID_AVAIL)
      //  {
      //    Serial.print("\nNo se pudo conectar a la nueva red: ");
      //    mensaje += "<li>NO SE PUDO CONECTAR A LA RED SELECCIONADA</li>";
      //    WiFi.mode(WIFI_AP);
      //    return;
      //  }
      //  Serial.print(".");
      //  delay(100);
      //  intento++;
      //}
      //IPAddress gateway;
      //IPAddress subnet;
      //subnet = WiFi.subnetMask();
      //Serial.print("\nMASCARA: ");
      //Serial.println(subnet);

      //gateway = WiFi.gatewayIP();
      //Serial.print("PUERTA DE ENLACE: ");
      //Serial.println(gateway);
      
      //mensaje += "<li>CONECTADO A NUEVA RED: ";
     // mensaje += WiFi.SSID();
     // mensaje += "</li>";
      
      //Serial.print("\nConectado a NUEVA red: ");
      //Serial.println(WiFi.SSID());
      //Serial.print("NUEVA Direccion IP: ");
      //Serial.println(WiFi.localIP());     
    }
}

void CambioMQTT()
{
  if(!server.hasArg("mqtt_server") || !server.hasArg("id_client") || !server.hasArg("usuario_mqtt") || !server.hasArg("password_mqtt") || !server.hasArg("prefijo_mqtt")
    || server.arg("mqtt_server") == NULL || server.arg("id_client") == NULL || server.arg("usuario_mqtt") == NULL || server.arg("password_mqtt") == NULL ||
    server.arg("prefijo_mqtt") == NULL)
    {
      Serial.println("NO SE HA MODIFICADO EL BROKER MQTT");
      mensaje += "<li>NO SE MODIFICÓ EL BROKER MQTT</li>";
    }
    else
    {
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


void Grabar(int dir, String dato)
{
  int espacio = dato.length(); 
  char almac[50]; 
  dato.toCharArray(almac, espacio+1);
  for (int i = 0; i < espacio; i++) {
    EEPROM.write(dir+i, almac[i]);
  }
  for (int i = espacio; i < 50; i++) {
    EEPROM.write(dir+i, 255);
  }
  EEPROM.commit();
}

String Leer(int dir)
{
   byte dato;
   String datoString;
   for (int i = dir; i < dir+50; i++) {
      dato = EEPROM.read(i);
      if (dato != 255) {
        datoString += (char)dato;
      }
   }
   return datoString;
}













void NoEncontrado()
{
  server.send(404, "text/html", pag.PMensaje("PÁGINA NO ENCONTRADA"));
}
