import cv2
import dlib
import sys

def detect_face_opencv():
    """
    任务一：利用 OpenCV 实时捕获视频并进行人脸矩形框检测
    要求：实时读取视频流，使用 opencv 检测人脸并画矩形框，最后实时显示。
    """
    print(">>> 正在启动 OpenCV 人脸检测... (按 'q' 键退出窗口)")
    
    # 1. 加载级联分类器文件
    try:
        face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
        if face_cascade.empty():
            raise IOError("无法加载 haarcascade_frontalface_default.xml，请检查文件路径！")
    except Exception as e:
        print(f"错误: {e}")
        return

    # 2. 打开默认摄像头
    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("错误: 无法打开摄像头！")
        return
        
    while True:
        ret, frame = cap.read()
        if not ret:
            print("无法获取画面流，正在退出...")
            break
            
        # 转为灰度图，提高检测速度和准确率
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        
        # 进行人脸检测
        faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))
        
        # 3. 遍历检测到的人脸并画绿色矩形框
        for (x, y, w, h) in faces:
            cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)
            
        # 实时显示在 opencv 窗口中
        cv2.imshow('2.1 OpenCV Face Detection', frame)
        
        # 按 'q' 键退出循环
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
            
    # 释放资源并销毁窗口
    cap.release()
    cv2.destroyAllWindows()


def detect_landmarks_dlib():
    """
    任务二：利用 dlib 检测视频流中的 68 个人脸特征点
    要求：实时读取视频流，使用 dlib 检测特征点并画在图像帧上，最后实时显示。
    """
    print(">>> 正在启动 dlib 68 个人脸特征点检测... (按 'q' 键退出窗口)")
    print(">>> 提示：dlib 加载模型可能需要几秒钟，请稍候...")
    
    # 1. 初始化 dlib 的人脸检测器和特征点预测器
    try:
        detector = dlib.get_frontal_face_detector()
        predictor = dlib.shape_predictor('shape_predictor_68_face_landmarks.dat')
    except RuntimeError as e:
        print(f"错误: 无法加载 shape_predictor_68_face_landmarks.dat，请确认文件是否存在！\n详细信息: {e}")
        return

    # 2. 打开默认摄像头
    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("错误: 无法打开摄像头！")
        return
        
    while True:
        ret, frame = cap.read()
        if not ret:
            print("无法获取画面流，正在退出...")
            break
            
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        
        # 使用 dlib 检测人脸位置
        faces = detector(gray, 0)
        
        # 3. 对每个检测到的人脸提取 68 个特征点
        for face in faces:
            landmarks = predictor(gray, face)
            
            # 遍历 68 个点，并在图像上绘制实心圆
            for n in range(0, 68):
                x = landmarks.part(n).x
                y = landmarks.part(n).y
                cv2.circle(frame, (x, y), 2, (0, 255, 0), -1)
                
        # 实时显示在 opencv 窗口中
        cv2.imshow('2.2 Dlib 68 Face Landmarks', frame)
        
        # 按 'q' 键退出循环
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
            
    # 释放资源并销毁窗口
    cap.release()
    cv2.destroyAllWindows()


def main():
    """主程序入口，提供简单的控制台运行界面"""
    while True:
        print("\n" + "="*45)
        print("   多媒体实验四：视频捕获与视频处理交互界面")
        print("="*45)
        print("  [1] 运行 2.1 OpenCV 实时视频人脸检测")
        print("  [2] 运行 2.2 dlib 68 个人脸特征点检测")
        print("  [q] 退出程序")
        print("="*45)
        
        choice = input("请输入对应的序号执行任务: ").strip().lower()
        
        if choice == '1':
            detect_face_opencv()
        elif choice == '2':
            detect_landmarks_dlib()
        elif choice == 'q':
            print("已退出程序。")
            sys.exit(0)
        else:
            print("无效的输入，请输入 1、2 或 q。")

if __name__ == "__main__":
    main()