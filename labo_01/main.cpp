#include <iostream>
#include "splaytree.h"

int main() {
    SplayTree<int, std::string> b ;
    std::cout << "empty tree" << std::endl;
    b.schrijf(std::cout);

    b.voegtoe(1, "een", false);
    b.voegtoe(5, "vijf", false);
    b.voegtoe(3, "drie", false);
    b.voegtoe(9, "negen",false);
    b.voegtoe(7, "zeven", false);

    b.schrijf(std::cout);
    b.teken("figure");
    std::cout << b.geefDiepte() << std::endl;


    return 0;
}