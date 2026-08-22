#include <iostream>

using namespace std;

int main() {
    int n;
    int count = 0;

    cout << "请输入物品价值 n: ";
    if (!(cin >> n)) return 0;

    cout << "所有可能的支付方式如下：" << endl;
    cout << "1元\t2元\t5元" << endl;
    cout << "------------------------" << endl;

    // 穷举 5 元硬币的数量
    for (int five = 0; five <= n / 5; ++five) {
        // 穷举 2 元硬币的数量
        for (int two = 0; two <= (n - five * 5) / 2; ++two) {
            // 剩余金额全部由 1 元硬币支付
            int one = n - five * 5 - two * 2;
            
            // 输出当前组合
            cout << one << "\t" << two << "\t" << five << endl;
            count++;
        }
    }

    cout << "------------------------" << endl;
    cout << "共有 " << count << " 种支付方式。" << endl;

    return 0;
}