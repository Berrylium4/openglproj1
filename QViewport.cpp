#include "QViewport.h"
#include "OpenGL.h"

double toradian(double deg)
{
	return (double)(deg * pi / 180);
}

void obj_type::reset() {
	strcpy(name, "");

	vertices_qty = 0;
	polygons_qty = 0;

	vertex_type vertex_temp;
	polygon_type polygon_temp;
	mapcoord_type mapcoord_temp;

	for (int i = 0; i < MAX_VERTICES; i++)
	{
		vertex[i] = vertex_temp;
		mapcoord[i] = mapcoord_temp;
	}

	for (int i = 0; i < MAX_POLYGONS; i++)
	{
		polygon[i] = polygon_temp;

	}
}

QViewport::QViewport(QWidget* parent) : QOpenGLWidget(parent) {
}

QViewport::~QViewport()
{
}

void QViewport::initializeGL()
{
	glewInit();
	if (glewIsSupported("GL_VERSION_2_0"))
		printf("Ready for OpenGL 2.0\n");
	else {
		printf("OpenGL 2.0 not supported\n");
		exit(1);
	}
	setShaders();
	//noshader(); // default option

	glClearColor(0.0, 0.0, 0.0, 0.0); // This clear the background color to black
	glShadeModel(GL_SMOOTH); // Type of shading for the polygons

	// Viewport transformation
	glViewport(0, 0, screen_width, screen_height);

	// Projection transformation
	glMatrixMode(GL_PROJECTION); // Specifies which matrix stack is the target for matrix operations 
	glLoadIdentity(); // We initialize the projection matrix as identity
	gluPerspective(45.0f, (GLfloat)screen_width / (GLfloat)screen_height, 10.0f, 10000.0f); // We define the "viewing volume"
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
	glEnable(GL_DEPTH_TEST); // We enable the depth test (also called z buffer)
	glDepthFunc(GL_LEQUAL);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)

	glEnable(GL_TEXTURE_2D); // This Enable the Texture mapping

	Load3DS(&object, model_filename.c_str());

	//object.id_texture = LoadBitmap("spaceshiptexture.bmp"); // The Function LoadBitmap() return the current texture ID

	// If the last function returns -1 it means the file was not found so we exit from the program
	if (object.id_texture == -1)
	{
		cout << "ERROR OPENING TEXTURE" << endl;
		//exit(0);
	}

	initializelight();
	///glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	//glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);

}

//===============================================================================================
void QViewport::paintGL() // when we render things
{
	// Shader =============================================

	if (changedshaders)
	{

		switch (shadertype)
		{
		case 0:
			noshader();
			break;
		case 1:
			noshader();
			perpixel();
			break;
		case 2:
			noshader();
			normalmapping();
			break;
		case 3:
			noshader();
			graytoon();
			break;
		}
		changedshaders = false;
	}
	// =====================================================

	//glBindTexture(GL_TEXTURE_2D, 0);
	//glDisable(GL_TEXTURE_2D); // Turn off texture

	if (refresh_model)
	{
		object.reset();
		Load3DS(&object, model_filename.c_str());
	}

	int l_index;


	checklight(); // enable/disable lights
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // This clear the background color to dark blue
	glMatrixMode(GL_MODELVIEW); // Modeling transformation


	//TRANSLATION AND ROTATION =======================================================================================
	//glLoadIdentity(); // Initialize the model matrix as identity
	glPushMatrix();

	glTranslatef(translate_x, translate_y, translate_z);

	glRotatef((GLfloat)rotation_angle_theta, 0.0, 1.0, 0.0);
	glRotatef(-(GLfloat)rotation_angle_phi, -cos(toradian(rotation_angle_theta)), 0.0, -sin(toradian(rotation_angle_theta)));
	glRotatef((GLfloat)rotation_yaw, 0.0, cos(toradian(rotation_angle_phi)), sin(toradian(rotation_angle_phi)));
	updatelight(); // Needs to be here so light also gets rotated with everything else

	//================================================================================================================

	//MATERIAL =======================================================================================================
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	//================================================================================================================
	//glBindTexture(GL_TEXTURE_2D, object.id_texture); // We set the active texture 
	if (shadertype == 2)
	{
		float s = 100.0f;
		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(s, 0.0f, 0.0f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, s, 0.0f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, s);
		glEnd();
		glEnable(GL_LIGHTING);

		//=========================================
		drawlight();
		//=============================================
	}

	if (shadertype == 2)
	{
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	int texture_location = glGetUniformLocation(p2, "color_texture");
	glUniform1i(texture_location, 0);
	glBindTexture(GL_TEXTURE_2D, normal_texture_color);

	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	int normal_location = glGetUniformLocation(p2, "normal_texture");
	glUniform1i(normal_location, 1);
	glBindTexture(GL_TEXTURE_2D, normal_texture);
	}

	glBegin(GL_TRIANGLES); // glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)

	for (l_index = 0; l_index < object.polygons_qty; l_index++)
	{
		//----------------- FIRST VERTEX -----------------
		// Texture coordinates of the first vertex
		//glNormal3fv(normal);
		glNormal3f(object.vertex[object.polygon[l_index].a].x,
			object.vertex[object.polygon[l_index].a].y,
			object.vertex[object.polygon[l_index].a].z); //Vertex definition
		glTexCoord2f(object.mapcoord[object.polygon[l_index].a].u,
			object.mapcoord[object.polygon[l_index].a].v);
		// Coordinates of the first vertex
		glVertex3f(object.vertex[object.polygon[l_index].a].x,
			object.vertex[object.polygon[l_index].a].y,
			object.vertex[object.polygon[l_index].a].z); //Vertex definition



//----------------- SECOND VERTEX -----------------
// Texture coordinates of the second vertex
		//glNormal3f(N.x, N.y, N.z);
		//glNormal3fv(normal);

		glNormal3f(object.vertex[object.polygon[l_index].b].x,
			object.vertex[object.polygon[l_index].b].y,
			object.vertex[object.polygon[l_index].b].z);
		glTexCoord2f(object.mapcoord[object.polygon[l_index].b].u,
			object.mapcoord[object.polygon[l_index].b].v);
		// Coordinates of the second vertex
		glVertex3f(object.vertex[object.polygon[l_index].b].x,
			object.vertex[object.polygon[l_index].b].y,
			object.vertex[object.polygon[l_index].b].z);


		//----------------- THIRD VERTEX -----------------
		// Texture coordinates of the third vertex
		//glNormal3f(N.x, N.y, N.z);
		//glNormal3fv(normal);

		glNormal3f(object.vertex[object.polygon[l_index].c].x,
			object.vertex[object.polygon[l_index].c].y,
			object.vertex[object.polygon[l_index].c].z);
		glTexCoord2f(object.mapcoord[object.polygon[l_index].c].u,
			object.mapcoord[object.polygon[l_index].c].v);
		// Coordinates of the Third vertex
		glVertex3f(object.vertex[object.polygon[l_index].c].x,
			object.vertex[object.polygon[l_index].c].y,
			object.vertex[object.polygon[l_index].c].z);
	}
	glEnd();

	if (shadertype == 2) {
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	}
	// Drawing axis and lights ====================================
	//glDisable(GL_TEXTURE_2D); // Turn off texture
	glDisable(GL_TEXTURE_2D);

	
	float s = 100.0f;
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(s, 0.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, s, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, s);
	glEnd();
	glEnable(GL_LIGHTING);

	//=========================================
	drawlight();
	//=============================================
	
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);

}

void QViewport::resizeGL(int width, int height)
{

	screen_width = width; // We obtain the new screen width values and store it
	screen_height = height; // Height value

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We clear both the color and the depth buffer so to draw the next frame
	glViewport(0, 0, screen_width, screen_height); // Viewport transformation

	glMatrixMode(GL_PROJECTION); // Projection transformation
	glLoadIdentity(); // We initialize the projection matrix as identity
	gluPerspective(45.0f, (GLfloat)screen_width / (GLfloat)screen_height, 1.0f, 500.0f);
	//gluPerspective(45.0f, (GLfloat)screen_width / (GLfloat)screen_height, 0.1f, 100.0f);

}

int QViewport::LoadBitmap(char* filename) {
	int i, j = 0; //Index variables
	FILE* l_file; //File pointer
	unsigned char* l_texture; //The pointer to the memory zone in which we will load the texture

	// windows.h gives us these types to work with the Bitmap files
	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;
	RGBTRIPLE rgb;

	num_texture++; // The counter of the current texture is increased

	if ((l_file = fopen(filename, "rb")) == NULL) return (-1); // Open the file for reading

	fread(&fileheader, sizeof(fileheader), 1, l_file); // Read the fileheader

	fseek(l_file, sizeof(fileheader), SEEK_SET); // Jump the fileheader
	fread(&infoheader, sizeof(infoheader), 1, l_file); // and read the infoheader

	// Now we need to allocate the memory for our image (width * height * color deep)
	l_texture = (byte*)malloc(infoheader.biWidth * infoheader.biHeight * 4);
	// And fill it with zeros
	memset(l_texture, 0, infoheader.biWidth * infoheader.biHeight * 4);

	// At this point we can read every pixel of the image
	for (i = 0; i < infoheader.biWidth * infoheader.biHeight; i++)
	{
		// We load an RGB value from the file
		fread(&rgb, sizeof(rgb), 1, l_file);

		// And store it
		l_texture[j + 0] = rgb.rgbtRed; // Red component
		l_texture[j + 1] = rgb.rgbtGreen; // Green component
		l_texture[j + 2] = rgb.rgbtBlue; // Blue component
		l_texture[j + 3] = 255; // Alpha value
		j += 4; // Go to the next position
	}

	fclose(l_file); // Closes the file stream

	glBindTexture(GL_TEXTURE_2D, num_texture); // Bind the ID texture specified by the 2nd parameter

	// The next commands sets the texture parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // If the u,v coordinates overflow the range 0,1 the image is repeated
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // The magnification function ("linear" produces better results)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); //The minifying function

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // We don't combine the color with the original surface color, use only the texture map.

	// Finally we define the 2d texture
	glTexImage2D(GL_TEXTURE_2D, 0, 4, infoheader.biWidth, infoheader.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);

	// And create 2d mipmaps for the minifying function
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, infoheader.biWidth, infoheader.biHeight, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);

	free(l_texture); // Free the memory we used to load the texture

	return (num_texture); // Returns the current texture OpenGL ID
}

char QViewport::Load3DS(obj_type* p_object, const char* p_filename) {
	int i; //Index variable

	FILE* l_file; //File pointer

	unsigned short l_chunk_id; //Chunk identifier
	unsigned int l_chunk_lenght; //Chunk lenght

	unsigned char l_char; //Char variable
	unsigned short l_qty; //Number of elements in each chunk

	unsigned short l_face_flags; //Flag that stores some face information

	if ((l_file = fopen(p_filename, "rb")) == NULL) return 0; //Open the file

	while (ftell(l_file) < filelength(fileno(l_file))) //Loop to scan the whole file 
	{
		//getche(); //Insert this command for debug (to wait for keypress for each chuck reading)

		fread(&l_chunk_id, 2, 1, l_file); //Read the chunk header
		//printf("ChunkID: %x\n", l_chunk_id);
		fread(&l_chunk_lenght, 4, 1, l_file); //Read the lenght of the chunk
		//printf("ChunkLenght: %x\n", l_chunk_lenght);

		switch (l_chunk_id)
		{
			//----------------- MAIN3DS -----------------
			// Description: Main chunk, contains all the other chunks
			// Chunk ID: 4d4d 
			// Chunk Lenght: 0 + sub chunks
			//-------------------------------------------
		case 0x4d4d:
			break;

			//----------------- EDIT3DS -----------------
			// Description: 3D Editor chunk, objects layout info 
			// Chunk ID: 3d3d (hex)
			// Chunk Lenght: 0 + sub chunks
			//-------------------------------------------
		case 0x3d3d:
			break;

			//--------------- EDIT_OBJECT ---------------
			// Description: Object block, info for each object
			// Chunk ID: 4000 (hex)
			// Chunk Lenght: len(object name) + sub chunks
			//-------------------------------------------
		case 0x4000:
			i = 0;
			do
			{
				fread(&l_char, 1, 1, l_file);
				p_object->name[i] = l_char;
				i++;
			} while (l_char != '\0' && i < 20);
			break;

			//--------------- OBJ_TRIMESH ---------------
			// Description: Triangular mesh, contains chunks for 3d mesh info
			// Chunk ID: 4100 (hex)
			// Chunk Lenght: 0 + sub chunks
			//-------------------------------------------
		case 0x4100:
			break;

			//--------------- TRI_VERTEXL ---------------
			// Description: Vertices list
			// Chunk ID: 4110 (hex)
			// Chunk Lenght: 1 x unsigned short (number of vertices) 
			//             + 3 x float (vertex coordinates) x (number of vertices)
			//             + sub chunks
			//-------------------------------------------
		case 0x4110:
			fread(&l_qty, sizeof(unsigned short), 1, l_file);
			p_object->vertices_qty = l_qty;
			printf("Number of vertices: %d\n", l_qty);
			for (i = 0; i < l_qty; i++)
			{
				fread(&p_object->vertex[i].x, sizeof(float), 1, l_file);
				//printf("Vertices list x: %f\n", p_object->vertex[i].x);
				fread(&p_object->vertex[i].y, sizeof(float), 1, l_file);
				//printf("Vertices list y: %f\n", p_object->vertex[i].y);
				fread(&p_object->vertex[i].z, sizeof(float), 1, l_file);
				//printf("Vertices list z: %f\n", p_object->vertex[i].z);
			}
			break;

			//--------------- TRI_FACEL1 ----------------
			// Description: Polygons (faces) list
			// Chunk ID: 4120 (hex)
			// Chunk Lenght: 1 x unsigned short (number of polygons) 
			//             + 3 x unsigned short (polygon points) x (number of polygons)
			//             + sub chunks
			//-------------------------------------------
		case 0x4120:
			fread(&l_qty, sizeof(unsigned short), 1, l_file);
			p_object->polygons_qty = l_qty;
			printf("Number of polygons: %d\n", l_qty);
			for (i = 0; i < l_qty; i++)
			{
				fread(&p_object->polygon[i].a, sizeof(unsigned short), 1, l_file);
				//printf("Polygon point a: %d\n", p_object->polygon[i].a);
				fread(&p_object->polygon[i].b, sizeof(unsigned short), 1, l_file);
				//printf("Polygon point b: %d\n", p_object->polygon[i].b);
				fread(&p_object->polygon[i].c, sizeof(unsigned short), 1, l_file);
				//printf("Polygon point c: %d\n", p_object->polygon[i].c);
				fread(&l_face_flags, sizeof(unsigned short), 1, l_file);
				//printf("Face flags: %x\n", l_face_flags);
			}
			break;

			//------------- TRI_MAPPINGCOORS ------------
			// Description: Vertices list
			// Chunk ID: 4140 (hex)
			// Chunk Lenght: 1 x unsigned short (number of mapping points) 
			//             + 2 x float (mapping coordinates) x (number of mapping points)
			//             + sub chunks
			//-------------------------------------------
		case 0x4140:
			fread(&l_qty, sizeof(unsigned short), 1, l_file);
			for (i = 0; i < l_qty; i++)
			{
				fread(&p_object->mapcoord[i].u, sizeof(float), 1, l_file);
				//printf("Mapping list u: %f\n", p_object->mapcoord[i].u);
				fread(&p_object->mapcoord[i].v, sizeof(float), 1, l_file);
				//printf("Mapping list v: %f\n", p_object->mapcoord[i].v);
			}
			break;

			//----------- Skip unknow chunks ------------
			//We need to skip all the chunks that currently we don't use
			//We use the chunk lenght information to set the file pointer
			//to the same level next chunk
			//-------------------------------------------
		default:
			fseek(l_file, l_chunk_lenght - 6, SEEK_CUR);
		}
	}
	fclose(l_file); // Closes the file stream

	refresh_model = false;

	return (1); // Returns ok
}

void QViewport::MouseMove(int x, int y) {

	rotation_angle_theta = (rotation_angle_theta + (x - mouse_cur[0])) % 360;
	rotation_angle_phi = (rotation_angle_phi + (y - mouse_cur[1])) % 360;
	mouse_cur[0] = x;
	mouse_cur[1] = y;
	//cout << "PHI: " << rotation_angle_phi << "THETA: " << rotation_angle_theta << endl;

}

void QViewport::KeyPress(string key)
{
	if (key == "w" || key == "W")
	{
		translate_y += 1;
	}
	else if (key == "s" || key == "S")
	{
		translate_y -= 1;
	}
	else if (key == "a" || key == "A")
	{
		translate_x -= 1;
	}
	else if (key == "d" || key == "D")
	{
		translate_x += 1;
	}
	else if (key == "r" || key == "R")
	{
		translate_z -= 1;
	}
	else if (key == "f" || key == "F")
	{
		translate_z += 1;
	}
	else if (key == "t" || key == "T")
	{
		rotation_yaw = (rotation_yaw + 5) % 360;
	}
	else if (key == "g" || key == "g")
	{
		rotation_yaw = (rotation_yaw - 5) % 360;
	}
}

void QViewport::initializelight() {
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);

	glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
	glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);

	glLightfv(GL_LIGHT3, GL_POSITION, light3_position);
	glLightfv(GL_LIGHT3, GL_AMBIENT, light3_ambient);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light3_diffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light3_specular);
}

void QViewport::checklight() {
	if (light1on)
	{
		glEnable(GL_LIGHT1);
	}
	else
	{
		glDisable(GL_LIGHT1);
	}

	if (light2on)
	{
		glEnable(GL_LIGHT2);
	}
	else
	{
		glDisable(GL_LIGHT2);
	}

	if (light3on)
	{
		glEnable(GL_LIGHT3);
	}
	else
	{
		glDisable(GL_LIGHT3);
	}
}


void QViewport::drawlight() {
	glDisable(GL_TEXTURE_CUBE_MAP_EXT);
	glDisable(GL_LIGHTING);
	glPointSize(10.0f);
	glBegin(GL_POINTS);
	if (light1on)
	{
		glColor3f(light1_diffuse[0], light1_diffuse[1], light1_diffuse[2]);
		glVertex3f(light1_position[0], light1_position[1], light1_position[2]);
	}

	if (light2on)
	{
		glColor3f(light2_diffuse[0], light2_diffuse[1], light2_diffuse[2]);
		glVertex3f(light2_position[0], light2_position[1], light2_position[2]);
	}

	if (light3on)
	{
		glColor3f(light3_diffuse[0], light3_diffuse[1], light3_diffuse[2]);
		glVertex3f(light3_position[0], light3_position[1], light3_position[2]);
	}

	glEnd();
	glEnable(GL_LIGHTING);
}

void QViewport::updatelight() {
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_direction);
	glLightfv(GL_LIGHT1, GL_CONSTANT_ATTENUATION, light1_constant);
	glLightfv(GL_LIGHT1, GL_LINEAR_ATTENUATION, light1_linear);
	glLightfv(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, light1_quadratic);

	if (light1_spotlight)
	{
		glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, light1_spot_cutoff);
	}
	else
	{
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 180.0);
	}
	glLightfv(GL_LIGHT1, GL_SPOT_EXPONENT, light1_spot_exp);


	glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
	glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2_direction);
	glLightfv(GL_LIGHT2, GL_CONSTANT_ATTENUATION, light2_constant);
	glLightfv(GL_LIGHT2, GL_LINEAR_ATTENUATION, light2_linear);
	glLightfv(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, light2_quadratic);
	glLightfv(GL_LIGHT2, GL_SPOT_CUTOFF, light2_spot_cutoff);
	glLightfv(GL_LIGHT2, GL_SPOT_EXPONENT, light2_spot_exp);
	if (light2_spotlight)
	{
		glLightfv(GL_LIGHT2, GL_SPOT_CUTOFF, light2_spot_cutoff);
	}
	else
	{
		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 180.0);
	}

	glLightfv(GL_LIGHT3, GL_POSITION, light3_position);
	glLightfv(GL_LIGHT3, GL_AMBIENT, light3_ambient);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light3_diffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light3_specular);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light3_direction);
	glLightfv(GL_LIGHT3, GL_CONSTANT_ATTENUATION, light3_constant);
	glLightfv(GL_LIGHT3, GL_LINEAR_ATTENUATION, light3_linear);
	glLightfv(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, light3_quadratic);
	glLightfv(GL_LIGHT3, GL_SPOT_CUTOFF, light3_spot_cutoff);
	glLightfv(GL_LIGHT3, GL_SPOT_EXPONENT, light3_spot_exp);
	if (light3_spotlight)
	{
		glLightfv(GL_LIGHT3, GL_SPOT_CUTOFF, light3_spot_cutoff);
	}
	else
	{
		glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 180.0);
	}

	//cout << "LIGHTS UPDATED\n";
}

// SHADERS =============================================================================
void QViewport::setShaders()
{
	char *vs = NULL, *fs = NULL, *fs2 = NULL;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	//f2 = glCreateShader(GL_FRAGMENT_SHADER);

	vs = textFileRead("./Shaders/perpixel.vert");
	fs = textFileRead("./Shaders/perpixel.frag");

	const char * vv = vs;
	const char * ff = fs;

	glShaderSource(v, 1, &vv, NULL);
	glShaderSource(f, 1, &ff, NULL);

	free(vs); free(fs);

	glCompileShader(v);
	glCompileShader(f);

	cout << " PER PIXEL VERTEX INFO \n";
	printShaderInfoLog(v);
	cout << " PER PIXEL FRAGMENT INFO \n";
	printShaderInfoLog(f);
	//printShaderInfoLog(f2);

	p1 = glCreateProgram();
	glAttachShader(p1, v);
	glAttachShader(p1, f);
	//=================================================================
	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	vs = textFileRead("./Shaders/Normal.vert");
	fs = textFileRead("./Shaders/Normal.frag");

	vv = vs;
	ff = fs;

	glShaderSource(v, 1, &vv, NULL);
	glShaderSource(f, 1, &ff, NULL);

	free(vs); free(fs);

	glCompileShader(v);
	glCompileShader(f);
	cout << " NORMAL VERTEX INFO \n";

	printShaderInfoLog(v);
	cout << " NORMAL FRAGMENT INFO \n";

	printShaderInfoLog(f);

	p2 = glCreateProgram();
	glAttachShader(p2, v);
	glAttachShader(p2, f);

	normal_texture_color = LoadTexture("color_map.raw", 256, 256);
	normal_texture = LoadTexture("normal_map.raw", 256, 256);
	//=================================================================
	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	vs = textFileRead("./Shaders/graytoon.vert");
	fs = textFileRead("./Shaders/graytoon.frag");

	vv = vs;
	ff = fs;

	glShaderSource(v, 1, &vv, NULL);
	glShaderSource(f, 1, &ff, NULL);

	free(vs); free(fs);

	glCompileShader(v);
	glCompileShader(f);
	cout << " NORMAL VERTEX INFO \n";

	printShaderInfoLog(v);
	cout << " NORMAL FRAGMENT INFO \n";

	printShaderInfoLog(f);

	p3 = glCreateProgram();
	glAttachShader(p3, v);
	glAttachShader(p3, f);
	//=================================================================
	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	vs = textFileRead("./Shaders/Invert.vert");
	fs = textFileRead("./Shaders/Invert.frag");

	vv = vs;
	ff = fs;

	glShaderSource(v, 1, &vv, NULL);
	glShaderSource(f, 1, &ff, NULL);

	free(vs); free(fs);

	glCompileShader(v);
	glCompileShader(f);
	cout << " NORMAL VERTEX INFO \n";

	printShaderInfoLog(v);
	cout << " NORMAL FRAGMENT INFO \n";

	printShaderInfoLog(f);

	p4 = glCreateProgram();
	glAttachShader(p4, v);
	glAttachShader(p4, f);
	//=================================================================
	//=================================================================

	/*
	glLinkProgram(p1);
	printProgramInfoLog(p1);
	glUseProgram(p1);
	loc = glGetUniformLocation(p1, "time");
	*/


}

void QViewport::printProgramInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}
}
void QViewport::printShaderInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}
}
int QViewport::printOglError(char *file, int line) {
	//
// Returns 1 if an OpenGL error occurred, 0 otherwise.
//
	GLenum glErr;
	int    retCode = 0;

	glErr = glGetError();
	while (glErr != GL_NO_ERROR)
	{
		printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
		retCode = 1;
		glErr = glGetError();
	}
	return retCode;
}

void QViewport::noshader() {
	//glLinkProgram(p0);
	//printProgramInfoLog(p0);
	glUseProgram(0);
	loc = glGetUniformLocation(p0, "time");
}

void QViewport::perpixel() {

	glLinkProgram(p1);
	printProgramInfoLog(p1);

	glUseProgram(p1);
	loc = glGetUniformLocation(p1, "time");

	// Per Pixel ================================================
	GLint light1on_Loc = glGetUniformLocation(p1, "light1_on");
	GLint light2on_Loc = glGetUniformLocation(p1, "light2_on");
	GLint light3on_Loc = glGetUniformLocation(p1, "light3_on");

	float light1consta_loc = glGetUniformLocation(p1, "light1_consta");
	float light1linear_loc = glGetUniformLocation(p1, "light1_linear");
	float light1quadra_loc = glGetUniformLocation(p1, "light1_quad");
	glUniform1f(light1consta_loc, light1_constant[0]);
	glUniform1f(light1linear_loc, light1_linear[0]);
	glUniform1f(light1quadra_loc, light1_quadratic[0]);

	float light2consta_loc = glGetUniformLocation(p1, "light2_consta");
	float light2linear_loc = glGetUniformLocation(p1, "light2_linear");
	float light2quadra_loc = glGetUniformLocation(p1, "light2_quad");
	glUniform1f(light2consta_loc, light2_constant[0]);
	glUniform1f(light2linear_loc, light2_linear[0]);
	glUniform1f(light2quadra_loc, light2_quadratic[0]);

	float light3consta_loc = glGetUniformLocation(p1, "light1_consta");
	float light3linear_loc = glGetUniformLocation(p1, "light1_linear");
	float light3quadra_loc = glGetUniformLocation(p1, "light1_quad");
	glUniform1f(light3consta_loc, light3_constant[0]);
	glUniform1f(light3linear_loc, light3_linear[0]);
	glUniform1f(light3quadra_loc, light3_quadratic[0]);

	if (light1on)
	{
		glUniform1f(light1on_Loc, true);
	}
	else
	{
		glUniform1f(light1on_Loc, false);
	}

	if (light2on)
	{
		glUniform1f(light2on_Loc, true);
	}
	else
	{
		glUniform1f(light2on_Loc, false);
	}

	if (light3on)
	{
		glUniform1f(light3on_Loc, true);
	}
	else
	{
		glUniform1f(light3on_Loc, false);
	}
}

void QViewport::normalmapping() {
	glLinkProgram(p2);
	printProgramInfoLog(p2);

	glUseProgram(p2);
	loc = glGetUniformLocation(p2, "time");

	// PASS LIGHT INFO TO FRAGMENT SHADER
	GLint light1on_Loc = glGetUniformLocation(p2, "light1_on");
	GLint light2on_Loc = glGetUniformLocation(p2, "light2_on");
	GLint light3on_Loc = glGetUniformLocation(p2, "light3_on");

	if (light1on)
	{
		glUniform1f(light1on_Loc, true);
	}
	else
	{
		glUniform1f(light1on_Loc, false);
	}

	if (light2on)
	{
		glUniform1f(light2on_Loc, true);
	}
	else
	{
		glUniform1f(light2on_Loc, false);
	}

	if (light3on)
	{
		glUniform1f(light3on_Loc, true);
	}
	else
	{
		glUniform1f(light3on_Loc, false);
	}
}

void QViewport::graytoon() {
	glLinkProgram(p3);
	printProgramInfoLog(p3);

	glUseProgram(p3);
	loc = glGetUniformLocation(p3, "time");

	// PASS LIGHT INFO TO FRAGMENT SHADER
	GLint light1on_Loc = glGetUniformLocation(p3, "light1_on");
	GLint light2on_Loc = glGetUniformLocation(p3, "light2_on");
	GLint light3on_Loc = glGetUniformLocation(p3, "light3_on");

	if (light1on)
	{
		glUniform1f(light1on_Loc, true);
	}
	else
	{
		glUniform1f(light1on_Loc, false);
	}

	if (light2on)
	{
		glUniform1f(light2on_Loc, true);
	}
	else
	{
		glUniform1f(light2on_Loc, false);
	}

	if (light3on)
	{
		glUniform1f(light3on_Loc, true);
	}
	else
	{
		glUniform1f(light3on_Loc, false);
	}
}


void QViewport::inverted() {
	glLinkProgram(p4);
	printProgramInfoLog(p4);

	glUseProgram(p4);
	loc = glGetUniformLocation(p4, "time");
}


GLuint QViewport::LoadTexture(const char * filename, int width, int height) {
	GLuint texture;
	unsigned char * data;
	FILE * file;

	//The following code will read in our RAW file  
	file = fopen(filename, "rb");

	if (file == NULL) return 0;
	data = (unsigned char *)malloc(width * height * 3);
	fread(data, width * height * 3, 1, file);

	fclose(file);

	glGenTextures(1, &texture); //generate the texture with the loaded data  
	glBindTexture(GL_TEXTURE_2D, texture); //bind the texture to it’s array  

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //set texture environment parameters  

	//And if you go and use extensions, you can use Anisotropic filtering textures which are of an  
	//even better quality, but this will do for now.  
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Here we are setting the parameter to repeat the texture instead of clamping the texture  
	//to the edge of our shape.  
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Generate the texture  
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	free(data); //free the texture  

	return texture; //return whether it was successfull  
}