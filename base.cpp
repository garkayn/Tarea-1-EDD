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

void operacion_1() {  
    Imagen* img = load("Pikachu.png");
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
    save (img, "operacion_1.png");
    delete p1;
}

void operacion_2(){
    Imagen* img = load("Pikachu.png");
    Imagen* copy = load("Pikachu.png");
    img->width = copy->height;
    img->height = copy->width;
    img->channels = copy->channels;
    int h=img->height;
    int w=img->width;
    int c=img->channels;
    for(int i = 0; i < w; i++){
        for(int j = 0; j < h; j++){
            int indice = c*(i*h + j);
            int indice_copy = c*(w*(h - 1 - j) + i);
            for(int k=0; k<c;k++){
                img->data[indice+k] = copy->data[indice_copy+k];
            }
        }
        
    }
    save(img,"operacion_2.png");
    delete copy;
}

void operacion_3(float atenuacion){
    Imagen* img = load("Pikachu.png");
    for(int i = 0; i < img->height * img->width * img->channels; i += img->channels){
        for(int j = 0; j < 3; j++){
            img->data[i + j] *= atenuacion;
        }      
    }
    save(img,"operacion_3.png");
}

void operacion_4(int limite){
    Imagen* img = load("Pikachu.png");
    for(int i = 0; i < img->height * img->width * img->channels; i += 4){
        float NTSC = img->data[i] * 0.299 + img->data[i + 1] * 0.587 + img->data[i + 2] * 0.114;
        if(NTSC < limite){
            for(int j = 0; j < 3; j++){
                img->data[i + j] = 0;
            }
        } else {
            for(int j = 0; j < 3; j++){
                img->data[i + j] = 255;
            } 
        }  
    }   
    save(img,"operacion_4.png");
}

// Creo que este es el error que se muestra
void operacion_4_error(int limite){
    Imagen* img = load("Pikachu.png");
    for(int i = 0; i < img->height * img->width * img->channels; i += 4){
        for(int j = 0; j < 3; j++){
            if(img->data[i + j] < limite){
                img->data[i + j] = 0;
            } else {
                img->data[i + j] = 255;
            }
        }
    }   
    save(img,"operacion_4_error.png");
}


char dictionary[] = ''

int main() {
    operacion_1();
    operacion_2();
    operacion_3(0.5);
    operacion_4(127);
    operacion_4_error(127);
}