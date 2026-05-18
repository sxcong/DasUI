#ifndef PROJECTDB_H
#define PROJECTDB_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDateTime>
#include <QVector>
#include <map>

struct ProjectData {
    QString name;
    QString memo;
    QString datetime;
};

class ProjectDB
{
public:
    ProjectDB();
    void loadConfig();
    static ProjectDB& Instance();
    void initDB();
    void initTable();
    bool addProject(const QString& name, const QString& memo);
    bool deleteProject(const QString& name);
    QVector<ProjectData> getAllProjects();

    void initProjectPathTable();
    bool addProjectPath(const QString& projectName, const QString& pathName);
    bool deleteProjectPath(const QString& projectName, const QString& pathName);
    int getProjectPath(const QString& projectName, QStringList& pathList);
private:
    QSqlDatabase db;
    //std::string db_path;
    QString m_dbPath;
};

#endif // PROJECTDB_H
