#include <GL/glut.h>  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int WINDOW_WIDTH ,WINDOW_HEIGHT;
static int window;

//flags
int controllineflag1 = 0;
int postpacketflag = 0;
int aflag = 0;
int getpacketflag = 0;
int ack = 0;
int timerstopflag = 0;
int packetcountenableflag = 0;

//color flags
int intrupt_color = 0;
int packet_colors = 0;
int back_color = 0;

//for menus and submenus
static int submenu_id;
static int bmenu_id;
static int menu_id;
static int nu_id;

int proc = 0;
float dir = 0.0;
int i;

//for translation and rotation and scaling
float packet = 0.0;
float timer = 0;
float angle = 0.0;
float scalepackets = 0.0;
float controllinepos1 = 0.001;
float controllinepos2 = 0.001;
int packetcountingangle = 30;
char noofpackets = '-';

//variables choices;
float packetspeed = 0.001;
float timerspeed = 0.0;
float angleofrotation = 3.0;
float controllinespeed = 0.001;

GLfloat colorslite[][3] = {{1.000, 0.980, 0.804},{0.902, 0.902, 0.980},{0.867, 0.627, 0.867},{0.565, 0.933, 0.565}};
GLfloat colorsdeep[][3] = {{0.804, 0.361, 0.361},{1.000, 0.271, 0.000},{1.000, 0.271, 0.000},{0.000, 0.392, 0.000},{0.000, 1.000, 1.000},{0.855, 0.647, 0.125}};

void instructionsscreen();
void instructionsscreen_caller();
void initGL();
void idle();
void display();
void reshape(GLsizei width, GLsizei height);
void keyboard(unsigned char key, int x, int y);
void drawText(char [],float ,float);
void drawCurrentText(char [],float ,float);
void drawSmallText(char [],float ,float);
void menuscreen_caller();
void menuscreen();
void projectscreen_caller();
void Setting();
void projectscreen();
void drawclient();
void drawserver();
void drawvisuals();
void drawpackets(int ,int );
void drawsignals();
void drawprogressbar();
void drawcontrollines(int ,float,float);
void refreshtimer();
void drawinstructions();
void drawcurrentdetails();
void drawcurrentstatus();
void Refresh();
void RefreshColor();
void menu(int num);
void createMenu();
void setBackground();


void setBackground()
{
  switch(back_color)
  {
    case 1:
      glClearColor(0.3,0.3,0.3,1.0);
      break;
    case 3:
      glClearColor(0.1,0.3,0.3,1.0);
      break;
    case 4:
      glClearColor(0.2,0.1,0.3,1.0);
      break;
    default:
      glClearColor(0.0,0.0,0.0,1.0);
      break;
  }
}

void RefreshColor()
{
glColor3f(0.0,1.0,0.0);
}

void projectscreen()
{
glClear(GL_COLOR_BUFFER_BIT);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();

//visuals
setBackground();
drawvisuals();
glColor3f(0.43,0.72,0.52);
drawCurrentText("Client",-0.65,0.8);
drawCurrentText("Server",0.7,0.8);
RefreshColor();
glColor3f(1.0,1.0,1.0);
glBegin(GL_LINES);
glVertex2d(-0.95,-0.95);
glVertex2d(0.5,-0.95);
glVertex2d(0.5,-0.47);
      glVertex2d(-0.95,-0.47);
  glEnd();
  RefreshColor();
    
  drawclient();
  drawserver();
  drawinstructions();
  drawcurrentstatus();
  glColor3f(0.0,1.0,0.0);

  drawcurrentdetails();
  //progress bar
  glPushMatrix();
  glBegin(GL_POLYGON);
      glVertex2f(-0.95,0.05);
      glVertex2f(-0.93 + timer,0.05);
      glVertex2f(-0.93 + timer,0.0);
      glVertex2f(-0.95,0.0);
  glEnd(); 
  glPopMatrix();
  //RefreshColor();

  if(proc == 1)
  {
      //post request
      //1.send the post request
      //control lines
      glPushMatrix();
      if(controllineflag1 == 1)
        glTranslated(100,0,0);
      glTranslatef(controllinepos1 - 0.08,0.0,0.0);
      drawcontrollines(1,-0.25,-0.3);
      glPopMatrix();
      //client
    
      if(controllineflag1 == 1)
      {
          //2.send packets
          //draw packets
          glPushMatrix();
          glTranslatef(-0.6 + packet,0.515f,0.0f);
          glScaled(1 + scalepackets,1 + scalepackets,1 + scalepackets);
          if(packetcountenableflag == 1)
            angleofrotation = packetcountingangle;
          
            glRotatef(angle, 0.0f, 0.0f, 1.0f);
         glTranslatef(0.4f,-0.5f,0.0f);         
          //glTranslatef(dir,0,0);
            drawpackets(1,1);
            glPopMatrix();
          
          if(postpacketflag == 1)
          {
            //3.get acknoledge;
            glPushMatrix();
            glTranslatef(controllinepos2,0.0,0.0);
            drawcontrollines(3,0.65,0.6);
            glPopMatrix();
          }
      }
    
  }
  else if(proc == 2)
  {
      //get request

      //1.request data from server

    
      glPushMatrix();
      if(aflag == 1)
        glTranslated(100,0,0);
      glTranslatef(-0.08 + controllinepos1,0.0,0.0);
      drawcontrollines(1,-0.4,-0.35);
      glPopMatrix();


     if(aflag == 1)
     {
         glPushMatrix();
          glTranslatef(0.9 -packet,0.55f,0.0f);
          glScaled(1 + scalepackets,1 + scalepackets,1 + scalepackets);
          if(packetcountenableflag == 1)
            angleofrotation = packetcountingangle;
          
          glRotatef(angle,0.0f,0.0f,1.0f);
          glTranslatef(-0.55,-0.55,0.0f);
          drawpackets(2,1);
          glPopMatrix();

          if(getpacketflag == 1)
          {
              //3.get acknoledge;
              glPushMatrix();
              glTranslatef(controllinepos2,0.0,0.0);
              drawcontrollines(3,-0.4,-0.35);
              glPopMatrix();              
          }
     }
  }
  //visuals end
  glutSwapBuffers();
  if(!timerstopflag)
    timer += timerspeed;
  angle += angleofrotation;
  //printf("postpacketflag = %d\n",postpacketflag);
}


void idle() {
  
  if(angleofrotation < 30.0)
    noofpackets = '1';
  //progress bar handling
  if(timer >= 1.88)
  {
    timerstopflag = 1;
    timerspeed = 0;
    timer = 0;
    Refresh();
  }
  if(proc == 1)
  {
    //for control lines no 1;
    //controllinepos1 += controllinespeed;
          if(controllinepos1 >= 1.0 && controllineflag1 != 1)
        {
          //control line 1 reached server
          controllineflag1 = 1;
          timer = 0;
          timerspeed = 0.0001;
        }
        if(controllineflag1 == 1)
        {
          if(packet >= 1.2 && postpacketflag != 1)
          {
              packetspeed = 100.0;
              postpacketflag = 1;
              timer = 0;
              timerspeed = 0.0001;
          }
          else
          {
              if(scalepackets + 1 < 1)
                scalepackets = 0.0;
              else if(packet - 0.4 >= 0.0)
                scalepackets -= 0.002;
              else
                scalepackets += 0.002;
          }
          if(postpacketflag == 1)
          {
            controllinepos2 -= controllinespeed;
          }
          if(controllinepos2 <= -1.0)
          {
            controllinepos2 = 100;
            Refresh();
          }
        } 
  }
  else if(proc == 2)
  {
    if(controllinepos1 >= 1.0 && controllineflag1 != 1)
    {
      //control line 1 reached server
      aflag = 1;
    }
    if(aflag == 1)
    {
      if(packet >= 1.25 && postpacketflag != 1)
      {
          packetspeed = 100.0;
          getpacketflag = 1;
      }
      else
      {
          if(scalepackets + 1 < 1)
            scalepackets = 0.0;
          else if(packet - 0.9 >= 0.0)
            scalepackets -= 0.002;
          else
            scalepackets += 0.002;
      }
      if(getpacketflag == 1)
      {
        controllinepos2 += controllinespeed;
      }
      if(controllinepos2 >= 1.0)
      {
        controllinepos2 = 100;
        Refresh();
      }
    }  
  }
  controllinepos1 += controllinespeed;
      packet += packetspeed;
        if(!timerstopflag)
          timer += timerspeed;

   glutPostRedisplay();  
}



void drawcontrollines(int color,float a,float b)
{
    switch(intrupt_color)
    {
      case 4:
        glColor3f(1.0,0.0,0.0);
        break;
      case 5:
        glColor3f(0.0,1.0,0.0);
        break;
      case 6:
        glColor3f(0.0,0.0,1.0);
        break;
      default:
        glColor3f(1.0,0.0,0.0);
        break;
    }
    glLineWidth(3);
    glBegin(GL_LINES);
      glVertex2d(a,0.51);
      glVertex2f(b,0.51);
    glEnd();
    glLineWidth(1);
}

void drawpackets(int flag,int no)
{

  switch(packet_colors)
  {
    case 7:
    glColor3d(1,0,0);
    break;
    case 8:
    glColor3d(0,1,1);
    break;
    case 9:
    glColor3d(0,0,1);
    break;
    default:
    glColor3d(1,0,0);
    break;
  }
    if(flag == 1)         //post packet get from server
    {
      // glColor3f(0.0,0.0,1.0);
      glBegin(GL_POLYGON);
        // glColor3fv(colorsdeep[4]);
        glVertex2f(-0.4,0.407);
        // glColor3fv(colorslite[3]);
        glVertex2f(-0.35,0.407);
        // glColor3fv(colorslite[3]);
        glVertex2f(-0.35,0.442);
        // glColor3fv(colorsdeep[4]);
        glVertex2f(-0.4,0.442);
      glEnd();
      RefreshColor();

      glEnd();
      RefreshColor();


    }
    else                //get packet from client
    {
       // glColor3f(0.0,0.0,1.0); 
       glBegin(GL_POLYGON);
        // glColor3fv(colorsdeep[4]);
        glVertex2f(0.6,0.55);
        // glColor3fv(colorslite[1]);
        glVertex2f(0.65,0.55);
        // glColor3fv(colorslite[1]);
        glVertex2f(0.65,0.585);
        // glColor3fv(colorsdeep[4]);
        glVertex2f(0.6,0.585);
      glEnd();
      RefreshColor();
    }
}

void drawprogressbar()
{
  glColor3f(0.0,1.0,0.0);
    glBegin(GL_POLYGON);
      glVertex2f(-0.95,0.05);
      glVertex2f(-0.93,0.05);
      glVertex2f(-0.93,0.0);
      glVertex2f(-0.95,0.0);
    glEnd();
}

void drawclient()
{
    //monitor
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0,1.0,0.0);
      glVertex2f(-0.85,0.45);
      glVertex2f(-0.6,0.45);
      glVertex2f(-0.6,0.7);
      glVertex2f(-0.85,0.7);
    glEnd();
    //monitor inner 
    glColor3f(0.14,0.64,0.75);
    glBegin(GL_LINE_LOOP);
      glVertex2f(-0.83,0.47);
      glVertex2f(-0.62,0.47);
      glVertex2f(-0.62,0.68);
      glVertex2f(-0.83,0.68);
    glEnd();
    RefreshColor();
    
    //keyboard
    //glColor3f(1.0,0.0,0.0);
    glBegin(GL_LINE_LOOP);
      glVertex2f(-0.9,0.38);
      glVertex2f(-0.55,0.38);
      glVertex2f(-0.6,0.43);
      glVertex2f(-0.85,0.43);
    glEnd();
    //RefreshColor();

    //cpu
    
    glBegin(GL_LINE_LOOP);
      glVertex2f(-0.5,0.38);
      glVertex2f(-0.4,0.38);
      glVertex2f(-0.4,0.65);
      glVertex2f(-0.5,0.65);
    glEnd();
    //cd driver
    glColor3f(0.14,0.64,0.75);
    glBegin(GL_LINE_LOOP);
      glVertex2f(-0.49,0.63);
      glVertex2f(-0.41,0.63);
      glVertex2f(-0.41,0.61);
      glVertex2f(-0.49,0.61);
    glEnd();
    RefreshColor();

}

void drawvisuals()
{
  glColor3d(1,1,1);
  
  //cmd section
  glBegin(GL_LINE_LOOP);
    glVertex2f(-0.95,-0.95);
    glVertex2f(0.5,-0.95);
    glVertex2f(0.5,-0.08);
    glVertex2f(-0.95,-0.08);
  glEnd();

  //progress bar
  glBegin(GL_LINE_LOOP);
    glVertex2f(-0.95,0.05);
    glVertex2f(0.95,0.05);
    glVertex2f(0.95,0.0);
    glVertex2f(-0.95,0.0);
  glEnd();

  //main body 
  glBegin(GL_LINE_LOOP);
    glVertex2f(-0.95,0.0);
    glVertex2f(0.95,0.0);
    glVertex2f(0.95,0.95);
    glVertex2f(-0.95,0.95);
  glEnd();

  //instruction section
  glBegin(GL_LINE_LOOP);
    glVertex2f(0.55,-0.95);
    glVertex2f(0.95,-0.95);
    glVertex2f(0.95,-0.08);
    glVertex2f(0.55,-0.08);
  glEnd();
  RefreshColor();

}

void refreshtimer()
{
  timer = 0.0;
}

void drawserver()
{
    // glColor3f(0.64,0.0,3.55);
  glLineWidth(1.3);
  glColor3fv(colorsdeep[4]);

  if(packetcountenableflag == 1)
            angleofrotation = packetcountingangle;
          
    glBegin(GL_LINE_LOOP);
      glVertex2f(0.65,0.35);
      glVertex2f(0.9,0.35);
      glVertex2f(0.9,0.45);
      glVertex2f(0.65,0.45);
    glEnd();
    //glColor3f(0.0,1.0,0.0);
    
    glBegin(GL_LINE_LOOP);
      glVertex2f(0.65,0.45);
      glVertex2f(0.9,0.45);
      glVertex2f(0.9,0.55);
      glVertex2f(0.65,0.55);
    glEnd();
    //glColor3f(0.0,0.0,1.0);
    glBegin(GL_LINE_LOOP);
      glVertex2f(0.65,0.55);
      glVertex2f(0.9,0.55);
      glVertex2f(0.9,0.65);
      glVertex2f(0.65,0.65);
    glEnd();
    //glColor3f(1.0,0.0,1.0);
    glBegin(GL_LINE_LOOP);
      glVertex2f(0.65,0.65);
      glVertex2f(0.9,0.65);
      glVertex2f(0.9,0.75);
      glVertex2f(0.65,0.75);
    glEnd();
    RefreshColor();
    glLineWidth(1.0);
}
void keyboard(unsigned char key, int x, int y)
{
   switch (key)
   {
      case 13:
          Refresh();
          glClearColor(0.0,1.0,0.5,0.0);
         menuscreen_caller();
         break;
      case '1':
      Refresh();
         projectscreen_caller();
         break;
      case '2':
          Refresh();
         instructionsscreen_caller();
         break;
      case 'g':
         Refresh();
         proc = 2;
         timerspeed = 0.0001;
         glutIdleFunc(idle);
         break;
      case 'p':
         Refresh();
         proc = 1;
         timerspeed = 0.0001;
         glutIdleFunc(idle);
         break;
      case 'i':
        if(timerspeed < 0.005)
          timerspeed += 0.0001;
         break;
      case 'd':
        if(timerspeed >= 0.0001)
         timerspeed -= 0.0001;
         break;
      case 't':
          refreshtimer();
          break;
      case '9':
          if(packetspeed <= 0.003)
            packetspeed += 0.001;
          break;
      case '8':
          if(packetspeed > 0.000)
            packetspeed -= 0.001;
          else
            packetspeed = 0.0;
          break;
      case 'z':
          if(controllinespeed > 0.0)
          controllinespeed -= 0.001;
          break;
      case 'x':
          if(controllinespeed <= 0.005)
          controllinespeed += 0.001;
          break;
      case 'a':
          if(angleofrotation == 0.0)
            angleofrotation = 360.0;
          else
            angleofrotation -= 1.0;
          packetcountenableflag = 0;
          break;
      case 's':
          if(angleofrotation == 360.0)
            angleofrotation = 0.0;
          else
            angleofrotation += 1.0;
          packetcountenableflag = 0;
          break;
      case 'r':
          Refresh();
          timerstopflag = 0;
          break;
      case ';':
          packetcountenableflag = 1;
          switch(packetcountingangle)
          {
            case 30:
              packetcountingangle = 184;
              noofpackets = '2';
              break;
            case 184:
              packetcountingangle = 122;
              noofpackets = '3';
              break;
            case 122:
              packetcountingangle = 92;
              noofpackets = '4';
              break;
            case 92:
              packetcountingangle = 73;
              noofpackets = '5';
              break;
            case 73:
              packetcountingangle = 61;
              noofpackets = '6';
              break;
            case 61:
              packetcountingangle = 46;
              noofpackets = '8';
              break;
            default:
              packetcountingangle = 30;
              packetcountenableflag = 0;
              noofpackets = '-';
          }
          break;
      case 'q':
         exit(0);
      case '3':
        exit(0);
   }
}

 


int main(int argc, char** argv) {
   glutInit(&argc, argv);          
   glutInitDisplayMode(GLUT_DOUBLE);  
   glutInitWindowSize(640, 480);  
   glutInitWindowPosition(50, 50); 
   window = glutCreateWindow("Project Info");  
   glutDisplayFunc(display);       
   glutReshapeFunc(reshape);       
   glutIdleFunc(idle); 
   glutKeyboardFunc(keyboard);            
   initGL();                       
   glutMainLoop();                
   return 0;
}

void display() 
{

   glClear(GL_COLOR_BUFFER_BIT);   
   glMatrixMode(GL_MODELVIEW);     
   glLoadIdentity();    
   glColor3f(0.5,1.0,0.0);
  drawText("Bangalore Institute of Technology, Bengaluru",-0.7,0.8);
  glColor3f(0.0,1.0,0.9);
    drawText("Computer Graphics Project",-0.5,0.6);
    glColor3f(1.0,0.9,0.0);
    glColor3f(0.64,0.0,3.55);
    drawSmallText("on",-0.1,0.5);
    glColor3f(1.0,0.31,0.352);          //color
    drawCurrentText("Client - Server Model",-0.35,0.4);
    glColor3f(0.32,0.123,0.84);
    drawSmallText("Press Enter to Continue",-0.35,-0.4);
    glColor3f(0.82,0.31,0.321);
   drawSmallText("By  Sujay B R",0.4,-0.8);
   drawSmallText("1BI14CS163",0.5,-0.9);
   RefreshColor();
   glutSwapBuffers();   
}

void reshape(GLsizei width, GLsizei height) {  
   if (height == 0) height = 1; 
   WINDOW_WIDTH = width;
   WINDOW_HEIGHT = height;               
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
   glViewport(0, 0, width, height);
 
   glMatrixMode(GL_PROJECTION);  
   glLoadIdentity();
   if (width >= height) {
      gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
   } else {
     gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
   }
}

void initGL() 
{
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}
 

void drawText(char string[],float x, float y)
{
   int len, i;
   glRasterPos2f(x,y);
   len=(int) strlen(string);
   for(i = 0; i < len; i++)
   {
     glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,string[i]);
   } 
}


void drawSmallText(char string[],float x, float y)
{
   int len, i;
   glRasterPos2f(x,y);
   len=(int) strlen(string);
   for(i = 0; i < len; i++)
   {
     glutBitmapCharacter(GLUT_BITMAP_9_BY_15,string[i]);
   } 
}


void drawCurrentText(char string[],float x, float y)
{
   int len, i;
   glRasterPos2f(x,y);
   len=(int) strlen(string);
   for(i = 0; i < len; i++)
   {
     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 ,string[i]);
   } 
}

void Setting(double r,double g,double b,double alpha)
{
   glClearColor (r, g, b, alpha);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}


void instructionsscreen()
{
    glClear(GL_COLOR_BUFFER_BIT );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(1.0,1.0,0.0);
    drawText("Instructions screen",-0.2,0.7);
    glColor3fv(colorsdeep[4]);
    drawSmallText("A Get request is used to retrieve data from a server.",-0.95,0.4);
    drawSmallText("A Post request is used to store some data into the server for future use.",-0.95,0.3);

    glColor3f(0.32,0.123,0.84);
    drawSmallText("Press '1' for Project Screen or press '3' to Quit",-0.65,-0.8);
    
    glColor3f(0.0,1.0,0.0);
    drawSmallText("Project Screen Commands:",-0.95,0.2);
    drawSmallText("Post request - p",-0.95,0.1);
    drawSmallText("Get request - g",0.0,0.1);
    // drawSmallText("",0.56,-0.25);''
    drawSmallText("inc timerspeed - i",-0.95,0.0);
    drawSmallText("dec timerspeed - d",0.0,0.0);
    drawSmallText("inc packetspeed - 9",-0.95,-0.1);
    drawSmallText("dec packetspeed - 8",0.0,-0.1);
    drawSmallText("Refresh Timer - t",-0.95,-0.2);
    drawSmallText("inc angle - s",-0.95,-0.3);
    drawSmallText("dec angle - a", 0.0,-0.3);
    drawSmallText("change no. of packets - ;",-0.95,-0.4);
    
    drawSmallText("Menu Screen - m",-0.95,-0.5);
    drawSmallText("Project Screen - 1",0.0,-0.5);
    drawSmallText("Instructions Screen - 2",-0.95,-0.6);
    // drawSmallText("               - 2",0.56,-0.85);
    drawSmallText("Quit - q",0.0,-0.6);
    
    glutSwapBuffers();
    RefreshColor();
}

void instructionsscreen_caller()
{
    glutDestroyWindow(window);
    glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (800, 600);
    glutInitWindowPosition (250,70);
    window=glutCreateWindow("Instructions");
    glutDisplayFunc(instructionsscreen);
    glutIdleFunc(instructionsscreen);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    Setting (0,0,0,0);
    glutMainLoop(); 
    //return 0;
}

void projectscreen_caller()
{
    glutDestroyWindow(window);
    glutInitDisplayMode ( GLUT_DOUBLE);
    WINDOW_WIDTH = 900;
    WINDOW_HEIGHT = 650;
    glutInitWindowSize (WINDOW_WIDTH,WINDOW_HEIGHT);
    glutInitWindowPosition(0,0);
    window=glutCreateWindow("Client Server Model");
    glutDisplayFunc(projectscreen);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    Setting (0,0,0,0);
    initGL();
    createMenu();
    glutMainLoop(); 
    //return 0;
}


void menuscreen_caller()
{
    glutDestroyWindow(window);
    glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (800, 600);
    glutInitWindowPosition (250, 70);
    window=glutCreateWindow("Menus");
    glutDisplayFunc(menuscreen);
    glutIdleFunc(menuscreen);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    gluOrtho2D (0.0, 200.0, 0.0, 150.0);
    Setting (0,0,0,0);
    glutMainLoop();
    //return 0;
}

void menuscreen()
{
    glClear(GL_COLOR_BUFFER_BIT );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(0.23,0.452,0.5653); 
    drawText("Implementation of Client Server Architecture",-0.5,0.7);
    glColor3f(0,1,0);
    drawText("1.Project Screen",-0.3,0.3);
    glColor3f(1,1,0);
    drawText("2.Instructions",-0.3,0.15);
    glColor3f(1,0.5,0);
    drawText("3.Quit",-0.3,0.0);
    glColor3f(0,0,0);
    glutSwapBuffers();
}


void drawinstructions()
{
    glColor3f(1.0,0.0,0.0);
    drawSmallText("timer",-0.9,0.07);
    RefreshColor();
    drawSmallText("Post request - p",0.56,-0.15);
    drawSmallText("Get request - g",0.56,-0.20);
    // drawSmallText("",0.56,-0.25);
    drawSmallText("inc timerspeed - i",0.56,-0.25);
    drawSmallText("dec timerspeed - d",0.56,-0.3);
    drawSmallText("inc packetspeed - 9",0.56,-0.35);
    drawSmallText("dec packetspeed - 8",0.56,-0.4);
    drawSmallText("Refresh Timer - t",0.56,-0.45);
    drawSmallText("inc angle - s",0.56,-0.5);
    drawSmallText("dec angle - a", 0.56,-0.55);
    drawSmallText("no. of packets - ;",0.56,-0.6);
    // drawSmallText("",0.56,-0.7);
    drawSmallText("Menu Screen - m",0.56,-0.65);
    drawSmallText("Project Screen - 1",0.56,-0.7);
    drawSmallText("Instructions Screen",0.56,-0.75);
    drawSmallText("               - 2",0.56,-0.8);
    drawSmallText("Quit - q",0.56,-0.85);
}
` 
void drawcurrentdetails()
{
    char str[1000];
      if(!timerstopflag)
      {
          if(proc == 1)
          {
              if(postpacketflag == 1)
                sprintf(str,"Request Type : Post\n      Current Status : Acknoledge from Server\n");
              else if(controllineflag1 == 1)
                sprintf(str,"Request Type : Post\n      Current Status : Client Posting Packets\n");
              else
              sprintf(str,"Request Type : Post\n        Current Status : Post Control Request from Client\n");
          }
          else if(proc == 2)
          {
              if(getpacketflag)
                  sprintf(str,"Request Type : Get          Current Status : Acknoledge from Client\n");
              else if(aflag)
                  sprintf(str,"Request Type : Get          Current Status : Getting Packets from Server\n");
              else
                  sprintf(str,"Request Type : Get          Current Status : Get Control Request from Client\n");

          }
          else
          { 
              glColor3f(0.0,0.0,1.0);
              sprintf(str,"Make a Get(g) Request or a Post(p) Request to Continue");
          }
          drawCurrentText(str,-0.9,-0.25);
          RefreshColor();
      }
      else
      { 
          glColor3f(1.0,0.0,0.0);
          drawCurrentText("Timed out! Packet/Request Discarded, Hit r to refresh",-0.9,-0.25);
          RefreshColor();
      }
      RefreshColor();
}

void drawcurrentstatus()
{
    char str[100];

    glColor3fv(colorslite[5]);
    sprintf(str,"Timer speed : %.4lf",timerspeed);
    drawCurrentText(str,-0.93,-0.55);

    sprintf(str,"Packet Speed : %.3lf", (packetspeed <= 50) ? packetspeed : 0.000);
    drawCurrentText(str,-0.23,-0.55);    

    sprintf(str,"Intrupt Speed : %.3lf", controllinespeed);
    drawCurrentText(str,-0.93,-0.65); 

    sprintf(str,"Angle of Rotation : %.3lf", angleofrotation);
    drawCurrentText(str,-0.93,-0.75);    

    sprintf(str,"Number of Packets : %c", noofpackets);
    drawCurrentText(str,-0.23,-0.65);

    switch(back_color)
        {
          case 1:
          // glColor3f(0.3,0.3,0.3);
          sprintf(str,"Background - Grey\n");
          break;
          case 3:
          // glColor3f(0.3, 0.3, 0.6);
          sprintf(str,"Background - Dark Green\n");
          break;
          case 4:
          sprintf(str,"Background - Purple\n");
          // glColor3f(0.5,0.1,0.5);
          break;
          default:
          sprintf(str,"Background - Black\n");
          // glColor3f(0.0,0.0,0.0);
          break;
        }
      drawCurrentText(str,-0.93,-0.85);

      switch(packet_colors)
      {
        case 7:
        glColor3f(1.0,0.0,0.0);
        break;
        case 9:
        glColor3f(0.0,0.0,1.0);
        break;
        case 8:
        glColor3f(0.0,1.0,1.0);
        break;
        default:
        glColor3f(1.0,0.0,0.0);
        break;
      }
      drawCurrentText("Packet Color",-0.23,-0.75);


      switch(intrupt_color)
    {
      case 4:
        glColor3f(1.0,0.0,0.0);
        break;
      case 5:
        glColor3f(0.0,1.0,0.0);
        break;
      case 6:
        glColor3f(0.0,0.0,1.0);
        break;
      default:
        glColor3f(1.0,0.0,0.0);
        break;
    }
    drawCurrentText("Control Lines Color",-0.23,-0.85);

    RefreshColor();
}

void Refresh()
{
     controllineflag1 = 0;
postpacketflag = 0;
aflag = 0;
getpacketflag = 0;
 ack = 0;
 proc = 0;
 dir = 0.0;
 packetcountenableflag = 0;

//for translation and rotation and scaling
 packet = 0.0;
 timer = 0;
 angle = 0.0;
  scalepackets = 0.0;
 controllinepos1 = 0.001;
 controllinepos2 = 0.001;

//color flags
intrupt_color = 0;
packet_colors = 0;
back_color = 0;  

//menu flags 
submenu_id = 0;
bmenu_id = 0;
menu_id = 0;
nu_id = 0;

//variables choices;
 packetspeed = 0.001;
  timerspeed = 0.0;
 angleofrotation = 3.0;
 controllinespeed = 0.001;

 glColor3f(0.0,1.0,0.0);

 drawcurrentdetails();

}

void createMenu(void)
{
 //sub menu entry
 submenu_id = glutCreateMenu(menu);
 glutAddMenuEntry("Grey",1);
 glutAddMenuEntry("Dark Green",2);
 glutAddMenuEntry("Black",3);
 glutAddMenuEntry("Purple",10);

 //submenu entry
 bmenu_id = glutCreateMenu(menu);
 glutAddMenuEntry("Red",4);
 glutAddMenuEntry("Green",5);
 glutAddMenuEntry("Blue",6);

 //sub menu entry
 menu_id = glutCreateMenu(menu);
 glutAddMenuEntry("Red",7);
 glutAddMenuEntry("cyan",8);
 glutAddMenuEntry("Blue",9);

 //main menu
 nu_id = glutCreateMenu(menu);
 glutAddSubMenu("Background", submenu_id);
 glutAddSubMenu("Control Lines", bmenu_id);
 glutAddSubMenu("Packets", menu_id);
 glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Handles the menus
void menu(int num)
{
 if(num == 0)
 {
 //glutDestroyWindow(window);
 exit(0);
 }
 else
 {
 switch(num)
  {
   
    case 1:
      back_color = 1;
      break;
    case 2:
    back_color = 3;
    break;
    case 3:
    back_color = 2;
    break;
    case 4:
    intrupt_color = 4;
    break;
    case 5:
    intrupt_color = 5;
    break;
    case 6:
    intrupt_color = 6;
    break;
    case 7:
    packet_colors = 7;
    break;
    case 8:
    packet_colors = 8;
    break;
    case 9:
    packet_colors = 9;
    break;
    case 10:
    back_color = 4;
    break;
  }
 
  glutPostRedisplay();
}
}