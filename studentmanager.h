#ifndef STUDENTMANAGER_H
#define STUDENTMANAGER_H

#include <QMap>
#include <QString>
#include <QVector>

class StudentManager {
public:
    static QMap<QString, QString> getStudent(int studentId);
    static QVector<QMap<QString, QString>> getAllStudents();
    static bool addStudent(QMap<QString, QString> data);
    static bool updateStudent(int id, QMap<QString, QString> data);
    static bool deleteStudent(int id);
};

#endif // STUDENTMANAGER_H
