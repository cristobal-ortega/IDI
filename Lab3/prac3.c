#if defined(__APPLE__)
    #include <OpenGl/OpenGL.h>
    #include <GLUT/GLUT.h>
#else
    #include <GL/gl.h>
    #include <GL/freeglut.h>
#endif
 
#include <stdio.h>
#include "model.h"
#include <math.h>

using namespace std;

#define PATH "models/"
#define LOAD_MODEL_PATH(PATH,x) PATH #x
#define LOAD_MODEL(x) LOAD_MODEL_PATH(PATH, x)

#define ROTATE_PARAMETER 0.05
#define MOVEMENT_PARAMETER 0.005
#define ZOOM_PARAMETER 0.05


#define FLOOR_SIDE 10

//MODELOS DISPONIBLES
#define LEGO 0
#define HOMER 1
#define COCHE 2

#define LEGO_PATH	legoman.obj
#define HOMER_PATH 	HomerProves.obj
#define COCHE_PATH	porsche.obj

#define DEFAULT COCHE
#define DEFAULT_PATH COCHE_PATH



//ESTRUCTURA DE MODELOS, SU USO NO SIRVE MUCHO AL SÓLO TENER UN MODELO
struct info_model{
	float box[3];
	float height;
	float width;
	float depth;
	float max_x;
	float min_x;
	float max_y;
	float min_y;
	float max_z;
	float min_z;
	float fs;
};




//INFO VENTANA && DRAG
int width = 600;
int height = 600;
int last_x = 0;
int last_y = 0;


//MODOS
int walls = 1;
int walk;
int tafa;
int left=0;
int zoom_mode = 0;
int rotate = 0;
int scalate = 0;
int movement = 0;
int cam_perspective = 1;

//MODO ROTATE
int rotate_x;
int rotate_y;
int rotate_z;
float anglex =0.0, angley= 0.0, anglez= 0.0;


//MODO MOVEMENT
float movement_x, movement_y, movement_z;



//MODO WALK && TAFANER
float angulo_m, angulo_t;
float pos_x_obs;
float pos_y_obs;
float pos_z_obs;
float pos_x_vpr;
float pos_y_vpr;
float pos_z_vpr;

//MODO SCALAR
float scale_x = 1.0f, scale_y = 1.0f, scale_z = 1.0f;

//MODELOS
Model m[3];
struct info_model info[3];

//CAMARA PERSPECTIVE && AXONOMETRICA
int w_window = 600;
int h_window = 600;
float raw;
float dist=0;
float width_w;
float height_w;
float rav;
float inc_x;
float inc_y;
float zoom;
float fov;
float zoom_axo;

//INFO ESFERA ESCENA
float radio;
float x_c,y_c,z_c;

//INFO COCHE
float pos_x;
float pos_z;
float acceleration;



//FUNCIONES (no estan todas)
void init(void);
void translaciones_model(int num);
void rotar(void);
void scalar(void);
struct info_model centro_caja_modelo(int num);
void pintar_modelo(int num);
void draw_axis(void);
void draw_floor(float side);
void pintar_ninot(void);
void dibujo_inicial(void);



struct info_model centro_caja_modelo(int num){
	struct info_model res;
	float min_x,min_y,min_z,max_x,max_y,max_z;
	const vector<Vertex>& vertices = m[num].vertices();
	int size_vertices = vertices.size();
	min_x = max_x = vertices[0];
	min_y = max_y = vertices[1];
	min_z = max_z = vertices[2];
	
	for(int i=3;i<size_vertices;i+=3){
		if(vertices[i]>max_x)
			max_x = vertices[i];
		else if(vertices[i] < min_x)
			min_x = vertices[i];
		
		if(vertices[i+1]>max_y)
			max_y = vertices[i+1];
		else if(vertices[i+1] < min_y)
			min_y = vertices[i+1];
		
		if(vertices[i+2]>max_z)
			max_z = vertices[i+2];
		else if(vertices[i+2] < min_z)
			min_z = vertices[i+2];
	}
	res.box[0] = (max_x+min_x)/2;
	res.box[1] = (max_y+min_y)/2;
	res.box[2] = (max_z+min_z)/2;
	res.width = abs(max_x-min_y);
	res.height = abs(max_y-min_y);
	res.depth = abs(max_z-min_z);
	res.max_x = max_x;
	res.min_x = min_x;
	res.max_y = max_y;
	res.min_y = min_y;
	res.max_z = max_z;
	res.min_z = min_z;
	
	float biggest_dim;

	if(res.height >= res.width){
		if(res.height >= res.depth)	
			biggest_dim = res.height;
		else
			biggest_dim = res.depth;
	}
	else{
		if(res.width >= res.depth)	
			biggest_dim = res.width;
		else
			biggest_dim = res.depth;
	}
	res.fs = (1.5/biggest_dim);

	//AJUSTER PARA CREAR CAJA MÍNIMA DEL COCHE
	res.width *= res.fs;
	res.height *= res.fs;
	res.depth *= res.fs;

	res.max_x = res.width/2;
	res.min_x = -res.max_x;
	res.max_y = (res.height/2);
	res.min_y = -res.max_y;
	res.max_z = res.depth/2;
	res.min_z = -res.max_z;
	

	return res;
}

void esfera_escena(void){
	float x_min,x_max,y_min,y_max,z_min,z_max;
	x_c,y_c,z_c;
	x_min  = -(FLOOR_SIDE/2.0);
	x_max = -x_min;
	y_min = -0.4;
	y_max = 0.8;
	z_min = x_min;
	z_max = x_max;
	
	x_c = (x_max+x_min)/2;
	y_c = (y_max+y_min)/2;
	z_c = (z_max+z_min)/2;

	float x_r,y_r,z_r;
	x_r = x_c + (x_max);
	y_r = y_c + (y_max);
	z_r = z_c + (z_max);

	float radio_suelo = sqrt( pow( (x_max-x_min)/2,2 ) + pow( (z_max-z_min)/2,2 ) );
	radio = sqrt( pow( (y_max-y_min)/2,2 )  + pow(radio_suelo,2) );

}

void draw_min_box(float x_min,float x_max,float y_min,float y_max,float z_min,float z_max){
	glPushMatrix();
		glColor3f (1,1,1);
		glPolygonMode(GL_FRONT_AND_BACK,(GLenum) GL_LINE);
			glBegin (GL_QUADS);
				glVertex3f(x_min, y_min,z_min);
				glVertex3f(x_min, y_min,z_max);
				glVertex3f(x_max, y_min,z_max);
				glVertex3f(x_max, y_min,z_min);			
			glEnd();
			glBegin (GL_QUADS);
				glVertex3f(x_max, y_min,z_min);
				glVertex3f(x_max, y_min,z_max);
				glVertex3f(x_max, y_max,z_max);
				glVertex3f(x_max, y_max,z_min);			
			glEnd();
			glBegin (GL_QUADS);
				glVertex3f(x_min, y_min,z_min);
				glVertex3f(x_min, y_min,z_max);
				glVertex3f(x_min, y_max,z_max);
				glVertex3f(x_min, y_max,z_min);			
			glEnd();
			glBegin (GL_QUADS);
				glVertex3f(x_min, y_max,z_min);
				glVertex3f(x_min, y_max,z_max);
				glVertex3f(x_max, y_max,z_max);
				glVertex3f(x_max, y_max,z_min);			
			glEnd();
		glPolygonMode(GL_FRONT_AND_BACK,(GLenum) GL_FILL);
	glPopMatrix();
}

void pintar_modelo(int num){
	const vector<Face>& faces = m[num].faces();
	const vector<Vertex>& vertices = m[num].vertices();

	glPushMatrix();

		translaciones_model(num);



		int size_faces = faces.size();
		for(int i=0;i<size_faces;++i){
			const Face &f = faces[i];
			glColor4fv(Materials[f.mat].diffuse);
			glBegin(GL_TRIANGLES);
				glVertex3dv(&vertices[f.v[0]]);
				glVertex3dv(&vertices[f.v[1]]);
				glVertex3dv(&vertices[f.v[2]]);
			glEnd();
		}

		draw_min_box(info[num].min_x,info[num].max_x,info[num].min_y,info[num].max_y,info[num].min_z,info[num].max_z);

	glPopMatrix();
}


void translaciones_model(int num){
	glTranslatef(0,(info[num].box[1]-info[num].min_y),0);
	glTranslatef(pos_x,0,pos_z);
	glRotatef(angulo_m,0,1,0);
	glScalef(info[num].fs,info[num].fs,info[num].fs);
	glTranslatef(-info[num].box[0],-info[num].box[0],-info[num].box[0]);
}

void rotar(void){
	glRotatef(rotate_y,1,0,0);
	glRotatef(-rotate_x,0,1,0);
}

void scalar(void){
	glScalef (scale_x, scale_y, scale_z);
}

void draw_axis(){
	glPushMatrix();
		glColor3f (0,0,0);
		glBegin (GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(2,0,0); 
			glVertex3f(0,0,0);
			glVertex3f(0,2,0);
			glVertex3f(0,0,0);
			glVertex3f(0,0,2); 
		glEnd();
	glPopMatrix ();

}

void draw_box_model(){
	glPushMatrix();
		int num = 2;
		float y2 = (info[num].box[1]-info[num].min_y);

		glTranslatef(0,y2,0);
	
		draw_min_box(info[num].min_x,info[num].max_x,info[num].min_y,info[num].max_y,info[num].min_z,info[num].max_z);
	glPopMatrix ();
}

void draw_floor(float side){
	glPushMatrix();
		glColor3f (1,0,0);
		side = side/2.0;
		glBegin (GL_QUADS);
			glVertex3f(-side, 0.0,-side);
			glVertex3f(-side, 0.0,side);
			glVertex3f(side, 0.0,side); 
			glVertex3f(side, 0.0,-side);
		glEnd();
	glPopMatrix ();
}


void pintar_ninot(float x, float y, float z){
	glPushMatrix();
		glColor3f (1,1,1);
		glTranslatef(x,y+0.4,z);
		glutWireSphere(0.4,50,50);
		glTranslatef(0,0.6,0);
		glutWireSphere(0.2,50,50);
		glTranslatef(0.1,0,0);
		glRotatef(90,0,1,0);
		glColor3f (1,0.6,0);
		glutWireCone(0.1,0.2,10,10);
	glPopMatrix ();
}

void pintar_pared(float sx, float sy, float sz, float cx, float cy, float cz){
	glPushMatrix();
		glColor3f (0,1,0);
		glTranslatef(cx,cy+0.5*sy,cz);
		glScalef(sx, sy, sz);
		glutSolidCube(1);
	glPopMatrix ();
}

void pinta_esfera_escena(void){
	glPushMatrix();
		glColor3f (1,1,1);
		glTranslatef(0,0.4,0);
		glutWireSphere(radio,50,50);
	glPopMatrix ();
}


void pos_camara(){
	if(walk){

		pos_x_obs = pos_x+info[DEFAULT].depth/2*sin(angulo_m*3.14/(180));
		pos_z_obs = pos_z+info[DEFAULT].depth/2*cos(angulo_m*3.14/(180));

		if(!tafa){
			pos_x_vpr = pos_x_obs + 2*sin(angulo_m*3.14/(180));
			pos_z_vpr = pos_z_obs + 2*cos(angulo_m*3.14/(180));
		}
		else{
			pos_x_vpr = pos_x_obs + 2*sin((angulo_m+angulo_t)*3.14/(180));
			pos_z_vpr = pos_z_obs + 2*cos((angulo_m+angulo_t)*3.14/(180));

		}
	
		glLoadIdentity();
		gluLookAt(pos_x_obs,pos_y_obs,pos_z_obs, pos_x_vpr, pos_y_vpr, pos_z_vpr, 0,1,0);


	}
	else{
		glLoadIdentity();
		glTranslatef(0,0,-(dist) );
		glRotatef(-angley,1,0,0);
		glRotatef(anglex,0,1,0);
		glTranslatef(-x_c,-y_c,-z_c);
	}
}

void refresh (void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(1,1,1,0);
	pos_camara();

	draw_axis();

	draw_floor(FLOOR_SIDE);

	if(scalate )	
		scalar();

	pintar_modelo(DEFAULT);
	pintar_ninot(2.5,0,2.5);
	pintar_ninot(2.5,0,-2.5);
	pintar_ninot(-2.5,0,-2.5);
	pintar_ninot(-2.5,0,2.5);

	if(walls){
		pintar_pared(4,1.5,0.2,2.5,0,1.5);
		pintar_pared(0.2,1.5,10,-4.9,0,0);
	}

	//pinta_esfera_escena();

	glutSwapBuffers();
}

void camera_perspective(void){
	if(fov+zoom <= 0 || fov+zoom >= 180)
		printf("No se puede hacer aplicar más zoom\n");	
	else
		fov +=zoom;
	zoom = 0;
	if(raw<1)
		fov *=1/raw;

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov,raw,0.1,2*dist);
	glMatrixMode(GL_MODELVIEW);
}

void camera_axo(){
	if(raw>rav){
		inc_x = (height_w*raw)/2;
	}else{
		inc_y = (width_w/raw)/2;
	}

	if(zoom_axo+(zoom*ZOOM_PARAMETER) <= 0.10)	
		printf("No se puede hacer aplicar más zoom\n");
	else
		zoom_axo += zoom*ZOOM_PARAMETER;	

	zoom = 0;

	float x_min = (fabs(x_c)-inc_x)*zoom_axo;
	float x_max = (fabs(x_c)+inc_x)*zoom_axo;
	float y_min = (fabs(y_c)-inc_y)*zoom_axo;
	float y_max = (fabs(y_c)+inc_y)*zoom_axo;

	

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(x_min, x_max,y_min,y_max,-dist,dist*2);
	glMatrixMode(GL_MODELVIEW);
}

void tipo_camara(void){
	if(cam_perspective)
		camera_perspective();
	else
		camera_axo();

}

void reshape(int w, int h){
	
	w_window = w;
	h_window = h;
	tipo_camara();


	glViewport(0,0,w,h);
}

void mouse(int button, int state, int x, int y){
	//if(button = GLUT_LEFT_BUTTON){	
		if(state == GLUT_DOWN){
			left= ~left;
			if(!left){
				last_x = x;
				last_y = y;
			}
		}
	//}

}

void drag(int x, int y)
{
	if(!left){
		if ( zoom_mode == 1){
		//if(glutGetModifiers() == GLUT_ACTIVE_SHIFT ){ //Not working
			zoom-=(last_y-y)*ZOOM_PARAMETER;
			tipo_camara();
		}
		else{
			if(rotate){
				rotate_x = last_x-x;
				rotate_y = last_y-y;
				anglex+=rotate_x;
				angley+=rotate_y;
			}
			if(movement){		
				movement_x += last_x-x;
				movement_z += last_y-y;
			}
		}
		last_x = x;
		last_y = y;
		pos_camara();
		glutPostRedisplay();
	} 

}

void dibujo_inicial(void){
	anglex =0.0;
	angley= 0.0;
	anglez= 0.0;
	fov = 2* (asin(radio/dist) * 180/3.14);
	zoom = 0;
	zoom_axo = 1;
	raw = (float)w_window/(float)h_window;
	width_w = 2*radio + fabs(x_c);
	height_w = 2*radio + fabs(y_c);
	rav = width_w/height_w;
	inc_x  = width_w/2;
	inc_y = height_w/2;

}
 

void keyboard(unsigned char key,int x, int y){
    if(key=='h'){
		printf("Pulsar R para activar/desactivar rotación\n");
        printf("--Click y arrastrar con el raton para rotar el objeto\n");
		printf("Pulsar Q izquierdo para aumentar/disminuir zoom\n");
		printf("Pulsar E para activar/desactivar escalado\n");
        printf("--Flechas teclado númerico, horizontales eje X, verticales eje Y\n");
		printf("Pulsar C para activar/desactivar movimiento de coche\n");
        printf("--W para avanzar\n");
		printf("--S para dar marcha atrás\n");
		printf("--D para girar hacía tu derecha\n");
		printf("--A para girar hacía tu izquierda\n");
		printf("--X para aumentar la velocidad\n");
		printf("--Z para disminuir la velocidad\n");
		printf("Pulsar T para activar/desactivar modo tafaner, mirar desde el coche\n");
		printf("Pulsar P para activar/desactivar camara perspectiva\n");
		printf("Pulsar V para activar/desactivar dibujado de paredes\n");
		printf("Pulsar I para volver al dibujo inicial\n");
        printf("ESC para cerrar la ventana\n");
    }
	else if( key == 'r' ){
		rotate = (rotate==0)?1:0;
		walk = 0;
		zoom_mode = 0;
		printf("rotate: %i\n",rotate);
		printf("movement: %i\n",walk);
		printf("zoom_mode: %i\n",zoom_mode);
	}
	else if( key == 'q' ){
		zoom_mode = (zoom_mode==0)?1:0;
		rotate = 0;
		printf("rotate: %i\n",rotate);
		printf("movement: %i\n",walk);
		printf("zoom_mode: %i\n",zoom_mode);
	}
	//MOVIMIENTO COCHE
	else if( key == 'w'){
		if(walk){
			pos_z += acceleration * cos(angulo_m*3.14/(180));
			pos_x += acceleration * sin(angulo_m*3.14/(180));

			pos_camara();
		}
	}
	else if( key == 's'){
		if(walk){
			pos_z -= acceleration * cos(angulo_m*3.14/(180));
			pos_x -= acceleration * sin(angulo_m*3.14/(180));

			pos_camara();
		}
	}
	else if( key == 'a'){
		if(walk){
			if(tafa)
				angulo_t +=10;			
			else
				angulo_m+=10;
		
		}
		
	}
	else if( key == 'd'){
		if(walk){
			if(tafa)
				angulo_t -=10;			
			else
				angulo_m-=10;
		
		}
	}
	else if( key == 't'){
		if(!tafa)
			angulo_t = 0;
		tafa = (tafa==0)?1:0;
		printf("tafaner: %i\n",tafa);
	}
	else if( key == 'x'){
		acceleration +=0.05;
	}
	else if( key == 'z'){
		acceleration -=0.05;
	}
	//END
	else if( key == 'e'){
		scalate = (scalate==0)?1:0;
		printf("scalate: %i\n",scalate);
	}
	else if( key == 'i'){
		printf("Reiniciando dibujo\n");
		init();
		dibujo_inicial();
		tipo_camara();
		pos_camara();
	}
	else if( key == 'c'){
		walk = (walk==0)?1:0;
		rotate = 0;		
		printf("movement: %i\n",walk);
		printf("rotate: %i\n",rotate);
	}
	else if( key == 'v'){
		walls = (walls==0)?1:0;
		printf("walls: %i\n",walls);
	}
	else if( key == 'p'){
		cam_perspective = (cam_perspective==0)?1:0;
		printf("cam_perspective: %i\n",cam_perspective);
		tipo_camara();
	}
	//Flechas de pad númerico
	else if( key == 56){
		scale_y +=ROTATE_PARAMETER;

	}
	else if( key == 50){
		scale_y -=ROTATE_PARAMETER;
	}
	else if( key == 54){
		scale_x +=ROTATE_PARAMETER;
	}
	else if( key == 52){
		scale_x -=ROTATE_PARAMETER;
	}
    else if( key == 27){
        exit(0);   
    }
	glutPostRedisplay();


}

void init(){
	movement_x = 0;
	movement_y = 0;
	movement_z = 0;
	scale_x = 1.0f, scale_y = 1.0f, scale_z = 1.0f;
	m[DEFAULT].load(LOAD_MODEL(DEFAULT_PATH));
	info[DEFAULT] = centro_caja_modelo(DEFAULT);

	esfera_escena();

	dist = radio +1;
	//INIT CAMERA
	fov = 2* (asin(radio/dist) * 180/3.14);
	zoom = 0;
	raw = (float)w_window/(float)h_window;
	width_w = 2*radio + fabs(x_c);
	height_w = 2*radio + fabs(y_c);
	rav = width_w/height_w;
	inc_x  = width_w/2;
	inc_y = height_w/2;
	anglex =0.0;
	angley= 0.0;
	anglez= 0.0;
	
	zoom_axo = 1;
	//MODELO
	angulo_m=0;
	angulo_t = 0;
	pos_x = 0;
	pos_z = 0;
	acceleration = 0.05;


	pos_y_obs = info[DEFAULT].box[1]-info[DEFAULT].min_y;


	pos_y_vpr = info[DEFAULT].box[1]-info[DEFAULT].min_y;

}


int main(int argc, char *argv[])
{
	init();
	glutInit( &argc, (char **)argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA| GLUT_DEPTH);
	glutInitWindowSize(600,600);
	glutCreateWindow("IDI: Practiques OpenGL");

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK,(GLenum) GL_FILL);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1,-1,1,-1,1);
	glMatrixMode(GL_MODELVIEW);

	glutReshapeFunc(reshape);
	glutDisplayFunc(refresh);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(drag);
	glutMouseFunc(mouse);

 
    glutMainLoop();
 
    return EXIT_SUCCESS;
}
