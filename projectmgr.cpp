#include "projectmgr.h"

void ProjectFile::setDASPath(const QString& path, const QStringList& list)
{
    dasFileList[path] = list;
}

ProjectMgr::ProjectMgr() {}

ProjectMgr &ProjectMgr::Instance()
{
    static ProjectMgr agent;
    return agent;
}

bool ProjectMgr::projectNew(const QString& name, const QString& memo)
{
    return ProjectDB::Instance().addProject(name, memo);
}

int ProjectMgr::projectOpen(const ProjectData &project)
{
    m_curProjectName = project.name;
    m_curProjectData = project;
    return 0;
}

int ProjectMgr::projectClose(const ProjectData &project)
{
    m_curProjectName = "";
    return 0;
}

bool ProjectMgr::projectDelete(const QString& name)
{
    return ProjectDB::Instance().deleteProject(name);
}

int ProjectMgr::ProjectListAll(QVector<ProjectData>& list)
{
    list = ProjectDB::Instance().getAllProjects();
    return list.size();
}

bool ProjectMgr::addProjectPath(const QString& pathName)
{
    return ProjectDB::Instance().addProjectPath(m_curProjectName, pathName);
}

bool ProjectMgr::deleteProjectPath(const QString& pathName)
{
    return ProjectDB::Instance().deleteProjectPath(m_curProjectName, pathName);
}

int ProjectMgr::getProjectPath(QStringList& pathList)
{
    return ProjectDB::Instance().getProjectPath(m_curProjectName, pathList);
}
