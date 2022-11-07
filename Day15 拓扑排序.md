# 拓扑排序

**参考资料**：[Topological Sorting - GeeksforGeeks](https://www.geeksforgeeks.org/topological-sorting/)

### 1. 概念

- 有向无环图(DAG)

- 拓扑序: a linear ordering of its vertices such that for every directed edge `u->v` from vertex `u` to vertex `v`, `u` comes before `v` in the ordering

  拓扑序存在当且仅当图为DAG。

### 2. 代码

```cpp
vector<int> TopoSort(vector<vector<int>>& g) { //g[u][v]:u->v
   int n = g.size(); // no. of nodes
    vector<int> res, indeg(n);
    queue<int> q;
    for (int i = 0; i < n; ++i) if (int j:g[i]) ++indeg[j];
    for (int i = 0; i < n; ++i) if (indeg[i] == 0) q.push(i);
    while (!q.empty()) {
        int i = q.front();
        q.pop();
        res.push_back(i);
        for (int j:g[i]) if (--indeg[j] == 0) q.push(j);
    }
    if ((int)res.size() < n) return {}; //有环，不存在拓扑序
    return res;
}
```

**用途**：求拓扑序，有向图探测环，有向图最长路...



### 3. 题目

[207. 课程表](https://leetcode.cn/problems/course-schedule/), [210. 课程表 II](https://leetcode.cn/problems/course-schedule-ii/), [1462. 课程表 IV](https://leetcode.cn/problems/course-schedule-iv/), [2192. 有向无环图中一个节点的所有祖先](https://leetcode.cn/problems/all-ancestors-of-a-node-in-a-directed-acyclic-graph/), [851. 喧闹和富有](https://leetcode.cn/problems/loud-and-rich/)

```cpp
//207
class Solution {
public:
    bool canFinish(int n, vector<vector<int>>& pre) {
        vector<int> indeg(n);
        vector<vector<int>> g(n);
        for (auto &e:pre) g[e[1]].push_back(e[0]), ++indeg[e[0]];
        queue<int> q;
        for (int i = 0; i < n; ++i) if (!indeg[i]) q.push(i);
        int cnt = 0;
        while (!q.empty()) {
            int i = q.front();
            q.pop();
            ++cnt;
            for (int j:g[i]) if (--indeg[j] == 0) q.push(j);
        }
        return cnt == n;
    }
};
```

```cpp
//210
class Solution {
public:
    vector<int> findOrder(int n, vector<vector<int>>& pre) {
        vector<vector<int>> g(n);
        vector<int> res, indeg(n);
        for (auto &e:pre) g[e[1]].push_back(e[0]), ++indeg[e[0]];
        queue<int> q;
        for (int i = 0; i < n; ++i) if (!indeg[i]) q.push(i);
        while (!q.empty()) {
            int i = q.front();
            q.pop();
            res.push_back(i);
            for (int j:g[i]) if (--indeg[j] == 0) q.push(j);
        }
        if ((int)res.size() < n) return {};
        return res;
    }
};
```

```cpp
//1462
class Solution {
public:
    vector<bool> checkIfPrerequisite(int n, vector<vector<int>>& pre, vector<vector<int>>& queries) {
        vector<vector<int>> g(n);
        vector<int> indeg(n);
        for (auto &e:pre) g[e[0]].push_back(e[1]), ++indeg[e[1]];
        queue<int> q;
        vector<unordered_set<int>> f(n);
        for (int i = 0; i < n; ++i) if (!indeg[i]) q.push(i);
        while (!q.empty()) {
            int i = q.front();
            q.pop();
            for (int j:g[i]) {
                f[j].insert(i);
                for (auto x:f[i]) f[j].insert(x);
                if (--indeg[j] == 0) q.push(j);
            }
        }

        vector<bool> res;
        for (auto q:queries) res.push_back(f[q[1]].count(q[0]));
        return res;
    }
};
```

```cpp
//2192 实际上和1462一模一样
class Solution {
public:
    vector<vector<int>> getAncestors(int n, vector<vector<int>>& edges) {
        vector<vector<int>> res(n), g(n);
        vector<int> indeg(n);
        for (auto &e:edges) g[e[0]].push_back(e[1]), ++indeg[e[1]];
        vector<unordered_set<int>> p(n);
        queue<int> q;
        for (int i = 0; i < n; ++i) if(!indeg[i]) q.push(i);
        while (!q.empty()) {
            int i = q.front();
            q.pop();
            for (int j:g[i]) {
                if (--indeg[j] == 0) q.push(j);
                p[j].insert(i);
                for (int x:p[i]) p[j].insert(x);
            }
        }
        for (int i = 0; i < n; ++i) {
            for (int j:p[i]) res[i].push_back(j);
            sort(res[i].begin(), res[i].end());
        }
        
        return res;
        
    }
};
```

```cpp
//851
class Solution {
public:
    vector<int> loudAndRich(vector<vector<int>>& richer, vector<int>& quiet) {
        int n = quiet.size();
        vector<vector<int>> g(n);
        vector<int> indeg(n);
        for (auto &e:richer) g[e[0]].push_back(e[1]), ++indeg[e[1]];
        queue<int> q;
        for (int i = 0; i < n; ++i) if (!indeg[i]) q.push(i);
        vector<int> res(n);
        for (int i = 0; i < n; ++i) res[i] = i;
        while (!q.empty()) {
            int i = q.front();
            q.pop();
            for (int j:g[i]) {
                if (quiet[res[i]] < quiet[res[j]]) res[j] = res[i];
                if (--indeg[j] == 0) q.push(j);
            }
        }
        return res;
    }
};
```

