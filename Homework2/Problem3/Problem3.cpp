#include <iostream>
using namespace std; 
class Sample { 
public:
    int x, y, z, s; 
    Sample (): x(2), y(3), z(4) {}
    Sample (int xa, int ya, int za): x(xa), y(ya), z(za) {}
    int operation() { 
    s = x*y*z; 
    return s; } 
    int operation(int offset) { 
    s = x*y*z + offset; 
    return s; } 
    }; 
int main() { 
    int result_1, result_2; 
    Sample S1(4,5,6);
    Sample S2;
    result_1 = S1.operation();
    result_2 = S2.operation(8);
    cout << "Result (S1): " << result_1 << endl; 
    cout << "Result (S2): " << result_2 << endl; 
    return 0; 
    }
