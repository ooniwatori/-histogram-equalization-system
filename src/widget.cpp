#include "widget.h"
#include "ui_widget.h"
#include"ImageProcessing.h"
#include<QPixmap>
#include<QFileDialog>
#include<QMessageBox>
#include<string>
#include<QDir>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}
void Widget::on_SelectImageBtn_clicked()
{
    QString filename=QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                QDir::homePath(),
                "Portable Gray Map (*.pgm)"
                );
    // open graph and show
    QPixmap original_graph(filename);
    imagePath = filename.toStdString();
    int w = ui->OriginalImageLabel->width();
    int h = ui->OriginalImageLabel->height();
    IP = new ImageProcessing(imagePath);
    if(!IP->isLoad()) return;
    ui->OriginalImageLabel->setPixmap(original_graph.scaled(w,h,Qt::KeepAspectRatio));
    // output intensity graph and show
    IP->OriginalIntensityGraph();
    QPixmap original_intensity_graph(QString::fromStdString(IP->getOriginalIntensityPath()));
    ui->OriginalIntensityLabel->setPixmap(original_intensity_graph.scaled(w+1,h+1,Qt::KeepAspectRatio));
}


void Widget::on_ProcessBtn_clicked()
{
    if(IP->isLoad()){
        // equalization by setting value
        IP->Equalization();

        // output processed image
        QString save_file_name = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::homePath(), "Portable Gray Map (*.pgm)");
        IP->outputImage(save_file_name.toStdString(), ImageProcessing::P2);

        // load processed image and show it
        int w = ui->ProcessedImageLabel->width();
        int h = ui->ProcessedImageLabel->height();
        QPixmap proccessed_graph(save_file_name);
        ui->ProcessedImageLabel->setPixmap(proccessed_graph.scaled(w,h,Qt::KeepAspectRatio));

        // output processed intensity graph and show
        // output intensity transfer function graph and show
        IP->ProcessedIntensityGraph();
        IP->IntensityFuncGraph();
        QPixmap processed_intensity_graph(QString::fromStdString(IP->getProcessedIntensityPath())),
                intensity_function_graph(QString::fromStdString(IP->getFunctionPath()));

        ui->ProcessedIntensityLabel->setPixmap(processed_intensity_graph.scaled(w+1,h+1,Qt::KeepAspectRatio));
        ui->IntensityFunctionLabel->setPixmap(intensity_function_graph.scaled(w,h,Qt::KeepAspectRatio));
    }
}



