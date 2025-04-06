#include "base.cpp"

void operacion_1() {  
    /*La idea de solucion, es recorrer fila por fila de arriba hacia abajo con 2 indices, uno que se ubique al extremo izquierdo de la fila,
     y que avanze hacia la derecha, y el otro que se ubique al extremo derecho de la fila, y avanze hacia la izquierda, para luego ocupar
    un puntero auxiliar, para hacer el cambio de los canales.*/
    Imagen* img = load("Blaziken.png");
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
    Imagen* img = load("Blaziken.png");
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
    /*Se multiplica cada valor de los canales RGB (sin contar el A) por el valor de atenuación.*/
    Imagen* img = load("Blaziken.png");
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
    /* Se calculará la luminosidad de cada pixel a traves de la formula luminosidad = R * 0.299 + G * 0.587 + B * 0.114.
    Después si la luminosidad es mas baja que el límite, transformamos el pixel a negro igualando cada canal a 0 y si es mayor que el limite, transformamos el pixel a blanco igualando cada canal a 255.*/
    Imagen* img = load("Blaziken.png");
    for(int i = 0; i < img->height * img->width * img->channels; i += img->channels){
        float luminosidad = img->data[i] * 0.299 + img->data[i + 1] * 0.587 + img->data[i + 2] * 0.114;
        if(luminosidad < limite){
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

void operacion_4_error(int limite){
    /* Este error se genera al no tener en cuenta la luminosidad de cada pixel, si no que la de cada canal. Esto causa una saturación en los colores.*/
    Imagen* img = load("Blaziken.png");
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
    string peticion = ""; //Es el string de ascii que quiera usar el usuario para ejecutar la función too_ascii
    for(int i=0; i<n; i++){
        char c;
        cout<<"Ingrese el caracter numero "<<i+1<<": "; //Se concatena el string a medida que el usuario ingresa caracteres.
        cin>>c;
        peticion +=c;
    }
    return peticion; //Se retorna la petición de caracteres ascii que quiera usar el usuario.
}

char** to_ascii(const char* filename, string peticion){
    /*En esta función se tomará en cuenta el largo del string que ingresó el usuario, para calcular la variable rango.
    Esta variable rango será usada para asignarla como límite de luminosidad a cada píxel.
    Se itera cada pixel, iterando una variable j sobre el largo del string.
    Luego si la luminosidad de este pixel, es menor al rango sobre el que estamos (rango*j), se le asignara el primer caracter que se encuentre en el indice j,
    notar que si el pixel es negro siempre se le asignara el primer caracter del string ascii.*/
    Imagen* img = load(filename);
    int N = peticion.length();
    int rango = 256/N;
    char* ascii = new char [img->channels * img->width * img->height];

    for(int i = 0; i < img->height * img->width * img->channels; i += img->channels){
        float luminosidad = img->data[i] * 0.299 + img->data[i + 1] * 0.587 + img->data[i + 2] * 0.114;
        for(int j=0; j<N;j++){
            if(luminosidad<(rango*j)){
                ascii[i / img->channels] = peticion[j]; //Ascii usa la posicion i/channels, ya que i siempre sera multiplo de channels. 
                break; //Break se usa para que se quede en la iteracion correcta, en caso contrario, sobreescribiria solamente el ultimo caracter ascii de peticion.
            }
            else{
                ascii[i / img->channels] = peticion[j];
            }
        }
    }
    //Segun se pide en la tarea, el arreglo de caracteres ascii se debe guardar en un arreglo bidimensional.
    //Dado que bidim, sera un arreglo bidimensional dinámico, este será un arreglo de punteros, de ahí el tipo char**.
    char** bidim = new char*[img->height]; //Se reserva memoria según la altura de la imagen.
    for(int i=0; i<img->height;i++){ //En este ciclo, se reserva la memoria en cada altura, según el ancho de la imagen.
        bidim[i] = new char[img->width];
    }
    /*En el siguiente ciclo, se guardará el arreglo unidimensional, en el bidimensional, de la forma [alto][ancho]*/
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
    /*En esta función se abre el archivo en modo escritura, para luego recorrer ascii de la forma [alto][ancho], y sobreescribir cada pixel, por un caracter asciii*/
    ofstream file;
    file.open("ascii.txt");
    Imagen* img = load("Blaziken.png");
    for(int i=0; i<img->height;i++){
        for(int j=0; j<img->width;j++){
            file <<ascii[i][j];
        }
        file <<endl;
    }
    //Como ascii es un arreglo bidimensinal, se debe eliminar primero la memoria que corresponda a cada ancho de la imagen.
    for (int i = 0; i < img->height; i++) {
        delete[] ascii[i];
    }
    //Se borra la información de ascii que corresponde a el alto de la imagen.
    delete[] ascii;
    stbi_image_free(img->data);
    delete img;
}

int main() {
    
    operacion_1();
    operacion_2();
    float atenuacion;
    cout<<"Ingrese nivel de atenuacion: ";cin>>atenuacion;
    operacion_3(atenuacion);
    int limite;
    cout<<"Ingrese el limite para operacion 4: ";cin>>limite;
    operacion_4(limite);
    int limite_error;
    cout<<"Ingrese el limite para operacion 4 error: ";cin>>limite_error;
    operacion_4_error(limite_error);
    string peticion = ingresar_ascii(); 
    char** ascii = to_ascii("Blaziken.png", peticion);
    save_ascii(ascii);
}