import sounddevice as sd
import soundfile as sf
import numpy as np

def process_audio():
    filename = 'music.wav'
    
    data, fs = sf.read(filename)
    print(f"音频采样率: {fs}, 数据形状: {data.shape}")

    samples_per_sec = fs
    total_secs = int(len(data) / samples_per_sec)
        
    keep_segments = []
        
    for i in range(total_secs):
        if i % 2 == 0:
            start = i * samples_per_sec
            end = (i + 1) * samples_per_sec
            keep_segments.append(data[start:end])

    new_data = np.concatenate(keep_segments, axis=0)

    output_name = 'music_edited.wav'
    sf.write(output_name, new_data, fs)
    print(f"处理完成！新音频已保存为: {output_name}")
        
    print("正在播放处理后的音频...")
    sd.play(new_data, fs)
    sd.wait()

if __name__ == "__main__":
    process_audio()