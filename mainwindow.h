#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qgraphicsview.h"
#include <QLabel>
#include <QMainWindow>
#include <QtOpenGLWidgets/qopenglwidget.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	void start_cv();
	bool cv_active = false;

	QLabel *img;

	void display_img(cv::Mat img);

	cv::VideoCapture cam;
	cv::Mat firstframe;

	void reset_frame();
	bool auto_reset = false;

private slots:
	void on_startCaptureButton_clicked();

	void on_stopCaptureButton_clicked();

	void on_resetFrameButton_clicked();

	void on_checkBox_stateChanged(int arg1);

private:
	Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
