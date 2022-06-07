#pragma once
#define SQUAREROOTF 1
#define PARABOLAF 2
#define HYPERBOLAF 3
#define SINUSOIDF 4
#define NUM    1000
#define TWOPI  (2 * 3.14159)
#ifndef FUNCTIONS_H  
#define  FUNCTIONS_H
#include "resource.h"

class Functions
{
public:
    virtual void RunFunction(int cx, int cy, D2D1_POINT_2F* apt) = 0;
    virtual ~Functions() {}
};

class TempFunction
{
public:
    TempFunction(Functions* f);
    ~TempFunction();
    void SetFunc(Functions* f);
    void RunFunction(int cx, int cy, D2D1_POINT_2F* apt);
private:
    Functions* t;
};

class HyperbolaFunction :
    public Functions
{
public:
    void RunFunction(int cx, int cy, D2D1_POINT_2F* apt);
};

class ParabolaFunction :
    public Functions
{
public:
    void RunFunction(int cx, int cy, D2D1_POINT_2F* apt);
};

class SinusoidFunction :
    public Functions
{
public:
    void RunFunction(int cx, int cy, D2D1_POINT_2F* apt);
};

class SquareRootFunction :
    public Functions
{
public:
    void RunFunction(int cx, int cy, D2D1_POINT_2F* apt);
};



#endif