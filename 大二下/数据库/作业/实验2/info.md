用户名: root
密码: OpenGauss@su
用户名: omm
密码: OpenGauss@omm
数据库密码GaussDB@123

创建新用户
CREATE USER wang_ziheng WITH PASSWORD 'wangziheng@123';
创建数据库
CREATE DATABASE sales OWNER wang_ziheng;
创建模式
\c sales
CREATE SCHEMA sales AUTHORIZATION wang_ziheng;
创建表

```sql
-- 确保已创建并切换到 sales 模式
SET search_path TO sales;

-- 1. 创建 REGIONS 表
CREATE TABLE regions (
    region_id NUMBER,
    region_name VARCHAR2(50)
);

-- 2. 创建 COUNTRIES 表
CREATE TABLE countries (
    country_id CHAR(2),
    country_name VARCHAR2(40),
    region_id NUMBER
);

-- 3. 创建 LOCATIONS 表
CREATE TABLE locations (
    location_id NUMBER,
    address VARCHAR2(255),
    postal_code VARCHAR2(20),
    city VARCHAR2(50),
    state VARCHAR2(50),
    country_id CHAR(2)
);

-- 4. 创建 WAREHOUSES 表
CREATE TABLE warehouses (
    warehouse_id NUMBER,
    warehouse_name VARCHAR2(255),
    location_id NUMBER
);

-- 5. 创建 EMPLOYEES 表
CREATE TABLE employees (
    employee_id NUMBER,
    first_name VARCHAR2(255),
    last_name VARCHAR2(255),
    email VARCHAR2(255),
    phone VARCHAR2(50),
    hire_date DATE,
    manager_id NUMBER,
    job_title VARCHAR2(255)
);

-- 6. 创建 PRODUCT_CATEGORIES 表
CREATE TABLE product_categories (
    category_id NUMBER,
    category_name VARCHAR2(255)
);

-- 7. 创建 PRODUCTS 表
CREATE TABLE products (
    product_id NUMBER,
    product_name VARCHAR2(255),
    description VARCHAR2(2000),
    standard_cost NUMBER(9,2),
    list_price NUMBER(9,2),
    category_id NUMBER
);

-- 8. 创建 INVENTORIES 表
CREATE TABLE inventories (
    product_id NUMBER,
    warehouse_id NUMBER,
    quantity NUMBER(8)
);

-- 9. 创建 CUSTOMERS 表
CREATE TABLE customers (
    customer_id NUMBER,
    name VARCHAR2(255),
    address VARCHAR2(255),
    website VARCHAR2(255),
    credit_limit NUMBER(8,2)
);

-- 10. 创建 CONTACTS 表
CREATE TABLE contacts (
    contact_id NUMBER,
    first_name VARCHAR2(255),
    last_name VARCHAR2(255),
    email VARCHAR2(255),
    phone VARCHAR2(20),
    customer_id NUMBER
);

-- 11. 创建 ORDERS 表
CREATE TABLE orders (
    order_id NUMBER,
    customer_id NUMBER,
    status VARCHAR2(20),
    salesman_id NUMBER,
    order_date DATE
);

-- 12. 创建 ORDER_ITEMS 表
CREATE TABLE order_items (
    order_id NUMBER,
    item_id NUMBER(12),
    product_id NUMBER,
    quantity NUMBER(8,2),
    unit_price NUMBER(8,2)
);
```



在完成数据导入后，下一步是根据“样例数据库模式”为表添加主键（Primary Key）和外键（Foreign Key）约束 。

### 1. 添加主键约束 (Primary Keys)

```
-- 设置当前模式
SET search_path TO sales;

ALTER TABLE regions ADD CONSTRAINT regions_pk PRIMARY KEY (region_id);
ALTER TABLE countries ADD CONSTRAINT countries_pk PRIMARY KEY (country_id);
ALTER TABLE locations ADD CONSTRAINT locations_pk PRIMARY KEY (location_id);
ALTER TABLE warehouses ADD CONSTRAINT warehouses_pk PRIMARY KEY (warehouse_id);
ALTER TABLE employees ADD CONSTRAINT employees_pk PRIMARY KEY (employee_id);
ALTER TABLE product_categories ADD CONSTRAINT product_categories_pk PRIMARY KEY (category_id);
ALTER TABLE products ADD CONSTRAINT products_pk PRIMARY KEY (product_id);
ALTER TABLE customers ADD CONSTRAINT customers_pk PRIMARY KEY (customer_id);
ALTER TABLE contacts ADD CONSTRAINT contacts_pk PRIMARY KEY (contact_id);
ALTER TABLE orders ADD CONSTRAINT orders_pk PRIMARY KEY (order_id);
-- 复合主键
ALTER TABLE order_items ADD CONSTRAINT order_items_pk PRIMARY KEY (order_id, item_id);
ALTER TABLE inventories ADD CONSTRAINT inventories_pk PRIMARY KEY (product_id, warehouse_id);
```

### 2. 添加外键约束 (Foreign Keys)

```
-- 地理位置相关关联
ALTER TABLE countries ADD CONSTRAINT fk_countries_regions FOREIGN KEY (region_id) REFERENCES regions(region_id);
ALTER TABLE locations ADD CONSTRAINT fk_locations_countries FOREIGN KEY (country_id) REFERENCES countries(country_id);
ALTER TABLE warehouses ADD CONSTRAINT fk_warehouses_locations FOREIGN KEY (location_id) REFERENCES locations(location_id);

-- 产品与库存关联
ALTER TABLE products ADD CONSTRAINT fk_products_categories FOREIGN KEY (category_id) REFERENCES product_categories(category_id);
ALTER TABLE inventories ADD CONSTRAINT fk_inventories_products FOREIGN KEY (product_id) REFERENCES products(product_id);
ALTER TABLE inventories ADD CONSTRAINT fk_inventories_warehouses FOREIGN KEY (warehouse_id) REFERENCES warehouses(warehouse_id);

-- 客户与联系人关联
ALTER TABLE contacts ADD CONSTRAINT fk_contacts_customers FOREIGN KEY (customer_id) REFERENCES customers(customer_id);

-- 订单、员工与明细关联
ALTER TABLE orders ADD CONSTRAINT fk_orders_customers FOREIGN KEY (customer_id) REFERENCES customers(customer_id);
ALTER TABLE orders ADD CONSTRAINT fk_orders_employees FOREIGN KEY (salesman_id) REFERENCES employees(employee_id);
ALTER TABLE order_items ADD CONSTRAINT fk_order_items_orders FOREIGN KEY (order_id) REFERENCES orders(order_id);
ALTER TABLE order_items ADD CONSTRAINT fk_order_items_products FOREIGN KEY (product_id) REFERENCES products(product_id);

-- 员工自关联（上级领导）
ALTER TABLE employees ADD CONSTRAINT fk_employees_manager FOREIGN KEY (manager_id) REFERENCES employees(employee_id);
```

### 3. 依次查看 12 张表的数据

```
SET search_path TO sales;
-- 1. 区域表
SELECT * FROM regions;
-- 2. 国家表
SELECT * FROM countries;
-- 3. 地点表
SELECT * FROM locations;
-- 4. 仓库表
SELECT * FROM warehouses;
-- 5. 员工表 (检查日期是否为 2016-06-17 格式)
SELECT * FROM employees;
-- 6. 产品分类表
SELECT * FROM product_categories;
-- 7. 产品表
SELECT * FROM products;
-- 8. 库存表 (数据量最大)
SELECT * FROM inventories;
-- 9. 客户表
SELECT * FROM customers;
-- 10. 联系人表
SELECT * FROM contacts;
-- 11. 订单表 (检查日期格式)
SELECT * FROM orders;
-- 12. 订单明细表
SELECT * FROM order_items;
```

为了完成实验报告并验证数据库状态，你可以执行以下这一组**元命令**。它们能最直观地展示你已经成功创建了模式、导入了数据并添加了约束。

```
-- 1. 切换到实验模式
SET search_path TO sales;

-- 2. 查看所有表及其描述（验证表是否存在）
\dt+

-- 3. 查看核心表结构与约束（验证主外键是否添加成功）
-- 重点查看底部的 "Indexes" 和 "Foreign-key constraints" 部分
\d+ sales.orders

-- 4. 统计各表行数（验证数据是否导入成功）
-- 如果之前导入成功，这里应该能看到 non-zero 的数字
SELECT 'orders' as table_name, count(*) FROM sales.orders
UNION ALL
SELECT 'employees', count(*) FROM sales.employees
UNION ALL
SELECT 'customers', count(*) FROM sales.customers;
```

在 openGauss 中，如果你需要彻底删除一个用户以及该用户拥有的所有对象（数据库、模式、表、索引等），必须遵循一定的顺序。因为数据库对象之间存在依赖关系，直接删除用户通常会报错。

请在 `gsql` 中以 **管理员（如 omm）** 身份执行以下步骤：

### 1. 强制删除数据库

```
-- 1. 强制断开所有连接到该数据库的会话（可选，防止数据库被占用无法删除）
SELECT pg_terminate_backend(pid) FROM pg_stat_activity 
WHERE datname = 'sales';

-- 2. 删除数据库
DROP DATABASE IF EXISTS sales;
```

------

### 2. 删除模式及其中所有表、索引

```
-- 切换到对应的数据库（如果还没删库，只是想清空模式）
-- \c sales

-- 删除模式及其包含的所有对象（表、索引、约束等）
DROP SCHEMA IF EXISTS sales CASCADE;
```

------

### 3. 删除用户

```
-- 如果该用户还拥有其他权限或对象，使用 CASCADE 强制清理
DROP USER IF EXISTS omm CASCADE; 
-- 或者你创建的拼音用户名，例如：
-- DROP USER IF EXISTS wang_ziheng CASCADE;
```

### 💡 总结：一条龙清理命令（慎用）

```
-- 确保不在 sales 库里
\c postgres

-- 1. 删库（会自动删除库内所有表和索引）
DROP DATABASE IF EXISTS sales;

-- 2. 删用户（CASCADE 会处理掉残留的权限关联）
DROP USER IF EXISTS omm CASCADE;
```