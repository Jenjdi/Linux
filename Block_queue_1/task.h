#include <iostream>
#include <functional>
#include<curl/curl.h>
using namespace std;
class Task
{
private:
    int _x;
    int _y;

public:
    Task(int x, int y)
        : _x(x),
          _y(y) {}
    Task() {}
    void Execute()
    {
        cout << "x+y= " << _x + _y << endl;
    }
    void Download()
    {
        
    }
};