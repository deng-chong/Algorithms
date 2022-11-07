# 状压DP

1. 最基本的一类: `dp[state]` 表示 `state` 表示的子集的函数值：

   [464. 我能赢吗](https://leetcode.cn/problems/can-i-win/) 

   `dp[vec]`: 当每个位置不只有01两个状态时，可对vector 进行 hash, 如 [638. 大礼包](https://leetcode.cn/problems/shopping-offers/), [1815. 得到新鲜甜甜圈的最多组数](https://leetcode.cn/problems/maximum-number-of-groups-getting-fresh-donuts/), [1255. 得分最高的单词集合](https://leetcode.cn/problems/maximum-score-words-formed-by-letters/)




2. 单纯考虑全排列

   [526. 优美的排列](https://leetcode.cn/problems/beautiful-arrangement/), [1879. 两个数组最小的异或值之和](https://leetcode.cn/problems/minimum-xor-sum-of-two-arrays/)

   既可以用 `dp[idx][state]`，也可用 `dp[state]` + `k=bitcount(state)` 状态转移

   

3. 全排列+前一个元素的信息 `dp[pre][state]`

    [943. 最短超级串](https://leetcode.cn/problems/find-the-shortest-superstring/), [996. 正方形数组的数目](https://leetcode.cn/problems/number-of-squareful-arrays/)

   

4.  子集划分

     [698. 划分为k个相等的子集](https://leetcode.cn/problems/partition-to-k-equal-sum-subsets/), [1681. 最小不兼容性](https://leetcode.cn/problems/minimum-incompatibility/) 

   这里面状态的定义不简单。**记住我们总是填满当前桶，再填下一个桶**。

   第一题判断能否将数字分入 $k$ 个桶，使各个桶中数字之和相等。用 `dp[state]` 表示 `state` 代表的子集能否在已经填入部分数字(即`(1<<n)^state`)的基础上完成目标，由于我们能够从 `state` 中唯一确定有多少填满了的桶、余下的一个不满的桶中的元素之和，因此就能确定 `dp[state]` 。

   第二题要麻烦一些。首先我们对数字从小到大排序，数字放入桶中时，要保持桶中元素严格单调增，一旦填满所有桶(`state==0`)，便得到了一个可行解。计算不可兼容性之和时，如果当前桶中仅有一个元素，记该桶贡献不可兼容性为0，若多于一个元素，贡献为这个元素与桶中上个元素之差。因此，仅

   知道 `state` 是不够的，还要知道当前桶中最后一个元素 `pre`，所以状态要定义成 `dp[pre][state]`。

   ```cpp
   function<int(int, int)> dfs = [&] (int pre, int state) {
   	if (!state) return 0;
   	if (dp[pre][state] >= 0) return dp[pre][state];
   	int res = inf;
   	int r = __builtin_popcount(state);
   	if (r % (n / k) == 0) {//新桶
   		for (int i = 0; i < n; ++i) if ((state >> i) & 1) {
   			res = min(res, dfs(nums[i], state ^ (1 << i)));
   		}
   	} else {
   		for (int i = 0; i < n; ++i) {
   			if (((state >> i) & 1) && nums[i] > pre) {
   				res = min(res, nums[i] - pre + 
                             		dfs(nums[i], state ^ (1 << i)));
   			}
   		}
   	}
   	return dp[pre][state] = res;
   };
   ```

   

5. [691. 贴纸拼词](https://leetcode.cn/problems/stickers-to-spell-word/), [1125. 最小的必要团队](https://leetcode.cn/problems/smallest-sufficient-team/)

   这两题十分相似，都可以用 *BFS* 求解；后者要求具体方案，更是可以方便地利用 BFS 过程构造 `prev` 数组求得。

   自下而上的填表DP也可以求解这两个题目，对第二题也可以方便得求出具体方案；但自上而下的记忆化搜索则**不太好**构造方案。

   

8. [1434. 每个人戴不同帽子的方案数](https://leetcode.cn/problems/number-of-ways-to-wear-different-hats-to-each-other/), [2172. 数组的最大与和](https://leetcode.cn/problems/maximum-and-sum-of-array/), 

   `dp[idx][state]` ：考虑数量少的一方的所有状态 `state` 和另一方的前 `idx` 个时的目标函数
   
   

9. 子集枚举。复杂度稍高 $O(3^n)$，但很好用。

 [698. 划分为k个相等的子集](https://leetcode.cn/problems/partition-to-k-equal-sum-subsets/), [1681. 最小不兼容性](https://leetcode.cn/problems/minimum-incompatibility/) , [1655. 分配重复整数](https://leetcode.cn/problems/distribute-repeating-integers/), [1723. 完成所有工作的最短时间](https://leetcode.cn/problems/find-minimum-time-to-finish-all-jobs/), [1986. 完成任务的最少工作时间段](https://leetcode.cn/problems/minimum-number-of-work-sessions-to-finish-the-tasks/)



8. 折半搜索

    [805. 数组的均值分割](https://leetcode.cn/problems/split-array-with-same-average/), [2035. 将数组分成两个数组并最小化数组和的差](https://leetcode.cn/problems/partition-array-into-two-arrays-to-minimize-sum-difference/), [1755. 最接近目标值的子序列和](https://leetcode.cn/problems/closest-subsequence-sum/)