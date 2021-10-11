# Diseño de interfaces con DGUS

DGUS (o SGUS en el caso de las pantallas hechas por XRD Electronics) es un software que permite diseñar las interfaces que se mostrarán en el display.

Este documento se refiere a la versión `DGUS 5.10`. Existen versiones más modernas que agregan algunas características, pero por lo general los conceptos generales se mantienen igual.

Un proyecto exportado consiste de varios sets de datos:

- Imágenes
- Estado inicial de las variables
- Configuración de elementos táctiles
- Configuración de elementos variables
- Íconos
- Tipografías
- Configuración de la pantalla

# Imágenes

El principal mecanismo de la interfaz se basa en imágenes sobre las cuales se superponen distintos tipos de elementos. Cada imagen es llamada "página", y es similar a una diapositiva en una presentación de PowerPoint. Hay formas para que un botón te lleve a otra página, e incluso para animar una secuencia de páginas.

Debido a que también se usan imágenes para mostrar efectos (por ejemplo, modificar la forma o el color de un botón cuando se está pulsando), se suelen intercalan imágenes normales e imágenes "con hover/efectos" una a continuación de la otra. También se pueden incluir imágenes de las cuales solo se muestre un fragmento rectangular (como un popup) o incluso usar cierta parte de la imagen como teclado.

# Variables

Para controlar lo que se muestra en la pantalla, y reaccionar ante las acciones del usuario, se utilizan variables de 16 bits. Dependiendo del display las mismas pueden estar en regiones distintas, pero por lo general son 56 KB y están dentro del rango 0x1000 y 0x6FFF. Cada una tiene un byte alto y un byte bajo, y por lo general solo se puede acceder a ellos de forma conjunta.

Los elementos que se agreguen a la interfaz podrán hacer referencia a las distintas variables, y las mismas también podrán ser leídas y modificadas a través del puerto de comunicaciones.

Todas las variables comienzan en 0x0000. Si se desea inicializar el valor de las mismas es posible crear un archivo `22_Config.bin` e incluirlo entre los archivos del proyecto.

# Configuración de elementos táctiles

Lo que el software denomina `Touch config` se corresponderían a Botones/Widgets que se pueden tocar. Los mismos no dibujan nada en la pantalla, solo reaccionan a pulsaciones.
- Variable data input: Muestra un teclado de popup, permite escribir números
- Popup window: Define un popup, que solo puede contener basic touches. Escribe lo obtenido por esos touches en una variable
- Incremental Adjustment: Sube o baja el valor de una variable
- Slider Adjustment: Mapea posición del táctil en el valor de una variable
- RTC Setting: Abre un popup de teclado para modificar la fecha/hora
- Return Key code: Settea un valor en una variable al presionar la región
- Text input (ASCII/GBK): Abre un teclado para editar un texto
- Basic touch: Define un area de efecto, jump y key value
- Firmware settings: Ejecuta un comando interno al presionar una determinada área
	
Luego de generar el proyecto esta información se almacena en un archivo `13_TouchConfig.bin`, suelen ser secuencias de 16/32/48 bytes (según el caso), y finaliza con FFFF.

# Configuración de elementos variables

Lo que el software denomina `Variable Config` son elementos que muestran cosas en pantalla, pero no se puede interactuar con ellas.
- Variable icon: Mappea un rango de una variable en un rango de íconos. Se supone que usa interpolación lineal ajustada al más cercano. Si la variable está fuera del rango no se dibuja ningún íconos.
- Animation icon: Cicla por un rango de íconos, cuando se escribe un determinado valor en una varible. Si es otro valor NO se muestra
- Slider: Mappea valor de variable en la posición horizontal/vertical de un ícono en pantalla
- Word art: Realiza números en base a una variable, usando sets de íconos que correspondan a los dígitos `0123456789-.` en ese orden
- Image animation: Anima imágenes con período fijo hasta llegar a la última
- Icon rotation: Mapea el valor de una variable en la rotación de un ícono
- Bit variable icon: permite mostrar varios íconos en base a los distintos bits de una determinada variable. Se puede hacer que se animen si están en 1, en 0 y otras opciones
- Text Number (Data variable): Muestra N dígitos, M decimales (punto fijo) con una determinada fuente
- Text: Muestra texto sacado de variables (se elige ancho, interletrado, etc)
- RTC: Muestra algún dato de la fecha/hora (similar a text number) pero con un formato parecido al date de PHP
- Hex variable: Similar al text number pero con datos en BCD
- Trend curve: muestra datos de curvas simil osciloscopio (los datos se sacan de otra memoria de variable que es tipo FIFO y solo se puede escribir o borrar completamente)
- Basic graphic: soporta el envío de comandos básicos para dibujar en una región de la imagen (es posible enviar varios comandos uno a continuación del otro en una lista)
  - Puntos: coordenada (x,y), color
  - Líneas: color, coordenada (x,y) del punto 0, 1, .. N
  - Rectángulos: coordenada arriba izquierda, coordenada abajo derecha, color
  - Rectángulo pintado: similar al de arriba
  - Círculo: coordenada del centro, radio, color
  - Copy paste: Extrae un fragmento de otra imagen (necesita coordenadas del área de corte y posición de pegado)
  - Display de ícono (ID ícono y coordenada)
  - Llenado de área (coordenadas y color a llenar)
  - Línea vertical: Color, X, Y inicial, Y final
- Table display: dibuja varios textos alineados según configuración
- QR: genera un código QR de 45x45 o 73x73 con datos extraidos de variables
- Special apps: Depende del fabricante y el firmware cargado. Algunos dibujan un alert fill con varias areas según las distancias a los centros
- Analog clock display: Dibuja las agujas de hora/minutos/segundos según el horario que esté configurado
	
Luego de generar el proyecto esta información se almacena en un archivo `14_VariableConfig.bin`, son comandos de 32 bytes cada uno, 64 por página.
	
# Otras peculiaridades
- Lo que la interfaz muestra como `VP` es el _Variable pointer_, se refiere a qué variable influye sobre ese elemento
- Lo que la interfaz define como `SP` es una fomra de hacer que los elementos variables puedan tener atributos modificables según lo presente en esas variables
- Íconos transparentes: pare determinar el color de transparencia, el display hace un flood fill desde el pixel de la esquina superior izquierda, por lo que se asume que todos los píxeles que tienen ese valor y están conectados a esa esquina son transparentes.
- Keying: permite que se active un botón de forma remota. El valor de X se convierte en FFFF, el byte más alto de Y en el Key CODE. No tiene efecto visual. Cuando se envia dicho VALOR a la variable 0x4F, es como si se hubiera apretado el botón
- Voice: la UI a veces muestra algo para configurar voice, pero no está documentado

# Íconos y Tipografías

[Ver más detalles en este otro documento](1_fonts_icons.md)

# Configuración de la pantalla

[Ver más detalles en este otro documento](3_sounds_config.md)

# Generación de archivos y programado de la pantalla

Una vez creado el proyecto y exportados los datos (Con el botón "Generate configuration file"), para hacer que la pantalla tome los cambios es necesario realizar lo siguiente:

- Copiar la carpeta de salida DWIN_SET (XRD_TFT en el caso de SGUS) del proyecto a la raiz de una tarjeta micro SD
- Encender la pantalla con la tarjeta micro SD conectada
- Aparecerá una pantalla mostrando el progreso de descarga de datos (cada uno de los tipos de recursos)

Algunos tips para asegurar que la carga funcione bien:

- Usar imágenes cuyos dígitos comiencen con 0 si son menores a 10, ya que caso contrario se cargan mal (por ejemplo 00, 01, 02, en lugar de 0, 1, 2)
- Si los archivos que comiezan con 13_ y 14_ tienen caracteres chinos, puede que la descarga falle, por lo que se aconseja renombrarlos por 13_TouchConfig.bin y 14_VariableConfig.bin respectivamente (solo aplica para SGUS)
- Si no aparece la pantalla azul de descarga de datos, revisar que la tarjeta micro SD esté formateada en FAT32 con un tamaño de bloques de 4069 bytes. Esto quizás requiera que la tarjeta sea de menos de 32 GB, dependiendo del sistema operativo.
