#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;

// 定义学生信息结构体
struct Student {
    string id;              // 学号
    string name;            // 姓名
    double scores[8];       // 8门课成绩
    double average;         // 平均分

    // 计算平均分的成员函数
    void calculateAverage() {
        double sum = 0;
        for (int i = 0; i < 8; ++i) {
            sum += scores[i];
        }
        average = sum / 8.0;
    }
};

// 比较函数：用于 sort 降序排列
bool compareStudents(const Student& a, const Student& b) {
    return a.average > b.average; 
}

int main() {
    int n;
    cout << "请输入学生人数: ";
    cin >> n;

    vector<Student> students(n);

    // 输入信息
    for (int i = 0; i < n; ++i) {
        cout << "正在输入第 " << i + 1 << " 个学生的信息：" << endl;
        cout << "学号: "; cin >> students[i].id;
        cout << "姓名: "; cin >> students[i].name;
        cout << "请输入8门课的成绩: ";
        for (int j = 0; j < 8; ++j) {
            cin >> students[i].scores[j];
        }
        students[i].calculateAverage(); // 计算平均分
    }

    // 按照平均成绩由高到低排序
    sort(students.begin(), students.end(), compareStudents);

    // 输出结果
    cout << "\n--- 学生成绩排名表 (按平均分降序) ---\n";
    cout << left << setw(15) << "学号" << setw(10) << "姓名";
    for (int j = 1; j <= 8; ++j) cout << "课" << j << "  ";
    cout << "平均分" << endl;

    for (const auto& s : students) {
        cout << left << setw(15) << s.id << setw(10) << s.name;
        for (int j = 0; j < 8; ++j) {
            cout << fixed << setprecision(1) << s.scores[j] << " ";
        }
        cout << " [" << s.average << "]" << endl;
    }

    return 0;
}