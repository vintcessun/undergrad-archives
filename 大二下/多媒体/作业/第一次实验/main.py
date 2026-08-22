data = [1, 2, 3, 12, 20, 100, 6, 9, 12, 8, 8, 3]

def get_average(data_list):
    total = 0
    for e in data_list:
        total += e
    avg = total / len(data_list)
    print(f"平均值为: {avg}")

def get_max(data_list):
    M = data_list[0]
    for e in data_list:
        M = max(M, e)
    print(f"最大值为: {M}")

def bubble_sort(data_list):
    n = len(data_list)
    for i in range(n):
        for j in range(0, n - i - 1):
            if data_list[j] > data_list[j + 1]:
                data_list[j], data_list[j + 1] = data_list[j + 1], data_list[j]
    print(f"排序后结果: {data_list}")

def insert_x(data_list, pos: int, d: int):
    data_list.insert(pos, d)
    print(f"修改后结果: {data_list}")

def del_x(data_list, pos: int):
    data_list.pop(pos)
    print(f"修改后结果: {data_list}")

get_average(data)
get_max(data)

src_data = data.copy()
bubble_sort(data)

data = src_data.copy()
insert_x(data, 3, 1)

data = src_data.copy()
del_x(data, 2)
