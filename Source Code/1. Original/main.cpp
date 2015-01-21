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
// #include <GL/glut.h> //GLUT library - Used in URI Clusters FOR LINUX


#define X_WINDOW  1000
#define Y_WINDOW  1000

unsigned char image[X_WINDOW][Y_WINDOW][3];
unsigned char cells[X_WINDOW][Y_WINDOW]; // 1 or 0
unsigned char new_cells[X_WINDOW][Y_WINDOW]; // 1 or 0
bool paused = true;


void process(){
    
    paused = (paused)?false:true;
    
//    while (!paused) {
        
        int counter = 0;
        
        // Calculate cells' new state
        for (int x = 0; x < X_WINDOW; x++){
            for (int y = 0; y < Y_WINDOW; y++){

                    counter += (cells[x-1][y+1] == 1) ? 1 : 0; // tl
                    counter += (cells[x+0][y+1] == 1) ? 1 : 0; // t
                    counter += (cells[x+1][y+1] == 1) ? 1 : 0; // tr
                    counter += (cells[x-1][y+0] == 1) ? 1 : 0; // l
                    counter += (cells[x+1][y+0] == 1) ? 1 : 0; // r
                    counter += (cells[x-1][y-1] == 1) ? 1 : 0; // bl
                    counter += (cells[x+0][y-1] == 1) ? 1 : 0; // b
                    counter += (cells[x+1][y-1] == 1) ? 1 : 0; // br

                // If counter equals 3
                if(counter == 3){
                    new_cells[x][y] = 1;
                } else if(counter == 2){
                    new_cells[x][y] = cells[x][y]; // if counter equals 2 then the same state remains onto next generation.
                } else {
                    new_cells[x][y] = 0; // otherwise set to dead.
                }
            
                counter = 0; // reset counter
            }
        }
    
        // copy array
        for (int x = 0; x < X_WINDOW; x++){
            for (int y = 0; y < Y_WINDOW; y++){
                cells[x][y] = new_cells[x][y];
            }
        }
        
        for (int x = 0; x < X_WINDOW; x++){
            for (int y = 0; y < Y_WINDOW; y++){
                if(cells[x][y] == true){ // if is living cell
                    image[x][y][0] = image[x][y][1] = image[x][y][2] = 0; // Black
                } else { // otherwise
                    image[x][y][0] = image[x][y][1] = image[x][y][2] = 255; // White
                }
            }
        }
        
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

/*
 Press any key to run one iteration.
 */
void keypressed(unsigned char key, int x, int y){

    struct timeval calculationStart, calculationEnd;
    gettimeofday(&calculationStart, NULL);
            process();
    gettimeofday(&calculationEnd, NULL);
    printf("Single-threaded Game Of Life (Calculation): %.2f sec\n", timeDifference(&calculationStart, &calculationEnd)/1000000);
    
};

void start(){
    
    /* initialize random seed: */
    srand(time(NULL)); // important call for rand();

    for (int x = 0; x < X_WINDOW; x++){
        for (int y = 0; y < Y_WINDOW; y++){
    
            int random_int = rand() % 2; // generate secret number between 0 and 1
            
            if(random_int == 1){
                cells[x][y] = 1;
                image[x][y][0] = image[x][y][1] = image[x][y][2] = 0; // Black
            } else {
                cells[x][y] = 0;
                image[x][y][0] = image[x][y][1] = image[x][y][2] = 255; // White
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
    glutMainLoop();           // Enter the infinitely event-processing loop
    
    return 0;
}

