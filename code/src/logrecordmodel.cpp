#include "detail/logrecordmodel.h"
#include <QStringList>
#include <QtGlobal>

static const QStringList headers{ "时间", "级别", "日志" };

LogRecordModel *LogRecordModel::instance()
{
    static LogRecordModel inst;
    return &inst;
}

QVariant LogRecordModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role)
    {
    case Qt::DisplayRole:
        if (orientation == Qt::Vertical)
        {
            return section + 1;
        }
        else
        {
            return headers.value(section);
        }
    default:
        break;
    }
    return QVariant();
}

int LogRecordModel::rowCount(const QModelIndex &parent) const
{
    return logs_.size();
}

int LogRecordModel::columnCount(const QModelIndex &parent) const
{
    return headers.size();
}

QVariant LogRecordModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= logs_.size())
    {
        return QVariant();
    }
    auto log = logs_.at(index.row());
    switch (role)
    {
    case Qt::DisplayRole:
        return columnDisplay(log, index.row(), index.column());
    default:
        break;
    }
    return QVariant();
}

void LogRecordModel::setNewRecordPosition(NewRecordPosition pos)
{
    pos_ = pos;
}

int LogRecordModel::columnWidth(int col)
{
    static QVector<int> colws{ 140, 60, 200 };
    return colws.value(col, 100);
}

void LogRecordModel::setDateTimeFormat(const QString &fmt)
{
    fmt_ = fmt;
}

void LogRecordModel::append(const LogRecord &log)
{
    auto rowc = logs_.size();
    switch (pos_)
    {
    case LogRecordModel::NewRecordPosition::Top:
        beginInsertRows(QModelIndex(), 0, 0);
        logs_.prepend(log);
        break;
    case LogRecordModel::NewRecordPosition::Bottom:
        beginInsertRows(QModelIndex(), rowc, rowc);
        logs_.append(log);
        break;
    default:
        break;
    }
    endInsertRows();
}

const QList<LogRecord> &LogRecordModel::logs() const
{
    return logs_;
}

LogRecord LogRecordModel::log(int i) const
{
    return logs_.value(i, LogRecord());
}

void LogRecordModel::clear()
{
    beginResetModel();
    logs_.clear();
    endResetModel();
}

QVariant LogRecordModel::columnDisplay(const LogRecord &log, int row, int col) const
{
    static QStringList levels{ "DEBUG", "WARNING", "CRITICAL", "FATAL", "INFO" };
    switch (col)
    {
    case 0:
        return log.time.toString(fmt_);
    case 1:
        return levels.value(log.level);
    case 2:
        return log.msg;
    }
    return QVariant();
}

void LogRecordModel::updateLogs()
{
}
