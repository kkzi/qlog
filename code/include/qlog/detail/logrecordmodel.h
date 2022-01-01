#pragma once

#include "qlogdef.h"

#include <QAbstractTableModel>
#include <QDateTime>
#include <QList>
#include <QMutex>
#include <QString>

struct LogRecord
{
    QDateTime time;
    QtMsgType level;
    QString msg;
};

class QLOG_API LogRecordModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum class NewRecordPosition
    {
        Top,
        Bottom,
    };

private:
    LogRecordModel() = default;
    ~LogRecordModel() = default;
    Q_DISABLE_COPY_MOVE(LogRecordModel)

public:
    static LogRecordModel *instance();

public:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

public:
    void setNewRecordPosition(NewRecordPosition pos);
    void setDateTimeFormat(const QString &fmt);
    void append(const LogRecord &log);
    const QList<LogRecord> &logs() const;
    LogRecord log(int i) const;
    int columnWidth(int col);
    void clear();

private:
    QVariant columnDisplay(const LogRecord &log, int row, int col) const;
    void updateLogs();

private:
    QString fmt_{ "MM-dd hh:mm:ss.zzz" };
    QList<LogRecord> logs_;
    NewRecordPosition pos_{ NewRecordPosition::Bottom };
};
