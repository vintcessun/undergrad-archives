import numpy as np
import matplotlib.pyplot as plt

# 解决中文显示问题
plt.rcParams["font.sans-serif"] = ["SimHei"]
plt.rcParams["axes.unicode_minus"] = False

# 双T电路幅频特性数据
f_kHz = np.array([1, 3, 7, 10, 12, 14, 15.92, 17, 20, 25, 30, 40, 100, 200, 300])
Uo = np.array(
    [
        0.9783,
        0.7862,
        0.41263,
        0.23411,
        0.11584,
        0.05668,
        0.0123,
        0.03018,
        0.12775,
        0.20158,
        0.28467,
        0.41026,
        0.6854,
        1,
        0.79,
    ]
)

R = 1000  # 1kΩ
C = 0.01e-6  # 0.01μF

# 生成更多拟合点
f_fit = np.logspace(0, 5, 1000)  # 覆盖较广频率范围
omega = 2 * np.pi * f_fit * 1000  # 将kHz转换为Hz计算角频率
numerator = np.abs(1 - (omega**2) * (C**2) * (R**2))
denominator = np.sqrt(
    (1 - (omega**2) * (C**2) * (R**2)) ** 2 + (4 * omega * C * R) ** 2
)
A_omega = numerator / denominator

# 绘制幅频特性曲线
plt.figure(figsize=(16, 8))
plt.plot(
    f_fit,
    A_omega,
    label=f"幅频特性拟合表达式：$A(\\omega) = \\frac{{|1 - \\omega^{2} C^{2} R^{2}|}}{{\\sqrt{{(1 - \\omega^{2} C^{2} R^{2})^{2} + (4 \\omega C R)^{2}}}}}$，$R={R / 1000}k\\Omega$，$C={C * 1e6}\\mu F$",
)
plt.scatter(f_kHz, Uo, color="red", label="数据点")
plt.xlabel("频率 f (kHz)")
plt.ylabel("输出电压 Uo (V)")
plt.title("表8 - 2 - 3 双T电路的幅频特性数据")
plt.xscale("log")  # 对数刻度适应宽频率范围
plt.legend()

plt.show()
