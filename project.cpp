//Jack Hill and Bailey Blum
//CSE20311.01
//Final Project
//12 December 2017
//project.cpp

#include <iostream>
#include "gfx.h"
#include <unistd.h>
#include <cstdlib>
#include <cmath>
#include <vector>

#define targetRadius 20
#define shotRadius 4
#define PI 3.1415926

using namespace std;

void drawTriangle(int xcoord, int ycoord, double theta);
void drawShot(int x, int y);
bool impact(int circleX, int circleY, int x, int y);

int main() {
    const int wd = 1000, ht = 650;
    char c;
    int triangleX = wd/2;
    int triangleY = ht/2 + 15;
	vector<int> circleX, circleY;
    int deltaTriangle = 5;
    vector<int> deltaCircleX, deltaCircleY;
    int deltaShot = 4;
    int deltat = 10000;
    vector<int> shotX, shotY;
    vector<bool> shot;
    vector<bool> circleThere = {1,1,1};
    bool triangleThere = true;
    double theta = PI/2;
    vector<double> shotTheta;
    int timer = 0;
	bool playing = true;

    //Open a new window for drawing.
    gfx_open(wd, ht, "Asteroids");
    gfx_color(255,255,255);
    gfx_text(25,225,"Use the T,G,F, and H keys to move/turn the triangle. Press spacebar to shoot.");
    gfx_text(175,235,"Destroy the white circles.");
    
    //gfx_color(14, 247, 72);
    drawTriangle(triangleX, triangleY, theta);
    //gfx_color(255, 255, 255);
    for (int i = 0; i < circleX.size(); i++)
		gfx_circle(circleX[i], circleY[i], targetRadius);

	//Add ball locations in X and Y directions, and speeds
	deltaCircleX.push_back(rand() % 10 - rand() % 10);
	deltaCircleY.push_back(rand() % 10 -  rand() % 10);
	circleX.push_back(75);
	circleY.push_back(125); //Ball 1
	
	deltaCircleX.push_back(rand() % 10 - rand() % 10);
	deltaCircleY.push_back(rand() % 10 -  rand() % 10);
	circleX.push_back(800);
	circleY.push_back(550); //Ball 2

	deltaCircleX.push_back(rand() % 10 - rand() % 10);
	deltaCircleY.push_back(rand() % 10 -  rand() % 10);
	circleX.push_back(75);
	circleY.push_back(550); //Ball 3

    
    c = gfx_wait();
    gfx_flush();

    while (playing) {
        //Set up window with circles and triangle
        gfx_clear();
        c = '1';
		
		for (int i = 0; i < circleX.size(); i++) {
			
       		if(circleThere[i] && impact(circleX[i], circleY[i], triangleX, triangleY)) triangleThere = false;
        	if(triangleThere){
            	//gfx_color(14, 247, 72);
            	drawTriangle(triangleX, triangleY, theta);
            	gfx_flush();
       		}
		}
        
        // Checks if there is still a shot
        for(int i = 0; i < shotX.size(); i++){
			for (int j = 0; j < circleX.size(); j++)
            	if(shotY[i] <= 0 || (circleThere[j] && impact(circleX[j], circleY[j], shotX[i], shotY[i]))) shot[i] = false;
        }
        // Updates the position of a shot
        for(int i = 0; i < shotX.size(); i++){
            if(shot[i]){
                shotY[i] -= deltaShot*sin(shotTheta[i]);
                shotX[i] += deltaShot*cos(shotTheta[i]);
                drawShot(shotX[i], shotY[i]);
                gfx_flush();
            }
        }

        // Check if there was an impact
        for(int i = 0; i < shotX.size(); i++){
			for (int j = 0; j < circleX.size(); j++) 
            	if (impact(circleX[j], circleY[j], shotX[i], shotY[i])){
                	circleThere[j] = false;
            	}
       		 }	
        
        //Reverses the change in the circle's position if it is at an edge in
        //the x direction
		for (int i = 0; i < deltaCircleX.size(); i++)
       		 if((deltaCircleX[i] > 0 && (circleX[i] + targetRadius + deltaCircleX[i] > wd)) || (deltaCircleX[i] < 0 && (circleX[i] - targetRadius - deltaCircleX[i] < 0))){
            	deltaCircleX[i] *= -1;
             }

        //Reverses the change in the circle's position if it is at an edge in
        //the y direction
		for (int i = 0; i < deltaCircleY.size(); i++)
        	if((deltaCircleY[i] < 0 && (circleY[i] - targetRadius - deltaCircleY[i] < 0)) || (deltaCircleY[i] > 0 && (circleY[i] + targetRadius + deltaCircleY[i] > ht))){
            	deltaCircleY[i] *= -1;
            }

        // Plots the circle if it is there
		for (int i = 0; i < circleThere.size(); i++)
        	if(circleThere[i]){
            	//gfx_color(255, 255, 255);
            	circleX[i] += deltaCircleX[i];
            	circleY[i] += deltaCircleY[i];
            	gfx_circle(circleX[i], circleY[i], targetRadius);
            	gfx_flush();
        	}

        // Updates c if there was a button pressed
        if(gfx_event_waiting()){
            c = gfx_wait();
            gfx_flush();
        }

        switch (c) {
            case 't':
                triangleY -= deltaTriangle*sin(theta);
                triangleX += deltaTriangle*cos(theta);
                break;
            case 'g':
                triangleY += deltaTriangle*sin(theta);
                triangleX -= deltaTriangle*cos(theta);
                break;
            case 'f':
                theta += PI/12;
                break;
            case 'h':
                theta -= PI/12;
                break;
            case 32:
                if(timer > 20){
                    shotX.push_back(triangleX + 2*12*cos(theta));
                    shotY.push_back(triangleY-12*sin(theta));
                    shot.push_back(true);
                    shotTheta.push_back(theta);
                    timer = 0;
                }
                break;
            case 'q':
                return 0;
        }
        usleep(deltat);
        timer++;
		
		if ((!circleThere[0] && !circleThere[1] && !circleThere[2]) || !triangleThere) {
			
			playing = false;
				
			deltaCircleX[0] = (rand() % 10 - rand() % 10);
			deltaCircleY[0] = (rand() % 10 -  rand() % 10);
			circleX[0] = (75);
			circleY[0] = (125); //Ball 1
			
			deltaCircleX[1] = (rand() % 10 - rand() % 10);
			deltaCircleY[1] = (rand() % 10 -  rand() % 10);
			circleX[1] = (800);
			circleY[1] = (550); //Ball 2

			deltaCircleX[2] = (rand() % 10 - rand() % 10);
			deltaCircleY[2] = (rand() % 10 -  rand() % 10);
			circleX[2] = (75);
			circleY[2] = (550); //Ball 3	

			circleThere = {true,true,true};
			triangleThere = true;
			
			shotX.clear();
			shotY.clear();
			shot.clear();
            shotTheta.clear();
		
			drawTriangle(triangleX,triangleY,theta);

			gfx_text(25,225,"Game Over! Press the spacebar to restart the game, or 'q' to quit.");
			gfx_flush();
            
            do{
			    c = gfx_wait();
			    if (c == 32)
				    playing = true;
			    if (c == 'q')
				    playing = false;
                }
            while(c != 32 && c != 'q');
		}
			 
    }
    return 0;
}

// Draws the traingle
void drawTriangle(int xcoord,int ycoord, double theta){
    int radius = 12;
    gfx_line(xcoord + 2*radius*cos(theta), ycoord - 2*radius*sin(theta), xcoord + radius*cos(theta + 2*PI/3), ycoord - radius*sin(theta + 2*PI/3));
    gfx_line(xcoord + radius*cos(theta + 2*PI/3), ycoord - radius*sin(theta + 2*PI/3), xcoord + radius*cos(theta + 4*PI/3), ycoord - radius*sin(theta + 4*PI/3));
    gfx_line(xcoord + radius*cos(theta + 4*PI/3), ycoord - radius*sin(theta + 4*PI/3), xcoord + 2*radius*cos(theta), ycoord - 2*radius*sin(theta));
}

// Draws the shot
void drawShot(int x, int y){
    //gfx_color(242,29, 21);
    gfx_circle(x, y, shotRadius);
}

// Checks to see if there was an impact
bool impact(int circleX, int circleY, int x, int y){
    if(x <= circleX + targetRadius && x >= circleX - targetRadius && y <= circleY + targetRadius && y >= circleY - targetRadius)
        return true;
    return false;
}
    


