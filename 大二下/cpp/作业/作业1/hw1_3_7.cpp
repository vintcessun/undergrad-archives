#include <iostream>
#include <cmath>

using namespace std;

int main() {
    double weight, distance;
    double totalFee = 0;

    cout << "请输入包裹重量（克）和邮寄距离（千米）: ";
    if (!(cin >> weight >> distance)) {
        return 1;
    }

    // 根据重量确定基础计费逻辑
    if (weight <= 15) {
        totalFee = 5;
    } else if (weight <= 30) {
        totalFee = 9;
    } else if (weight <= 45) {
        totalFee = 12;
    } else if (weight <= 60) {
        // 14元基础费 + 每满1000千米加收1元
        totalFee = 14 + floor(distance / 1000) * 1;
    } else {
        // 60克以上：15元基础费 + 每满1000千米加收2元
        totalFee = 15 + floor(distance / 1000) * 2;
    }

    cout << "邮寄收费总额为: " << totalFee << " 元" << endl;

    return 0;
}