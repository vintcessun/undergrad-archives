import numpy as np
import matplotlib.pyplot as plt

# 设置中文显示
plt.rcParams["font.family"] = ["SimHei", "WenQuanYi Micro Hei", "Heiti TC"]
plt.rcParams["axes.unicode_minus"] = False

def calculate_phasors(UL, UB, R, L, f):
    """计算电路参数和相量关系"""
    omega = 2 * np.pi * f
    ZL = 1j * omega * L
    ZB = R
    Z_total = ZL + ZB
    
    # 计算电流
    I_mag = 220 / np.abs(Z_total)
    I_angle = -np.arctan2(ZL.imag, ZB)  # 电流相位
    
    # 计算各元件电压相量
    UL_phasor = I_mag * np.abs(ZL) * np.exp(1j * (I_angle + np.pi/2))  # 电感电压超前电流90度
    UB_phasor = I_mag * np.abs(ZB) * np.exp(1j * I_angle)  # 电阻电压与电流同相
    US_phasor = UL_phasor + UB_phasor  # 总电压相量
    
    return {
        'UL': UL_phasor,
        'UB': UB_phasor,
        'US': US_phasor,
        'I': I_mag * np.exp(1j * I_angle),
        'Z_total': Z_total,
        'frequency': f,
        'omega': omega
    }

def draw_phasor_diagram(phasors, figsize=(10, 8)):
    """绘制2D相量图"""
    plt.figure(figsize=figsize)
    ax = plt.gca()
    ax.set_aspect('equal')
    ax.set_title('电压相量图')
    ax.set_xlabel('实部')
    ax.set_ylabel('虚部')
    
    # 绘制坐标轴
    ax.axhline(y=0, color='k', linestyle='-', alpha=0.3)
    ax.axvline(x=0, color='k', linestyle='-', alpha=0.3)
    
    # 绘制电压相量
    vectors = [
        (phasors['UB'].real, phasors['UB'].imag, '电阻电压 UB', 'blue'),
        (phasors['UL'].real, phasors['UL'].imag, '镇流器电压 UL', 'red'),
        (phasors['US'].real, phasors['US'].imag, '电源电压 US', 'green')
    ]
    
    max_val = 0
    for x, y, label, color in vectors:
        ax.arrow(0, 0, x, y, head_width=5, head_length=8, fc=color, ec=color, label=label)
        max_val = max(max_val, abs(x), abs(y))
    
    # 添加文本标签
    for x, y, label, _ in vectors:
        ax.text(x*1.1, y*1.1, label, fontsize=10)
    
    ax.legend()
    ax.set_xlim(-max_val*1.2, max_val*1.2)
    ax.set_ylim(-max_val*1.2, max_val*1.2)
    ax.grid(True, linestyle='--', alpha=0.7)
    
    plt.tight_layout()
    plt.show()
    
    # 返回计算结果
    return {
        'UL_magnitude': np.abs(phasors['UL']),
        'UL_angle': np.degrees(np.angle(phasors['UL'])),
        'UB_magnitude': np.abs(phasors['UB']),
        'UB_angle': np.degrees(np.angle(phasors['UB'])),
        'US_magnitude': np.abs(phasors['US']),
        'US_angle': np.degrees(np.angle(phasors['US'])),
        'UL_plus_UB_magnitude': np.abs(phasors['UL'] + phasors['UB']),
        'UL_plus_UB_angle': np.degrees(np.angle(phasors['UL'] + phasors['UB'])),
        'magnitude_difference': np.abs(np.abs(phasors['US']) - np.abs(phasors['UL'] + phasors['UB'])),
        'angle_difference': np.abs(np.angle(phasors['US']) - np.angle(phasors['UL'] + phasors['UB']))
    }

def main():
    """主函数：执行计算和绘图"""
    # 电路参数设置（示例值）
    UL = 180  # 镇流器电压幅值 (V)
    UB = 110  # 日光灯电压幅值 (V)
    R = 100   # 日光灯电阻 (Ω)
    L = 0.5   # 镇流器电感 (H)
    f = 50    # 电源频率 (Hz)
    
    print("正在计算日光灯电路的电压相量关系...")
    print(f"电路参数: R = {R}Ω, L = {L}H, f = {f}Hz")
    
    # 计算相量
    phasors = calculate_phasors(UL, UB, R, L, f)
    
    # 绘制相量图
    results = draw_phasor_diagram(phasors)
    
    # 打印结果
    print("\n计算结果:")
    print(f"镇流器电压 UL: {results['UL_magnitude']:.2f}V, 相位角: {results['UL_angle']:.2f}°")
    print(f"日光灯电压 UB: {results['UB_magnitude']:.2f}V, 相位角: {results['UB_angle']:.2f}°")
    print(f"电源电压 US: {results['US_magnitude']:.2f}V, 相位角: {results['US_angle']:.2f}°")
    print("\n电压幅值关系:")
    print(f"UL + UB 的幅值: {results['UL_plus_UB_magnitude']:.2f}V")
    print(f"US 的幅值: {results['US_magnitude']:.2f}V")
    print(f"幅值差异: {results['magnitude_difference']:.2f}V")
    print("\n电压相位关系:")
    print(f"UL + UB 的相位角: {results['UL_plus_UB_angle']:.2f}°")
    print(f"US 的相位角: {results['US_angle']:.2f}°")
    print(f"相位差异: {np.degrees(results['angle_difference']):.2f}°")
    
    print("\n理论分析:")
    if np.abs(results['magnitude_difference']) < 1e-6 and np.abs(results['angle_difference']) < 1e-6:
        print("UL + UB 完全等于 US，符合基尔霍夫电压定律。")
    else:
        print("UL + UB 不等于 US 的幅值，这是因为镇流器电压与日光灯电压存在相位差。")
        print("在向量相加时，不仅要考虑幅值，还要考虑相位关系。")
        print("向量和的幅值为: |UL + UB| = √(UL² + UB² + 2·UL·UB·cos(φ))")
        print("其中 φ 是 UL 和 UB 之间的相位差。")

if __name__ == "__main__":
    main()    