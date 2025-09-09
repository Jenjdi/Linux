#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
void AdjustDown(vector<int>& arr,int parent,int size)
{
    int child=parent*2+1;
    while(child<size)
    {
        if(child+1<size&&arr[child]<arr[child+1])//找到最大的孩子结点
            child++;
        if(arr[parent]>arr[child])
        {
            swap(arr[child],arr[parent]);
            parent=child;
        }
        else
            break;
    }
}
void heapsort(vector<int>& arr)
{
    int n=arr.size();
    for(int i=n/2-1;i>=0;i--)
    {
        AdjustDown(arr,i,n);
    }
    for(int end=n-1;end>0;end--)
    {
        swap(arr[0],arr[end]);
        AdjustDown(arr,0,end);
    }
}
int main()
{
    vector<int> arr = {2, 8, 1, 8, 1, 8, 5, 6, 3, 4, 54, 854, 564, 561};
    heapsort(arr);
    for (auto &e : arr)
    {
        cout << e << " ";
    }
}