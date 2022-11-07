# 数位DP

### 1. 参考资料

[数位 DP - OI Wiki ](https://oi-wiki.org/dp/number/) 

模板：[【力扣周赛 306】数位 DP | LeetCode 算法刷题_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1rS4y1s721?spm_id_from=333.1007.top_right_bar_window_dynamic.content.click&vd_source=6dba954e883b0b04f8c601334ee177d2)

数位 DP 中通常会利用常规计数问题技巧，比如把一个区间内的答案拆成两部分相减，即

$ans[l:r]=ans[0:r]-ans[0:l-1]$



### 2. 题目

[357. 统计各位数字都不同的数字个数 - 力扣（LeetCode）](https://leetcode.cn/problems/count-numbers-with-unique-digits/)

[600. 不含连续1的非负整数 - 力扣（LeetCode）](https://leetcode.cn/problems/non-negative-integers-without-consecutive-ones/)

[902. 最大为 N 的数字组合 - 力扣（LeetCode）](https://leetcode.cn/problems/numbers-at-most-n-given-digit-set/)

[1012. 至少有 1 位重复的数字 - 力扣（LeetCode）](https://leetcode.cn/problems/numbers-with-repeated-digits/)



```cpp
//357 ans = 1+\sum_{1<=k<=n}9*9!/(10-k)!
class Solution {
    int fact[11];
public:
    int countNumbersWithUniqueDigits(int n) {
        fact[0] = 1;
        for (int i = 1; i <= 10; ++i) fact[i] = i * fact[i - 1];
        int res = 1;
        for (int i = 1; i <= n; ++i) res += 9 * fact[9] / fact[10 - i];
        return res;
    }
};
```

```cpp
//600
class Solution {
    unordered_map<int, int> memo;
public:
    int findIntegers(int n) {
        if (n <= 2) return n + 1;
        if (memo.count(n)) return memo[n];
        int k = getLen(n) - 1;
        int x = (1 << k) - 1;
        int y = (n & (1 << (k - 1))) ? ((1 << (k - 1)) - 1) : (n ^ (1 << k));
        return memo[n] = findIntegers(x) + findIntegers(y);
    }

    int getLen(int x) {
        int cnt = 0;
        while (x) x >>= 1, ++cnt;
        return cnt;
    }
};
```

```cpp
//902
class Solution {
public:
    int atMostNGivenDigitSet(vector<string>& digits, int n) {
        vector<int> v;
        for (auto &s:digits) v.push_back(s[0] - '0');
        int m = v.size();
        unordered_map<int, int> memo;

        function<int(int)> f = [&] (int x) {//长度与x相同且不大于x的数的个数
            if (x < 10) {
                int cnt = 0;
                for (int i = 0; i < m; ++i) if (v[i] <= x) ++cnt;
                return cnt;
            }
            if (memo.count(x)) return memo[x];
            string s = to_string(x);
            int res = 0, k = s.size();
            for (int c:v) {
                if (c < s[0] - '0') res += (int)pow(m, k - 1);
                else if (c == s[0] - '0' && k > 1 && s[1] != '0') {
                    res += f(stoi(s.substr(1)));
                } 
            }
            return memo[x] = res;
        };

        int res = f(n);
        for (int k = (int)to_string(n).size() - 1; k > 0; --k)
            res += (int)pow(m, k);
        return res;
    }
};
```

```cpp
//1012
class Solution {
    int fact[10], used[10];
public:
    int numDupDigitsAtMostN(int n) {
        if (n <= 10) return 0;
        fact[0] = 1;
        for (int i = 1; i < 10; ++i) fact[i] = i * fact[i - 1];
        string s = to_string(n);
        int K = s.size();
        for (int k = 1; k < K; ++k) n -= 9 * fact[9] / fact[10 - k];

        function<int(int)> dfs = [&] (int idx) {
            if (idx == K - 1) {
                int cnt = 0;
                for (int i = 0; i < 10; ++i) 
                    if (!used[i] && i <= s[idx] - '0')
                        ++cnt;
                return cnt;
            }
            int res = 0;
            for (int i = 0; i < 10; ++i) if (!used[i]) {
                if (i < s[idx] - '0' && i + idx > 0) 
                    res += fact[9 - idx] / fact[10 - K];
                if (i == s[idx] - '0') {
                    used[i] = 1;
                    res += dfs(idx + 1);
                    used[i] = 0;
                }
            }
            return res;
        };

        return n - dfs(0);
    }
};
```

