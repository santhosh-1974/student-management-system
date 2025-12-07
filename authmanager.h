#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <QString>

class AuthManager {
public:
    // Returns role (e.g., "admin", "student") or empty string on fail
    static QString login(const QString &username, const QString &password);
    // Returns student id for a username or -1
    static int getStudentId(const QString &username);
    // Change password for a username
    static bool changePassword(const QString &username, const QString &newPassword);
};

#endif // AUTHMANAGER_H
