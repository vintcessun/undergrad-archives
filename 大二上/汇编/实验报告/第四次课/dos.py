import time
import pyautogui
import os


def send_to_dosbox_exact():
    time.sleep(3)
    print("请在3秒内切换到DOSBox窗口...")
    time.sleep(3)

    commands = [
        "debug",
        "a 100",
        "mov ax,31",
        "push ax",
        "mov ax,21",
        "push ax",
        "mov ax,42",
        "push ax",
        "mov ax,52",
        "push ax",
        "mov ax,87",
        "push ax",
        "mov ax,23",
        "push ax",
        "mov ax,98",
        "push ax",
        "mov ax,1",
        "push ax",
        "mov byte ptr [0A00], 0",
        "mov byte ptr [0A01], 0",
        "mov byte ptr [0A02], 0",
        "mov cx, 8",
        "jmp 140",
        "",
        "a 140",
        "pop ax",
        "cmp ax, 42",
        "je 0170",
        "jg 0180",
        "jl 0190",
        "jmp 160",
        "",
        "a 160",
        "dec cx",
        "cmp cx,0",
        "jg 140",
        "call 200",
        "int 3",
        "",
        "a 170",
        "inc byte ptr [0A00]",
        "jmp 160",
        "",
        "a 180",
        "inc byte ptr [0A01]",
        "jmp 160",
        "",
        "a 190",
        "inc byte ptr [0A02]",
        "jmp 160",
        "",
        "a 200",
        "mov al, [0A00]",
        "call 290",
        "mov dl, 0D",
        "int 21",
        "mov dl, 0A",
        "int 21",
        "mov al, [0A01]",
        "call 290",
        "mov dl, 0D",
        "int 21",
        "mov dl, 0A",
        "int 21",
        "mov al, [0A02]",
        "call 290",
        "mov dl, 0D",
        "int 21",
        "mov dl, 0A",
        "int 21",
        "ret",
        "",
        "a 290",
        "add al, 30",
        "mov dl, al",
        "mov ah, 02",
        "int 21",
        "ret",
        "",
        "",
    ]

    print("开始自动输入Debug命令...")

    for command in commands:
        command = command.lower()
        if command == "":  # 空行
            pyautogui.press("enter")
            time.sleep(0.1)
            continue

        print(f"输入: {command}")

        if command[1] == " ":
            os.system("pause")
            time.sleep(5)
        # 逐个字符输入，确保完整性
        for char in command:
            pyautogui.write(char)
            time.sleep(0.05)  # 每个字符间延迟

        # 回车执行命令
        pyautogui.press("enter")
        time.sleep(0.8)  # 命令执行延迟

    print("命令输入完成！")


if __name__ == "__main__":
    send_to_dosbox_exact()
