#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include"ImageProcessing.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_SelectImageBtn_clicked();

    void on_ProcessBtn_clicked();


private:
    Ui::Widget *ui;
    std::string imagePath;
    ImageProcessing* IP;
};
#endif // WIDGET_H
