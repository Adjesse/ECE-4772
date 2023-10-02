#include <iostream>
using namespace std; 

class Circle { 
private: 
    float radius;
public: 
    Circle () {}
    Circle (float ra): radius(ra) {}
    void compute_perimeter () { 
    float perimeter = 3.14*2*radius; 
    cout << "Perimeter is :" << perimeter << endl; 
    } 
    };

int main() { 
 Circle C;
 C.radius = 3;
 C.compute_perimeter();
 return 0; 
}

