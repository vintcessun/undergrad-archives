#include <bits/stdc++.h>
#include <optional>
using namespace std;

template<class T>
requires totally_ordered<T>
T quick_select(vector<T>& nums, const int left, const int right, const int k) {
	if (left == right) return nums[left];

	auto pivot = nums[left + (right - left) / 2];
	auto i = left - 1, j = right + 1;
	while (i < j) {
		do i++; while (nums[i] < pivot);
		do j--; while (nums[j] > pivot);
		if (i < j) swap(nums[i], nums[j]);
	}

	auto cnt = j - left + 1;
	if (k <= cnt)return quick_select(nums, left, j, k);
	else return quick_select(nums, j + 1, right, k - cnt);
}

template<class T>
requires totally_ordered<T>
optional<T> solve(vector<T>& nums) {
	int n = (int)nums.size();
	auto candidate = quick_select(nums, 0, n - 1, (n + 1) / 2);

	auto cnt = 0;
	for (const auto& e : nums) if (e == candidate) cnt++;
	if (cnt > n / 2) return candidate;
	return nullopt;
}

template<class T>
ostream& operator<<(ostream& os, const optional<T> opt) {
	if (opt.has_value()) {
		os << "Some(" << opt.value() << ")";
	}
	else {
		os << "None";
	}
	return os;
}

struct Point {
	int x, y;
};

int main() {
	// 测试用例 1: 整数数组 (有主元素 3)
	std::vector<int> nums1 = { 3, 1, 3, 2, 3, 3, 4 };
	// 测试用例 2: 整数数组 (无主元素)
	std::vector<int> nums2 = { 1, 2, 3, 4 };

	// 测试用例 3: 坐标数组 (有主元素 {1,1})
	//std::vector<Point> pts1 = { {1,1}, {2,2}, {1,1}, {3,3}, {1,1} };
	// 测试用例 4: 坐标数组 (无主元素)
	//std::vector<Point> pts2 = { {0,0}, {1,1}, {2,2} };

	cout << "--- 算法测试开始 ---" << endl;

	cout << "测试用例1: " << solve(nums1) << endl;
	cout << "测试用例2: " << solve(nums2) << endl;

	//solve(pts1);
	//solve(pts2);

	return 0;
}