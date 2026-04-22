// Problem 085 - 修剪二叉树
// Build: cmake . && make
// Produces executable: code

#include <bits/stdc++.h>
using namespace std;

struct Node {
    int val;
    int left;
    int right;
};

// Read all ints from stdin until -2 (terminator). Any trailing whitespace is ignored.
static vector<long long> read_input_tokens() {
    vector<long long> a;
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long x;
    while (cin >> x) {
        if (x == -2) break;
        a.push_back(x);
    }
    return a;
}

// Build tree from a possibly truncated extended preorder sequence where -1 is null,
// and missing trailing entries imply nulls. Index advances as we consume tokens.
static int build_tree(const vector<long long>& a, size_t& idx, vector<Node>& pool) {
    if (idx >= a.size()) return -1;
    long long v = a[idx++];
    if (v == -1) return -1;
    int id = (int)pool.size();
    pool.push_back({(int)v, -1, -1});
    pool[id].left = build_tree(a, idx, pool);
    pool[id].right = build_tree(a, idx, pool);
    return id;
}

// Recursively prune subtrees that do not contain a 1. Returns pointer to pruned subtree root.
static int prune(int u, vector<Node>& pool) {
    if (u == -1) return -1;
    int l = prune(pool[u].left, pool);
    int r = prune(pool[u].right, pool);
    pool[u].left = l;
    pool[u].right = r;
    if (pool[u].val == 0 && l == -1 && r == -1) return -1;
    return u;
}

// Preorder print with -1 for null pointers into a vector for controlled spacing.
static void preorder_emit(int u, const vector<Node>& pool, vector<long long>& out) {
    if (u == -1) { out.push_back(-1); return; }
    out.push_back(pool[u].val);
    preorder_emit(pool[u].left, pool, out);
    preorder_emit(pool[u].right, pool, out);
}

// Free entire tree to avoid leaks (post-order delete).
static void free_tree() {}

int main() {
    vector<long long> tokens = read_input_tokens();
    size_t idx = 0;
    vector<Node> pool;
    pool.reserve(60000);
    int root = build_tree(tokens, idx, pool);
    root = prune(root, pool);

    vector<long long> out;
    preorder_emit(root, pool, out);

    // Print joined by spaces
    for (size_t i = 0; i < out.size(); ++i) {
        if (i) cout << ' ';
        cout << out[i];
    }
    cout << '\n';

    free_tree();
    return 0;
}
