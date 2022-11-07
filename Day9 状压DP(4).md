# 状压DP







### 题目

[691. 贴纸拼词](https://leetcode.cn/problems/stickers-to-spell-word/)

解法一

```cpp
//691 dfs+memo
class Solution {
    int cnt[26], dp[1<<15];
    const int inf = 0x3f3f3f3f;
public:
    int minStickers(vector<string>& stickers, string target) {
        if (!check(stickers, target)) return -1;
        int n = target.size();
        memset(dp, 0x3f, sizeof(dp));
        int res = dfs(stickers, target, (1 << n) - 1);
        return res == inf ? -1 : res;
    }

    int dfs(vector<string>& stickers, string& target, int state) {
        if (dp[state] < inf) return dp[state];
        if (!state) return dp[state] = 0;
        int n = target.size();
        for (auto &s:stickers) {
            int nextState = state;
            memset(cnt, 0, sizeof(cnt));
            for (auto c:s) ++cnt[c - 'a'];
            for (int i = 0; i < n; ++i) if (state & (1 << i)){
                if (cnt[target[i] - 'a']) {
                    --cnt[target[i] - 'a'];
                    nextState ^= 1 << i;
                }
            }
            if (nextState != state)
                dp[state] = min(dp[state], 1 + dfs(stickers, target, nextState));
        }
        return dp[state];
    }

    bool check(vector<string>& stickers, string &target) {
        for (auto &s:stickers) for (auto c:s) ++cnt[c - 'a'];
        for (auto c:target) if (!cnt[c - 'a']) return false;
        return true;
    }
};
```

解法二

```cpp
//691 bfs
class Solution {
    int dp[1 << 15], cnt[26];
public:
    int minStickers(vector<string>& stickers, string target) {
        int n = target.size();
        memset(dp, -1, sizeof(dp));
        queue<int> q;
        dp[0] = 0;
        q.push(0);
        while(!q.empty()) {
            int state = q.front();
            q.pop();
            for (auto &s:stickers) {
                memset(cnt, 0, sizeof(cnt));
                for (auto c:s) ++cnt[c - 'a'];
                int nextState = state;
                for (int i = 0; i < n; ++i) {
                    if (!((nextState >> i) & 1) && cnt[target[i] - 'a'] > 0) {
                        nextState |= 1 << i;
                        --cnt[target[i] - 'a'];
                    }
                }
                if (dp[nextState] < 0) {
                    dp[nextState] = 1 + dp[state];
                    if (nextState == (1 << n) - 1) return dp[nextState];
                    q.push(nextState);
                }
            }
        }
        return -1;
    }
};
```



[805. 数组的均值分割](https://leetcode.cn/problems/split-array-with-same-average/)

```cpp
//805, 折半搜索
class Solution {
public:
    //1<=n<=30, 0<=nums[i]<=1e4
    bool splitArraySameAverage(vector<int>& nums) {
        int n = nums.size(), sum = accumulate(nums.begin(), nums.end(), 0);
        vector<unordered_set<int>> v(n / 2 + 1);
        for (int x = 0; x < (1 << n/2); ++x) {
            int s = 0, k = __builtin_popcount(x);
            for (int i = 0; i < n / 2; ++i) if ((x >> i) & 1) s += nums[i];
            v[k].insert(s);
        }
        for (int x = 0; x < (1 << (n + 1)/2); ++x) {
            int s = 0, k = __builtin_popcount(x);
            for (int i = 0; i < (n + 1) / 2; ++i) if ((x >> i) & 1) s += nums[n / 2 + i];
            for (int l = 0; l <= n / 2; ++l) if (k + l > 0 && k + l < n){
                if ((k + l) * sum % n == 0 && v[l].count((k + l) * sum / n - s))
                    return true;
            }
        }
        return false;
    }
};
```

**这题用到了折半搜索的技巧**，即将数组拆成前后两半，一般处理后存储起来，另一半在线查询即可。类似的题目还有 [1755. 最接近目标值的子序列和](https://leetcode.cn/problems/closest-subsequence-sum/)，[2035. 将数组分成两个数组并最小化数组和的差](https://leetcode.cn/problems/partition-array-into-two-arrays-to-minimize-sum-difference/) 。这些题目的**数据量特点**是 $n$ 在 20 ~ 40这个范围，直接枚举所有子集会超时，但将其分成两半、再分别枚举时，时间复杂度可以接受。

此外，如果修改一下数据范围，本题还有基于**背包**的解法，时间复杂度为 $O(n^2\cdot sum(nums))$，代码如下：

```cpp
//805, 背包, O(n^2*sum(nums)), 超时
class Solution {
    //dp[i][k][x]: 考虑前i个数, 其中是否存在k个数满足和为x
    int dp[31][300005];
public:
    bool splitArraySameAverage(vector<int>& nums) {
        int n = nums.size(), s = accumulate(nums.begin(), nums.end(), 0);
        dp[0][0] = 1;
        for (int i = 0; i < n; ++i) {
            for (int k = i + 1; k >= 1; --k) {
                for (int x = s; x >= nums[i]; --x) {
                    dp[k][x] |= dp[k - 1][x - nums[i]];
                }
            }
        }
        for (int k = 1; k < n; ++k) {
            if ((k * s) % n == 0 && dp[k][k * s / n]) return true;
        }
        return false;
    }
};
```



下面两个题目具有一定相似性，利用记忆化存储将全排列的 $O(n!)$ 的时间复杂度降低到 $O(n^22^n)$，并且 dp 表多一个维度，`dp[i][state]` 表示 `state` 表示的子集的左侧元素为 `nums[i]`时的目标值。

[996. 正方形数组的数目](https://leetcode.cn/problems/number-of-squareful-arrays/)，[943. 最短超级串](https://leetcode.cn/problems/find-the-shortest-superstring/)

```cpp
//996, O(n^2*2^n)
class Solution {
    int dp[12][1 << 12];
    int f[12][12];
    int fact[13] = {1};
public:
    int numSquarefulPerms(vector<int>& nums) {
        int n = nums.size();
        sort(nums.begin(), nums.end());
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                int x = nums[i] + nums[j];
                int y = sqrt(x);
                if (y * y == x) f[i][j] = f[j][i] = 1;
            }
        }

        function<int(int, int)> dfs = [&] (int state, int pre) {
            if (!state) return 1;
            if (pre >= 0 && dp[pre][state]) return dp[pre][state];
            int res = 0;
            for (int i = 0; i < n; ++i) if ((state >> i) & 1) {
                if (pre >= 0 && !f[pre][i]) continue;
                res += dfs(state ^ (1 << i), i);
            }
            if (pre >= 0) dp[pre][state] = res;
            return res;
        };

        int res = dfs((1 << n) - 1, -1);
        for (int i = 1; i <= n; ++i) fact[i] = i * fact[i - 1];
        unordered_map<int, int> cnt;
        for (auto x:nums) ++cnt[x];
        for (auto it:cnt) res /= fact[it.second];
        return res;
    }
};
```

