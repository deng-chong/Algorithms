#include <functional>
#include <iostream>
#include <random>
#include <vector>
using namespace std;

void selection(vector<int>& v) {
    int n = v.size();
    for (int i = 0; i < n; ++i) {
        int idx = i;
        for (int j = i + 1; j < n; ++j) {
            if (v[j] < v[idx]) idx = j;
        }
        for (int j = idx; j > i; --j) {
            swap(v[j], v[j - 1]);
        }
    }
}

void insertion(vector<int>& v) {
    int n = v.size();
    for (int i = 1; i < n; ++i) {
        int j = i;
        while (j > 0 && v[j] < v[j - 1]) {
            swap(v[j], v[j - 1]);
            --j;
        }
    }
}

void bubble(vector<int>& v) {
    int n = v.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n - 1 - i; ++j) {
            if (v[j] > v[j + 1]) swap(v[j], v[j + 1]);
        }
    }
}


void quick_sort(vector<int>& v) {
    if (v.size() <= 1) return;
    default_random_engine e(time(0));
    uniform_int_distribution u;

    auto partition = [&](int l, int r) {
        int mid = l + u(e) % (r - l + 1);
        int key = v[mid], idx = l;
        swap(v[l], v[mid]);
        for (int i = l + 1; i <= r; ++i) {
            if (v[i] < key) {
                swap(v[i], v[++idx]);
            }
        }
        swap(v[l], v[idx]);
        return idx;
    };

    function<void(int, int)> quick_sort = [&](int l, int r) {
        if (l >= r) return;
        int mid = partition(l, r);
        quick_sort(l, mid - 1);
        quick_sort(mid + 1, r);
    };

    quick_sort(0, (int)v.size() - 1);
}

void merge_sort(vector<int>& v) {
    if (v.size() <= 1) return;
    vector<int> t(v);

    auto merge = [&](int l, int mid, int r) {
        for (int i = l; i <= r; ++i) t[i] = v[i];
        for (int i = l, j = mid + 1, k = l; k <= r; ++k) {
            if (i > mid)
                v[k] = t[j++];
            else if (j > r)
                v[k] = t[i++];
            else
                v[k] = t[i] < t[j] ? t[i++] : t[j++];
        }
    };

    function<void(int, int)> merge_sort = [&](int l, int r) {
        if (l >= r) return;
        int mid = (l + r) / 2;
        merge_sort(l, mid);
        merge_sort(mid + 1, r);
        merge(l, mid, r);
    };

    merge_sort(0, v.size() - 1);
}

void heap_sort(vector<int>& v) {
    function<void(int, int)> heapify = [&](int i, int sz) {
        int l = 2 * i + 1, r = 2 * i + 2;
        int idx = i;
        if (l < sz && v[idx] < v[l]) idx = l;
        if (r < sz && v[idx] < v[r]) idx = r;
        if (i != idx) {
            swap(v[i], v[idx]);
            heapify(idx, sz);
        }
    };
    int n = v.size();
    for (int i = n / 2 - 1; i >= 0; --i) heapify(i, n);
    for (int i = n - 1; i > 0; --i) {
        swap(v[0], v[i]);
        heapify(0, i);
    }
}

bool is_sorted(vector<int>& v) {
    for (int i = 0; i + 1 < v.size(); ++i)
        if (v[i] > v[i + 1]) return false;
    return true;
}

int main() {
    vector<int> v;
    default_random_engine e(time(0));
    uniform_int_distribution u;
    for (int i = 0; i < 100000; ++i) v.push_back(u(e));
    merge_sort(v);
    cout << is_sorted(v);
}
