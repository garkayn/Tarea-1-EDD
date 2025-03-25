#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

// Importacion de librerias. Los archivos correspondientes deben estar en el mismo directorio que el .cpp.
#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>

using namespace std;

// Struct que almacena la data de una imagen
struct Imagen{
    unsigned char* data;
    int width, height, channels;
};

// Abrir imagen desde memoria local
Imagen* load(const char* filename) {
    Imagen* img = new Imagen();
    img->data = stbi_load(filename, &img->width, &img->height, &img->channels, 0);
    return img;
}

// Guardar imagen en memoria local
void save(Imagen* img, const char* filename) {
    stbi_write_png(filename, img->width, img->height, img->channels, img->data, img->width * img->channels);
    cout << "Imagen guardada:  " << filename << "\n";
}

void operacion_1(Imagen* img) {  
    unsigned char* p1 = new unsigned char();

    for(int i = 0; i < (img->height); i++){    
        for(int j = 0; j < (img->width/2); j++){
            int id= i * img->width * img->channels + (img->width * img->channels -j * img->channels- img->channels);
            int ii= i * img->width * img->channels  + j * img->channels;
            for(int k = 0; k<img->channels;k++){
                *p1 = img->data[id + k]; //Esto lo recorre de derecha a izquierda
                img->data[id+k] = img->data[ii+k]; //Esto lo recorre de izquierda a derecha
                img->data[ii + k] = *p1;
            }
        }
    }
    delete p1;
}

void operacion_2(Imagen* img){
    
}

unsigned char *guardar_pixeles(Imagen* img){
    unsigned char* p_pixel = new unsigned char[(img->height * img->channels * img->width)];
    for(int i = 0; i<(img->height * img->width);i++){
        unsigned char* p_canal = new unsigned char[(img->channels)];
        for(int j=0; j<img->channels; j++){
            p_canal[j] = img->data[j + i*img->channels];
        }
        p_pixel[i] = *p_canal;
    }
    return p_pixel;
}
    

int main() {
    //Cargamos la imagen
    Imagen* img = load("Pikachu.png");
    
    operacion_1(img);

    unsigned char *a = guardar_pixeles(img);
    for (int i = 0; i < img->height * img->channels * img->width; i++) {
        cout << a[i];
    }

    // Almacenamos el resultado
    save(img, "output.png");
}
//Para crear un puntero que apunte a la direccion base del arreglo es de la forma
//tipo_de_dato nombre_del_puntero = new array[largo]
//Si creo este puntero dentro de una función, el array no se borra de la memoria, no funciona como memoria local,
//Sin embargo el puntero si, que hay que siosi retornarlo, no seai won