#if defined(__APPLE__)
    #include <OpenGl/OpenGL.h>
    #include <GLUT/GLUT.h>
#else
    #include <GL/gl.h>
    #include <GL/freeglut.h>
#endif

#include <stdio.h>

int left=0;
int vectorgiro[3];
int anguloy = 0;
int angulox = 0;
int lastx;
int lasty;


void pintar_ninot(){
	glutWireSphere(0.4,50,50);
	glTranslatef(0,0.6,0);
	glutWireSphere(0.2,50,50);
	glTranslatef(0.1,0,0);
	glRotatef(90,0,1,0);
	glutWireCone(0.1,0.2,10,10);
}


void refresh (void)
{

	glClear(GL_COLOR_BUFFER_BIT);
	/*glBegin(GL_TRIANGLES);
	    glVertex3f(-0.5,(float)-(1/3),0.0);
	    glVertex3f(0.5,(float)-(1/3),0.0);
	    glVertex3f(0.0,(float)2/3,0.0);
	glEnd();*/
	//glRotatef(10,vectorgiro[0],vectorgiro[1],vectorgiro[2]);
	glLoadIdentity();
	glPushMatrix();
		pintar_ninot();
	glPopMatrix();
	glutSwapBuffers();

}

void initGL(){

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
				lastx = x;
				lasty = y;
			}
		}
	//}

}

void drag(int x, int y)
{

}

void init(){

}

 
int main(int argc, char *argv[])
{
	initGL();
	init();
	glutInit( &argc, (char **)argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowSize(600,600);
	glutCreateWindow("IDI: Practiques OpenGL");

	//Añadido para rotaciones
	glMatrixMode (GL_PROJECTION);
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
