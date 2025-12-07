#ifndef ADMINDASHBOARD_H
#define ADMINDASHBOARD_H

#include <QWidget>

namespace Ui { class AdminDashboard; }

class StudentManager;
class RequestManager;

class AdminDashboard : public QWidget
{
    Q_OBJECT

public:
    explicit AdminDashboard(QWidget *parent = nullptr);
    ~AdminDashboard();

private slots:
    void on_btnSearch_clicked();
    void on_btnAdd_clicked();
    void on_btnUpdate_clicked();
    void on_btnDelete_clicked();
    void on_btnResolved_clicked();
    void on_btnClear_clicked();
    void on_btnAddAdmin_clicked();
    void on_btnLogout_clicked();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::AdminDashboard *ui;
    StudentManager *studentManager;
    RequestManager *requestManager;
    void loadRequests();
    void clearFields();
};

#endif // ADMINDASHBOARD_H
