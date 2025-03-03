#pragma once

#ifndef SHABBY_DATABASE_EXPORTS
#define SHABBY_CLASS __declspec(dllimport)
#define SHABBY_TEMPLATE
#else //EXPORT
#define SHABBY_CLASS __declspec(dllexport)
#define SHABBY_TEMPLATE __declspec(dllexport)
#endif

#ifndef DATABASE_H
#define DATABASE_H

#include <mysql.h>
#include <string>
#include <mutex>
#include <stdexcept>
#include <iostream>
#include <optional>
#include <chrono>
#include <thread>
#include <atomic>

class SHABBY_CLASS MySQLDatabase
{
public:
    // ��ȡ����ʵ��
    static MySQLDatabase& GetInstance();

    // ��ʼ�����ݿ�����
    void Init(const std::string& host, const std::string& user, const std::string& password, const std::string& dbname);

    // �Ͽ����Ӳ�ֹͣ����
    void Disconnect();

    // ִ�в�ѯ
    std::optional<MYSQL_RES*> Query(const std::string& query_str);

    // ִ�������ġ�ɾ����
    bool Execute(const std::string& query_str);

    // ����Ƿ�����
    bool IsConnected();

    // �������ݿ��� 
    MYSQL* GetConnection();

private:
    MySQLDatabase() : m_conn(nullptr), m_heartbeatActive(false) {}
    ~MySQLDatabase() { Disconnect(); }
    // ɾ���������캯���͸�ֵ���������ȷ������ģʽ
    MySQLDatabase(const MySQLDatabase&) = delete;
    MySQLDatabase& operator=(const MySQLDatabase&) = delete;

    // ���Ӳ�����ֱ���ɹ�
    void ConnectWithRetry();

    // �����߳��߼�
    void heartbeatCheck();

    MYSQL* m_conn;
    std::string m_host, m_user, m_password, m_dbname;
    std::mutex m_mutex; // �������ݿ����ӵ��̰߳�ȫ

    std::atomic<bool> m_heartbeatActive; // ���������̵߳�����״̬
    std::thread m_heartbeatThread;       // �����߳�
};

#endif // DATABASE_H
