#include "studentdashboard.h"
#include "ui_studentdashboard.h"
#include "studentmanager.h"
#include "requestmanager.h"
#include "authmanager.h"

#include <QDebug>
#include <QMessageBox>
#include <QTableWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QStyleOption>
#include <QPainter>

StudentDashboard::StudentDashboard(int studentId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StudentDashboard),
    id(studentId)
{
    ui->setupUi(this);

    this->setFixedSize(1000, 650);
    loadProfile();
    loadRequests();

    connect(ui->btnSubmit, &QPushButton::clicked,
            this, &StudentDashboard::on_btnSubmit_clicked);

    // If you later add change password fields, this will work automatically
    if (auto btnc = this->findChild<QPushButton*>("btnChangePass"))
        connect(btnc, &QPushButton::clicked, this, &StudentDashboard::on_btnChangePass_clicked);
}

StudentDashboard::~StudentDashboard()
{
    delete ui;
}

void StudentDashboard::loadProfile()
{
    if (id <= 0) {
        qDebug() << "Invalid student ID — loadProfile skipped";
        return;
    }

    auto map = StudentManager::getStudent(id);
    if (map.isEmpty()) return;

    ui->lblRoll->setText(map.value("roll"));
    ui->lblName->setText(map.value("name"));
    ui->lblPhone->setText(map.value("phone"));
    ui->lblEmail->setText(map.value("email"));
    ui->lblDept->setText(map.value("department"));
    ui->lblCgpa->setText(map.value("cgpa"));
}

void StudentDashboard::loadRequests()
{
    if (id <= 0) return;

    auto list = RequestManager::getStudentRequests(id);

    QTableWidget *tbl = ui->tblRequests;

    tbl->clear();
    tbl->setColumnCount(3);
    tbl->setHorizontalHeaderLabels({"ID", "Message", "Resolved"});
    tbl->setRowCount(list.size());

    for (int i = 0; i < list.size(); i++) {
        auto &m = list[i];
        tbl->setItem(i, 0, new QTableWidgetItem(m.value("id")));
        tbl->setItem(i, 1, new QTableWidgetItem(m.value("message")));
        tbl->setItem(i, 2, new QTableWidgetItem(m.value("resolved") == "1" ? "Yes" : "No"));
    }
}

void StudentDashboard::on_btnSubmit_clicked()
{
    QString msg = ui->txtRequests->text().trimmed();

    if (msg.isEmpty()) {
        QMessageBox::warning(this, "Request", "Enter request message.");
        return;
    }

    if (id <= 0) {
        QMessageBox::warning(this, "Request", "Student ID unknown.");
        return;
    }

    if (!RequestManager::createRequest(id, msg)) {
        QMessageBox::warning(this, "Request", "Failed to submit request.");
        return;
    }

    QMessageBox::information(this, "Request", "Request submitted successfully!");

    loadRequests();
    ui->txtRequests->clear();
}

void StudentDashboard::on_btnChangePass_clicked()
{
    auto newPassLE = this->findChild<QLineEdit*>("edtNewPass");
    auto confirmLE = this->findChild<QLineEdit*>("edtConfirmPass");

    if (!newPassLE || !confirmLE) {
        QMessageBox::warning(this, "Change Password", "Password fields missing in UI.");
        return;
    }

    QString np = newPassLE->text().trimmed();
    QString cp = confirmLE->text().trimmed();

    if (np.isEmpty() || cp.isEmpty()) {
        QMessageBox::warning(this, "Change Password", "Enter new password and confirmation.");
        return;
    }

    if (np != cp) {
        QMessageBox::warning(this, "Change Password", "Passwords do not match.");
        return;
    }

    auto profile = StudentManager::getStudent(id);
    if (profile.isEmpty()) {
        QMessageBox::warning(this, "Change Password", "Student profile not found.");
        return;
    }

    QString username = profile.value("roll").trimmed();

    if (AuthManager::changePassword(username, np)) {
        QMessageBox::information(this, "Change Password", "Password changed successfully.");
        newPassLE->clear();
        confirmLE->clear();
    } else {
        QMessageBox::warning(this, "Change Password", "Failed to change password.");
    }
}
void StudentDashboard::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);        // <-- use initFrom
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}
