#include<iostream>
#include<vector>
using namespace std;
int main()
{
    int m,n;
    m=3,n=4;
    vector<vector<int>> nums((m),vector<int>(n));
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            nums[i][j]=1;
        }
    }
    vector<vector<int>> dp(m+1,vector<int>(n+1));
    for(int i=1;i<=m;i++)
    {
        for(int j=1;j<=n;j++)
        {
            dp[i][j]=dp[i-1][j]+dp[i][j-1]+nums[i-1][j-1];
        }
    }
    cout<<dp[m][n];
}