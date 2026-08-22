import matplotlib.pyplot as plt
from collections import Counter

def analyze_text():
    with open('alphatwice.txt', 'r', encoding='utf-8') as f:
        content = f.read()

    letters = [char for char in content if char.isalpha()]
    counts = Counter(letters)

    top_10 = counts.most_common(10)
    
    labels, values = zip(*top_10) if top_10 else ([], [])

    plt.figure(figsize=(10, 6))
    plt.bar(labels, values, color='skyblue', edgecolor='navy')
    
    plt.title('Top 10 English Letters Frequency (Case Sensitive)')
    plt.xlabel('Letters')
    plt.ylabel('Appearance Counts')
    
    plt.savefig('letter_histogram.png')
    print("直方图已保存为: letter_histogram.png")
    plt.show()

if __name__ == "__main__":
    analyze_text()