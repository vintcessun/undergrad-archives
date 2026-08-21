import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# 解决中文显示问题
plt.rcParams["font.sans-serif"] = ["SimHei"]
plt.rcParams["axes.unicode_minus"] = False


# 定义带通滤波器函数
def bandpass(f, Ui, R, L, C):
    f0 = 1 / (2 * np.pi * np.sqrt(L * C))
    numerator = Ui * R * f
    denominator = np.sqrt((f**2 - f0**2) ** 2 + (f * R / L) ** 2)
    return numerator / denominator


# 输入实验数据
f_band = np.array([1, 1.3, 2, 3, 4.1, 6, 10, 12.3, 15, 20])
Uo_band = np.array(
    [1.0747, 1.2909, 1.5657, 1.7299, 1.8109, 1.7097, 1.4394, 1.2825, 1.1747, 0.9714]
)

# 曲线拟合
params, cov = curve_fit(bandpass, f_band, Uo_band)
Ui_fit, R_fit, L_fit, C_fit = params

# 生成拟合曲线数据
f_fit = np.linspace(min(f_band), max(f_band), 1000)
Uo_fit = bandpass(f_fit, Ui_fit, R_fit, L_fit, C_fit)

# 绘制图形
plt.scatter(f_band, Uo_band, label="数据点", color="blue")
plt.plot(
    f_fit,
    Uo_fit,
    "r--",
    label=f"拟合表达式：\n$A(f) = \\frac{{fR}}{{\\sqrt{{(f^2 - f_0^2)^2 + (f\\frac{{R}}{{L}})^2}}}}$，其中$f_0 = \\frac{{1}}{{2\\pi\\sqrt{{LC}}}}$",
)
plt.xlabel("频率f（kHz）")
plt.ylabel("Uo（V）")
plt.title("表8-1-4 带通滤波器频率特性实验数据")
plt.legend()
plt.figure(figsize=(16, 8))
plt.tight_layout()
plt.show()

# 计算通频带
fc1 = 1.2
fc2 = 14
fBW = fc2 - fc1
print(f"通频带fBW = {fBW} kHz")
