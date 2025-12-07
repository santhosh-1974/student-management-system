#include "authmanager.h"
#include "database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

QString AuthManager::login(const QString &username, const QString &password) {
    if (!Database::connect()) return "";

    QSqlQuery q;
    q.prepare("SELECT role FROM users WHERE username=:u AND password=:p LIMIT 1");
    q.bindValue(":u", username.trimmed());
    q.bindValue(":p", password.trimmed());
    q.exec();

    if (q.next())
        return q.value(0).toString();

    return "";
}

int AuthManager::getStudentId(const QString &username) {
    if (!Database::connect()) return -1;

    QSqlQuery q;
    q.prepare("SELECT student_id FROM users WHERE username=:u LIMIT 1");
    q.bindValue(":u", username.trimmed());
    q.exec();

    if (q.next())
        return q.value(0).toInt();

    return -1;
}

bool AuthManager::changePassword(const QString &username, const QString &newPassword) {
    if (!Database::connect()) return false;

    QSqlQuery q;
    q.prepare("UPDATE users SET password=:p WHERE username=:u");
    q.bindValue(":p", newPassword.trimmed());
    q.bindValue(":u", username.trimmed());
    return q.exec();
}
