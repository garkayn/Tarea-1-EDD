# Filtros de imagen y conversión ASCII en C++

Programa de consola para aplicar transformaciones a una imagen PNG. Implementa espejo horizontal, rotación, atenuación, umbral de luminosidad y conversión de la imagen a arte ASCII.

## Requisitos y ejecución

Se requiere un compilador C++11 o posterior. Las cabeceras `stb_image` y `stb_image_write` están incluidas.

```bash
g++ -std=c++17 -O2 tarea_1.cpp -o tarea_1
./tarea_1
```

En Windows, ejecuta `tarea_1.exe`. Corre el programa desde esta carpeta: `Blaziken.png` es la imagen de entrada. El programa solicita los valores para atenuación, umbral y caracteres ASCII, y genera archivos `operacion_*.png`.

## Detalles

La operación 4 calcula un umbral por luminosidad del píxel. También se conserva `operacion_4_error` como comparación didáctica: umbraliza cada canal por separado y puede distorsionar los colores.

Trabajo académico de Estructuras de Datos. Revisa el enunciado y conserva los créditos originales antes de reutilizarlo.
