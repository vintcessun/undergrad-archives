import java.sql.*;

public class OpenGaussDemo {

    static final String DB_URL =
            "jdbc:postgresql://113.44.178.91:26000/demo";

    static final String USER = "dbuser";
    static final String PASS = "GaussDB@123";
    
    public static void main(String[] args) {

        try {

            System.out.println("连接数据库...");

            Connection conn =
                    DriverManager.getConnection(
                            DB_URL,
                            USER,
                            PASS);

            System.out.println("实例化 Statement 对象...");

            Statement stmt =
                    conn.createStatement();

            String sql =
                    "SELECT id,name,url FROM demo.websites";

            ResultSet rs =
                    stmt.executeQuery(sql);

            while (rs.next()) {

                int id =
                        rs.getInt("id");

                String name =
                        rs.getString("name");

                String url =
                        rs.getString("url");

                System.out.println(
                        "ID: " + id +
                        ", 站点名称: " + name +
                        ", 站点 URL: " + url);
            }

            rs.close();
            stmt.close();
            conn.close();

        } catch (Exception e) {

            e.printStackTrace();
        }

        System.out.println("Goodbye!");
    }
}