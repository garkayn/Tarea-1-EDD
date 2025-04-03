#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

// Importacion de librerias. Los archivos correspondientes deben estar en el mismo directorio que el .cpp.
#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>
#include <string>
#include<fstream>

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
    /*La idea de solucion, es recorrer fila por fila de arriba hacia abajo con 2 indices, uno que se ubique al extremo izquierdo de la fila,
     y que avanze hacia la derecha, y el otro que se ubique al extremo derecho de la fila, y avanze hacia la izquierda, para luego ocupar
    un puntero auxiliar, para hacer el cambio de los canales.*/
    Imagen* img = load("Pikachu.png");
    unsigned char* p_aux = new unsigned char();
    for(int i = 0; i < (img->height); i++){    
        for(int j = 0; j < (img->width/2); j++){ 
            int id= i * img->width * img->channels + (img->width * img->channels -j * img->channels- img->channels); 
            int ii= i * img->width * img->channels  + j * img->channels; 
            for(int k = 0; k<img->channels;k++){ //El for recorre los canales de cada pixel para hacer el intercambio de los colores de forma correcta.
                *p_aux = img->data[id + k]; //Este se ubica a la derecha.
                img->data[id+k] = img->data[ii+k]; //Este se ubica a la izquierda.
                img->data[ii + k] = *p_aux;
            }
        }
    }
    save (img, "operacion_1.png");
    delete p_aux;
    stbi_image_free(img->data);
    delete img;
}

void operacion_2(){
    /* La idea de solucion, es guardar todo el espacio de memoria para la imagen para la imagen rotada, para luego leer la imagen original
    de manera conveniente, para luego escribir sobre el espacio de memoria anteriormente mencionado.    
    La imagen original sera leida columna por columna de izquierda a derecha y leyendo cada columna de abajo hacia arriba,
     y sera escrite fila por fila de arriba hacia abajo, y escribiendo cada fila de izquierda a derecha */
    Imagen* img = load("Pikachu.png");
    Imagen* img_rotada = new Imagen();
    /*Se pasa el alto de la imagen original, al ancho de la imagen rotada, y el ancho de la imagen original, al alto de la imagen rotada,
    ya que al rotar la imagen, tambien se cambian sus dimensiones, y asi el struct de la imagen rotada, puede pasar correctamente la informacion
    a la funcion save.*/
    img_rotada->width = img->height;
    img_rotada->height = img->width;
    img_rotada->channels = img->channels;
    img_rotada->data = new unsigned char[img->width * img->height * img->channels]; 
    for(int i = 0; i < img->width; i++){
        for(int j = 0; j < img->height; j++){
            for(int k = 0; k < img->channels; k++){
                img_rotada->data[img->channels * (i * img->height + j) + k] = 
                    img->data[img->channels * (img->width * (img->height - 1 - j) + i) + k];
            }
        }
    }
    save(img_rotada,"operacion_2.png");
    stbi_image_free(img->data); /*Esta funcion fue buscada en internet, ya que al no hacer save al puntero img,
    la funcion stbi_write_png no libera la memoria ocupada por la imagen original. Y encontramos que para borrar el data de la imagen, se
    usa stbi_image_free.*/
    delete img;
    delete[] img_rotada->data;
    delete img_rotada;
}

void operacion_3(float atenuacion){
    Imagen* img = load("Pikachu.png");
    for(int i = 0; i < img->height * img->width * img->channels; i += img->channels){
        for(int j = 0; j < 3; j++){
            img->data[i + j] *= atenuacion;
        }      
    }
    save(img,"operacion_3.png");
    stbi_image_free(img->data);
    delete img;
}

void operacion_4(int limite){
    Imagen* img = load("Pikachu.png");
    for(int i = 0; i < img->height * img->width * img->channels; i += img->channels){
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
    stbi_image_free(img->data);
    delete img;
}

// Creo que este es el error que se muestra
void operacion_4_error(int limite){
    Imagen* img = load("Pikachu.png");
    for(int i = 0; i < img->height * img->width * img->channels; i += img->channels){
        for(int j = 0; j < 3; j++){
            if(img->data[i + j] < limite){
                img->data[i + j] = 0;
            } else {
                img->data[i + j] = 255;
            }
        }
    }   
    save(img,"operacion_4_error.png");
    stbi_image_free(img->data);
    delete img;
}

string ingresar_ascii(){
    //Esta función le pedirá al usuario el largo del arreglo de caracteres que desee utilizar para el ascii,
    //junto con dichos caracteres.
    int n;
    cout<<"Ingrese el largo del arreglo ascii: ";
    cin>>n; //El usuario ingresa el largo del arreglo.
    string peticion = ""; //Preguntar si podemos ocupar getline
    for(int i=0; i<n; i++){
        char c;
        cout<<"Ingrese el caracter numero "<<i+1<<": ";
        cin>>c;
        peticion +=c;
    }
    return peticion;
}

char** to_ascii(const char* filename, string peticion){
    Imagen* img = load(filename);
    int N = peticion.length();
    int rango = 256/N;
    char* ascii = new char [img->channels * img->width * img->height];

    for(int i = 0; i < img->height * img->width * img->channels; i += img->channels){
        float NTSC = img->data[i] * 0.299 + img->data[i + 1] * 0.587 + img->data[i + 2] * 0.114;
        for(int j=0; j<N;j++){
            if(NTSC<(rango*(j-1) + rango)){
                ascii[i / img->channels] = peticion[j];
                break; //Break se usa para que se quede con la iteracion correcta, en caso contrario, sobreescribiria solamente el ultimo caracter ascii de peticion
            }
            else{
                ascii[i / img->channels] = peticion[j];
            }
        }
    }

    char** bidim = new char*[img->height];
    for(int i=0; i<img->height;i++){
        bidim[i] = new char[img->width];
    }
    int pixel = 0;
    for(int i = 0; i < img->height; i++) {
        for(int j = 0; j <img->width; j++) {
            bidim[i][j] = ascii[pixel++];
        }
    }

    stbi_image_free(img->data);
    delete img;
    delete[] ascii;
    return bidim;
}

void save_ascii(char** ascii){
    ofstream file;
    file.open("ascii.txt");
    Imagen* img = load("siu.jpg");
    for(int i=0; i<img->height;i++){
        for(int j=0; j<img->width;j++){
            file <<ascii[i][j];
        }
        file <<endl;
    }
    for (int i = 0; i < img->height; i++) {
        delete[] ascii[i];
    }
    delete[] ascii;
    stbi_image_free(img->data);
    delete img;
}

int main() {
    operacion_1();
    operacion_2();
    //operacion_3(0.5);
    //operacion_4(127);
    //operacion_4_error(127);
    //string peticion = ingresar_ascii(); 
    //char** ascii = to_ascii("siu.jpg", peticion);
    //save_ascii(ascii);
}