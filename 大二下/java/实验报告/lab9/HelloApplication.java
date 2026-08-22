package com.example.demo;

import javafx.application.Application;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.collections.transformation.FilteredList;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

public class HelloApplication extends Application {

    private TableView<Student> studentTable;
    private ObservableList<Student> masterData;
    private FilteredList<Student> filteredData;

    // 查询字段
    private TextField searchIdField;
    private TextField searchPhoneField;
    private TextField searchClassField;
    private ComboBox<String> searchPoliticsCombo;

    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage primaryStage) {
        primaryStage.setTitle("学生管理系统");

        // 创建菜单栏左侧面板
        VBox menuPanel = createMenuPanel();

        // 创建查询面板
        GridPane searchPanel = createSearchPanel();

        // 创建学生表格
        studentTable = createStudentTable();

        // 初始化数据 (模拟一些学生)
        masterData = FXCollections.observableArrayList();
        loadSampleData();

        // 包装为FilteredList用于动态过滤
        filteredData = new FilteredList<>(masterData, p -> true);
        studentTable.setItems(filteredData);

        // 布局整合
        BorderPane root = new BorderPane();
        root.setLeft(menuPanel);

        VBox centerContent = new VBox(10);
        centerContent.setPadding(new Insets(10));
        centerContent.getChildren().addAll(searchPanel, studentTable);
        root.setCenter(centerContent);

        Scene scene = new Scene(root, 900, 600);
        primaryStage.setScene(scene);
        primaryStage.show();

        // 绑定查询事件
        setupSearchFilters();
    }

    private VBox createMenuPanel() {
        VBox menuBox = new VBox(15);
        menuBox.setPadding(new Insets(20, 10, 20, 10));
        menuBox.setStyle("-fx-background-color: #f0f0f0; -fx-border-color: #cccccc; -fx-border-width: 0 1 0 0;");

        Label titleLabel = new Label("系统菜单");
        titleLabel.setStyle("-fx-font-size: 18px; -fx-font-weight: bold;");

        Button studentMgmtBtn = new Button("学生管理");
        studentMgmtBtn.setMaxWidth(Double.MAX_VALUE);
        studentMgmtBtn.setOnAction(e -> showStudentManagement());

        Button courseMgmtBtn = new Button("课程管理");
        courseMgmtBtn.setMaxWidth(Double.MAX_VALUE);
        courseMgmtBtn.setOnAction(e -> showCourseManagement());

        Button enrollmentMgmtBtn = new Button("选课管理");
        enrollmentMgmtBtn.setMaxWidth(Double.MAX_VALUE);
        enrollmentMgmtBtn.setOnAction(e -> showEnrollmentManagement());

        menuBox.getChildren().addAll(titleLabel, studentMgmtBtn, courseMgmtBtn, enrollmentMgmtBtn);
        return menuBox;
    }

    private GridPane createSearchPanel() {
        GridPane grid = new GridPane();
        grid.setHgap(10);
        grid.setVgap(10);
        grid.setPadding(new Insets(10, 0, 10, 0));
        grid.setStyle("-fx-border-color: #ddd; -fx-border-width: 0 0 1 0; -fx-padding: 0 0 10 0;");

        Label idLabel = new Label("学号:");
        searchIdField = new TextField();
        searchIdField.setPromptText("学号");

        Label phoneLabel = new Label("电话:");
        searchPhoneField = new TextField();
        searchPhoneField.setPromptText("电话");

        Label classLabel = new Label("班级:");
        searchClassField = new TextField();
        searchClassField.setPromptText("班级");

        Label politicsLabel = new Label("政治面貌:");
        searchPoliticsCombo = new ComboBox<>();
        searchPoliticsCombo.getItems().addAll("全部", "中共党员", "中共预备党员", "共青团员", "群众");
        searchPoliticsCombo.setValue("全部");

        Button searchBtn = new Button("查询");
        searchBtn.setOnAction(e -> applyFilters());

        Button resetBtn = new Button("重置");
        resetBtn.setOnAction(e -> resetFilters());

        grid.add(idLabel, 0, 0);
        grid.add(searchIdField, 1, 0);
        grid.add(phoneLabel, 2, 0);
        grid.add(searchPhoneField, 3, 0);
        grid.add(classLabel, 4, 0);
        grid.add(searchClassField, 5, 0);
        grid.add(politicsLabel, 0, 1);
        grid.add(searchPoliticsCombo, 1, 1);
        grid.add(searchBtn, 3, 1);
        grid.add(resetBtn, 4, 1);

        return grid;
    }

    private TableView<Student> createStudentTable() {
        TableView<Student> table = new TableView<>();

        TableColumn<Student, String> idCol = new TableColumn<>("学号");
        idCol.setCellValueFactory(new PropertyValueFactory<>("studentId"));
        idCol.setPrefWidth(100);

        TableColumn<Student, String> nameCol = new TableColumn<>("姓名");
        nameCol.setCellValueFactory(new PropertyValueFactory<>("name"));
        nameCol.setPrefWidth(100);

        TableColumn<Student, String> phoneCol = new TableColumn<>("电话");
        phoneCol.setCellValueFactory(new PropertyValueFactory<>("phone"));
        phoneCol.setPrefWidth(120);

        TableColumn<Student, String> classNameCol = new TableColumn<>("班级");
        classNameCol.setCellValueFactory(new PropertyValueFactory<>("className"));
        classNameCol.setPrefWidth(120);

        TableColumn<Student, String> politicsCol = new TableColumn<>("政治面貌");
        politicsCol.setCellValueFactory(new PropertyValueFactory<>("politicsStatus"));
        politicsCol.setPrefWidth(120);

        TableColumn<Student, String> emailCol = new TableColumn<>("邮箱");
        emailCol.setCellValueFactory(new PropertyValueFactory<>("email"));
        emailCol.setPrefWidth(150);

        table.getColumns().addAll(idCol, nameCol, phoneCol, classNameCol, politicsCol, emailCol);
        table.setColumnResizePolicy(TableView.CONSTRAINED_RESIZE_POLICY);

        return table;
    }

    private void loadSampleData() {
        masterData.addAll(
                new Student("2024001", "张三", "13800000001", "计算机1班", "中共党员", "zhangsan@example.com"),
                new Student("2024002", "李四", "13800000002", "计算机1班", "共青团员", "lisi@example.com"),
                new Student("2024003", "王芳", "13800000003", "计算机2班", "中共预备党员", "wangfang@example.com"),
                new Student("2024004", "赵强", "13800000004", "软件工程1班", "群众", "zhaoqiang@example.com"),
                new Student("2024005", "孙丽", "13800000005", "软件工程1班", "中共党员", "sunli@example.com"),
                new Student("2024006", "周杰", "13912345678", "计算机2班", "共青团员", "zhoujie@example.com"),
                new Student("2024007", "吴迪", "13788889999", "计算机1班", "群众", "wudi@example.com")
        );
    }

    private void setupSearchFilters() {
        // 实时监听文本变化进行过滤 (也可由按钮触发，这里由按钮触发更直观，但为了体验保留实时也可以)
        // 为简单统一在按钮点击时调用applyFilters，也可以用listener，这里由按钮统一控制。
    }

    private void applyFilters() {
        String idFilter = searchIdField.getText().trim().toLowerCase();
        String phoneFilter = searchPhoneField.getText().trim().toLowerCase();
        String classFilter = searchClassField.getText().trim().toLowerCase();
        String politicsFilter = searchPoliticsCombo.getValue();

        filteredData.setPredicate(student -> {
            // 学号条件
            if (idFilter != null && !idFilter.isEmpty()) {
                if (!student.getStudentId().toLowerCase().contains(idFilter)) {
                    return false;
                }
            }
            // 电话条件
            if (phoneFilter != null && !phoneFilter.isEmpty()) {
                if (!student.getPhone().toLowerCase().contains(phoneFilter)) {
                    return false;
                }
            }
            // 班级条件
            if (classFilter != null && !classFilter.isEmpty()) {
                if (!student.getClassName().toLowerCase().contains(classFilter)) {
                    return false;
                }
            }
            // 政治面貌条件
            if (politicsFilter != null && !"全部".equals(politicsFilter)) {
                if (!student.getPoliticsStatus().equals(politicsFilter)) {
                    return false;
                }
            }
            return true;
        });
    }

    private void resetFilters() {
        searchIdField.clear();
        searchPhoneField.clear();
        searchClassField.clear();
        searchPoliticsCombo.setValue("全部");
        filteredData.setPredicate(student -> true);
    }

    // 菜单功能占位方法
    private void showStudentManagement() {
        showInfoDialog("学生管理", "点击了学生管理菜单，此处可扩展学生增删改查功能。");
    }

    private void showCourseManagement() {
        showInfoDialog("课程管理", "点击了课程管理菜单，此处可扩展课程管理功能。");
    }

    private void showEnrollmentManagement() {
        showInfoDialog("选课管理", "点击了选课管理菜单，此处可扩展选课管理功能。");
    }

    private void showInfoDialog(String title, String content) {
        Alert alert = new Alert(Alert.AlertType.INFORMATION);
        alert.setTitle(title);
        alert.setHeaderText(null);
        alert.setContentText(content);
        alert.showAndWait();
    }

    // 学生实体类
    public static class Student {
        private final String studentId;
        private final String name;
        private final String phone;
        private final String className;
        private final String politicsStatus;
        private final String email;

        public Student(String studentId, String name, String phone, String className, String politicsStatus, String email) {
            this.studentId = studentId;
            this.name = name;
            this.phone = phone;
            this.className = className;
            this.politicsStatus = politicsStatus;
            this.email = email;
        }

        public String getStudentId() { return studentId; }
        public String getName() { return name; }
        public String getPhone() { return phone; }
        public String getClassName() { return className; }
        public String getPoliticsStatus() { return politicsStatus; }
        public String getEmail() { return email; }
    }
}