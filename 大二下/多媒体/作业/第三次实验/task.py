import os

class BMPImage:
    def __init__(self):
        self.width = 0
        self.height = 0
        self.bit_count = 0
        self.palette = []  # 用于存放 (B, G, R) 元组的调色板
        self.pixel_data = [] # 存放二维像素数据矩阵

    def read(self, filepath):
        """解析 BMP 位图文件底层二进制数据"""
        with open(filepath, 'rb') as f:
            # --- 1. 解析文件头 (14 Bytes) ---
            bfType = f.read(2)
            if bfType != b'BM':
                raise ValueError("读取的不是标准的BMP文件")
            f.read(8) # 跳过 bfSize (4字节) 和两个保留字段 (各2字节)
            bfOffBits = int.from_bytes(f.read(4), 'little')

            # --- 2. 解析信息头 (40 Bytes) ---
            biSize = int.from_bytes(f.read(4), 'little')
            self.width = int.from_bytes(f.read(4), 'little', signed=True)
            self.height = int.from_bytes(f.read(4), 'little', signed=True)
            f.read(2) # 跳过 biPlanes
            self.bit_count = int.from_bytes(f.read(2), 'little')
            f.seek(14 + 40) # 将指针移动到信息头末尾

            # --- 3. 解析调色板 (仅8位伪彩色图拥有) ---
            if self.bit_count == 8:
                self.palette = []
                for _ in range(256):
                    b = int.from_bytes(f.read(1), 'little')
                    g = int.from_bytes(f.read(1), 'little')
                    r = int.from_bytes(f.read(1), 'little')
                    f.read(1) # 跳过 1 字节的保留位
                    self.palette.append((b, g, r))

            # --- 4. 解析像素数据区 ---
            f.seek(bfOffBits)
            # BMP要求图像每一行的字节数必须是4的倍数，计算对齐后的行字节数
            row_padded = (self.width * self.bit_count + 31) // 32 * 4
            
            self.pixel_data = []
            for _ in range(abs(self.height)):
                row_data = f.read(row_padded)
                row_pixels = []
                if self.bit_count == 24:
                    # 24位图直接读取 B, G, R
                    for x in range(self.width):
                        b, g, r = row_data[x*3], row_data[x*3+1], row_data[x*3+2]
                        row_pixels.append((b, g, r))
                elif self.bit_count == 8:
                    # 8位图读取的是调色板索引
                    for x in range(self.width):
                        row_pixels.append(row_data[x]) 
                self.pixel_data.append(row_pixels)

    def write(self, filepath):
        """将对象内存中的数据重新编码并保存为 BMP 文件"""
        with open(filepath, 'wb') as f:
            row_padded = (self.width * self.bit_count + 31) // 32 * 4
            image_size = row_padded * abs(self.height)
            palette_size = 1024 if self.bit_count == 8 else 0
            offset = 14 + 40 + palette_size
            file_size = offset + image_size

            # 写入文件头
            f.write(b'BM')
            f.write(file_size.to_bytes(4, 'little'))
            f.write((0).to_bytes(4, 'little'))
            f.write(offset.to_bytes(4, 'little'))

            # 写入信息头
            f.write((40).to_bytes(4, 'little'))
            f.write(self.width.to_bytes(4, 'little', signed=True))
            f.write(self.height.to_bytes(4, 'little', signed=True))
            f.write((1).to_bytes(2, 'little'))
            f.write(self.bit_count.to_bytes(2, 'little'))
            f.write((0).to_bytes(4, 'little')) # biCompression
            f.write(image_size.to_bytes(4, 'little'))
            f.write((0).to_bytes(8, 'little')) # X/Y PelsPerMeter
            f.write((256 if self.bit_count == 8 else 0).to_bytes(4, 'little')) # biClrUsed
            f.write((0).to_bytes(4, 'little')) # biClrImportant

            # 写入调色板 (如果是8位图)
            if self.bit_count == 8:
                for color in self.palette:
                    f.write(color[0].to_bytes(1, 'little')) # B
                    f.write(color[1].to_bytes(1, 'little')) # G
                    f.write(color[2].to_bytes(1, 'little')) # R
                    f.write((0).to_bytes(1, 'little'))      # Reserved

            # 写入像素数据并补齐4字节对齐的空位
            for row in self.pixel_data:
                row_bytes = bytearray()
                if self.bit_count == 24:
                    for b, g, r in row:
                        row_bytes.extend([b, g, r])
                elif self.bit_count == 8:
                    row_bytes.extend(row)
                
                padding_len = row_padded - len(row_bytes)
                row_bytes.extend(b'\x00' * padding_len)
                f.write(row_bytes)

    def convert_24_to_8_gray(self):
        """任务一：读取24位真彩色，转化为灰色图像，存储为8位伪彩色"""
        if self.bit_count != 24: 
            return
        
        # 1. 创建包含256个灰阶的调色板 (R=G=B=索引值)
        self.palette = [(i, i, i) for i in range(256)]
        
        # 2. 遍历像素利用心理学公式计算灰度，并将其作为索引
        new_pixel_data = []
        for row in self.pixel_data:
            new_row = []
            for b, g, r in row:
                gray = int(0.299 * r + 0.587 * g + 0.114 * b)
                new_row.append(gray) # 此处的灰度值恰好等于我们在调色板里创建的索引
            new_pixel_data.append(new_row)
        
        self.pixel_data = new_pixel_data
        self.bit_count = 8 # 修改位深标识

    def convert_8_to_24(self):
        """任务二：读取8位伪彩色，转化为24位真彩色"""
        if self.bit_count != 8: 
            return
        
        # 1. 遍历原有的索引像素矩阵，通过查调色板还原真实的 B, G, R 色彩值
        new_pixel_data = []
        for row in self.pixel_data:
            new_row = []
            for idx in row:
                new_row.append(self.palette[idx]) 
            new_pixel_data.append(new_row)
        
        # 2. 重新赋值像素区，清除不再需要的调色板，并修改位深标识
        self.pixel_data = new_pixel_data
        self.bit_count = 24
        self.palette = [] 

def task1(path:str):
    """任务一：将24位真彩色BMP图像转换为8位灰度图像，并保存为新的BMP文件"""
    task1_img = BMPImage()
    task1_img.read(path)
    task1_img.convert_24_to_8_gray()
    task1_img.write(path.split('.')[0] + "_8bit_gray.bmp")

def task2(path:str):
    """任务二：将8位伪彩色BMP图像还原为24位真彩色图像，并保存为新的BMP文件"""
    task2_img = BMPImage()
    task2_img.read(path)
    task2_img.convert_8_to_24()
    task2_img.write(path.split('.')[0] + "_24bit_color.bmp")

if __name__ == "__main__":
    # --- 执行任务一 ---
    for e in os.listdir("24位真彩色BMP"):
        if e.endswith(".bmp"):
            task1(os.path.join("24位真彩色BMP", e))
    print("任务一完成：24位真彩色已成功转换为8位灰度图。")

    # --- 执行任务二 ---
    for e in os.listdir("8位伪彩色BMP"):
        if e.endswith(".bmp"):
            task2(os.path.join("8位伪彩色BMP", e))
    print("任务二完成：8位伪彩色已成功还原为24位真彩色图。")