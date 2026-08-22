from collections import Counter
import json
import sys


def build_probability_table(text):
    freq = Counter(text)

    total = sum(freq.values())

    prob_table = {}

    low = 0.0

    for ch, count in sorted(freq.items()):
        high = low + count / total

        prob_table[ch] = {
            "prob": count / total,
            "low": low,
            "high": high
        }

        low = high

    return prob_table


def arithmetic_encode(text, prob_table):
    low = 0.0
    high = 1.0

    for ch in text:
        range_width = high - low

        symbol_low = prob_table[ch]["low"]
        symbol_high = prob_table[ch]["high"]

        new_low = low + range_width * symbol_low
        new_high = low + range_width * symbol_high

        low = new_low
        high = new_high

    code = (low + high) / 2

    return code, low, high


def save_probability_table(prob_table, filename):
    with open(filename, "w", encoding="utf-8") as f:
        json.dump(prob_table, f, ensure_ascii=False, indent=4)


def main():
    input_file = sys.argv[1]

    with open(input_file, "r", encoding="utf-8") as f:
        text = f.read()

    print("原始文本内容：")
    print(text)

    prob_table = build_probability_table(text)

    print("\n字符概率区间：")

    for ch, info in prob_table.items():
        print(
            f"{repr(ch)} -> "
            f"概率={info['prob']:.6f}, "
            f"区间=[{info['low']:.6f}, {info['high']:.6f})"
        )

    code, low, high = arithmetic_encode(text, prob_table)

    print("\n算术编码结果：")
    print(code)

    print("\n最终编码区间：")
    print(f"[{low}, {high})")

    original_bits = len(text) * 8

    compressed_bits = 64

    ratio = compressed_bits / original_bits * 100

    print("\n压缩统计：")
    print(f"原始大小：{original_bits} bit")
    print(f"压缩后大小：{compressed_bits} bit")
    print(f"压缩率：{ratio:.2f}%")

    input_name = input_file.rsplit(".", 1)[0]
    save_probability_table(prob_table, f"prob_table_{input_name}.json")

    with open(f"encoded_result_{input_file}", "w", encoding="utf-8") as f:
        f.write(str(code))

    print(f"\n概率区间表已保存为 prob_table_{input_name}.json")
    print(f"编码结果已保存为 encoded_result_{input_file}")


if __name__ == "__main__":
    main()