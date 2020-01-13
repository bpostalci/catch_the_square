#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  500
#define WINDOW_HEIGHT 500

#define  TIMER_PERIOD  timerPeriod // Period for the timer.
#define  TIMER_ON     1     // 0:disable timer, 1:enable timer

#define PI 3.1415
#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

//Constants for states
#define LOAD 0
#define MENU 1
#define RUN 2
#define GAMEOVER 3

//Global variables
int gameState = GAMEOVER;
int dotCount = 0;
int elapsedTime = 0;
int curMenu = 0;
double timerPeriod = 16.66;
int counter = 0; //Number of random cells produced so far
int score = 0; //Number of catches
float fps = 1000/timerPeriod;
char str[100];

int rx, ry; //random cell numbers
int cx = -100, cy = -100; //current mouse position in terms of number (not pixel)

//For r,g,b colors
int r = 255, g = 255, b = 255;
int windowWidth = WINDOW_WIDTH;
int windowHeight = WINDOW_HEIGHT;

//
// to draw circle, center at (x,y)
//  radius r
//
void circle( int x, int y, int r ){
	float angle ;
    	glBegin( GL_POLYGON ) ;
	for ( int i = 0 ; i < 100 ; i++ )
	{
		angle = 2*PI*i/100;
		glVertex2f( x+r*cos(angle), y+r*sin(angle)) ;
	}
	glEnd();
}
void circle_wire(int x, int y, int r){
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}
void print(int x, int y, char *string, void *font ){
	int len, i ;

	glRasterPos2f( x, y );
	len = (int) strlen( string );
	for ( i =0; i<len; i++ )
	{
		glutBitmapCharacter( font, string[i]);
	}
}
// display text with variables.
void vprint(int x, int y, void *font, char *string , ...){
	va_list ap;
	va_start ( ap, string );
	char str[1024] ;
	vsprintf( str, string, ap ) ;
	va_end(ap) ;
	
	int len, i ;
	glRasterPos2f( x, y );
	len = (int) strlen( str );
	for ( i =0; i<len; i++ )
	{
		glutBitmapCharacter( font, str[i]);
	}
}
void vprint2(int x, int y, float size, char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf(str, string, ap);
	va_end(ap);
	glPushMatrix();
		glTranslatef(x, y, 0);
		glScalef(size, size, 1);
		
		int len, i;
		len = (int)strlen(str);
		for (i = 0; i<len; i++)
		{
			glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
		}
	glPopMatrix();
}

void display_load(){
	
	strcpy(str, "LOADING");
	vprint2(-50, -50, 0.2, str);
	
	strcpy(str, ".");
	for(int i = 0; i < dotCount; i++)
		vprint2(80 + i * 20, - 50,0.2,str); //Increase dot's
		
	for(int i = 0;i  < 1000000; i++) {}
}
void display_menu(){
	glColor3f(1,0,1);
	
	strcpy(str, "MENU");
	vprint(-30,100,GLUT_BITMAP_TIMES_ROMAN_24, str);
	
	glColor3f(1,0.5,0);
	strcpy(str, "START GAME");
	vprint(-50, 50,GLUT_BITMAP_HELVETICA_18, str);
	strcpy(str, "OPTIONS");
	vprint(-50, 20,GLUT_BITMAP_HELVETICA_18,str);
	strcpy(str, "EXIT");
	vprint(-50,-10,GLUT_BITMAP_HELVETICA_18,str);
	
	glColor3f(1,0,0);
	circle(-80, 55 - 30*curMenu,5);
	
	switch(curMenu){
		case 0:
			glColor3f(0,1,0);
			strcpy(str, "Space to start game");
			vprint(-80,-150, GLUT_BITMAP_9_BY_15, str);
		break;
		case 1:
			glColor3f(0,1,0);
			strcpy(str, "Space to display options");
			vprint(-80,-150, GLUT_BITMAP_9_BY_15, str);
		break;
		case 2:
			glColor3f(0,1,0);
			strcpy(str, "Space to exit");
			vprint(-80,-150, GLUT_BITMAP_9_BY_15, str);
		
	}
	
	
}
void drawGridLines(){
	glColor3ub(125,125,90);
	glLineWidth(3);
	for(int x = -200; x <= 200; x+=50){
		glBegin(GL_LINES);
		
		//Vertical lines
		glVertex2f(x, -200);
		glVertex2f(x,200);
		
		//Horizontal lines
		glVertex2f(-200, x);
		glVertex2f(200, x);
		glEnd();
	}
}
void drawRandomCell(){
	
	int lx = rx * 50;
	int ly = ry * 50;
	
	glColor3ub(r,g,b);
	glRectf(lx + 3, ly + 3, lx + 46, ly + 46);
	
}
void drawCurrentCell(){
	if(cx <= 3 && cx >= -4 && cy <= 3 && cy >= -4){
		int lx = cx * 50;
		int ly = cy * 50;
		
		glColor3f(1,0,0);
		glBegin(GL_LINE_LOOP);
		
		glVertex2f(lx, ly);
		glVertex2f(lx + 50, ly);
		
		glVertex2f(lx + 50, ly + 50);
		glVertex2f(lx, ly + 50);
		
		glEnd();
		
	}
}
void display_run(){
	drawGridLines();
	drawRandomCell();
	drawCurrentCell();
	
	glColor3f(1,1,1);
	strcpy(str, "Score: %d/%d");
	vprint(-240,-240, GLUT_BITMAP_TIMES_ROMAN_24, str, score, counter);
	
	glColor3f(1,0,0);
	strcpy(str, "ESC to exit");
	vprint(-50,-240, GLUT_BITMAP_9_BY_15, str, score, counter);
	
	glColor3ub(10,90,70);
	strcpy(str, "fps: %.f");
	vprint(180,220, GLUT_BITMAP_9_BY_15, str,fps);
}
void display_gameover(){
	
	glColor3f(0,1,0);
	strcpy(str, "Score: %d/%d");
	vprint(-100, -20,GLUT_BITMAP_9_BY_15 ,str, score, counter);
	
	glColor3f(1,0,0);
	strcpy(str, "GAME OVER");
	vprint(-100, 10,GLUT_BITMAP_TIMES_ROMAN_24 ,str);
	
	glColor3f(0,1,1);
	strcpy(str, "Press F5 to return the menu");
	vprint(-100, -50,GLUT_BITMAP_9_BY_15 ,str);
	
}

//
// To display onto window using OpenGL commands
//
void display()
{
	
	//
	// clear window to black
	//
	glClearColor( 0, 0 , 0 , 0 );
	glClear( GL_COLOR_BUFFER_BIT );
	
	
	switch (gameState) {
		case LOAD: display_load(); break;
		case MENU: display_menu(); break;
		case RUN: display_run(); break;
		case GAMEOVER: display_gameover(); break;
	}
 
	glutSwapBuffers();


}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y )
{
   // exit when ESC is pressed.
   if ( key == 27 )
	   exit(0);
   
   if(key == ' ' && gameState == MENU && curMenu == 0){
		gameState = RUN;
		timerPeriod = 1000;
	}
	else if(curMenu == 1)
		gameState = MENU;
	else if(curMenu == 2)
		exit(0);
   
   
    // to refresh the window it calls display() function
   glutPostRedisplay() ;
}

void onKeyUp(unsigned char key, int x, int y )
{
   // exit when ESC is pressed.
   if ( key == 27 )
	   exit(0);
    
    // to refresh the window it calls display() function
   glutPostRedisplay() ;
}
//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown( int key, int x, int y )
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;

	}
	
	if(gameState == MENU){
	   if(key == GLUT_KEY_DOWN){
			curMenu = (curMenu + 1) % 3;
	   }
	   if(key == GLUT_KEY_UP){
		   if(curMenu == 0)
				curMenu = 3;
			curMenu = (curMenu - 1) % 3;
	   }
	}
	
	//Restart the game by resetting all game variables
	if(gameState == GAMEOVER && key == GLUT_KEY_F5){
		gameState = MENU;
		counter = 0;
		score = 0;
		cx = -100;
		cy = -100;
		timerPeriod = 300;
		r = 255, g = 255, b = 255;
	}
	
	 // to refresh the window it calls display() function
	glutPostRedisplay() ;
}


//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp( int key, int x, int y )
{
  	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;

	}
	 // to refresh the window it calls display() function
	glutPostRedisplay() ;
}


//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick( int button, int stat, int x, int y )
{
	// Write your codes here.
	if(gameState == RUN && button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN){
		if(cx == rx && cy == ry){
			score++;
			timerPeriod -= 25;
			r = rand() % 256;
			g = rand() % 256;
			b = rand() % 256;
		}
	}
	 // to refresh the window it calls display() function
	glutPostRedisplay() ; 
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize( int w, int h )
{
	winWidth = w;
	winHeight = h;
	glViewport( 0,0,w,h) ;
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( -w/2, w/2, -h/2, h/2, -1, 1);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown( int x, int y ) {
	// Write your codes here.


	
	 // to refresh the window it calls display() function	
	glutPostRedisplay() ;
}

void onMove( int x, int y ) {
	// Write your codes here.
	if(gameState == RUN) {
		//Convert glut to OpenGL
		x = x - winWidth / 2;
		y = winWidth / 2 - y;
		
		//Convert from OpenGL pixel coord. to cell coord.
		cx = floor(x / 50.0);
		cy = floor(y / 50.0);
		//printf("Current cell x = %d\ny = %d\n", cx, cy);	
	}
	
	 // to refresh the window it calls display() function
    glutPostRedisplay() ;
}

#if TIMER_ON == 1
void onTimer( int v ) {
	 
	glutTimerFunc( TIMER_PERIOD, onTimer, 0 ) ;
	// Write your codes here.
	
	if(gameState == LOAD){
		dotCount = (dotCount + 1) % 4;
		elapsedTime += 300;
		if(elapsedTime > 3000){
			gameState = MENU;
		}
	}
	else if (gameState == RUN) {
		rx = rand() % 8 - 4; //[-4 , 3]
		ry = rand() % 8 -4;
		counter++;
		if(counter > 30){
			gameState = GAMEOVER;
		}
		
		//printf("%.2f\n", fps);
	}
	
    
	
	 // to refresh the window it calls display() function
	glutPostRedisplay() ; // display()

}
#endif

void Init() {
	srand(time(0));
	
	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
}


int main( int argc, char *argv[] )
{

	glutInit(&argc, argv );
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow( "Finite State Machines" ) ;

	glutDisplayFunc( display ) ;
    glutReshapeFunc( onResize );
	//
	// keyboard registration
	//
	glutKeyboardFunc( onKeyDown ) ;
	glutSpecialFunc( onSpecialKeyDown ) ;

	glutKeyboardUpFunc( onKeyUp ) ;
	glutSpecialUpFunc( onSpecialKeyUp ) ;

	//
	// mouse registration
	//
    glutMouseFunc( onClick) ;
	glutMotionFunc( onMoveDown ) ;
	glutPassiveMotionFunc( onMove ) ; 
	
	
	
    #if  TIMER_ON == 1
	// timer event
	glutTimerFunc( TIMER_PERIOD, onTimer, 0 ) ;
	#endif

	Init();
	
	glutMainLoop();
	
	return 0;
}
