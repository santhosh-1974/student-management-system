#include "loginwindow.h"
#include "ui_loginwindow.h"

#include "authmanager.h"
#include "mainwindow.h"

#include <QMessageBox>
#include <QLineEdit>
#include <QDebug>
#include <QStyleOption>
#include <QPainter>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    this->setFixedSize(1000, 650);

    // Set object name for style-sheet
    setObjectName("LoginWindow");

    // Hide password text
    if (auto pwd = findChild<QLineEdit*>("txtPassword")) {
        pwd->setEchoMode(QLineEdit::Password);
    }

    // NOTE: Do NOT manually connect btnLogin->clicked to on_btnLogin_clicked()
    // — we rely on Qt auto-connect using the naming convention.
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}

void LoginWindow::on_btnLogin_clicked()
{
    QString username = ui->txtUsername->text().trimmed();
    QString password = ui->txtPassword->text().trimmed();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Login Failed",
                             "Please enter both username and password.");
        return;
    }

    QString role = AuthManager::login(username, password);
    if (role.isEmpty()) {
        QMessageBox::warning(this, "Login Failed",
                             "Invalid username or password!");
        return;
    }

    MainWindow *mw = new MainWindow(username, role);
    mw->show();
    this->hide();  // hide login window — don’t close, to avoid quitting the app accidentally
}
