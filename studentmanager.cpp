#include "studentmanager.h"
#include "database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>


QMap<QString, QString> StudentManager::getStudent(int studentId) {
    QMap<QString, QString> res;
    if (!Database::connect()) return res;

    QSqlQuery q;
    q.prepare("SELECT id, roll, name, department, year, email, phone, cgpa "
              "FROM students WHERE id = :id");
    q.bindValue(":id", studentId);

    if (!q.exec()) {
        qWarning() << "getStudent failed:" << q.lastError().text();
        return res;
    }

    if (q.next()) {
        res["id"] = q.value(0).toString();
        res["roll"] = q.value(1).toString();
        res["name"] = q.value(2).toString();
        res["department"] = q.value(3).toString();
        res["year"] = q.value(4).toString();
        res["email"] = q.value(5).toString();
        res["phone"] = q.value(6).toString();
        res["cgpa"] = q.value(7).toString();
    }

    return res;
}

QVector<QMap<QString, QString>> StudentManager::getAllStudents() {
    QVector<QMap<QString, QString>> list;
    if (!Database::connect()) return list;

    QSqlQuery q("SELECT id, roll, name, department, year, email, phone, cgpa FROM students");

    if (!q.exec()) {
        qWarning() << "getAllStudents failed:" << q.lastError().text();
        return list;
    }

    while (q.next()) {
        QMap<QString, QString> m;
        m["id"] = q.value(0).toString();
        m["roll"] = q.value(1).toString();
        m["name"] = q.value(2).toString();
        m["department"] = q.value(3).toString();
        m["year"] = q.value(4).toString();
        m["email"] = q.value(5).toString();
        m["phone"] = q.value(6).toString();
        m["cgpa"] = q.value(7).toString();
        list.append(m);
    }

    return list;
}

bool StudentManager::addStudent(QMap<QString, QString> data) {
    if (!Database::connect()) return false;

    QString roll = data.value("roll").trimmed();
    QString name = data.value("name").trimmed();

    // Insert into students table
    QSqlQuery q;
    q.prepare("INSERT INTO students (roll, name, department, year, email, phone, cgpa) "
              "VALUES (:roll, :name, :department, :year, :email, :phone, :cgpa)");

    q.bindValue(":roll", roll);
    q.bindValue(":name", name);
    q.bindValue(":department", data.value("department").trimmed());
    q.bindValue(":year", data.value("year").trimmed());
    q.bindValue(":email", data.value("email").trimmed());
    q.bindValue(":phone", data.value("phone").trimmed());
    q.bindValue(":cgpa", data.value("cgpa").trimmed());

    if (!q.exec()) {
        qWarning() << "addStudent failed:" << q.lastError().text();
        return false;
    }

    int studentId = q.lastInsertId().toInt();
    qDebug() << "Inserted STUDENT with ID:" << studentId;

    // LOGIN CREATION
    QString username = roll;          // username = roll number
    QString password = name + roll;   // password = name + roll

    QSqlQuery u;
    u.prepare("INSERT INTO users (username, password, role, student_id) "
              "VALUES (:u, :p, 'student', :sid)");
    u.bindValue(":u", username);
    u.bindValue(":p", password);
    u.bindValue(":sid", studentId);

    if (!u.exec()) {
        qWarning() << "FAILED TO INSERT STUDENT LOGIN:" << u.lastError().text();
    } else {
        qDebug() << "STUDENT LOGIN INSERTED → username:" << username
                 << "password:" << password
                 << "student_id:" << studentId;
    }

    return true;
}

bool StudentManager::updateStudent(int id, QMap<QString, QString> data) {
    if (!Database::connect()) return false;

    QSqlQuery q;
    q.prepare("UPDATE students SET roll=:roll, name=:name, department=:department, "
              "year=:year, email=:email, phone=:phone, cgpa=:cgpa "
              "WHERE id=:id");

    q.bindValue(":roll", data.value("roll").trimmed());
    q.bindValue(":name", data.value("name").trimmed());
    q.bindValue(":department", data.value("department").trimmed());
    q.bindValue(":year", data.value("year").trimmed());
    q.bindValue(":email", data.value("email").trimmed());
    q.bindValue(":phone", data.value("phone").trimmed());
    q.bindValue(":cgpa", data.value("cgpa").trimmed());
    q.bindValue(":id", id);

    if (!q.exec()) {
        qWarning() << "updateStudent failed:" << q.lastError().text();
        return false;
    }

    return true;
}

bool StudentManager::deleteStudent(int id) {
    if (!Database::connect()) return false;

    // Delete login first
    QSqlQuery u;
    u.prepare("DELETE FROM users WHERE student_id=:sid");
    u.bindValue(":sid", id);
    u.exec();

    // Delete student record
    QSqlQuery q;
    q.prepare("DELETE FROM students WHERE id = :id");
    q.bindValue(":id", id);

    return q.exec();
}
