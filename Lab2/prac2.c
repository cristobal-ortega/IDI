#if defined(__APPLE__)
    #include <OpenGl/OpenGL.h>
    #include <GLUT/GLUT.h>
#else
    #include <GL/gl.h>
    #include <GL/freeglut.h>
#endif

#include <stdio.h>
#include "model.h"

using namespace std;

#define PATH "models/"
#define LOAD_MODEL_PATH(PATH,x) PATH #x
#define LOAD_MODEL(x) LOAD_MODEL_PATH(PATH, x)

#define ROTATE_PARAMETER 0.05
#define MOVEMENT_PARAMETER 0.005
//MODELOS DISPONIBLES
#define LEGO 0
#define HOMER 1

#define LEGO_PATH legoman.obj
#define HOMER_PATH HomerProves.obj

#define DEFAULT LEGO
#define DEFAULT_PATH LEGO_PATH


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
};


int last_x = 0;
int last_y = 0;
int left=0;
int vectorgiro[3];
int rotate_x;
int rotate_y;
int rotate_z;
int rotate = 0;
int scalate = 0;
int movement = 0;
float movement_x, movement_y, movement_z;
int scaleVector[3];
float scale_x = 1.0f, scale_y = 1.0f, scale_z = 1.0f;
Model m[2];
struct info_model info[2];


//Funcionicas
void init(void);
void mover(void);
void rotar(void);
void scalar(void);
struct info_model centro_caja_modelo(int num);
void pintar_modelo(int num);
void draw_axis(void);
void draw_floor(void);
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
	
	return res;
}

void pintar_modelo(int num){
	const vector<Face>& faces = m[num].faces();
	const vector<Vertex>& vertices = m[num].vertices();

	glPushMatrix();
		//glLoadIdentity();
		//Calculos de escalado:
		float fs = (0.5/info[num].height);
		////
		mover();
		glTranslatef(0.75,-0.4,0.75);
		//Sin calcular direccion
		float x = (abs(info[num].max_x-info[num].box[0]))*fs;
		float y = (abs(info[num].min_y-info[num].box[1]))*fs;
		float z = (abs(info[num].max_z-info[num].box[2]))*fs;
		//Con direccion
		float x2 = (info[num].box[0]-info[num].max_x)*fs;
		float y2 = (info[num].box[1]-info[num].min_y)*fs;
		float z2 = (info[num].box[2]-info[num].max_z)*fs;
		//printf("x: %f, x2: %f, y: %f, y2: %f z:%f z2:%f\n",x,x2,y,y2,z,z2);
		glTranslatef(x2,y2,z2);
		glScalef(fs,fs,fs);
		
		glTranslatef(-info[num].box[0],-info[num].box[1],-info[num].box[2]);
		//glTranslatef(-info[num].box[0],-info[num].box[1],-info[num].box[2]);
		
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
	
	glPopMatrix();

}


void mover(void){
	glTranslatef(-(movement_x*MOVEMENT_PARAMETER),movement_y*MOVEMENT_PARAMETER,0);
}

void rotar(void){
	glRotatef(rotate_y,1,0,0);
	glRotatef(-rotate_x,0,1,0);
}

void scalar(void){
	glScalef (scale_x, scale_y, scale_z);
	scale_x = 1.0f, scale_y = 1.0f, scale_z = 1.0f;
}

void draw_axis(){
	glPushMatrix();
		//glLoadIdentity();
		glColor3f (1,0,0);
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

void draw_floor(){
	glPushMatrix();
		//glLoadIdentity();
		glColor3f (0,0,0);
		float side = 1.5/2.0;
		glBegin (GL_QUADS);
			glVertex3f(-side, -0.4,-side);
			glVertex3f(-side, -0.4,side);
			glVertex3f(side, -0.4,side); 
			glVertex3f(side, -0.4,-side);
		glEnd();
	glPopMatrix ();
}


void pintar_ninot(){
	glPushMatrix();
		glColor3f (0,0,1);
		glTranslatef(0,0,0);
		glutWireSphere(0.4,50,50);
		glTranslatef(0,0.6,0);
		glutWireSphere(0.2,50,50);
		glTranslatef(0.1,0,0);
		glRotatef(90,0,1,0);
		glColor3f (1,0.6,0);
		glutWireCone(0.1,0.2,10,10);
	glPopMatrix ();
}



void refresh (void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1,1,1,1);
	draw_axis();
	draw_floor();
	
	if(!movement){
		if(rotate )
			rotar();
		if(scalate )
			scalar();
	}
	pintar_modelo(DEFAULT);
	pintar_ninot();
	//glutWireTeapot(0.5);
	glutSwapBuffers();

}


void reshape(int w, int h){
    if(w>h){
        int amplada = (w-h)/2;
        glViewport(amplada,0,h,h);
    }else{
        int alzada = (h-w)/2;       
        glViewport(0,alzada,w,w);
    }

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
		if(rotate){
			rotate_x = last_x-x;
			rotate_y = last_y-y;
		}
		if(movement){		
			movement_x += last_x-x;
			movement_y += last_y-y;
		}
		last_x = x;
		last_y = y;
		
		glutPostRedisplay();
	} 

}

void dibujo_inicial(void){
	glPopMatrix();
	glPushMatrix();
	glLoadIdentity();

}
 

void keyboard(unsigned char key,int x, int y){
	//printf("Tecla pulsada: %i\n",key);
    if(key=='h'){
		printf("Pulsar R para activar/desactivar rotación\n");
        printf("--Click y arrastrar con el raton para rotar el objeto\n");
		printf("Pulsar S para activar/desactivar escalado\n");
        printf("--Flechas teclado númerico, horizontales eje X, verticales eje Y\n");
		printf("Pulsar C para activar/desactivar movimiento de legoman\n");
        printf("--Click y arrastrar con el raton para mover el objeto\n");
		printf("Pulsar I para volver al dibujo inicial\n");
        printf("ESC para cerrar la ventana\n");
    }
	else if( key == 'r' ){
		rotate = (rotate==0)?1:0;
		movement = 0;
		printf("rotate: %i\n",rotate);
		printf("movement: %i\n",movement);
	}
	else if( key == 's'){
		scalate = (scalate==0)?1:0;
		printf("scalate: %i\n",scalate);
	}
	else if( key == 'i'){
		printf("Reiniciando dibujo\n");
		init();
		dibujo_inicial();
	}
	else if( key == 'c'){
		movement = (movement==0)?1:0;
		rotate = 0;		
		printf("movement: %i\n",movement);
		printf("rotate: %i\n",rotate);
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
	vectorgiro[0] = 0;
	vectorgiro[1] = 0;
	vectorgiro[2] = 0;
	scaleVector[0] = 0;
	scaleVector[1] = 0;
	scaleVector[2] = 0;
	movement_x = 0;
	movement_y = 0;
	movement_z = 0;
	scale_x = 1.0f, scale_y = 1.0f, scale_z = 1.0f;
	m[DEFAULT].load(LOAD_MODEL(DEFAULT_PATH));
	info[DEFAULT] = centro_caja_modelo(DEFAULT);



}


int main(int argc, char *argv[])
{
	init();
	glutInit( &argc, (char **)argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA| GLUT_DEPTH);
	glutInitWindowSize(600,600);
	glutCreateWindow("IDI: Practiques OpenGL");
	//Añadido para rotaciones
	glMatrixMode (GL_PROJECTION);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK,(GLenum) GL_FILL);
	glLoadIdentity();
	glOrtho(-1,1,-1,1,-1,1);
	glMatrixMode(GL_MODELVIEW);
	////
	glutReshapeFunc(reshape);
	glutDisplayFunc(refresh);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(drag);
	glutMouseFunc(mouse);
 
    glutMainLoop();
 
    return EXIT_SUCCESS;
}
