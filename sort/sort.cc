#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
// template<class T>
// void insert_sort(vector<T>& v)
// {
//     for(int i=0;i<v.size()-1;i++)
//     {
//         int end=i;
//         T key=v[end+1];
//         while(end>=0&&key<v[end])
//         {
//             v[end+1]=v[end];
//             end--;
//         }
//         v[end+1]=key;
//     }
// }
// int main()
// {
//     vector<int> arr={2,8,1,8,1,8,5,6,1,4,54,854,564,561};
//     insert_sort(arr);
//     for(auto& e: arr)
//     {
//         cout<<e<<" ";
//     }
// }
// void shell_sort_v1(vector<int> &arr)
// {
//     int gap = 3;
//     for (int j = 0; j < gap; j++)                       // 前j个表示已经排好序的
//     {                                                   // 这种方法是一组一组排
//         for (int i = j; i < arr.size() - gap; i += gap) // 进行一次间隔为gap的插入排序
//         {
//             int end = i;
//             int key = arr[end + gap];
//             while (end >= 0 && key < arr[end])
//             {
//                 arr[end + gap] = arr[end];
//                 end -= gap;
//             }
//             arr[end + gap] = key;
//         }
//     }
//     //最终并不是有序，还需要进行一次gap为1的插入排序才能完全有序
// }
// void shell_sort_v2(vector<int> &arr)
// {
//     int gap = 3;
//     // 这种方法是多组并排
//     for (int i = 0; i < arr.size() - gap; ++i) // 进行一次间隔为gap的插入排序
//     {
//         int end = i;
//         int key = arr[end + gap];
//         while (end >= 0 && key < arr[end])
//         {
//             arr[end + gap] = arr[end];
//             end -= gap;
//         }
//         arr[end + gap] = key;
//     }
//     //最终并不是有序，还需要进行一次gap为1的插入排序才能完全有序
// }

// void shell_sort_v3(vector<int> &arr)
// {
//     int gap = arr.size();
//     while (gap > 1)
//     {
//         gap /= 2;                                  // 动态调整gap，先接近有序，最后一次gap=1令其有序
//         for (int i = 0; i < arr.size() - gap; ++i) // 进行一次间隔为gap的插入排序
//         {
//             int end = i;
//             int key = arr[end + gap];
//             while (end >= 0 && key < arr[end])
//             {
//                 arr[end + gap] = arr[end];
//                 end -= gap;
//             }
//             arr[end + gap] = key;
//         }
//     }
// }

// void select_sort(vector<int> &arr)
// {
//     int begin = 0, end = arr.size() - 1;
//     while (begin < end)
//     {
//         int mini = begin, maxi = begin; // 双指针一次选两个
//         for (int i = begin + 1; i <= end; i++)
//         {
//             if (arr[i] < arr[mini])
//             {
//                 mini = i;
//             }
//             if (arr[i] > arr[maxi])
//             {
//                 maxi = i;
//             }
//         }
//         swap(arr[begin], arr[mini]);
//         if (maxi == begin) // 当maxi位于起始位置时，会被最小值换走，此时最大值位于原来最小值的位置，因此将maxi改为mini
//         {
//             maxi = mini;
//         }
//         swap(arr[end], arr[maxi]);
//         begin++;
//         end--; // 双指针移动
//     }
// }
// void AdjustUp(vector<int>& arr)
// {
//     int child=arr.size()-1;
//     int parent=(child-1)/2;
//     while (parent>0)
//     {
//         if(arr[child]>arr[parent])
//         {
//             swap(arr[child],arr[parent]);
//             child=parent;
//             parent=(parent-1)/2;
//         }
//         else
//         {
//             break;
//         }
//     }
    
// }

// void heap_sort(vector<int>& arr)
// {
    
// }
int GetMid(vector<int>& arr,int begin,int end)
{
    int mid=(begin+end)/2;
    if(arr[begin]<arr[end])
    {
        if(arr[mid]>arr[begin])
            return mid;
        else if(arr[mid]<arr[begin])
            return begin;
        else
            return end;
    }
    else//end >= begin
    {
        if(arr[mid]<arr[begin])
            return begin;
        else if(arr[mid]>arr[begin])
            return mid;
        else return end;
    }
}

void quick_sort(vector<int>& arr,int begin,int end)
{
    if(begin>=end)
    {
        return;
    }
    // int left=begin,right=end;
    // int keyi=begin;
    // while(left<right)
    // {
    //     while (left<right&&arr[left]<=arr[keyi])
    //     {
    //         left++;
    //     }
    //     while(left<right&&arr[right]>=arr[keyi])
    //     {
    //         right--;
    //     }
    //     swap(arr[left],arr[right]);
    // }
    // swap(arr[keyi],arr[left]);
    // keyi=left;
    // quick_sort(arr,begin,keyi-1);
    // quick_sort(arr,keyi+1,end);
    int midi=GetMid(arr,begin,end);
    swap(arr[begin],arr[midi]);
    int left = begin, right = end;
	int keyi = begin;

	while (left < right)
	{
        //一定要是先找小再找大，为了让相遇位置一定是小于基准值的
		// 右边找小
		while (left < right && arr[right] >= arr[keyi])
		{
			--right;
		}

		// 左边找大
		while (left < right && arr[left] <= arr[keyi])
		{
			++left;
		}

		swap(arr[left], arr[right]);
	}

	swap(arr[left], arr[keyi]);
	keyi = left;

	// [begin, keyi-1] keyi [keyi+1, end]
	quick_sort(arr, begin, keyi - 1);
	quick_sort(arr, keyi+1, end);
}
#include<stdio.h>
int main()
{
    vector<int> arr = {2, 8, 1, 8, 1, 8, 5, 6, 3, 4, 54, 854, 564, 561};
    quick_sort(arr,0,arr.size()-1);
    for (auto &e : arr)
    {
        cout << e << " ";
    }
}
