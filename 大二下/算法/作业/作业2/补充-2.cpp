#include <bits/stdc++.h>
#include <optional>
using namespace std;

template<class T>
requires equality_comparable<T>
optional<T> pair_choose(vector<T>& nums) {
	if (nums.size() == 0)return nullopt;
	if (nums.size() == 1)return nums[0];
	int n = (int)nums.size();
	vector<T> next_nums;
	if (n % 2 == 1) {
		next_nums.push_back(nums[0]);
		for (int i = 1; i < n; i += 2) {
			if (nums[i] == nums[i + 1])next_nums.push_back(nums[i]);
		}
	}
	else {
		for (int i = 0; i < n; i += 2) {
			if (nums[i] == nums[i + 1])next_nums.push_back(nums[i]);
		}
	}
	return pair_choose(next_nums);
}

template<class T>
requires equality_comparable<T>
optional<T> solve(vector<T>& nums) {
	int n = (int)nums.size();
	auto candidate = pair_choose(nums);

	auto cnt = 0;
	for (const auto& e : nums) if (e == candidate) cnt++;
	if (cnt > n / 2) return candidate;
	return nullopt;
}

template <typename T>
concept can_stream_out = requires(std::ostream & os, T const& a) {
	{ os << a } -> std::convertible_to<std::ostream&>;
};

template<class T>
requires can_stream_out<T>
ostream& operator<<(ostream& os, const optional<T>& opt) {
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
	bool operator==(const Point&) const = default;
};

ostream& operator<<(ostream& os, const Point& pt) {
	os << "(" << pt.x << ", " << pt.y << ")";
	return os;
}

int main() {
	// 测试用例 1: 整数数组 (有主元素 3)
	std::vector<int> nums1 = { 3, 1, 3, 2, 3, 3, 4 };
	// 测试用例 2: 整数数组 (无主元素)
	std::vector<int> nums2 = { 1, 2, 3, 4 };

	// 测试用例 3: 坐标数组 (有主元素 {1,1})
	std::vector<Point> pts1 = { {1,1}, {2,2}, {1,1}, {3,3}, {1,1} };
	// 测试用例 4: 坐标数组 (无主元素)
	std::vector<Point> pts2 = { {0,0}, {1,1}, {2,2} };

	cout << "--- 算法测试开始 ---" << endl;

	cout << "测试用例1: " << solve(nums1) << endl;
	cout << "测试用例2: " << solve(nums2) << endl;

	cout << "测试用例3: " << solve(pts1) << endl;
	cout << "测试用例4: " << solve(pts2) << endl;

	return 0;
}