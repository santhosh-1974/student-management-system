#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "admindashboard.h"
#include "studentdashboard.h"
#include "authmanager.h"

#include <QPushButton>
#include <QDebug>
#include <QStyleOption>
#include <QPainter>

MainWindow::MainWindow(const QString &username,
                       const QString &role,
                       QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    username(username),
    role(role)
{
    ui->setupUi(this);

    this->setFixedSize(1000, 650);

    qDebug() << "MainWindow ctor called, role =" << role;

    // Show/hide buttons based on role
    if (role == "admin") {
        ui->btnOpenStudent->setVisible(false);
    } else {
        ui->btnOpenAdmin->setVisible(false);
    }

    // Manual only connections — avoid auto-connect / duplicate
    connect(ui->btnOpenAdmin, &QPushButton::clicked,
            this, &MainWindow::handleOpenAdminClicked,
            Qt::UniqueConnection);
    connect(ui->btnOpenStudent, &QPushButton::clicked,
            this, &MainWindow::handleOpenStudentClicked,
            Qt::UniqueConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleOpenAdminClicked()
{
    if (role != "admin") return;

    qDebug() << "Opening AdminDashboard";
    AdminDashboard *adm = new AdminDashboard();
    adm->setAttribute(Qt::WA_DeleteOnClose);
    adm->show();
    this->close();  // or this->hide()

}

void MainWindow::handleOpenStudentClicked()
{
    int sid = AuthManager::getStudentId(username);

    qDebug() << "Opening StudentDashboard";
    StudentDashboard *sd = new StudentDashboard(sid);
    sd->setAttribute(Qt::WA_DeleteOnClose);
    sd->show();
    this->close();  // or this->hide()

}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QMainWindow::paintEvent(event);
}
