#include "DataBase.h"

MySQLDatabase& MySQLDatabase::GetInstance()
{
    static MySQLDatabase instance;
    return instance;
}

void MySQLDatabase::Init(const std::string& host, const std::string& user, const std::string& password, const std::string& dbname)
{
    m_host = host;
    m_user = user;
    m_password = password;
    m_dbname = dbname;

    // 建立初始连接，确保成功连接
    ConnectWithRetry();

    // 启动心跳线程
    m_heartbeatActive = true;
    m_heartbeatThread = std::thread([this]() { heartbeatCheck(); });
}

void MySQLDatabase::Disconnect()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (m_conn)
        {
            mysql_close(m_conn);
            m_conn = nullptr;
        }
    }

    // 停止心跳线程
    m_heartbeatActive = false;
    if (m_heartbeatThread.joinable())
    {
        m_heartbeatThread.join();
    }
}

std::optional<MYSQL_RES*> MySQLDatabase::Query(const std::string& query_str)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (!m_conn)
    {
        return std::nullopt;
    }

    if (mysql_query(m_conn, query_str.c_str()))
    {
        std::cerr << "Query failed: " << mysql_error(m_conn) << std::endl;
        return std::nullopt;
    }

    return mysql_store_result(m_conn);
}

bool MySQLDatabase::Execute(const std::string& query_str)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (!m_conn)
    {
        return false;
    }

    if (mysql_query(m_conn, query_str.c_str()))
    {
        std::cerr << "Execute failed: " << mysql_error(m_conn) << std::endl;
        return false;
    }

    return true;
}

bool MySQLDatabase::IsConnected()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_conn && mysql_ping(m_conn) == 0;
}

MYSQL* MySQLDatabase::GetConnection()
{
    std::lock_guard<std::mutex> lock(m_mutex); // 确保线程安全
    return m_conn;
}

void MySQLDatabase::ConnectWithRetry()
{
    while (true)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (m_conn)
        {
            mysql_close(m_conn);
        }

        m_conn = mysql_init(nullptr);
        if (m_conn && mysql_real_connect(m_conn, m_host.c_str(), m_user.c_str(), m_password.c_str(), m_dbname.c_str(), 0, nullptr, 0))
        {
            std::cout << "Connected to the database successfully!" << std::endl;
            return;
        }

        std::cerr << "Connection failed: " << (m_conn ? mysql_error(m_conn) : "mysql_init failed") << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3)); // 每3秒重试
    }
}

void MySQLDatabase::heartbeatCheck()
{
    while (m_heartbeatActive)
    {
        std::this_thread::sleep_for(std::chrono::minutes(1)); // 每1分钟检查一次连接
        if (!IsConnected())
        {
            std::cerr << "Heartbeat check failed. Attempting to reconnect..." << std::endl;
            ConnectWithRetry();
        }
    }
}
