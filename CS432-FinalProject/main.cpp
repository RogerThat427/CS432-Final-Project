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



//Objects
Plane* plane;
Cube* tableTop;
Cube* legOne;
Cube* legTwo;
Cube* legThree;
Cube* legFour;

vector<Cube*> cubePieces;
vector<Sphere*> spherePieces;

Camera* cam;
Camera* cam1;
Camera* cam2;
vector<Light> lights;
vector<Drawable*>drawables;
vector<bool>enabled;
double theta = 45;
int interval = 1;
bool using2 = false;
bool playerCube = false;
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
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    
    cam2 = new Camera(0,.5,10,true);
    cam1 = new Camera(0,10,0,false);
    cam = cam1;

    
    enabled.push_back(true);
    enabled.push_back(true);
    
    tableTop = new Cube();
    legOne = new Cube();
    legTwo = new Cube();
    legThree = new Cube();
    legFour = new Cube();
    
    tableTop -> setVertices(vec4(0.6,0.3,0.5,1),vec4(0.6,0.3,-0.5,1),vec4(-0.6,0.3,-0.5,1),vec4(-0.6,0.3,0.5,1),vec4(0.6,0.2,0.5,1),vec4(.6,.2,-.5,1),vec4(-0.6,0.2,-0.5,1),vec4(-0.6,0.2,0.5,1));
    
    legOne -> setVertices(vec4(0.5,0.2,0.4,1), vec4(0.5,0.2,0.3,1), vec4(0.4,0.2,0.3,1), vec4(0.4,0.2,0.4,1), vec4(0.5,-0.3,0.4,1), vec4(0.5,-0.3,0.3,1), vec4(0.4,-0.3,0.3,1), vec4(0.4,-0.3,0.4,1));
    
    legTwo -> setVertices(vec4(-0.5,0.2,0.4,1), vec4(-0.5,0.2,0.3,1), vec4(-0.4,0.2,0.3,1), vec4(-0.4,0.2,0.4,1), vec4(-0.5,-0.3,0.4,1), vec4(-0.5,-0.3,0.3,1), vec4(-0.4,-0.3,0.3,1), vec4(-0.4,-0.3,0.4,1));
    
    legThree -> setVertices(vec4(-0.5,0.2,-0.4,1), vec4(-0.5,0.2,-0.3,1), vec4(-0.4,0.2,-0.3,1), vec4(-0.4,0.2,-0.4,1), vec4(-0.5,-0.3,-0.4,1), vec4(-0.5,-0.3,-0.3,1), vec4(-0.4,-0.3,-0.3,1), vec4(-0.4,-0.3,-0.4,1));
    
    legFour -> setVertices(vec4(0.5,0.2,-0.4,1), vec4(0.5,0.2,-0.3,1), vec4(0.4,0.2,-0.3,1), vec4(0.4,0.2,-0.4,1), vec4(0.5,-0.3,-0.4,1), vec4(0.5,-0.3,-0.3,1), vec4(0.4,-0.3,-0.3,1), vec4(0.4,-0.3,-0.4,1));
    
    tableTop->setMaterial(material_ambient, material_diffuse, material_specular, material_shininess);
    legOne->setMaterial(material_ambient, material_diffuse, material_specular, material_shininess);
    legTwo->setMaterial(material_ambient, material_diffuse, material_specular, material_shininess);
    legThree->setMaterial(material_ambient, material_diffuse, material_specular, material_shininess);
    legFour->setMaterial(material_ambient, material_diffuse, material_specular, material_shininess);
    
    tableTop->setModelMatrix(Scale(2, 2, 2)*Translate(0, 0, 0));  //scale it
    legOne->setModelMatrix(Scale(2, 2, 2)*Translate(0, 0, 0));  //scale it
    legTwo->setModelMatrix(Scale(2, 2, 2)*Translate(0, 0, 0));  //scale it
    legThree->setModelMatrix(Scale(2, 2, 2)*Translate(0, 0, 0));  //scale it
    legFour->setModelMatrix(Scale(2, 2, 2)*Translate(0, 0, 0));  //scale it
    
    
    drawables.push_back(tableTop);
    drawables.push_back(legOne);
    drawables.push_back(legTwo);
    drawables.push_back(legThree);
    drawables.push_back(legFour);
    
    plane = new Plane();
    plane->setMaterial(material_ambient2, material_diffuse2, material_specular2, material_shininess);
    drawables.push_back(plane);
    
    //lights
    lights.push_back(Light(light_position0,light_ambient0, light_diffuse0, light_specular0));
    lights.push_back(Light(light_position1,light_ambient1, light_diffuse1, light_specular1));
    glutTimerFunc(10,timerCallback,0);
    
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
        Cube* newCube = new Cube();
        newCube -> setVertices(cubeVerticies[0], cubeVerticies[1], cubeVerticies[2], cubeVerticies[3], cubeVerticies[4], cubeVerticies[5], cubeVerticies[6], cubeVerticies[7]);
        newCube->setMaterial(material_ambient, material_diffuse, material_specular, material_shininess);
        newCube->setModelMatrix(Scale(2, 2, 2)*Translate(0, 0, 0));  //scale it
        drawables.push_back(newCube);
        cubePieces.push_back(newCube);
    } else {
        Sphere* newSphere = new Sphere();
        newSphere->setMaterial(material_ambient, material_diffuse, material_specular, material_shininess);
        newSphere->setModelMatrix(Translate(x, .7, z));  //scale it
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
        if(isOccupied[0] == 1){
            cubeWins();
        }
        else{
            sphereWins();
        }
    }
    else if(isOccupied[3] == isOccupied[4] && isOccupied[4] == isOccupied[5] && isOccupied[5] != 0){
        //Middle Across
        if(isOccupied[3] == 1){
            cubeWins();
        }
        else{
            sphereWins();
        }
    }
    else if(isOccupied[6] == isOccupied[7] && isOccupied[7] == isOccupied[8] && isOccupied[8] != 0){
        //Bottom Across
        if(isOccupied[6] == 1){
            cubeWins();
        }
        else{
            sphereWins();
        }
    }
    
    else if(isOccupied[0] == isOccupied[3] && isOccupied[3] == isOccupied[6] && isOccupied[6] != 0){
        //Left Down
        if(isOccupied[0] == 1){
            cubeWins();
        }
        else{
            sphereWins();
        }
    }
    else if(isOccupied[1] == isOccupied[4] && isOccupied[4] == isOccupied[7] && isOccupied[7] != 0){
        //Middle Down
        if(isOccupied[1] == 1){
            cubeWins();
        }
        else{
            sphereWins();
        }
    }
    else if(isOccupied[2] == isOccupied[5] && isOccupied[5] == isOccupied[8] && isOccupied[8] != 0){
        //Right Down
        if(isOccupied[2] == 1){
            cubeWins();
        }
        else{
            sphereWins();
        }
    }
    
    else if(isOccupied[0] == isOccupied[4] && isOccupied[4] == isOccupied[8] && isOccupied[8] != 0){
        //Top Left to Bottom Right
        if(isOccupied[0] == 1){
            cubeWins();
        }
        else{
            sphereWins();
        }
    }
    else if(isOccupied[2] == isOccupied[4] && isOccupied[4] == isOccupied[6] && isOccupied[6] != 0){
        //Top Right to Bottom Left
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
    for(int i = 0; i < cubePieces.size(); i++){
        vec3 centers = cubePieces.at(i)->getCenter();
        cubePieces.at(i)->setModelMatrix(Scale(2,2,2) * Translate(centers[0], centers[1], centers[2]) * RotateY(10) * Translate(-centers[0], -centers[1], -centers[2]));
    }
}

void sphereWins(){
    //Run if 3 Spheres in a row
    for(int i = 0; i < spherePieces.size(); i++){
        vec3 centers = spherePieces.at(i)->getCenter();
        spherePieces.at(i)->setModelMatrix(Translate(centers[0], centers[1], centers[2]) * RotateY(10) * Translate(-centers[0], -centers[1], -centers[2]));
    }
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
    theta=theta+0.1;
    
    if(theta==360.1){
        theta = 0;
    }
    float rad = theta*2.0*3.14/360;
    light_position0 = vec4(0,20*sin(rad), 20*cos(rad),0.0 );
    lights[0] = Light(light_position0,light_ambient0, light_diffuse0, light_specular0);
    
    glutTimerFunc(interval, timerCallback, value);
    glutPostRedisplay();
}


void close(){
    for (unsigned int i = 0; i < drawables.size(); i++)
        delete(drawables[i]);
    
    if(windowID>0)
        glutDestroyWindow(windowID);
    
    exit(EXIT_SUCCESS);
}

