#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtApplication.h"

#include <qfiledialog.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QColorDialog>

#include <QMouseEvent>
#include <QKeyEvent>
#include <QEvent>
#include <QDebug>

#include <string>

class QtApplication : public QMainWindow
{
    Q_OBJECT

public:
    QtApplication(QWidget *parent = Q_NULLPTR);
	~QtApplication(); // destructor

	int x, y;
	bool pressed;

	void mouseMoveEvent(QMouseEvent* event);
	void keyPressEvent(QKeyEvent *ev);
	void keyReleaseEvent(QKeyEvent *ev);

	int timerId;
	void timerEvent(QTimerEvent* event);




public slots:
	void input_model();
	void input_mat_ambient();
	void input_mat_diffuse();
	void input_mat_specular();
	void input_mat_shininess(int shiny);

	void input_shader(int index);

	//lights 1 ===================
	void lights1_on(bool toggled);
	void lights1_pos_x(int num);
	void lights1_pos_y(int num);
	void lights1_pos_z(int num);

	void lights1_ambient();
	void lights1_diffuse();
	void lights1_specular();

	void lights1_point(bool toggled);
	void lights1_direction(bool toggled);
	void lights1_spot(bool toggled);

	void lights1_dir_x(int num);
	void lights1_dir_y(int num);
	void lights1_dir_z(int num);

	void lights1_constant(double val);
	void lights1_linear(double val);
	void lights1_quadratic(double val);

	void lights1_spot_cutoff(double val);
	void lights1_spot_exp(double val);

	//========================================
	void lights2_on(bool toggled);
	void lights2_pos_x(int num);
	void lights2_pos_y(int num);
	void lights2_pos_z(int num);
			   
	void lights2_ambient();
	void lights2_diffuse();
	void lights2_specular();
			   
	void lights2_point(bool toggled);
	void lights2_direction(bool toggled);
	void lights2_spot(bool toggled);
			   
	void lights2_dir_x(int num);
	void lights2_dir_y(int num);
	void lights2_dir_z(int num);
			   
	void lights2_constant(double val);
	void lights2_linear(double val);
	void lights2_quadratic(double val);
			   
	void lights2_spot_cutoff(double val);
	void lights2_spot_exp(double val);

	//========================================
	void lights3_on(bool toggled);
	void lights3_pos_x(int num);
	void lights3_pos_y(int num);
	void lights3_pos_z(int num);
			   
	void lights3_ambient();
	void lights3_diffuse();
	void lights3_specular();
			   
	void lights3_point(bool toggled);
	void lights3_direction(bool toggled);
	void lights3_spot(bool toggled);
			   
	void lights3_dir_x(int num);
	void lights3_dir_y(int num);
	void lights3_dir_z(int num);
			   
	void lights3_constant(double val);
	void lights3_linear(double val);
	void lights3_quadratic(double val);
			   
	void lights3_spot_cutoff(double val);
	void lights3_spot_exp(double val);



private:
    Ui::QtApplicationClass ui;
};
