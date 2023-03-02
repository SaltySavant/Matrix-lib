#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <typeinfo>
#include <limits>
#include <utility>
#include <cstdint>
#include <cstdlib>
#include <algorithm>
#include <chrono>
#include <memory_resource>
#include <cstdlib>
#include <thread>
#include <mutex>


template<typename T>
class matrix{
private:
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
    std::vector<T> vmatrix;
    std::size_t row;
    std::size_t column;
    std::size_t blockSize = 1;
    std::size_t threads = 1;
    void setStringToMatrix(const std::string&);
    bool add(size_t index,const T& data);                  ///Add data in a matrix///
    bool change(size_t index,const T& data);               ///Change data in a matrix///
    const std::vector<T>& getMatrix() const;
    void blocked_column_multi_output_parallel_mmul(const std::vector<T>& B,std::vector<T>& C, std::size_t P,std::size_t start_col,std::size_t end_col,std::size_t chunkSize) const;

public:
    /////Constructors-Destructor/////
    matrix() = default;
    ~matrix() = default;
    matrix(size_t,size_t);                              ///Constructor to create an empty matrix///
    matrix(const std::vector<T> &vec, const size_t &row, const size_t &column);
    //////////////Methods to use//////////////
    void setNumberOfThreads(const std::size_t);  ///Changes the number of threads for calculations///
    void setBlockSize(const std::size_t );       ///Changes the number Blocks for calculations///
   const std::size_t getNumberOfThreads();
    const std::size_t getBlockSize();
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
    void setOptimalParams();
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
#include <math.h>

//////////////Private//////////////
template<typename T>
const std::size_t matrix<T>::getNumberOfThreads(){
    return threads;
}

template<typename T>
const std::size_t matrix<T>::getBlockSize(){
    return blockSize;
}

template<typename T>
void matrix<T>::setOptimalParams() {
    std::size_t numCores = std::thread::hardware_concurrency();
    std::size_t chunkSize = sqrt(row / (2 * sizeof(size_t)));
    chunkSize = std::max(chunkSize, static_cast<std::size_t>(1));
    chunkSize = std::min(chunkSize, row);

    std::size_t num_threads = std::min(numCores, row / chunkSize/ chunkSize);
    std::max(num_threads, static_cast<std::size_t>(1));

    threads = num_threads;
    blockSize = chunkSize;
}

template<typename T>
void matrix<T>::blocked_column_multi_output_parallel_mmul(const std::vector<T>& B,std::vector<T>& C, std::size_t P,std::size_t start_col,std::size_t end_col,std::size_t chunkSize) const{
    // For each chunk of columns
    for (std::size_t col_chunk = start_col; col_chunk < end_col; col_chunk += chunkSize)
        // For each chunk of rows
        for (std::size_t row_chunk = 0; row_chunk < row; row_chunk += chunkSize)
            // For each block of elements in this row of this column chunk
            // Solve for 16 elements at a time
            for (std::size_t tile = 0; tile < column; tile += chunkSize)
                // For apply that tile to each row of the row chunk
                for (std::size_t r = 0; r < chunkSize; r++)
                    // For each row in the tile
                    for (std::size_t tile_row = 0; tile_row < chunkSize; tile_row++)
                        // Solve for each element in this tile row
                        for (std::size_t idx = 0; idx < chunkSize; idx++)
                            C[(r + row_chunk) * P + col_chunk + idx] +=
                                    vmatrix[(r + row_chunk) * column + tile + tile_row] *
                                    B[tile * P + tile_row * P + col_chunk + idx];
}

template<typename T>
matrix<T>::matrix(const std::vector<T> &vec, const size_t &row, const size_t &column):column(column),row(row) {
    if(row <= 0 || column <= 0){
        throw std::invalid_argument("It is not possible to create a matrix with parameters row = 0 , column = 0 (or less)");
    }
    else if(vec.size() != row*column){
        throw std::invalid_argument( "The size of the std::vector<T>& matrix must be equal to the product of row by column");
    }
    vmatrix = vec;
}

template<typename T>
const std::vector<T>& matrix<T>::getMatrix() const{
    return vmatrix;
}

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
///Changes the number Blocks for calculations///
template<typename T>
void matrix<T>::setBlockSize(const std::size_t blocks) {
    if(blocks > row || blocks > column){
        throw std::invalid_argument("The number of blocks used for calculation can't be more than row or column");
    }
    blockSize = blocks;
}

///Changes the number of threads for calculations///
template<typename T>
void matrix<T>::setNumberOfThreads(const std::size_t numThreads) {
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
    if(row <= 0 || column <= 0){
        throw std::invalid_argument("It is not possible to create a matrix with parameters row = 0 , column = 0 (or less)");
    }
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
    matrix<T> mxt(row , column);
    std::vector<std::thread> thrds;
    thrds.reserve(threads);
    std::size_t n_cols = row*column / threads;

    // Launch threads
    std::size_t start_col = 0;

    for (std::size_t i = 0; i < threads; i++) {
        auto end_col = (start_col+n_cols);
        if(threads-1 == i){
            auto remains = row*column % threads;
            if(remains != 0){
                end_col += remains;
            }
        }
        thrds.emplace_back([&,start_col,end_col] {
            for(std::size_t i = start_col ; i < end_col;i++){
                mxt.change(i, vmatrix[i] + other.vmatrix[i]);
            }
        });

        start_col = end_col;
    }

    // Wait for all threads to complete
    for (auto &t : thrds) t.join();
    thrds.clear();
    return mxt;
}


///Matrix difference///
template<typename T>
matrix<T> matrix<T>::operator-(const matrix<T> &other) const {
    if(other.row != row || other.column != column){
        throw std::invalid_argument("In order to add or subtract matrices, it is necessary that the number of rows and columns coincide for both matrices");
    }
    matrix<T> mxt(row , column);
    std::vector<std::thread> thrds;
    thrds.reserve(threads);
    std::size_t n_cols = row*column / threads;

    // Launch threads
    std::size_t start_col = 0;

    for (std::size_t i = 0; i < threads; i++) {
        auto end_col = (start_col+n_cols);
        if(threads-1 == i){
            auto remains = row*column % threads;
            if(remains != 0){
                end_col += remains;
            }
        }
        thrds.emplace_back([&,start_col,end_col] {
            for(std::size_t i = start_col ; i < end_col;i++){
                mxt.change(i, vmatrix[i] - other.vmatrix[i]);
            }
        });

        start_col = end_col;
    }

    // Wait for all threads to complete
    for (auto &t : thrds) t.join();
    thrds.clear();
    return mxt;
}


///Multiplication matrix by num///
template<typename T>
matrix<T> matrix<T>::operator*(T val) const{
    matrix<T> mxt(row , column);
    std::vector<std::thread> thrds;
    thrds.reserve(threads);
    std::size_t n_cols = row*column / threads;

    // Launch threads
    std::size_t start_col = 0;

    for (std::size_t i = 0; i < threads; i++) {
        auto end_col = (start_col+n_cols);
        if(threads-1 == i){
            auto remains = row*column % threads;
            if(remains != 0){
                end_col += remains;
            }
        }
        thrds.emplace_back([&,start_col,end_col] {
            for(std::size_t i = start_col ; i < end_col;i++){
                mxt.change(i, vmatrix[i] * val);
            }
        });

        start_col = end_col;
    }

    // Wait for all threads to complete
    for (auto &t : thrds) t.join();
    thrds.clear();
    return mxt;
}

///Matrix multiplication///
template<typename T>
matrix<T> matrix<T>::operator*(const matrix<T> &other) const {
    if (column != other.row) {
        throw std::invalid_argument("Error: Incompatible matrix sizes for multiplication.");
    }

    std::size_t P = other.column;
    std::vector<T> B = other.vmatrix;
    std::vector<T> C(row * P);

    std::vector<std::thread> thrds;
    thrds.reserve(threads);
    std::size_t n_cols = P / threads;
    std::cout << n_cols << std::endl;
    // Launch threads
    std::size_t start_col = 0;

    std::size_t n_blocks = P / blockSize;
    std::size_t remains = P % blockSize;
    if (remains != 0) {
      remains = blockSize - remains;
    }
    else{
        remains = blockSize;
    }

    for (std::size_t i = 0; i < threads; i++) {
       auto end_col = (start_col+n_cols);
        if(threads-1 == i)
        {
                end_col = P;
        }
        thrds.emplace_back([&,start_col,end_col] {
            blocked_column_multi_output_parallel_mmul(B, C, P,start_col,end_col,remains);
        });

       start_col = end_col;
    }

    // Wait for all threads to complete
    for (auto &t : thrds) t.join();
    thrds.clear();

    matrix<T> mxt(C, row, P);
    return mxt;
}
