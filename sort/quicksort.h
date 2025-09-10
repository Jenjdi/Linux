#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <stack>
using namespace std;
// 霍尔法
//  void quicksort(vector<int>&arr, int low, int high) {
//      int i = low;
//      int j = high;
//      if(i >= j) {
//          return;
//      }
//     int temp = arr[low];
//     while(i != j) {
//         while(arr[j] >= temp && i < j) {
//             j--;
//         }
// 	        while(arr[i] <= temp && i < j) {
//             i++;
//         }
// 	        if(i < j) {
//             swap(arr[i], arr[j]);
//         }
//     }
//     swap(arr[low], arr[i]);
//     quicksort(arr, low, i - 1);
//     quicksort(arr, i + 1, high);
// }
// 挖坑法
// void quicksort(vector<int> &arr, int begin, int end)
// {
//     if(begin>=end)
//         return;
//     int key = arr[begin];
//     int hole = begin;
//     int left = begin, right = end;
//     while (left < right)
//     {
//         while (left < right && arr[right] >= key)
//         {
//             right--;
//         }
//         if (arr[right] < key)
//         {
//             arr[hole] = arr[right];
//             hole = right;
//         }
//         while (left < right && arr[left] <= key)
//         {
//             left++;
//         }
//         if (arr[left] > key)
//         {
//             arr[hole] = arr[left];
//             hole = left;
//         }
//     }
//     arr[hole] = key;
//     quicksort(arr, begin, hole - 1);
//     quicksort(arr, hole + 1, end);
// }

// 前后指针法
// int partition(vector<int> &arr, int begin, int end)
// {
//     int key = arr[begin];
//     int keyi = begin;
//     int prev = begin, cur = begin + 1;
//     while (cur <= end)
//     {
//         if (arr[cur] < key)
//         {
//             prev++;
//             swap(arr[prev], arr[cur]);
//         }
//         cur++;
//     }
//     swap(arr[prev], arr[keyi]);
//     keyi = prev;
//     return keyi;
//     // quicksort(arr, begin, keyi - 1);
//     // quicksort(arr, keyi + 1, end);
// }

// 递归形式
// void quicksort(vector<int> &arr, int begin, int end)
// {
//     if (begin >= end)
//         return;
//     int keyi = partition(arr, begin, end);
//     quicksort(arr, begin, keyi - 1);
//     quicksort(arr, keyi + 1, end);
// }
// // 非递归形式
// void quicksort_NonRecursive(vector<int> &arr, int begin, int end)
// {
//     stack<int> st;
//     st.push(end);
//     st.push(begin);
//     while (!st.empty())
//     {
//         int left = st.top();
//         st.pop();
//         int right = st.top();
//         st.pop();
//         int keyi = partition(arr, left, right);
//         //[left,keyi-1],keyi,[keyi+1,right]
//         if (left < keyi - 1)
//         {
//             st.push(keyi - 1);
//             st.push(left);
//         }
//         if (right > keyi + 1)
//         {
//             st.push(right);
//             st.push(keyi + 1);
//         }
//     }
// }
// 霍尔法
int partition1(vector<int> &arr, int begin, int end)
{
    int key = begin;
    int left = begin, right = end;
    while (left < right)
    {
        while (left < right && arr[right] >= arr[key])
            right--;
        while (left < right && arr[left] <= arr[key])
            left++;
        swap(arr[left], arr[right]);
    }
    swap(arr[key], arr[left]);
    key = left;
    return key;
}
// 挖坑法
int partition2(vector<int> &arr, int begin, int end)
{
    int key = arr[begin];
    int hole = begin;
    int left = begin, right = end;
    while (left < right)
    {
        while (left < right && arr[right] >= key)
            right--;
        if (arr[right] < key)
        {
            arr[hole] = arr[right];
            hole = right;
        }

        while (left < right && arr[left] <= key)
            left++;
        if (arr[left] > key)
        {
            arr[hole] = arr[left];
            hole = left;
        }
    }
    arr[hole] = key;
    return hole;
}

//前后指针法
int partition3(vector<int>& arr,int begin,int end)
{
    int prev=begin,cur=prev+1;
    int key=begin;
    while(cur<=end)
    {
        if(arr[cur]<arr[key])
        {
            prev++;
            swap(arr[prev],arr[cur]);
        }
        cur++;
    }
    swap(arr[prev],arr[key]);
    key=prev;
    return key;
}

void QuickSortR(vector<int> &arr, int begin, int end)
{
    if (begin >= end)
        return;
    int key = partition3(arr, begin, end);
    QuickSortR(arr, begin, key - 1);
    QuickSortR(arr, key + 1, end);
}
void QuickSortN(vector<int> &arr, int begin, int end)
{
    stack<int> st;

    st.push(end-1);
    st.push(begin);
    while (!st.empty())
    {
        int left = st.top();
        st.pop();
        int right = st.top();
        st.pop();
        int key = partition3(arr, left, right);
        //[left,key-1],key,[key+1,right]
        if (left < key - 1)
        {
            st.push(key - 1);
            st.push(left);
        }
        if (right > key + 1)
        {
            st.push(right);
            st.push(key+1);
        }
    }
}
// int main()
// {
//     vector<int> arr = {2, 8, 1, 8, 1, 8, 5, 6, 3, 4, 54, 854, 564, 561};
//     QuickSortN(arr,0,arr.size());
//     for (auto &e : arr)
//     {
//         cout << e << " ";
//     }
// }
