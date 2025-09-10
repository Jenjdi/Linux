#include <iostream>
#include <vector>
using namespace std;
void countsort(vector<int> &arr)
{
    int n = arr.size();
    int max = arr[0], min = arr[0];
    for (int i = 1; i < n; i++)
    {
        if (arr[i] < min)
            min = arr[i];
        if (arr[i] > max)
            max = arr[i];
    }
    int range = max - min+1;//获取到数组中最大和最小之间的间隔
    vector<int> count(range);//开辟一个类似于哈希表的数组，用于存放对应位置的值的数量
    int j = 0;
    for (int i = 0; i < n; i++)
        count[arr[i] - min]++;//计算每个位置的值的数量分别为多少，同时减去min是为了减少空间占用
    for(int i=0;i<count.size();i++)
    {
        while(count[i]--)
        {
            arr[j++]=i+min;//从头取出数据将其放回到原数组即可得到排好序的数组
        }
    }
}
int main()
{
    vector<int> arr = {2, 8, 1, 8, 1, 8, 5, 6, 3, 4, 54, 854, 564, 561};
    countsort(arr);
    for (auto &e : arr)
    {
        cout << e << " ";
    }
}