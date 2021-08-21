#include <QApplication>

#include <QTableView>
#include <QThread>
#include <qlog/qlog.h>

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    install_log_handler("logtable");

    QTableView table;
    table.setModel(qlogmodel);
    table.show();

    QThread::create([] {
        for (int i = 0; i < 10; ++i)
        {
            QLOGI("INFO MSG %1", i);
            QLOGE("ERRO MSG %1", i);
            QThread::sleep(1);
        }
    })->start();

    return a.exec();
}