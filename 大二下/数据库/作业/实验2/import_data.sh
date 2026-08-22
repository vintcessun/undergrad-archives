#!/bin/bash

# --- 配置信息 ---
DB_NAME="sales"
DB_USER="omm"  # 或者你创建的拼音用户名
DB_PORT="26000"
DATA_DIR="/home/omm/lab1/data"  # 替换为你CSV文件所在的实际绝对路径
SCHEMA="sales"

# --- 待导入的文件列表 (注意 order_items 的文件名纠错) ---
declare -A TABLES
TABLES=(
    ["regions"]="regions.csv"
    ["countries"]="countries.csv"
    ["locations"]="locations.csv"
    ["warehouses"]="warehouses.csv"
    ["employees"]="employees.csv"
    ["product_categories"]="product_categories.csv"
    ["products"]="products.csv"
    ["customers"]="customers.csv"
    ["contacts"]="contacts.csv"
    ["orders"]="orders.csv"
    ["order_items"]="order_itmes.csv" # 匹配你图中拼写错误的 order_itmes.csv
    ["inventories"]="inventories.csv"
)

echo "开始导入数据到数据库 $DB_NAME..."

# --- 执行导入 ---
for table in "${!TABLES[@]}"; do
    csv_file="${TABLES[$table]}"
    echo "正在导入表: $SCHEMA.$table 来自文件: $csv_file"
    
    # 使用 gsql 执行 COPY 命令
    # CSV 格式说明: HEADER 表示第一行是列名，DELIMITER ',' 表示逗号分隔
    # 修改后的 gsql 执行行
    gsql -d postgres -p 26000 -h /opt/huawei/wisequery/omm_mppdb -c \
    "SET datestyle TO 'SQL, DMY'; COPY $SCHEMA.$table FROM '$DATA_DIR/$csv_file' WITH (FORMAT 'csv', HEADER 'true', DELIMITER ',');"
done

echo "所有数据导入尝试完成！"
