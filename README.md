# Matrix C++ library 
[![Language](https://img.shields.io/badge/language-С++-blue.svg)](https://isocpp.org/) [![Standard](https://img.shields.io/badge/c%2B%2B-20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
## Requirements
No special requirement. You should already have this :

+ C++20 compatible compiler.

## Installation

The library consists of one file: `lib/Matrix.h`.

Just copy the `lib` folder in your project and you can `#include "lib/Matrix.h"` to your code. That is all.

## Overview
The library is a template class 'matrix' that accepts only numeric variables as an argument T ,
the matrix class is based on std::vector

**The 'matrix' class supports several matrix initialization methods:**

+ Initializing an empty matrix n x m
```cpp
    matrix<int> mtx(5,10);
```
+ Initialization of the matrix based on std::vector
```cpp
    std::vector<std::vector<int>> vec;
    std::size_t row;
    std::size_t column;
    /*
      Initializing a vec & row & column
    */
    matrix<int> mtx(myMatrix,row,column);
```
+ Initializing a matrix based on std::string

  It is important to put a space (' ') between the string elements in the variable std::string
```cpp
    std::string str = "2 -1 1"
                      "-5 0 2";
    matrix<int> mxt(2,3);
    mxt.setMatrix(str);
```
+ Load a matrix from a file

  It is important to put a space (' ') between the elements of the string
```cpp
    std::string filename = "matrix.txt"
    matrix<int> mxt;
    mxt.loadMatrix(filename);
```

**Now the library can solve such problems as:**

1. Arithmetic operations on matrices, namely '+' , '-' , '*'

## Example
__Attention the 'setOptimalParams()' function is heuristic, and the optimal parameter values may vary depending on the specific system and matrices!!!__
+ The example demonstrates basic arithmetic operations such as '+' , '-' , '*'
```cpp
#include "lib/Matrix.h"
#include <string>

int main(){
    std::string str0 = "2 -1 -5 0";

    std::string str1 = "-4 -3 3 -1";

    std::string str2 = "-7 -1 0 8\n";

    matrix<int> mxt0(2,2);
    matrix<int> mxt1(2,2);
    matrix<int> mxt2(2,2);

    mxt0.setMatrix(str0);
    mxt1.setMatrix(str1);
    mxt2.setMatrix(str2);
    
    mxt0.setOptimalParams();
    mxt1.setOptimalParams();
    mxt2.setOptimalParams();

    auto newMtx = (mxt2 + mxt0 - mxt1) * mxt1;
}
```
