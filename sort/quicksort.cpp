#include<iostream>
#include<vector>
#include<algorithm>
#include<stdlib.h>
using namespace std;
void quicksort(vector<int>&arr, int low, int high) {
    int i = low; 
    int j = high;
    if(i >= j) {
        return;
    }
 
    int temp = arr[low];
    while(i != j) {
        while(arr[j] >= temp && i < j) {
            j--;
        }
	while(arr[i] <= temp && i < j) {
            i++;
        }
	if(i < j) {
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[low], arr[i]);
    quicksort(arr, low, i - 1);
    quicksort(arr, i + 1, high);
}
int main()
{
    
    vector<int> arr={10,9,8,7,6,5,4,3,2,1};
    quicksort(arr,0,10);
    for(int i=0;i<10;i++)
    {
        cout<<arr[i]<<" ";
    }
}