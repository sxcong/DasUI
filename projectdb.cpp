#include "projectdb.h"
#include <QDebug>
#include <QCoreApplication>
#include <QSettings>
#include "global.h"

void debugSqlError(const QSqlQuery &query) {
    QSqlError err = query.lastError();
    if (err.isValid()) {
        qDebug() << "--- SQL Error Report ---";
        qDebug() << "Error Text:" << err.text();
        qDebug() << "Error Type:" << err.type();
        qDebug() << "Last Query:" << query.lastQuery(); // 看看最后执行的 SQL 长什么样
        qDebug() << "-------------------------";
    }
}

ProjectDB::ProjectDB()
{
    loadConfig();
}

void ProjectDB::loadConfig()
{
    QString filePath = QCoreApplication::applicationDirPath()+"/config/sqlite.ini";
    if(!filePath.isEmpty())
    {
        QSettings fileConfig(filePath, QSettings::IniFormat);
        fileConfig.setIniCodec("UTF8");

        QString dbPath = "db/project.sdb";
        if(fileConfig.contains("db_path"))
        {
            dbPath = fileConfig.value("db_path").toString();
        }
        m_dbPath = QCoreApplication::applicationDirPath() + "/" + dbPath;
        qDebug()<<"ProjectDB createFullFilePath"<<m_dbPath;
        createFullFilePath(m_dbPath);
    }
}
ProjectDB& ProjectDB::Instance()
{
    static ProjectDB agent;
    return agent;
}

void ProjectDB::initDB()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(m_dbPath);
    if (!db.open()) {
        qDebug() << "Error: connection with database fail";
    } else {
        initTable();
        initProjectPathTable();
    }
}

// 初始化表
void ProjectDB::initTable() {
    QSqlQuery query;
    bool bRet = query.exec("CREATE TABLE IF NOT EXISTS project ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "name TEXT UNIQUE, "
               "memo TEXT, "
               "datetime TEXT)");
     qDebug() << "ProjectDB initTable"<<bRet;

    if (!bRet)
    {
        QSqlError error = query.lastError();

        // 打印不同层级的错误信息
        qDebug() << "错误描述:" << error.text();           // 数据库传回的可读文本
        qDebug() << "数据库错误码:" << error.nativeErrorCode(); // 原始错误码 (如 SQLite 的 1)
        qDebug() << "驱动错误类型:" << error.type();
    }
}


// 增加数据
bool ProjectDB::addProject(const QString& name, const QString& memo) {
    QSqlQuery query;
    query.prepare("INSERT INTO project (name, memo, datetime) VALUES (:name, :memo, :dt)");
    query.bindValue(":name", name);
    query.bindValue(":memo", memo);
    query.bindValue(":dt", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    return query.exec();
}

// 删除数据
bool ProjectDB::deleteProject(const QString& name) {
    QSqlQuery query;
    query.prepare("DELETE FROM project WHERE name = :name");
    query.bindValue(":name", name);
    return query.exec();
}

// 查询所有
QVector<ProjectData> ProjectDB::getAllProjects() {
    QVector<ProjectData> list;
    QSqlQuery query("SELECT name, memo, datetime FROM project");
    while (query.next()) {
        list.append({query.value(0).toString(),
                     query.value(1).toString(),
                     query.value(2).toString()});
    }
    return list;
}

//工区和路径关联
void ProjectDB::initProjectPathTable()
{
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS project_path ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "project_name TEXT, "
               "path_name TEXT, "
               "datetime TEXT)");
}

bool ProjectDB::addProjectPath(const QString& projectName, const QString& pathName)
{
    QSqlQuery query;
    query.prepare("INSERT INTO project_path(project_name, path_name, datetime) VALUES (:project_name, :path_name, :dt)");
    query.bindValue(":project_name", projectName);
    query.bindValue(":path_name", pathName);
    query.bindValue(":dt", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    bool bRet = query.exec();

    if (!bRet)
    {
        QSqlError error = query.lastError();

        // 打印不同层级的错误信息
        qDebug() << "错误描述:" << error.text();           // 数据库传回的可读文本
        qDebug() << "数据库错误码:" << error.nativeErrorCode(); // 原始错误码 (如 SQLite 的 1)
        qDebug() << "驱动错误类型:" << error.type();
    }
    return bRet;
}
bool ProjectDB::deleteProjectPath(const QString& projectName, const QString& pathName)
{
    QSqlQuery query;
    query.prepare("DELETE FROM project_path WHERE project_name =:project_name AND path_name = :path_name");
    query.bindValue(":project_name", projectName);
    query.bindValue(":path_name", pathName);
    return query.exec();
}
int ProjectDB::getProjectPath(const QString& projectName, QStringList& pathList)
{
    QSqlQuery query;
    // 1. 准备 SQL 语句
    query.prepare("SELECT path_name FROM project_path WHERE project_name = :project_name");

    // 2. 绑定参数 (注意名字要和 SQL 里的 :project_name 完全一致)
    query.bindValue(":project_name", projectName);

    // 3. 执行查询
    if (!query.exec()) {
        qDebug() << "执行失败:" << query.lastError().text();
    } else {
        while (query.next()) {
            qDebug() << query.value(0).toString();
            pathList.append(query.value(0).toString());
        }
    }
    debugSqlError(query);
    return pathList.size();
}
