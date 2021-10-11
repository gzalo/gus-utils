# Creación de tipografías e íconos

Como parte de las interfaces, es posible usar tipografías. Hay dos tipos: la tipografía 0 (que es una especial que contiene varios tamaños de fuente) y las otras, que solo representan un único tamaño. La relación entre el ancho y el alto siempre es 1:2, es decir, son el doble de alto que de ancho.

Para generar estas fonts es posible usar dos tools que suelen ser provistas por el fabricante:
- `DW_0Font.exe` para generar la tipografía 0 
- `ttfFont.exe` para generar las otras tipografías

Las tipografías soportadas son monocromáticas (no tienen antialiasing) y el ancho de los caracteres es uniforme (monoespaciada) por lo que la calidad gráfica no es exactamente la mejor. Debido a estas limitaciones, es recomendable usar íconos como textos o números, siempre que sea posible. El único caso en el que no se podrían usar es cuando se desea mostrar un texto generado dinámicamente, cuyos valores no se sepan de antemano.

Los íconos son similares a las tipografías, pero soportan colores, y pueden ser más grandes (hasta 1023x1023, igual el soporte depende de la versión de DGUS usada). Es posible usar cualquier imagen.

El caso típico de uso de íconos es mostrar un texto dinámico en función de una variable elegida. Para realizarlos es necesario realizar una imagen por cada texto que se desee mostrar, y que estén ubicados en la misma posición. Para ello se puede usar cualquier editor de imágenes, asegurándose que el color de fondo sea similar al color de fondo de la imagen sobre la cual se incrustará el ícono, para lograr que el efecto de transparencia funcione correctamente.

Es posible usar la tool [Genico](https://genico.gzalo.com/) para generar imágenes númericas que se puedan convertir en un ícono, para mostrar duraciones de tiempos, fechas y horas o números en general.

## Datos internos de cada archivo

Estos datos se obtuvieron mediante ingeniería inversa, y no es necesario conocerlos para usar la pantalla

### 0_Font.HZK
	
- Data: width dividido en bits
- Por cada imagen usa ceil(width/8)*height bytes
- Rango tipografías:	
- Tamaños: (i)*(i*2)
- i va de 4 a 64 inclusive
- Caracteres del 0 al 127 inclusive

- Peso total esperado: 3082752 bytes (2.93 MB), muy comprimibles
	
### 36_Font.DZK

El ID debe ser >=24, el resto está "reservado" por la primera fuente.

Similar al anterior, pero un solo tamaño

### 32_Ico0.ICO

256KB de headers (max imágenes 16k)

Por cada ícono se agregan 8 bytes con los siguientes datos:
	- 0: width, (0 si es 255?)
	- 1: height (0 si es 255?)
	- 2,3,4,5: offset al archivo (en WORDS respecto de inicio del archivo)
	- 6,7: Color del pixel 0,0 (probablemente se usa para transparencia?)
	
Las imágenes se guardan pegadas a partir de los 256KB, y pegadas (cada una usa W*H*2 bytes)
