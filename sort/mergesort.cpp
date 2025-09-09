#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
void _mergesort(vector<int>& arr,int begin,int end,vector<int>& tmp)
{
    if(begin>=end)
        return;
    int mid=(begin+end)/2;
    _mergesort(arr,begin,mid,tmp);
    _mergesort(arr,mid+1,end,tmp);
    int begin1=begin,end1=mid;
    int begin2=mid+1,end2=end;
    int i=begin;
    while(begin1<=end1&&begin2<=end2)
    {
        if(arr[begin1]<arr[begin2])
            tmp[i++]=arr[begin1++];
        else
            tmp[i++]=arr[begin2++];
    }
    while(begin1<=end1)
    {
        tmp[i++]=arr[begin1++];
    }
    while(begin2<=end2)
        tmp[i++]=arr[begin2++];
    arr=tmp;
}
void mergesort(vector<int>& arr)
{
    vector<int> tmp(arr.size());
    _mergesort(arr,0,arr.size()-1,tmp);
}
int main()
{
    vector<int> arr = {2, 8, 1, 8, 1, 8, 5, 6, 3, 4, 54, 854, 564, 561};
    mergesort(arr);
    for (auto &e : arr)
    {
        cout << e << " ";
    }
}