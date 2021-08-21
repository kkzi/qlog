#include "qlog.h"

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QMutex>
#include <QString>
#include <QTextCodec>
#include <QTextStream>
#include <mutex>

#ifdef _WIN32
    #include <Windows.h>
#endif

static size_t __thread_id()
{
#ifdef _WIN32
    return static_cast<size_t>(::GetCurrentThreadId());
#elif defined(__linux__)
    #if defined(__ANDROID__) && defined(__ANDROID_API__) && (__ANDROID_API__ < 21)
        #define SYS_gettid __NR_gettid
    #endif
    return static_cast<size_t>(::syscall(SYS_gettid));
#elif defined(_AIX) || defined(__DragonFly__) || defined(__FreeBSD__)
    return static_cast<size_t>(::pthread_getthreadid_np());
#elif defined(__NetBSD__)
    return static_cast<size_t>(::_lwp_self());
#elif defined(__OpenBSD__)
    return static_cast<size_t>(::getthrid());
#elif defined(__sun)
    return static_cast<size_t>(::thr_self());
#elif __APPLE__
    uint64_t tid;
    pthread_threadid_np(nullptr, &tid);
    return static_cast<size_t>(tid);
#else  // Default to standard C++11 (other Unix)
    return static_cast<size_t>(std::hash<std::thread::id>()(std::this_thread::get_id()));
#endif
}

static QChar __level2char(QtMsgType level)
{
    static QMap<QtMsgType, QChar> chars{
        { QtDebugMsg, 'D' },
        { QtWarningMsg, 'W' },
        { QtInfoMsg, 'I' },
        { QtCriticalMsg, 'E' },
        { QtFatalMsg, 'E' },
    };
    return chars.value(level);
}

static void __do_install_log_handler(const QString &name, QDir dir)
{
    if (!dir.exists())
    {
        dir.root().mkpath(dir.absolutePath());
    }
    QString path = QString("%0/%1-%2.log").arg(dir.absolutePath()).arg(name).arg(QDateTime::currentDateTime().toString("MM-dd-hh-mm-ss-zzz"));
    static QFile file(path);
    if (!file.open(QFile::WriteOnly | QFile::Truncate))
    {
        qCritical() << "initialize log file failed";
    }
    static QTextStream stream(&file);
    static QMutex streamMutex;
    stream.setCodec(QTextCodec::codecForName("UTF-8"));

    qInstallMessageHandler([](QtMsgType msgType, const QMessageLogContext &ctx, const QString &msg) {
        auto now = QDateTime::currentDateTime();
        const QStringList &rows = msg.trimmed().split('\n', Qt::SkipEmptyParts);
        QMutexLocker lock(&streamMutex);
        for (const auto &it : rows)
        {
            QString &&m = it.trimmed();
            if (m.startsWith('"')) m.remove(0, 1);
            if (m.endsWith('"')) m.remove(m.length() - 1, 1);
            if (m.isEmpty())
            {
                continue;
            }

            auto &&line = QString("[%1][%2] %3 [%4:%5, %6]")
                              .arg(__level2char(msgType))
                              .arg(now.toString("MM-dd hh:mm:ss.zzz"))
                              .arg(m)
                              .arg(QString(ctx.file).split('\\').last())
                              .arg(ctx.line)
                              .arg(__thread_id());

            stream << line << endl;

            qt_message_output(msgType, ctx, line);
        }
        stream.flush();
    });
}

void install_log_handler(const char *name, const QString &path)
{
    static std::once_flag flag;
    std::call_once(flag, [name, path] {
        __do_install_log_handler(name, path);
    });
}
