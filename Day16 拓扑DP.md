# 拓扑DP

### 1. 前言

动态规划要满足**无后效性**，拓扑序便满足此性质。在DAG上进行状态转移时，对于每个点，可以转移得到它的节点都已经被我们处理过了，从而满足DP的无后效性。换言之，假设

$dp[i]=f(\{dp[j]|j\in S_i\})$，

按拓扑序更新节点 $dp$ 值时，可以保证它依赖的节点的 $dp$ 值已经全部计算出来。

### 题目

[329. 矩阵中的最长递增路径](https://leetcode.cn/problems/longest-increasing-path-in-a-matrix/), [1857. 有向图中最大颜色值](https://leetcode.cn/problems/largest-color-value-in-a-directed-graph/), [2050. 并行课程 III](https://leetcode.cn/problems/parallel-courses-iii/) 

[1976. 到达目的地的方案数](https://leetcode.cn/problems/number-of-ways-to-arrive-at-destination/), [1786. 从第一个节点出发到最后一个节点的受限路径数](https://leetcode.cn/problems/number-of-restricted-paths-from-first-to-last-node/) 

```cpp
//329 DAG最长路 O(m+n)
class Solution {
    int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
public:
    int longestIncreasingPath(vector<vector<int>>& mat) {
        int m = mat.size(), n = mat[0].size();
        vector<vector<int>> g(m * n);
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                for (int k = 0; k < 4; ++k) {
                    int x = i + dx[k], y = j + dy[k];
                    if (x >= 0 && x < m && y >= 0 && y < n && mat[x][y] > mat[i][j]) {
                        g[i * n + j].push_back(x * n + y);
                    }
                }
            }
        }
        return longestPath(g);
    }

    int longestPath(vector<vector<int>>& g) {
        int n = g.size();
        vector<int> indeg(n), dist(n, 1);
        for (int i = 0; i < n; ++i) {
            for (int j:g[i]) ++indeg[j];
        }
        queue<int> q;
        for (int i = 0; i < n; ++i) if (!indeg[i]) q.push(i);
        while (!q.empty()) {
            int i = q.front();
            q.pop();
            for (int j:g[i]) {
                dist[j] = max(dist[j], 1 + dist[i]);
                if (--indeg[j] == 0) q.push(j);
            }
        }
        return *max_element(dist.begin(), dist.end());
    }
};
```

```cpp
//1786
class Solution {
    const int inf = INT_MAX, mod = 1e9+7;
public:
    int countRestrictedPaths(int n, vector<vector<int>>& edges) {
        vector<vector<pair<int, int>>> g(n);
        for (auto &e:edges) {
            g[e[0] - 1].push_back({e[1] - 1, e[2]});
            g[e[1] - 1].push_back({e[0] - 1, e[2]});
        }
        vector<int> dist = dijkstra(g, n - 1);
        
        vector<int> dp(n), indeg(n);
        dp[0] = 1;
        for (int i = 0; i < n; ++i) {
            for (auto& [j, d]:g[i]) if (dist[i] > dist[j]) ++indeg[j];
        }
        queue<int> q;
        for (int i = 0; i < n; ++i) if (!indeg[i]) q.push(i);
        while (!q.empty()) {
            int i = q.front();
            q.pop();
            for (auto& [j, d]:g[i]) if (dist[i] > dist[j]) {
                dp[j] = (dp[j] + dp[i]) % mod;
                if (--indeg[j] == 0) q.push(j);
            }
        }
        return dp[n - 1];
    }

    vector<int> dijkstra(vector<vector<pair<int,int>>>& g, int s){
        int n = g.size();
        vector<int> dist(n, inf);
        set<pair<int, int>> st;
        dist[s] = 0;
        st.insert({0, s});
        while(!st.empty()){
            auto u = st.begin()->second;
            st.erase(st.begin());
            for(auto& [v, d] : g[u]){
                if(dist[v] - d > dist[u]){
                    if(dist[v] != inf) st.erase({dist[v], v});
                    dist[v] = dist[u] + d;
                    st.emplace(dist[v], v);
                }
            }
        }
        return dist;
    }
};
```

```cpp
//1857
class Solution {
    int dp[100001][26];
public:
    int largestPathValue(string colors, vector<vector<int>>& edges) {
        int n = colors.size();
        vector<vector<int>> g(n);
        vector<int> indeg(n);
        for (auto &e:edges) {
            g[e[0]].push_back(e[1]);
            ++indeg[e[1]];
        }
        queue<int> q;
        for (int i = 0; i < n; ++i) if (!indeg[i]) q.push(i);
        int cnt = 0;
        while (!q.empty()) {
            int i = q.front();
            q.pop();
            ++cnt;
            ++dp[i][colors[i] - 'a'];
            for (int j:g[i]) {
                for (int c = 0; c < 26; ++c) dp[j][c] = max(dp[j][c], dp[i][c]);
                if (--indeg[j] == 0) q.push(j);
            }
        }
        if (cnt < n) return -1;
        int res = 0;
        for (int i = 0; i < n; ++i) for (int c = 0; c < 26; ++c) res = max(res, dp[i][c]);
        return res;
    }
};
/*     另一种更新顺序
        queue<int> q;
        for (int i = 0; i < n; ++i) {
            if (!indeg[i]) q.push(i);
            dp[i][colors[i] - 'a'] = 1;
        }
        int cnt = 0;
        while (!q.empty()) {
            int i = q.front();
            q.pop();
            ++cnt;
            for (int j:g[i]) {
                for (int c = 0; c < 26; ++c) 
                    dp[j][c] = max(dp[j][c], dp[i][c] + (colors[j] - 'a' == c));
                if (--indeg[j] == 0) q.push(j);
            }
        }
*/
```

```cpp
//1976 单源最短路构成有向无环图
class Solution {
    const int inf = INT_MAX, mod = 1e9+7;
public:
    int countPaths(int n, vector<vector<int>>& roads) {
        vector<vector<pair<int, int>>> g(n);
        for (auto &e:roads) {
            g[e[0]].push_back({e[1], e[2]});
            g[e[1]].push_back({e[0], e[2]});
        }
        vector<long long> dist = Dijkstra(g, 0);
        vector<int> indeg(n);
        for (int i = 0; i < n; ++i) {
            for (auto& [j, d]:g[i]) {
                if (dist[j] - dist[i] == d) ++indeg[j];
            }
        }
        queue<int> q;
        vector<int> dp(n);
        for (int i = 0; i < n; ++i) if (!indeg[i]) q.push(i);
        dp[0] = 1;
        while (!q.empty()) {
            int i = q.front();
            q.pop();
            for (auto& [j, d]:g[i]) if (dist[j] - dist[i] == d){
                dp[j] = (dp[j] + dp[i]) % mod;
                if (--indeg[j] == 0) q.push(j);
            }
        }
        return dp[n - 1];
    }

    vector<long long> Dijkstra(vector<vector<pair<int,int>>>& g, int s){
        const long long inf = LLONG_MAX;
        int n = g.size();
        vector<long long> dist(n, inf);
        set<pair<int, int>> st;
        dist[s] = 0;
        st.insert({0, s});
        while(!st.empty()){
            auto u = st.begin()->second;
            st.erase(st.begin());
            for(auto [v, d] : g[u]){
                if(dist[v] - d > dist[u]){
                    if(dist[v] != inf) st.erase({dist[v], v});
                    dist[v] = dist[u] + d;
                    st.emplace(dist[v], v);
                }
            }
        }
        return dist;
    }
};
```

```cpp
//2050
class Solution {
    int dp[50005];
public:
    int minimumTime(int n, vector<vector<int>>& relations, vector<int>& time) {
        vector<vector<int>> g(n);
        vector<int> indeg(n);
        for (auto &e:relations) {
            --e[0], --e[1];
            g[e[0]].push_back(e[1]);
            ++indeg[e[1]];
        }
        queue<int> q;
        for (int i = 0; i < n; ++i) {
            if (!indeg[i]) q.push(i);
            dp[i] = time[i];
        }
        while (!q.empty()) {
            int i = q.front();
            q.pop();
            for (int j:g[i]) {
                dp[j] = max(dp[j], dp[i] + time[j]);
                if (--indeg[j] == 0) q.push(j);
            }
        }
        return *max_element(dp, dp + n);
    }
};
/*     另一种更新顺序
        queue<int> q;
        for (int i = 0; i < n; ++i) if (!indeg[i]) q.push(i);
        while (!q.empty()) {
            int i = q.front();
            q.pop();
            dp[i] += time[i]; //课程的最早结束时间
            for (int j:g[i]) {
                dp[j] = max(dp[j], dp[i]); //更新课程的最早开始时间
                if (--indeg[j] == 0) q.push(j);
            }
        }
        return *max_element(dp, dp + n);
*/
```

