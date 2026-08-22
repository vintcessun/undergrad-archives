**(0) 启动与连接**

根据数据库信息文件，首先切换用户并启动 openGauss 数据库，然后使用 `gsql` 连接：

Bash

```
su - omm
gs_om -t start
gsql -d postgres -p 26000 -h /opt/huawei/wisequery/omm_mppdb
```

------

**第一部分（题目部分）** (1)为地区表 regions 新增一条记录:('5','Oceania'). 



**第二部分（运行的SQL）**

SQL

```
INSERT INTO regions (region_id, region_name) VALUES ('5', 'Oceania');
```

**第三部分（预期结果）**

Plaintext

```
openGauss=# INSERT INTO regions (region_id, region_name) VALUES ('5', 'Oceania');
INSERT 0 1
```

**第四部分（预期结果图片的阐释）**

终端返回 `INSERT 0 1`，代表成功向数据库中插入了 1 条记录。这验证了 `INSERT INTO ... VALUES` 基础语法在不违反主键等完整性约束的情况下，能将单行数据正确写入表中。

------

**第一部分（题目部分）** (2)将 countries 表中的国家名为Austrialia的region_id改为5。 



**第二部分（运行的SQL）**

SQL

```
UPDATE countries SET region_id = '5' WHERE country_name = 'Australia';
```

**第三部分（预期结果）**

Plaintext

```
openGauss=# UPDATE countries SET region_id = '5' WHERE country_name = 'Australia';
UPDATE 1
```

**第四部分（预期结果图片的阐释）**

执行界面显示 `UPDATE 1`。说明 `UPDATE` 语句配合 `WHERE` 条件精准匹配到了“Australia”这一行（注意修正了原题中的拼写错误），并成功将其所在的区域外键 `region_id` 修改为刚建立的“5”（大洋洲），实现了行级数据的字段更新。

------

**第一部分（题目部分）** (3)使用一条批量插入数据语句为 countries 表新增5条记录:('NO', 'Norway','1'), ('ES', 'Spain', '1'), ('SE', 'Sweden','1'), ('PT', 'Portugal', '1'), ('NZ', 'New Zealand', '5'). 



**第二部分（运行的SQL）**

SQL

```
INSERT INTO countries (country_id, country_name, region_id) 
VALUES ('NO', 'Norway', '1'), 
       ('ES', 'Spain', '1'), 
       ('SE', 'Sweden', '1'), 
       ('PT', 'Portugal', '1'), 
       ('NZ', 'New Zealand', '5');
```

**第三部分（预期结果）**

Plaintext

```
openGauss=# INSERT INTO countries (country_id, country_name, region_id) VALUES ...
INSERT 0 5
```

**第四部分（预期结果图片的阐释）**

界面提示 `INSERT 0 5`，意味着 5 条记录被一次性写入。这展示了标准 SQL 支持多值插入特性，通过在 `VALUES` 关键字后追加多个括号包裹的记录值，能够有效减少数据库连接与日志解析开销，提升小批量数据的插入效率。

------

**第一部分（题目部分）** (4)创建一张名为Asia_countries (country_id,country_name)的新表,其中字段为 countries 表中的同名字段。 



**第二部分（运行的SQL）**

SQL

```
CREATE TABLE Asia_countries (
    country_id CHAR(2) PRIMARY KEY,
    country_name VARCHAR(40)
);
```

**第三部分（预期结果）**

Plaintext

```
openGauss=# CREATE TABLE Asia_countries ( ... );
CREATE TABLE
```

**第四部分（预期结果图片的阐释）**

界面返回 `CREATE TABLE`，表明 DDL（数据定义语言）执行成功。数据库在 schema 中注册了一个新的空表结构，其字段类型和长度与源表保持一致，为下一步的数据导入准备好了容器。

------

**第一部分（题目部分）** (5)将 countries 表中所有亚洲国家的数据插入到该表中。(要求使用插入子查询结果的方法实现) 



**第二部分（运行的SQL）**

*(注：设亚洲对应的 region_id 为 3)*

SQL

```
INSERT INTO Asia_countries (country_id, country_name)
SELECT country_id, country_name FROM countries WHERE region_id = '3';
```

**第三部分（预期结果）**

Plaintext

```
openGauss=# INSERT INTO Asia_countries (country_id, country_name) SELECT ...
INSERT 0 6
```

**第四部分（预期结果图片的阐释）**

结果显示成功插入了多条数据（数量取决于样例库中亚洲国家的实际行数）。这是 `INSERT INTO ... SELECT ...` 语法的典型应用，它直接在数据库内核层面将查询结果集作为数据源批量插入目标表，常用于数据备份和表间数据迁移。

------

**第一部分（题目部分）** (6)创建一张名为 order_total (order_id, total_price)的视图,该视图存放每个订单号及其总价,其中 total_price为总价,其值为数量 quantity与单价unit price 乘积之和,order id, quantity和unit_price为order_items 表中的同名字段。 



**第二部分（运行的SQL）**

SQL

```
CREATE VIEW order_total (order_id, total_price) AS
SELECT order_id, SUM(quantity * unit_price) 
FROM order_items 
GROUP BY order_id;
```

**第三部分（预期结果）**

Plaintext

```
openGauss=# CREATE VIEW order_total (order_id, total_price) AS ...
CREATE VIEW
```

**第四部分（预期结果图片的阐释）**

系统返回 `CREATE VIEW`。视图本身并不在磁盘上存储任何实体数据，它仅仅在数据字典中保存了这条包含聚合函数（`SUM`）和分组逻辑（`GROUP BY`）的 SQL 定义，属于虚拟表，方便后续简化复杂查询。

------

**第一部分（题目部分）** (7)查询 order_total 视图中订单号order_id为97的总价并记录该结果。 



**第二部分（运行的SQL）**

SQL

```
SELECT total_price FROM order_total WHERE order_id = 97;
```

**第三部分（预期结果）**

Plaintext

```
openGauss=# SELECT total_price FROM order_total WHERE order_id = 97;
 total_price 
-------------
     1500.00
(1 row)
```

**第四部分（预期结果图片的阐释）**

终端输出了订单 97 的总价。这验证了视图可以像普通物理表一样被 `SELECT` 语句查询。此时 openGauss 实际上在后台透明地执行了视图定义中的逻辑，汇总出了该订单当前的真实总价。我们需要记录这个数值（假设为 1500.00）以备后用。

------

**第一部分（题目部分）** (8)将 order_items 表中product_id为99的单价unit_price增加4元。 



**第二部分（运行的SQL）**

SQL

```
UPDATE order_items SET unit_price = unit_price + 4 WHERE product_id = 99;
```

**第三部分（预期结果）**

Plaintext

```
openGauss=# UPDATE order_items SET unit_price = unit_price + 4 WHERE product_id = 99;
UPDATE 3
```

**第四部分（预期结果图片的阐释）**

显示 `UPDATE` 以及受影响的行数。自增表达式 `unit_price + 4` 直接在底层物理基表上修改了数值，所有包含商品 99 的订单明细数据的单价已被成功刷新。

------

**第一部分（题目部分）** (9)查询视图 order_total 中订单号order_id为97的总价,将其与第(7)步的结果进行比较,观察其异同。 



**第二部分（运行的SQL）**

SQL

```
SELECT total_price FROM order_total WHERE order_id = 97;
```

**第三部分（预期结果）**

Plaintext

```
openGauss=# SELECT total_price FROM order_total WHERE order_id = 97;
 total_price 
-------------
     1520.00
(1 row)
```

**第四部分（预期结果图片的阐释）**

查询出的数值变大了。对比步骤(7)，我们只修改了底层表 `order_items` 的数据，并未修改视图结构，但查询视图的结果自动发生了变化。这直观地阐释了视图的“动态性”特征：视图的数据总是实时派生自其依赖的底层基表。

------

**第一部分（题目部分）** (10)使用 delete 命令删除 Asia_countries 表中country_id为IN的记录。 



**第二部分（运行的SQL）**

SQL

```
DELETE FROM Asia_countries WHERE country_id = 'IN';
```

**第三部分（预期结果）**

Plaintext

```
openGauss=# DELETE FROM Asia_countries WHERE country_id = 'IN';
DELETE 1
```

**第四部分（预期结果图片的阐释）**

界面提示 `DELETE 1`。说明 `DELETE` 语句通过 `WHERE` 子句实现了精确的行级删除，仅抹除了“印度（IN）”这单行数据，保留了表中的其他记录。这属于 DML 操作，会产生事务日志。

------

**第一部分（题目部分）** (11)使用 truncate 命令清空 Asia_countries表的所有记录。 



**第二部分（运行的SQL）**

SQL

```
TRUNCATE TABLE Asia_countries;
```

**第三部分（预期结果）**

Plaintext

```
openGauss=# TRUNCATE TABLE Asia_countries;
TRUNCATE TABLE
```

**第四部分（预期结果图片的阐释）**

终端返回 `TRUNCATE TABLE`。与 `DELETE` 不同，`TRUNCATE` 不逐行删除数据并记录日志，而是直接在物理层面上释放该表占用的所有数据页，操作极快。它属于 DDL 操作，无法带 `WHERE` 条件，适用于快速清空全表。

------

**第一部分（题目部分）** (12)删除 Asia_countries 表和视图 order_total. 



**第二部分（运行的SQL）**

SQL

```
DROP TABLE Asia_countries;
DROP VIEW order_total;
```

**第三部分（预期结果）**

Plaintext

```
openGauss=# DROP TABLE Asia_countries;
DROP TABLE
openGauss=# DROP VIEW order_total;
DROP VIEW
```

**第四部分（预期结果图片的阐释）**

终端先后返回 `DROP TABLE` 和 `DROP VIEW`。`DROP` 指令彻底销毁了这两个数据库对象，它们的表结构、视图定义从数据库的数据字典中被永久抹除，占用的空间和资源被系统回收。

------

**第一部分（题目部分）** (13)使用命令\d employees 查看 employees表的外码约束语句,包括on delete cascade 选项。 



**第二部分（运行的SQL）**

SQL

```
\d employees
```

**第三部分（预期结果）**

Plaintext

```
openGauss=# \d employees
               Table "public.employees"
   Column    |          Type          | Modifiers 
-------------+------------------------+-----------
 employee_id | integer                | not null
 ...
 manager_id  | integer                | 
Foreign-key constraints:
    "emp_manager_fk" FOREIGN KEY (manager_id) REFERENCES employees(employee_id) ON DELETE CASCADE
```

**第四部分（预期结果图片的阐释）**

使用 openGauss 特有的元命令 `\d` 输出了表的 Schema 描述。底部约束信息明确显示该表存在一个自引用的外键，并且配置了 `ON DELETE CASCADE` 属性。这意味着如果某位经理的记录被删除，以此经理为 `manager_id` 的所有下属员工记录也会被数据库引擎自动级联删除。

------

**第一部分（题目部分）** (14)查询 employees 表中 manager_id为1的记录。 



**第二部分（运行的SQL）**

SQL

```
SELECT employee_id, first_name, manager_id FROM employees WHERE manager_id = 1;
```

**第三部分（预期结果）**

Plaintext

```
openGauss=# SELECT employee_id, first_name, manager_id FROM employees WHERE manager_id = 1;
 employee_id | first_name | manager_id 
-------------+------------+------------
           2 | John       |          1
           3 | Alice      |          1
(2 rows)
```

**第四部分（预期结果图片的阐释）**

界面展示了直接汇报给 1 号经理的从属员工记录。这一步确认了子表（这里是自引用，相当于子记录）中确实存在依赖于 `employee_id = 1` 的数据，为后续验证外键完整性的阻断与级联机制提供了事实基准。

------

**第一部分（题目部分）** (15)修改 employees 表的外码约束,去掉外码约束中的on delete cascade 选项,但保留原有的外码引用,即 manager_id引用本表上的 employee_id。(可通过先删后建实现) 



**第二部分（运行的SQL）**

SQL

```
ALTER TABLE employees DROP CONSTRAINT emp_manager_fk;
ALTER TABLE employees ADD CONSTRAINT emp_manager_fk FOREIGN KEY (manager_id) REFERENCES employees(employee_id);
```

**第三部分（预期结果）**

Plaintext

```
openGauss=# ALTER TABLE employees DROP CONSTRAINT emp_manager_fk;
ALTER TABLE
openGauss=# ALTER TABLE employees ADD CONSTRAINT emp_manager_fk FOREIGN KEY (manager_id) REFERENCES employees(employee_id);
ALTER TABLE
```

**第四部分（预期结果图片的阐释）**

终端连续两次提示 `ALTER TABLE` 成功。因为关系型数据库通常不支持直接“修改”外键属性，所以采用先 `DROP CONSTRAINT` 移除旧约束，再 `ADD CONSTRAINT` 重建不带 `CASCADE` 选项的新约束的方式。此时外键回退到了默认的严格模式（`NO ACTION` 或 `RESTRICT`）。

------

**第一部分（题目部分）** (16)删除 employees 表中 employee_id为1的记录,观察操作结果。 



**第二部分（运行的SQL）**

SQL

```
DELETE FROM employees WHERE employee_id = 1;
```

**第三部分（预期结果）**

Plaintext

```
openGauss=# DELETE FROM employees WHERE employee_id = 1;
ERROR:  update or delete on table "employees" violates foreign key constraint "emp_manager_fk" on table "employees"
DETAIL:  Key (employee_id)=(1) is still referenced from table "employees".
```

**第四部分（预期结果图片的阐释）**

界面抛出了红色的 `ERROR` 异常信息。由于在第(15)步移除了 `ON DELETE CASCADE` 级联删除选项，当试图删除 1 号员工时，数据库引擎发现表中还有其他记录（如步骤14查出的员工2和3）的 `manager_id` 依赖于该员工。为了维护参照完整性，数据库拒绝了该删除操作。

------

**第一部分（题目部分）** (17)修改 employees 表的外码约束,增加on delete cascade 选项,即回到最初的外码约束状态。 



**第二部分（运行的SQL）**

SQL

```
ALTER TABLE employees DROP CONSTRAINT emp_manager_fk;
ALTER TABLE employees ADD CONSTRAINT emp_manager_fk FOREIGN KEY (manager_id) REFERENCES employees(employee_id) ON DELETE CASCADE;
```

**第三部分（预期结果）**

Plaintext

```
openGauss=# ALTER TABLE employees DROP CONSTRAINT emp_manager_fk;
ALTER TABLE
openGauss=# ALTER TABLE employees ADD CONSTRAINT emp_manager_fk FOREIGN KEY (manager_id) REFERENCES employees(employee_id) ON DELETE CASCADE;
ALTER TABLE
```

**第四部分（预期结果图片的阐释）**

再次通过先删后建的方式成功执行 `ALTER TABLE`。这为 `employees` 表重新加上了带有级联删除（`ON DELETE CASCADE`）特性的自引用外键约束，恢复了实验初始的约束状态。

------

**第一部分（题目部分）** (18)再次执行第(16)步,观察操作结果。 



**第二部分（运行的SQL）**

SQL

```
DELETE FROM employees WHERE employee_id = 1;
```

**第三部分（预期结果）**

Plaintext

```
openGauss=# DELETE FROM employees WHERE employee_id = 1;
DELETE 1
```

**第四部分（预期结果图片的阐释）**

这次界面没有报错，而是成功提示 `DELETE 1`。因为加上了 `ON DELETE CASCADE` 选项，数据库在删除主记录（employee_id = 1）时，会自动静默地扫描并删除所有依赖该记录的子记录（manager_id = 1 的所有员工）。这体现了外键级联选项在处理关联数据销毁时的强大自动化能力。