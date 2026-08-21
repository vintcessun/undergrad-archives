import matplotlib.pyplot as plt
import numpy as np

# 解决中文显示问题
plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False

# 高通滤波器数据
f_high = np.array([1, 3, 6, 8.1, 10, 15, 20])
Uo_high = np.array([0.26013, 0.7271, 1.2107, 1.414, 1.5351, 1.6805, 1.7805])
Ui_high = 2
fc_high = 7.81
y_high = 1 / ((Uo_high / Ui_high) ** 2)
x_high = 1 / (f_high ** 2)

# 线性拟合高通
coefficients_high = np.polyfit(x_high, y_high, 1)
b_high, a_high = coefficients_high
fit_high = a_high + b_high * x_high

# 低通滤波器数据
f_low = np.array([1, 3, 6, 7.2, 10, 15, 20])
Uo_low = np.array([1.9809, 1.8467, 1.5376, 1.414, 1.1699, 0.8666, 0.6782])
Ui_low = 2
fc_low = 6.7
y_low = 1 / ((Uo_low / Ui_low) ** 2)
x_low = f_low ** 2

# 线性拟合低通
coefficients_low = np.polyfit(x_low, y_low, 1)
b_low, a_low = coefficients_low
fit_low = a_low + b_low * x_low

# 绘制高通滤波器
plt.subplot(1, 2, 1)
plt.scatter(x_high, y_high, label='数据点', color='blue')
plt.plot(x_high, fit_high, 'r--', label=f'拟合表达式: y = {a_high:.4f} + {b_high:.4f}x')
plt.xlabel('1/f^2 (kHz^-2)')
plt.ylabel('1/( (Uo/Ui)^2 )')
plt.title('表8 - 1 - 2 高通滤波器频率特性实验数据')
plt.legend()

# 绘制低通滤波器
plt.subplot(1, 2, 2)
plt.scatter(x_low, y_low, label='数据点', color='green')
plt.plot(x_low, fit_low, 'r--', label=f'拟合表达式: y = {a_low:.4f} + {b_low:.4f}x')
plt.xlabel('f^2 (kHz^2)')
plt.ylabel('1/( (Uo/Ui)^2 )')
plt.title('表8 - 1 - 3 低通滤波器频率特性实验数据')
plt.legend()

# 设置图片大小
plt.figure(figsize=(16, 8))
plt.tight_layout()
plt.show()