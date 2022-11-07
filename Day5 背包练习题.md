# 背包练习题

[1155. 掷骰子的N种方法](https://leetcode-cn.com/problems/number-of-dice-rolls-with-target-sum/)

```cpp
//1155, 1<=n,k<=30,1<=target<=1000
class Solution {
    const int mod = 1e9+7;
    int dp[40][1005];
public:
    int numRollsToTarget(int n, int k, int target) {
        dp[0][0] = 1;
        for (int x = 1; x <= target; ++x) {
            for (int i = 0; i < n; ++i) {//和上一层循环可以交换, 为什么??
                for (int j = 1; j <= k; ++j) {
                    if (x >= j) dp[i + 1][x] = (dp[i + 1][x] + dp[i][x - j]) % mod;
                }
            }
        }
        return dp[n][target] % mod;
    }
};
```



[1449. 数位成本和为目标值的最大数字](https://leetcode-cn.com/problems/form-largest-integer-with-digits-that-add-up-to-target/) （**初始化是难点**）

```cpp
//1449,len(cost)==9,1<=cost[i]<=5000,1<=target<=5000
class Solution {
public:
    string largestNumber(vector<int>& cost, int target) {
        vector<string> dp(target + 1, "0");
        dp[0] = "";
        for (int i = 0; i < 9; ++i) {
            for (int x = cost[i]; x <= target; ++x) {
                if (dp[x - cost[i]] != "0") {
                    string s = to_string(i + 1) + dp[x - cost[i]];
                    if (s.size() > dp[x].size() || s.size() == dp[x].size() && s > dp[x]) dp[x] = s;
                }
            }
        }
        return dp[target];
    }
};
```



[879. 盈利计划](https://leetcode-cn.com/problems/profitable-schemes/) 

```cpp
//879
class Solution {
public:
    int profitableSchemes(int n, int minProfit, vector<int>& group, vector<int>& profit) {
        int m = group.size(), mod = 1e9+7;
        vector<vector<int>> dp(n + 1, vector<int>(minProfit + 1));
        dp[0][0] = 1;
        for (int i = 0; i < m; ++i) {
            for (int x = n; x >= group[i]; --x) {
                for (int y = minProfit; y >= 0; --y) {
                    dp[x][y] = (dp[x][y] + dp[x - group[i]][max(0, y - profit[i])]) % mod;
                }
            }
        }
        int res = 0;
        for (int x = 0; x <= n; ++x) res = (res + dp[x][minProfit]) % mod;
        return res;
    }
};
```

参考 [【宫水三叶】特殊多维费用背包问题解决方案 - 盈利计划](https://leetcode-cn.com/problems/profitable-schemes/solution/gong-shui-san-xie-te-shu-duo-wei-fei-yon-7su9/)

