#include <bits/stdc++.h>

using namespace std;

typedef complex<double> ftype;
const double pi = acos(-1);
const int maxn = 1 << 20;
ftype w[maxn];
void init() {
    for(int i = 0; i < maxn; i++) {
        w[i] = polar(1., 2 * pi / maxn * i);
    }
}
template<typename T>
void fft(T *in, ftype *out, int n, int k = 1) {
    if(n == 1) {
        *out = *in;
        return;
    }
    int t = maxn / n;
    n >>= 1;
    fft(in, out, n, 2 * k);
    fft(in + k, out + n, n, 2 * k);
    for(int i = 0, j = 0; i < n; i++, j += t) {
        ftype t = w[j] * out[i + n];
        out[i + n] = out[i] - t;
        out[i] += t;
    }
}
vector<ftype> evaluate(vector<int> p) {
    while(__builtin_popcount(p.size()) != 1) {
        p.push_back(0);
    }
    vector<ftype> res(p.size());
    fft(p.data(), res.data(), p.size());
    return res;
}
vector<int> interpolate(vector<ftype> p) {
    int n = p.size();
    vector<ftype> inv(n);
    fft(p.data(), inv.data(), n);
    vector<int> res(n);
    for(int i = 0; i < n; i++) {
        res[i] = round(real(inv[i]) / n);
    }
    reverse(begin(res) + 1, end(res));
    return res;
}
void align(vector<int> &a, vector<int> &b) {
    int n = a.size() + b.size() - 1;
    while(a.size() < n) {
        a.push_back(0);
    }
    while(b.size() < n) {
        b.push_back(0);
    }
}
vector<int> poly_multiply(vector<int> a, vector<int> b) {
    align(a, b);
    auto A = evaluate(a);
    auto B = evaluate(b);
    for(int i = 0; i < A.size(); i++) {
        A[i] *= B[i];
    }
    return interpolate(A);
}
const int base = 10;
vector<int> normalize(vector<int> c) {
    int carry = 0;
    for(auto &it: c) {
        it += carry;
        carry = it / base;
        it %= base;
    }
    while(carry) {
        c.push_back(carry % base);
        carry /= base;
    }
    return c;
} 
//multiple of two number
vector<int> multiply(vector<int> a, vector<int> b) {
    return normalize(poly_multiply(a, b));
}
int main() {
    init(); //coef of x^0 x^1 ...
    vector<int>a, b, ans;
    for(int i = 0 ; i <= 100000 ; i++) {
        a.push_back(1);
        b.push_back(1);
    }
    
    ans = poly_multiply(a, b);
    while(ans.back() == 0) ans.pop_back();
    for(int i = 0 ; i < ans.size() ; i++)
        cout<<i<<" "<<ans[i]<<endl;
}