# 6x2-Antenna-Switch
Este proyecto trata de crear un control remoto para un cnmutador de 6 antenas para dos radios. Comunmente conocido como un 6x2
Después de probar con microcontroladores tipo Arduino, ESP32 y similares, he visto que usar una Raspberry Pi me da una solución más robusta, pudiendo usarla para más funciones además del control del conmutador de 6x2.

Para ello es necesario la parte de hardware de conmutación que sirve como ejemplo las opciones de RemoteQTH

https://remoteqth.com/six-to-two-antenna-switch.php

https://remoteqth.com/hf-switching-and-matching.php

Por otra parte una raspberry Pi con Raspbian Lite (yo he usado la versión de 32 bits pero imagino que funcionará con cualquier GNU/linux en una Raspberry Pi).

Para el control he usado dos módulos de relés de 12vdc con entrada de optoacopladores como estos, fácilmente localizables.

https://es.aliexpress.com/item/1005004496013493.html


# Instalación del software

Lo primero es instalar Raspbian lite en la Raspberry Pi. 
El instalador oficial te da opciones que habría que habilitar para facilitar la configuración.

https://www.raspberrypi.com/software/
![Raspberry Pi Instaler](https://assets.raspberrypi.com/static/36bcf234997c01a1e9722e40f43b6fd3/1783f/windows.webp)


