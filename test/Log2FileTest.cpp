
#include <qlog/qlog.h>
#include <QCoreApplication>
#include <QDebug>


int main(int argc, char** argv)
{
    QCoreApplication a(argc, argv);
    install_log_handler("log2file");

    qDebug() << "debug hello";
    qWarning() << "warning hello";
    qCritical() << "critical hello";
    qDebug("debug hello 2");
    qDebug("中文");

    qDebug("-------");


    QLOGD("debug hello");
    QLOGW("warning hello");
    QLOGE("critical hello");
    QLOGD("debug hello %1 %2", 3, "中文");

    return a.exec();
}