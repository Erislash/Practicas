#ifndef Pagina_h
#define Pagina_h

#include "Arduino.h"


class Pagina{
    public:
        String PConfiguracion();
        String PInicio();
        String PError(String ph_mensaje);
};

#endif
