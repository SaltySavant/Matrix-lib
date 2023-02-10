#include "../lib/Matrix.h"
#include <string>
#include <iostream>

int main(){
    std::string str = "2 -1 1\n"
                      "-5 0 2\n";

    std::string str2 = "-4 -3\n"
                       "3 -1\n"
                       "6 2\n";

    matrix<int> mxt;
    matrix<int> mxt2;
    mxt.setMatrix(str);
    mxt2.setMatrix(str2);
    auto x = mxt2 * mxt;
    auto y = mxt * mxt2;
    x.display();
    std::cout << std::endl;
    y.display();

}