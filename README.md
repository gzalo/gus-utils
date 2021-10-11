# Manejo de pantallas táctiles simil DGUS (DWIN) / SGUS / VGUS
## [Gonzalo Ávila Alterach](https://gzalo.com) - Enero de 2021

Algunos fabricantes que venden pantallas HMI con software y configuración similar: 
* [Beijing DWIN Technology Co., Ltd](http://www.dwin.com.cn/home/English/index.html)
* [XRD Electronics / SGUS](http://www.xrd-lcd.com/)
* [Beijing STONE Technology Co., Ltd / VGUS](http://www.stone-hmi.com/industrialtype)

## Documentación

[Diseño de interfaces con DGUS](docs/0_ui.md)

[Creación de tipografías e íconos](docs/1_fonts_icons.md)

[Transferencia de datos y comunicación con un MCU](docs/2_comms.md)

[Configuración del LCD y reproducción de sonidos](docs/3_sounds_config.md)

[(Avanzado) DWIN-OS, programando aplicaciones nativas en Assembler](docs/4_dwinos_asm.md)

[(Avanzado) DWIN-OS, programando aplicaciones nativas en C](docs/5_dwinos_c.md)

## Código de ejemplo

Dentro de la carpeta [code](code/) puede encontrarse un pequeño ejemplo de una biblioteca para manejar estas pantallas desde un microcontrolador de la serie PIC18. Está pensado para ser compilado utilizando PIC C Compiler/CCS, por lo que seguramente requirirá modificaciones para compilarlo con XC8 o en el IDE Arduino.

Advertencias: 
* El modo que agrega un checksum CRC al final de los comandos enviados y recibidos (que se puede habilitar cambiando la macro LCD_USE_CRC a 1) no fue probado, por lo que puede llegar a fallar en algunos casos.

* Algunos registros y variables pueden estar en posiciones de memoria distintas, en función del tipo y modelo de display. Antes de usar, revisar con la hoja de datos correspondiente.

## Licencia

El código de este repositorio está licenciado bajo la [Licencia MIT](https://opensource.org/licenses/MIT), por lo que puede ser utilizado y modificado para cualquier aplicación, incluyendo software propietario y comercial.

El texto e imágenes de este repositorio están licenciadas bajo la [Licencia CC BY 4.0](https://creativecommons.org/licenses/by/4.0/deed.es), por lo que se pueden utilizar y copiar libremente, siempre que se mantenga la atribución al autor original.
