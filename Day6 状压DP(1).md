# 状压DP

### 参考资料

[经典动态规划 (八) -- 状态压缩 - YouTube](https://www.youtube.com/watch?v=o5s37gYkMe8) 

### 要点

![image-20220425102411854](C:\Users\19751\AppData\Roaming\Typora\typora-user-images\image-20220425102411854.png)

![image-20220425102531064](C:\Users\19751\AppData\Roaming\Typora\typora-user-images\image-20220425102531064.png)



![image-20220425102613115](C:\Users\19751\AppData\Roaming\Typora\typora-user-images\image-20220425102613115.png)

- 枚举子集，或用做标记数组 `visited`

- 数据量很小，指数级时间复杂度
- 借助记忆化存储，可以将 $O(n!)$ 的时间复杂度降低至 $O(poly(n)\cdot2^n)$ 

### 题目

[526. 优美的排列](https://leetcode.cn/problems/beautiful-arrangement/)

```cpp
//526, 没有备忘录的dfs搜索, O(n!), 76ms
class Solution {
    int f[16];
public:
    //1<=n<=15
    int countArrangement(int n) { 
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= n; ++j) if (i % j == 0 || j % i == 0)
            f[i] |= 1 << j;
        }
        int res = 0;
        function<void(int, int)> dfs = [&] (int idx, int state) {
            if (idx > n) {
                ++res;
                return;
            }
            for (int i = 1; i <= n; ++i) {
                if (((f[idx] >> i) & 1) && !((state >> i) & 1)) {
                    dfs(idx + 1, state | (1 << i));
                }
            }
        };
        dfs(1, 0);
        return res;
    }
};
```

**加上备忘录**

```cpp
//526, 加了备忘录的dfs搜索, O(n*2^n), 4ms
class Solution {
    int f[16];
    int memo[1<<16];
public:
    //1<=n<=15
    int countArrangement(int n) {
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= n; ++j) if (i % j == 0 || j % i == 0)
            f[i] |= 1 << j;
        }
        memset(memo, -1, sizeof(memo));
        function<int(int, int)> dfs = [&] (int idx, int state) {
            if (idx > n) return 1;
            if (memo[state] >= 0) return memo[state];
            int res = 0;
            for (int i = 1; i <= n; ++i) {
                if (((f[idx] >> i) & 1) && !((state >> i) & 1)) {
                    res += dfs(idx + 1, state | (1 << i));
                }
            }
            return memo[state] = res;
        };
        return dfs(1, 0);
    }
};
```

**tip**：这题从后往前dfs会更快点(即`idx`从`n`递减至`0`)，`n=20`时，两者用时分别为 60ms 和 20ms。

```cpp
//526, 官解, 自下而上, O(n*2^n), 4ms
class Solution {
    int dp[1<<16];
public:
    //1<=n<=15
    int countArrangement(int n) {
        dp[0] = 1;
        for (int x = 1; x < (1 << n); ++x) {
            int k = __builtin_popcount(x);//给数字k找个位置放下, 类似idx
            for (int i = 0; i < n; ++i) {
                if (((x >> i) & 1) && (k % (i + 1) == 0 || (i + 1) % k == 0)) 
                    dp[x] += dp[x - (1 << i)];
            }
        }
        return dp[(1 << n) - 1];
    }
};
```



### 练习题

[464. 我能赢吗](https://leetcode.cn/problems/can-i-win/)

```cpp
//464, O(n*2^n), 80ms
class Solution {
    int dp[1<<20];
public:
    //1<=n<=20, 0<=tot<=300
    bool canIWin(int n, int tot) {
        if (tot > n*(1+n)/2) return false;
        memset(dp, -1, sizeof(dp));
        return dfs(n, (1 << n) - 1, tot);
    }

    int dfs(int n, int state, int tot) {
        if (dp[state] >= 0) return dp[state];
        if (!state) return dp[state] = 0;
        for (int i = 0; i < n; ++i) if (state & (1<<i) && i + 1 >= tot)
            return dp[state] = 1;
        for (int i = 0; i < n; ++i) if (state & (1<<i) && !dfs(n, state & ~(1<<i), tot - i - 1))
            return dp[state] = 1;
        return dp[state] = 0;
    }
};
```





