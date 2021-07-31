#include<bits/stdc++.h>
using namespace std;


class TreeNode{
  public:
  int len;
  int total;
  int maxSuffixOddLen;
  int maxSuffixEvenLen;
  int maxSum;
  int maxPrefix;
  int minPrefix;
};

int getMax(vector<int> inp){
  return *max_element(inp.begin(), inp.end());
}

int getMin(vector<int> inp){
  return *min_element(inp.begin(), inp.end());
}

TreeNode mergeTreeNode(TreeNode left, TreeNode right){
    if(left.len == 0){
      return right;
    }
    if(right.len == 0){
      return left;
    }
    TreeNode ans;
 
    ans.len = left.len + right.len;

    if(left.len%2 == 0){
      ans.total = left.total + right.total;
      ans.maxPrefix = getMax({left.maxPrefix, left.total + right.maxPrefix, ans.total});
      ans.minPrefix = getMin({left.minPrefix, left.total + right.minPrefix, ans.total});
    } 
    else{
      ans.total = left.total - right.total;
      ans.maxPrefix = getMax({left.maxPrefix, left.total - right.minPrefix, ans.total});
      ans.minPrefix = getMin({left.minPrefix, left.total - right.maxPrefix, ans.total});
    }

    if(right.len%2 == 0){
      ans.maxSuffixEvenLen = getMax({right.maxSuffixEvenLen, left.maxSuffixEvenLen + right.total, right.total});
      ans.maxSuffixOddLen = getMax({right.maxSuffixOddLen, left.maxSuffixOddLen - right.total});
    }
    else{
      ans.maxSuffixEvenLen = getMax({right.maxSuffixEvenLen, left.maxSuffixOddLen - right.total});
      ans.maxSuffixOddLen = getMax({right.maxSuffixOddLen, left.maxSuffixEvenLen + right.total});
    }

    ans.maxSum = getMax({left.maxSum, right.maxSum, left.maxSuffixEvenLen + right.maxPrefix, left.maxSuffixOddLen - right.minPrefix, 
                      ans.maxPrefix, ans.maxSuffixOddLen, ans.maxSuffixEvenLen});

    return ans;
}

void buildTree(int *a, TreeNode *tree, int start, int end, int treeNode)
{
    if (start == end)
    {
        tree[treeNode].len = 1;
        tree[treeNode].total = tree[treeNode].maxSuffixOddLen = tree[treeNode].minPrefix = tree[treeNode].maxPrefix = tree[treeNode].maxSum = a[start];
        tree[treeNode].maxSuffixEvenLen = 0;
        return;
    }
    int mid = start + ((end - start) / 2);
    buildTree(a, tree, start, mid, 2 * treeNode);
    buildTree(a, tree, mid + 1, end, (2 * treeNode) + 1);
    TreeNode left = tree[2 * treeNode];
    TreeNode right = tree[(2 * treeNode) + 1];
    
    TreeNode mergedNode = mergeTreeNode(left, right);

    tree[treeNode] = mergedNode;
    return;
}

TreeNode query(TreeNode *tree, int start, int end, int treeNode, int left1, int right1)
{
    if (left1 > end || right1 < start)
    {
        TreeNode a;
        a.len = 0;
        return a;
    }

    if (left1 <= start && right1 >= end)
    {
        return tree[treeNode];
    }

    int mid = start + ((end - start) / 2);
    TreeNode left = query(tree, start, mid, 2 * treeNode, left1, right1);
    TreeNode right = query(tree, mid + 1, end, 1 + (2 * treeNode), left1, right1);
    return mergeTreeNode(left, right);
}

int main()
{
    int n;
    cin >> n;
    int *a = new int[n];
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }
    TreeNode *tree = new TreeNode[4 * n];
    buildTree(a, tree, 0, n - 1, 1);

    int nQueries;
    cin >> nQueries;
    while(true){
      int l, r;
      cin >> l >> r;
      cout << query(tree, 0, n-1, 1, l, r).maxSum << endl;
    }
}