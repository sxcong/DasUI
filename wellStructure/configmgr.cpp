
#include <QFile>
#include <QCoreApplication>
#include <QSettings>
#include "configmgr.h"
#include "QDebug"

ConfigMgr::ConfigMgr()
{

}

ConfigMgr &ConfigMgr::instance()
{
    static ConfigMgr tmp;
    return tmp;
}

void ConfigMgr::loadConfig()
{
    QString cfgpath = QCoreApplication::applicationDirPath()+"/config/structConfig.ini";
    if (QFile::exists(cfgpath))
    {
        QSettings setting(cfgpath, QSettings::IniFormat);
        for(int i = 0;i< 6;i++)
        {

            if (setting.contains(QString("m_w_%1").arg(i)))
            {
                m_compara[i].m_w = setting.value(QString("m_w_%1").arg(i)).toInt();
            }

            if (setting.contains(QString("m_h_%1").arg(i)))
            {
                m_compara[i].m_h = setting.value(QString("m_h_%1").arg(i)).toInt();
            }
        }


        if (setting.contains(QString("Fulllength")))
        {
            m_Fulllength = setting.value("Fulllength").toInt();
        }

        m_realLength = m_compara[3].m_h;
    }
}

void ConfigMgr::saveConfig()
{
    QString cfgpath = QCoreApplication::applicationDirPath()+"/config/structConfig.ini";

    QSettings setting(cfgpath, QSettings::IniFormat);
    for(int i = 0;i< 6;i++)
    {
       setting.setValue(QString("m_w_%1").arg(i), m_compara[i].m_w);
       setting.setValue(QString("m_h_%1").arg(i), m_compara[i].m_h);
    }
    setting.setValue("Fulllength",m_Fulllength);

    m_realLength = m_compara[3].m_h;
}


