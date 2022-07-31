#include "mainwindow.h"
#include "qlabel.h"
#include "ui_mainwindow.h"

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using cv::COLOR_BGR2RGB;

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	this->img = this->findChild<QLabel *>("img");
	this->cam = cv::VideoCapture(0);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::start_cv() {

	std::vector<std::vector<cv::Point> > cnts;

	cv::Mat frame,delta,thresh;
	cv::Mat frame_default;

	this->reset_frame();

	this->cv_active = true;
	while(cv_active) {
		if(this->auto_reset) this->reset_frame();
		cam >> frame;
		cam >> frame_default;
		cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
		GaussianBlur(frame, frame, cv::Size(21, 21), 0);
		cv::absdiff(firstframe,frame,delta);
		threshold(delta, thresh, 25, 255, cv::THRESH_BINARY);
		dilate(thresh, thresh, cv::Mat(), cv::Point(-1,-1), 2);
		findContours(thresh, cnts, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
		for(auto cnt : cnts) {
			if(contourArea(cnt) < 2000) {
				continue;
			}

			cv::Rect rect = cv::boundingRect(cnt);
			cv::rectangle(frame_default,rect,cv::Scalar(255,100,25),2);

			putText(frame_default, "Motion detected", cv::Point(10, 20), cv::QT_FONT_NORMAL, 0.75, cv::Scalar(0,0,255),2);
		}
		this->display_img(frame_default);
	   cv::waitKey(10);
	}
}

void MainWindow::display_img(cv::Mat img) {
	cv::cvtColor(img, img, COLOR_BGR2RGB);
	QImage image(img.data, img.cols, img.rows, QImage::Format_RGB888);
	QPixmap pixmap = QPixmap::fromImage(image);
	this->img->setPixmap(pixmap);
}

void MainWindow::reset_frame() {
	this->cam >> this->firstframe;
	cvtColor(firstframe, firstframe, cv::COLOR_BGR2GRAY);
	GaussianBlur(firstframe, firstframe, cv::Size(21, 21), 0);
}

void MainWindow::on_startCaptureButton_clicked() {
	this->start_cv();
}


void MainWindow::on_stopCaptureButton_clicked() {
	this->cv_active = false;
}


void MainWindow::on_resetFrameButton_clicked() {
	this->reset_frame();
}


void MainWindow::on_checkBox_stateChanged(int arg1) {
	std::cout << arg1 << std::endl;
	this->auto_reset = arg1;
}

