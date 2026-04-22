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

// Iteratively build tree from extended preorder where -1 is null.
static int build_tree(const vector<long long>& a, size_t& idx, vector<Node>& pool) {
    if (idx >= a.size()) return -1;
    long long first = a[idx++];
    if (first == -1) return -1;
    int root = (int)pool.size();
    pool.push_back({(int)first, -1, -1});
    struct Frame { int u; int state; }; // 0: left, 1: right, 2: done
    vector<Frame> st;
    st.push_back({root, 0});
    while (!st.empty()) {
        Frame &fr = st.back();
        int u = fr.u;
        if (fr.state == 0) {
            if (idx >= a.size()) { pool[u].left = -1; fr.state = 1; continue; }
            long long v = a[idx++];
            if (v == -1) { pool[u].left = -1; fr.state = 1; continue; }
            int id = (int)pool.size();
            pool.push_back({(int)v, -1, -1});
            pool[u].left = id;
            fr.state = 1;
            st.push_back({id, 0});
        } else if (fr.state == 1) {
            if (idx >= a.size()) { pool[u].right = -1; fr.state = 2; continue; }
            long long v = a[idx++];
            if (v == -1) { pool[u].right = -1; fr.state = 2; continue; }
            int id = (int)pool.size();
            pool.push_back({(int)v, -1, -1});
            pool[u].right = id;
            fr.state = 2;
            st.push_back({id, 0});
        } else {
            st.pop_back();
        }
    }
    return root;
}

// Recursively prune subtrees that do not contain a 1. Returns pointer to pruned subtree root.
// Iterative prune using postorder to compute if a subtree contains a 1.
static int prune(int root, vector<Node>& pool) {
    if (root == -1) return -1;
    vector<char> hasOne(pool.size(), 0);
    vector<pair<int,bool>> st;
    st.reserve(pool.size());
    st.push_back({root, false});
    while (!st.empty()) {
        auto [u, vis] = st.back(); st.pop_back();
        if (u == -1) continue;
        if (!vis) {
            st.push_back({u, true});
            st.push_back({pool[u].right, false});
            st.push_back({pool[u].left, false});
        } else {
            bool l = (pool[u].left != -1) && hasOne[pool[u].left];
            bool r = (pool[u].right != -1) && hasOne[pool[u].right];
            hasOne[u] = (pool[u].val == 1) || l || r;
        }
    }
    if (!hasOne[root]) return -1;
    // Second pass to cut pruned children
    vector<int> q; q.push_back(root);
    for (size_t i = 0; i < q.size(); ++i) {
        int u = q[i];
        int l = pool[u].left;
        int r = pool[u].right;
        if (l != -1) {
            if (!hasOne[l]) pool[u].left = -1; else q.push_back(l);
        }
        if (r != -1) {
            if (!hasOne[r]) pool[u].right = -1; else q.push_back(r);
        }
    }
    return root;
}

// Preorder print with -1 for null pointers into a vector for controlled spacing.
static void preorder_emit(int root, const vector<Node>& pool, vector<long long>& out) {
    if (root == -1) { out.push_back(-1); return; }
    vector<int> st; st.push_back(root);
    while (!st.empty()) {
        int u = st.back(); st.pop_back();
        if (u == -1) { out.push_back(-1); continue; }
        out.push_back(pool[u].val);
        int r = pool[u].right;
        int l = pool[u].left;
        st.push_back(r == -1 ? -1 : r);
        st.push_back(l == -1 ? -1 : l);
    }
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
