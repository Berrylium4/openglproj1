#include "QtApplication.h"

QtApplication::QtApplication(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	timerId = startTimer(50);
	QSize size = this->ui.viewport->size();
	this->ui.viewport->screen_width = size.width();
	this->ui.viewport->screen_height = size.height();

	ui.comboBox_Shader->addItem("Off");
	ui.comboBox_Shader->addItem("Per Pixel");
	ui.comboBox_Shader->addItem("Normal Mapping");
	ui.comboBox_Shader->addItem("Graytoon");
	//ui.comboBox_Shader->addItem("Custom1");


}

QtApplication::~QtApplication()
{
	killTimer(timerId);
}

void QtApplication::input_model() {
	QString file_name = QFileDialog::getOpenFileName(this, "Open Model", "../QtWidgetsApplication1/Models");
	//QMessageBox::information(this, "..", file_name); //debug
	std::string temp = file_name.toStdString();

	this->ui.viewport->model_filename = temp;
	this->ui.viewport->refresh_model = true;

	// Parsing for the name of the model
	std::string temp2 = "Current Model: ";

	int index = 0;
	for (int i = temp.length() - 1; i >= 0; i--)
	{
		char letter = temp[i];
		if (letter == '/')
		{
			index = i + 1;
			break;
		}
	}
	for (index; index < temp.length(); index++)
	{
		temp2 += temp[index];
	}
	file_name = QString::fromStdString(temp2);
	ui.title_model->setText(file_name);
	ui.title_model->update();

	this->ui.viewport->update();

}

void QtApplication::mouseMoveEvent(QMouseEvent* event)
{
	this->x = event->x();
	this->y = event->y();
	//cout << "X: " << x << " Y: " << y << endl;

	this->ui.viewport->MouseMove(x, y);
	this->ui.viewport->update();
}
void QtApplication::keyPressEvent(QKeyEvent *ev)
{
	//cout << "Key: " << ev->text().toStdString() << endl;
	this->ui.viewport->KeyPress(ev->text().toStdString());

}

void QtApplication::keyReleaseEvent(QKeyEvent *ev)
{
	//what happens if a key is released

}
void QtApplication::timerEvent(QTimerEvent* event)
{
	//cout << "TIMER CALLED" << endl;
	// mapFromGlobal gives coord relative to Qt window instead of entire computer's window
	this->ui.viewport->mouse_cur[0] = this->mapFromGlobal(QCursor::pos()).x();
	this->ui.viewport->mouse_cur[1] = this->mapFromGlobal(QCursor::pos()).y();


	this->ui.viewport->update();
}


void QtApplication::input_mat_ambient() {
	QColor color = QColorDialog::getColor(Qt::black, this);
	float r, g, b;
	if (color.isValid())
	{
		r = color.red() / 255.0f;
		g = color.green() / 255.0f;
		b = color.blue() / 255.0f;
		this->ui.viewport->mat_ambient[0] = r;
		this->ui.viewport->mat_ambient[1] = g;
		this->ui.viewport->mat_ambient[2] = b;
	}
	string temp = "(";
	string temp2 = "background-color: rgb";
	QString css;

	temp += std::to_string(color.red());
	temp += ",";
	temp += std::to_string(color.green());
	temp += ",";
	temp += std::to_string(color.blue());
	temp += ")";
	temp2 += temp;

	css = QString::fromStdString(temp2);
	this->ui.Mat_Ambient->setStyleSheet(css);
	css = QString::fromStdString(temp);
	this->ui.Mat_Ambient->setText(css);
	this->ui.Mat_Ambient->update();

	this->ui.viewport->update();
}
void QtApplication::input_mat_diffuse() {
	QColor color = QColorDialog::getColor(Qt::black, this);
	float r, g, b;
	if (color.isValid())
	{
		r = color.red() / 255.0f;
		g = color.green() / 255.0f;
		b = color.blue() / 255.0f;
		this->ui.viewport->mat_diffuse[0] = r;
		this->ui.viewport->mat_diffuse[1] = g;
		this->ui.viewport->mat_diffuse[2] = b;
	}
	string temp = "(";
	string temp2 = "background-color: rgb";
	QString css;

	temp += std::to_string(color.red());
	temp += ",";
	temp += std::to_string(color.green());
	temp += ",";
	temp += std::to_string(color.blue());
	temp += ")";
	temp2 += temp;

	css = QString::fromStdString(temp2);
	this->ui.Mat_Diffuse->setStyleSheet(css);
	css = QString::fromStdString(temp);
	this->ui.Mat_Diffuse->setText(css);
	this->ui.Mat_Diffuse->update();

	this->ui.viewport->update();
}
void QtApplication::input_mat_specular() {
	QColor color = QColorDialog::getColor(Qt::black, this);
	float r, g, b;
	if (color.isValid())
	{
		r = color.red() / 255.0f;
		g = color.green() / 255.0f;
		b = color.blue() / 255.0f;
		this->ui.viewport->mat_specular[0] = r;
		this->ui.viewport->mat_specular[1] = g;
		this->ui.viewport->mat_specular[2] = b;
	}
	string temp = "(";
	string temp2 = "background-color: rgb";
	QString css;

	temp += std::to_string(color.red());
	temp += ",";
	temp += std::to_string(color.green());
	temp += ",";
	temp += std::to_string(color.blue());
	temp += ")";
	temp2 += temp;

	css = QString::fromStdString(temp2);
	this->ui.Mat_Specular->setStyleSheet(css);
	css = QString::fromStdString(temp);
	this->ui.Mat_Specular->setText(css);
	this->ui.Mat_Specular->update();

	this->ui.viewport->update();
}
void QtApplication::input_mat_shininess(int shiny) {
	this->ui.viewport->mat_shininess[0] = shiny;
	this->ui.viewport->update();
}

void QtApplication::input_shader(int index) {
	switch (index) {
	case 0:
		this->ui.viewport->shadertype = 0;
		break;
	case 1:
		this->ui.viewport->shadertype = 1;
		break;
	case 2:
		this->ui.viewport->shadertype = 2;
		break;
	case 3:
		this->ui.viewport->shadertype = 3;
		break;
	}
	this->ui.viewport->changedshaders = true;
	this->ui.viewport->update();
}


void QtApplication::lights1_on(bool toggled) {
	if (toggled)
	{
		this->ui.viewport->light1on = true;
	}
	else
	{
		this->ui.viewport->light1on = false;
	}
	this->ui.viewport->update_lights = true;
	this->ui.viewport->changedshaders = true; //update shader
	this->ui.viewport->update();

}


void QtApplication::lights1_pos_x(int num) {
	this->ui.viewport->light1_position[0] = num;
	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}
void QtApplication::lights1_pos_y(int num) {
	this->ui.viewport->light1_position[1] = num;
	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}
void QtApplication::lights1_pos_z(int num) {
	this->ui.viewport->light1_position[2] = num;
	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}

void QtApplication::lights1_ambient() {
	QColor color = QColorDialog::getColor(Qt::black, this);
	float r, g, b;
	if (color.isValid())
	{
		r = color.red() / 255.0f;
		g = color.green() / 255.0f;
		b = color.blue() / 255.0f;
		this->ui.viewport->light1_ambient[0] = r;
		this->ui.viewport->light1_ambient[1] = g;
		this->ui.viewport->light1_ambient[2] = b;
	}
	string temp = "(";
	string temp2 = "background-color: rgb";
	QString css;

	temp += std::to_string(color.red());
	temp += ",";
	temp += std::to_string(color.green());
	temp += ",";
	temp += std::to_string(color.blue());
	temp += ")";
	temp2 += temp;

	css = QString::fromStdString(temp2);
	this->ui.Light1_Ambient->setStyleSheet(css);
	css = QString::fromStdString(temp);
	this->ui.Light1_Ambient->setText(css);
	this->ui.Light1_Ambient->update();

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}
void QtApplication::lights1_diffuse() {
	QColor color = QColorDialog::getColor(Qt::black, this);
	float r, g, b;
	if (color.isValid())
	{
		r = color.red() / 255.0f;
		g = color.green() / 255.0f;
		b = color.blue() / 255.0f;
		this->ui.viewport->light1_diffuse[0] = r;
		this->ui.viewport->light1_diffuse[1] = g;
		this->ui.viewport->light1_diffuse[2] = b;
	}
	string temp = "(";
	string temp2 = "background-color: rgb";
	QString css;

	temp += std::to_string(color.red());
	temp += ",";
	temp += std::to_string(color.green());
	temp += ",";
	temp += std::to_string(color.blue());
	temp += ")";
	temp2 += temp;

	css = QString::fromStdString(temp2);
	this->ui.Light1_Diffuse->setStyleSheet(css);
	css = QString::fromStdString(temp);
	this->ui.Light1_Diffuse->setText(css);
	this->ui.Light1_Diffuse->update();

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}
void QtApplication::lights1_specular() {
	QColor color = QColorDialog::getColor(Qt::black, this);
	float r, g, b;
	if (color.isValid())
	{
		r = color.red() / 255.0f;
		g = color.green() / 255.0f;
		b = color.blue() / 255.0f;
		this->ui.viewport->light1_specular[0] = r;
		this->ui.viewport->light1_specular[1] = g;
		this->ui.viewport->light1_specular[2] = b;
	}
	string temp = "(";
	string temp2 = "background-color: rgb";
	QString css;

	temp += std::to_string(color.red());
	temp += ",";
	temp += std::to_string(color.green());
	temp += ",";
	temp += std::to_string(color.blue());
	temp += ")";
	temp2 += temp;

	css = QString::fromStdString(temp2);
	this->ui.Light1_Specular->setStyleSheet(css);
	css = QString::fromStdString(temp);
	this->ui.Light1_Specular->setText(css);
	this->ui.Light1_Specular->update();

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}

void QtApplication::lights1_point(bool toggled) {
	if (toggled)
	{
		this->ui.viewport->light1_directional = false;
		this->ui.viewport->light1_spotlight = false;
		this->ui.Light1_Directional->setChecked(false);
		this->ui.Light1_Spotlight->setChecked(false);

	}

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();

}
void QtApplication::lights1_direction(bool toggled) {
	if (toggled)
	{
		this->ui.viewport->light1_directional = true;
		this->ui.viewport->light1_spotlight = false;
		this->ui.Light1_Point->setChecked(false);
		this->ui.Light1_Spotlight->setChecked(false);

	}

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();

}

void QtApplication::lights1_spot(bool toggled) {
	if (toggled)
	{
		this->ui.viewport->light1_spotlight = true;
		this->ui.Light1_Point->setChecked(false);
		this->ui.Light1_Directional->setChecked(false);
	}
	else 
	{
		this->ui.viewport->light1_spotlight = false;
	}

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}

void QtApplication::lights1_dir_x(int num) {
	this->ui.viewport->light1_direction[0] = num;

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}
void QtApplication::lights1_dir_y(int num) {
	this->ui.viewport->light1_direction[1] = num;

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}
void QtApplication::lights1_dir_z(int num) {
	this->ui.viewport->light1_direction[2] = num;

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}

void QtApplication::lights1_constant(double val) {
	this->ui.viewport->light1_constant[0] = val;
	
	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}
void QtApplication::lights1_linear(double val) {
	this->ui.viewport->light1_linear[0] = val;

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}
void QtApplication::lights1_quadratic(double val) {
	this->ui.viewport->light1_quadratic[0] = val;

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}

void QtApplication::lights1_spot_cutoff(double val) {
	this->ui.viewport->light1_spot_cutoff[0] = val;
	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}
void QtApplication::lights1_spot_exp(double val) {
	this->ui.viewport->light1_spot_exp[0] = val;
	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}

//=============================================================
// LIGHT 2

void QtApplication::lights2_on(bool toggled) {
	if (toggled)
	{
		this->ui.viewport->light2on = true;
	}
	else
	{
		this->ui.viewport->light2on = false;
	}
	this->ui.viewport->update_lights = true;
	this->ui.viewport->changedshaders = true; //update shader
	this->ui.viewport->update();

}


void QtApplication::lights2_pos_x(int num) {
	this->ui.viewport->light2_position[0] = num;
	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}
void QtApplication::lights2_pos_y(int num) {
	this->ui.viewport->light2_position[1] = num;
	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}
void QtApplication::lights2_pos_z(int num) {
	this->ui.viewport->light2_position[2] = num;
	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}

void QtApplication::lights2_ambient() {
	QColor color = QColorDialog::getColor(Qt::black, this);
	float r, g, b;
	if (color.isValid())
	{
		r = color.red() / 255.0f;
		g = color.green() / 255.0f;
		b = color.blue() / 255.0f;
		this->ui.viewport->light2_ambient[0] = r;
		this->ui.viewport->light2_ambient[1] = g;
		this->ui.viewport->light2_ambient[2] = b;
	}
	string temp = "(";
	string temp2 = "background-color: rgb";
	QString css;

	temp += std::to_string(color.red());
	temp += ",";
	temp += std::to_string(color.green());
	temp += ",";
	temp += std::to_string(color.blue());
	temp += ")";
	temp2 += temp;

	css = QString::fromStdString(temp2);
	this->ui.Light2_Ambient->setStyleSheet(css);
	css = QString::fromStdString(temp);
	this->ui.Light2_Ambient->setText(css);
	this->ui.Light2_Ambient->update();

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}
void QtApplication::lights2_diffuse() {
	QColor color = QColorDialog::getColor(Qt::black, this);
	float r, g, b;
	if (color.isValid())
	{
		r = color.red() / 255.0f;
		g = color.green() / 255.0f;
		b = color.blue() / 255.0f;
		this->ui.viewport->light2_diffuse[0] = r;
		this->ui.viewport->light2_diffuse[1] = g;
		this->ui.viewport->light2_diffuse[2] = b;
	}
	string temp = "(";
	string temp2 = "background-color: rgb";
	QString css;

	temp += std::to_string(color.red());
	temp += ",";
	temp += std::to_string(color.green());
	temp += ",";
	temp += std::to_string(color.blue());
	temp += ")";
	temp2 += temp;

	css = QString::fromStdString(temp2);
	this->ui.Light2_Diffuse->setStyleSheet(css);
	css = QString::fromStdString(temp);
	this->ui.Light2_Diffuse->setText(css);
	this->ui.Light2_Diffuse->update();

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}
void QtApplication::lights2_specular() {
	QColor color = QColorDialog::getColor(Qt::black, this);
	float r, g, b;
	if (color.isValid())
	{
		r = color.red() / 255.0f;
		g = color.green() / 255.0f;
		b = color.blue() / 255.0f;
		this->ui.viewport->light2_specular[0] = r;
		this->ui.viewport->light2_specular[1] = g;
		this->ui.viewport->light2_specular[2] = b;
	}
	string temp = "(";
	string temp2 = "background-color: rgb";
	QString css;

	temp += std::to_string(color.red());
	temp += ",";
	temp += std::to_string(color.green());
	temp += ",";
	temp += std::to_string(color.blue());
	temp += ")";
	temp2 += temp;

	css = QString::fromStdString(temp2);
	this->ui.Light2_Specular->setStyleSheet(css);
	css = QString::fromStdString(temp);
	this->ui.Light2_Specular->setText(css);
	this->ui.Light2_Specular->update();

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}

void QtApplication::lights2_point(bool toggled) {
	if (toggled)
	{
		this->ui.viewport->light2_directional = false;
		this->ui.viewport->light2_spotlight = false;
		this->ui.Light2_Directional->setChecked(false);
		this->ui.Light2_Spotlight->setChecked(false);

	}

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();

}
void QtApplication::lights2_direction(bool toggled) {
	if (toggled)
	{
		this->ui.viewport->light2_directional = true;
		this->ui.viewport->light2_spotlight = false;
		this->ui.Light2_Point->setChecked(false);
		this->ui.Light2_Spotlight->setChecked(false);

	}

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();

}

void QtApplication::lights2_spot(bool toggled) {
	if (toggled)
	{
		this->ui.viewport->light2_spotlight = true;
		this->ui.Light2_Point->setChecked(false);
		this->ui.Light2_Directional->setChecked(false);
	}
	else
	{
		this->ui.viewport->light2_spotlight = false;
	}

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}

void QtApplication::lights2_dir_x(int num) {
	this->ui.viewport->light2_direction[0] = num;

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}
void QtApplication::lights2_dir_y(int num) {
	this->ui.viewport->light2_direction[1] = num;

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}
void QtApplication::lights2_dir_z(int num) {
	this->ui.viewport->light2_direction[2] = num;

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}

void QtApplication::lights2_constant(double val) {
	this->ui.viewport->light2_constant[0] = val;

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}
void QtApplication::lights2_linear(double val) {
	this->ui.viewport->light2_linear[0] = val;

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}
void QtApplication::lights2_quadratic(double val) {
	this->ui.viewport->light2_quadratic[0] = val;

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}

void QtApplication::lights2_spot_cutoff(double val) {
	this->ui.viewport->light2_spot_cutoff[0] = val;
	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}
void QtApplication::lights2_spot_exp(double val) {
	this->ui.viewport->light2_spot_exp[0] = val;
	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}

//=============================================================
// LIGHT 3

void QtApplication::lights3_on(bool toggled) {
	if (toggled)
	{
		this->ui.viewport->light3on = true;
	}
	else
	{
		this->ui.viewport->light3on = false;
	}
	this->ui.viewport->update_lights = true;
	this->ui.viewport->changedshaders = true; //update shader
	this->ui.viewport->update();

}


void QtApplication::lights3_pos_x(int num) {
	this->ui.viewport->light3_position[0] = num;
	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}
void QtApplication::lights3_pos_y(int num) {
	this->ui.viewport->light3_position[1] = num;
	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}
void QtApplication::lights3_pos_z(int num) {
	this->ui.viewport->light3_position[2] = num;
	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}

void QtApplication::lights3_ambient() {
	QColor color = QColorDialog::getColor(Qt::black, this);
	float r, g, b;
	if (color.isValid())
	{
		r = color.red() / 255.0f;
		g = color.green() / 255.0f;
		b = color.blue() / 255.0f;
		this->ui.viewport->light3_ambient[0] = r;
		this->ui.viewport->light3_ambient[1] = g;
		this->ui.viewport->light3_ambient[2] = b;
	}
	string temp = "(";
	string temp2 = "background-color: rgb";
	QString css;

	temp += std::to_string(color.red());
	temp += ",";
	temp += std::to_string(color.green());
	temp += ",";
	temp += std::to_string(color.blue());
	temp += ")";
	temp2 += temp;

	css = QString::fromStdString(temp2);
	this->ui.Light3_Ambient->setStyleSheet(css);
	css = QString::fromStdString(temp);
	this->ui.Light3_Ambient->setText(css);
	this->ui.Light3_Ambient->update();

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}
void QtApplication::lights3_diffuse() {
	QColor color = QColorDialog::getColor(Qt::black, this);
	float r, g, b;
	if (color.isValid())
	{
		r = color.red() / 255.0f;
		g = color.green() / 255.0f;
		b = color.blue() / 255.0f;
		this->ui.viewport->light3_diffuse[0] = r;
		this->ui.viewport->light3_diffuse[1] = g;
		this->ui.viewport->light3_diffuse[2] = b;
	}
	string temp = "(";
	string temp2 = "background-color: rgb";
	QString css;

	temp += std::to_string(color.red());
	temp += ",";
	temp += std::to_string(color.green());
	temp += ",";
	temp += std::to_string(color.blue());
	temp += ")";
	temp2 += temp;

	css = QString::fromStdString(temp2);
	this->ui.Light3_Diffuse->setStyleSheet(css);
	css = QString::fromStdString(temp);
	this->ui.Light3_Diffuse->setText(css);
	this->ui.Light3_Diffuse->update();

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}
void QtApplication::lights3_specular() {
	QColor color = QColorDialog::getColor(Qt::black, this);
	float r, g, b;
	if (color.isValid())
	{
		r = color.red() / 255.0f;
		g = color.green() / 255.0f;
		b = color.blue() / 255.0f;
		this->ui.viewport->light3_specular[0] = r;
		this->ui.viewport->light3_specular[1] = g;
		this->ui.viewport->light3_specular[2] = b;
	}
	string temp = "(";
	string temp2 = "background-color: rgb";
	QString css;

	temp += std::to_string(color.red());
	temp += ",";
	temp += std::to_string(color.green());
	temp += ",";
	temp += std::to_string(color.blue());
	temp += ")";
	temp2 += temp;

	css = QString::fromStdString(temp2);
	this->ui.Light3_Specular->setStyleSheet(css);
	css = QString::fromStdString(temp);
	this->ui.Light3_Specular->setText(css);
	this->ui.Light3_Specular->update();

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}

void QtApplication::lights3_point(bool toggled) {
	if (toggled)
	{
		this->ui.viewport->light3_directional = false;
		this->ui.viewport->light3_spotlight = false;
		this->ui.Light3_Directional->setChecked(false);
		this->ui.Light3_Spotlight->setChecked(false);

	}

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();

}
void QtApplication::lights3_direction(bool toggled) {
	if (toggled)
	{
		this->ui.viewport->light3_directional = true;
		this->ui.viewport->light3_spotlight = false;
		this->ui.Light3_Point->setChecked(false);
		this->ui.Light3_Spotlight->setChecked(false);

	}

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();

}

void QtApplication::lights3_spot(bool toggled) {
	if (toggled)
	{
		this->ui.viewport->light3_spotlight = true;
		this->ui.Light3_Point->setChecked(false);
		this->ui.Light3_Directional->setChecked(false);
	}
	else
	{
		this->ui.viewport->light3_spotlight = false;
	}

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}

void QtApplication::lights3_dir_x(int num) {
	this->ui.viewport->light3_direction[0] = num;

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}
void QtApplication::lights3_dir_y(int num) {
	this->ui.viewport->light3_direction[1] = num;

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}
void QtApplication::lights3_dir_z(int num) {
	this->ui.viewport->light3_direction[2] = num;

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}

void QtApplication::lights3_constant(double val) {
	this->ui.viewport->light3_constant[0] = val;

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}
void QtApplication::lights3_linear(double val) {
	this->ui.viewport->light3_linear[0] = val;

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}
void QtApplication::lights3_quadratic(double val) {
	this->ui.viewport->light3_quadratic[0] = val;

	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}

void QtApplication::lights3_spot_cutoff(double val) {
	this->ui.viewport->light3_spot_cutoff[0] = val;
	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}
void QtApplication::lights3_spot_exp(double val) {
	this->ui.viewport->light3_spot_exp[0] = val;
	this->ui.viewport->update_lights = true;
	this->ui.viewport->update();
}