#ifndef QVIEW
#define QVIEW

#include <GL/glew.h>
#include <QOpenGLWidget>
#include <vector>
#include <iostream>

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <conio.h>
#include <io.h>

#include "Vector3f.h"
#include "textfile.h"

using std::cout;
using std::endl;
using std::string;

#define MAX_VERTICES 8000 // Max number of vertices (for each object)
#define MAX_POLYGONS 8000 // Max number of polygons (for each object)
const static double pi = 3.1415926535897932;


class vertex_type {
public:
	float x = 0;
	float y = 0;
	float z = 0;
};

// The polygon (triangle), 3 numbers that aim 3 vertices

class polygon_type {
public:
	int a = 0;
	int b = 0;
	int c = 0;
};

// The mapcoord type, 2 texture coordinates for each vertex

class mapcoord_type {
public:
	float u = 0;
	float v = 0;
};

// The object type

class obj_type {
public:
	char name[20];

	int vertices_qty;
	int polygons_qty;

	vertex_type vertex[MAX_VERTICES];
	polygon_type polygon[MAX_POLYGONS];
	mapcoord_type mapcoord[MAX_VERTICES];
	int id_texture;

	void reset();
};



class QViewport : public QOpenGLWidget {
	Q_OBJECT

public:
	std::string model_filename = "./Models/blend.3ds";

	bool refresh_model = true;

	// CLASS OBJECTS DECLARATION
	vertex_type vertex_type_obj;
	polygon_type polygon_type_obj;
	mapcoord_type mapcoord_type_obj;

	// Now the object is generic, the cube has annoyed us a little bit, or not?
	obj_type object;
	obj_type* obj_type_ptr;


	// The width and height of your window
	int screen_width = 0;
	int screen_height = 0;

	// Absolute rotation values (0-359 degrees) and rotation increments for each frame
	double rotation_x = 0, rotation_x_increment = 0.1;
	double rotation_y = 0, rotation_y_increment = 0.05;
	double rotation_z = 0, rotation_z_increment = 0.03;

	// Flag for rendering as lines or filled polygons
	int filling = 1; //0=OFF 1=ON

	// Counter to keep track of the last loaded texture
	int num_texture = 0;


	// Camera related ====================================================
	int mouse_cur[2] = { 0, 0 };
	int mouse_last[2] = { 0, 0 };


	float rotation_scale = 0.01f;
	int rotation_angle_theta = 0; // horizontal
	int rotation_angle_phi = 0; // vertical
	int rotation_yaw = 0;

	float dist_to_center = 50.0;
	float eye[3] = { 0, 0, dist_to_center };
	float center[3] = { 0, 0, 0 };
	float up[3] = { 0, dist_to_center, 0 };

	float translate_x = 0;
	float translate_y = -8;
	float translate_z = -50;
	//====================================================================
	//Material Related
	GLfloat mat_ambient[4] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat mat_diffuse[4] = { 0.1, 0.5, 0.8, 1.0 };
	GLfloat mat_specular[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[1] = { 0.0 };
	//====================================================================
	//Lighting
	bool update_lights = false;

	bool light1on = true;
	GLfloat light1_ambient[4] = { 0.1, 0.2, 0.4, 1.0 };
	GLfloat light1_diffuse[4] = { 0.1, 0.2, 0.8, 1.0 };
	GLfloat light1_specular[4] = { 0.0, 0.0, 1.0, 1.0 };
	bool light1_directional = false; // false = point light
	bool light1_spotlight = false;
	GLfloat light1_position[4] = { 25.0, 0.0, 3.0 , 0.0 };
	GLfloat light1_direction[4] = { -1.0, 0.0, 0.0 , 0.0 };
	//GL_CONSTANT_ATTENUATION, GL_LINEAR_ATTENUATION, and GL_QUADRATIC_ATTENUATION
	GLfloat light1_constant[1] = { 1.0 };
	GLfloat light1_linear[1] = { 0.0 };
	GLfloat light1_quadratic[1] = { 0.0 };
	GLfloat light1_spot_cutoff[1] = { 45.0 };
	GLfloat light1_spot_exp[1] = { 0.0 };

	bool light2on = false;
	GLfloat light2_ambient[4] = { 0.4, 0.2, 0.1, 1.0 };
	GLfloat light2_diffuse[4] = { 0.8, 0.2, 0.1, 1.0 };
	GLfloat light2_specular[4] = { 1.0, 0.0, 0.0, 1.0 };
	bool light2_directional = false;
	bool light2_spotlight = false;
	GLfloat light2_position[4] = { 0.0, 25.0, 0.0 , 0.0 };
	GLfloat light2_direction[4] = { -1.0, 0.0, 0.0 , 0.0 };
	GLfloat light2_constant[1] = { 1.0 };
	GLfloat light2_linear[1] = { 0.0 };
	GLfloat light2_quadratic[1] = { 0.0 };
	GLfloat light2_spot_cutoff[1] = { 45.0 };
	GLfloat light2_spot_exp[1] = { 0.0 };


	bool light3on = false;
	GLfloat light3_ambient[4] = { 0.1, 0.4, 0.2, 1.0 };
	GLfloat light3_diffuse[4] = { 0.1, 0.8, 0.2, 1.0 };
	GLfloat light3_specular[4] = { 0.0, 1.0, 0.0, 1.0 };
	bool light3_directional = false;
	bool light3_spotlight = false;
	GLfloat light3_position[4] = { 0.0, 0.0, 25.0 , 0.0 };
	GLfloat light3_direction[4] = { -1.0, 0.0, 0.0 , 0.0 };
	GLfloat light3_constant[1] = { 1.0 };
	GLfloat light3_linear[1] = { 0.0 };
	GLfloat light3_quadratic[1] = { 0.0 };
	GLfloat light3_spot_cutoff[1] = { 45.0 };
	GLfloat light3_spot_exp[1] = { 0.0 };

	// SHADER RELATED ===================================================
	bool changedshaders = false;

	void noshader();
	void setShaders();

	void printProgramInfoLog(GLuint obj);
	void printShaderInfoLog(GLuint obj);
	int printOglError(char *file, int line);
	int shadertype = 1; // 0 = Off, 1 = Per Pixel, 2 = Normal Mapping, 3 = Phong
	// 4 = Custom1  5 = Custom2
	GLint loc;
	GLuint v, f, v2,f2, p, p0, p1, p2, p3, p4, p5;

	//shader setup
	void perpixel();
	void normalmapping();
	GLuint normal_texture_color;
	GLuint normal_texture;
	GLuint LoadTexture(const char * filename, int width, int height);
	void graytoon();
	void inverted();
	//===================================================================


public:

	QViewport(QWidget* parent);
	virtual ~QViewport();

	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);

public:
	int LoadBitmap(char* filename);
	char Load3DS(obj_type* p_object, const char* p_filename);

	void MouseMove(int x, int y);
	void KeyPress(string key);

	void initializelight();
	void checklight();
	void drawlight();
	void updatelight();
};


#endif