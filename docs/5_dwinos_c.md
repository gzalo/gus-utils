# DWIN-OS, programando aplicaciones nativas en C

De forma similar al caso anterior, DWIN también provee proveen un compilador de C, que hace posible utilizar un lenguaje de más alto nivel (como lo es C) para programar
comportamiento en las pantallas HMI.

Esta característica puede ser muy útil para realizar comunicaciones con otros dispositivos, teniendo en cuenta que los displays tienen puertos serie adicionales.

Para compilar ese programa puede usarse el `DWIN C Compiler 1.1.0.7`.

El mismo no posee soporte para la biblioteca estándar de C (libc), pero tiene unas bibliotecas adicionales (en los archivos `00.LIB` y `01.LIB`, que deberán ser copiados a la tarjeta microSD junto al código compilado) para poder realizar distintas operaciones que afecten a las variables y memoria del LCD. Por ejemplo,

- `void read_dgus_vp(int dgus_vp,char *buf,int len)`
- `void write_dgus_vp(int dgus_vp,char *buf,int len)`
- `void read_register(int register_num, char *buf)`
- `void write_register(int register_num, char *buf)`
(y varias funciones más, que de fondo llaman a las instrucciones correspondientes que realizan esas operaciones)

El compilador y la arquitectura tiene algunas limitaciones que es necesario tener en cuenta:
- No soporta strings constantes, es necesario declararlas como parte de un array local
- No soporta alocaciones en heap (malloc/free), por lo que todas las variables deberán ser globales o locales
- El stack es muy limitado (256 byes o 17 niveles) y el espacio para variables también (max 1792 bytes)
- El código no puede ser tan largo, ya que el máximo es 32KB
- No hay soporte para double, solo float, y es más lento
- Al contrario que cuando se programa en assembler, en este caso se pueden realizar loops infinitos si se desea procesar datos continuamente y no solo al principio de un programa

Ejemplo de aplicación básica escrita en C:

```c

int getSingleDgusRegister(int reg){
	char out[2] = {0,0};
	read_dgus_vp(reg, out, 1);
	return (out[0] << 8) | out[1];
}
void setSingleDgusRegister(int reg, int val){
	char out[2] = {val >> 8, val & 0xFF};
	write_dgus_vp(reg, out, 1);
}

void sendMessage(){
	char data_serial[18] = "Give me the key\r\n";
	send_data_com(4,data_serial,17);
}

void changeScreen(int screen){
	char out[4] = {0x5A, 0x01, screen>>8, screen&0xFF};
	write_dgus_vp(0x0084, out, 2);
}

void Delayms (unsigned int xms) {
	unsigned int i, j;
	for (i = xms; i> 0; i--)
		for (j = 110; j> 0; j--);
}

int currScreen = 0, prevScreen = 0, soundMuted = 0;

int main(){
	com_config(4,0,115200);
	
	while(1){
		currScreen = getSingleDgusRegister(0x0014);
		soundMuted = getSingleDgusRegister(0x2000);
		
		// Screen change trigger
		if(currScreen != prevScreen){			
			if(currScreen == 28 && soundMuted == 1){
				sendMessage();
			}
		}
				
		// Store current screen ID
		prevScreen = currScreen;		
		Delayms(100);
	}
	
	return 0;
}

void Time1_IRQHandler(void){
}
void Time2_IRQHandler(void){
}
void Time0_IRQHandler(void){
}
```
