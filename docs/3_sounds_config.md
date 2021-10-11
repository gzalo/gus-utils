# Configuración del LCD y reproducción de sonidos

Los módulos LCD suelen tener una forma de configurar su comportamiento básico.

En los displays de DGUS/DWIN, por lo general el archivo de configuración es un archivo binario llamado `T5UID2.CFG` (o similar), y en el mismo se pueden configurar diversos detalles de funcionamiento como:

- Data auto uploading
- Deshabilitar tarjeta
- Hacer sonido altocar botones
- Backlight siempre encendido o que se apague después de un tiempo
- Rotación de la pantalla
- Baudrate de la comunicación
- Brillo de la pantalla y timeout del backlight
- Música cuando se enciende (segmento inicial, duración en bloques y volumen) 
- Habilitar/Deshabilitar pantalla táctil
- Detalles del hardware de la pantalla táctil y resolución del display
- Encriptado de tarjeta sd y actualización

Por lo general, en la página web del fabricante se consiguen archivos de demostración, y los mismos se pueden editar con un editor hexadecimal como HxD para adaptarlo a los usos del usuario.

Los displays de SGUS, por ejemplo llevan un archivo de texto `Config.txt` con varios "registros" de configuración uno en cada línea. No es necesario escribir todos, el valor se mantiene y solo se modifica cuando un determinado registro está presente. Es conveniente revisar la hoja de datos de cada display en particular, ya que algunos pueden tener ligeras diferencias. Por ejemplo, esta podría ser una configuración en uno de ellos (modelo `XG080SMN01N`):

- `R1=07` # settea el baudrate de la comunicación serie a 115200 bps
- `R2=08` # habilita que al tocar botones de la interfaz se envíen los datos correspondientes a través del puerto serie
- `R3=5A` # primer byte del header que se usará en las comunicaciones
- `RA=A5` # segundo byte del header que se usará en las comunicaciones
- `RD=FF`
- `STOP_DWIN_OS` # previene que se ejecuten aplicaciones paralelas programadas en assembler o C. El modo opuesto sería `RUN_DWIN_OS`
- `RC=20` # Deshabilita buzzer al tocar un elemento
- `RE=FF`

# Reproducción de sonidos

Algunas pantallas poseen la capacidad de almacenar y reproducir sonidos cortos. Por lo general ya poseen un amplificador en la misma placa, por lo que es posible conectarlo a cualquier parlante de 8 ohms de baja potencia.

Los audios se codifican en segmentos de duración fija (dependiendo del modelo, por lo general 1,024 o 2,048 segundos). Para crearlos es necesario codificar un audio existente para que sea **mono, de 16 bits y con 32 KHz de tasa de muestreo**. Para realizar eso es posible usar un programa libre y gratuito como el Audacity. Para asegurarse que no queden espacios en blanco o ruido entre sonidos, es posible rellenarlos hasta un múltiplo del tamaño del segmento.

El audio de cada segmento deberá ser codificado en WAV, y el nombre de archivo deberá ser `ID+(nombre optional).wav`. Donde el ID es el índice del segmento inicial del audio. Hay que tener en cuenta de elegir índices lo suficientemente separados para que audios largos no se solapen.

El comando para reproducir un sonidos involucra escribir en una variable especial, que depende de cada módulo. Por ejemplo, en el T5UID2 es posible escribir en 0xA0 los siguientes datos:

- Índice del primer segmento de sonido a reproducir (entre 0x00 y 0xFF)
- Cantidad de secciones a reproducir (entre 0x01 y 0xFF)
- Volumen (0-0xFF)
- 0x00 o cualquier valor (al leer este byte el módulo dice cuántos segmentos quedan pendientes reproducir)

Para dejar de reproducir un sonido es posible enviar 0x00 en los primeros dos bytes.

También es posible hacer que se reproduzca un sonido al encender la pantalla, como se describió más arriba.
