#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qgraphicsview.h"
#include <QLabel>
#include <QMainWindow>
#include <qcombobox.h>
#include <QtOpenGLWidgets/qopenglwidget.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>

enum FrameLayer {
	DEFAULT,
	DELTA,
	GRAYSCALE,
	CONTOUR,
};

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
	QLabel *contour_area;
	QLabel *blur_radius;
	QLabel *diff_thresh_label;
	QLabel *capture_label;
	QLabel *motion_label;
	QComboBox *display_layer;

	void display_img(cv::Mat img);

	cv::VideoCapture cam;
	cv::Mat firstframe;

	void reset_frame();
	void preprocess_frame(cv::Mat&);
	bool auto_reset = false;
	int min_coutour_area = 1000;
	int blur_size = 21;
	int diff_threshold = 25;

	FrameLayer get_selected_layer();

private slots:
	void on_startCaptureButton_clicked();

	void on_stopCaptureButton_clicked();

	void on_resetFrameButton_clicked();

	void on_checkBox_stateChanged(int arg1);

	void on_contourSizeThreshold_valueChanged(int position);

	void on_blurRadius_valueChanged(int value);

	void on_threshold_valueChanged(int value);

private:
	Ui::MainWindow *ui;
	void update_area_label();
	void update_blur_label();
	void update_thresh_label();
};
#endif // MAINWINDOW_H
