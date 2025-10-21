#include <iostream>
#include <stdexcept>
#include <string> 

template <typename Tipo>
class matrizBase {
protected:
    int filas;
    int columnas;

public:
    matrizBase(int num_filas, int num_columnas) : filas(num_filas), columnas(num_columnas) {}
    
    virtual ~matrizBase() = default; 
    
    virtual void cargarValores() = 0;
    virtual matrizBase<Tipo>* sumar(const matrizBase<Tipo>& otra) const = 0;
    virtual void imprimir() const = 0;
    
    virtual Tipo obtenerValor(int fila, int columna) const = 0;

    int obtenerFilas() const { return filas; }
    int obtenerColumnas() const { return columnas; }

    matrizBase<Tipo>* operator+(const matrizBase<Tipo>& otra) const {
        return this->sumar(otra);
    }
};

template <typename Tipo>
class matrizDinamica : public matrizBase<Tipo> {
private:
    Tipo** datos;

    void asignarmemoria() {
        datos = new Tipo*[this->filas];
        for (int i = 0; i < this->filas; ++i) {
            datos[i] = new Tipo[this->columnas]{}; 
        }
    }

    void liberarmemoria() {
        if (datos) {
            for (int i = 0; i < this->filas; ++i) {
                delete[] datos[i];
            }
            delete[] datos;
            datos = nullptr;
        }
    }

    void copiarDatos(const matrizBase<Tipo>& otra) {
         for (int i = 0; i < this->filas; ++i) {
            for (int j = 0; j < this->columnas; ++j) {
                datos[i][j] = otra.obtenerValor(i, j);
            }
        }
    }

public:
    matrizDinamica(int num_filas, int num_columnas) : matrizBase<Tipo>(num_filas, num_columnas) {
        asignarmemoria();
    }

    ~matrizDinamica() override {
        liberarmemoria();
        std::cout << "(Destructor: memoria liberada - matriz Dinamica)" << std::endl;
    }

    matrizDinamica(const matrizDinamica& otra) 
        : matrizBase<Tipo>(otra.filas, otra.columnas) {
        asignarmemoria();
        copiarDatos(otra);
    }

    matrizDinamica& operator=(const matrizDinamica& otra) {
        if (this != &otra) {
            liberarmemoria();
            this->filas = otra.filas;
            this->columnas = otra.columnas;
            asignarmemoria();
            copiarDatos(otra);
        }
        return *this;
    }

    matrizDinamica(matrizDinamica&& otra) noexcept 
        : matrizBase<Tipo>(otra.filas, otra.columnas), datos(otra.datos) {
        otra.datos = nullptr;
        otra.filas = 0;
        otra.columnas = 0;
    }

    matrizDinamica& operator=(matrizDinamica&& otra) noexcept {
        if (this != &otra) {
            liberarmemoria();
            this->filas = otra.filas;
            this->columnas = otra.columnas;
            datos = otra.datos;
            otra.datos = nullptr;
            otra.filas = 0;
            otra.columnas = 0;
        }
        return *this;
    }

    void cargarValores() override {
        std::cout << "Cargando " << this->filas * this->columnas << " valores para matriz Dinamica (" << this->filas << "x" << this->columnas << "):" << std::endl;
        for (int i = 0; i < this->filas; ++i) {
            for (int j = 0; j < this->columnas; ++j) {
                std::cout << "Valor [" << i << "][" << j << "]: ";
                std::cin >> datos[i][j];
            }
        }
    }

    matrizBase<Tipo>* sumar(const matrizBase<Tipo>& otra) const override {
        if (this->filas != otra.obtenerFilas() || this->columnas != otra.obtenerColumnas()) {
            throw std::invalid_argument("Las dimensiones de las matrices no coinciden para la suma");
        }

        matrizDinamica<Tipo>* resultado = new matrizDinamica<Tipo>(this->filas, this->columnas);
        
        for (int i = 0; i < this->filas; ++i) {
            for (int j = 0; j < this->columnas; ++j) {
                resultado->datos[i][j] = this->datos[i][j] + otra.obtenerValor(i, j);
            }
        }

        return resultado;
    }

    void imprimir() const override {
        for (int i = 0; i < this->filas; ++i) {
            std::cout << "| ";
            for (int j = 0; j < this->columnas; ++j) {
                std::cout << datos[i][j] << " | ";
            }
            std::cout << std::endl;
        }
    }

    Tipo obtenerValor(int fila, int columna) const override {
        if (fila < 0 || fila >= this->filas || columna < 0 || columna >= this->columnas) {
            throw std::out_of_range("indice fuera de rango");
        }
        return datos[fila][columna];
    }
};

template <typename Tipo, int Filas, int Columnas>
class matrizEstatica : public matrizBase<Tipo> {
private:
    Tipo datos[Filas][Columnas]{};

public:
    matrizEstatica() : matrizBase<Tipo>(Filas, Columnas) {}

    ~matrizEstatica() override {
        std::cout << "(Destructor: memoria liberada - matriz Estatica)" << std::endl;
    }

    void cargarValores() override {
        std::cout << "Cargando " << Filas * Columnas << " valores para matriz Estatica (" << Filas << "x" << Columnas << "):" << std::endl;
        for (int i = 0; i < this->filas; ++i) {
            for (int j = 0; j < this->columnas; ++j) {
                std::cout << "Valor [" << i << "][" << j << "]: ";
                std::cin >> datos[i][j];
            }
        }
    }

    matrizBase<Tipo>* sumar(const matrizBase<Tipo>& otra) const override {
        if (this->filas != otra.obtenerFilas() || this->columnas != otra.obtenerColumnas()) {
            throw std::invalid_argument("Las dimensiones de las matrices no coinciden para la suma");
        }

        matrizEstatica<Tipo, Filas, Columnas>* resultado = new matrizEstatica<Tipo, Filas, Columnas>();
        
        for (int i = 0; i < this->filas; ++i) {
            for (int j = 0; j < this->columnas; ++j) {
                resultado->datos[i][j] = this->datos[i][j] + otra.obtenerValor(i, j);
            }
        }

        return resultado;
    }

    void imprimir() const override {
        for (int i = 0; i < this->filas; ++i) {
            std::cout << "| ";
            for (int j = 0; j < this->columnas; ++j) {
                std::cout << datos[i][j] << " | ";
            }
            std::cout << std::endl;
        }
    }

    Tipo obtenerValor(int fila, int columna) const override {
         if (fila < 0 || fila >= this->filas || columna < 0 || columna >= this->columnas) {
            throw std::out_of_range("indice fuera de rango");
        }
        return datos[fila][columna];
    }
};


void demostrarConFloat() {
    std::cout << ">> Demostracion de Genericidad (Tipo FLOAT) <<" << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    
    matrizBase<float>* punteroDinamicoFloatA = nullptr;
    matrizBase<float>* punteroEstaticoFloatB = nullptr;
    matrizBase<float>* punteroResultadoFloatC = nullptr;

    try {
        std::cout << "1. Creando matriz Dinamica A (3x2)..." << std::endl;
        punteroDinamicoFloatA = new matrizDinamica<float>(3, 2);
        punteroDinamicoFloatA->cargarValores(); 
        std::cout << "\nmatriz A (Dinamica):" << std::endl;
        punteroDinamicoFloatA->imprimir();
        std::cout << std::endl;

        std::cout << "2. Creando matriz Estatica B (3x2)..." << std::endl;
        punteroEstaticoFloatB = new matrizEstatica<float, 3, 2>();
        punteroEstaticoFloatB->cargarValores();
        std::cout << "\nmatriz B (Estatica):" << std::endl;
        punteroEstaticoFloatB->imprimir();
        std::cout << std::endl;

        std::cout << "3. Realizando suma polimorfica: C = A + B" << std::endl;
        std::cout << "(Invocando A->sumar(B), metodo de matrizDinamica)" << std::endl;
        
        punteroResultadoFloatC = (*punteroDinamicoFloatA) + (*punteroEstaticoFloatB); 
        
        std::cout << "\nResultado C (Tipo Dinamica):" << std::endl;
        punteroResultadoFloatC->imprimir();
        std::cout << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "ERROR (Float Demo): Se produjo una excepcion: " << e.what() << std::endl;
    }

    std::cout << "4. Liberando memoria (Float)..." << std::endl;
    delete punteroResultadoFloatC;
    delete punteroEstaticoFloatB;
    delete punteroDinamicoFloatA; 
}

void demostrarConInt() {
    std::cout << ">> Demostracion de Genericidad (Tipo INT) <<" << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
    
    matrizBase<int>* punteroEstaticoIntA = nullptr;
    matrizBase<int>* punteroDinamicoIntB = nullptr;
    matrizBase<int>* punteroResultadoIntC = nullptr;

    try {
        std::cout << "1. Creando matriz Estatica A (2x3)..." << std::endl;
        punteroEstaticoIntA = new matrizEstatica<int, 2, 3>();
        punteroEstaticoIntA->cargarValores(); 
        std::cout << "\nmatriz A (Estatica):" << std::endl;
        punteroEstaticoIntA->imprimir();
        std::cout << std::endl;

        std::cout << "2. Creando matriz Dinamica B (2x3)..." << std::endl;
        punteroDinamicoIntB = new matrizDinamica<int>(2, 3);
        punteroDinamicoIntB->cargarValores(); 
        std::cout << "\nmatriz B (Dinamica):" << std::endl;
        punteroDinamicoIntB->imprimir();
        std::cout << std::endl;

        std::cout << "3. Realizando suma polimorfica: C = A + B" << std::endl;
        std::cout << "(Invocando A->sumar(B), metodo de matrizEstatica)" << std::endl;

        punteroResultadoIntC = (*punteroEstaticoIntA) + (*punteroDinamicoIntB); 
        
        std::cout << "\nResultado C (Tipo Estatica):" << std::endl;
        punteroResultadoIntC->imprimir();
        std::cout << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "ERROR (Int Demo): Se produjo una excepcion: " << e.what() << std::endl;
    }


    std::cout << "4. Liberando memoria (Int)..." << std::endl;
    delete punteroResultadoIntC; 
    delete punteroDinamicoIntB; 
    delete punteroEstaticoIntA;
}


int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "--- Sistema Generico de algebra Lineal ---" << std::endl;
    std::cout << "========================================" << std::endl << std::endl;

    demostrarConFloat();

    std::cout << std::endl << "========================================" << std::endl << std::endl;

    demostrarConInt();

    std::cout << std::endl << "========================================" << std::endl;
    std::cout << "Sistema finalizado correctamente." << std::endl;
    return 0;
}