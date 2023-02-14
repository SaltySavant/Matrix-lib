#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <typeinfo>
#include <limits>
#include <utility>
#include <cstdint>

template<typename T>
class matrix{
private:
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
    std::vector<T> vmatrix;
    size_t row;
    size_t column;
    std::uint8_t threads = 2;
    void setStringToMatrix(const std::string&);
    bool add(size_t index,const T& data);                  ///Add data in a matrix///
    bool change(size_t index,const T& data);               ///Change data in a matrix///
public:
    /////Constructors-Destructor/////
    matrix() = default;
    ~matrix() = default;
    matrix(size_t,size_t);                              ///Constructor to create an empty matrix///
    //////////////Methods to use//////////////
    void setNumberOfThreads(const std::uint8_t);  ///Changes the number of threads for calculations///
    void display();                               ///Print matrix to console///
    void loadMatrix(const std::string&);          ///Reading a matrix from a file///
    void setMatrix (const std::string&);          ///Methods to create a matrix from string///
    bool add(size_t ,size_t , const T&);          ///Add data in a matrix///
    bool change(size_t , size_t , const T&);      ///Change data in a matrix///
    bool clear();                                 ///Clear data and size a matrix///
    const std::pair<size_t,size_t> size() const;  ///Size matrix///
    void resize(size_t n ,size_t m);              ///Resize matrix///
    void resize(const std::pair<size_t,size_t>&); ///Resize matrix with help pair///
    const T getElement(size_t r , size_t c) const;
    //////////Overloaded Operators//////////
    matrix<T> operator+ (const matrix<T>& other) const ;    ///Matrix addition///
    matrix<T> operator- (const matrix<T>& other) const ;    ///Matrix difference///
    matrix<T> operator* (T val) const ;                     ///Multiplication matrix by num///
    matrix<T> operator* (const matrix<T>& other) const;     ///Matrix multiplication///
};

///////////////Implementation///////////////
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <thread>

//////////////Private///////////
///Splits a string into a substring by value///
template <typename T>
void matrix<T>::setStringToMatrix(const std::string& s) {

    vmatrix.clear();
    vmatrix.shrink_to_fit();
    std::istringstream is(s);
    T num;
    while (is >> num) {
        vmatrix.push_back(num);
    }
    if(is.fail()){
        is.clear();
    }

    if(vmatrix.size() < row * column){
        vmatrix.resize(row * column , 0);
    }
}

///Add or change data in a matrix///
template <typename T>
bool matrix<T>::add(size_t index,const T& data){
    try {
        vmatrix.at(index) += data;
    } catch (const std::out_of_range& e) {
        std::cout << "Out of range: '" << e.what() <<"' \nYou should expand the size of the matrix with the 'resize' function and try adding the data again" << std::endl;
        return false;
    }
    return true;
}

template <typename T>
bool matrix<T>::change(size_t index,const T& data){
    try {
        vmatrix.at(index) = data;
    } catch (const std::out_of_range& e) {
        std::cout << "Out of range: '" << e.what() <<"' \nYou should expand the size of the matrix with the 'resize' function and try adding the data again" << std::endl;
        return false;
    }
    return true;
}
//////////////Public////////////////
///Changes the number of threads for calculations///
template<typename T>
void matrix<T>::setNumberOfThreads(const std::uint8_t numThreads) {
    if(numThreads <= 0){
        throw std::invalid_argument("The number of threads used for calculation can't be less than one");
    }
    else{
        threads = numThreads;
    }
}

///Constructor to create an empty matrix///
template<typename T>
matrix<T>::matrix(size_t row, size_t column):row(row),column(column) {
    vmatrix.resize(row*column);
}

//////////////Methods to use//////////////
///Print matrix to console///
template<typename T>
void matrix<T>::display() {
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            std::cout << vmatrix[i * column + j] << " ";
        }
        std::cout << std::endl;
    }
}

///Reading a matrix from a file///
template<typename T>
void matrix<T>::loadMatrix(const std::string& filename) {
    std::ifstream mMatrix(filename);
    std::string content;
    if (mMatrix.is_open())
    {
        std::string line;
        while (std::getline(mMatrix, line))
        {
            content += line;
        }
    }
    else
    {
        std::cerr << "Error opening file" << std::endl;
        mMatrix.close();
        return;
    }

    mMatrix.close();
    setStringToMatrix(content);
}


template<typename T>
const T matrix<T>::getElement(size_t r , size_t c) const{
    try {
        return vmatrix.at(r*column+c);
    } catch (const std::out_of_range& e) {
        std::cout << "Out of range: '" << e.what() <<"' \nYou should expand the size of the matrix with the 'resize' function and try getting the data again" << std::endl;
        return false;
    }
}

///Сreate a vector from string///
template<typename T>
void matrix<T>::setMatrix(const std::string &mMatrix) {
    setStringToMatrix(mMatrix);
}

///Add data in a matrix///
template<typename T>
bool matrix<T>::add(size_t ro, size_t col, const T& data) {
    try {
        vmatrix.at(ro*column+col) += data;
    } catch (const std::out_of_range& e) {
        std::cout << "Out of range: '" << e.what() <<"' \nYou should expand the size of the matrix with the 'resize' function and try adding the data again" << std::endl;
        return false;
    }
    return true;
}


///Change data in a matrix///
template<typename T>
bool matrix<T>::change(size_t ro, size_t col,const T& data) {
    try {
        vmatrix.at(ro*column+col) = data;
    } catch (const std::out_of_range& e) {
        std::cout << "Out of range: '" << e.what() <<"' \nYou should expand the size of the matrix with the 'resize' function and try adding the data again" << std::endl;
        return false;
    }
    return true;
}


///Clear data and size a matrix///
template<typename T>
bool matrix<T>::clear() {
    try{
        vmatrix.clear();
    }
    catch (std::exception& e) {
        std::cout << "Failed to clear matrix: '" << e.what() << "'" << std::endl;
        return false;
    }
    return true;
}

///Size matrix///
template<typename T>
const std::pair<size_t,size_t> matrix<T>::size() const {
    return std::pair<size_t,size_t>(row,column);
}

///Resize matrix///
template<typename T>
void matrix<T>::resize(size_t ro, size_t col) {
    try {
        row =ro;
        column = col;
        vmatrix.resize(ro*col);
    } catch (const std::exception& e) {
        std::cerr << "Resize - an error occurred: " << e.what() << std::endl;
        return;
    }

}

///Resize matrix with help pair///
template<typename T>
void matrix<T>::resize(const std::pair<size_t,size_t>& size) {
    try {
            row = size.first;
            column = size.second;
            vmatrix.resize(row*column);
    } catch (const std::exception& e) {
        std::cerr << "Resize - an error occurred: " << e.what() << std::endl;
        return;
    }

}

//////////Overloaded Operators//////////
///Matrix addition///
template<typename T>
matrix<T> matrix<T>::operator+(const matrix<T> &other) const {
    if(other.row != this->row || other.column != this->column){
        throw std::invalid_argument("In order to add or subtract matrices, it is necessary that the number of rows and columns coincide for both matrices");
    }
        matrix<T> mxt(this->row , this->column);
            for (size_t i = 0; i < this->row*this->column; ++i) {
                mxt.change(i, other.vmatrix[i]+ this->vmatrix[i]);
            }
    return mxt;
}


///Matrix difference///
template<typename T>
matrix<T> matrix<T>::operator-(const matrix<T> &other) const {
    if(other.row != row || other.column != column){
        throw std::invalid_argument("In order to add or subtract matrices, it is necessary that the number of rows and columns coincide for both matrices");
    }
    matrix<T> mxt(row , column);
    std::vector<std::thread> usingThreads;
    usingThreads.reserve(threads);
    for (size_t i = 0; i < row*column; ++i) {
        mxt.change(i, vmatrix[i] - other.vmatrix[i]);
    }
    return mxt;
}


///Multiplication matrix by num///
template<typename T>
matrix<T> matrix<T>::operator*(T val) const{
    matrix<T> mxt(row , column);
    std::vector<std::thread> usingThreads;
    usingThreads.reserve(threads);
    for(std::uint8_t i = 0; i < threads; i++) {
        usingThreads.emplace_back([&,i](){
                for (size_t j = 0; j < this->row * this->column; j += threads) {
                    mxt.change(j, vmatrix[j] * val);
                }
            });
        }
    for(std::thread &t : usingThreads){
        t.join();
    }
    return mxt;
}


///Matrix multiplication///
template<typename T>
matrix<T> matrix<T>::operator*(const matrix<T> &other) const{
    if(column != other.row){
        throw std::invalid_argument("Error: Incompatible matrix sizes for multiplication.");
    }
    matrix<T> mxt(row, other.column);
    std::vector<std::thread> usingThreads;
    usingThreads.reserve(threads);

    for(std::uint8_t i = 0; i < threads; i++){
        usingThreads.emplace_back([&,i](){
            for(size_t j = i; j < other.column; j += threads){
                for(size_t l = 0; l < row; l++){
                    for(size_t k = 0; k < column; k++){
                        mxt.vmatrix[l*mxt.column +j] += vmatrix[l*column +k] * other.vmatrix[k* other.column +j];
                    }
                }
            }
        });
    }
   for(std::thread &t : usingThreads){
        t.join();
    }
    return mxt;
}
