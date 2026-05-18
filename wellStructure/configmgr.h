#ifndef CONFIGMGR_H
#define CONFIGMGR_H

#include <QString>

struct ComponentPara
{
     int m_w;
     int m_h;
};

class ConfigMgr
{
public:
    static ConfigMgr& instance();
    void loadConfig();
    void saveConfig();

    ComponentPara m_compara[6];//1,2,3,4，5，6分别为一级套管，油管，射孔，水泥间隙，二级套管，三级套管

    int m_Fulllength = 0;//井身长度（大体）
    int m_realLength = 0;
private:
    ConfigMgr();
};

#endif // STORAGECONFIG_H
