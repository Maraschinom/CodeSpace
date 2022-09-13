#include <bits/stdc++.h>
const int N = 2e5 + 5;
int n, m, q, cnt, timestamp;
int dep[N], rt[N], l[N], r[N], f[N][25], g[N][25];
std::vector<int> G[N], bus[N];

struct treenode {
    int ls, rs, s;
} tr[N << 6];

void Dfs(int x) { // DFS����xΪ�������������ϵ�DFS�����Ҷ˵� 
    l[x] = ++ timestamp; // һ���� 
    dep[x] = dep[f[x][0]] + 1;
    for (int y : G[x]) // ö�ٶ��� 
        Dfs(y);
    r[x] = timestamp;
}

int Lca(int x, int y) { // ������LCA 
    if (dep[x] < dep[y]) std::swap(x, y); // �ն�xΪ��������һ�� 
    for (int i = 24; i >= 0; --i) // �����߱�������ͬһ�߶� 
        if (dep[f[x][i]] >= dep[y])
            x = f[x][i];
    if (x == y) // ���������ͬһλ�� ��λ�ü�ΪLCA ֱ�ӷ��� 
        return x;
    for (int i = 24; i >= 0; --i) // ������߲�ͬ��һֱ������ 
        if (f[x][i] != f[y][i]) {
            x = f[x][i];
            y = f[y][i];
        }
    return f[x][0]; // ����LCAһ��Ϊ���ߵĸ��� 
}

void Update(int &rt, int l, int r, int x) {
    tr[++ cnt] = tr[rt]; // �����µĽڵ� 
    ++ tr[cnt].s;
    rt = cnt;
    if (l == r) return;
    int mid = (l + r) >> 1;
    if (x <= mid)
        Update(tr[rt].ls, l, mid, x);
    else
        Update(tr[rt].rs, mid + 1, r, x); 
}

int Query(int rt, int l, int r, int ql, int qr) {
    if (!rt) return 0; // ����ýڵ�Ϊ�� ����Ϊ0 
    if (l == ql && r == qr) // һ���� 
		return tr[rt].s; 
    int mid = (l + r) >> 1;
    if (qr <= mid)
		return Query(tr[rt].ls, l, mid, ql, qr);
    else if (ql > mid)
		return Query(tr[rt].rs, mid + 1, r, ql, qr);
    else 
		return Query(tr[rt].ls, l, mid, ql, mid) + Query(tr[rt].rs, mid + 1, r, mid + 1, qr);
}

int main() {
    scanf("%d", &n);
    for (int i = 2; i <= n; ++i) { // �����ͼ ���ﱣ֤ÿ���ڵ�ĸ��ױ��С�ڸýڵ� 
        scanf("%d", &f[i][0]);
        G[f[i][0]].push_back(i);
    }
    for (int j = 1; j < 25; ++j) // ������LCA 
        for (int i = 1; i <= n; ++i)
            f[i][j] = f[f[i][j - 1]][j - 1];
    Dfs(1);
    scanf("%d", &m);
    for (int i = 1; i <= n; ++i) g[i][0] = i; // g[i][j]��ʾ��ǰ�ڵ�������2^j�������ߵ���ǳ�Ľڵ� 
    for (int i = 1; i <= m; ++i) {
        int x, y;
        scanf("%d%d", &x, &y);
        int lca = Lca(x, y);
        g[x][0] = std::min(g[x][0], lca);
        g[y][0] = std::min(g[y][0], lca);
        bus[l[x]].push_back(l[y]);
        bus[l[y]].push_back(l[x]);
    }
    for (int i = n; i >= 1; --i) // һ���� 
        g[f[i][0]][0] = std::min(g[f[i][0]][0], g[i][0]);
    for (int j = 1; j < 25; ++j)
        for (int i = 1; i <= n; ++i)
            g[i][j] = g[g[i][j - 1]][j - 1]; // һ���� 
    for (int i = 1; i <= n; ++i) {
        rt[i] = rt[i - 1];
        for (int j : bus[i])
            Update(rt[i], 1, n, j);
    }
    scanf("%d", &q);
    for (int i = 1; i <= q; ++i) {
        int x, y;
        scanf("%d%d", &x, &y);
        if (x == y) {
            puts("0");
            continue;
        }
        int lca = Lca(x, y);
        if (g[x][24] > lca || g[y][24] > lca) {
            puts("-1");
            continue;
        }
        int ans = 2; // һ���� 
        for (int j = 24; j >= 0; --j)
            if (g[x][j] > lca) {
                x = g[x][j];
                ans += 1 << j;
            }
        for (int j = 24; j >= 0; --j)
            if (g[y][j] > lca) {
                y = g[y][j];
                ans += 1 << j;
            }
        if (lca == x || lca == y)
            -- ans;
        else
            ans -= (bool)(Query(rt[r[x]], 1, n, l[y], r[y]) - Query(rt[l[x] - 1], 1, n, l[y], r[y]));
        printf("%d\n", ans);
    }
    return 0;
}
