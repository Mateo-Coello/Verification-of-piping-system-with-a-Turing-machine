//
// Created by mai on 9/16/22.
//

#ifndef CONS_GEOM_ELEMENTS_H
#define CONS_GEOM_ELEMENTS_H

typedef struct next_element_post{
    float x;
    float y;
    float z;
} NEP;

NEP makeCap(float depth, float r, int n, int orientation);
NEP makeCylinder(float depth, float r, int n, int orientation);
NEP makeElbow(float r, int n);

#endif //CONS_GEOM_ELEMENTS_H
