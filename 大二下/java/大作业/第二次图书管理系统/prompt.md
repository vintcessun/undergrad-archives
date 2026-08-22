# JavaFX 图书管理系统 —— 完整开发计划

> **目标**：使用 Java + JavaFX 构建一个具备图书管理、查询、借阅管理、数据持久化功能的桌面应用。
> **本文档可直接发送给 Codex / Claude Code 执行**，包含完整的项目结构、类设计、关键代码片段和评分对照。

---

## 一、评分标准深度解读（重中之重⚠️）

根据作业要求，**代码仅占 20%，回答问题占 80%**。这意味着：

| 评分项 | 比例 | 关键要求 | 应对策略 |
|--------|------|----------|----------|
| 面向对象设计 | 25% | ≥4个类，体现封装/继承/多态 | Book, User(抽象类), Reader/Admin(继承), BorrowRecord, LibrarySystem；使用多态处理权限 |
| 集合框架 | 20% | ArrayList/HashMap/LinkedList | ArrayList存图书列表, HashMap<String,Book>按ISBN索引, LinkedList存借阅记录 |
| 异常处理 | 15% | 输入校验、文件IO异常、业务异常 | 自定义 LibraryException；每个IO操作 try-catch；输入非空/格式校验 |
| IO文件操作 | 15% | FileReader/FileWriter/BufferedReader/BufferedWriter | **必须用这四个类**，不能用 ObjectOutputStream 或序列化 |
| 代码规范 | 15% | 命名、注释、缩进 | 驼峰命名、每个方法 Javadoc 注释、统一4空格缩进 |
| 图形界面 | 10% | JavaFX | TabPane 多标签页布局，TableView 展示数据 |

### 🔴 回答问题准备（占总分 80%）

你必须能够回答以下问题：
1. **为什么选择 HashMap 存储图书？** → O(1) 通过 ISBN 查找，避免遍历
2. **继承和多态体现在哪里？** → User 是抽象类，Reader 和 Admin 继承它，重写 getPermissions()
3. **异常处理的设计思路？** → 分层：自定义业务异常 + IO 异常 + 输入校验异常
4. **文件格式为什么用特定分隔符？** → 用 `|` 分隔而非逗号，避免与书名中的逗号冲突
5. **借阅规则如何实现？** → 借阅前检查库存>0 且 HashMap 中无该读者对该ISBN的未还记录

---

## 二、项目结构

```
com.library/
├── Main.java                    // JavaFX Application 入口
├── model/                       // 数据模型层
│   ├── Book.java                // 图书实体类
│   ├── User.java                // 用户抽象基类
│   ├── Reader.java              // 普通读者（继承 User）
│   ├── Admin.java               // 管理员（继承 User）
│   └── BorrowRecord.java        // 借阅记录实体类
├── service/                     // 业务逻辑层
│   └── LibrarySystem.java       // 核心业务逻辑
├── util/                        // 工具层
│   ├── FileUtil.java            // IO 文件读写
│   └── LibraryException.java    // 自定义异常
└── ui/                          // JavaFX 界面层
    └── LibraryUI.java           // 界面构建与事件处理
```

---

## 三、类设计详解与关键代码

### 3.1 Book.java — 图书实体

```java
package com.library.model;

/**
 * 图书实体类，封装图书的基本信息
 * 体现面向对象的【封装】特性：所有属性私有，通过 getter/setter 访问
 */
public class Book {
    private String title;      // 书名
    private String author;     // 作者
    private String isbn;       // ISBN（唯一标识）
    private String publisher;  // 出版社
    private int stock;         // 库存数量

    public Book(String title, String author, String isbn, String publisher, int stock) {
        this.title = title;
        this.author = author;
        this.isbn = isbn;
        this.publisher = publisher;
        this.stock = stock;
    }

    // --- getter/setter 省略，全部生成 ---

    public String getTitle() { return title; }
    public void setTitle(String title) { this.title = title; }
    public String getAuthor() { return author; }
    public void setAuthor(String author) { this.author = author; }
    public String getIsbn() { return isbn; }
    public String getPublisher() { return publisher; }
    public void setPublisher(String publisher) { this.publisher = publisher; }
    public int getStock() { return stock; }
    public void setStock(int stock) { this.stock = stock; }

    /**
     * 序列化为文件存储格式，使用 | 分隔
     * 选择 | 而非逗号：避免书名/作者中包含逗号导致解析错误
     */
    public String toFileString() {
        return title + "|" + author + "|" + isbn + "|" + publisher + "|" + stock;
    }

    /**
     * 从文件行解析为 Book 对象
     */
    public static Book fromFileString(String line) {
        String[] parts = line.split("\\|");
        if (parts.length != 5) {
            throw new IllegalArgumentException("图书数据格式错误: " + line);
        }
        return new Book(parts[0].trim(), parts[1].trim(), parts[2].trim(),
                        parts[3].trim(), Integer.parseInt(parts[4].trim()));
    }

    @Override
    public String toString() {
        return String.format("《%s》 作者:%s ISBN:%s 出版社:%s 库存:%d",
                title, author, isbn, publisher, stock);
    }
}
```

### 3.2 User.java — 抽象用户基类（体现继承与多态）

```java
package com.library.model;

/**
 * 用户抽象基类
 * 体现面向对象的【继承】和【多态】特性：
 * - Reader 和 Admin 继承此类
 * - getRole() 和 hasPermission() 由子类各自实现（多态）
 */
public abstract class User {
    protected String username;
    protected String password;

    public User(String username, String password) {
        this.username = username;
        this.password = password;
    }

    public String getUsername() { return username; }
    public String getPassword() { return password; }

    /** 抽象方法：获取用户角色描述 */
    public abstract String getRole();

    /** 抽象方法：判断是否拥有指定权限（多态的核心体现） */
    public abstract boolean hasPermission(String action);
}
```

### 3.3 Reader.java & Admin.java

```java
package com.library.model;

/**
 * 普通读者，只有查询和借阅权限
 */
public class Reader extends User {
    public Reader(String username, String password) {
        super(username, password);
    }

    @Override
    public String getRole() { return "普通读者"; }

    @Override
    public boolean hasPermission(String action) {
        // 读者只能查询和借还书
        return action.equals("QUERY") || action.equals("BORROW") || action.equals("RETURN");
    }
}
```

```java
package com.library.model;

/**
 * 管理员，拥有全部权限
 */
public class Admin extends User {
    public Admin(String username, String password) {
        super(username, password);
    }

    @Override
    public String getRole() { return "管理员"; }

    @Override
    public boolean hasPermission(String action) {
        return true; // 管理员拥有所有权限
    }
}
```

### 3.4 BorrowRecord.java — 借阅记录

```java
package com.library.model;

import java.time.LocalDate;
import java.time.format.DateTimeFormatter;

/**
 * 借阅记录实体类
 */
public class BorrowRecord {
    private String readerName;    // 读者姓名
    private String bookIsbn;      // 图书ISBN
    private String bookTitle;     // 图书名称（冗余存储，便于展示）
    private LocalDate borrowDate; // 借阅日期
    private boolean returned;     // 是否已归还

    private static final DateTimeFormatter FMT = DateTimeFormatter.ofPattern("yyyy-MM-dd");

    public BorrowRecord(String readerName, String bookIsbn, String bookTitle,
                        LocalDate borrowDate, boolean returned) {
        this.readerName = readerName;
        this.bookIsbn = bookIsbn;
        this.bookTitle = bookTitle;
        this.borrowDate = borrowDate;
        this.returned = returned;
    }

    // --- getter/setter ---
    public String getReaderName() { return readerName; }
    public String getBookIsbn() { return bookIsbn; }
    public String getBookTitle() { return bookTitle; }
    public LocalDate getBorrowDate() { return borrowDate; }
    public boolean isReturned() { return returned; }
    public void setReturned(boolean returned) { this.returned = returned; }

    /**
     * 序列化为文件格式
     */
    public String toFileString() {
        return readerName + "|" + bookIsbn + "|" + bookTitle + "|"
               + borrowDate.format(FMT) + "|" + (returned ? "1" : "0");
    }

    /**
     * 从文件行解析
     */
    public static BorrowRecord fromFileString(String line) {
        String[] parts = line.split("\\|");
        if (parts.length != 5) {
            throw new IllegalArgumentException("借阅记录格式错误: " + line);
        }
        return new BorrowRecord(
            parts[0].trim(), parts[1].trim(), parts[2].trim(),
            LocalDate.parse(parts[3].trim(), FMT),
            parts[4].trim().equals("1")
        );
    }
}
```

### 3.5 LibraryException.java — 自定义异常

```java
package com.library.util;

/**
 * 自定义图书馆业务异常
 * 用于处理：库存不足、重复借阅、图书不存在等业务场景
 */
public class LibraryException extends Exception {
    public LibraryException(String message) {
        super(message);
    }
}
```

### 3.6 FileUtil.java — IO 文件操作（评分重点⚠️）

```java
package com.library.util;

import com.library.model.Book;
import com.library.model.BorrowRecord;
import java.io.*;
import java.util.ArrayList;
import java.util.List;

/**
 * 文件读写工具类
 * 【必须】使用 FileReader/FileWriter/BufferedReader/BufferedWriter
 * 这是评分明确要求的 IO 流类型
 */
public class FileUtil {

    /**
     * 从文件读取所有图书
     */
    public static List<Book> loadBooks(String filePath) {
        List<Book> books = new ArrayList<>();
        File file = new File(filePath);
        if (!file.exists()) {
            return books; // 文件不存在，返回空列表
        }
        // 使用 BufferedReader 包装 FileReader（评分要求）
        try (BufferedReader br = new BufferedReader(new FileReader(file))) {
            String line;
            while ((line = br.readLine()) != null) {
                line = line.trim();
                if (!line.isEmpty()) {
                    try {
                        books.add(Book.fromFileString(line));
                    } catch (IllegalArgumentException e) {
                        System.err.println("跳过格式错误的行: " + e.getMessage());
                    }
                }
            }
        } catch (IOException e) {
            System.err.println("读取图书文件失败: " + e.getMessage());
        }
        return books;
    }

    /**
     * 保存所有图书到文件
     */
    public static void saveBooks(String filePath, List<Book> books) {
        // 使用 BufferedWriter 包装 FileWriter（评分要求）
        try (BufferedWriter bw = new BufferedWriter(new FileWriter(filePath))) {
            for (Book book : books) {
                bw.write(book.toFileString());
                bw.newLine();
            }
        } catch (IOException e) {
            System.err.println("保存图书文件失败: " + e.getMessage());
        }
    }

    /**
     * 从文件读取所有借阅记录
     */
    public static List<BorrowRecord> loadRecords(String filePath) {
        List<BorrowRecord> records = new ArrayList<>();
        File file = new File(filePath);
        if (!file.exists()) {
            return records;
        }
        try (BufferedReader br = new BufferedReader(new FileReader(file))) {
            String line;
            while ((line = br.readLine()) != null) {
                line = line.trim();
                if (!line.isEmpty()) {
                    try {
                        records.add(BorrowRecord.fromFileString(line));
                    } catch (Exception e) {
                        System.err.println("跳过格式错误的记录: " + e.getMessage());
                    }
                }
            }
        } catch (IOException e) {
            System.err.println("读取借阅记录失败: " + e.getMessage());
        }
        return records;
    }

    /**
     * 保存所有借阅记录到文件
     */
    public static void saveRecords(String filePath, List<BorrowRecord> records) {
        try (BufferedWriter bw = new BufferedWriter(new FileWriter(filePath))) {
            for (BorrowRecord record : records) {
                bw.write(record.toFileString());
                bw.newLine();
            }
        } catch (IOException e) {
            System.err.println("保存借阅记录失败: " + e.getMessage());
        }
    }
}
```

### 3.7 LibrarySystem.java — 核心业务逻辑

```java
package com.library.service;

import com.library.model.Book;
import com.library.model.BorrowRecord;
import com.library.util.FileUtil;
import com.library.util.LibraryException;

import java.time.LocalDate;
import java.util.*;
import java.util.stream.Collectors;

/**
 * 图书馆系统核心业务逻辑
 * 集合框架使用说明：
 * - HashMap<String, Book>: 按 ISBN 快速索引，O(1) 查找
 * - ArrayList<Book>: 有序存储图书列表，支持遍历
 * - LinkedList<BorrowRecord>: 借阅记录频繁增删，LinkedList 性能更优
 */
public class LibrarySystem {
    private HashMap<String, Book> bookMap;          // ISBN -> Book 快速索引
    private LinkedList<BorrowRecord> borrowRecords; // 借阅记录

    private static final String BOOKS_FILE = "books.txt";
    private static final String RECORDS_FILE = "records.txt";

    public LibrarySystem() {
        // 启动时从文件加载数据
        List<Book> bookList = FileUtil.loadBooks(BOOKS_FILE);
        bookMap = new HashMap<>();
        for (Book book : bookList) {
            bookMap.put(book.getIsbn(), book);
        }
        borrowRecords = new LinkedList<>(FileUtil.loadRecords(RECORDS_FILE));
    }

    /** 保存所有数据到文件 */
    public void saveAll() {
        FileUtil.saveBooks(BOOKS_FILE, new ArrayList<>(bookMap.values()));
        FileUtil.saveRecords(RECORDS_FILE, new ArrayList<>(borrowRecords));
    }

    // =================== 图书管理 ===================

    /** 添加图书 */
    public void addBook(Book book) throws LibraryException {
        if (book.getIsbn() == null || book.getIsbn().trim().isEmpty()) {
            throw new LibraryException("ISBN 不能为空");
        }
        if (bookMap.containsKey(book.getIsbn())) {
            throw new LibraryException("ISBN 已存在: " + book.getIsbn());
        }
        bookMap.put(book.getIsbn(), book);
        saveAll();
    }

    /** 删除图书 */
    public void deleteBook(String isbn) throws LibraryException {
        if (!bookMap.containsKey(isbn)) {
            throw new LibraryException("图书不存在: " + isbn);
        }
        // 检查是否有未归还的借阅记录
        boolean hasBorrow = borrowRecords.stream()
            .anyMatch(r -> r.getBookIsbn().equals(isbn) && !r.isReturned());
        if (hasBorrow) {
            throw new LibraryException("该图书有未归还的借阅记录，无法删除");
        }
        bookMap.remove(isbn);
        saveAll();
    }

    /** 修改图书 */
    public void updateBook(String isbn, String newTitle, String newAuthor,
                           String newPublisher, int newStock) throws LibraryException {
        Book book = bookMap.get(isbn);
        if (book == null) {
            throw new LibraryException("图书不存在: " + isbn);
        }
        if (newStock < 0) {
            throw new LibraryException("库存不能为负数");
        }
        book.setTitle(newTitle);
        book.setAuthor(newAuthor);
        book.setPublisher(newPublisher);
        book.setStock(newStock);
        saveAll();
    }

    /** 获取所有图书 */
    public List<Book> getAllBooks() {
        return new ArrayList<>(bookMap.values());
    }

    // =================== 图书查询 ===================

    /** 按书名模糊查询 */
    public List<Book> searchByTitle(String keyword) {
        return bookMap.values().stream()
            .filter(b -> b.getTitle().contains(keyword))
            .collect(Collectors.toList());
    }

    /** 按作者查询 */
    public List<Book> searchByAuthor(String author) {
        return bookMap.values().stream()
            .filter(b -> b.getAuthor().contains(author))
            .collect(Collectors.toList());
    }

    /** 按 ISBN 精确查询 */
    public Book searchByIsbn(String isbn) {
        return bookMap.get(isbn); // HashMap O(1) 查找
    }

    // =================== 借阅管理 ===================

    /** 借书 */
    public void borrowBook(String isbn, String readerName) throws LibraryException {
        // 输入校验
        if (isbn == null || isbn.trim().isEmpty()) {
            throw new LibraryException("ISBN 不能为空");
        }
        if (readerName == null || readerName.trim().isEmpty()) {
            throw new LibraryException("读者姓名不能为空");
        }

        Book book = bookMap.get(isbn);
        if (book == null) {
            throw new LibraryException("图书不存在: " + isbn);
        }

        // 规则①：库存为 0 时不可借阅
        if (book.getStock() <= 0) {
            throw new LibraryException("库存不足，无法借阅: " + book.getTitle());
        }

        // 规则②：同一本书同一人不可重复借阅
        boolean alreadyBorrowed = borrowRecords.stream()
            .anyMatch(r -> r.getBookIsbn().equals(isbn)
                        && r.getReaderName().equals(readerName)
                        && !r.isReturned());
        if (alreadyBorrowed) {
            throw new LibraryException(readerName + " 已经借阅了《" + book.getTitle() + "》，不可重复借阅");
        }

        // 执行借阅：库存 -1，添加记录
        book.setStock(book.getStock() - 1);
        borrowRecords.add(new BorrowRecord(readerName, isbn, book.getTitle(),
                                           LocalDate.now(), false));
        saveAll();
    }

    /** 还书 */
    public void returnBook(String isbn, String readerName) throws LibraryException {
        Book book = bookMap.get(isbn);
        if (book == null) {
            throw new LibraryException("图书不存在: " + isbn);
        }

        // 查找未归还的借阅记录
        Optional<BorrowRecord> record = borrowRecords.stream()
            .filter(r -> r.getBookIsbn().equals(isbn)
                      && r.getReaderName().equals(readerName)
                      && !r.isReturned())
            .findFirst();

        if (!record.isPresent()) {
            throw new LibraryException("未找到 " + readerName + " 借阅《" + book.getTitle() + "》的记录");
        }

        // 执行归还：标记已还，库存 +1
        record.get().setReturned(true);
        book.setStock(book.getStock() + 1);
        saveAll();
    }

    /** 获取所有未归还的借阅记录 */
    public List<BorrowRecord> getUnreturnedRecords() {
        return borrowRecords.stream()
            .filter(r -> !r.isReturned())
            .collect(Collectors.toList());
    }

    /** 获取所有借阅记录 */
    public List<BorrowRecord> getAllRecords() {
        return new ArrayList<>(borrowRecords);
    }
}
```

### 3.8 LibraryUI.java — JavaFX 界面（纯 Java 代码方式，不使用 FXML）

```java
package com.library.ui;

import com.library.model.Book;
import com.library.model.BorrowRecord;
import com.library.model.User;
import com.library.model.Admin;
import com.library.model.Reader;
import com.library.service.LibrarySystem;
import com.library.util.LibraryException;

import javafx.application.Application;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.layout.*;
import javafx.stage.Stage;

/**
 * JavaFX 图形界面主类
 * 使用 TabPane 实现多功能标签页切换
 * 不使用 FXML，纯 Java 代码构建界面（更适合本项目体量）
 */
public class LibraryUI extends Application {

    private LibrarySystem system = new LibrarySystem();
    private User currentUser;

    // ===== 图书管理表格 =====
    private TableView<Book> bookTable;
    private ObservableList<Book> bookData;

    // ===== 借阅记录表格 =====
    private TableView<BorrowRecord> recordTable;
    private ObservableList<BorrowRecord> recordData;

    @Override
    public void start(Stage primaryStage) {
        // 先显示登录界面
        showLoginDialog(primaryStage);
    }

    /**
     * 登录对话框
     */
    private void showLoginDialog(Stage primaryStage) {
        Dialog<User> dialog = new Dialog<>();
        dialog.setTitle("图书管理系统 - 登录");
        dialog.setHeaderText("请选择角色并登录");

        // 表单
        GridPane grid = new GridPane();
        grid.setHgap(10);
        grid.setVgap(10);
        grid.setPadding(new Insets(20));

        TextField usernameField = new TextField();
        usernameField.setPromptText("用户名");
        PasswordField passwordField = new PasswordField();
        passwordField.setPromptText("密码");
        ComboBox<String> roleBox = new ComboBox<>();
        roleBox.getItems().addAll("管理员", "普通读者");
        roleBox.setValue("管理员");

        grid.add(new Label("用户名:"), 0, 0);
        grid.add(usernameField, 1, 0);
        grid.add(new Label("密码:"), 0, 1);
        grid.add(passwordField, 1, 1);
        grid.add(new Label("角色:"), 0, 2);
        grid.add(roleBox, 1, 2);

        dialog.getDialogPane().setContent(grid);

        ButtonType loginBtn = new ButtonType("登录", ButtonBar.ButtonData.OK_DONE);
        dialog.getDialogPane().getButtonTypes().addAll(loginBtn, ButtonType.CANCEL);

        dialog.setResultConverter(btn -> {
            if (btn == loginBtn) {
                String role = roleBox.getValue();
                String username = usernameField.getText();
                String password = passwordField.getText();
                if ("管理员".equals(role)) {
                    return new Admin(username, password);
                } else {
                    return new Reader(username, password);
                }
            }
            return null;
        });

        Optional<User> result = dialog.showAndWait();
        if (result.isPresent()) {
            currentUser = result.get();
            showMainUI(primaryStage);
        } else {
            System.exit(0);
        }
    }

    /**
     * 主界面
     */
    private void showMainUI(Stage primaryStage) {
        TabPane tabPane = new TabPane();

        // Tab 1: 图书管理（管理员可增删改，读者只读）
        Tab bookTab = new Tab("📚 图书管理");
        bookTab.setClosable(false);
        bookTab.setContent(createBookManagementPane());

        // Tab 2: 图书查询
        Tab searchTab = new Tab("🔍 图书查询");
        searchTab.setClosable(false);
        searchTab.setContent(createSearchPane());

        // Tab 3: 借阅管理
        Tab borrowTab = new Tab("📖 借阅管理");
        borrowTab.setClosable(false);
        borrowTab.setContent(createBorrowPane());

        // Tab 4: 借阅记录
        Tab recordTab = new Tab("📋 借阅记录");
        recordTab.setClosable(false);
        recordTab.setContent(createRecordPane());

        tabPane.getTabs().addAll(bookTab, searchTab, borrowTab, recordTab);

        // 顶部状态栏（使用 CSS 样式类 .status-bar，颜色 #1A73E8）
        HBox statusBar = new HBox();
        statusBar.getStyleClass().add("status-bar");
        statusBar.setPadding(new Insets(8, 16, 8, 16));
        statusBar.setAlignment(Pos.CENTER_LEFT);
        Label statusLabel = new Label("当前用户: " + currentUser.getUsername()
                                      + " | 角色: " + currentUser.getRole());
        statusLabel.setStyle("-fx-text-fill: white; -fx-font-weight: bold; -fx-font-size: 14px;");
        Pane spacer = new Pane();
        HBox.setHgrow(spacer, Priority.ALWAYS);
        Label versionLabel = new Label("图书管理系统 v1.0");
        versionLabel.setStyle("-fx-text-fill: white; -fx-font-size: 12px;");
        statusBar.getChildren().addAll(statusLabel, spacer, versionLabel);

        VBox root = new VBox(statusBar, tabPane);
        VBox.setVgrow(tabPane, Priority.ALWAYS);

        Scene scene = new Scene(root, 900, 650);

        // 加载 CSS 样式表（防御性加载，缺失时降级为默认样式）
        try {
            String css = getClass().getResource("/theme.css").toExternalForm();
            scene.getStylesheets().add(css);
        } catch (NullPointerException e) {
            System.err.println("警告：未找到 theme.css，使用默认样式");
        }

        primaryStage.setTitle("图书管理系统");
        primaryStage.setScene(scene);
        primaryStage.show();

        // 窗口关闭时保存数据
        primaryStage.setOnCloseRequest(e -> system.saveAll());
    }

    /**
     * 创建图书管理面板 — 包含 TableView + 增删改按钮
     */
    @SuppressWarnings("unchecked")
    private VBox createBookManagementPane() {
        // 表格
        bookTable = new TableView<>();
        bookData = FXCollections.observableArrayList(system.getAllBooks());

        TableColumn<Book, String> titleCol = new TableColumn<>("书名");
        titleCol.setCellValueFactory(new PropertyValueFactory<>("title"));
        titleCol.setPrefWidth(180);

        TableColumn<Book, String> authorCol = new TableColumn<>("作者");
        authorCol.setCellValueFactory(new PropertyValueFactory<>("author"));
        authorCol.setPrefWidth(120);

        TableColumn<Book, String> isbnCol = new TableColumn<>("ISBN");
        isbnCol.setCellValueFactory(new PropertyValueFactory<>("isbn"));
        isbnCol.setPrefWidth(150);

        TableColumn<Book, String> pubCol = new TableColumn<>("出版社");
        pubCol.setCellValueFactory(new PropertyValueFactory<>("publisher"));
        pubCol.setPrefWidth(150);

        TableColumn<Book, Integer> stockCol = new TableColumn<>("库存");
        stockCol.setCellValueFactory(new PropertyValueFactory<>("stock"));
        stockCol.setPrefWidth(80);

        bookTable.getColumns().addAll(titleCol, authorCol, isbnCol, pubCol, stockCol);
        bookTable.setColumnResizePolicy(TableView.CONSTRAINED_RESIZE_POLICY); // 列宽自动平铺
        bookTable.setItems(bookData);

        // 按钮区域 — 统一创建，通过 setVisible/setManaged 控制权限
        HBox buttonBox = new HBox(10);
        buttonBox.setPadding(new Insets(10));
        buttonBox.setAlignment(Pos.CENTER_LEFT);

        Button addBtn = new Button("添加图书");
        addBtn.setOnAction(e -> showAddBookDialog());

        Button deleteBtn = new Button("删除图书");
        deleteBtn.setOnAction(e -> handleDeleteBook());

        Button editBtn = new Button("修改图书");
        editBtn.setOnAction(e -> showEditBookDialog());

        Button refreshBtn = new Button("刷新列表");
        refreshBtn.getStyleClass().add("button-secondary"); // 次要按钮样式
        refreshBtn.setOnAction(e -> refreshBookTable());

        buttonBox.getChildren().addAll(addBtn, deleteBtn, editBtn, refreshBtn);

        // 权限控制：非管理员隐藏增删改按钮，同时让出布局空间
        if (!currentUser.hasPermission("ADD")) {
            addBtn.setVisible(false);    addBtn.setManaged(false);
            deleteBtn.setVisible(false); deleteBtn.setManaged(false);
            editBtn.setVisible(false);   editBtn.setManaged(false);
        }

        VBox pane = new VBox(10, bookTable, buttonBox);
        pane.setPadding(new Insets(10));
        VBox.setVgrow(bookTable, Priority.ALWAYS);
        return pane;
    }

    /**
     * 创建查询面板
     */
    private VBox createSearchPane() {
        // 搜索条件区域
        HBox searchBox = new HBox(10);
        searchBox.setPadding(new Insets(10));
        searchBox.setAlignment(Pos.CENTER_LEFT);

        ComboBox<String> searchType = new ComboBox<>();
        searchType.getItems().addAll("按书名", "按作者", "按ISBN");
        searchType.setValue("按书名");

        TextField searchField = new TextField();
        searchField.setPromptText("输入查询关键词...");
        searchField.setPrefWidth(250);

        Button searchBtn = new Button("查询");
        Button showAllBtn = new Button("显示全部");
        showAllBtn.getStyleClass().add("button-secondary");

        // 查询结果表格（完整列定义）
        TableView<Book> searchTable = new TableView<>();
        ObservableList<Book> searchData = FXCollections.observableArrayList();

        TableColumn<Book, String> sTitleCol = new TableColumn<>("书名");
        sTitleCol.setCellValueFactory(new PropertyValueFactory<>("title"));
        TableColumn<Book, String> sAuthorCol = new TableColumn<>("作者");
        sAuthorCol.setCellValueFactory(new PropertyValueFactory<>("author"));
        TableColumn<Book, String> sIsbnCol = new TableColumn<>("ISBN");
        sIsbnCol.setCellValueFactory(new PropertyValueFactory<>("isbn"));
        TableColumn<Book, String> sPubCol = new TableColumn<>("出版社");
        sPubCol.setCellValueFactory(new PropertyValueFactory<>("publisher"));
        TableColumn<Book, Integer> sStockCol = new TableColumn<>("库存");
        sStockCol.setCellValueFactory(new PropertyValueFactory<>("stock"));

        searchTable.getColumns().addAll(sTitleCol, sAuthorCol, sIsbnCol, sPubCol, sStockCol);
        searchTable.setColumnResizePolicy(TableView.CONSTRAINED_RESIZE_POLICY);
        searchTable.setItems(searchData);

        searchBtn.setOnAction(e -> {
            String keyword = searchField.getText().trim();
            if (keyword.isEmpty()) {
                showAlert(Alert.AlertType.WARNING, "请输入查询关键词");
                return;
            }
            List<Book> results;
            switch (searchType.getValue()) {
                case "按书名": results = system.searchByTitle(keyword); break;
                case "按作者": results = system.searchByAuthor(keyword); break;
                case "按ISBN":
                    Book b = system.searchByIsbn(keyword);
                    results = b != null ? List.of(b) : List.of();
                    break;
                default: results = List.of();
            }
            searchData.setAll(results);
            if (results.isEmpty()) {
                showAlert(Alert.AlertType.INFORMATION, "未找到匹配的图书");
            }
        });

        showAllBtn.setOnAction(e -> searchData.setAll(system.getAllBooks()));

        searchBox.getChildren().addAll(searchType, searchField, searchBtn, showAllBtn);

        VBox pane = new VBox(10, searchBox, searchTable);
        pane.setPadding(new Insets(10));
        VBox.setVgrow(searchTable, Priority.ALWAYS);
        return pane;
    }

    /**
     * 创建借阅管理面板
     */
    private VBox createBorrowPane() {
        GridPane form = new GridPane();
        form.setHgap(10);
        form.setVgap(10);
        form.setPadding(new Insets(15));

        TextField isbnField = new TextField();
        isbnField.setPromptText("输入图书 ISBN");
        TextField readerField = new TextField();
        readerField.setPromptText("输入读者姓名");

        form.add(new Label("图书 ISBN:"), 0, 0);
        form.add(isbnField, 1, 0);
        form.add(new Label("读者姓名:"), 0, 1);
        form.add(readerField, 1, 1);

        Button borrowBtn = new Button("借书");
        Button returnBtn = new Button("还书");

        borrowBtn.setOnAction(e -> {
            try {
                system.borrowBook(isbnField.getText().trim(), readerField.getText().trim());
                showAlert(Alert.AlertType.INFORMATION, "借阅成功！");
                refreshBookTable();
                refreshRecordTable();
            } catch (LibraryException ex) {
                showAlert(Alert.AlertType.ERROR, ex.getMessage());
            }
        });

        returnBtn.setOnAction(e -> {
            try {
                system.returnBook(isbnField.getText().trim(), readerField.getText().trim());
                showAlert(Alert.AlertType.INFORMATION, "归还成功！");
                refreshBookTable();
                refreshRecordTable();
            } catch (LibraryException ex) {
                showAlert(Alert.AlertType.ERROR, ex.getMessage());
            }
        });

        HBox btnBox = new HBox(10, borrowBtn, returnBtn);
        form.add(btnBox, 1, 2);

        VBox pane = new VBox(20, form);
        pane.setPadding(new Insets(10));
        return pane;
    }

    /**
     * 创建借阅记录面板
     */
    @SuppressWarnings("unchecked")
    private VBox createRecordPane() {
        recordTable = new TableView<>();
        recordData = FXCollections.observableArrayList(system.getUnreturnedRecords());

        TableColumn<BorrowRecord, String> rNameCol = new TableColumn<>("读者");
        rNameCol.setCellValueFactory(new PropertyValueFactory<>("readerName"));
        TableColumn<BorrowRecord, String> rBookCol = new TableColumn<>("书名");
        rBookCol.setCellValueFactory(new PropertyValueFactory<>("bookTitle"));
        TableColumn<BorrowRecord, String> rIsbnCol = new TableColumn<>("ISBN");
        rIsbnCol.setCellValueFactory(new PropertyValueFactory<>("bookIsbn"));
        TableColumn<BorrowRecord, String> rDateCol = new TableColumn<>("借阅日期");
        rDateCol.setCellValueFactory(new PropertyValueFactory<>("borrowDate"));
        TableColumn<BorrowRecord, Boolean> rStatusCol = new TableColumn<>("已归还");
        rStatusCol.setCellValueFactory(new PropertyValueFactory<>("returned"));

        recordTable.getColumns().addAll(rNameCol, rBookCol, rIsbnCol, rDateCol, rStatusCol);
        recordTable.setColumnResizePolicy(TableView.CONSTRAINED_RESIZE_POLICY);
        recordTable.setItems(recordData);

        HBox btnBox = new HBox(10);
        btnBox.setPadding(new Insets(10));
        Button showUnreturned = new Button("未归还记录");
        showUnreturned.setOnAction(e -> recordData.setAll(system.getUnreturnedRecords()));
        Button showAll = new Button("全部记录");
        showAll.setOnAction(e -> recordData.setAll(system.getAllRecords()));
        btnBox.getChildren().addAll(showUnreturned, showAll);

        VBox pane = new VBox(10, recordTable, btnBox);
        pane.setPadding(new Insets(10));
        VBox.setVgrow(recordTable, Priority.ALWAYS);
        return pane;
    }

    // =================== 对话框方法 ===================

    private void showAddBookDialog() {
        Dialog<Book> dialog = new Dialog<>();
        dialog.setTitle("添加图书");
        GridPane grid = new GridPane();
        grid.setHgap(10); grid.setVgap(10); grid.setPadding(new Insets(20));

        TextField titleF = new TextField(); titleF.setPromptText("书名");
        TextField authorF = new TextField(); authorF.setPromptText("作者");
        TextField isbnF = new TextField(); isbnF.setPromptText("ISBN");
        TextField pubF = new TextField(); pubF.setPromptText("出版社");
        TextField stockF = new TextField(); stockF.setPromptText("库存数量");

        grid.add(new Label("书名:"), 0, 0); grid.add(titleF, 1, 0);
        grid.add(new Label("作者:"), 0, 1); grid.add(authorF, 1, 1);
        grid.add(new Label("ISBN:"), 0, 2); grid.add(isbnF, 1, 2);
        grid.add(new Label("出版社:"), 0, 3); grid.add(pubF, 1, 3);
        grid.add(new Label("库存:"), 0, 4); grid.add(stockF, 1, 4);

        dialog.getDialogPane().setContent(grid);
        ButtonType addBtn = new ButtonType("添加", ButtonBar.ButtonData.OK_DONE);
        dialog.getDialogPane().getButtonTypes().addAll(addBtn, ButtonType.CANCEL);

        dialog.setResultConverter(btn -> {
            if (btn == addBtn) {
                try {
                    int stock = Integer.parseInt(stockF.getText().trim());
                    return new Book(titleF.getText().trim(), authorF.getText().trim(),
                                    isbnF.getText().trim(), pubF.getText().trim(), stock);
                } catch (NumberFormatException ex) {
                    showAlert(Alert.AlertType.ERROR, "库存必须为数字");
                    return null;
                }
            }
            return null;
        });

        Optional<Book> result = dialog.showAndWait();
        result.ifPresent(book -> {
            try {
                system.addBook(book);
                refreshBookTable();
                showAlert(Alert.AlertType.INFORMATION, "添加成功！");
            } catch (LibraryException ex) {
                showAlert(Alert.AlertType.ERROR, ex.getMessage());
            }
        });
    }

    private void handleDeleteBook() {
        Book selected = bookTable.getSelectionModel().getSelectedItem();
        if (selected == null) {
            showAlert(Alert.AlertType.WARNING, "请先选择要删除的图书");
            return;
        }
        Alert confirm = new Alert(Alert.AlertType.CONFIRMATION,
            "确认删除《" + selected.getTitle() + "》？", ButtonType.YES, ButtonType.NO);
        confirm.showAndWait().ifPresent(btn -> {
            if (btn == ButtonType.YES) {
                try {
                    system.deleteBook(selected.getIsbn());
                    refreshBookTable();
                } catch (LibraryException ex) {
                    showAlert(Alert.AlertType.ERROR, ex.getMessage());
                }
            }
        });
    }

    private void showEditBookDialog() {
        Book selected = bookTable.getSelectionModel().getSelectedItem();
        if (selected == null) {
            showAlert(Alert.AlertType.WARNING, "请先选择要修改的图书");
            return;
        }
        Dialog<Book> dialog = new Dialog<>();
        dialog.setTitle("修改图书");
        GridPane grid = new GridPane();
        grid.setHgap(10); grid.setVgap(10); grid.setPadding(new Insets(20));

        // 预填充当前值
        TextField titleF = new TextField(selected.getTitle());
        TextField authorF = new TextField(selected.getAuthor());
        TextField isbnF = new TextField(selected.getIsbn());
        isbnF.setEditable(false); // ISBN 不可修改
        isbnF.setStyle("-fx-background-color: #F1F3F4;");
        TextField pubF = new TextField(selected.getPublisher());
        TextField stockF = new TextField(String.valueOf(selected.getStock()));

        grid.add(new Label("书名:"), 0, 0); grid.add(titleF, 1, 0);
        grid.add(new Label("作者:"), 0, 1); grid.add(authorF, 1, 1);
        grid.add(new Label("ISBN:"), 0, 2); grid.add(isbnF, 1, 2);
        grid.add(new Label("出版社:"), 0, 3); grid.add(pubF, 1, 3);
        grid.add(new Label("库存:"), 0, 4); grid.add(stockF, 1, 4);

        dialog.getDialogPane().setContent(grid);
        ButtonType saveBtn = new ButtonType("保存", ButtonBar.ButtonData.OK_DONE);
        dialog.getDialogPane().getButtonTypes().addAll(saveBtn, ButtonType.CANCEL);

        dialog.setResultConverter(btn -> {
            if (btn == saveBtn) {
                try {
                    int stock = Integer.parseInt(stockF.getText().trim());
                    return new Book(titleF.getText().trim(), authorF.getText().trim(),
                                    isbnF.getText().trim(), pubF.getText().trim(), stock);
                } catch (NumberFormatException ex) {
                    showAlert(Alert.AlertType.ERROR, "库存必须为数字");
                    return null;
                }
            }
            return null;
        });

        Optional<Book> result = dialog.showAndWait();
        result.ifPresent(book -> {
            try {
                system.updateBook(book.getIsbn(), book.getTitle(), book.getAuthor(),
                                  book.getPublisher(), book.getStock());
                refreshBookTable();
                showAlert(Alert.AlertType.INFORMATION, "修改成功！");
            } catch (LibraryException ex) {
                showAlert(Alert.AlertType.ERROR, ex.getMessage());
            }
        });
    }

    // =================== 工具方法 ===================

    private void refreshBookTable() {
        if (bookData != null) {
            bookData.setAll(system.getAllBooks());
        }
    }

    private void refreshRecordTable() {
        if (recordData != null) {
            recordData.setAll(system.getUnreturnedRecords());
        }
    }

    private void showAlert(Alert.AlertType type, String message) {
        Alert alert = new Alert(type, message);
        alert.showAndWait();
    }
}
```

### 3.9 Main.java — 程序入口

```java
package com.library;

import com.library.ui.LibraryUI;
import javafx.application.Application;

/**
 * 程序入口
 */
public class Main {
    public static void main(String[] args) {
        Application.launch(LibraryUI.class, args);
    }
}
```

---

## 四、集合框架使用说明（面试/检查必问）

| 集合类型 | 使用位置 | 选择理由 |
|----------|----------|----------|
| `HashMap<String, Book>` | LibrarySystem.bookMap | ISBN作为key，O(1)查找；借阅时快速验证图书是否存在 |
| `ArrayList<Book>` | FileUtil 返回值 / 表格数据 | 顺序存储，支持随机访问，适合展示列表 |
| `LinkedList<BorrowRecord>` | LibrarySystem.borrowRecords | 借阅记录频繁在尾部添加，LinkedList 的 add 操作为 O(1) |
| `ObservableList` (JavaFX) | UI 表格绑定 | JavaFX 数据绑定机制，数据变化自动刷新界面 |

---

## 五、异常处理汇总

| 场景 | 异常类型 | 处理方式 |
|------|----------|----------|
| ISBN 为空 | LibraryException | 弹窗提示 |
| ISBN 重复 | LibraryException | 弹窗提示 |
| 库存不足 | LibraryException | 弹窗提示 |
| 重复借阅 | LibraryException | 弹窗提示 |
| 库存输入非数字 | NumberFormatException | try-catch + 弹窗提示 |
| 文件读取失败 | IOException | try-catch + 日志输出 |
| 文件行格式错误 | IllegalArgumentException | 跳过该行 + 警告日志 |

---

## 六、文件存储格式

**books.txt** 示例：
```
Java编程思想|Bruce Eckel|978-7-111-21382-6|机械工业出版社|5
数据结构与算法|严蔚敏|978-7-302-33064-6|清华大学出版社|3
```

**records.txt** 示例：
```
张三|978-7-111-21382-6|Java编程思想|2025-05-15|0
李四|978-7-302-33064-6|数据结构与算法|2025-05-10|1
```

（最后一列：0=未归还，1=已归还）

---

### 🧠 最终设计思考与演进闭环

## 七、界面设计方案

### 7.1 界面整体架构与控件级参数

本系统全面放弃 FXML，采用纯 Java 代码（Pure Java API）进行界面布局与动态构建。通过顶层容器的嵌套组合，实现响应式与层次分明的结构。

#### 7.1.1 全局视觉架构

主窗口采用经典的“顶部状态栏 + 中央核心工作区”的响应式垂直布局：

- **顶层根容器**：`VBox` (作为根 Scene 的容器，无 padding)。
- **状态栏 (Status Bar)**：位于顶部的 `HBox`。内部包含左侧当前用户信息 Label 和右侧系统时间/版本 Label，两侧利用 `Pane` 进行弹簧式平铺分隔（`HBox.setHgrow(spacer, Priority.ALWAYS)`）。
- **主内容区**：承载 `TabPane`，其高度和宽度自动填充父级剩余空间。

#### 7.1.2 各模块布局流向与精确尺寸

系统各主界面的标准组件尺寸、间距（Padding/Spacing）以及网格间隙（Gap）严格规范如下表，以确保整体视觉体验的一致性：

| **界面模块**        | **布局容器层次 (Layout Hierarchy)**    | **关键布局参数 (px)**                                        | **控件推荐尺寸 (prefWidth / prefHeight)**                    |
| ------------------- | -------------------------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| **登录弹窗**        | `Dialog` → `DialogPane` → `GridPane`   | `padding: 24` `hgap: 12`, `vgap: 16`                         | 输入框/角色ComboBox: `220 × 36` 确认/取消按钮: `90 × 32`     |
| **Tab 1: 图书管理** | `VBox` → `TableView` + `HBox (操作栏)` | `padding: 16` `spacing: 16`                                  | `TableView`: `760 × 400` 增/删/改/刷新按钮: `100 × 36`       |
| **Tab 2: 图书查询** | `VBox` → `HBox (搜索栏)` + `TableView` | `padding: 16` `spacing: 16`                                  | 检索条件 ComboBox: `120 × 36` 搜索框: `260 × 36` 搜索按钮: `80 × 36` |
| **Tab 3: 借阅管理** | `VBox` → `GridPane (业务表单卡片)`     | `VBox padding: 40` `GridPane padding: 24` `hgap: 16`, `vgap: 20` | 登记输入框 (ISBN/读者): `280 × 36` 借书/还书核心按钮: `130 × 40` |
| **Tab 4: 借阅记录** | `VBox` → `TableView` + `HBox (过滤栏)` | `padding: 16` `spacing: 16`                                  | `TableView`: `760 × 400` 视图切换单选按钮: `120 × 32`        |
| **通用增改弹窗**    | `Dialog` → `DialogPane` → `GridPane`   | `padding: 20` `hgap: 10`, `vgap: 14`                         | 数据输入框: `240 × 34`                                       |

### 7.2 核心 UI 控制逻辑实现规范

#### 7.2.1 安全而优雅的动态按钮权限控制

在 `LibraryUI` 的初始化流程中，系统不采用传统的“根据权限动态添加/移除子节点（`getChildren().add/remove`）”的繁琐逻辑。统一采用 `setVisible` 与 `setManaged` 联动组合拳。当非管理员（普通读者）登录时，UI 保持节点树的稳定性，通过让出物理空间实现界面的完美自适应重排：

Java

```
// 角色权限动态控制核心逻辑示例
if ("普通读者".equals(currentUserRole)) {
    // 隐藏管理员专属操作按钮
    addButton.setVisible(false);
    addButton.setManaged(false); // 让出布局流空间，防止界面留白空秃
    
    deleteButton.setVisible(false);
    deleteButton.setManaged(false);
    
    updateButton.setVisible(false);
    updateButton.setManaged(false);
    
    // 此时底部的 HBox 操作栏将只保留并自动靠左对齐 "刷新列表" 按钮
}
```

#### 7.2.2 自定义样式类的手动绑定机制

为了在界面中拉开视觉层级，主行动按钮（如添加、修改、借还书）直接继承默认的 `.button` 高亮纯蓝样式。而对于“刷新列表”、“重置”等次要辅助按钮，必须在 Java 代码中手动将其添加到自定义样式类 `button-secondary` 中：

Java

```
// 辅助按钮样式手动绑定示例
Button refreshBtn = new Button("刷新列表");
// 手动追加自定义样式类，使其应用白底蓝字的次要按钮风格
refreshBtn.getStyleClass().add("button-secondary"); 
```

#### 7.2.3 稳妥的样式表资源加载方案

为了规避由于不同操作系统（Windows / Linux）或打包成 JAR 后引起的相对路径失效问题，所有界面的 CSS 加载统一从类路径资源根目录（`resources`）进行绝对路径检索，并辅以空指针异常保护：

Java

```
// 在主舞台或 Scene 初始化时标准引入方法
try {
    String css = getClass().getResource("/theme.css").toExternalForm();
    scene.getStylesheets().add(css);
} catch (NullPointerException e) {
    System.err.println("警告：未在 resources 根目录下找到 theme.css 配置文件！系统将降级使用默认 Modena 样式。");
}
```

#### 7.2.4 TableView 智能列宽自动平铺逻辑

为了配合 UI 整体的现代极简风格，防止表格内部出现多余的灰色空白区域或横向滚动条，每个 `TableView` 在实例化时均需要强制绑定列宽自适应策略：

Java

```
// 确保表格列百分比平铺、无右侧死角的关键逻辑
tableView.setColumnResizePolicy(TableView.CONSTRAINED_RESIZE_POLICY);
```

### 7.3 完整生产级 CSS 视觉样式表 (theme.css)

以下样式文件需放置在项目的 `src/main/resources/theme.css`（或标准的资源根目录）下。该方案移除了不支持的 transition 属性，确保在纯 JavaFX 环境下拥有完美的渲染效率、斑马纹效果、圆角控件及高精度的色调融合。

CSS

```
/* ==========================================================================
   📚 图书管理系统 - 现代扁平化主样式表 (最终修正版)
   ========================================================================== */

/* 7.3.1 基础根节点与全局字体 */
.root {
    -fx-font-family: "Microsoft YaHei", "Segoe UI", sans-serif;
    -fx-font-size: 14px;
    -fx-background-color: #F8F9FA; /* 极简灰白背景 */
    -fx-text-fill: #202124;
}

/* 7.3.2 顶部状态栏样式 */
.status-bar {
    -fx-background-color: #1A73E8; /* 现代高雅蓝（深海蓝体系主色） */
    -fx-background-radius: 0;
}
.status-bar .label {
    -fx-text-fill: white;
    -fx-font-weight: bold;
}

/* 7.3.3 TabPane 选项卡美化 */
.tab-pane {
    -fx-tab-min-height: 40px;
    -fx-tab-max-height: 40px;
}
.tab-pane .tab-header-area {
    -fx-padding: 0;
    -fx-background-color: #FFFFFF;
    -fx-border-color: transparent transparent #DADCE0 transparent;
    -fx-border-width: 1;
}
.tab {
    -fx-background-color: transparent;
    -fx-background-insets: 0;
    -fx-padding: 0 24 0 24;
    -fx-cursor: hand;
}
.tab .tab-label {
    -fx-text-fill: #5F6368; /* 次要暗灰文字 */
    -fx-font-weight: bold;
}
.tab:selected {
    -fx-background-color: transparent;
    -fx-border-color: transparent transparent #1A73E8 transparent;
    -fx-border-width: 0 0 3 0; /* 选中时底部的蓝色高亮条 */
}
.tab:selected .tab-label {
    -fx-text-fill: #1A73E8;
}
.tab:hover {
    -fx-background-color: #E8F0FE; /* 浅蓝悬浮反馈 */
}

/* 7.3.4 按钮通用样式与状态 */
.button {
    -fx-background-color: #1A73E8;
    -fx-text-fill: white;
    -fx-font-weight: bold;
    -fx-background-radius: 6; /* 扁平化圆角 */
    -fx-cursor: hand;
    -fx-padding: 6 16 6 16;
}
.button:hover {
    -fx-background-color: #1557B0; /* 悬浮时加深蓝色 */
    -fx-effect: dropshadow(three-pass-box, rgba(26,115,232,0.3), 6, 0, 0, 2); /* 悬浮微阴影 */
}
.button:pressed {
    -fx-background-color: #0B3C80; /* 点击态 */
}

/* 次要/辅助/刷新按钮样式（通过 Java 手动绑定 .button-secondary 激活） */
.button-secondary {
    -fx-background-color: #FFFFFF;
    -fx-text-fill: #1A73E8;
    -fx-border-color: #1A73E8;
    -fx-border-width: 1;
    -fx-border-radius: 6;
}
.button-secondary:hover {
    -fx-background-color: #E8F0FE;
    -fx-effect: none;
}

/* 7.3.5 输入控件 (TextField, PasswordField & ComboBox) */
.text-field {
    -fx-background-color: #FFFFFF;
    -fx-background-radius: 6;
    -fx-border-color: #DADCE0;
    -fx-border-width: 1;
    -fx-border-radius: 6;
    -fx-padding: 6 10 6 10;
    -fx-text-fill: #202124;
}
.text-field:focused {
    -fx-border-color: #1A73E8;
    -fx-background-color: #FFFFFF;
    -fx-effect: dropshadow(three-pass-box, rgba(26,115,232,0.1), 4, 0, 0, 0);
}
.combo-box {
    -fx-background-color: #FFFFFF;
    -fx-background-radius: 6;
    -fx-border-color: #DADCE0;
    -fx-border-width: 1;
    -fx-border-radius: 6;
    -fx-padding: 2 4 2 4;
}
.combo-box:focused {
    -fx-border-color: #1A73E8;
}

/* 7.3.6 现代表格 (TableView) 斑马纹与表头 */
.table-view {
    -fx-background-color: transparent;
    -fx-border-color: #DADCE0;
    -fx-border-width: 1;
    -fx-border-radius: 6;
    -fx-background-radius: 6;
}
.table-view .column-header-background {
    -fx-background-color: #F1F3F4; /* 表头浅灰背景 */
}
.table-view .column-header, .table-view .filler {
    -fx-background-color: transparent;
    -fx-border-color: transparent #DADCE0 transparent transparent;
}
.table-view .column-header .label {
    -fx-text-fill: #202124;
    -fx-font-weight: bold;
    -fx-padding: 10 0 10 0;
}
/* 数据行交替变色（斑马纹） */
.table-row-cell {
    -fx-background-color: #FFFFFF;
    -fx-border-color: transparent transparent #F1F3F4 transparent;
}
.table-row-cell:odd {
    -fx-background-color: #F8F9FA; /* 奇数行浅灰，形成交替斑马纹 */
}
.table-row-cell:selected {
    -fx-background-color: #E8F0FE; /* 选中行柔和高亮蓝 */
}
.table-row-cell:selected .text {
    -fx-fill: #1A73E8;
    -fx-font-weight: bold;
}

/* 7.3.7 弹窗面板 (DialogPane) */
.dialog-pane {
    -fx-background-color: #FFFFFF;
    -fx-border-radius: 8;
    -fx-background-radius: 8;
}
.dialog-pane .header-panel {
    -fx-background-color: #F8F9FA;
    -fx-padding: 16;
}
.dialog-pane .label {
    -fx-text-fill: #202124;
}
```

---

## 八、Codex 执行指令

将以下指令发送给 Codex，它可以基于本文档生成完整项目：

> 请基于本文档创建一个完整的 JavaFX 图书管理系统项目。
>
> **项目要求**：
> 1. 包名：`com.library`，按照文档的项目结构组织代码
> 2. 完整实现文档中所有类（Book, User, Reader, Admin, BorrowRecord, LibrarySystem, FileUtil, LibraryException, LibraryUI, Main）
> 3. LibraryUI 中所有标注"省略"的方法需要补全（如 showEditBookDialog、createSearchPane 的完整列定义）
> 4. 添加 CSS 样式美化界面（参考 Gemini 方案或自行设计）
> 5. 确保 IO 操作只使用 FileReader/FileWriter/BufferedReader/BufferedWriter
> 6. 每个类、每个方法添加 Javadoc 注释
> 7. 所有异常场景必须有 try-catch 处理
> 8. 在 books.txt 中预置 5 条测试图书数据
> 9. 窗口关闭时自动保存数据（已有 setOnCloseRequest）
> 10. 测试以下流程：添加图书 → 查询图书 → 借阅 → 归还 → 查看记录

---

## 九、UML 类图（文档提交用）

```
┌────────────────────┐
│    <<abstract>>     │
│       User          │
├────────────────────┤
│ # username: String  │
│ # password: String  │
├────────────────────┤
│ + getRole(): String │          ┌──────────────────┐
│ + hasPermission()   │          │      Book         │
│   : boolean         │          ├──────────────────┤
└──────┬───┬─────────┘          │ - title: String   │
       │   │                     │ - author: String  │
       │   │                     │ - isbn: String    │
  ┌────┘   └────┐               │ - publisher: Str  │
  ▼              ▼               │ - stock: int      │
┌────────┐  ┌────────┐          ├──────────────────┤
│ Reader │  │ Admin  │          │ + toFileString()  │
├────────┤  ├────────┤          │ + fromFileString()│
│        │  │        │          └──────────────────┘
└────────┘  └────────┘
                                 ┌──────────────────┐
┌────────────────────┐          │  BorrowRecord     │
│  LibrarySystem     │          ├──────────────────┤
├────────────────────┤          │ - readerName      │
│ - bookMap: HashMap │◆────────▶│ - bookIsbn        │
│ - borrowRecords:   │          │ - bookTitle       │
│   LinkedList       │          │ - borrowDate      │
├────────────────────┤          │ - returned        │
│ + addBook()        │          └──────────────────┘
│ + deleteBook()     │
│ + borrowBook()     │          ┌──────────────────┐
│ + returnBook()     │          │    FileUtil       │
│ + searchByTitle()  │          ├──────────────────┤
│ + searchByIsbn()   │          │ + loadBooks()     │
│ + saveAll()        │          │ + saveBooks()     │
└────────────────────┘          │ + loadRecords()   │
                                │ + saveRecords()   │
┌────────────────────┐          └──────────────────┘
│  LibraryException  │
│  extends Exception │
└────────────────────┘
```

---

## 十、提交检查清单

- [ ] 完整工程源代码（com.library 包结构），打包为 .zip
- [ ] 4 张运行截图：图书管理 / 图书查询 / 借阅管理 / 借阅记录
- [ ] Word/PDF 文档包含：① 类图（见第九节） ② 核心功能说明 ③ 遇到的问题及解决方法
- [ ] books.txt 和 records.txt 能正确读写
- [ ] 关闭程序后重新打开，数据不丢失
- [ ] 能清晰回答：OOP 设计（封装继承多态在哪里）、集合选择理由、异常处理思路、IO 流用法
