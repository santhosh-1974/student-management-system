#include "requestmanager.h"
#include "database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool RequestManager::createRequest(int studentId, const QString &msg) {
    if (!Database::connect()) return false;

    QSqlQuery q;
    q.prepare("INSERT INTO requests (student_id, message, resolved) "
              "VALUES (:sid, :msg, 0)");
    q.bindValue(":sid", studentId);
    q.bindValue(":msg", msg);

    if (!q.exec()) {
        qWarning() << "createRequest failed:" << q.lastError().text();
        return false;
    }

    return true;
}

QVector<QMap<QString, QString>> RequestManager::getStudentRequests(int studentId) {
    QVector<QMap<QString, QString>> list;
    if (!Database::connect()) return list;

    QSqlQuery q;
    q.prepare("SELECT id, message, resolved FROM requests WHERE student_id = :sid");
    q.bindValue(":sid", studentId);

    if (!q.exec()) {
        qWarning() << "getStudentRequests failed:" << q.lastError().text();
        return list;
    }

    while (q.next()) {
        QMap<QString, QString> m;
        m["id"] = q.value(0).toString();
        m["message"] = q.value(1).toString();
        m["resolved"] = q.value(2).toString();
        list.append(m);
    }
    return list;
}

QVector<QMap<QString, QString>> RequestManager::getAllRequests() {
    QVector<QMap<QString, QString>> list;
    if (!Database::connect()) return list;

    QSqlQuery q(
        "SELECT r.id, s.roll, s.name, r.message, r.resolved "
        "FROM requests r "
        "LEFT JOIN students s ON r.student_id = s.id"
        );

    if (!q.exec()) {
        qWarning() << "getAllRequests failed:" << q.lastError().text();
        return list;
    }

    while (q.next()) {
        QMap<QString, QString> m;
        m["id"] = q.value(0).toString();
        m["student_roll"] = q.value(1).toString();
        m["student_name"] = q.value(2).toString();
        m["message"] = q.value(3).toString();
        m["resolved"] = q.value(4).toString();
        list.append(m);
    }
    return list;
}

bool RequestManager::markResolved(int requestId) {
    if (!Database::connect()) return false;

    QSqlQuery q;
    q.prepare("UPDATE requests SET resolved = 1 WHERE id = :id");
    q.bindValue(":id", requestId);

    if (!q.exec()) {
        qWarning() << "markResolved failed:" << q.lastError().text();
        return false;
    }
    return true;
}
