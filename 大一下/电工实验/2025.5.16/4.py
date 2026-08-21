import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import FancyArrowPatch

# 设置中文字体支持
plt.rcParams["font.family"] = ["SimHei"]
plt.rcParams["axes.unicode_minus"] = False  # 确保负号正确显示

# 实验数据
# 相电压(V)
U_UV = 220.38
U_VW = 219.16
U_WU = 216.7

# 相电流(mA)
I_UX = 65.9
I_VY = 67.5
I_WZ = 67.6

# 线电流(mA) - 计算值
I_U = np.sqrt((I_UX * np.cos(np.deg2rad(0)) - I_WZ * np.cos(np.deg2rad(120)))**2 + 
              (I_UX * np.sin(np.deg2rad(0)) - I_WZ * np.sin(np.deg2rad(120)))**2)
I_V = np.sqrt((I_VY * np.cos(np.deg2rad(-120)) - I_UX * np.cos(np.deg2rad(0)))**2 + 
              (I_VY * np.sin(np.deg2rad(-120)) - I_UX * np.sin(np.deg2rad(0)))**2)
I_W = np.sqrt((I_WZ * np.cos(np.deg2rad(120)) - I_VY * np.cos(np.deg2rad(-120)))**2 + 
              (I_WZ * np.sin(np.deg2rad(120)) - I_VY * np.sin(np.deg2rad(-120)))**2)

# 设定相电压角度
angle_UV = 0
angle_VW = -120
angle_WU = 120

# 创建图形
plt.figure(figsize=(10, 8))
ax = plt.gca()

# 绘制带箭头的相电压相量
def plot_arrow(x_start, y_start, x_end, y_end, color, linestyle, label, width=0.5):
    arrow = FancyArrowPatch((x_start, y_start), (x_end, y_end), 
                           arrowstyle='->, head_width=6, head_length=8',
                           color=color, linestyle=linestyle, linewidth=width,
                           label=label)
    ax.add_patch(arrow)

# 绘制相电压相量
plot_arrow(0, 0, U_UV * np.cos(np.deg2rad(angle_UV)), U_UV * np.sin(np.deg2rad(angle_UV)),
          'blue', '-', f'相电压 $U_{{UV}}$ = {U_UV:.2f}V', 2)
plot_arrow(0, 0, U_VW * np.cos(np.deg2rad(angle_VW)), U_VW * np.sin(np.deg2rad(angle_VW)),
          'green', '-', f'相电压 $U_{{VW}}$ = {U_VW:.2f}V', 2)
plot_arrow(0, 0, U_WU * np.cos(np.deg2rad(angle_WU)), U_WU * np.sin(np.deg2rad(angle_WU)),
          'red', '-', f'相电压 $U_{{WU}}$ = {U_WU:.2f}V', 2)

# 绘制相电流相量
plot_arrow(0, 0, I_UX * np.cos(np.deg2rad(angle_UV)), I_UX * np.sin(np.deg2rad(angle_UV)),
          'blue', '--', f'相电流 $I_{{UX}}$ = {I_UX:.2f}mA', 1.5)
plot_arrow(0, 0, I_VY * np.cos(np.deg2rad(angle_VW)), I_VY * np.sin(np.deg2rad(angle_VW)),
          'green', '--', f'相电流 $I_{{VY}}$ = {I_VY:.2f}mA', 1.5)
plot_arrow(0, 0, I_WZ * np.cos(np.deg2rad(angle_WU)), I_WZ * np.sin(np.deg2rad(angle_WU)),
          'red', '--', f'相电流 $I_{{WZ}}$ = {I_WZ:.2f}mA', 1.5)

# 计算并绘制线电流相量
I_U_x = I_UX * np.cos(np.deg2rad(angle_UV)) - I_WZ * np.cos(np.deg2rad(angle_WU))
I_U_y = I_UX * np.sin(np.deg2rad(angle_UV)) - I_WZ * np.sin(np.deg2rad(angle_WU))
plot_arrow(0, 0, I_U_x, I_U_y, 'blue', '-.', f'线电流 $I_U$ = {I_U:.2f}mA', 1.5)

I_V_x = I_VY * np.cos(np.deg2rad(angle_VW)) - I_UX * np.cos(np.deg2rad(angle_UV))
I_V_y = I_VY * np.sin(np.deg2rad(angle_VW)) - I_UX * np.sin(np.deg2rad(angle_UV))
plot_arrow(0, 0, I_V_x, I_V_y, 'green', '-.', f'线电流 $I_V$ = {I_V:.2f}mA', 1.5)

I_W_x = I_WZ * np.cos(np.deg2rad(angle_WU)) - I_VY * np.cos(np.deg2rad(angle_VW))
I_W_y = I_WZ * np.sin(np.deg2rad(angle_WU)) - I_VY * np.sin(np.deg2rad(angle_VW))
plot_arrow(0, 0, I_W_x, I_W_y, 'red', '-.', f'线电流 $I_W$ = {I_W:.2f}mA', 1.5)

# 标注各相量的大小和角度
def add_annotation(x_end, y_end, text, color):
    plt.annotate(text,
                xy=(x_end, y_end),
                xytext=(x_end*1.1, y_end*1.1),
                arrowprops=dict(arrowstyle='->', color=color),
                color=color)

# 标注相电压
add_annotation(U_UV * np.cos(np.deg2rad(angle_UV)), U_UV * np.sin(np.deg2rad(angle_UV)), 
               f"U_UV={U_UV:.2f}V\n∠{angle_UV}°", 'blue')
add_annotation(U_VW * np.cos(np.deg2rad(angle_VW)), U_VW * np.sin(np.deg2rad(angle_VW)), 
               f"U_VW={U_VW:.2f}V\n∠{angle_VW}°", 'green')
add_annotation(U_WU * np.cos(np.deg2rad(angle_WU)), U_WU * np.sin(np.deg2rad(angle_WU)), 
               f"U_WU={U_WU:.2f}V\n∠{angle_WU}°", 'red')

# 标注相电流
add_annotation(I_UX * np.cos(np.deg2rad(angle_UV)), I_UX * np.sin(np.deg2rad(angle_UV)), 
               f"I_UX={I_UX:.2f}mA", 'blue')
add_annotation(I_VY * np.cos(np.deg2rad(angle_VW)), I_VY * np.sin(np.deg2rad(angle_VW)), 
               f"I_VY={I_VY:.2f}mA", 'green')
add_annotation(I_WZ * np.cos(np.deg2rad(angle_WU)), I_WZ * np.sin(np.deg2rad(angle_WU)), 
               f"I_WZ={I_WZ:.2f}mA", 'red')

# 标注线电流
add_annotation(I_U_x, I_U_y, f"I_U={I_U:.2f}mA", 'blue')
add_annotation(I_V_x, I_V_y, f"I_V={I_V:.2f}mA", 'green')
add_annotation(I_W_x, I_W_y, f"I_W={I_W:.2f}mA", 'red')

# 设置中文标签
plt.xlabel('实轴')
plt.ylabel('虚轴')
plt.title('不对称负载三角形连接的相量图')
plt.legend(loc='upper right')
plt.grid(True)
plt.axis('equal')
plt.axis([-250, 250, -250, 250])  # 设置坐标轴范围

# 添加说明文本
plt.figtext(0.15, 0.01, f"实验数据: 相电压(U_UV={U_UV}V, U_VW={U_VW}V, U_WU={U_WU}V), "
                       f"相电流(I_UX={I_UX}mA, I_VY={I_VY}mA, I_WZ={I_WZ}mA), "
                       f"线电流(I_U={I_U:.1f}mA, I_V={I_V:.1f}mA, I_W={I_W:.1f}mA)", 
           fontsize=9, ha="left")

plt.tight_layout()
plt.show()