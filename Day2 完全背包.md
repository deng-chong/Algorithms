# 完全背包

### 参考资料

宫水三叶：[分享｜深度讲解背包问题](https://leetcode-cn.com/circle/discuss/GWpXCM/) 

### 完全背包

**问题描述**：有 `n` 种物品，价值和体积分别为 `value[i]` 和 `sz[i]`，每个物品均有无数件。现有一个容量为 `cap` 的背包，求可以装的物品的最大价值。

$\max_{x_i\in\{0\}\bigcup \mathrm{N}_+}\sum_{i=1}^nvalue[i]\cdot x_i$

$s.t.\ \sum_{i=1}^nsz[i]\cdot x_i\leqslant cap$

**代码**：

```cpp
int maxValue(vector<int>& value, vector<int>&sz, int cap) {
    int n = value.size();
    vector<int> dp(1 + cap);
    for (int i = 0; i < n; ++i) {
        for (int x = sz[i]; x <= cap; ++x) {
            dp[x] = max(dp[x], dp[x - sz[i]] + value[i]);
        }
    }
    return dp[cap];
}
```

**题目**：

[322. 零钱兑换](https://leetcode.cn/problems/coin-change/)，[279. 完全平方数](https://leetcode.cn/problems/perfect-squares/)，[518. 零钱兑换 II](https://leetcode.cn/problems/coin-change-2/) ，[377. 组合总和 Ⅳ](https://leetcode.cn/problems/combination-sum-iv/)

```cpp
//322 零钱兑换
class Solution {
public:
    //n<=12,amount<=10000
    int coinChange(vector<int>& coins, int amount) {
        int n = coins.size(), inf = 0x3f3f3f3f;
        vector<int> dp(1 + amount, inf);//求最小，所以用inf初始化
        dp[0] = 0;//别忘了
        for (int i = 0; i < n; ++i) {
            for (int x = coins[i]; x <= amount; ++x) {
                dp[x] = min(dp[x], 1 + dp[x - coins[i]]);
            }
        }
        return dp[amount] == inf ? -1 : dp[amount];
    }
};
```

```cpp
//279 完全平方数
class Solution {
public:
    //1<=n<=10000
    int numSquares(int n) {
        vector<int> dp(1 + n, n);
        dp[0] = 0;
        for (int i = 1; i <= (int)sqrt(n); ++i) {
            for (int x = i * i; x <= n; ++x) {
                dp[x] = min(dp[x], 1 + dp[x - i * i]);
            }
        }
        return dp[n];
    }
};
```


```cpp
//518 零钱兑换II
class Solution {
public:
    //n<=300,amount<=5000
    int change(int amount, vector<int>& coins) {
        int n = coins.size();
        vector<int> dp(1 + amount);
        dp[0] = 1;//别忘了
        for (int i = 0; i < n; ++i) {
            for (int x = coins[i]; x <= amount; ++x) {
                dp[x] += dp[x - coins[i]];
            }
        }
        return dp[amount];
    }
};
```

```cpp
//377. 组合总和 Ⅳ（其实是求排列数！）
class Solution {
    //如果求组合数就是外层for循环遍历物品，内层for遍历背包。
    //如果求排列数就是外层for遍历背包，内层for循环遍历物品。
    //tip: 看作爬楼梯，nums数组中的值想象成一次可以攀爬的步数
public:
    //n<=200,1<=nums[i]<=1000,1<=target<=1000
    int combinationSum4(vector<int>& nums, int target) {
        int n = nums.size(), inf = 0x3f3f3f3f;
        vector<int> dp(target + 1);
        dp[0] = 1;
        for (int x = 0; x <= target; ++x) {
            for (auto num:nums) {
                if (num <= x && dp[x - num] < INT_MAX - dp[x]) //注意有些中间dp值会很大，所以要避免溢出
                    dp[x] += dp[x - num];
            }
        }
        return dp[target];
    }
};
```

**遍历顺序**：

组合（不考虑顺序）：先物品后背包 **（常见）**

排列（考虑顺序）：先背包后物品

518题和377题就是例子，前者求组合数，后者求排列数。



