#ifndef KIJANGSETTINGS_H
#define KIJANGSETTINGS_H

#include <QDebug>


class KijangSettings
{
public:
    KijangSettings();
    ~KijangSettings();

public slots:
    void save();
    void load();
};

#endif // KIJANGSETTINGS_H
