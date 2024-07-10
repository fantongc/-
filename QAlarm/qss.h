#ifndef QSS_H
#define QSS_H
#include <QString>
#include <QFile>

class qss
{
public:
    qss();
    QString loadStyleSheet(const QString &sheetName);
};

#endif // QSS_H
