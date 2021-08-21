#pragma once

#include "detail/logrecordmodel.h"
#define qlogmodel LogRecordModel::instance()

QLOG_API void install_log_handler(const char *name, const QString &path = "./logs");

template <typename... Args>
static QString _expand_qstring_(QString fmt, Args... args)
{
    ((fmt = std::move(fmt.arg(args))), ...);
    return std::move(fmt);
}

#define _qlog_(_LEVEL_, _MSG_, ...)                                                                                                                            \
    {                                                                                                                                                          \
        auto &&text = _expand_qstring_(_MSG_, __VA_ARGS__);                                                                                                    \
        qt_message_output(QtMsgType::_LEVEL_, QMessageLogContext(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC, ""), text);                       \
        qlogmodel->append({ QDateTime::currentDateTime(), QtMsgType::_LEVEL_, text });                                                                         \
    }

#define QLOGD(fmt, ...) _qlog_(QtDebugMsg, fmt, __VA_ARGS__)
#define QLOGI(fmt, ...) _qlog_(QtInfoMsg, fmt, __VA_ARGS__)
#define QLOGW(fmt, ...) _qlog_(QtWarningMsg, fmt, __VA_ARGS__)
#define QLOGE(fmt, ...) _qlog_(QtCriticalMsg, fmt, __VA_ARGS__)
