//
// A GLSL "Hello World"
// Display a blue square
//

#include "Angel.h"  //includes gl.h, glut.h and other stuff...
#include "Camera.h"  //for camera objects (for use in future assignments)
#include "Light.h"	//for lights (for use in future assignments)
#include "Sphere.h"  //blue box object!
#include "Cube.h"
#include "Plane.h"
#include "Table.h"

//Forward declarations
void init(void);
void display(void);
void keyboard(unsigned char, int, int);
void resize(int width, int height);
void close(void);
void specialKeyboard(int, int, int);
void timerCallback(int value);

void makeMove(int move);
int getPlayerNumber();
void checkForWin();
void cubeWins();
void sphereWins();
void noWins();

bool cubeWinner, sphereWinner;

vec4 light_position0(0,0,0,0 );
vec4 light_position1(0, 0, 0, 0.0 );

vec4 light_ambient0( 0.2, 0.2, 0.2, 1.0 );
vec4 light_diffuse0( 1.0, 1.0, 1.0, 1.0 );
vec4 light_specular0( 1.0, 1.0, 1.0, 0.0 );

vec4 light_ambient1( 0.2, 0.2, 0.2, 1.0 );
vec4 light_diffuse1( 1.0, 1.0, 1.0, 1.0 );
vec4 light_specular1( 1.0, 1.0, 1.0, 1.0 );

vec4 cubeVerticies[8];
int isOccupied[9];
int winningPos[3];


//Objects
Plane* plane;
Table* table;
vector<Cube*> cubePieces;
vector<Sphere*> spherePieces;

Camera* cam;
Camera* cam1;
Camera* cam2;
vector<Light> lights;
vector<Drawable*>drawables;
vector<bool>enabled;
double theta = 0;
double alpha = 1;
int interval = 1;
bool using2 = false;
bool playerCube = false;
bool growing = true;
vec4 material_ambient( 1.0, 0.0, 1.0, 1.0 );
vec4 material_diffuse( 1.0, 0.8, 0.0, 1.0 );
vec4 material_specular( 1.0, 0.8, 0.0, 1.0 );

vec4 material_ambient2( 0, 1, 0, 1.0 );
vec4 material_diffuse2( 0, 1,1, 0 );
vec4 material_specular2( 0, 1, 0, 1.0 );
float material_shininess = 100.0;

vec2 coordinates[9] = {vec2(-.7,-0.7),vec2(0,-0.7),vec2(.7,-0.7),vec2(-0.7,0),vec2(0,0),vec2(0.7,0),vec2(-0.7,0.7),vec2(0,0.7),vec2(0.7,0.7)};


GLuint windowID=0;
//----------------------------------------------------------------------------

int main(int argc, char **argv)
{
    //Allow for placement on board
    for(int i = 0; i < 9; i++){
        isOccupied[i] = false;
    }
    
    cubeWinner = false;
    sphereWinner = false;
    
    //initialize GLUT
    glutInit(&argc, argv);
#ifdef __APPLE__
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
#else
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif
    glutInitWindowSize(512, 512);
    
    windowID = glutCreateWindow("3D Sample");
    
    //print out info about our system
    fprintf(stdout, "OpenGL Version: %s\n", glGetString(GL_VERSION));
    fprintf(stdout, "GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    
    //initialize glew on non-apple systems
#ifndef __APPLE__
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif
    
    init(); //initalize objects
    
    //set up the callback functions
    glutDisplayFunc(display);
    glutWMCloseFunc(close);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    //start the main event listening loop
    glutMainLoop();
    return 0;
}

// Initialization
void init()
{
    glClearColor(0.3, 0.8, 1, 1.0);
    glEnable(GL_DEPTH_TEST);
    
    cam2 = new Camera(0,.5,10,true);
    cam1 = new Camera(0,10,0,false);
    cam = cam1;

    
    enabled.push_back(true);
    enabled.push_back(true);
    
    
    vec4 planeVertices[4] = { vec4(-10, -1, 10, 1), vec4(10, -1, 10, 1), vec4(10, -1, -10, 1), vec4(-10, -1, -10, 1)};
    plane = new Plane(planeVertices, "grass.ppm");
    drawables.push_back(plane);
    
    table = new Table();
    drawables.push_back(table);
    
    //lights
    lights.push_back(Light(light_position0,light_ambient0, light_diffuse0, light_specular0));
    lights.push_back(Light(light_position1,light_ambient1, light_diffuse1, light_specular1));
    
}

//----------------------------------------------------------------------------
//Display callback
void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    light_position1 = vec4(cam->at[0], cam->at[1], cam->at[2]-1.0, 0.0 );
    lights[1] = Light(light_position1,light_ambient1, light_diffuse1, light_specular1);
    for (unsigned int i = 0; i < drawables.size(); i++)
        drawables[i]->draw(cam,lights,enabled);
    glutSwapBuffers();
}

//----------------------------------------------------------------------------
//Window resize callback
void resize(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);  //make the viewport the entire window
}





//----------------------------------------------------------------------------
//Keyboard event callback
void keyboard( unsigned char key, int x, int y )
{
    int k;
    switch( key ) {
        case ' ':
            if(using2){
                cam = cam1;
            } else {
                cam = cam2;
            }
            using2 = !using2;
            display();
            break;
        case 033:  // Escape key
        case 'q': case 'Q':
            close();
            break;
        case 'X':
            cam->pitchDown();
            glutPostRedisplay();
            break;
        case 'x':
            cam->pitchUp();
            glutPostRedisplay();
            break;
        case 'C':
            cam->yawClockwise();
            glutPostRedisplay();
            break;
        case 'c':
            cam->yawCClockwise();
            glutPostRedisplay();
            break;
        case 'Z':
            cam->rollClockwise();
            glutPostRedisplay();
            break;
        case 'z':
            cam->rollCClockwise();
            glutPostRedisplay();
            break;
        case '1':
            if(isOccupied[0] == 0){
                cubeVerticies[0] = vec4(-0.4,0.4,-0.4,1);
                cubeVerticies[1] = vec4(-0.4,0.4,-0.3,1);
                cubeVerticies[2] = vec4(-0.3,0.4,-0.3,1);
                cubeVerticies[3] = vec4(-0.3,0.4,-0.4,1);
                cubeVerticies[4] = vec4(-0.4,0.3,-0.4,1);
                cubeVerticies[5] = vec4(-0.4,0.3,-0.3,1);
                cubeVerticies[6] = vec4(-0.3,0.3,-0.3,1);
                cubeVerticies[7] = vec4(-0.3,0.3,-0.4,1);
                k = (int)key - 49;
                isOccupied[0] = getPlayerNumber();
                makeMove(k);
            }
            break;
        case '2':
            if(isOccupied[1] == 0){
                cubeVerticies[0] = vec4(0.05,0.4,-0.4,1);
                cubeVerticies[1] = vec4(0.05,0.4,-0.3,1);
                cubeVerticies[2] = vec4(-0.05,0.4,-0.3,1);
                cubeVerticies[3] = vec4(-0.05,0.4,-0.4,1);
                cubeVerticies[4] = vec4(0.05,0.3,-0.4,1);
                cubeVerticies[5] = vec4(0.05,0.3,-0.3,1);
                cubeVerticies[6] = vec4(-0.05,0.3,-0.3,1);
                cubeVerticies[7] = vec4(-0.05,0.3,-0.4,1);
                k = (int)key - 49;
                isOccupied[1] = getPlayerNumber();
                makeMove(k);
            }
            break;
        case '3':
            if(isOccupied[2] == 0){
                cubeVerticies[0] = vec4(0.4,0.4,-0.4,1);
                cubeVerticies[1] = vec4(0.4,0.4,-0.3,1);
                cubeVerticies[2] = vec4(0.3,0.4,-0.3,1);
                cubeVerticies[3] = vec4(0.3,0.4,-0.4,1);
                cubeVerticies[4] = vec4(0.4,0.3,-0.4,1);
                cubeVerticies[5] = vec4(0.4,0.3,-0.3,1);
                cubeVerticies[6] = vec4(0.3,0.3,-0.3,1);
                cubeVerticies[7] = vec4(0.3,0.3,-0.4,1);
                k = (int)key - 49;
                isOccupied[2] = getPlayerNumber();
                makeMove(k);
            }
            break;
        case '4':
            if(isOccupied[3] == 0){
                cubeVerticies[0] = vec4(-0.4,0.4,-0.05,1);
                cubeVerticies[1] = vec4(-0.4,0.4,0.05,1);
                cubeVerticies[2] = vec4(-0.3,0.4,0.05,1);
                cubeVerticies[3] = vec4(-0.3,0.4,-0.05,1);
                cubeVerticies[4] = vec4(-0.4,0.3,-0.05,1);
                cubeVerticies[5] = vec4(-0.4,0.3,0.05,1);
                cubeVerticies[6] = vec4(-0.3,0.3,0.05,1);
                cubeVerticies[7] = vec4(-0.3,0.3,-0.05,1);
                k = (int)key - 49;
                isOccupied[3] = getPlayerNumber();
                makeMove(k);
            }
            break;
        case '5':
            if(isOccupied[4] == 0){
                cubeVerticies[0] = vec4(0.05,0.4,-0.05,1);
                cubeVerticies[1] = vec4(0.05,0.4,0.05,1);
                cubeVerticies[2] = vec4(-0.05,0.4,0.05,1);
                cubeVerticies[3] = vec4(-0.05,0.4,-0.05,1);
                cubeVerticies[4] = vec4(0.05,0.3,-0.05,1);
                cubeVerticies[5] = vec4(0.05,0.3,0.05,1);
                cubeVerticies[6] = vec4(-0.05,0.3,0.05,1);
                cubeVerticies[7] = vec4(-0.05,0.3,-0.05,1);
                k = (int)key - 49;
                isOccupied[4] = getPlayerNumber();
                makeMove(k);
            }
            break;
        case '6':
            if(isOccupied[5] == 0){
                cubeVerticies[0] = vec4(0.4,0.4,-0.05,1);
                cubeVerticies[1] = vec4(0.4,0.4,0.05,1);
                cubeVerticies[2] = vec4(0.3,0.4,0.05,1);
                cubeVerticies[3] = vec4(0.3,0.4,-0.05,1);
                cubeVerticies[4] = vec4(0.4,0.3,-0.05,1);
                cubeVerticies[5] = vec4(0.4,0.3,0.05,1);
                cubeVerticies[6] = vec4(0.3,0.3,0.05,1);
                cubeVerticies[7] = vec4(0.3,0.3,-0.05,1);
                k = (int)key - 49;
                isOccupied[5] = getPlayerNumber();
                makeMove(k);
            }
            break;
        case '7':
            if(isOccupied[6] == 0){
                cubeVerticies[0] = vec4(-0.4,0.4,0.4,1);
                cubeVerticies[1] = vec4(-0.4,0.4,0.3,1);
                cubeVerticies[2] = vec4(-0.3,0.4,0.3,1);
                cubeVerticies[3] = vec4(-0.3,0.4,0.4,1);
                cubeVerticies[4] = vec4(-0.4,0.3,0.4,1);
                cubeVerticies[5] = vec4(-0.4,0.3,0.3,1);
                cubeVerticies[6] = vec4(-0.3,0.3,0.3,1);
                cubeVerticies[7] = vec4(-0.3,0.3,0.4,1);
                k = (int)key - 49;
                isOccupied[6] = getPlayerNumber();
                makeMove(k);
            }
            break;
        case '8':
            if(isOccupied[7] == 0){
                cubeVerticies[0] = vec4(0.05,0.4,0.4,1);
                cubeVerticies[1] = vec4(0.05,0.4,0.3,1);
                cubeVerticies[2] = vec4(-0.05,0.4,0.3,1);
                cubeVerticies[3] = vec4(-0.05,0.4,0.4,1);
                cubeVerticies[4] = vec4(0.05,0.3,0.4,1);
                cubeVerticies[5] = vec4(0.05,0.3,0.3,1);
                cubeVerticies[6] = vec4(-0.05,0.3,0.3,1);
                cubeVerticies[7] = vec4(-0.05,0.3,0.4,1);
                k = (int)key - 49;
                isOccupied[7] = getPlayerNumber();
                makeMove(k);
            }
            break;
        case '9':
            if(isOccupied[8] == 0){
                cubeVerticies[0] = vec4(0.4,0.4,0.4,1);
                cubeVerticies[1] = vec4(0.4,0.4,0.3,1);
                cubeVerticies[2] = vec4(0.3,0.4,0.3,1);
                cubeVerticies[3] = vec4(0.3,0.4,0.4,1);
                cubeVerticies[4] = vec4(0.4,0.3,0.4,1);
                cubeVerticies[5] = vec4(0.4,0.3,0.3,1);
                cubeVerticies[6] = vec4(0.3,0.3,0.3,1);
                cubeVerticies[7] = vec4(0.3,0.3,0.4,1);
                k = (int)key - 49;
                isOccupied[8] = getPlayerNumber();
                makeMove(k);
            }
            break;
    }
}

void makeMove(int position){
    float x = coordinates[position].x;
    float z = coordinates[position].y;
    if(playerCube){
        Cube* newCube = new Cube(cubeVerticies);
        newCube->setMaterial(material_ambient, material_diffuse, material_specular, material_shininess);
        newCube->setModelMatrix(Scale(2, 2, 2)*Translate(0, 0, 0));  //scale it
        newCube->setBoardPosition(position);
        drawables.push_back(newCube);
        cubePieces.push_back(newCube);
    } else {
        Sphere* newSphere = new Sphere();
        newSphere->setMaterial(material_ambient, material_diffuse, material_specular, material_shininess);
        newSphere->x = x;
        newSphere->z = z;
        newSphere->setModelMatrix(Translate(x, .7, z));  //scale it
        newSphere->setBoardPosition(position);
        drawables.push_back(newSphere);
        spherePieces.push_back(newSphere);
    }
    playerCube = !playerCube;
    glutPostRedisplay();
    checkForWin();
}

int getPlayerNumber(){
    if(playerCube){
        return 1;
    }
    else{
        return 2;
    }
}

void checkForWin(){
    
    //  0   1   2
    //  3   4   5
    //  6   7   8
    
    if(isOccupied[0] == isOccupied[1] && isOccupied[1] == isOccupied[2] && isOccupied[2] != 0){
        //Top Across
        winningPos[0] = 0;
        winningPos[1] = 1;
        winningPos[2] = 2;
        if(isOccupied[0] == 1){
            cubeWins();
        }
        else{
            sphereWins();
        }
    }
    else if(isOccupied[3] == isOccupied[4] && isOccupied[4] == isOccupied[5] && isOccupied[5] != 0){
        //Middle Across
        winningPos[0] = 3;
        winningPos[1] = 4;
        winningPos[2] = 5;
        if(isOccupied[3] == 1){
            cubeWins();
        }
        else{
            sphereWins();
        }
    }
    else if(isOccupied[6] == isOccupied[7] && isOccupied[7] == isOccupied[8] && isOccupied[8] != 0){
        //Bottom Across
        winningPos[0] = 6;
        winningPos[1] = 7;
        winningPos[2] = 8;
        if(isOccupied[6] == 1){
            cubeWins();
        }
        else{
            sphereWins();
        }
    }
    
    else if(isOccupied[0] == isOccupied[3] && isOccupied[3] == isOccupied[6] && isOccupied[6] != 0){
        //Left Down
        winningPos[0] = 0;
        winningPos[1] = 3;
        winningPos[2] = 6;
        if(isOccupied[0] == 1){
            cubeWins();
        }
        else{
            sphereWins();
        }
    }
    else if(isOccupied[1] == isOccupied[4] && isOccupied[4] == isOccupied[7] && isOccupied[7] != 0){
        //Middle Down
        winningPos[0] = 1;
        winningPos[1] = 4;
        winningPos[2] = 7;
        if(isOccupied[1] == 1){
            cubeWins();
        }
        else{
            sphereWins();
        }
    }
    else if(isOccupied[2] == isOccupied[5] && isOccupied[5] == isOccupied[8] && isOccupied[8] != 0){
        //Right Down
        winningPos[0] = 2;
        winningPos[1] = 5;
        winningPos[2] = 8;
        if(isOccupied[2] == 1){
            cubeWins();
        }
        else{
            sphereWins();
        }
    }
    
    else if(isOccupied[0] == isOccupied[4] && isOccupied[4] == isOccupied[8] && isOccupied[8] != 0){
        //Top Left to Bottom Right
        winningPos[0] = 0;
        winningPos[1] = 4;
        winningPos[2] = 8;
        if(isOccupied[0] == 1){
            cubeWins();
        }
        else{
            sphereWins();
        }
    }
    else if(isOccupied[2] == isOccupied[4] && isOccupied[4] == isOccupied[6] && isOccupied[6] != 0){
        //Top Right to Bottom Left
        winningPos[0] = 2;
        winningPos[1] = 4;
        winningPos[2] = 6;
        if(isOccupied[2] == 1){
            cubeWins();
        }
        else{
            sphereWins();
        }
    }
    else{
        int points = 0;
        for(int i = 0; i < 9; i++){
            points = points + isOccupied[i];
        }
        if(points == 14){
            noWins();
        }
    }
}

void cubeWins(){
    //Run if 3 Cubes in a row
    cubeWinner = true;
    glutTimerFunc(50, timerCallback, 0);
}

void sphereWins(){
    //Run if 3 Spheres in a row
    cubeWinner = false;
    glutTimerFunc(50, timerCallback, 0);
}

void noWins(){
    //Run if out of moves
}

void specialKeyboard( int key, int x, int y )
{
    if(key == GLUT_KEY_UP){
        cam->moveTowards();
        glutPostRedisplay();
    } else if(key == GLUT_KEY_DOWN){
        cam->moveAway();
        glutPostRedisplay();
    } else if(key == GLUT_KEY_RIGHT){
        cam->moveRight();
        glutPostRedisplay();
    } else if(key == GLUT_KEY_LEFT){
        cam->moveLeft();
        glutPostRedisplay();
    }
}


void timerCallback(int value)
{
    theta=theta+5;
    
    if(growing) {
        alpha=alpha+0.05;
        if(alpha > 2) growing = false;
    } else {
        alpha=alpha-0.05;
        if(alpha < 1) growing = true;
    }
    
    if(cubeWinner){
        for(int i = 0; i < cubePieces.size(); i++){
            if(cubePieces.at(i)->getBoardPosition() == winningPos[0] || cubePieces.at(i)->getBoardPosition() == winningPos[1] || cubePieces.at(i)->getBoardPosition() == winningPos[2]){
                vec3 centers = cubePieces.at(i)->getCenter();
                cubePieces.at(i)->setModelMatrix(Scale(2,2,2) * Translate(centers[0], centers[1], centers[2]) * RotateY(theta) * Translate(-centers[0], -centers[1], -centers[2]));
            }
        }
    }
    else{
        for(int i = 0; i < spherePieces.size(); i++){
            if(spherePieces.at(i)->getBoardPosition() == winningPos[0] || spherePieces.at(i)->getBoardPosition() == winningPos[1] || spherePieces.at(i)->getBoardPosition() == winningPos[2]){
                spherePieces.at(i)->setModelMatrix(Translate(spherePieces.at(i)->x, 0.7, spherePieces.at(i)->z)*Scale(alpha,alpha,alpha));
            }
        }
    }
    
    
    glutTimerFunc(50, timerCallback, value);
    glutPostRedisplay();
}


void close(){
    for (unsigned int i = 0; i < drawables.size(); i++)
        delete(drawables[i]);
    
    if(windowID>0)
        glutDestroyWindow(windowID);
    
    exit(EXIT_SUCCESS);
}

