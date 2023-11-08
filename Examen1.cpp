#include <iostream>
#include <fstream>
#include <vector>
#include <complex>
#include <cmath>

using namespace std;

// Definimos un tipo 'Complejo' para manejar números complejos.
typedef complex<double> Complejo;

// Función para realizar la Transformada Rápida de Fourier (FFT).
void fft(vector<Complejo> &datos) {
    int n = datos.size();
    // Caso base: si la longitud del vector es 1, no hacer nada.
    if (n <= 1) return;

    // Separar los elementos pares e impares en dos vectores.
    vector<Complejo> par(n / 2), impar(n / 2);
    for (int i = 0; i < n / 2; i++) {
        par[i] = datos[i * 2];
        impar[i] = datos[i * 2 + 1];
    }

    // Aplicar FFT recursivamente a las partes par e impar.
    fft(par);
    fft(impar);

    // Combinar los resultados de las FFTs parciales.
    for (int i = 0; i < n / 2; i++) {
        double angulo = -2 * M_PI * i / n;
        Complejo w(cos(angulo), sin(angulo)); // Factor complejo w_n^k
        Complejo temp = w * impar[i];
        datos[i] = par[i] + temp;
        datos[i + n / 2] = par[i] - temp;
    }
}

// Función para calcular la magnitud de un número complejo.
double magnitud(Complejo c) {
    return abs(c);
}

int main() {
    string nombreBase, extension;
    int frecuenciaMuestreo; // Frecuencia de muestreo

    // Obtener los datos del archivo y la frecuencia de muestreo del usuario.
    cout << "Ingrese el nombre base del archivo: ";
    cin >> nombreBase;
    cout << "Ingrese la extension del archivo (con punto inicial): ";
    cin >> extension;
    cout << "Ingrese la frecuencia de muestreo: ";
    cin >> frecuenciaMuestreo;
    
    string nombreArchivo = nombreBase + extension;

    // Abrir el archivo  para lectura.
    ifstream archivo(nombreArchivo.c_str());
    if (!archivo) {
        cout << "No se pudo abrir el archivo " << nombreArchivo << endl;
        return 1;
    }

    // Leer los datos del archivo y almacenarlos en un vector de números complejos.
    vector<Complejo> datos;
    double valor;
    while (archivo >> valor) {
        datos.push_back(Complejo(valor, 0.0));
    }

    archivo.close();

    // Realizar la FFT a los datos leídos.
    fft(datos);

    // Crear un archivo de salida para escribir los resultados de la FFT.
    string nombreArchivoSalida = nombreBase + "_FFT" + extension;
    ofstream archivoSalida(nombreArchivoSalida);
    if (!archivoSalida) {
        cout << "No se pudo crear el archivo de salida " << nombreArchivoSalida << endl;
        return 1;
    }

    // Escribir la magnitud y la frecuencia de cada componente en el archivo de salida.
    int n = datos.size();
    double maxAmplitud = 0.0;
    int indiceMaxAmplitud = 0;
    for (int i = 0; i < n / 2; i++) {
        double frecuencia = static_cast<double>(i) * frecuenciaMuestreo / n;
        double amplitud = (magnitud(datos[i]) / n) * 2; // Calcula la amplitud actual
        archivoSalida << "Amplitud[" << i << "]: " << scientific << amplitud << " Frecuencia[" << i << "]: " << fixed << frecuencia << endl;
    
        // Comprobar si la amplitud actual es la máxima.
        if (amplitud > maxAmplitud) {
            maxAmplitud = amplitud;
            indiceMaxAmplitud = i;
        }
    }

    archivoSalida.close();

    // Calcular la frecuencia con la máxima amplitud y mostrarla.
    double maxFrecuencia = static_cast<double>(indiceMaxAmplitud) * frecuenciaMuestreo / n;
    cout << "Amplitud maxima en:" << endl;
    cout << "Frecuencia de " << maxFrecuencia << " Hz con " << maxAmplitud << " de amplitud" << endl;

    return 0;
}
