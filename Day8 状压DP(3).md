# 状压DP







### 题目

[1986. 完成任务的最少工作时间段](https://leetcode.cn/problems/minimum-number-of-work-sessions-to-finish-the-tasks/)

方法一：

```cpp
//1986, O(n*3^n), n <=14
class Solution {
    int memo[1<<15], sums[1<<15];
public:
    int minSessions(vector<int>& tasks, int sessionTime) {
        int n = tasks.size();
        memset(memo, -1, sizeof(memo));
        for (int x = 0; x < (1 << n); ++x) {
            int s = 0;
            for (int i = 0; i < n; ++i) if ((x >> i) & 1) s += tasks[i];
            sums[x] = s;
        }
        return dfs(tasks, (1 << n) - 1, sessionTime);
    }
    int dfs(vector<int>& tasks, int state, int time) {
        if (!state) return 0;
        if (memo[state] >= 0) return memo[state];
        if (sums[state] <= time) return memo[state] = 1;
        int res = tasks.size(), subset = state - 1;
        while (subset) {//枚举子集
            if (sums[state] - sums[subset] <= time) 
                res = min(res, 1 + dfs(tasks, subset, time));
            subset = (subset - 1) & state;
        }
        return memo[state] = res;
    }
};
```

方法二

```cpp
//1986, O(n*2^n), n<=14
class Solution {
    //dp[state]: state下工作总时长的最小值
    int dp[1 << 14];
public:
    int minSessions(vector<int>& tasks, int time) {
        int n = tasks.size();
        memset(dp, -1, sizeof(dp));
        int tot = dfs(tasks, time, (1 << n) - 1);
        return tot / time + (tot % time != 0);
    }
    
    int dfs(vector<int>& tasks, int time, int state) {
        if (!state) return 0;
        if (dp[state] >= 0) return dp[state];
        int res = INT_MAX, n = tasks.size();
        for (int i = 0; i < n; ++i) {
            if ((state >> i) & 1) {
                int t = dfs(tasks, time, state ^ (1 << i));
                if (t % time + tasks[i] <= time) {//session剩余时间够用
                    res = min(res, t + tasks[i]);
                } else {//新开一个session放tasks[i]
                    res = min(res, t - t % time + time + tasks[i]);
                }
            }
        }
        return dp[state] = res;
    }
};
```

[847. 访问所有节点的最短路径](https://leetcode.cn/problems/shortest-path-visiting-all-nodes/)

```cpp
//847
class Solution {
    //dp[i][state]: 已访问节点集为state且最后一个节点为i的最短路长度
    int dp[12][1 << 12]; 
    const int inf = 0x3f3f3f3f;
public:
    int shortestPathLength(vector<vector<int>>& g) {
        int n = g.size();
        queue<pair<int, int>> q;
        memset(dp, 0x3f, sizeof(dp));
        for (int i = 0; i < n; ++i) {
            dp[i][1 << i] = 0;
            q.push({i, 1 << i});
        }
        while (!q.empty()) {
            auto [i, state] = q.front();
            q.pop();
            for (auto j:g[i]) if (dp[j][state | (1 << j)] == inf){
                dp[j][state | (1 << j)] = 1 + dp[i][state];
                q.push({j, state | (1 << j)});
            }
        }
        int res = inf;
        for (int i = 0; i < n; ++i) res = min(res, dp[i][(1 << n) - 1]);
        return res;
    }
};
```



[1255. 得分最高的单词集合](https://leetcode.cn/problems/maximum-score-words-formed-by-letters/)

```cpp
//1255
class Solution {
    int cnt[26];
    int dp[1 << 14];
public:
    int maxScoreWords(vector<string>& words, vector<char>& letters, vector<int>& score) {
        int n = words.size();
        for (auto c:letters) ++cnt[c - 'a'];
        memset(dp, -1, sizeof(dp));

        function<int(int)> dfs = [&] (int state) {
            if (dp[state] >= 0) return dp[state];
            int res = 0;
            for (int i = 0; i < n; ++i) if ((state >> i) & 1){
                bool flag = true;
                int s = 0;
                for (auto c:words[i]) {
                    if (--cnt[c - 'a'] < 0) flag = false;
                    s += score[c - 'a'];
                }
                if (flag) res = max(res, s + dfs(state ^ (1 << i)));
                for (auto c:words[i]) ++cnt[c - 'a'];
            }
            return dp[state] = res;
        };

        return dfs((1 << n) - 1);
    }
};
```

