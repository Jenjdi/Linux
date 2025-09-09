#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
void shell_sort(vector<int>& arr)
{
    int gap=arr.size();
    while(gap>0)
    {
        gap/=2;
        for(int i=0;i<arr.size()-gap;i++)
        {
            int end=i;
            int key=arr[end+gap];
            while(arr[end]>key&&end>=0)
            {
                arr[end+gap]=arr[end];
                end-=gap;
            }
            arr[end+gap]=key;
        }
    }
}
int main()
{
    vector<int> arr = {2, 8, 1, 8, 1, 8, 5, 6, 3, 4, 54, 854, 564, 561};
    shell_sort(arr);
    for (auto &e : arr)
    {
        cout << e << " ";
    }
}