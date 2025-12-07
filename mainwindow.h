#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString &username,
                        const QString &role,
                        QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleOpenAdminClicked();
    void handleOpenStudentClicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::MainWindow *ui;
    QString username;
    QString role;
};

#endif // MAINWINDOW_H
