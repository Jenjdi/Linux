#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
// void _mergesort(vector<int>& arr,int begin,int end,vector<int>& tmp)
// {
//     if(begin>=end)
//         return;
//     int mid=(begin+end)/2;
//     _mergesort(arr,begin,mid,tmp);
//     _mergesort(arr,mid+1,end,tmp);
//     int begin1=begin,end1=mid;
//     int begin2=mid+1,end2=end;
//     int i=begin;
//     while(begin1<=end1&&begin2<=end2)
//     {
//         if(arr[begin1]<arr[begin2])
//             tmp[i++]=arr[begin1++];
//         else
//             tmp[i++]=arr[begin2++];
//     }
//     while(begin1<=end1)
//     {
//         tmp[i++]=arr[begin1++];
//     }
//     while(begin2<=end2)
//         tmp[i++]=arr[begin2++];
//     arr=tmp;
// }
// void mergesort(vector<int>& arr)
// {
//     vector<int> tmp(arr.size());
//     _mergesort(arr,0,arr.size()-1,tmp);
// }
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
        tmp[i++]=arr[begin1++];
    while(begin2<=end2)
        tmp[i++]=arr[begin2++];
    arr=tmp;
}
void _mergesortN(vector<int>& arr,int begin,int end,vector<int>& tmp)
{
    int gap=1;
    while(gap<end)
    {
        //数组可以直接按照一个一个的进行分组，省略掉不断向下递归的过程
        for(int i=begin;i<end;i+=gap*2)
        {
            //[i,i+gap-1],[i+gap,2*gap-1]
            //[begin1,end1],[begin2,end2]
            int begin1=i,end1=i+gap-1;
            int begin2=i+gap,end2=i+2*gap-1;
            if(end1>=end||begin2>=end)//防止在循环过程中发生越界，一旦end1或者begin2发生了越界，说明当前不足或只有一组
            //因此直接退出当前循环即可
                break;
            if(end2>=end)//end2越界直接将end2设置为数组的最后位置即可
                end2=end-1;
            int j=begin1;
            while(begin1<=end1&&begin2<=end2)
            {
                if(arr[begin1]<arr[begin2])
                    tmp[j++]=arr[begin1++];
                else
                    tmp[j++]=arr[begin2++];
            }
            while(begin1<=end1)
                tmp[j++]=arr[begin1++];
            while(begin2<=end2)
                tmp[j++]=arr[begin2++];
            arr=tmp;
        }
        gap*=2;
    }
}
void mergesort(vector<int>& arr)
{
    vector<int> tmp(arr);
    _mergesortN(arr,0,arr.size(),tmp);
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