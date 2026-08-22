#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
多媒体技术实验五：霍夫曼编码文本压缩
功能：读取文本文件，统计字节频率，构造霍夫曼树，生成霍夫曼编码，
      将压缩后的 0/1 二进制编码串保存到文件，并输出编码表和压缩统计信息。

用法：
    python huffman_compress.py example.txt
    python huffman_compress.py example.txt -o example_huffman.txt
"""

from __future__ import annotations

import argparse
import heapq
import json
from dataclasses import dataclass, field
from pathlib import Path
from typing import Dict, Optional, Union

EOF = "<EOF>"              # 结束符，用于标记文本结束
Symbol = Union[int, str]   # 普通字节使用 int，结束符使用 str


@dataclass(order=True)
class Node:
    """霍夫曼树结点。order=True 使其可以放入优先队列。"""
    weight: int
    order: int
    symbol: Optional[Symbol] = field(default=None, compare=False)
    left: Optional["Node"] = field(default=None, compare=False)
    right: Optional["Node"] = field(default=None, compare=False)

    def is_leaf(self) -> bool:
        return self.left is None and self.right is None


def count_frequency(data: bytes) -> Dict[Symbol, int]:
    """统计输入文本中每个字节的出现次数，并额外加入结束符 EOF。"""
    freq: Dict[Symbol, int] = {}
    for b in data:
        freq[b] = freq.get(b, 0) + 1
    freq[EOF] = 1
    return freq


def build_huffman_tree(freq: Dict[Symbol, int]) -> Node:
    """根据频率表构造霍夫曼树。"""
    heap = []
    counter = 0
    for symbol, weight in freq.items():
        heapq.heappush(heap, Node(weight=weight, order=counter, symbol=symbol))
        counter += 1

    while len(heap) > 1:
        left = heapq.heappop(heap)
        right = heapq.heappop(heap)
        parent = Node(
            weight=left.weight + right.weight,
            order=counter,
            left=left,
            right=right,
        )
        counter += 1
        heapq.heappush(heap, parent)

    return heap[0]


def generate_codes(root: Node) -> Dict[Symbol, str]:
    """递归遍历霍夫曼树，生成每个符号对应的 0/1 编码。"""
    codes: Dict[Symbol, str] = {}

    def dfs(node: Node, prefix: str) -> None:
        if node.is_leaf():
            # 如果文件为空，树中只有 EOF 一个结点，此时给它编码 0
            codes[node.symbol] = prefix or "0"
            return
        if node.left is not None:
            dfs(node.left, prefix + "0")
        if node.right is not None:
            dfs(node.right, prefix + "1")

    dfs(root, "")
    return codes


def encode_data(data: bytes, codes: Dict[Symbol, str]) -> str:
    """将原始文本数据转换为霍夫曼编码串，并在末尾追加 EOF 编码。"""
    return "".join(codes[b] for b in data) + codes[EOF]


def symbol_to_display(symbol: Symbol) -> str:
    """将符号转换为适合打印/保存的形式。"""
    if symbol == EOF:
        return EOF
    if symbol == 10:
        return "\\n"
    if symbol == 13:
        return "\\r"
    if symbol == 9:
        return "\\t"
    if symbol == 32:
        return "空格"
    if isinstance(symbol, int) and 32 <= symbol <= 126:
        return chr(symbol)
    return f"0x{symbol:02X}"


def save_code_table(codes: Dict[Symbol, str], path: Path) -> None:
    """保存霍夫曼编码表为 JSON 文件。"""
    table = {symbol_to_display(k): v for k, v in sorted(codes.items(), key=lambda item: str(item[0]))}
    path.write_text(json.dumps(table, ensure_ascii=False, indent=2), encoding="utf-8")


def compress(input_path: Path, output_path: Path) -> None:
    """执行完整压缩流程：读取文件、建树、编码、保存结果、输出统计信息。"""
    data = input_path.read_bytes()
    freq = count_frequency(data)
    root = build_huffman_tree(freq)
    codes = generate_codes(root)
    bit_string = encode_data(data, codes)

    output_path.write_text(bit_string, encoding="utf-8")
    code_table_path = output_path.with_suffix(output_path.suffix + ".codes.json")
    save_code_table(codes, code_table_path)

    original_symbols = len(data) + 1  # 加上 EOF
    original_bits = original_symbols * 8
    compressed_bits = len(bit_string)
    ratio = compressed_bits / original_bits if original_bits else 0

    print("========== 霍夫曼编码压缩结果 ==========")
    print(f"输入文件：{input_path}")
    print(f"输出编码文件：{output_path}")
    print(f"编码表文件：{code_table_path}")
    print(f"原始字节数（含结束符）：{original_symbols}")
    print(f"原始按 8 bit 计算：{original_bits} bit")
    print(f"霍夫曼编码长度：{compressed_bits} bit")
    print(f"压缩后/原始比例：{ratio:.2%}")
    print("\n字符频率与编码表：")
    for symbol, code in sorted(codes.items(), key=lambda item: (len(item[1]), item[1])):
        print(f"{symbol_to_display(symbol):>6}  频率={freq[symbol]:<3}  编码={code}")
    print("\n压缩后的 0/1 编码串（前100个字符）：")
    print(bit_string[:100])


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="使用霍夫曼编码压缩文本文件")
    parser.add_argument("input", help="输入文本文件路径，例如 example.txt")
    parser.add_argument("-o", "--output", help="输出编码文件路径，默认在原文件名后加 _huffman.txt")
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    input_path = Path(args.input)
    if not input_path.exists():
        raise FileNotFoundError(f"输入文件不存在：{input_path}")

    if args.output:
        output_path = Path(args.output)
    else:
        output_path = input_path.with_name(input_path.stem + "_huffman.txt")

    compress(input_path, output_path)


if __name__ == "__main__":
    main()
