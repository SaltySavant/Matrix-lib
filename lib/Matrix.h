#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <typeinfo>
#include <limits>
#include <utility>

template<typename T>
class matrix{
private:
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
    std::vector<std::vector<T>> vmatrix;                                          ///Matrix
    std::vector<std::string> split(const std::string& s, char delimiter);         ///Splits a string into a substring by value///
public:
    /////Constructors-Destructor/////
    matrix() = default;
    ~matrix() = default;
    matrix(int,int);                              ///Constructor to create an empty matrix///
    explicit matrix(std::vector<std::vector<T>>); ///Constructor to copy from vector///
    //////////////Methods to use//////////////
    void display();                               ///Print matrix to console///
    void loadMatrix(std::fstream);                ///Reading a matrix from a file///
    void setMatrix(const std::string& mMatrix);   ///Methods to create a matrix from string///
    bool add(int , int , T);                      ///Add data in a matrix///
    bool change(int , int , T );                  ///Change data in a matrix///
    bool clear();                                 ///Clear data and size a matrix///
    const std::pair<size_t,size_t> size() const;  ///Size matrix///
    void resize(int n ,int m);                    ///Resize matrix///
    void resize(const std::pair<size_t,size_t>&); ///Resize matrix with help pair///
    //////////Overloaded Operators//////////
    matrix<T> operator+ (const matrix<T>& other) const ;    ///Matrix addition///
    matrix<T> operator- (const matrix<T>& other) const ;    ///Matrix difference///
    matrix<T> operator* (T val) const ;                     ///Multiplication matrix by num///
    matrix<T> operator* (const matrix<T>& other) const;     ///Matrix multiplication///
};


///////////////Implementation///////////////
#include <iostream>
#include <sstream>

//////////////Private///////////
///Splits a string into a substring by value///
template <typename T>
std::vector<std::string> matrix<T>::split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream token_stream(s);
    while (std::getline(token_stream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

//////////////Public////////////////
///Constructor to copy vector from matrix///
template<typename T>
matrix<T>::matrix(std::vector<std::vector<T>> mMatrix):vmatrix(mMatrix) {}

///Constructor to create an empty matrix///
template<typename T>
matrix<T>::matrix(int n, int m) {
    std::vector<std::vector<int>> vec;
    int x = 0;
    for(int i = 0; i < n; i++){
        std::vector<int> s;
        for(int k = 0; k < m; k++){
            s.push_back(x);
        }
        vec.push_back(s);
    }
    vmatrix = vec;
}

//////////////Methods to use//////////////
///Print matrix to console///
template<typename T>
void matrix<T>::display() {
    for(auto const& vec : vmatrix){
        for(auto const& itr : vec){
            std::cout << itr << ' ';
        }
        std::cout << std::endl;
    }
}

///Reading a matrix from a file///
template<typename T>
void matrix<T>::loadMatrix(std::fstream mMatrix) {
    std::string content;
    if (mMatrix.is_open())
    {
        std::string line;
        while (std::getline(mMatrix, line))
        {
            content += line + '\n';
        }
        mMatrix.close();
    }
    else
    {
        std::cerr << "Error opening file" << std::endl;
        return;
    }

    setMatrix(content);
}

///Сreate a vector from string///
template<typename T>
void matrix<T>::setMatrix(const std::string &mMatrix) {
    auto vectors = split(mMatrix,'\n');
    size_t last;
    for(auto const& itr:vectors){
        auto row = matrix::split(itr,' ');
        std::vector<T> vec;
        if(row.size() != last && vectors[0] != itr){
            std::cout << "The matrix must have equal row sizes" << std::endl;
            continue;
        }

            for(int i = 0;i < row.size();i++){

                T result;
             try {
                    result = (T)std::stod(row[i]);
             } catch (const std::invalid_argument& e) {
                 return;
              }

              vec.push_back(result);
            }

        last = row.size();
        vmatrix.push_back(vec);

    }
}

///Add or change data in a matrix///
template<typename T>
bool matrix<T>::add(int n, int m, T data) {
    try {
        vmatrix.at(n).at(m) += data;
    } catch (const std::out_of_range& e) {
        std::cout << "Out of range: '" << e.what() <<"' \nYou should expand the size of the matrix with the 'resize' function and try adding the data again" << std::endl;
        return false;
    }
    return true;
}

///Add or change data in a matrix///
template<typename T>
bool matrix<T>::change(int n, int m, T data) {
    try {
        vmatrix.at(n).at(m) = data;
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
    return std::pair<size_t,size_t>(vmatrix.size(),vmatrix[0].size());
}

///Resize matrix///
template<typename T>
void matrix<T>::resize(int n, int m) {
    try {
        vmatrix.resize(n);
        for(int i = 0 ; i < vmatrix.size(); i++){
            vmatrix[i].resize(m);
        }
    } catch (const std::exception& e) {
        std::cerr << "Resize - an error occurred: " << e.what() << std::endl;
        return;
    }

}

///Resize matrix with help pair///
template<typename T>
void matrix<T>::resize(const std::pair<size_t,size_t>& size) {
    try {
        vmatrix.resize(size.first);
        for(int i = 0 ; i < vmatrix.size(); i++){
            vmatrix[i].resize(size.second);
        }
    } catch (const std::exception& e) {
        std::cerr << "Resize - an error occurred: " << e.what() << std::endl;
        return;
    }

}

//////////Overloaded Operators//////////
///Matrix addition///
template<typename T>
matrix<T> matrix<T>::operator+(const matrix<T> &other) const {
    if(other.size() != this->size()){
        return  matrix<T>();
    }
    else{
        matrix<T> newMatrix;
        size_t n = this->size().first;
        size_t m = this->size().second;
        newMatrix.resize(this->size());

        for(auto i = 0;i < n; i++){
            for(auto j = 0;j < m; j++){
                newMatrix.change(i,j, this->vmatrix[i][j]+other.vmatrix[i][j]);
            }
        }
        return newMatrix;
    }

}

///Matrix difference///
template<typename T>
matrix<T> matrix<T>::operator-(const matrix<T> &other) const {
    if(other.size() != this->size()){
        return  matrix<T>();
    }
    else{
        matrix<T> newMatrix;
        size_t n = this->size().first;
        size_t m = this->size().second;
        newMatrix.resize(this->size());

        for(auto i = 0;i < n; i++){
            for(auto j = 0;j < m; j++){
                newMatrix.change(i,j, this->vmatrix[i][j]-other.vmatrix[i][j]);
            }
        }
        return newMatrix;
    }

}

///Multiplication matrix by num///
template<typename T>
matrix<T> matrix<T>::operator*(T val) const{
    matrix<T> newMatrix;
    size_t n = this->size().first;
    size_t m = this->size().second;
    newMatrix.resize(this->size());

    for(auto i = 0;i < n; i++){
        for(auto j = 0;j < m; j++){
            newMatrix.change(i,j, this->vmatrix[i][j]*val);
        }
    }
    return newMatrix;
}

///Matrix multiplication///
template<typename T>
matrix<T> matrix<T>::operator*(const matrix<T> &other) const{
    auto sizeMy = this->size();
    auto sizeOther = other.size();
    if(sizeMy.second != sizeOther.first)
        {
            return matrix<T>();
        }
    
        matrix<T> newMtx;
        newMtx.resize(other.size().second, sizeMy.first);
        
        for(int i = 0 ; i < sizeMy.first; i++){
            for(int j = 0 ; j < sizeOther.second ; j++ ){
                for(int k = 0 ; k < sizeMy.second; k++){
                    newMtx.add(i, j, this->vmatrix[i][k]*other.vmatrix[k][j]);
                }
            }
        }
        return newMtx;
}
