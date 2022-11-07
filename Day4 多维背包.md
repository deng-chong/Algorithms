# 多维背包

### 参考资料

宫水三叶：[分享｜深度讲解背包问题](https://leetcode-cn.com/circle/discuss/GWpXCM/) 

### 多维背包

多维背包指的是**含有多个对背包的约束条件**。

**问题描述**：有 `n` 种物品，其中第 `i` 种物品有 `nums[i]` 件，价值、体积和重量分别为 `value[i]`、`sz[i]` 和 `weight[i]`。现有一个容量为 `S`、最大承重量为 `W` 的背包，求能装的东西的最大价值。

$\max_{0\leqslant x_i\leqslant nums[i]}\sum_{i=1}^nvalue[i]\cdot x_i$

$s.t.\ \sum_{i=1}^nsz[i]\cdot x_i\leqslant S$

​       $\sum_{i=1}^nweight[i]\cdot x_i\leqslant W$



**代码**： 时间复杂度 $O(nSW)$，空间复杂度 $O(SW)$

```cpp
//多维 01 背包
int maxValue(vector<int>& value, vector<int>& sz, vector<int>& weight, int S, int W) {
    int n = value.size();
    vector<vector<int>> dp(1 + S, vector<int>(1 + W));
    for (int i = 0; i < n; ++i) {
        for (int x = S; x >= sz[i]; --x) {
            for (int y = W; y >= weight[i]; --y) {
                dp[x][y] = max(dp[x][y], value[i] + dp[x - sz[i]][y - weight[i]]);
            }
        }
    }
    return dp[S][W];
}
```

```cpp
//多维 完全 背包
int maxValue(vector<int>& value, vector<int>& sz, vector<int>& weight, int S, int W) {
    int n = value.size();
    vector<vector<int>> dp(1 + S, vector<int>(1 + W));
    for (int i = 0; i < n; ++i) {
        for (int x = sz[i]; x <= S; ++x) {
            for (int y = weight[i]; y <= W, ++y) {
                dp[x][y] = max(dp[x][y], value[i] + dp[x - sz[i]][y - weight[i]]);
            }
        }
    }
    return dp[S][W];
}
```



**题目**：[474. 一和零](https://leetcode-cn.com/problems/ones-and-zeroes/) 

```cpp
//474 一和零
class Solution {
    int dp[101][101];
public:
    int findMaxForm(vector<string>& strs, int m, int n) {
        for (auto &s:strs) {
            int cnt0 = 0, cnt1 = 0;
            for (auto c:s) if (c == '0') ++cnt0; else ++cnt1;
            for (int i = m; i >= cnt0; --i) {
                for (int j = n; j >= cnt1; --j) {
                    dp[i][j] = max(dp[i][j], 1 + dp[i - cnt0][j - cnt1]);
                }
            }
        }
        return dp[m][n];
    }
};

```