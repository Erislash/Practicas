#ifndef Pagina_h
#define Pagina_h

#include "Arduino.h"


class Pagina{
    public:
        String PConfiguracion(int ph_cantidadRedes, String ph_Red[]);
        String PInicio();
        String PMensaje(String ph_mensaje);
};

#endif
