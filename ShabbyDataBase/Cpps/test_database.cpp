#include "DataBase.h"
#include <iostream>



int main() {
	// 启动测试
	MySQLDatabase& db = MySQLDatabase::GetInstance();
	db.Init("localhost", "root", "123456", "meta_graph_model");

    // 插入 ModelBluePrint 数据
    std::string insertModelQuery = "INSERT INTO ModelBluePrint (name) VALUES ('Model_A'), ('Model_B');";
    if (db.Execute(insertModelQuery)) {
        std::cout << "Insert ModelBluePrint operation successful." << std::endl;
    }
    else {
        std::cerr << "Insert ModelBluePrint operation failed." << std::endl;
    }

    // 查询 ModelBluePrint 数据
    std::string selectModelQuery = "SELECT * FROM ModelBluePrint;";
    auto modelResult = db.Query(selectModelQuery);
    if (modelResult) {
        MYSQL_RES* res = modelResult.value();
        MYSQL_ROW row;

        std::cout << "ModelBluePrint Query Results:" << std::endl;
        while ((row = mysql_fetch_row(res)) != nullptr) {
            std::cout << "Model ID: " << row[0] << ", Name: " << row[1] << std::endl;
        }

        mysql_free_result(res);
    }
    else {
        std::cerr << "Query ModelBluePrint operation failed." << std::endl;
    }

    // 插入 FilePath 数据
    std::string insertFilePathQuery =
        "INSERT INTO FilePath (model_id, file_path) VALUES (1001, '/path/to/model_a/file1'), (1002, '/path/to/model_a/file2'), (1003, '/path/to/model_b/file1');";
    if (db.Execute(insertFilePathQuery)) {
        std::cout << "Insert FilePath operation successful." << std::endl;
    }
    else {
        std::cerr << "Insert FilePath operation failed." << std::endl;
    }

    // 查询 FilePath 数据
    std::string selectFilePathQuery = "SELECT * FROM FilePath;";
    auto filePathResult = db.Query(selectFilePathQuery);
    if (filePathResult) {
        MYSQL_RES* res = filePathResult.value();
        MYSQL_ROW row;

        std::cout << "FilePath Query Results:" << std::endl;
        while ((row = mysql_fetch_row(res)) != nullptr) {
            std::cout << "File ID: " << row[0] << ", Model ID: " << row[1] << ", File Path: " << row[2] << ", Last Updated: " << row[3] << std::endl;
        }

        mysql_free_result(res);
    }
    else {
        std::cerr << "Query FilePath operation failed." << std::endl;
    }

    // 更新 FilePath 数据
    std::string updateFilePathQuery = "UPDATE FilePath SET file_path = '/updated/path/to/file1_changed' WHERE file_id = 1001;";
    if (db.Execute(updateFilePathQuery)) {
        std::cout << "Update FilePath operation successful." << std::endl;
    }
    else {
        std::cerr << "Update FilePath operation failed." << std::endl;
    }

    // 删除 ModelBluePrint 数据（级联删除 FilePath）
    std::string deleteModelQuery = "DELETE FROM ModelBluePrint WHERE name = 'Model_B';";
    if (db.Execute(deleteModelQuery)) {
        std::cout << "Delete ModelBluePrint operation successful." << std::endl;
    }
    else {
        std::cerr << "Delete ModelBluePrint operation failed." << std::endl;
    }

    // 查询 FilePath 数据以验证级联删除
    auto filePathResultAfterDelete = db.Query(selectFilePathQuery);
    if (filePathResultAfterDelete) {
        MYSQL_RES* res = filePathResultAfterDelete.value();
        MYSQL_ROW row;

        std::cout << "FilePath Query Results After Delete:" << std::endl;
        while ((row = mysql_fetch_row(res)) != nullptr) {
            std::cout << "File ID: " << row[0] << ", Model ID: " << row[1] << ", File Path: " << row[2] << ", Last Updated: " << row[3] << std::endl;
        }

        mysql_free_result(res);
    }
    else {
        std::cerr << "Query FilePath operation after delete failed." << std::endl;
    }

    // 断开数据库连接
    db.Disconnect();
}
