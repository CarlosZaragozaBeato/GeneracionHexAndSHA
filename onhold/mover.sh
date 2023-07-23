
#!/bin/bash

# Carpeta que contiene los archivos txt a procesar
folderPath="./archivos1/"

# Directorio base donde se encuentran las carpetas creadas anteriormente
basePath="./sha/"

# Recorre todos los archivos txt en la carpeta dada
for file in "${folderPath}"*.txt; do
    if [[ -f "$file" ]]; then
        # Obtener el título del archivo txt (sin la extensión)
        title=$(basename -- "$file" .txt)

        # Obtener las dos primeras letras del título
        firstTwoChars="${title:0:2}"

        # Directorio destino para mover el archivo
        destinationFolder="${basePath}${firstTwoChars}/"

        # Verificar si la carpeta destino existe, si no, se crea
        if [ ! -d "$destinationFolder" ]; then
            mkdir -p "$destinationFolder"
        fi

        # Mover el archivo al directorio destino
        mv "$file" "${destinationFolder}"
    fi
done
