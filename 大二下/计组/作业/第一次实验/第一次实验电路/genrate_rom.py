def generate_logisim_rom(text):
    header = "v2.0 raw"
    filename = f"ROM内容——{text}.txt"
    
    encoded_bytes = text.encode('gb2312')
    hex_data = encoded_bytes.hex()
        
    formatted_hex = " ".join(hex_data[i:i+4] for i in range(0, len(hex_data), 4))
        
    with open(filename, 'w', encoding='utf-8') as f:
        f.write(header + "\n")
        f.write(formatted_hex + "\n")
            
    print(f"成功生成文件: {filename}")
    print(f"包含内容: {formatted_hex}")
        
# 设置变量
sentence = "兄弟们，原神救了我一命，原神牛逼"

# 执行
generate_logisim_rom(sentence)