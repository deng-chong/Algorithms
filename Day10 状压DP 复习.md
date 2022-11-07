# 状压DP 总结

### 题型

1. `dp[state]`: dfs+memo, 用整数表示数组的子集并作为memo的key, 有时也可直接用vector hash.
   - 基础：[526. 优美的排列](https://leetcode.cn/problems/beautiful-arrangement/), [464. 我能赢吗](https://leetcode.cn/problems/can-i-win/), [1986. 完成任务的最少工作时间段](https://leetcode.cn/problems/minimum-number-of-work-sessions-to-finish-the-tasks/)(1986题dp变量的定义是关键和难点, 不像前两个那么直接), [691. 贴纸拼词](https://leetcode.cn/problems/stickers-to-spell-word/)(691题可以用下面的vector hash解决, 但会更麻烦, 由于len(target)很小, 可以直接处理; 但若target再长一些, 需要用vector hash处理), [698. 划分为k个相等的子集](https://leetcode.cn/problems/partition-to-k-equal-sum-subsets/)(状态的定义和转移比较难想)
   - Vector Hash: 当每个位置不只有01两个状态时，可对vector 进行 hash, 如 [638. 大礼包](https://leetcode.cn/problems/shopping-offers/), [1815. 得到新鲜甜甜圈的最多组数](https://leetcode.cn/problems/maximum-number-of-groups-getting-fresh-donuts/), [1255. 得分最高的单词集合](https://leetcode.cn/problems/maximum-score-words-formed-by-letters/)

2. `dp[i][state]`: 状态不仅由子集决定，还与某个元素有关。

   [847. 访问所有节点的最短路径](https://leetcode.cn/problems/shortest-path-visiting-all-nodes/), [996. 正方形数组的数目](https://leetcode.cn/problems/number-of-squareful-arrays/), [943. 最短超级串](https://leetcode.cn/problems/find-the-shortest-superstring/)

   [2172. 数组的最大与和](https://leetcode.cn/problems/maximum-and-sum-of-array/), [1434. 每个人戴不同帽子的方案数](https://leetcode.cn/problems/number-of-ways-to-wear-different-hats-to-each-other/)

3. 牵扯到一些技巧: 
   - 和其他算法结合, 如 **bfs** 等: [847. 访问所有节点的最短路径](https://leetcode.cn/problems/shortest-path-visiting-all-nodes/), [691. 贴纸拼词](https://leetcode.cn/problems/stickers-to-spell-word/) 
   - 遍历子集 $O(n^k3^n)$: [698. 划分为k个相等的子集](https://leetcode.cn/problems/partition-to-k-equal-sum-subsets/), [1986. 完成任务的最少工作时间段](https://leetcode.cn/problems/minimum-number-of-work-sessions-to-finish-the-tasks/), [1655. 分配重复整数](https://leetcode.cn/problems/distribute-repeating-integers/) (通常是次优的) 
   - 折半搜索 $O(n^k2^n)\rightarrow O(n^{k'}2^{n/2})$: [805. 数组的均值分割](https://leetcode.cn/problems/split-array-with-same-average/), [1755. 最接近目标值的子序列和](https://leetcode.cn/problems/closest-subsequence-sum/), [2035. 将数组分成两个数组并最小化数组和的差](https://leetcode.cn/problems/partition-array-into-two-arrays-to-minimize-sum-difference/) 
   - 二部图匹配: [1595. 连通两组点的最小成本](https://leetcode.cn/problems/minimum-cost-to-connect-two-groups-of-points/), [1125. 最小的必要团队](https://leetcode.cn/problems/smallest-sufficient-team/)
   - 如何确定状态？**如何(对谁)压缩？** [1434. 每个人戴不同帽子的方案数](https://leetcode.cn/problems/number-of-ways-to-wear-different-hats-to-each-other/), [2172. 数组的最大与和](https://leetcode.cn/problems/maximum-and-sum-of-array/), [698. 划分为k个相等的子集](https://leetcode.cn/problems/partition-to-k-equal-sum-subsets/) 



### 题目：合作开发

![image-20220428140906529](C:\Users\19751\AppData\Roaming\Typora\typora-user-images\image-20220428140906529.png)

```cpp
//合作开发, O(2^m*n)
class Solution {
    using ll = long long;
    vector<ll> primes;
public:
    int coopDevelop(vector<vector<int>>& skills) {
        for (ll i = 2; primes.size() < 1000; ++i) if (isPrime(i)) primes.push_back(i);
        
        sort(skills.begin(), skills.end(), [&](const auto& x, const auto& y){
            return x.size() < y.size();
        });
        unordered_map<ll, ll> cnt;
        ll n = skills.size(), res = n * (n - 1) / 2, mod = 1e9+7;
        for (auto &skill:skills) {
            ll k = skill.size();
            for (ll i = 0; i < (1 << k); ++i) {
                ll code = hash(skill, i);
                if (cnt.count(code)) res -= cnt[code];
            }
            ++cnt[hash(skill, (1 << k) - 1)];
        }
        return res % mod;
    }
    
    bool isPrime(ll x) {
        for (ll i = 2; i * i <= x; ++i) if (x % i == 0) return false;
        return true;
    }
    
    ll hash(vector<int>& nums, ll mask) {
        ll product = 1;
        for (auto x:nums) {
            if (mask & 1) product *= primes[x - 1];
            mask >>= 1;
        }
        return product;
    }
};
```

