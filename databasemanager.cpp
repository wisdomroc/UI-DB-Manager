#include "databasemanager.h"
#include <QDebug>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent)
{
}

DatabaseManager::~DatabaseManager()
{
    if(db.isOpen())
        db.close();
}

bool DatabaseManager::initDatabase(const QString &ip, int port, const QString &database, const QString &username, const QString &password)
{
    QStringList strList = db.drivers();
    qDebug() << "qt supported database names: " << strList;
    qDebug() << "we use QMYSQL" << endl;
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(ip);
    db.setPort(port);
    db.setDatabaseName(database);
    db.setUserName(username);
    db.setPassword(password);
    bool ok = db.open();
    return ok;
}

QStringList DatabaseManager::getTables(const QString &databaseName)
{
    QStringList tableNames;
    QSqlQuery query;
    QString selectStr = tr("select table_name from information_schema.tables where TABLE_SCHEMA='%1'").arg(databaseName);
    bool ok = query.exec(selectStr);
    if(!ok)
    {
        QString errStr = query.lastError().text();
        qDebug() << errStr << endl;
    }
    while (query.next()) {
        QString tableName = query.value(0).toString();
        tableNames.append(tableName);
    }
    return tableNames;
}

QStringList DatabaseManager::getTableColumns(const QString &databaseName, const QString &tableName)
{
    QStringList columnNames;
    QSqlQuery query;
    QString selectStr = tr("SELECT  COLUMN_NAME FROM information_schema.`COLUMNS` WHERE TABLE_SCHEMA='%1' AND TABLE_NAME='%2'").arg(databaseName).arg(tableName);
    bool ok = query.exec(selectStr);
    if(!ok)
    {
        QString errStr = query.lastError().text();
        qDebug() << errStr << endl;
    }
    while (query.next()) {
        QString columnName = query.value(0).toString();
        columnNames.append(columnName);
    }
    return columnNames;
}

QStringList DatabaseManager::getGroupNames(const QString &dbInfos)
{
    QStringList groupNames;
    QSqlQuery query;
    QString selectStr = tr("SELECT  title_name FROM group_info WHERE db_infos=%1").arg(dbInfos);
    bool ok = query.exec(selectStr);
    if(!ok)
    {
        QString errStr = query.lastError().text();
        qDebug() << errStr << endl;
    }
    while (query.next()) {
        QString groupName = query.value(0).toString();
        groupNames.append(groupName);
    }
    return groupNames;
}

//!表group_info列结构---id---db_infos---table_name---table_columns---field_names---title_name
int DatabaseManager::saveGroupInfo(const QString &dbInfo, const QString &tableName, const QString &tableColumns, const QString &fieldNames, const QString &titleName)
{
    QStringList columnNames;
    QSqlQuery query;
    QString createStr = tr("CREATE TABLE  group_info(id int NOT NULL AUTO_INCREMENT PRIMARY KEY,"
                                                                "db_infos varchar(255),"
                                                                "table_name varchar(255),"
                                                                "table_columns varchar(255),"
                                                                "field_names varchar(255),"
                                                                "title_name varchar(255))");
    qDebug() << "create SQL: " << createStr;
    bool ok = query.exec(createStr);
    if(!ok)
    {
        QString errStr = query.lastError().text();
        qDebug() << errStr << endl;
        if(!errStr.contains("already exists"))
        {
            return -1;
        }
    }

    QStringList groupNames = getGroupNames(dbInfo);
    if(groupNames.contains(titleName))
    {
        return -2;
    }

    QString insertStr = tr("INSERT INTO group_info(db_infos, table_name, table_columns, field_names, title_name) VALUES ('%1', '%2', '%3', '%4', '%5')")
            .arg(dbInfo).arg(tableName).arg(tableColumns).arg(fieldNames).arg(titleName);
    qDebug() << "insert SQL: " << insertStr;
    ok = query.exec(insertStr);
    if(!ok)
    {
        QString errStr = query.lastError().text();
        qDebug() << errStr << endl;
        return -3;
    }
    return 0;
}
