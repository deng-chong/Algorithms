# 多重背包

### 参考资料

宫水三叶：[分享｜深度讲解背包问题](https://leetcode-cn.com/circle/discuss/GWpXCM/) 

### 多重背包

**问题描述**：有 `n` 种物品，其中第 `i` 种物品有 `nums[i]` 件，价值和体积分别为 `value[i]` 和 `sz[i]`。现有一个容量为 `cap` 的背包，求能装的东西的最大价值。

$\max_{0\leqslant x_i\leqslant nums[i]}\sum_{i=1}^nvalue[i]\cdot x_i$

$s.t.\ \sum_{i=1}^nsz[i]\cdot x_i\leqslant cap$

**代码**：

最基本版本

```cpp
//最基本解法，时间复杂度O(n*cap^2)
int maxValue(vector<int>& nums, vector<int>& value, vector<int>& sz, int cap){
    int n = nums.size();
    vector<int> dp(1 + cap);
    for (int i = 0; i < n; ++i) {
        for (int x = cap; x >= sz[i]; --x) {//从大到小遍历！
            for (int k = 0; k <= nums[i] && k <= x / sz[i]; ++k)
                dp[x] = max(dp[x], k * value[i] + dp[x - k * sz[i]]);
        }
    }
    return dp[cap];
} 
```

二进制优化版本

```cpp
//二进制优化，O(cap*sum(log(nums[i])))
int maxValue(vector<int>& nums, vector<int>& value, vector<int>& sz, int cap) {
    vector<int> v1, v2;
    //扁平化：nums[i]份->ceil(log(nums[i]))份
    for (int i = 0; i < (int)nums.size(); ++i) {
        int num = nums[i];
        for (int k = 1; k <= num; k <<= 1) {
            num -= k;
            v1.push_back(k * value[i]);
            v2.push_back(k * sz[i]);
        }
        if (num > 0) {
            v1.push_back(num * value[i]);
            v2.push_back(num * sz[i]);
        }
    }
    
    int n = v1.size();
    vector<int> dp(1 + cap);
    for (int i = 0; i < n; ++i) {
        for (int x = cap; x >= v2[i]; --x) {
            dp[x] = max(dp[x], v1[i] + dp[x - v2[i]]);
        }
    }
    return dp[cap];
}
```

