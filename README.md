# Compilación

Para comenzar con el proceso de compilación se debe estar posicionado en la carpeta que contiene el archivo makefile y ejecutar el siguiente comando:

```sh
$ make
```

Se va a crear un directorio 'build' que contiene el código fuente compilado, para ejecutar se debe entrar al directorio 'build' y ejecuta el siguiente comando dependiendo de si se necesita ejecutar de manera secuencial o paralela:

Secuencial:

```sh
$ ./mandelbrot -i 500 -a -1 -b -1 -c 1 -d 1 -s 0.001 -f salida.raw
```

En donde cada punto es lo siguiente:
  -i: depth, o número máximo de iteraciones
  -a: límite inferior del componente real del plano complejo 
  -b: límite inferior componente imaginario
  -c: límite superior componente real
  -d: límitee superior componente imaginario
  -s: muestreo
  -f: archivo de salida

Paralela:

```sh
$ ./mandelbrotp -i 500 -a -1 -b -1 -c 1 -d 1 -s 0.001 -f salida.raw -t 12
```

En donde cada punto es lo siguiente:
  -i: depth, o número máximo de iteraciones
  -a: límite inferior del componente real del plano complejo 
  -b: límite inferior componente imaginario
  -c: límite superior componente real
  -d: límitee superior componente imaginario
  -s: muestreo
  -f: archivo de salida
  -t: cantidad de hebras
  
Obteniendo para estos párametros, tanto en el caso secuencial como paralelo el siguiente resultado:

![Alt text](img/ejemplo.png?raw=true "Ejemplo del conjunto mandelbrot")
