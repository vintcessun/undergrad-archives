import matplotlib.pyplot as plt
import numpy as np
from matplotlib.patches import Arrow, Circle, Rectangle
from matplotlib.lines import Line2D
import matplotlib.patheffects as path_effects

# 设置中文字体
plt.rcParams["font.family"] = ["SimHei", "WenQuanYi Micro Hei", "Heiti TC"]
plt.rcParams["axes.unicode_minus"] = False  # 正确显示负号

# 创建图形
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 8), dpi=300)
fig.suptitle("直流法判断同名端示意图", fontsize=18, y=0.95)


# 定义绘制线圈的函数
def draw_coil(ax, x, y, width, height, name, dot_pos="left"):
    # 绘制线圈
    for i in range(3):
        ax.add_patch(
            Rectangle(
                (x + i * width / 3, y), width / 3, height, fill=False, linewidth=2
            )
        )

    # 绘制同名端标记
    if dot_pos == "left":
        ax.add_patch(Circle((x, y + height / 2), width / 10, fill=True, color="black"))
    else:
        ax.add_patch(
            Circle((x + width, y + height / 2), width / 10, fill=True, color="black")
        )

    # 添加线圈名称
    ax.text(
        x + width / 2,
        y + height * 1.1,
        f"线圈{name}",
        ha="center",
        fontsize=12,
        path_effects=[
            path_effects.Stroke(linewidth=1, foreground="white"),
            path_effects.Normal(),
        ],
    )


# 绘制铁棒插入场景
def draw_insert_scene(ax):
    # 绘制线圈1（接电源）
    draw_coil(ax, 1, 2, 1.5, 1, "1")
    ax.text(
        0.5,
        2.5,
        "直流电源",
        fontsize=12,
        path_effects=[
            path_effects.Stroke(linewidth=1, foreground="white"),
            path_effects.Normal(),
        ],
    )

    # 绘制电源符号
    ax.add_patch(Rectangle((0.8, 2.2), 0.2, 0.6, fill=False, linewidth=2))
    ax.add_patch(Rectangle((1, 2.5), 0.1, 0.2, fill=True, color="black"))

    # 绘制线圈2（接电流表）
    draw_coil(ax, 4, 2, 1.5, 1, "2", dot_pos="right")
    ax.text(
        5.5,
        2.5,
        "模拟电流表",
        fontsize=12,
        path_effects=[
            path_effects.Stroke(linewidth=1, foreground="white"),
            path_effects.Normal(),
        ],
    )

    # 绘制电流表符号
    ax.add_patch(Rectangle((4.8, 1.8), 0.9, 0.8, fill=False, linewidth=2))
    ax.plot([5.25, 5.65], [2, 2], "r-", linewidth=3)  # 指针

    # 绘制铁棒
    ax.add_patch(
        Rectangle((2.75, 1), 0.5, 2, fill=True, color="gray", hatch="///", label="铁棒")
    )
    ax.arrow(
        2.75,
        1,
        0,
        0.5,
        head_width=0.2,
        head_length=0.1,
        fc="gray",
        ec="gray",
        label="插入方向",
    )

    # 添加说明文字
    ax.text(
        3.25,
        0.5,
        "铁棒迅速插入瞬间",
        ha="center",
        fontsize=12,
        path_effects=[
            path_effects.Stroke(linewidth=1, foreground="white"),
            path_effects.Normal(),
        ],
    )
    ax.text(
        3.25,
        0.2,
        "电流表指针正偏 → 同名端如图标记",
        ha="center",
        fontsize=10,
        path_effects=[
            path_effects.Stroke(linewidth=1, foreground="white"),
            path_effects.Normal(),
        ],
    )

    # 绘制连接线
    ax.plot([2.5, 4], [2.5, 2.5], "k-", linewidth=2)  # 线圈1到线圈2的连接线
    ax.plot([1, 1], [2.5, 3], "k-", linewidth=2)  # 电源正极连接线
    ax.plot([2.5, 2.5], [3, 3], "k-", linewidth=2)  # 连接线
    ax.plot([4, 5.7], [2.5, 2.5], "k-", linewidth=2)  # 线圈2到电流表的连接线
    ax.plot([1, 1], [1.5, 1], "k-", linewidth=2)  # 电源负极连接线
    ax.plot([1, 2.5], [1, 1], "k-", linewidth=2)  # 连接线
    ax.plot([5.7, 5.7], [2.5, 1.8], "k-", linewidth=2)  # 电流表连接线
    ax.plot([4, 4], [1.5, 1], "k-", linewidth=2)  # 线圈2负极连接线
    ax.plot([4, 5.7], [1, 1], "k-", linewidth=2)  # 连接线


# 绘制铁棒拔出场景
def draw_pull_out_scene(ax):
    # 绘制线圈1（接电源）
    draw_coil(ax, 1, 2, 1.5, 1, "1")
    ax.text(
        0.5,
        2.5,
        "直流电源",
        fontsize=12,
        path_effects=[
            path_effects.Stroke(linewidth=1, foreground="white"),
            path_effects.Normal(),
        ],
    )

    # 绘制电源符号
    ax.add_patch(Rectangle((0.8, 2.2), 0.2, 0.6, fill=False, linewidth=2))
    ax.add_patch(Rectangle((1, 2.5), 0.1, 0.2, fill=True, color="black"))

    # 绘制线圈2（接电流表）
    draw_coil(ax, 4, 2, 1.5, 1, "2", dot_pos="right")
    ax.text(
        5.5,
        2.5,
        "模拟电流表",
        fontsize=12,
        path_effects=[
            path_effects.Stroke(linewidth=1, foreground="white"),
            path_effects.Normal(),
        ],
    )

    # 绘制电流表符号（指针反偏）
    ax.add_patch(Rectangle((4.8, 1.8), 0.9, 0.8, fill=False, linewidth=2))
    ax.plot([5.65, 5.25], [2, 2], "r-", linewidth=3)  # 反偏指针

    # 绘制铁棒
    ax.add_patch(
        Rectangle((2.75, 1), 0.5, 2, fill=True, color="gray", hatch="///", label="铁棒")
    )
    ax.arrow(
        2.75,
        3,
        0,
        -0.5,
        head_width=0.2,
        head_length=0.1,
        fc="gray",
        ec="gray",
        label="拔出方向",
    )

    # 添加说明文字
    ax.text(
        3.25,
        0.5,
        "铁棒迅速拔出瞬间",
        ha="center",
        fontsize=12,
        path_effects=[
            path_effects.Stroke(linewidth=1, foreground="white"),
            path_effects.Normal(),
        ],
    )
    ax.text(
        3.25,
        0.2,
        "电流表指针反偏 → 验证同名端标记",
        ha="center",
        fontsize=10,
        path_effects=[
            path_effects.Stroke(linewidth=1, foreground="white"),
            path_effects.Normal(),
        ],
    )

    # 绘制连接线（与插入场景相同）
    ax.plot([2.5, 4], [2.5, 2.5], "k-", linewidth=2)
    ax.plot([1, 1], [2.5, 3], "k-", linewidth=2)
    ax.plot([2.5, 2.5], [3, 3], "k-", linewidth=2)
    ax.plot([4, 5.7], [2.5, 2.5], "k-", linewidth=2)
    ax.plot([1, 1], [1.5, 1], "k-", linewidth=2)
    ax.plot([1, 2.5], [1, 1], "k-", linewidth=2)
    ax.plot([5.7, 5.7], [2.5, 1.8], "k-", linewidth=2)
    ax.plot([4, 4], [1.5, 1], "k-", linewidth=2)
    ax.plot([4, 5.7], [1, 1], "k-", linewidth=2)


# 绘制两个场景
draw_insert_scene(ax1)
draw_pull_out_scene(ax2)

# 设置坐标轴属性
for ax in [ax1, ax2]:
    ax.set_xlim(0, 7)
    ax.set_ylim(0, 4)
    ax.axis("off")
    ax.set_title("", fontsize=14)

# 添加整体说明
fig.text(
    0.5,
    0.02,
    "说明：当铁棒插入时，磁通量增加，线圈2中产生感应电流使电流表正偏；\n"
    "当铁棒拔出时，磁通量减少，感应电流方向相反导致电流表反偏。\n"
    "根据指针偏转方向可判断两线圈的同名端（图中黑点标记处）。",
    ha="center",
    fontsize=12,
    wrap=True,
)

# 调整布局并显示
plt.tight_layout(rect=[0, 0, 1, 0.95])
plt.savefig("dc_method_coil_dots.png", dpi=300, bbox_inches="tight")
plt.show()
