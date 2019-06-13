#include <Pagina.h>                   //LIBRERIA DE LAS WEBS
#include <ESP8266WiFi.h>              //LIBRERIAS DEL NODE
#include <ESP8266WebServer.h>

String ssid = "NOMBRE DE LA RED";              //NOMBRE DE LA RED WIFI A CONECTAR
String contra = "CONTRASEÑA";           //CONTRASEÑA DE LA RED
Pagina pag;                           //INICIALIZACIÓN DEL OBJETO PAGINA


ESP8266WebServer server(80);          //INICIALIZACION DEL OBJETO SERVIDOR

void Principal();                     //DECLARACIÓN DEL PROTOTIPO DE LAS FUNCIONES DEL SERVIDOR
void Login();
void Error();


String usuario = "Usuario";           //USUARIO PARA ACCEDER A LA PÁGINA DE CONFIGURACIÓN
String pass = "Usuario123";           //CONTRASEÑA PARA ACCEDER


void setup()
{
  Serial.begin(115200);               //INICIO EL MONITOR SERIE
  WiFi.begin(ssid, contra);           //CONECTO A WIFI

  Serial.print("\nCONECTANDO");
  while(WiFi.status() != WL_CONNECTED)  //HASTA QUE NO CONECTE IMPRIMO PUNTOS
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nConectado a red: ");
  Serial.println(WiFi.SSID());
  Serial.print("Direccion IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, Acceso);    //INDICO LA FUNCIÓN A LLAMAR DEPENDIENDO DE LA PÁGINA ACCEDIDA
  server.on("/entrar", HTTP_POST, Congifuracion);
  server.onNotFound(NoEncontrado);

  server.begin();                       //INICIO EL SERVIDOR
  Serial.println("\nEL SERVIDOR HA INICIADO");
}

void loop()
{
  server.handleClient();                  //USO LA FUNCION HANDLECLIENT DE LA CLASE SERVER PARA QUE SE OCUPE DE ADMINISTRAR LAS PÁGINAS
}

void Acceso()
{
  //ENVÍO LA PÁGINA DE ACCESO AL SERVIDOR
   server.send(200, "text/html", pag.PInicio()); 
}

void Congifuracion()
{
  //COMPRUEBO SI EL ACCESO ES CORRECTO
  if(!server.hasArg("usuario") || !server.hasArg("password") || server.arg("usuario") == NULL || server.arg("password") == NULL)
  {
    server.send(400, "text/html", pag.PError("ACCESO INCOMPLETO"));
    return;
  }
  if(server.arg("usuario") == usuario && server.arg("password") == pass)
  {
    //SI USUARIO Y CONTRASEÑA SON CORRECTOS, ENVÍO LA PÁGINA DE CONFIGURACIÓN
    server.send(200, "text/html", pag.PConfiguracion());
    
  }else
  {
    server.send(401, "text/html", pag.PError("USUARIO O CONTRASEÑA INVÁLIDOS"));
  }
}

void NoEncontrado()
{
  server.send(404, "text/html", pag.PError("PÁGINA NO ENCONTRADA"));
}
