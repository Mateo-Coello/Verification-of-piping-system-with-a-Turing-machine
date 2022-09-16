//
// Created by mai on 9/16/22.
//
#include <math.h>

typedef struct next_element_post{
    float x;
    float y;
    float z;
    int n_start;
    int t_start;
} NEP;

NEP makeCap(float nodes[][3], int triangles[][3], int n_start, int t_start, float depth, float r, int n, int orientation){

    int numNode = n_start;
    int numTri = t_start;
    float d_theta = (theta_1-theta_0)/((float) n);

    if(orientation == 0) { // vertical cylinder
        //---------------------Make Circle-----------------------
        for(int i = 0; i < n; i++) {
            float angle=theta_0+i*d_theta;
            nodes[numNode][0]=x0 + r*cos(angle);
            nodes[numNode][1]=y0 + r*sin(angle);
            nodes[numNode++][2]=z0 + 0;
        }
        //-------------------------------------------------------

        //---------------------Extrude Circle--------------------
        for (int i = n_start; i < n_start+n; i++) {
            nodes[numNode][0] = nodes[i][0];
            nodes[numNode][1] = nodes[i][1];
            nodes[numNode++][2] = nodes[i][2] + depth;
        }
        //-------------------------------------------------------
    }
    else if(orientation == 1){ // horizontal cylinder
        //---------------------Make Circle-----------------------
        for(int i=0; i<n; i++){
            float angle=theta_0+i*d_theta;
            nodes[numNode][0]=x0 + 0;
            nodes[numNode][1]=y0 + r*cos(angle);
            nodes[numNode++][2]=z0 + r*sin(angle);
        }
        //-------------------------------------------------------

        //---------------------Extrude Circle--------------------
        for (int i = n_start; i < n_start+n; i++) {
            nodes[numNode][0] = nodes[i][0] - depth;
            nodes[numNode][1] = nodes[i][1];
            nodes[numNode++][2] = nodes[i][2];
        }
        //-------------------------------------------------------
    }

    // triangles for the sides of the cylinder
    for(int i=n_start; i<n_start+n-1; i++){
        triangles[numTri][0]=i;
        triangles[numTri][1]=i+1;
        triangles[numTri++][2]=i+n;
        triangles[numTri][0]=i+1;
        triangles[numTri][1]=i+n+1;
        triangles[numTri++][2]=i+n;
    }

    // close the cylinder from the lateral sides
    int limit = n_start+n;
    int t_limit = n_start+2*n;

    triangles[numTri][0]=limit-1;
    triangles[numTri][1]=0;
    triangles[numTri++][2]=2*limit-1;
    triangles[numTri][0]=0;
    triangles[numTri][1]=limit;
    triangles[numTri++][2]=2*limit-1;

    // center point of cap
    if(orientation == 0) {// vertical
        nodes[2*limit][0] = 0;
        nodes[2*limit][1] = 0;
        nodes[2*limit][2] = depth;
    }
    else if(orientation == 1){// horizontal
        nodes[2*limit][0] = -depth;
        nodes[2*limit][1] = 0;
        nodes[2*limit][2] = 0;
    }

    // cylinder top cap
    for(int i=n; i < 2*n; i++){
        if(i == 2*n-1){
            triangles[numTri][0]=2*limit-1;
            triangles[numTri][1]=limit;
            triangles[numTri++][2]=2*limit;
            continue;
        }
        triangles[numTri][0]=i;
        triangles[numTri][1]=i+1;
        triangles[numTri++][2]=2*limit;
    }

    NEP next_position = {x = nodes[2*limit][0], y = nodes[2*limit][1], z = nodes[2*limit][2], n_start=numNode, t_start=numTri};

    return next_position;
}

NEP makeCylinder(float nodes[][3], int triangles[][3], int start, float depth, float r, int n, int orientation){
    int ct = 0;

    if(orientation == 0) { // vertical cylinder
        //---------------------Make Circle-----------------------
        for(int i = 0; i < n; i++) {
            float angle=theta_0+i*d_theta;
            nodes[numNode][0]=x0 + r*cos(angle);
            nodes[numNode][1]=y0 + r*sin(angle);
            nodes[numNode++][2]=z0 + 0;
        }
        //-------------------------------------------------------

        //---------------------Extrude Circle--------------------
        for (int i = n_start; i < n_start+n; i++) {
            nodes[numNode][0] = nodes[i][0];
            nodes[numNode][1] = nodes[i][1];
            nodes[numNode++][2] = nodes[i][2] + depth;
        }
        //-------------------------------------------------------
    }
    else if(orientation == 1){ // horizontal cylinder
        //---------------------Make Circle-----------------------
        for(int i=0; i<n; i++){
            float angle=theta_0+i*d_theta;
            nodes[numNode][0]=x0 + 0;
            nodes[numNode][1]=y0 + r*cos(angle);
            nodes[numNode++][2]=z0 + r*sin(angle);
        }
        //-------------------------------------------------------

        //---------------------Extrude Circle--------------------
        for (int i = n_start; i < n_start+n; i++) {
            nodes[numNode][0] = nodes[i][0] - depth;
            nodes[numNode][1] = nodes[i][1];
            nodes[numNode++][2] = nodes[i][2];
        }
        //-------------------------------------------------------
    }

    // triangles for the sides of the cylinder
    for(int i=n_start; i<n_start+n-1; i++){
        triangles[numTri][0]=i;
        triangles[numTri][1]=i+1;
        triangles[numTri++][2]=i+n;
        triangles[numTri][0]=i+1;
        triangles[numTri][1]=i+n+1;
        triangles[numTri++][2]=i+n;
    }

    // close the cylinder from the lateral sides
    int limit = (n_start+n);

    // close the cylinder from the sides
    triangles[ct][0]=n-1;
    triangles[ct][1]=0;
    triangles[ct++][2]=2*n-1;
    triangles[ct][0]=0;
    triangles[ct][1]=n;
    triangles[ct++][2]=2*n-1;

    // bottom cap
    nodes[2*][0] = 0;
    nodes[2*limit][1] = 0;
    nodes[2*limit][2] = 0;

    if(orientation == 0) {// vertical
        nodes[2*n+1][0] = 0;
        nodes[2*n+1][1] = 0;
        nodes[2*n+1][2] = depth;
    }
    else if(orientation == 1){// horizontal
        nodes[2*n+1][0] = -depth;
        nodes[2*n+1][1] = 0;
        nodes[2*n+1][2] = 0;
    }

    // cylinder bottom cap
    for(int i=0; i<n; i++){
        if(i == n-1){
            triangles[ct][0]=n-1;
            triangles[ct][1]=0;
            triangles[ct++][2]=2*n;
            continue;
        }
        triangles[ct][0]=i;
        triangles[ct][1]=i+1;
        triangles[ct++][2]=2*n;
    }

    //  cylinder top cap
    for(int i=n; i<2*n; i++){
        if(i == 2*n-1){
            triangles[ct][0]=2*n-1;
            triangles[ct][1]=n;
            triangles[ct][2]=2*n+1;
            continue;
        }
        triangles[ct][0]=i;
        triangles[ct][1]=i+1;
        triangles[ct++][2]=2*n+1;
    }
    NEP next_position = {x = nodes[2*n][0], y = nodes[2*n][1], z = nodes[2*n][2]};

    return next_position;
}

NEP makeElbow(float nodes[][3], int triangles[][3], int start, float r, int n){
    int nNode = 0;
    int nTri = 0;

    float elb_theta_0 = ((3.0f*3.14159f)/2.0f);
    float theta_0 = 0.0f*3.14159f;
    float theta_1 = 2.0f*3.14159f;

    float d_theta = (theta_1-theta_0)/((float) n);
    float elb_theta = (theta_1-elb_theta_0)/(35.0f);
    printf("elbow theta: %f\n",elb_theta);

    float i_ring[n][3];

    // initial ring
    for (int i=start; i<start+n; i++) {
        float angle = theta_0 + i * d_theta;
        i_ring[i][0] = x0;
        i_ring[i][1] = y0 + r * cos(angle);
        i_ring[i][2] = z0 + r * sin(angle) - 3*r;
    }

    for(int ring=0; ring<36; ring++) {
        float elb_angle = elb_theta_0 + ring * elb_theta;
        for (int i = 0; i < n; i++) {
            float shift    = - i_ring[i][2];
            nodes[ct][0]   = i_ring[i][0] - shift * cos(elb_angle);
            nodes[ct][1]   = i_ring[i][1];
            nodes[ct][2]   = shift * sin(elb_angle) + 0.75;
            ct++;
        }
    }

    for (int ring = 0; ring < 35; ring++) {
        for (int i = 0; i < n - 1; i++) {
            triangles[nTri][0] = ring * n + i;
            triangles[nTri][1] = ring * n + i + 1;
            triangles[nTri++][2] = (ring + 1) * n + i;
            triangles[nTri][0] = ring * n + i + 1;
            triangles[nTri][1] = (ring + 1) * n + i + 1;
            triangles[nTri++][2] = (ring + 1) * n + i;
        }
        triangles[nTri][0] = (ring + 1) * n - 1;
        triangles[nTri][1] = ring * n;
        triangles[nTri++][2] = (ring + 2) * n - 1;
        triangles[nTri][0] = ring * n;
        triangles[nTri][1] = (ring + 1) * n;
        triangles[nTri++][2] = (ring + 2) * n - 1;
    }
    return;
}