### 1. 登录与环境准备 (ECS 指令)

首先，你需要通过 SSH 登录到华为 ECS 服务器，然后切换到 `omm` 用户并进入数据库环境。

Bash

```
# 1. 如果你当前是 root 用户，切换到 omm 操作系统管理员
su - omm
# 提示输入密码时，输入: OpenGauss@omm

# 2. 使用你提供的 gsql 命令连接到默认的 postgres 数据库
gsql -d postgres -p 26000 -h /opt/huawei/wisequery/omm_mppdb
# 提示输入密码时，输入: GaussDB@123
```

### 第一步：检查数据库当前状态

首先确认数据库集群或单节点的状态。在 `omm` 用户终端中输入：

Bash

```
gs_om -t status
```

- 如果显示 `cluster state: Normal`，说明数据库是正常的（但通常你遇到那个报错时，状态多半是 `Unavailable` 或者 `Stopped`）。

- **备用命令：** 如果提示找不到 `gs_om` 命令，可以直接查看系统进程，看看 `gaussdb` 是否存在：

  Bash

  ```
  ps -ef | grep gaussdb
  ```

  如果没有一大串带有 `/opt/.../bin/gaussdb` 的进程，就确实验证了数据库没启动。

### 第二步：启动 openGauss 数据库

既然没启动，我们就需要手动拉起它。执行以下命令：

Bash

```
gs_om -t start
```

*启动过程可能需要几十秒钟。* 当你看到类似 `Starting cluster... Successfully started.` 的提示时，说明数据库已经成功运行。

**(补充说明)：** 如果你的实验环境比较特殊，没有使用集群管理工具 `gs_om`，你可能需要使用 `gs_ctl` 来启动。这需要知道你的数据目录（Data Directory），命令通常长这样（注意替换你的实际数据目录）：

Bash

```
gs_ctl start -D /opt/huawei/install/data/dn
```

### 第三步：确认端口监听正常

数据库启动后，验证一下它是不是乖乖地在 26000 端口上监听：

Bash

```
netstat -nlp | grep 26000
```

如果你能看到类似 `tcp 0 0 0.0.0.0:26000 ... LISTEN` 以及相关的 `unix` 套接字信息，说明环境已经完全就绪。

### 第四步：重新连接数据库

现在你可以再次尝试连接了。

你之前使用的命令：

Bash

```
gsql -d postgres -p 26000 -h /opt/huawei/wisequery/omm_mppdb
```

进入数据库后，执行以下命令建立实验要求的 `SALES` 数据库并切换进去：

SQL

```
-- 创建 SALES 数据库
CREATE DATABASE SALES;

-- 切换到 SALES 数据库
\c SALES;
```

------

### 2. 构建数据表结构 (DDL 指令)

根据实验三的查询要求，我们需要建立顾客、订单、订单细节、地址、产品、雇员和联系人表（第20题的 `discounts` 表实验要求你手动创建，故此处不包含）。

SQL

```
-- 创建顾客表 (customers)
CREATE TABLE customers (
    customer_id INT PRIMARY KEY,
    name VARCHAR(100),
    credit_limit DECIMAL(10, 2)
);

-- 创建订单表 (orders)
CREATE TABLE orders (
    order_id INT PRIMARY KEY,
    customer_id INT,
    status VARCHAR(50),
    order_date DATE,
    salesman_id INT -- 用于测试“没有销售员负责的订单”
);

-- 创建订单细节表 (order_items)
CREATE TABLE order_items (
    order_id INT,
    product_id INT,
    quantity INT,
    unit_price DECIMAL(10,2),
    PRIMARY KEY (order_id, product_id)
);

-- 创建地址表 (locations)
CREATE TABLE locations (
    country_id VARCHAR(10),
    city VARCHAR(50),
    state VARCHAR(50) -- 允许 NULL 以测试排序
);

-- 创建产品表 (products)
CREATE TABLE products (
    product_id INT PRIMARY KEY,
    product_name VARCHAR(100),
    description VARCHAR(255),
    list_price DECIMAL(10,2),
    standard_cost DECIMAL(10,2),
    category_id INT
);

-- 创建雇员表 (employees)
CREATE TABLE employees (
    employee_id INT PRIMARY KEY,
    first_name VARCHAR(50),
    last_name VARCHAR(50),
    manager_id INT -- 用于测试自身连接
);

-- 创建联系人表 (contacts)
CREATE TABLE contacts (
    contact_id INT PRIMARY KEY,
    first_name VARCHAR(50),
    last_name VARCHAR(50),
    phone VARCHAR(20),
    email VARCHAR(100)
);
```

------

### 3. 插入模拟测试数据 (DML 指令)

以下数据经过特殊设计，完全覆盖了实验三中例如 `Kingston`、以 `Asus` 开头、满足 `Je_i` 匹配、`state` 为 `NULL` 以及总金额大于 1,000,000 等查询要求。

SQL

```
-- 1. 插入顾客表数据 (插入21条以验证 LIMIT 20 限制)
INSERT INTO customers VALUES 
(1, 'Alice', 5000.00), (2, 'Bob', 10000.00), (3, 'Charlie', 15000.00),
(4, 'David', 8000.00), (5, 'Eve', 12000.00), (6, 'Frank', 3000.00),
(7, 'Grace', 20000.00), (8, 'Heidi', 6000.00), (9, 'Ivan', 7000.00),
(10, 'Judy', 9000.00), (11, 'Mallory', 11000.00), (12, 'Niaj', 13000.00),
(13, 'Olivia', 14000.00), (14, 'Peggy', 16000.00), (15, 'Rupert', 17000.00),
(16, 'Sybil', 18000.00), (17, 'Trent', 19000.00), (18, 'Victor', 21000.00),
(19, 'Walter', 22000.00), (20, 'Xavier', 25000.00), (21, 'Yvonne', 30000.00);

-- 2. 插入订单表数据 (包含 NULL 销售员记录)
INSERT INTO orders VALUES 
(101, 1, 'Shipped', '2023-10-01', 1),
(102, 2, 'Pending', '2023-10-05', NULL), -- 没有销售员负责
(103, 1, 'Delivered', '2023-10-10', 2),
(104, 3, 'Pending', '2023-10-12', NULL);

-- 3. 插入订单细节数据 (包含总价大于 1,000,000 的记录以及重复项测试)
INSERT INTO order_items VALUES 
(101, 201, 10, 50.00),
(101, 202, 5, 250000.00), -- 5 * 250000 = 1,250,000 (总金额>1000000)
(102, 203, 1, 660.00),
(103, 201, 10, 50.00);   -- 用于测试去重 (DISTINCT)

-- 4. 插入地址表数据 (包含 NULL state 用于验证 NULLS FIRST/LAST)
INSERT INTO locations VALUES 
('US', 'New York', 'NY'),
('CN', 'Beijing', 'Beijing'),
('JP', 'Tokyo', NULL),       -- state 为空
('FR', 'Paris', NULL);       -- state 为空

-- 5. 插入产品表数据 (覆盖价格、特定命名、毛利计算)
INSERT INTO products VALUES 
(201, 'Kingston USB Drive', '32GB Flash Drive', 50.00, 20.00, 1),
(202, 'Asus ROG Laptop', 'High performance gaming laptop', 2000.00, 1500.00, 4),
(203, 'Generic Monitor', '24 inch display', 660.00, 400.00, 4),  -- 价格在 650-680 之间，category为4
(204, 'Asus Router', 'WiFi 6 Router', 550.00, 300.00, 4),        -- 价格>500，category为4，以Asus开头
(205, 'Kingston RAM', '16GB DDR4', 670.00, 350.00, 2);           -- 价格在 650-680 之间

-- 6. 插入雇员表数据 (包含经理-员工层级结构)
INSERT INTO employees VALUES 
(1, 'John', 'Doe', NULL),     -- 顶级经理
(2, 'Jane', 'Smith', 1),      -- Jane的经理是John
(3, 'Alan', 'Turing', 1),     -- Alan的经理是John
(4, 'Ada', 'Lovelace', 2);    -- Ada的经理是Jane

-- 7. 插入联系人表数据 (覆盖正则及字符匹配)
INSERT INTO contacts VALUES 
(1, 'Jedi', 'Knight', '13800138000', 'jedi@example.com'),       -- 长度4，Je开头，i结尾，不是+1开头
(2, 'Jerry', 'Mouse', '+1-800-555-1234', 'jerry@example.com'),   -- Je开头，>=3个字符，是+1开头
(3, 'Jean', 'Valjean', '13900139000', 'jean@example.com'),      -- Je开头，不是+1开头
(4, 'Tom', 'Cat', '13700137000', 'tom@example.com');             -- 干扰项
```

执行完毕后，你的环境就已经完全准备就绪。可以直接对照实验三的要求开始编写并执行查询语句了。

### (1) 查询顾客表中的顾客号、顾客名和信用卡额度

**SQL 查询语句：**

SQL

```
SELECT customer_id, name, credit_limit FROM customers;
```

### (2)查询顾客的所有信息，且只显示前 20 条记录

**SQL 查询语句：**

SQL

```
SELECT * FROM customers LIMIT 20;
```

### （3）查询订单表中的订单号，顾客号，状态，订单日期，并按订单日期降序显示结果

**SQL 查询语句：**

SQL

```
SELECT order_id, customer_id, status, order_date FROM orders ORDER BY order_date DESC;
```

### （4）查询联系表中的名（first name）和姓（last name），并按名升序，姓降序显示 

**SQL 查询语句：**

SQL

```
SELECT first_name, last_name 
FROM contacts 
ORDER BY first_name ASC, last_name DESC;
```

### （5）执行以下语句并观察 state 列 NULL 值的显示位置，得出结论。

SELECT country_id, city,state FROM locations ORDER BY city,state; 

SELECT country_id, city,state FROM locations ORDER BY state ASC NULLS FIRST ; 

SELECT country_id, city,state FROM locations ORDER BY state ASC NULLS LAST;

**SQL 查询语句与执行：**

SQL

```
-- 语句 1：默认排序
SELECT country_id, city, state FROM locations ORDER BY city, state; 

-- 语句 2：指定空值在前
SELECT country_id, city, state FROM locations ORDER BY state ASC NULLS FIRST; 

-- 语句 3：指定空值在后
SELECT country_id, city, state FROM locations ORDER BY state ASC NULLS LAST;
```

### （6）查询订单细节表中（order_items）的产品号和数量，查询结果应无重复元组。

**SQL 查询语句：**

SQL

```
SELECT DISTINCT product_id, quantity FROM order_items;
```

### （7）查询产品表中的产品名为‘Kingston’的产品名，产品描述和价格。

**SQL 查询语句：**

SQL

```
SELECT product_name, description, list_price 
FROM products 
WHERE product_name = 'Kingston';
```

### （8）查询产品表中所有价格大于 500 且 category_id 为 4 的产品名和价格 

**SQL 查询语句：**

SQL

```
SELECT product_name, list_price 
FROM products 
WHERE list_price > 500 AND category_id = 4;
```

### （9）查询产品表中所有价格在 650 和 680 之间的产品名和价格并按价格升序显示结果 

**SQL 查询语句：**

SQL

```
SELECT product_name, list_price 
FROM products 
WHERE list_price BETWEEN 650 AND 680 
ORDER BY list_price ASC;
```

### （10）查询雇员表中的名和姓，名和姓的字段分别显示为"First Name"和"Family Name"。

**SQL 查询语句：**

SQL

```
SELECT first_name AS "First Name", last_name AS "Family Name" 
FROM employees;
```

### （11）查询产品表中的产品名及毛利，并按毛利结果降序显示，毛利名为 gross_profit，毛利=list_price - standard_cost。

**SQL 查询语句：**

SQL

```
SELECT product_name, (list_price - standard_cost) AS gross_profit 
FROM products 
ORDER BY gross_profit DESC;
```

### （12）查询雇员表中每个雇员对应的经理名

**SQL 查询语句：**

SQL

```
SELECT e.first_name || ', ' || e.last_name AS employee_name, 
       m.first_name || ', ' || m.last_name AS manager_name
FROM employees e 
JOIN employees m ON e.manager_id = m.employee_id;
```

### （13）查询产品表中所有以 Asus 开头的产品名和价格，并以价格降序显示

**SQL 查询语句：**

SQL

```
SELECT product_name, list_price 
FROM products 
WHERE product_name LIKE 'Asus%' 
ORDER BY list_price DESC;
```

### （14）查询联系表中电话号码不是以 ‘+1’ 开头的名、姓和电话号码，并以名升序显示

**SQL 查询语句：**

SQL

```
SELECT first_name, last_name, phone 
FROM contacts 
WHERE phone NOT LIKE '+1%' 
ORDER BY first_name ASC;
```

### （15）查询联系表中的电话号码和电子邮件，要求名(first_name) 的长度为 4 且以 'Je' 开头，以 'i' 结尾，按名升序显示

**SQL 查询语句：**

SQL

```
SELECT phone, email 
FROM contacts 
WHERE first_name LIKE 'Je_i' 
ORDER BY first_name ASC;
```

### （16）查询联系表中所有以开头 'Je' 的名，且至少包含 3 个字符的名，姓，电子邮件和电话

**SQL 查询语句：**

SQL

```
SELECT first_name, last_name, email, phone 
FROM contacts 
WHERE first_name LIKE 'Je_%';
```

### （17）查询订单表中所有没有销售员负责的订单

**SQL 查询语句：**

SQL

```
SELECT * FROM orders 
WHERE salesman_id IS NULL;
```

### （18）统计每个顾客的订单总数（查询订单表）

**SQL 查询语句：**

SQL

```
SELECT customer_id, COUNT(*) AS total_orders 
FROM orders 
GROUP BY customer_id;
```

### （19）统计每个订单的总价格大于 1000000 的订单号和总价格，并按总价格降序显示结果

**SQL 查询语句：**

SQL

```
SELECT order_id, SUM(unit_price * quantity) AS total_price 
FROM order_items 
GROUP BY order_id 
HAVING SUM(unit_price * quantity) > 1000000 
ORDER BY total_price DESC;
```

### （20）创建一个折扣表 discounts

**SQL 查询语句：**

SQL

```
CREATE TABLE discounts (
    product_id NUMBER, 
    discount_message VARCHAR2(255) NOT NULL, 
    PRIMARY KEY(product_id)
);
```

### （21）插入 3 条数据并查询折扣信息出现“25%”的产品号和折扣信息

**SQL 查询语句与数据插入：**

SQL

```
-- 1. 插入实验要求的 3 条数据
INSERT INTO discounts(product_id, discount_message) VALUES(1, 'Buy 1 and Get 25% OFF on 2nd ');
INSERT INTO discounts(product_id, discount_message) VALUES(2, 'Buy 2 and Get 50% OFF on 3rd ');
INSERT INTO discounts(product_id, discount_message) VALUES(3, 'Buy 3 Get 1 free');

-- 2. 执行包含转义字符的模糊查询
SELECT product_id, discount_message 
FROM discounts 
WHERE discount_message LIKE '%25\%%' ESCAPE '\';
```