# 树形DP

### 1. 参考资料

[树形 DP - OI Wiki (oi-wiki.org)](https://oi-wiki.org/dp/tree/)

### 2. 题目

[337. 打家劫舍 III - 力扣](https://leetcode.cn/problems/house-robber-iii/)

[P1352 没有上司的舞会 - 洛谷](https://www.luogu.com.cn/problem/P1352)

[LCP 34. 二叉树染色 - 力扣](https://leetcode.cn/problems/er-cha-shu-ran-se-UGC/)

[124. 二叉树中的最大路径和 - 力扣](https://leetcode.cn/problems/binary-tree-maximum-path-sum/)

```cpp
//337
class Solution {
    unordered_map<TreeNode*, int> x, y;
public:
    int rob(TreeNode* root) {
        dfs(root);
        return max(x[root], y[root]);
    }

    void dfs(TreeNode* r) {
        if (!r) return;
        dfs(r->left);
        dfs(r->right);
        x[r] = r->val + y[r->left] + y[r->right];
        y[r] = max(x[r->left], y[r->left]) + max(x[r->right], y[r->right]);
    }
};
```





```cpp
//P1352
#include<iostream>
#include<vector>
#include<cstring>
using namespace std;

int n, root;
int r[6005], fa[6005];
int dp[60005][2];
vector<int> g[6005];

void dfs(int u) {
    dp[u][1] = r[u];
    dp[u][0] = 0;
    for (int v:g[u]) {
        dfs(v);
        dp[u][0] += max(dp[v][0], dp[v][1]);
        dp[u][1] += dp[v][0];
    }
}
int main() {
    cin >> n;
    for (int i = 0; i < n; ++i) cin >> r[i];
    memset(fa, -1, sizeof(fa));
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        --u, --v;
        fa[u] = v;
        g[v].push_back(u);
    }
    for (int i = 0; i < n; ++i) if (fa[i] == -1) root = i;
    memset(dp, 0, sizeof(dp));
    dfs(root);
    cout << max(dp[root][0], dp[root][1]);
    return 0;
}
```





```cpp
//LCP 34
class Solution {
    unordered_map<TreeNode*, int> f[11];
    int K;
public:
    int maxValue(TreeNode* r, int k) {
        K = k;
        return dfs(r, k);
    }

    int dfs(TreeNode* r, int k) {
        if (!r) return 0;
        if (f[k].count(r)) return f[k][r];
        int res = dfs(r->left, K) + dfs(r->right, K); //是K不是k
        for (int i = 0; i < k; ++i) 
            res = max(res, r->val + dfs(r->left, i) + dfs(r->right, k - 1 - i));
        return f[k][r] = res;
    }
};
```

```cpp
//124
class Solution {
    int res = INT_MIN;
public:
    int maxPathSum(TreeNode* root) {
        dfs(root);
        return res;
    }

    int dfs(TreeNode *r) {
        if (!r) return 0;
        int left = dfs(r->left);
        int right = dfs(r->right);
        res = max(res, r->val + max(0, left) + max(0, right));
        return r->val + max(0, max(left, right));
    }
};
```