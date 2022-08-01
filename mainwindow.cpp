#include "mainwindow.h"
#include "qlabel.h"
#include "ui_mainwindow.h"

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc/imgproc.hpp>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	this->img = this->findChild<QLabel*>("img");
	this->contour_area = this->findChild<QLabel*>("contourAreaLabel");
	this->blur_radius = this->findChild<QLabel*>("blurRadiusLabel");
	this->diff_thresh_label = this->findChild<QLabel*>("thresholdLabel");
	this->display_layer = this->findChild<QComboBox*>("displayLayer");

	this->capture_label = this->findChild<QLabel*>("capture_label");
	this->motion_label = this->findChild<QLabel*>("motion_label");

	this->display_layer->addItem("Default",QVariant(DEFAULT));
	this->display_layer->addItem("Grayscale",QVariant(GRAYSCALE));
	this->display_layer->addItem("Delta",QVariant(DELTA));
	this->display_layer->addItem("Contour",QVariant(CONTOUR));

	this->update_area_label();
	this->update_blur_label();
	this->update_thresh_label();

	this->capture_label->setText("NO");
	this->motion_label->setText("NO");

	this->cam = cv::VideoCapture(0);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::start_cv() {

	std::vector<std::vector<cv::Point> > cnts;

	cv::Mat frame_grayscale,delta,thresh,display_target;
	cv::Mat frame_default;

	this->reset_frame();

	this->cv_active = true;
	int frames = 0;
	this->capture_label->setText("YES");
	while(cv_active) {
		this->motion_label->setText("NO");
		if(this->auto_reset and ++frames%2 == 0) this->reset_frame();
		cam >> frame_grayscale;
		this->preprocess_frame(frame_grayscale);

		cv::absdiff(firstframe,frame_grayscale,delta);
		threshold(delta, thresh, this->diff_threshold, 255, cv::THRESH_BINARY);
		dilate(thresh, thresh, cv::Mat(), cv::Point(-1,-1), 2);

		findContours(thresh, cnts, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
		switch(this->get_selected_layer()) {
			case DEFAULT:
				cam >> display_target;
				break;
			case DELTA:
				display_target = delta;
				break;
			case GRAYSCALE:
				display_target = frame_grayscale;
				break;
			case CONTOUR:
				display_target = thresh;
				break;
		}
		for(auto cnt : cnts) {
			if(contourArea(cnt) < this->min_coutour_area) {
				continue;
			}

			cv::Rect rect = cv::boundingRect(cnt);
			cv::rectangle(display_target,rect,cv::Scalar(255,100,25),2);

//			putText(display_target, "Motion detected", cv::Point(10, 20), cv::QT_FONT_NORMAL, 0.75, cv::Scalar(0,0,255),2);
			this->motion_label->setText("YES");
		}
		this->display_img(display_target);
	   cv::waitKey(25);
	}
}

void MainWindow::display_img(cv::Mat img) {
	cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
	QImage image(img.data, img.cols, img.rows, QImage::Format_RGB888);
	QPixmap pixmap = QPixmap::fromImage(image);
	this->img->setPixmap(pixmap);
}

void MainWindow::reset_frame() {
	this->cam >> this->firstframe;
	cvtColor(firstframe, firstframe, cv::COLOR_BGR2GRAY);
	GaussianBlur(firstframe, firstframe, cv::Size(this->blur_size,this->blur_size), 0);
}

void MainWindow::preprocess_frame(cv::Mat &frame) {
	cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
	GaussianBlur(frame, frame, cv::Size(this->blur_size,this->blur_size), 0);
}

FrameLayer MainWindow::get_selected_layer() {
	QVariant var = this->display_layer->currentData();
	FrameLayer layer = var.value<FrameLayer>();
	return layer;
}

void MainWindow::on_startCaptureButton_clicked() {
	this->start_cv();
}


void MainWindow::on_stopCaptureButton_clicked() {
	this->capture_label->setText("NO");
	this->cv_active = false;
}


void MainWindow::on_resetFrameButton_clicked() {
	this->reset_frame();
}


void MainWindow::on_checkBox_stateChanged(int arg1) {
	this->auto_reset = arg1;
}




void MainWindow::update_area_label() {
	std::string label = "Contour area: "+std::to_string(this->min_coutour_area);
	QString qLabel = QString::fromStdString(label);
	this->contour_area->setText(qLabel);
}

void MainWindow::update_blur_label() {
	std::string label = "Blur size: "+std::to_string(this->blur_size);
	QString qLabel = QString::fromStdString(label);
	this->blur_radius->setText(qLabel);
}

void MainWindow::update_thresh_label() {
	std::string label = "Diff Threshold: "+std::to_string(this->diff_threshold);
	QString qLabel = QString::fromStdString(label);
	this->diff_thresh_label->setText(qLabel);
}

void MainWindow::on_contourSizeThreshold_valueChanged(int position) {
	this->min_coutour_area = position;
	this->update_area_label();
	this->reset_frame();
}

void MainWindow::on_blurRadius_valueChanged(int position) {
	position = 2*position+1;
	this->blur_size = position;
	this->update_blur_label();
	this->reset_frame();
}

void MainWindow::on_threshold_valueChanged(int value) {
	this->diff_threshold = value;
	this->update_thresh_label();
	this->reset_frame();
}

