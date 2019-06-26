#include <ESP8266mDNS.h>
#include <Pagina.h>                   //LIBRERIA DE LAS WEBS
#include <ESP8266WiFi.h>              //LIBRERIAS DEL NODE
#include <ESP8266WebServer.h>

String ssid = "TP-LINK_33976C";    //TP-LINK_33976C          //NOMBRE DE LA RED WIFI A CONECTAR
String contra = "";           //CONTRASEÑA DE LA RED
Pagina pag;                           //INICIALIZACIÓN DEL OBJETO PAGINA





MDNSResponder mdns;







ESP8266WebServer server(80);          //INICIALIZACION DEL OBJETO SERVIDOR
String usuario = "Usuario";           //USUARIO PARA ACCEDER A LA PÁGINA DE CONFIGURACIÓN
String pass = "Usuario";           //CONTRASEÑA PARA ACCEDER




String mensaje = "";                  //VARIABLE DONDE SE GUARDARA EL MENSAJE MOSTRADO LUEGO DE APLICAR CAMBIOS


void setup()
{
  Serial.begin(115200);               //INICIO EL MONITOR SERIE
  //WiFi.begin(ssid, contra);           //CONECTO A WIFI


  WiFi.mode(WIFI_AP);

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



  //Serial.print("\nCONECTANDO");
  //while(WiFi.status() != WL_CONNECTED)  //HASTA QUE NO CONECTE IMPRIMO PUNTOS
  //{
   // Serial.print(".");
   // delay(500);
  //}
  //Serial.print("\nConectado a red: ");
 // Serial.println(WiFi.SSID());
 // Serial.print("Direccion IP: ");
 // Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, Acceso);    //INDICO LA FUNCIÓN A LLAMAR DEPENDIENDO DE LA PÁGINA ACCEDIDA
  server.on("/entrar", HTTP_POST, Congifuracion);
  server.on("/cambioRed", HTTP_POST, Cambio);
  server.onNotFound(NoEncontrado);




  if(mdns.begin("node2073", WiFi.localIP()))
        Serial.println("DNS ESTABLECIDO");
      else
        Serial.println("DNS NO ESTABLECIDO");



  
  server.begin();                       //INICIO EL SERVIDOR


  MDNS.addService("http", "tcp", 80);
  
}

void loop()
{
  server.handleClient();                  //USO LA FUNCION HANDLECLIENT DE LA CLASE SERVER PARA QUE SE OCUPE DE ADMINISTRAR LAS PÁGINAS
}

void Acceso()
{
  //ENVÍO LA PÁGINA DE ACCESO AL SERVIDOR
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
   
  }else
  {
    server.send(401, "text/html", pag.PMensaje("USUARIO O CONTRASEÑA INVÁLIDOS"));
  }
}

void Cambio()
{
  CambioRed();
  CambioAcceso();
  server.send(200, "text/html", pag.PMensaje(mensaje).c_str());
  mensaje = "";
}


void CambioAcceso()
{
  if (!server.hasArg("usuario_web") || !server.hasArg("password_web") || server.arg("usuario_web") == NULL || server.arg("password_web") == NULL)
    {
      mensaje += "<li>NO SE MODIFICO EL USUARIO NI CONTRASEÑA<li>";
      //server.send(200, "text/html", pag.PMensaje("NO SE PUDO MODIFICAR EL USUARIO O CONTRASEÑA"));
      return;
    }else
    {
      usuario = server.arg("usuario_web");
      pass = server.arg("password_web");
      Serial.println("USUARIO Y CONTRASEÑA MODIFICADOS");
      mensaje += "<li>USUARIO Y CONTRASEÑA MODIFICADOS</li>";
      //server.send(200, "text/html", pag.PMensaje("USUARIO Y CONTRASEÑA MODIFICADOS"));
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

      ssid = nSSID;
      contra = ncontra;
      

      
      if(server.arg("ip") == "dinamica")
      {
        Serial.println("SELECCIONADA RED DINAMICA");
        WiFi.disconnect();
        WiFi.begin(ssid, contra);
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
          mensaje += "<li>CONFIGURANDO IP ESTÁTICA</li>";

          Serial.println(ipEstatica);
          Serial.println(compuerta);
          Serial.println(mascara);
          Serial.println(dns);
          WiFi.disconnect();
          if(!WiFi.config(ipEstatica, compuerta, mascara, dns))
          {
            Serial.println("NO SE PUDO REALIZAR UNA CONFIGURACIÓN ESTÁTICA");
            mensaje += "NO SE PUDO REALIZAR UNA CONFIGURACIÓN ESTÁTICA";
          }
          WiFi.mode(WIFI_AP);
          WiFi.begin(ssid, contra);
        }

        
      }
      else
      {
        Serial.println("NO RECIBO RED");
      }


      Serial.print("\nRE-CONECTANDO");
      int intento = 0;
      while(WiFi.status() != WL_CONNECTED)  //HASTA QUE NO CONECTE IMPRIMO PUNTOS
      {
        if((intento > 30) || WiFi.status() == WL_NO_SSID_AVAIL)
        {
        //  WiFi.begin(ssid, contra);
        //  while(WiFi.status() != WL_CONNECTED)
          //{
      //      Serial.print(".");
    //        delay(500);
  //        }
//
          Serial.print("\nNo se pudo conectar a la nueva red: ");
          mensaje += "<li>NO SE PUDO CONECTAR A LA RED SELECCIONADA</li>";
          WiFi.mode(WIFI_AP);
//
      //    Serial.println("\nRE-CONECTADO A RED ANTERIOR: ");
     //     Serial.println(WiFi.SSID());
      //    Serial.println(WiFi.localIP());
      //    server.send(400, "text/html", pag.PMensaje("No se pudo conectar a la red establecida. Conectado A Red anterior"));
          return;
        }
        Serial.print(".");
        delay(500);
        intento++;
     
      }
      IPAddress gateway;
      IPAddress subnet;
      subnet = WiFi.subnetMask();
      Serial.print("\nMASCARA: ");
      Serial.println(subnet);

      gateway = WiFi.gatewayIP();
      Serial.print("PUERTA DE ENLACE: ");
      Serial.println(gateway);
      
      mensaje += "<li>CONECTADO A NUEVA RED: ";
      mensaje += WiFi.SSID();
      mensaje += "</li>";
      
      Serial.print("\nConectado a NUEVA red: ");
      Serial.println(WiFi.SSID());
      Serial.print("NUEVA Direccion IP: ");
      Serial.println(WiFi.localIP());     
    }
}


void NoEncontrado()
{
  server.send(404, "text/html", pag.PMensaje("PÁGINA NO ENCONTRADA"));
}
