// Problem 085 - 修剪二叉树
// Build: cmake . && make
// Produces executable: code

#include <bits/stdc++.h>
using namespace std;

struct Node {
    int val;
    Node* left;
    Node* right;
    Node(int v) : val(v), left(nullptr), right(nullptr) {}
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
static Node* build_tree(const vector<long long>& a, size_t& idx) {
    if (idx >= a.size()) {
        return nullptr; // implicit null
    }
    long long v = a[idx++];
    if (v == -1) return nullptr;
    // v expected to be 0 or 1
    Node* node = new (nothrow) Node((int)v);
    if (!node) return nullptr; // out of memory fallback
    node->left = build_tree(a, idx);
    node->right = build_tree(a, idx);
    return node;
}

// Recursively prune subtrees that do not contain a 1. Returns pointer to pruned subtree root.
static Node* prune(Node* root) {
    if (!root) return nullptr;
    root->left = prune(root->left);
    root->right = prune(root->right);
    if (root->val == 0 && root->left == nullptr && root->right == nullptr) {
        delete root;
        return nullptr;
    }
    return root;
}

// Preorder print with -1 for null pointers into a vector for controlled spacing.
static void preorder_emit(Node* root, vector<long long>& out) {
    if (!root) {
        out.push_back(-1);
        return;
    }
    out.push_back(root->val);
    preorder_emit(root->left, out);
    preorder_emit(root->right, out);
}

// Free entire tree to avoid leaks (post-order delete).
static void free_tree(Node* root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    delete root;
}

int main() {
    vector<long long> tokens = read_input_tokens();
    size_t idx = 0;
    Node* root = build_tree(tokens, idx);
    root = prune(root);

    vector<long long> out;
    preorder_emit(root, out);

    // Print joined by spaces
    for (size_t i = 0; i < out.size(); ++i) {
        if (i) cout << ' ';
        cout << out[i];
    }
    cout << '\n';

    free_tree(root);
    return 0;
}

