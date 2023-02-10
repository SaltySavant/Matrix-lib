# Matrix C++ library 
[![Language](https://img.shields.io/badge/language-С++-blue.svg)](https://isocpp.org/) [![Standard](https://img.shields.io/badge/c%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
## Requirements
No special requirement. You should already have this :

+ C++17 compatible compiler.

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
    /*
      Initializing a vec
    */
    matrix<int> mtx(myMatrix);
```
+ Initializing a matrix based on std::string

  It is important to put a space (' ') between the elements of the rows and '\n' between the columns in the variable std::string
```cpp
    std::string str = "2 -1 1\n"
                      "-5 0 2\n";
    matrix<int> mxt;
    mxt.setMatrix(str);
```
+ Load a matrix from a file

  It is important to put a space (' ') between the row elements and '\n' between the columns in the file
```cpp
    std::string filename = "matrix.txt"
    matrix<int> mxt;
    mxt.loadMatrix(filename);
```

**Now the library can solve such problems as:**

1. Arithmetic operations on matrices, namely '+' , '-' , '*'

## Example
+ The example demonstrates basic arithmetic operations such as '+' , '-' , '*'
```cpp
#include "lib/Matrix.h"
#include <string>

int main(){
    std::string str0 = "2 -1\n"
                      "-5 0\n";

    std::string str1 = "-4 -3\n"
                       "3 -1\n";

    std::string str2 = "-7 -1\n"
                       "0 8\n";

    matrix<int> mxt0;
    matrix<int> mxt1;
    matrix<int> mxt2;

    mxt0.setMatrix(str0);
    mxt1.setMatrix(str1);
    mxt2.setMatrix(str2);

    auto newMtx = (mxt2 + mxt0 - mxt1) * mxt1;
    newMtx.display();
}
```
