#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
// 插入排序
void insertsort(vector<int>& arr)
{
    for (int i = 1; i < arr.size(); i++) {
        int key = arr[i];
        int j = i - 1;
        while ((j >= 0) && key < arr[j]) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}
int main()
{
    vector<int> arr = {8,9,485,5,98,4,5,2,5,645661,64,646,14,6121};
    insertsort(arr);
    for (auto e : arr)
    {
        cout << e << " ";
    }
}