import numpy as np
import matplotlib.pyplot as plt

# 解决中文显示问题
plt.rcParams["font.sans-serif"] = ["SimHei"]
plt.rcParams["axes.unicode_minus"] = False

# 幅频特性数据
f_amp = np.array([30, 90, 200, 300, 367, 500, 1000, 2000, 4000, 8000, 35000])
Uo_amp = np.array(
    [
        0.0835,
        0.20905,
        0.30645,
        0.32767,
        0.3294,
        0.31958,
        0.25108,
        0.15538,
        0.08356,
        0.04276,
        0.00988,
    ]
)
R = 2000
C = 0.2e-6

# 计算理论谐振频率和幅频最大值
f_0_theory = 1 / (2 * np.pi * R * C)  # 理论谐振频率计算
A_max_theory = 1 / 3  # 理论幅频最大值为1/3

# 生成更多拟合点（幅频）
f_amp_fit = np.logspace(np.log10(30), np.log10(35000), 1000)
omega_amp = 2 * np.pi * f_amp_fit
A_omega = 1 / np.sqrt(9 + (omega_amp * R * C - 1 / (omega_amp * R * C)) ** 2)

# 找拟合曲线的最大值及其频率
A_max_fit = np.max(A_omega)
index_max = np.argmax(A_omega)
f_0_fit = f_amp_fit[index_max]

# 绘制幅频特性
plt.figure(figsize=(16, 8))
plt.plot(
    f_amp_fit,
    A_omega,
    label=f"幅频特性拟合表达式：$A(\\omega) = \\frac{{1}}{{\\sqrt{{3^2 + \\left(\\omega RC - \\frac{{1}}{{\\omega RC}}\\right)^2}}}}$，$R={R / 1000}k\\Omega$，$C={C * 1e6}\\mu F$",
)
plt.scatter(f_amp, Uo_amp, color="red", label="数据点")
# 标注理论值和拟合最大值（理论值在左，最大值在右）
plt.annotate(
    f"理论值：\n谐振频率：{f_0_theory:.2f} Hz\n幅频最大值：{A_max_theory:.4f}",
    xy=(f_0_theory, A_max_theory),
    xytext=(f_0_theory - 180, A_max_theory - 0.05),
    arrowprops=dict(facecolor="black", shrink=0.05),
)
plt.annotate(
    f"拟合最大值：\n谐振频率：{f_0_fit:.2f} Hz\n幅频最大值：{A_max_fit:.4f}",
    xy=(f_0_fit, A_max_fit),
    xytext=(f_0_fit + 180, A_max_fit - 0.05),
    arrowprops=dict(facecolor="black", shrink=0.05),
)
plt.xlabel("频率 f (Hz)")
plt.ylabel("输出电压 Uo (V)")
plt.title("表8 - 2 - 1 幅频特性数据")
plt.xscale("log")
plt.legend()

# 相频特性数据
f_phase = np.array([30, 90, 200, 300, 367, 500, 1000, 2000, 4000, 8000, 35000])
phase_meas = np.array(
    [77.39, 47.59, 19.69, 4.21, -2.22, -16.12, -40.41, -62.81, -75.017, -87.47, -88.012]
)

# 生成更多拟合点（相频）
f_phase_fit = np.logspace(np.log10(30), np.log10(35000), 1000)
omega_phase = 2 * np.pi * f_phase_fit
phi_omega = -np.arctan((omega_phase * R * C - 1 / (omega_phase * R * C)) / 3) * (
    180 / np.pi
)  # 转换为角度

# 绘制相频特性
plt.figure(figsize=(16, 8))
plt.plot(
    f_phase_fit,
    phi_omega,
    label=f"相频特性拟合表达式：$\\varphi(\\omega) = -\\arctan\\left(\\frac{{\\omega RC - \\frac{{1}}{{\\omega RC}}}}{{3}}\\right)$，$R={R / 1000}k\\Omega$，$C={C * 1e6}\\mu F$",
)
plt.scatter(f_phase, phase_meas, color="red", label="数据点")
plt.xlabel("频率 f (Hz)")
plt.ylabel("相位差 φ (度)")
plt.title("表8 - 2 - 2 相频特性数据")
plt.xscale("log")
plt.legend()

plt.show()
