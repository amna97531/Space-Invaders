//  Includes
#include <stdio.h>
#include <math.h>
#include <random>
#include <glut.h>
#include <vector>
#include <iostream>
using namespace std;




//-----------------

//  Methods Signatures
void drawRect(int x, int y, int w, int h);
void KeyReleased(unsigned char key, int x, int y);
void KeyPressed(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Timer(int value);
void EnemyTimer(int value);
bool collisions(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
void drawlifeBar(int widthlives);
void resetGame();
void drawHeart(int x, int y);
bool powerUp(int xrandom, int yrandom, int wL, int x2, int y2, int w2, int h2);
void drawspaceship(int red, int green, int blue);
void drawPowerUp(int xrandom, int yrandom);
void powerUpAquiredTimer(int value);
void powerUpTimer(int value);
void powerUpTimeUp(int value);
void disappear(int value);
void Display();
//-----------------

//  Global Variables
int selectedBar = 0; // used to determine which bar has the mouse currently over it
int xrandom = 0;
int yrandom = 0;
int wL = 50;//length and width of power up
int heightBullet = 10;
int widthBullet = 8;
int heightSpaceShip = 80;
int widthSpaceShip = 120;
int bLSpaceShipX = 340;
int bLSpaceShipY = 380;
int heightEnemy = 200;
int widthEnemy = 120;
int bLEnemyX = 340;//Bottom left point of bounding box X coordinate
int bLEnemyY = 560;//Bottom left point of bounding box Y coordinate
int translationSpaceshipX = 0;
int translationSpaceshipY = 0;
int originPositionSpaceshipX = 400; //x value of origin of spaceship
int originPositionSpaceshipY = 402.5;//Y value of origin of spaceship
int originPositionEnemyX = 400;
int originPositionEnemyY = 640;
int translationEnemyX = 0;
int widthlives = 500;
int deductionEnemyLives = 50;
bool powerUpAcquired= false;
bool a, w, s, d = false;
bool reachedBorder = false;
bool gameWon = false;
bool resetGame2 = false;
int playerLives = 3;
bool touching = false;
int red = 0;
int green = 0;
int blue = 1;
bool timeUp = false;
bool istherePowerUp = false;
class Point {
public:int x; int y;
      int getx() {
          return x;
      };

};
vector<Point> bullets;
vector<Point> enemyBullets;


//-----------------

void main(int argc, char** argr) {
    glutInit(&argc, argr);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(150, 150);
    collisions(0,0,10,10,5,5,2,2);
    glutCreateWindow("chicken invadors bas morhaka shwaya");
    glutDisplayFunc(Display);
    glutMouseFunc(Mouse);       // sets the Mouse handler function; called when a mouse button is clicked
   // glutTimerFunc(0, Timer, 0); // sets the Timer handler function; which runs every `Threshold` milliseconds (1st argument)
    EnemyTimer(0);
    Timer(0);
    powerUpTimer(0);
    powerUpTimeUp(0);
    glutKeyboardFunc(KeyPressed);      // sets the Keyboard handler function; called when a key is pressed
    glutKeyboardUpFunc(KeyReleased);  // sets the KeyboardUp handler function; called when a key is released
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glClearColor(1, 1, 1, 0);
    gluOrtho2D(0, 800, 0, 800);

    glutMainLoop();
}

void drawRect(int x, int y, int w, int h) {
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}


// Mouse handler function
//   button: the mouse button clicked: left, right or middle
//   state:  the state of the button: clicked (down) or released (up)
//   x  : the X coordinate of the mouse cursor at the moment of clicking the mouse button
//   y  : the Y coordinate of the mouse cursor at the moment of clicking the mouse button
void Mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        Point bulletPoint;
        bulletPoint.x = translationSpaceshipX + originPositionSpaceshipX;
        bulletPoint.y = translationSpaceshipY + originPositionSpaceshipY;
        bullets.push_back(bulletPoint);
    }

    if (gameWon && resetGame2) {
        if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
            gameWon = false;
            resetGame2 = false;
            glClearColor(1, 1, 1, 0);
            resetGame();
        }
    }
    glutPostRedisplay();
}
void disactivatePowerUp(int value) {
    powerUpAcquired = false;
    blue = 1;
    red = 0;
    green = 0;
    drawspaceship(red, green, blue);

}
void Timer(int value) {
    // set the ball's Y coordinate to a random number between 10 and 780 (since the window's height is 800)

    if (!gameWon) {
        if (translationEnemyX > -335 && reachedBorder == false) translationEnemyX -= 5;
        if (translationEnemyX < -330) reachedBorder = true;
        if (reachedBorder == true) translationEnemyX += 5;
        if (translationEnemyX > 330) reachedBorder = false;
        for (auto& it : bullets) {
            it.y += 5;
        }
        for (auto& it : enemyBullets) {
            it.y -= 5;
        }
        int x1 = originPositionSpaceshipX + translationSpaceshipX - (widthSpaceShip / 2);
        int y1 = originPositionSpaceshipY + translationSpaceshipY - (heightSpaceShip / 2);
        int x2 = originPositionEnemyX + translationEnemyX - (widthEnemy / 2);
        int y2 = originPositionEnemyY - (heightEnemy / 2);

        bool x = collisions(x1, y1, widthSpaceShip, heightSpaceShip, x2, y2, widthEnemy, heightEnemy);
        bool power = powerUp(xrandom, yrandom,  wL,  x1,  y1, widthSpaceShip, heightSpaceShip);
        if (power == true) {
            powerUpAcquired = true;
            istherePowerUp = false;
            blue = 0;
            red = 1;
            green = 1;
            glutTimerFunc(5000, disactivatePowerUp, 0);
        }

        int i = 0;
        for (auto& it : bullets) {
            if (collisions(it.x, it.y, widthBullet, heightBullet, x2, y2, widthEnemy, heightEnemy) == true ) {
                widthlives -= deductionEnemyLives;
                bullets.erase(bullets.begin() + i);
            }
            else {
                i++;
            }
            /* printf("%d", widthlives);

             printf("%s", "\n------------\n");*/

            if (widthlives <= 0) {
                widthlives = 0;
                printf("%s", "Game Over!");
                gameWon = true;
                glClearColor(0, 1, 0, 0);
            }
        }
        i = 0;
        for (auto& it : enemyBullets) {
            if (collisions(it.x, it.y, widthBullet, heightBullet, x1, y1, widthSpaceShip, heightSpaceShip) == true && powerUpAcquired == false) {
                playerLives -= 1;
                enemyBullets.erase(enemyBullets.begin() + i);
            }
            else {
                i++;
            }
            if (playerLives <= 0) {
                printf("%s", "Game Over!");
                gameWon = true;
                glClearColor(1, 0, 0, 0);
            }
        }


        // ask OpenGL to recall the display function to reflect the changes on the window
        glutPostRedisplay();

        // recall the Timer function after 20 seconds (20,000 milliseconds)
        glutTimerFunc(500, Timer, 0);
    }
}
void drawCircle(int x, int y, float r){
    glPushMatrix();
    glTranslatef(x, y, 0);
    GLUquadric* quadObj = gluNewQuadric();
    gluDisk(quadObj, 0, r, 50, 50);
    glPopMatrix();
}
void drawHeart(int x, int y) {
    glColor3f(1, 0, 0);
    drawCircle(x - 6, y, 7);
    drawCircle(x + 6, y,7);
    glColor3f(1, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y - 14);
    glVertex2f(x - 12.5, y - 4);
    glVertex2f(x + 12.5, y - 4);
    glEnd();


}
void resetGame() {
    selectedBar = 0; // used to determine which bar has the mouse currently over it
    xrandom = rand() % 700;
    yrandom = rand() % 700;
    wL = 50;//length and width of power up
    heightBullet = 10;
    widthBullet = 8;
    heightSpaceShip = 80;
    widthSpaceShip = 120;
    bLSpaceShipX = 340;
    bLSpaceShipY = 380;
    heightEnemy = 200;
    widthEnemy = 120;
    bLEnemyX = 340;//Bottom left point of bounding box X coordinate
    bLEnemyY = 560;//Bottom left point of bounding box Y coordinate
    translationSpaceshipX = 0;
    translationSpaceshipY = 0;
    originPositionSpaceshipX = 400; //x value of origin of spaceship
    originPositionSpaceshipY = 402.5;//Y value of origin of spaceship
    originPositionEnemyX = 400;
    originPositionEnemyY = 640;
    translationEnemyX = 0;
    widthlives = 500;
    deductionEnemyLives = 50;
    powerUpAcquired = false;
    a, w, s, d = false;
    reachedBorder = false;
    gameWon = false;
    resetGame2 = false;
    playerLives = 3;
    touching = false;
    red = 0;
    green = 0;
    blue = 1;
    timeUp = false;
    istherePowerUp = false;
     bullets.clear();
     enemyBullets.clear();
}
void EnemyTimer(int value)
{
    printf("%s",gameWon?"true\n ---------------":"false \n ---------------");
    if (!gameWon) {
        Point bulletPoint;
        bulletPoint.x = translationEnemyX + originPositionEnemyX - 8;
        bulletPoint.y = 600;
        enemyBullets.push_back(bulletPoint);
        glutTimerFunc(1000, EnemyTimer, 0);
    }

}
void powerUpAquiredTimer(int value) {
    if (touching == true && istherePowerUp== true) {
        powerUpAcquired = false;
        istherePowerUp = false;
    }
    glutTimerFunc(1000, powerUpAquiredTimer, 0);

}
void powerUpTimer(int value) {
    if (!istherePowerUp) {
        istherePowerUp = true;
        xrandom = rand() % 450 + 150;
        yrandom = rand() % 450 + 150;
        glutTimerFunc(5000, disappear, 0);
    }
    glutTimerFunc(15000, powerUpTimer, 0);

}
void powerUpTimeUp(int value) {
    if (powerUpAcquired == false && istherePowerUp== true) {
    timeUp = true;
}
    glutTimerFunc(100000, powerUpTimeUp, 0);

}


void disappear(int value)
{
    istherePowerUp = false;
}
void drawPartialCircle(int x, int y, int sangle, int eangle ) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    GLUquadric* quadObj = gluNewQuadric();
    gluPartialDisk(quadObj, 0, 25, 25, 50, sangle, eangle);
    glPopMatrix();
}

void drawspaceship(int red, int green , int blue) {
    glColor3f(red, green, blue);//blue
    glBegin(GL_POLYGON);
    glVertex3f(350.0f, 390.0f, 0.0f);
    glVertex3f(350.0f, 415.0f, 0.0f);
    glVertex3f(450.0f, 415.0f, 0.0f);
    glVertex3f(450.0f, 390.0f, 0.0f);
    glEnd();
    drawPartialCircle(400,400,-90,180);

    glColor3f(red, green, blue);//blue
    glBegin(GL_QUADS);
    glVertex3f(395.0f, 400.0f, 0.0f);
    glVertex3f(395.0f, 450.0f, 0.0f);
    glVertex3f(405.0f, 450.0f, 0.0f);
    glVertex3f(405.0f, 400.0f, 0.0f);
    glEnd();
    glColor3f(red, green, blue);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawRect(350, 390, 100, 60);//Hit box spaceship
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


}
void drawBullet(int x, int y) {
  
    glColor3f(0.0f, 1.0f, 0.0f);//green
    glBegin(GL_TRIANGLES);
    glVertex3f(x, y+10, 0.0f);
    glVertex3f(x + 8, y + 10, 0.0f);
    glVertex3f(x + 4,  y + 20, 0.0f);
    glEnd();
    glColor3f(0.0f, 1.0f, 0.0f);
    drawRect(x,y, 8, 10);
    glColor3f(0.0f, 1.0f, .0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawRect(x, y, 8,10 );
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void drawEnemyBullet(int x, int y) {

    glColor3f(0.0f, 1.0f, 0.0f);//green
    glBegin(GL_TRIANGLES);
    glVertex3f(x, y, 0.0f);
    glVertex3f(x + 8, y, 0.0f);
    glVertex3f(x + 4, y - 10, 0.0f);
    glEnd();
    glColor3f(0.0f, 1.0f, 0.0f);
    drawRect(x, y, 8, 10);
    glColor3f(0.0f, 1.0f, 0.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawRect(x, y, 8, 10);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void drawEnemy() {
    glColor3f(1.0f, 0.0f, 0.0f);//red
    glBegin(GL_QUADS);
    glVertex3f(400.0f, 750.0f, 0.0f);
    glVertex3f(350.0f, 630.0f, 0.0f);
    glVertex3f(450.0f, 630.0f, 0.0f);
    glVertex3f(400.0f, 750.0f, 0.0f);
    glEnd();
    drawPartialCircle(400, 635, -90, 360);
    //glBegin(GL_POINTS);s

    glColor3f(1.0f, 0.0f, 0.0f);//red
    glBegin(GL_POLYGON);
    glVertex3f(395.0f, 650.0f, 0.0f);
    glVertex3f(395.0f, 600.0f, 0.0f);
    glVertex3f(405.0f, 600.0f, 0.0f);
    glVertex3f(405.0f, 650.0f, 0.0f);
    glEnd();
    glColor3f(1.0f, 0.0f, 0.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawRect(350, 600, 100, 150);//Hit box enemy
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void drawPowerUp(int xrandom, int yrandom) {
    glColor3f(1.0f, 1.0f, 0.0f);//yellow
    glBegin(GL_POLYGON);
    glVertex3f(xrandom, yrandom, 0.0f);
    glVertex3f(xrandom + wL, yrandom, 0.0f);
    glVertex3f(xrandom + wL, yrandom + wL, 0.0f);
    glVertex3f(xrandom, yrandom + wL, 0.0f);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawRect(xrandom, yrandom, wL, wL);//Hit box enemy
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

bool powerUp(int xrandom, int yrandom, int wL, int x2, int y2, int w2, int h2) {
    int tRpowerUpX = xrandom + wL;
    int tRpowerUpY = yrandom + wL;
    int bRspaceshipX = x2 + w2;
    int tRspaceshipY = y2 + h2;

    if (xrandom < x2 && x2 < tRpowerUpX && yrandom < tRspaceshipY && tRspaceshipY < tRpowerUpY) {
        powerUpAcquired = true;
        touching = true;
    }
    if (xrandom < bRspaceshipX && bRspaceshipX < tRpowerUpX && yrandom < tRspaceshipY && tRspaceshipY < tRpowerUpY) {
        powerUpAcquired = true;
        touching = true;
    }
    if (xrandom < x2 && x2 < tRpowerUpX && yrandom < y2 && y2 < tRpowerUpY) {
        powerUpAcquired = true;
        touching = true;
    }
    if (xrandom < bRspaceshipX && bRspaceshipX < tRpowerUpX && yrandom < y2 && y2 < tRpowerUpY) {
        powerUpAcquired = true;
        touching = true;
    }
    if (tRpowerUpX < bRspaceshipX && tRpowerUpX > x2 && yrandom < tRspaceshipY && yrandom > y2) {
        powerUpAcquired = true;
        touching = true;
    }
    if (xrandom < bRspaceshipX && xrandom > x2 && yrandom < tRspaceshipY && yrandom > y2) {
        powerUpAcquired = true;
        touching = true;
    }
    if (tRpowerUpX > x2 && tRpowerUpX < bRspaceshipX && tRpowerUpY  < tRspaceshipY && tRpowerUpY > y2) {
        powerUpAcquired = true;
        touching = true;
    }
    if (xrandom > x2 && xrandom < bRspaceshipX && tRpowerUpY > y2 && tRpowerUpY < tRspaceshipY) {
        powerUpAcquired = true;
        touching = true;
    }
    return powerUpAcquired;


}
bool collisions(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    bool collided = false;
    int bRenemyX = x1+w1;
    int tRenemyY = y1 + h1;
    int bRspaceshipX = x2 + w2;
    int tRspaceshipY = y2 + h2;

    if (x1 < x2 && x2 < bRenemyX && y1 < tRspaceshipY && tRspaceshipY < tRenemyY) collided = true;//collision with corner 1 for spaceship

    if (x1 < bRspaceshipX && bRspaceshipX < bRenemyX && y1 < tRspaceshipY && tRspaceshipY < tRenemyY) collided = true;//collision with corner 2 for spaceship

    if (x1 < x2 && x2 < bRenemyX && y1 < y2 && y2 < tRenemyY) collided = true;//collision with corner 3 for spaceship

    if (x1 < bRspaceshipX && bRspaceshipX  < bRenemyX && y1 < y2 && y2 < tRenemyY) collided = true;//collision with corner 4 for spaceship
    //printf("%s", collided ? "true" : "false");
    if (bRenemyX < bRspaceshipX && bRenemyX > x2 && y1 < tRspaceshipY && y1 > y2) collided = true;//collision with corner 1 for enemy 
    //printf("%s", collided ? "true" : "false");
    if (x1 < bRspaceshipX && x1 > x2 && y1 < tRspaceshipY && y1 > y2) collided = true;//collision with corner 2 for enemy 
    //printf("%s", collided ? "true" : "false");
    if (bRenemyX > x2 && bRenemyX < bRspaceshipX && tRenemyY  < tRspaceshipY && tRenemyY > y2) collided = true;//collision with corner 3 for enemy 
    //printf("%s", collided ? "true" : "false");
    if (x1 > x2 && x1 < bRspaceshipX && tRenemyY > y2 && tRenemyY < tRspaceshipY) collided = true;//collision with corner 4 for enemy 
    //printf("%s", collided ? "true" : "false");


    return collided;

}
void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    

    if(!resetGame2){
        if (istherePowerUp)
        {
            drawPowerUp(xrandom, yrandom);
        }
        if (istherePowerUp == false && timeUp == false) {
           
            

        }
        int xShift = 0;
        for (int i = 0; i < playerLives; i++) {
            drawHeart(120 - xShift, 765);
            xShift += 30;


        }
    glPushMatrix();
    glTranslatef(translationSpaceshipX, translationSpaceshipY, 0);
    drawspaceship(red, green, blue);
    glPopMatrix();
    for (auto& it : bullets) {

        drawBullet(it.x, it.y);
    }

    for (auto& it : enemyBullets) {
        glPushMatrix();

        drawEnemyBullet(it.x, it.y);
        glPopMatrix();
    }

    glPushMatrix();
    glTranslatef(translationEnemyX, 0, 0);
    drawEnemy();
    glPopMatrix();


    drawlifeBar(widthlives);
}   
    if (gameWon)
    {
        printf("WOOOOON");
        resetGame();
    }
    if (gameWon)
        resetGame2 = true;
   
    glFlush();
}
void drawlifeBar(int widthlives) {

    glColor3f(0.0f, 0.0f, 0.0f);//Black border of life Bar
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawRect(150, 750, 499, 20);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(0.0f, 1.0f, 0.0f);
    drawRect(149, 750, widthlives, 20);


}
void KeyPressed(unsigned char key, int x, int y)
{
    if (key == 'a') a = true;
    if (key == 'd') d = true;
    if (key == 'w') w = true;
    if (key == 's') s = true;

    if (a == true && translationSpaceshipX >= -329) translationSpaceshipX -= 10;
    if (d == true && translationSpaceshipX <= glutGet(GLUT_WINDOW_WIDTH) - 500) translationSpaceshipX += 10;
    if (w == true && translationSpaceshipY <= glutGet(GLUT_WINDOW_HEIGHT) - 390) translationSpaceshipY += 10;
    if (s == true && translationSpaceshipY >= -360) translationSpaceshipY -= 10;
    glutPostRedisplay();
}
void KeyReleased(unsigned char key, int x, int y)
{
    if (key == 'a') a = false;
    if (key == 'd') d = false;
    if (key == 'w') w = false;
    if (key == 's') s = false;
    glutPostRedisplay();
}
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-2, 2, -2, 2);

    /* Uncomment to show x,y axis and intersection points
    // Draw X, Y axis
    glColor3ub(0, 0, 255);  // Color blue
    glBegin(GL_LINES);
        glVertex2f(-2, 0);
        glVertex2f(2, 0);
        glVertex2f(0, -2);
        glVertex2f(0, 2);
    glEnd();
    
    // Draw intersection points
    glPointSize(3);
    glColor3ub(0, 255, 0);
    glBegin(GL_POINTS);
        glVertex2f(-1, 0);
        glVertex2f(1, 0);
        glVertex2f(0, -1);
        glVertex2f(0, 1);
    glEnd();*/

    /*
    Graph function:
    (x^2 + y^2 - 1)^3 - x^2*y^3 = 0
    => y^2 - y*cbrt(x^2) + x^2 - 1 = 0 
    */
    glPointSize(1);
    glColor3ub(255, 0, 0);  // Color Red
    glBegin(GL_POINTS);
        for (float x = -1.139; x <= 1.139; x += 0.001) 
        {
            float delta = cbrt(x*x) * cbrt(x*x) - 4*x*x + 4;
            float y1 = (cbrt(x*x) + sqrt(delta)) / 2;
            float y2 = (cbrt(x*x) - sqrt(delta)) / 2;
            glVertex2f(x, y1);
            glVertex2f(x, y2);
        }
    glEnd();
    
    glFlush();
}