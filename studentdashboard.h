#ifndef STUDENTDASHBOARD_H
#define STUDENTDASHBOARD_H

#include <QWidget>

namespace Ui { class StudentDashboard; }

class StudentDashboard : public QWidget
{
    Q_OBJECT

public:
    explicit StudentDashboard(int studentId, QWidget *parent = nullptr);
    ~StudentDashboard();

private slots:
    void on_btnSubmit_clicked();
    void on_btnChangePass_clicked();
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    Ui::StudentDashboard *ui;
    int id;
    void loadProfile();
    void loadRequests();
};

#endif // STUDENTDASHBOARD_H
