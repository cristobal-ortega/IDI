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

Model m;
int left=0;
int vectorgiro[3];
int anguloy = 0;
int angulox = 0;
int lastx;
int lasty;

void movimiento(){
	if(vectorgiro[1]){
		glRotatef(anguloy,1,0,0);
	}
	if(vectorgiro[0]){
		glRotatef(angulox,0,1,0);
	}

}

float* centro_caja_modelo(void){
	float min_x,min_y,min_z,max_x,max_y,max_z;
	const vector<Vertex>& vertices = m.vertices();
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
			max_y = vertices[i];
		else if(vertices[i+1] < min_y)
			min_y = vertices[i];
	}
	float ret[3];
	ret[0] = (max_x+min_x)/2;
	ret[1] = (max_y+min_y)/2;
	ret[2] = (max_z+min_z)/2;
	//printf("x: %f, y: %f\n",ret[0],ret[1]);
	return ret;
}

void pintar_homer(void){
	int i = 0;
	const vector<Face>& faces = m.faces();
	const vector<Vertex>& vertices = m.vertices();
	int size_faces = faces.size();
	for(i=0;i<size_faces;++i){
		const Face &f = faces[i];
		glColor4fv(Materials[f.mat].diffuse);
		glBegin(GL_TRIANGLES);
		    glVertex3dv(&vertices[f.v[0]]);
		    glVertex3dv(&vertices[f.v[1]]);
		    glVertex3dv(&vertices[f.v[2]]);
		glEnd();
	}
	
}


void refresh (void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1,1,1,1);
	//glLoadIdentity();
		movimiento();
		pintar_homer();	
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
	if(state == GLUT_DOWN){
		left= ~left;
		if(!left){
			lastx = x;
			lasty = y;
		}
	}

}

void drag(int x, int y)
{

	if(!left){
		if(lastx>x){
			vectorgiro[1]=1;		
		}else if( lastx<x){
			vectorgiro[1]=1;
			
		}
		if(lasty > y){
			vectorgiro[0]=1;
		}else if(lasty < y){
			vectorgiro[0]=1;

		}
		angulox = lastx-x;
		anguloy = lasty-y;
		lastx = x;
		lasty=y;
		glutPostRedisplay();
	} 

}

void init(){

	m.load("HomerProves.obj");
	vectorgiro[0] = 0;
	vectorgiro[1] = 0;
	vectorgiro[2] = 0;

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
	glLoadIdentity();
	glOrtho(-1,1,-1,1,-1,1);
	glMatrixMode(GL_MODELVIEW);
	////
	glutReshapeFunc(reshape);
	glutDisplayFunc(refresh);
	glutMotionFunc(drag);
	glutMouseFunc(mouse);
 
    glutMainLoop();
 
    return EXIT_SUCCESS;
}
