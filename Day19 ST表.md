# ST表

### 参考

[ST 表 - OI Wiki (oi-wiki.org)](https://oi-wiki.org/ds/sparse-table/)

**问题**：快速计算 `nums[l] op nums[l + 1] op ... op nums[r]`

**适用情况**：可重复贡献 (x op x = x)，数组不可修改。

**复杂度**：预处理 $O(n\log n)$，查询 $O(1)$。

**注**：1. 求和是不可重复贡献运算，不能使用ST表，但可以借助前缀和数组实现 $O(n)$预处理和$O(1)$查询；相对地，取最大/最小/按位与/按位或等运算属于可重复贡献，可用ST表实现快速查询。）

2. 不管哪类运算，我们均可借助线段树实现 $O(\log n)$ 时间查询；但对可重复贡献运算，用ST表可以算得更快，但代价是不允许修改数组！

### 代码

```cpp
class ST {
    int f[100005][21];
    int Logn[100005];
    
public:
    ST(){}
    ST(vector<int>& nums) {
        int n = nums.size();
        f.resize(n), Logn.resize(n + 1);
        Logn[1] = 0, Logn[2] = 1;
        for (int i = 3; i <= n; ++i) Logn[i] = 1 + Logn[i >> 1];
        for (int i = 0; i < n; ++i) f[i][0] = nums[i];
        for (int i = n - 1; i >= 0; --i) {
            for (int j = 1; i + (1 << j) - 1 < n ; ++j) {
                f[i][j] = max(f[i][j - 1], f[i + (1 << (j - 1))][j - 1]);
            }
        }
    }
    int query(int L, int R) {
        int x = Logn[R - L + 1];
        return max(f[L][x], f[R - (1 << x) + 1][x]);
    }
};
```



### 题目

[1521. 找到最接近目标值的函数值 - 力扣（LeetCode）](https://leetcode.cn/problems/find-a-value-of-a-mysterious-function-closest-to-target/)

