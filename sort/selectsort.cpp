#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
// 选择排序
// void selectsort(vector<int>& arr)
// {
//     for (int i = 0; i < arr.size()-1; i++) {
//         int minnum = i;
//         for (int j = i + 1; j < arr.size(); j++) {
//             if (arr[j] < arr[minnum]){
//                 minnum = j;
//             }
//         }
//         if (minnum != i)
//         {
//             swap(arr[minnum], arr[i]);
//         }
//     }
// }
void selectsort(vector<int>& arr)
{
    int left=0,right=arr.size()-1;
    
    while(left<right)
    {
        int min=left,max=left;
        for(int i=left+1;i<right;i++)
        {
            if(arr[i]<arr[min])
                min=i;
            if(arr[i]>arr[max])
                max=i;
        }
        swap(arr[min],arr[left]);
        if(max==left)
        {
            max=min;
        }
        swap(arr[max],arr[right]);
        left++;
        right--;
    }
}
int main()
{
    vector<int> arr = {8,9,485,5,98,4,5,2,5,645661,64,646,14,6121};
    selectsort(arr);
    for (auto e : arr)
    {
        cout << e << " ";
    }
}