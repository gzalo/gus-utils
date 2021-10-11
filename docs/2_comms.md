# Transferencia de datos y comunicación con un MCU

El módulo soporta un puerto serie asíncrono típico (con 1 bit de start, 8 de datos y uno de stop), para transferir datos desde y hacia la pantalla. La velocidad de transferencia puede ser configurada a los valores típicos como 9600 o 115200 bits por segundo ([ver más detalles](3_sounds_config.md)). Todos los valores que contengan más de un byte son transferidos desde el byte más significante al menor.

[Aquí puede verse](../code/lcd.c) un pequeño ejemplo de una biblioteca para manejar estas pantallas desde un microcontrolador de la serie PIC18, recibiendo y enviando datos cuando sea necesario.

Los datos enviados (en ambos sentidos) se componen de 5 partes:

- Header: dos bytes con un valor fijo, por lo general 0x5A 0xA5 pero puede cambiarse desde la configuración
- Largo: un byte indicando el largo de los próximos datos a enviar/recibir (hasta 254, no incluye los dos bytes del header ni el byte de este campo)
- Comando: un byte entre 0x80 y 0x84 indicando el comando 
- Datos: N bytes en función del comando
- Checksum CRC: dos bytes opcionales con el checksum del comandos y datos

# Comandos soportados

- 0x80: escribe en uno o más registros contiguos. Datos requeridos:
  - 1 byte: dirección del registro inicial
  - N bytes: datos a escribir
- 0x81: lee de uno o más registros contiguos. Datos requeridos:
  - 1 byte: dirección del registro inicial
  - 1 byte: cantidad de registros a leer
  - Cuando el módulo contesta, escribe una secuencia similar pero con `0x81 0xLL 0xD0 0xD1 0xD2...` donde LL es la cantidad de registros leída y D0 a DL son los datos leidos
- 0x82: escribe en la memoria de variables de DGUS. Datos requeridos:
  - 2 bytes: Addr inicial (primero byte alto, luego bajo)
  - 2*N bytes: datos a escribir
- 0x83: lee de la memoria de variables de DGUS. Datos requeridos:
  - 2 bytes: Addr inicial (primero byte alto, luego bajo)
  - 1 bytes: cantidad de variables a leer (hasta 0x7F = 127 variables)
  - Cuando el módulo responde, escribe una secuencia similar pero con `0x83 0xLL 0xV0H 0xV0L 0xV1H 0xV1L...` donde LL es la cantidad de variables leídas, y `V0H V0L` son las partes altas y bajas de cada variable leída.
- 0x84: escribe en la memoria de curvas. Recibe un flag indicando qué canal se modificó y los datos de cada uno de ellos

Si se desea acceder a la memoria flash, de audio y la de tipografías, será necesario hacerlo a través de sus registros. Dicho proceso no se incluyen en este documento, pero se puede realizar sin problemas.
