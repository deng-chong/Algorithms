# 状压DP







### 题目

[1815. 得到新鲜甜甜圈的最多组数](https://leetcode.cn/problems/maximum-number-of-groups-getting-fresh-donuts/)

```cpp
//1815, 1<=batchSize<=9, len(groups)<=30, 1<=groups[i]<=10^9
class Solution {
    struct VectorHasher {
        size_t operator()(const vector<int>& vec) const {
            size_t hash = vec.size();
            for (int x : vec) {
                hash ^= x + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
            return hash;
        }
    };
public:
    int maxHappyGroups(int batchSize, vector<int>& groups) {
        vector<int> cnt(batchSize);
        for (auto group : groups){
            ++cnt[group % batchSize];
        }
        //预处理包含1-2个group的组
        int happy = cnt[0];
        for (int i = 1; i < (batchSize + 1) / 2; ++i) {
            int num = i == batchSize - i ? cnt[i] / 2 : min(cnt[i], cnt[batchSize - i]);
            happy += num;
            cnt[i] -= num, cnt[batchSize - i] -= num;
        }

        unordered_map<vector<int>, int, VectorHasher> memo;
        function<int(int)> dfs = [&](int sum) {
            auto it = memo.find(cnt);
            if (it != memo.end()) return it->second;
            int ans = 0;
            for (int i = 1; i < batchSize; ++i) {
                if (!cnt[i]) continue;
                --cnt[i];
                ans = max(ans, (sum == 0) + dfs((sum + i) % batchSize));
                ++cnt[i];
            }
            return memo[cnt] = ans;
        };
        return happy + dfs(0);
    }
};
```





[638. 大礼包](https://leetcode.cn/problems/shopping-offers/) 

```cpp
//638, dfs+memo, O(mn*base^n)
class Solution {
    const int base = 11; //base = 1+max(needs)
public:
    int shoppingOffers(vector<int>& price, vector<vector<int>>& special, vector<int>& needs) {  
        int n = price.size(), m = special.size(), res = INT_MAX;
        sort(special.begin(), special.end());
        special.erase(unique(special.begin(), special.end()), special.end());
        unordered_map<int, int> memo;

        function<int()> dfs = [&]() {
            int state = 0;
            for (int i = 0; i < n; ++i) state = state * base + needs[i];
            if (!state) return 0;
            if (memo.count(state)) return memo[state];
            int res = 0;
            for (int i = 0; i < n; ++i) res += price[i] * needs[i];
            for (int i = 0; i < m; ++i) {
                bool flag = true;
                for (int j = 0; j < n; ++j) if (special[i][j] > needs[j]) flag = false;
                if (flag) {
                    for (int j = 0; j < n; ++j) needs[j] -= special[i][j];
                    res = min(res, special[i][n] + dfs());
                    for (int j = 0; j < n; ++j) needs[j] += special[i][j];
                }
            }
            return memo[state] = res;
        };
        return dfs();
    }
};
```



[698. 划分为k个相等的子集](https://leetcode.cn/problems/partition-to-k-equal-sum-subsets/) 

解法一：

```cpp
//698, O(n*3^n), 枚举子集的子集时间复杂度为 O(3^n), 900ms
class Solution {
    int dp[1 << 16];
public:
    //1<=k<=len(nums)<=16,0<nums[i]<1e4
    bool canPartitionKSubsets(vector<int>& nums, int k) {
        if (k == 1) return true;
        int n = nums.size(), s = accumulate(nums.begin(), nums.end(), 0);
        sort(nums.begin(), nums.end());
        if (s % k || nums.back() > s / k) return false;
        memset(dp, -1, sizeof(dp));

        //state表示的nums的子集能否分成num个等和子集
        function<int(int, int)> dfs = [&] (int state, int num) {
            if (dp[state] >= 0) return dp[state];
            if (num == 1) return dp[state] = 1;
            int n = nums.size(), s = 0;
            for (int i = 0; i < n; ++i) if (state & (1 << i)) s += nums[i];

            int subset = state - 1;
            while (subset) {
                int t = 0;
                for (int i = 0; i < n; ++i) if (subset & (1 << i)) t += nums[i];
                if (num * t == s && dfs(state & ~subset, num - 1))
                    return dp[state] = 1;
                subset = (subset - 1) & state;
            }
            return dp[state] = 0;
        };
        
        return dfs((1 << n) - 1, k);
    }
};
```

解法二：参考题解 [经典回溯算法：集合划分问题 - 划分为k个相等的子集 ](https://leetcode.cn/problems/partition-to-k-equal-sum-subsets/solution/by-lfool-d9o7/)

```cpp
//698, O(n*2*n), 24ms
class Solution {
    int memo[1<<16];
public:
    bool canPartitionKSubsets(vector<int>& nums, int k) {
        if (k == 1) return true;
        int s = accumulate(nums.begin(), nums.end(), 0);
        if (s % k != 0) return false;
        sort(nums.rbegin(), nums.rend());//降序更快
        if (nums[0] > s / k) return false;
        int n = nums.size(), target = s / k;
        memset(memo, -1, sizeof(memo));
        
        function<int(int, int, int)> dfs = [&](int state, int curSum, int restBucket) {
            if (restBucket == 1) return 1;
            if (memo[state] >= 0) return memo[state];
            if (curSum == target) 
                return memo[state] = dfs(state, 0, restBucket - 1);
            for (int i = 0; i < n; ++i) {//桶选球
                if (((state >> i)&1) && curSum + nums[i] <= target) {
                    if (dfs(state ^ (1 << i), curSum + nums[i], restBucket)) 
                        return memo[state] = 1;
                }
            }
            return memo[state] = 0;
        };

        return dfs((1 << n) - 1, 0, k);
    }
};
```

注意到以上 dfs 函数的参数 `curSum`和 `restBucket` 都是由 `state` 唯一确定的，故以上函数可以精简一下，如下。


```cpp
//698, O(n*2*n), 32ms
class Solution {
    int memo[1<<16];
public:
    bool canPartitionKSubsets(vector<int>& nums, int k) {
        if (k == 1) return true;
        int s = accumulate(nums.begin(), nums.end(), 0);
        if (s % k != 0) return false;
        sort(nums.rbegin(), nums.rend());//降序更快
        if (nums[0] > s / k) return false;
        int n = nums.size(), target = s / k;
        memset(memo, -1, sizeof(memo));
        
        function<int(int)> dfs = [&](int state) {
            if (!state) return 1;
            if (memo[state] >= 0) return memo[state];
            int sum = 0;
            for (int i = 0; i < n; ++i) if ((state >> i) & 1) sum += nums[i];
            int curSum = (target - sum % target) % target; //当前桶中元素和

            for (int i = 0; i < n; ++i) {//桶选球
                if (((state >> i) & 1) && curSum + nums[i] <= target) {
                    if (dfs(state ^ (1 << i))) 
                        return memo[state] = 1;
                }
            }
            return memo[state] = 0;
        };

        return dfs((1 << n) - 1);
    }
};
```

