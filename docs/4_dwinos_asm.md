# DWIN-OS, programando aplicaciones nativas en Assembler

Con el objetivo de soportar aplicaciones más complejas (no solo utilizando el sistema de interfaces generadas por DGUS), algunas pantallas soportan también ser programadas
en su propio lenguaje de programación interpretado.

Para ello, utilizan un lenguaje Assembler, con instrucciones y sintaxis muy similar a las uasdas en los microcontroladores 8051.

Para compilar programas hechos en Assembler, es posible usar el software `DWIN OS Builder 3.0.2.6`.

La memoria programable para aplicaciones soporta hasta 32764 instrucciones distintas, ya que el espacio de memoria designado para las mismas es de 256 KB. 
Debido a que todo corre en un mismo núcleo, no se pueden hacer loops infinitos, por lo que el programa debería correr en pocos milisegundos y terminar con la instrucción `END`, que devuelve el control al software de DGUS.

Memorias a las que puede acceder un programa:
	- Registros de DWIN-OS (256, de 8 bits cada uno)
	- Registros DGUS (256, 8 bit), permiten configurar detalles de la pantalla
	- RAM de DGUS (0x0000-0x6FFF), permite cambiar las variables que se muestran como datos en la interfaz
	- Memoria FLASH / espacio usado para tipografías (24 MB)
	- Buffer de curvas, el que se usa para mostrar gráficos de líneas
	
Opcodes soportados:

- Moves
	- `MOVXR` - Mueve datos entre registros y variables DGUS (y viceversa)
	- `MOVC` - Lee datos desde la memoria de código de programa 
	- `MOV` - Mueve datos entre entre uno o más registros
	- `MOVRD` - Mueve datos de registros a registros DGUS
	- `MOVDR` - Mueve datos de registros DGUS a registros
	- `MOVXL` - Mueve datos de variables DGUS a biblioteca de tipografías
	- `MOVXX` - Mueve datos entre dos variables de DGUS, apuntadas por `<R0,R1>` hasta `<R2,R3>`
	- `MOVA` - Mueve un espacio de memoria apuntado por R2 a un espacio apuntado por R3
	- `LDBR` - Settea los valores de uno o más registros a un determinado valor de 8 bits
	- `LDWR` - Carga un valor inmediato de 16 bits en dos registros 

	- `WRLINE` - escribe en la memoria de buffer de curvas
	- `ERASE` - Borra una entrada de la biblioteca de tipografías (índices permitidos 0x20-0x7F)
	
- Aritméticos
	- `ADD` - Calcula una suma de 32 bit con salida de 64 bit
	- `SUB` - Calcula una resta de 32 bit con salida de 64 bit
	- `MAC` - Calcula el MAC de 32 bit con salida de 64 bit
	- `DIV` - Calcula una división de 64 bit (con/sin redondeo)
	- `SMAC` - Calcula el MAC de 16 bit con salida de 32 bit
	- `INC` - Suma un número a un register (8/16 bit)
	- `DEC` - Resta un número a un registro (8/16 bit)
	- `ROOTLE` - Resuelve una ecuación lineal
	
	- `CRCA` - CRC-16 (ANSI)
	- `CRCC` - CRC-16 (CCITT)
	- `SUMADD` - Acumula una suma de 8 bit
	- `SUMADDC` - Acumula una suma de 8 bit con carry
	- `SUMXOR` - Calcula el xor de una secuencia de registros 
	- `HEXBCD` - Convierte hex a bcd
	- `BCDHEX` - Convierte bcd a hex
	- `ASCHEX` - Convierte una Ascii string en un hex de 64 bit
	- `TIME` - Resta tiempos (usando el RTC)
	- `ADDL14` - Agrega una display variable a la pantalla
	- `SQRT` - Calcula la raíz cuadrada (64 bit)
	
- Comunicaciones
	- `RMODBUS` - Lee datos de address, cmd, data, len usando el protocolo `Modbus` desde `com0`
	- `COMSET` - Cambia la configuración del puerto `com1`
	- `COMTXD` - Envía datos a un puerto serie (`com0`/`com1`)
	- `CPRTS` - Envía el contenido de variables de SGUS al puerto serie (`com0`/`com1`)
	- `RDXLEN` - Devuelve la cantidad de bytes presentes en la cola de recepción del puerto `com1`
	- `RDXDAT` - Lee de la cola de recepción de `com1` a registros
	- `COMTXI` - envía datos desde registros al puerto serie
	- `SCAN` - Lee un caracter de la interfaz visible en la pantalla 
	- `RD645` - Lee datos usando el protocolo `DL/T645`
	
- Comparaciones
	- `TESTS` - Revisa si dos secuencias de registros coinciden (R0 se establece a 0 en caso positivo)
	- `JB` - Salta a una posición si el N-esimo bit de un registro está setteado
	- `CJNE` - Salta a una posición si dos registros de 8 bits no son iguales
	- `JS` - Salta a una posición si un registro (de 8 bits) es menor a otro
	- `IJNE` - Salta a una posición si un registro tiene un valor distinto al especificado
	
- Manipulación de bits	
	- `EXP` - Expande el signo de un registro de 8 o 16 bits a 32
	- `LDADR` - Carga 24 bits desde la memoria de código a los registros <R5,R6,R7>
	- `AND` - AND lógica entre registros
	- `OR` - OR lógica entre registros
	- `XOR` - XOR lógica entre registros
	- `BITS` - Convierte los bits de un registro en 8 variables (cada una es un bit)
	- `BITI` - Inversa de la operación `BITS`
	- `HEXASC` - Convierte un entero de 32 bits en ASCII 
	
- Otros
	- `NOP` - No hace nada
	
- Stack
	- `EXIT` - Termina el programa, con alguna condición o posibilidad de cambiar de imagen
	- `RET` - Vuelve de una función
	- `CALL` - Llama a una función
	- `GOTO` - Salta a un punto del programa
    - `END` - Termina el programa

Pseudo assembly: para ayudar a simplificar el código, el assembler también soporta:
    - `nombreLabel:`: Define un lugar del código al que después se podrá saltar
    - `EQU`: macros que reemplazan un nombre por un valor literal
    - `DB`: define un byte o un word literal en el código, se podría leer usando la instrucción `LDADR`

Luego de compilado el programa, se genera un archivo `DWINOS_xyz.bin` que contiene el código ensamblado. Es necesario copiar dicho archivo a la misma carpeta DWIN_SET para que sea cargado.

También puede ser necesario habilitar que corran este tipo de programas, lo que se hace modificando el archivo `T5UID2_1024600.CFG` (o similar, dependiendo de la pantalla) o `CONFIG.TXT` (para las pantallas SGUS)

Ejemplo de un código sencillo que reproduce un sonido al entrar a una determinada página
```
ORG 1000H
GOTO START
NOP
NOP
NOP
ORG 1080H
START:

; Guardo en R100/101 el id de la imagen actual (registro 0x14)
LDWR R0, 0014H
MOVXR R100, 1, 1

; Guardo en R128/129 los datos de la variable 0x2000 (que va a estar en 1 si están habilitados los sonidos)
LDWR R0, 2000H
MOVXR R128, 1, 1

; Salteo la sección si el sonido no está muteado
IJNE R129, 1, SoundEnabled

; Freno todos los sonidos escribiendo en los registros correspondientes
LDWR R0, 00A0H
LDWR R30, 0000H
LDWR R32, 5000H 
MOVXR R30, 0, 2

SoundEnabled:

; Si el id de la imagen actual cambió, intento ver si hay algún sonido para reproducir
CJNE R100, R102, NewScreen
CJNE R101, R103, NewScreen
GOTO EndNewScreen

NewScreen:
; Elijo el sonido en función de la pantalla actual

; Freno el sonido actual
LDWR R30, 0000H

; Pantalla con id 14
IJNE R101, 14, Not14
LDWR R30, 0002H ; Elijo el sonido que arranca en la posición 0x00
Not14:

; Pantalla con id 16
IJNE R101, 16, Not16
LDWR R30, 0402H ; Elijo el sonido que arranca en la posición 0x04
Not16:

; Reproduzco el sonido elegido
LDWR R0, 00A0H
LDWR R32, 5000H
MOVXR R30, 0, 2

EndNewScreen:

; Muevo a R102/103 el id de la imagen actual
MOV R100, R102, 2

END
```
