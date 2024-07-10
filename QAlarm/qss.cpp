#include "qss.h"

qss::qss()
{

}

QString qss::loadStyleSheet(const QString &sheetName)
{
    QString styleSheet;
    QFile file(sheetName);
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        styleSheet = QLatin1String(file.readAll());
    }
    return styleSheet;
}
