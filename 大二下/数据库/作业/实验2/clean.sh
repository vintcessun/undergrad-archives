cd /home/omm/lab1/data

# 备份原始数据
mkdir -p backup
cp *.csv backup/

# 批量替换：将“1月”到“12月”替换为数字，并处理掉多余空格
# 针对 employees.csv 和 orders.csv 特别清洗
for f in employees.csv orders.csv; do
    sed -i 's/1月/01/g' $f
    sed -i 's/2月/02/g' $f
    sed -i 's/3月/03/g' $f
    sed -i 's/4月/04/g' $f
    sed -i 's/5月/05/g' $f
    sed -i 's/6月/06/g' $f
    sed -i 's/7月/07/g' $f
    sed -i 's/8月/08/g' $f
    sed -i 's/9月/09/g' $f
    sed -i 's/10月/10/g' $f
    sed -i 's/11月/11/g' $f
    sed -i 's/12月/12/g' $f
    sed -i 's/ //g' $f  # 去掉所有空格（解决“6月 ”问题）
done