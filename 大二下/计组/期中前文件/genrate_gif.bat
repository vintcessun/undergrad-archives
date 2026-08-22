@echo off
set "input_file=%~1"
set "output_file=%~dpn1.gif"

echo 正在处理: %input_file%
echo 输出位置: %output_file%

:: 执行ffmpeg转换命令
ffmpeg -i "%input_file%" -vf "fps=15,scale=480:-1:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse" "%output_file%"

echo.
echo 转换完成！按任意键退出。
pause