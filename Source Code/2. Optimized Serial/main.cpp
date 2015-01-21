// Linux Compile: gcc -o <compiled_code_name> <code_name>.cpp -lGL -lglut -lGLU -lpthread -lpthread -lstdc++
//
//  main.cpp
//  GameOfLife
//
//  Created by Daniel Inoa Llenas on 4/19/14.
//  Copyright (c) 2014 Daniel Inoa Llenas. All rights reserved.
//

#include <iostream>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <sys/time.h> //timing library
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h> // Used in Xcode FOR MAC
//#include <GL/glut.h> //GLUT library - Used in URI Clusters FOR LINUX

#define X_WINDOW 1000
#define Y_WINDOW 1000

#define BLACK 0
#define WHITE 255

unsigned char image[X_WINDOW][Y_WINDOW][3];
unsigned char cells[X_WINDOW][Y_WINDOW];
bool paused = true;

void process(){

    paused = !paused;
    
//    while (!paused) {
    
        /* FIRST LOOP */
        /*
         - Loop through the grid.
         - Look for for living cells.
         - Add one to neighboring cells' counter.
         */
        
        for (int x = 0; x < X_WINDOW; x++){
            for (int y = 0; y < Y_WINDOW; y++){
                if(image[x][y][0] == BLACK){
                    ++cells[x-1][y+1]; // add one to top left neighbor
                    ++cells[x+0][y+1]; // add one to top neighbor
                    ++cells[x+1][y+1]; // add one to top right neighbor
                    ++cells[x-1][y+0]; // add one to left neighbor
                    ++cells[x+1][y+0]; // add one to right neighbor
                    ++cells[x-1][y-1]; // add one to bottom left neighbor
                    ++cells[x+0][y-1]; // add one to bottom neighbor
                    ++cells[x+1][y-1]; // add one to bottom right neighbor
                }
            }
        }
    
        /* SECOND LOOP */
        /*
         - Loop through the grid.
         - Set state of cells for the next generation based on their neighbor-count.
         */
        for (int x = 0; x < X_WINDOW; x++){
            for (int y = 0; y < Y_WINDOW; y++){
                
                // If counter equals 3
                if(cells[x][y] == 3){
                    image[x][y][0] = image[x][y][1] = image[x][y][2] = BLACK;
                } else if(cells[x][y] == 2){
                    // remains the unchanged.
                } else {
                    image[x][y][0] = image[x][y][1] = image[x][y][2] = WHITE;
                }
            }
        }
    
        memset(cells, 0, sizeof(cells)); // reset/clean array
    
        glDrawPixels(X_WINDOW, Y_WINDOW, GL_RGB, GL_UNSIGNED_BYTE, image);
        glFlush();
    
//    }
    
}

/*
 / Method to calculate the time differences in program execution
 */
double timeDifference (struct timeval * start, struct timeval * end){
    struct timeval diff;
    
    if (start->tv_sec == end->tv_sec) {
        diff.tv_sec = 0;
        diff.tv_usec = end->tv_usec - start->tv_usec;
    } else {
        diff.tv_usec = 1000000 - start->tv_usec;
        diff.tv_sec = end->tv_sec - (start->tv_sec + 1);
        diff.tv_usec += end->tv_usec;
        
        if (diff.tv_usec >= 1000000) {
            diff.tv_usec -= 1000000;
            diff.tv_sec += 1;
        }
    }
    
    return ((double)diff.tv_sec + (diff.tv_usec / 1.0e6)) * 1.0e6;
}

void keypressed(unsigned char key, int x, int y){
    struct timeval calculationStart, calculationEnd;
    gettimeofday(&calculationStart, NULL);
    process();
    gettimeofday(&calculationEnd, NULL);
    printf("Optimized Single-threaded Game Of Life (Calculation): %.2f sec\n", timeDifference(&calculationStart, &calculationEnd)/1000000);
};

void start(){
    /* initialize random seed: */
    srand(time(NULL)); // important call for rand();
    
    for (int x = 0; x < X_WINDOW; x++){
        for (int y = 0; y < Y_WINDOW; y++){
            
            int random_int = rand() % 2; // generate random number between 0 and 1
            
            if(random_int == 1){
                image[x][y][0] = image[x][y][1] = image[x][y][2] = BLACK;
            } else {
                image[x][y][0] = image[x][y][1] = image[x][y][2] = WHITE;
            }
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels(X_WINDOW, Y_WINDOW, GL_RGB, GL_UNSIGNED_BYTE, image);
    glFlush();
}

int main(int argc, char *argv[]){

    glutInit(&argc, argv);                 // Initialize GLUT
    glutInitWindowSize(X_WINDOW, Y_WINDOW);   // Set the window's initial width & height
    glutInitWindowPosition(400, 50); // Position the window's initial top-left corner
    glutCreateWindow("Game Of Life with OpenGL"); // Create a window with the given title
    
    start();
    
    glutDisplayFunc(display); // Register display callback handler for window re-paint
    glutKeyboardFunc(keypressed);
    glutMainLoop();           // Enter the indefinite event-processing loop
    
    
    return 0;
}

