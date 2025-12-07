#include "database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QDebug>

bool Database::connect() {

    QString dbFile = "students.db";

    // --- DO NOT DELETE DATABASE ANYMORE ---
    // If you still have this, REMOVE IT:
    // QFile::remove(dbFile);

    // Create or open DB
    QSqlDatabase db;

    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::database();
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbFile);
    }

    if (!db.open()) {
        qWarning() << "DB OPEN FAILED:" << db.lastError().text();
        return false;
    }

    qDebug() << "DATABASE OPENED:" << dbFile;

    QSqlQuery q;

    q.exec("CREATE TABLE IF NOT EXISTS users ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "username TEXT UNIQUE,"
           "password TEXT,"
           "role TEXT,"
           "student_id INTEGER)");

    q.exec("CREATE TABLE IF NOT EXISTS students ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "roll TEXT UNIQUE,"
           "name TEXT,"
           "department TEXT,"
           "year TEXT,"
           "email TEXT,"
           "phone TEXT,"
           "cgpa TEXT)");

    q.exec("CREATE TABLE IF NOT EXISTS requests ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "student_id INTEGER,"
           "message TEXT,"
           "resolved INTEGER DEFAULT 0)");

    // Add default admin ONLY IF NOT EXISTS
    q.prepare("INSERT OR IGNORE INTO users (username, password, role) "
              "VALUES ('Ravi','Ravi123','admin')");
    q.exec();

    return true;
}
