#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
//整体再顺一遍
void AdjustDown(vector<int>& arr,int parent,int size)
{
    int child=parent*2+1;
    while(child<size)//对一个父节点调整完成后，其子节点也需要继续进行调整
    {
        if(child+1<size&&arr[child]<arr[child+1])//找到最大的孩子结点
            child++;
        if(arr[parent]<arr[child])
        {
            swap(arr[child],arr[parent]);
            parent=child;
            child=parent*2+1;//继续找到孙结点，不断向下调整
        }
        else
            break;
    }
}
void heapsort(vector<int>& arr)
{
    int n=arr.size();
    for(int i=n/2-1;i>=0;i--)//从最后一个父节点开始调整
    {
        AdjustDown(arr,i,n-1);
    }
    for(int end=n-1;end>0;end--)
    {
        swap(arr[0],arr[end]);
        AdjustDown(arr,0,end-1);
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