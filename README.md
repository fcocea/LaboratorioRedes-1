# Transferencia de archivos por TCP 

## Introducción
El objetivo de este proyecto es realizar la transferencia de archivos entre un cliente y un servidor a través de TCP. El cliente puede enviar archivos encriptados o no encriptados, y el servidor puede desencriptar los archivos encriptados. Para la encriptación de los archivos se utiliza la librería [libsodium](https://libsodium.gitbook.io/doc/).
## Requisitos
Los requisitos para poder ejecutar el proyecto son los siguientes:
* CMake 3.10
* Make
* C++ 11
* Libsodium

### Instalación de Libsodium
En sistemas tipo Debian, es posible instalar libsodium con el siguiente comando:
```bash
$ sudo apt-get install libsodium-dev
```
Por otra parte en basados en Arch, es posible instalar libsodium con el siguiente comando:
```bash
$ sudo pacman -S libsodium
```
Para otros sistemas operativos, consultar la [documentación](https://libsodium.gitbook.io/doc/installation).

## Compilación
Para instalar y compilar el proyecto es necesario ejecutar los siguientes comandos:
```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```
## Uso
Para iniciar el servidor es necesario especificar el puerto en el que se va a escuchar. El servidor se ejecuta de la siguiente manera:
```bash
$ ./server -p <puerto>
```
> Para temas prácticos se trabaja en la misma red local, sin embargo, es posible realizar la transferencia de archivos a través de internet, siempre y cuando se conozca la dirección IP del servidor y se realicen las modificaciones pertinentes

El cliente puede realizar el envío de archivos al servidor de una manera encriptada o no encriptada. Dependiendo de los parámetros que se le pasen al cliente, se ejecuta de la siguiente manera:
```bash
$ ./client -p <puerto> -f <archivo> [-e]
```
donde `-e` corresponde a un parámetro opcional y especifica que el archivo se enviará encriptado. 

> Se espera que el cliente y el servidor se ejecuten en directorios distintos para evitar conflictos con los archivos generados.

La generación de clave es automática y única para cada solicitud de envío de archivo y almacenada en el mismo directorio de ejecución del cliente, siguiendo el formato `<archivo>.key`. Es necesario compartir esta clave con el servidor para que este pueda desencriptar el archivo.

Es posible realizar la desencriptación de un archivo encriptado con el siguiente comando:
```bash
$ ./decrypt -f <archivo> -k <clave>
```
Se almacenará el archivo desencriptado con el mismo nombre que el archivo encriptado.

> Tener en cuenta que la encriptación y desencriptación es más lenta según el tamaño del archivo.
### Créditos
Proyecto realizado por:
* Francisco Cea
* Oscar Castillo
* Matías Gayoso




