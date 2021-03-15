#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QStringList>

class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

public:
    bool initDatabase(const QString &ip, int port, const QString &database, const QString &username, const QString &password);
    QStringList getTables(const QString &databaseName);
    QStringList getTableColumns(const QString &databaseName, const QString &tableName);
    QStringList getGroupNames(const QString &dbInfos);
    int saveGroupInfo(const QString &dbInfo, const QString &tableName, const QString &tableColumns, const QString &fieldNames, const QString &titleName);

private:


private:
    QSqlDatabase db;

public slots:
};

#endif // DATABASEMANAGER_H
