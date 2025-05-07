#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
// 冒泡排序
void bubblesort(vector<int>& arr)
{
    for (int i = 0; i < arr.size() - 1; i++) {
        for (int j = 0; j < arr.size() - 1 - i; j++)
        // 每进行一次排序，就会确定最后一个位置的数字，
        // 因此每进行一次循环，就去除掉最后一个位置
        {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j] , arr[j + 1]);
            }
        }
    }
}
int main()
{
    vector<int> arr = {8,9,485,5,98,4,5,2,5,645661,64,646,14,6121};
    bubblesort(arr);
    for (auto e : arr)
    {
        cout << e << " ";
    }
}