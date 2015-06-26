#if defined(__APPLE__)
    #include <OpenGl/OpenGL.h>
    #include <GLUT/GLUT.h>
#else
    #include <GL/gl.h>
    #include <GL/freeglut.h>
#endif

#include <stdio.h>

int left=0;
int ancho=0;
int largo=0;

void refresh (void)
{

	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	    glVertex3f(-0.5,(float)-(1/3),0.0);
	    glVertex3f(0.5,(float)-(1/3),0.0);
	    glVertex3f(0.0,(float)2/3,0.0);
	glEnd();
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
	ancho = w;
	largo = h;

}

void keyboard(unsigned char key,int x, int y){
    if(key=='h'){
        printf("Click con el raton para cambiar el color\n");
        printf("ESC para cerrar la ventana\n");
    }
    else if( key == 27){
        exit(0);   
    }


}

void mouse(int button, int state, int x, int y){
	//if(button = GLUT_LEFT_BUTTON){	
		if(state == GLUT_DOWN){
			left= ~left;
		}
	//}

}

void drag(int x, int y)
{

	if(left!=0){
		glClearColor((float)x/ancho,(float)x/largo,(float)y/ancho,(float)y/largo);
		glutPostRedisplay();
	} 

}

int main(int argc,const char *argv[])
{
	initGL();
	glutInit( &argc, (char **)argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowSize(600,600);
	glutCreateWindow("IDI: Practiques OpenGL");
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutDisplayFunc(refresh);
	glutMotionFunc(drag);
	glutMouseFunc(mouse);
	glutMainLoop();

return 0;

}
