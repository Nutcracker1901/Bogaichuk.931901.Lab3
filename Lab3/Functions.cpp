#include <Windows.h>
#include <math.h>
#include <d2d1.h>
#include "Functions.h"

TempFunction::TempFunction(Functions* f) : t(f) 
{
}
TempFunction::~TempFunction() 
{ 
    delete t; 
}
void TempFunction::SetFunc(Functions* f) 
{ 
    t = f; 
}
void TempFunction::RunFunction(int cx, int cy, D2D1_POINT_2F* apt) 
{
    return t->RunFunction(cx, cy, apt);
}

void HyperbolaFunction::RunFunction(int cx, int cy, D2D1_POINT_2F* apt) 
{
    double x, y, coeff;
    coeff = cx * cy / 64.0;
    for (int i = 0; i < NUM; i++)        {
        x = i * (double)cx / NUM;
        if (i < NUM / 2 || i > NUM / 2) 
        {
            y = (int)(-coeff / (x - cx / 2) + cy / 2);
        }
        else 
        {
            y = cy / 2; 
        }
        
        if (i < NUM / 2) {
            apt[i].x = x;
            apt[i].y = y;
        }
        else if (i > NUM / 2) {
            apt[i].x = x;
            apt[i].y = y;
        }
    }
}

void ParabolaFunction::RunFunction(int cx, int cy, D2D1_POINT_2F* apt)
{
    double x, coeff;
    coeff = 2.0 * cy / cx;
    for (int i = 0; i < NUM; i++)
    {
        x = i * cx / NUM;
        apt[i].x = x;
        apt[i].y = (int)(coeff * (-x * x / cx + x));       
    }
    
}

void SinusoidFunction::RunFunction(int cx, int cy, D2D1_POINT_2F* apt)
{
    for (int i = 0; i < NUM; i++)
    {
        apt[i].x = i * cx / NUM;
        apt[i].y = (int)(cy / 2 * (1 - sin(TWOPI * i / NUM)));   
    }   
}

void SquareRootFunction::RunFunction(int cx, int cy, D2D1_POINT_2F* apt)
{
    for (int i = 0; i < NUM; i++)
    {
        apt[i].x = cx / 2 + i * cx / (2 * NUM);
        apt[i].y = (int)(cy / 2 - sqrt(i) * cy / (2 * sqrt(NUM)));   
    }   
}

