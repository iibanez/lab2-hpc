import commands

archivo = open("tiempos.txt","w")

for i in range(1,25):

	suma = 0
	for j in range(3):
		comando = "time ./build/mandelbrotp -i 500 -a -1 -b -1 -c 1 -d 1 -s 0.001 -f salida3.raw -t "+str(i)
		resultado = commands.getoutput(comando)
		separado = resultado.split("\t")
		suma += float(separado[1][2:7])
	
	suma = suma/3
	archivo.write(str(suma).replace(".",",")+"\n")

archivo.close()