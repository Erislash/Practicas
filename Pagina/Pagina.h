#ifndef Pagina_h
#define Pagina_h

#include "Arduino.h"


class Pagina{
    public:
        String PConfiguracion(String ph_Redes);
        String PInicio();
        String PMensaje(String ph_mensaje);
};

#endif
