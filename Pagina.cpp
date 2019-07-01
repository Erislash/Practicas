#include "Arduino.h"
#include "Pagina.h"

String Pagina::PConfiguracion()
{
    String html ="<!DOCTYPE html> <html lang=\"es\"> <head> <meta charset=\"UTF-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\"> <link rel=\"stylesheet\" href=\"CSS/Estilos.css\"> <title>Portal de Configuracion</title> </head> <style> * { margin: 0; padding: 0; box-sizing: border-box; } .contenedor { overflow: hidden; width: 90%; margin: 0 auto; } .contenedor .titulo { color: #AF0F0F; width: 100%; text-align: center; border-bottom: 1px solid #000; line-height: 0.1em; margin-top: 30px; } .contenedor .titulo span { background: #fff; padding: 0 10px; } .contenedor .formulario { overflow: hidden; margin: 20px auto; display: flex; justify-content: center; padding: 30px; width: 70%; border: 3px double #AF0F0F; } .contenedor .formulario form { width: 50%; } .contenedor .formulario form .seccion { padding: 10px 0; } .contenedor .formulario form .selector-ip { display: flex; padding: 10px; justify-content: space-around; } .contenedor .formulario form .selector-ip input{ display: none; } .contenedor .formulario form .selector-ip label{ background: rgb(231, 12, 12); color: #FFF; padding: 10px 20px; cursor: pointer; opacity: 0.5; } .contenedor .formulario form .selector-ip input[type=\"radio\"]:checked + label{ background: rgb(70, 8, 8); opacity: 1; } .contenedor .formulario form .seccion hr { margin-bottom: 10px; color: #AF0F0F; } .contenedor .formulario form .seccion .campo { display: flex; align-items: center; flex-direction: row; justify-content: space-between; } .contenedor .formulario form .seccion .campo .texto{ margin: 5px 0; padding: 3px 7px; } .contenedor .formulario form .seccion .campo .sta { width: 20%; -webkit-appearance: textfield; -moz-appearance:textfield; } .contenedor .formulario form .seccion .campo .sta::-webkit-inner-spin-button { -webkit-appearance: none; margin: 0; } .contenedor .formulario form .seccion .est { display: flex; flex-direction: row; } .contenedor .formulario form .botones { margin-top: 15px; display: flex; justify-content: space-around; } .contenedor .formulario form .botones .boton { display: none; } .contenedor .formulario form .boton + label { background: #AF0F0F; padding: 10px 20px; color: #FFF; cursor: pointer; transition: all ease 0.4s; } .contenedor .formulario form .boton + label:hover { background: #490606; } ";
    html +="@media (max-width: 950px) { .contenedor { width: 100%; } .contenedor .titulo { color: #AF0F0F; width: 100%; border-bottom: none; } .contenedor .formulario { width: 90%; } } ";
    html +="@media (max-width: 650px) { .contenedor .titulo { text-align: left; } .contenedor .formulario { margin-top: 0px; border: none; width: 100%; } .contenedor .formulario form { width: 80%; } .contenedor .formulario form .seccion h2 { text-align: center; } .contenedor .formulario form .seccion .campo { padding: 5px 10px; } } ";
    html +="@media (max-width: 415px) { h2{ font-size: 20px; } body{ font-size: 11px; } .contenedor { width: 100%; } .contenedor .formulario form { width: 100%; } } </style> <body> <div class=\"contenedor\"> <h1 class=\"titulo\"><span>CONFIGURACIÓN</span></h1> <div class=\"formulario\"> <form action=\"/cambio\" method = \"POST\"> <section class = \"seccion\"> <h2>Configuracion de Red WiFi</h2> <hr> <div class=\"campo\"> <label for=\"ssid_ap\">SSID:</label> <input class = \"texto\" type=\"text\" name=\"ssid_ap\" id = \"ssid_ap\" maxlength=\"20\" placeholder=\"SSID\"> </div> <div class=\"campo\"> <label for=\"password_ap\">Contraseña:</label> <input placeholder = \"Contraseña\" type=\"password\" class = \"texto\" id = \"password_ap\" name=\"password_ap\"> </div> <div class=\"selector-ip\" id=\"selector\"> <input type=\"radio\" name=\"ip\" value=\"dinamica\" id=\"btn_dinamica\" checked> <label for=\"btn_dinamica\">Dinámica</label> <input type=\"radio\" name=\"ip\" value=\"estatica\" id=\"btn_estatica\"> <label for=\"btn_estatica\">Estática</label> </div> <article class = \"seccion estatico\" id=\"estatico\"> <label for=\"ip_est\">IP:</label> <div class=\"campo est\"> <input oninput=\"javascript: if (this.value.length > 3) this.value = this.value.slice(0, 3);\" class = \"texto sta\" type=\"number\" name=\"ip_est_1\" id = \"ip_est\" placeholder=\"IP\"> <input oninput=\"javascript: if (this.value.length > 3) this.value = this.value.slice(0, 3);\" class = \"texto sta\" type=\"number\" name=\"ip_est_2\" id = \"ip_est\" placeholder=\"IP\"> <input oninput=\"javascript: if (this.value.length > 3) this.value = this.value.slice(0, 3);\" class = \"texto sta\" type=\"number\" name=\"ip_est_3\" id = \"ip_est\" placeholder=\"IP\"> <input oninput=\"javascript: if (this.value.length > 3) this.value = this.value.slice(0, 3);\" class = \"texto sta\" type=\"number\" name=\"ip_est_4\" id = \"ip_est\" placeholder=\"IP\"> </div> <label for=\"mascara_est\">Máscara:</label> <div class=\"campo est\"> <!-- <label for=\"mascara_est\">Máscara:</label> --> <input oninput=\"javascript: if(this.value.length > 3) this.value = this.value.slice(0, 3);\" class = \"texto sta\" type=\"number\" name=\"mascara_est_1\" id = \"mascara_est\" maxlength=\"20\" placeholder=\"Máscara\"> <input oninput=\"javascript: if(this.value.length > 3) this.value = this.value.slice(0, 3);\" class = \"texto sta\" type=\"number\" name=\"mascara_est_2\" id = \"mascara_est\" maxlength=\"20\" placeholder=\"Máscara\"> <input oninput=\"javascript: if(this.value.length > 3) this.value = this.value.slice(0, 3);\" class = \"texto sta\" type=\"number\" name=\"mascara_est_3\" id = \"mascara_est\" maxlength=\"20\" placeholder=\"Máscara\"> <input oninput=\"javascript: if(this.value.length > 3) this.value = this.value.slice(0, 3);\" class = \"texto sta\" type=\"number\" name=\"mascara_est_4\" id = \"mascara_est\" maxlength=\"20\" placeholder=\"Máscara\"> </div> <label for=\"puerta_est\">Puerta De Enlace:</label> <div class=\"campo est\"> <input oninput=\"javascript: if(this.value.length > 3) this.value = this.value.slice(0, 3);\" class = \"texto sta\" type=\"number\" name=\"puerta_est_1\" id = \"puerta_est\" maxlength=\"20\" placeholder=\"Gateway\"> <input oninput=\"javascript: if(this.value.length > 3) this.value = this.value.slice(0, 3);\" class = \"texto sta\" type=\"number\" name=\"puerta_est_2\" id = \"puerta_est\" maxlength=\"20\" placeholder=\"Gateway\"> <input oninput=\"javascript: if(this.value.length > 3) this.value = this.value.slice(0, 3);\" class = \"texto sta\" type=\"number\" name=\"puerta_est_3\" id = \"puerta_est\" maxlength=\"20\" placeholder=\"Gateway\"> <input oninput=\"javascript: if(this.value.length > 3) this.value = this.value.slice(0, 3);\" class = \"texto sta\" type=\"number\" name=\"puerta_est_4\" id = \"puerta_est\" maxlength=\"20\" placeholder=\"Gateway\"> </div> <label for=\"dns_est\">DNS:</label> <div class=\"campo est\"> <input oninput=\"javascript: if (this.value.length > 3) this.value = this.value.slice(0, 3);\" class = \"texto sta\" type=\"number\" name=\"dns_est_1\" id = \"dns_est\" maxlength=\"20\" placeholder=\"DNS\"> <input oninput=\"javascript: if (this.value.length > 3) this.value = this.value.slice(0, 3);\" class = \"texto sta\" type=\"number\" name=\"dns_est_2\" id = \"dns_est\" maxlength=\"20\" placeholder=\"DNS\"> <input oninput=\"javascript: if (this.value.length > 3) this.value = this.value.slice(0, 3);\" class = \"texto sta\" type=\"number\" name=\"dns_est_3\" id = \"dns_est\" maxlength=\"20\" placeholder=\"DNS\"> <input oninput=\"javascript: if (this.value.length > 3) this.value = this.value.slice(0, 3);\" class = \"texto sta\" type=\"number\" name=\"dns_est_4\" id = \"dns_est\" maxlength=\"20\" placeholder=\"DNS\"> </div> </article> </section> <section class = \"seccion\"> <h2>Modificar Usuario de Acceso</h2> <hr> <div class=\"campo\"> <label for=\"usuario_web\">Nuevo Usuario:</label> <input class = \"texto\" type=\"text\" name=\"usuario_web\" id = \"usuario_web\" maxlength=\"20\" placeholder=\"Usuario\"> </div> <div class=\"campo\"> <label for=\"password_web\">Nueva Contraseña:</label> <input placeholder = \"Contraseña\" class = \"texto\" type=\"password\" id = \"password_web\" name=\"password_web\"> </div> </section> <section class = \"seccion\"> <h2>Broker MQTT</h2> <hr> <div class=\"campo\"> <label for=\"mqtt_server\">Nombre de IP o Servidor:</label> <input class = \"texto\" type=\"text\" name=\"mqtt_server\" id = \"mqtt_server\" maxlength=\"20\" placeholder=\"Servidor MQTT\"> </div> <div class=\"campo\"> <label for=\"id_client\">ID Cliente:</label> <input class = \"texto\" type=\"text\" id = \"id_client\" name=\"id_client\" placeholder = \"ID Cliente\"> </div> <div class=\"campo\"> <label for=\"usuario_mqtt\">Usuario</label> <input class = \"texto\" type=\"text\" id = \"usuario_mqtt\" name=\"usuario_mqtt\" placeholder = \"Usuario\"> </div> <div class=\"campo\"> <label for=\"password_mqtt\">Contraseña</label> <input class = \"texto\" type=\"password\" id = \"password_mqtt\" name=\"password_mqtt\" placeholder = \"Contraseña\"> </div> <div class=\"campo\"> <label for=\"prefijo_mqtt\">Prefijo Tópicos</label> <input class = \"texto\" type=\"text\" id = \"prefijo_mqtt\" name=\"prefijo_mqtt\" placeholder = \"Prefijo Tópicos\"> </div> </section> <div class=\"botones\"> <input type=\"submit\" name = \"aplicar\" id = \"aplicar\" class=\"boton\" onclick=\"this.form.action='/aplicar'\"> <label for=\"aplicar\">Aplicar</label> <input type=\"submit\" name = \"resetear\" id = \"resetear\" class=\"boton\" onclick=\"this.form.action='/reset'\"> <label for=\"resetear\">Reiniciar</label> <input type=\"submit\" name = \"fabrica\" id = \"fabrica\" class=\"boton\" onclick=\"this.form.action='/restablecer'\"> <label for=\"fabrica\">Reestablecer</label> </div> </form> </div> </div> </body> <script> var estatico = document.getElementById('estatico'); var dinamico = document.getElementById('btn_dinamica'); var seleccionado = document.getElementById('btn_estatica'); estatico.style.display = 'none'; seleccionado.addEventListener('change', function() { if(seleccionado.checked) { estatico.style.display = 'block'; } }); dinamico.addEventListener('change', function() { if(dinamico.checked) { estatico.style.display = 'none'; } }); </script> </html>";

    return html;
}

String  Pagina::PInicio()
{
    String html ="<!DOCTYPE html> <html lang=\"es\"> <head> <meta charset=\"UTF-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\"> <title>INGRESO</title> </head> <style> * { margin: 0; padding: 0; box-sizing: border-box; } body { background: rgb(50, 50, 50); } .contenedor { margin: 0 auto; width: 90%; } .formulario { background: #AF0F0F; margin: 0 auto; width: 40%; padding: 20px; border-radius: 10px; } .formulario form h2 { color: #FFF; text-align: center; } .formulario form .seccion { display: flex; flex-direction: column; justify-content: space-between; align-items: center; text-align: center; padding: 30px; } .formulario form .seccion .campo { color: #FFF; width: 49%; display: block; } .formulario form .seccion .campo label{ display: block; } .formulario form .seccion .campo .texto { margin: 5px 0; padding: 3px 7px; border-radius: 5px;} .acceder{ display: none; } .btn-acceso{ background: #FFF; padding: 10px 20px; color: #AF0F0F; cursor: pointer; transition: all ease .4s; border-radius: 5px; } .btn-acceso:hover{ color: #FFF; background: rgb(65, 6, 6); } .acceso{ width: 100%; text-align: center; margin-bottom: 20px; } ";
    html +="@media (max-width: 1200px){ .formulario{ width: 60%; } } ";
    html +="@media (max-width: 800px){ .formulario{ width: 80%; } } ";
    html +="@media (max-width: 600px){ .formulario{ width: 90%; } .formulario form .seccion { display: flex; flex-direction: column; } .formulario form .seccion .campo { color: #FFF; width: 100%; } } ";
    html +="@media (max-width: 350px){ .formulario{ width: 95%; } .formulario form { padding: 0; } .formulario form .seccion { padding: 10px 0; } .formulario form .seccion .campo .texto { margin: 2px 0; padding: 0px 0px; border-radius: 0px; } } </style> <body> <div class=\"contenedor\"> <div class=\"formulario\"> <form action=\"/acceder\" method=\"POST\"> <h2>INGRESO</h2> <hr> <section class = \"seccion\"> <div class=\"campo\"> <label for=\"usuario\">Nombre de usuario:</label> <input class = \"texto\" type=\"text\" name=\"usuario\" id = \"usuario\" maxlength=\"20\" placeholder=\"Usuario\"> </div> <div class=\"campo\"> <label for=\"password\">Contraseña:</label> <input placeholder = \"Contraseña\" type=\"password\" class = \"texto\" id = \"password\" name=\"password\"> </div> </section> <section class=\"acceso\"> <input type=\"submit\" value=\"Acceder\" class=\"acceder\" id=\"acceder\" name=\"acceder\"> <label for=\"acceder\" class = \"btn-acceso\">ACCEDER</label> </section> </form> </div> </div> </body> </html>";
    
    return html;
}

String  Pagina::PMensaje(String ph_mensaje)
{
    String html ="<!DOCTYPE html> <html lang=\"es\"> <head> <meta charset=\"UTF-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\"> <title>Mensaje</title> </head> <style> * { padding: 0; margin: 0; box-sizing: border-box; } .contenedor { margin: 0 auto; width: 80%; display: flex; flex-direction: column; } .mensaje { margin-top: 20px; text-align: center; } button { display: none; } .volver { color:#FFF; margin: 30px auto; background: #AF0F0F; padding: 30px 50px; text-align: center; cursor: pointer; transition: all ease .5s; } .volver:hover { background: rgb(92, 9, 9); } </style> <body> <div class=\"contenedor\">";
    html += "<ul class=\"mensaje\">" + ph_mensaje + "</ul>"; 
    html += "<button onclick=\"location.href = '/';\" id=\"volver\">Volver</button> <label class=\"volver\" for=\"volver\">Volver</label> </div> </body> </html>";

    return html;
}