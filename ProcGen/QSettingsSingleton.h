#pragma once

#include <QSettings>

class QSettingsSingleton {
public:
    static QSettings& get() {
        if (instance == nullptr) {
            instance = new QSettings("nitzryan", "procgen");
        }
        return *instance;
    }
private:
    QSettingsSingleton() = default;
    ~QSettingsSingleton() = default;
    static QSettings* instance;
};