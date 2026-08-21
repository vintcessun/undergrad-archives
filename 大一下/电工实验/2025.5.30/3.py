import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit

# 解决中文显示问题
plt.rcParams["font.sans-serif"] = ["SimHei"]
plt.rcParams["axes.unicode_minus"] = False

# 数据定义
U1 = np.array([43, 40, 36, 33, 27, 22, 16, 12])
I1 = np.array([0.403, 0.281, 0.206, 0.163, 0.105, 0.084, 0.066, 0.059])


# 定义不同阶数的多项式拟合函数
def cubic_func(x, a, b, c, d):
    return a * x**3 + b * x**2 + c * x + d


# 执行拟合
popt_cubic, _ = curve_fit(cubic_func, U1, I1)

# 生成平滑曲线数据（增加到500个点进一步提升平滑度）
x_fit = np.linspace(U1.min(), U1.max(), 500)
y_cubic = cubic_func(x_fit, *popt_cubic)

# 创建画布
plt.figure(figsize=(16, 12))

# 绘制原始数据点
plt.plot(U1, I1, "o", color="black", label="原始数据点", markersize=8)

# 绘制不同阶数的拟合曲线
plt.plot(
    x_fit,
    y_cubic,
    "-",
    linewidth=2,
    label=f"$I_1 = {popt_cubic[0]:.8f}U_1^3 + {popt_cubic[1]:.6f}U_1^2 + {popt_cubic[2]:.6f}U_1 + {popt_cubic[3]:.6f}$",
)


# 图表设置
plt.title("变压器空载特性曲线的多项式拟合对比")
plt.xlabel("U1(V)")
plt.ylabel("I1(A)")
plt.legend(loc="upper left", fontsize=10)
plt.grid(linestyle="--", alpha=0.7)
plt.ylim(0, I1.max() * 1.1)  # 设置y轴下限为0，更好展示电流特性

# 显示图形
plt.tight_layout()
plt.show()
