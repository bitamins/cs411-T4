#ifndef NEWSPAGES_H
#define NEWSPAGES_H

#include <QWidget>
#include <QLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>

class newsPages : public QWidget
{
    Q_OBJECT

private:
    QHBoxLayout pageLayout;
    QLabel pageLeft;
    QLabel pageRight;
    QSpinBox pageBox;

public:
    explicit newsPages(QWidget *parent = nullptr);

signals:

public slots:
    int on_left_clicked();
    int on_right_clicked();
    int on_spinbox_changed();
};

#endif // NEWSPAGES_H
