#ifndef PROJECTMGR_H
#define PROJECTMGR_H

#include <QObject>
#include "projectdb.h"

class ProjectFile
{
public:
    void setDASPath(const QString& path, const QStringList& list);
    QString m_dasPath;
    QString m_dtsPath;
    std::map<QString, QStringList> dasFileList;
    std::map<QString, QStringList> dtsFileList;
};

class ProjectMgr
{
public:
    ProjectMgr();
    static ProjectMgr& Instance();
    bool projectNew(const QString& name, const QString& memo);
    int projectOpen(const ProjectData& project);
    int projectClose(const ProjectData& project);
    bool projectDelete(const QString& name);
    int ProjectListAll(QVector<ProjectData>& list);

    bool addProjectPath(const QString& pathName);
    bool deleteProjectPath(const QString& pathName);
    int getProjectPath(QStringList& pathList);

public:
    QString m_curProjectName; //当前使用的 Project name
    ProjectData m_curProjectData;//当前使用的 ProjectData
    ProjectFile m_projectFile;
};

#endif // PROJECTMGR_H
