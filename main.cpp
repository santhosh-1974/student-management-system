#include <QApplication>
#include <QDebug>
#include <QSqlQuery>

#include "loginwindow.h"
#include "database.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Connect to database
    if (!Database::connect()) {
        qWarning() << "DB connection failed!";
    }

    // 🔥 DEBUG PRINT: SHOW USERS TABLE EVERY TIME APP STARTS
    {
        QSqlQuery q("SELECT id, username, password, role, student_id FROM users");
        qDebug() << "---------------- USERS TABLE ----------------";
        while (q.next()) {
            qDebug()
            << "id:" << q.value(0).toString()
            << "username:" << q.value(1).toString()
            << "password:" << q.value(2).toString()
            << "role:" << q.value(3).toString()
            << "student_id:" << q.value(4).toString();
        }
        qDebug() << "------------------------------------------------";
    }

    // Start login window
    LoginWindow w;
    w.show();

    return a.exec();
}
