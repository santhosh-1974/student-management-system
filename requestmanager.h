#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <QString>
#include <QVector>
#include <QMap>

class RequestManager
{
public:
    static bool createRequest(int studentId, const QString &msg);
    static QVector<QMap<QString, QString>> getStudentRequests(int studentId);
    static QVector<QMap<QString, QString>> getAllRequests();
    static bool markResolved(int requestId);
};

#endif // REQUESTMANAGER_H
