#include "admindashboard.h"
#include "ui_admindashboard.h"

#include "studentmanager.h"
#include "requestmanager.h"
#include "database.h"

#include <QMessageBox>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QStyleOption>
#include <QPainter>

AdminDashboard::AdminDashboard(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AdminDashboard)
{
    ui->setupUi(this);

    this->setFixedSize(1000, 650);


    // Set object name for stylesheet
    this->setObjectName("AdminDashboard");



    studentManager  = new StudentManager();
    requestManager  = new RequestManager();

    loadRequests();

    connect(ui->btnSearch,    &QPushButton::clicked, this, &AdminDashboard::on_btnSearch_clicked);
    connect(ui->btnAdd,       &QPushButton::clicked, this, &AdminDashboard::on_btnAdd_clicked);
    connect(ui->btnUpdate,    &QPushButton::clicked, this, &AdminDashboard::on_btnUpdate_clicked);
    connect(ui->btnDelete,    &QPushButton::clicked, this, &AdminDashboard::on_btnDelete_clicked);
    connect(ui->btnResolved,  &QPushButton::clicked, this, &AdminDashboard::on_btnResolved_clicked);
    connect(ui->btnClear,     &QPushButton::clicked, this, &AdminDashboard::on_btnClear_clicked);
    connect(ui->btnAddAdmin,  &QPushButton::clicked, this, &AdminDashboard::on_btnAddAdmin_clicked);
}

AdminDashboard::~AdminDashboard()
{
    delete ui;
}
void AdminDashboard::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);        // <-- use initFrom
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}

void AdminDashboard::on_btnSearch_clicked()
{
    QString roll = ui->txtSearchRoll->text().trimmed();

    if (roll.isEmpty()) {
        QMessageBox::warning(this, "Search", "Please enter a Roll Number.");
        return;
    }

    auto all = StudentManager::getAllStudents();
    bool found = false;

    for (auto &m : all) {
        if (m["roll"] == roll) {

            ui->txtRoll->setText(m["roll"]);
            ui->txtName->setText(m["name"]);
            ui->txtDept->setText(m["department"]);
            ui->txtPhone->setText(m["phone"]);
            ui->txtEmail->setText(m["email"]);
            ui->txtCgpa->setText(m["cgpa"]);
            found = true;
            break;
        }
    }

    if (!found)
        QMessageBox::information(this, "Search", "Student not found.");
}

void AdminDashboard::on_btnAdd_clicked()
{
    QMap<QString, QString> data;

    data["roll"]       = ui->txtRoll->text().trimmed();
    data["name"]       = ui->txtName->text().trimmed();
    data["department"] = ui->txtDept->text().trimmed();
    data["phone"]      = ui->txtPhone->text().trimmed();
    data["email"]      = ui->txtEmail->text().trimmed();
    data["cgpa"]       = ui->txtCgpa->text().trimmed();
    data["year"]       = " ";

    if (data["roll"].isEmpty()) {
        QMessageBox::warning(this, "Add", "Roll Number is required.");
        return;
    }

    // Add to database
    if (!StudentManager::addStudent(data)) {
        QMessageBox::warning(this, "Add", "Failed to add student. Roll may already exist.");
        return;
    }

    // Show actual username/password
    QString username = data["roll"];
    QString password = data["name"] + data["roll"];

    QMessageBox::information(
        this,
        "Student Added",
        "Student added successfully!\n\n"
        "LOGIN DETAILS:\n"
        "Username: " + username + "\n"
                         "Password: " + password
        );

    clearFields();
}

void AdminDashboard::on_btnUpdate_clicked()
{
    QString roll = ui->txtRoll->text().trimmed();
    if (roll.isEmpty()) {
        QMessageBox::warning(this, "Update", "Roll Number is required.");
        return;
    }

    auto all = StudentManager::getAllStudents();
    int id = -1;

    for (auto &m : all)
        if (m["roll"] == roll)
            id = m["id"].toInt();

    if (id < 0) {
        QMessageBox::warning(this, "Update", "Student not found.");
        return;
    }

    QMap<QString, QString> data;

    data["roll"]       = ui->txtRoll->text().trimmed();
    data["name"]       = ui->txtName->text().trimmed();
    data["department"] = ui->txtDept->text().trimmed();
    data["phone"]      = ui->txtPhone->text().trimmed();
    data["email"]      = ui->txtEmail->text().trimmed();
    data["cgpa"]       = ui->txtCgpa->text().trimmed();
    data["year"]       = " ";

    if (!StudentManager::updateStudent(id, data)) {
        QMessageBox::warning(this, "Update", "Failed to update.");
        return;
    }

    QMessageBox::information(this, "Update", "Updated successfully!");
}

void AdminDashboard::on_btnDelete_clicked()
{
    QString roll = ui->txtRoll->text().trimmed();

    if (roll.isEmpty()) {
        QMessageBox::warning(this, "Delete", "Enter Roll Number.");
        return;
    }

    auto all = StudentManager::getAllStudents();
    int id = -1;

    for (auto &m : all)
        if (m["roll"] == roll)
            id = m["id"].toInt();

    if (id < 0) {
        QMessageBox::warning(this, "Delete", "Student not found.");
        return;
    }

    if (!StudentManager::deleteStudent(id)) {
        QMessageBox::warning(this, "Delete", "Delete failed.");
        return;
    }

    QMessageBox::information(this, "Delete", "Deleted successfully!");
    clearFields();
}

void AdminDashboard::loadRequests()
{
    auto all = RequestManager::getAllRequests();

    ui->tblRequests->clear();
    ui->tblRequests->setColumnCount(5);
    ui->tblRequests->setHorizontalHeaderLabels(QStringList()
                                               << "ID"
                                               << "Roll"
                                               << "Name"
                                               << "Message"
                                               << "Resolved");

    ui->tblRequests->setRowCount(all.size());

    for (int i = 0; i < all.size(); ++i) {
        auto &r = all[i];

        ui->tblRequests->setItem(i, 0, new QTableWidgetItem(r["id"]));
        ui->tblRequests->setItem(i, 1, new QTableWidgetItem(r["student_roll"]));
        ui->tblRequests->setItem(i, 2, new QTableWidgetItem(r["student_name"]));
        ui->tblRequests->setItem(i, 3, new QTableWidgetItem(r["message"]));
        ui->tblRequests->setItem(i, 4, new QTableWidgetItem(r["resolved"] == "1" ? "Yes" : "No"));
    }
}

void AdminDashboard::on_btnResolved_clicked()
{
    int row = ui->tblRequests->currentRow();

    if (row < 0) {
        QMessageBox::warning(this, "Resolve", "Select a request.");
        return;
    }

    int reqId = ui->tblRequests->item(row, 0)->text().toInt();

    if (RequestManager::markResolved(reqId)) {
        QMessageBox::information(this, "Resolve", "Marked as resolved.");
        loadRequests();
    } else {
        QMessageBox::warning(this, "Resolve", "Failed.");
    }
}

void AdminDashboard::on_btnClear_clicked()
{
    clearFields();
    ui->txtSearchRoll->clear();
}

void AdminDashboard::on_btnAddAdmin_clicked()
{
    QString user = ui->txtAdminUser->text().trimmed();
    QString pass = ui->txtAdminPass->text().trimmed();

    if (user.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this, "Add Admin", "Enter username and password.");
        return;
    }

    QSqlQuery q;
    q.prepare("INSERT INTO users (username, password, role) VALUES (:u, :p, 'admin')");
    q.bindValue(":u", user);
    q.bindValue(":p", pass);

    if (!q.exec()) {
        QMessageBox::warning(this, "Add Admin", "Failed to add admin. Maybe username exists.");
        qWarning() << "add admin failed:" << q.lastError().text();
        return;
    }

    QMessageBox::information(this, "Add Admin", "Admin added successfully!");
    ui->txtAdminUser->clear();
    ui->txtAdminPass->clear();
}
#include "loginwindow.h"

void AdminDashboard::on_btnLogout_clicked()
{
    // Create a new login window
    LoginWindow *login = new LoginWindow();
    login->show();
    // Close or hide this dashboard
    this->close();  // or this->deleteLater();
}


void AdminDashboard::clearFields()
{
    ui->txtRoll->clear();
    ui->txtName->clear();
    ui->txtDept->clear();
    ui->txtPhone->clear();
    ui->txtEmail->clear();
    ui->txtCgpa->clear();
}
