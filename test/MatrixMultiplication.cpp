#include "../lib/Matrix.h"
#include <string>

int main(){
    std::string str0 = "2 -1\n"
                      "-5 0\n";

    std::string str1 = "-4 -3\n"
                       "3 -1\n";

    matrix<int> mxt0;
    matrix<int> mxt1;

    mxt0.setMatrix(str0);
    mxt1.setMatrix(str1);

    auto newMtx = mxt0 * mxt1;
    newMtx.display();
}