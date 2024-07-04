#ifndef CUSTOM_APP_H
#define CUSTOM_APP_H

#include <QList>
#include <QObject>
#include <QUrl>

class CustomApp : public QObject
{
    Q_OBJECT

public:

    static CustomApp *instance();
    explicit CustomApp();
    ~CustomApp() override;

    void loadInitialDocument();

};

#endif