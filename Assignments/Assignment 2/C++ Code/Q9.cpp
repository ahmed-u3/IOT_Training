#include <iostream>
#include <string>


class shape {
public:
    double area;
    virtual double area() { return area; } // a function will be overwrite
 };

class Rectangle : public base {
public:
    double length;
    double width;
    Rectangle(double l , double w ){
        length=l;
        width=w;
        }
    double area() { return length*width; }

};

class Circle : public base {
public:
    double radius;
    Circle(double r ){
        radius=r;
        }
    double area() { return (22*radius*radius)/7; } //py *radius**2

};

class Triangle : public base {
public:
    double base;
    double height;
    Triangle(double b , double h ){
        base=b;
        height=h;
        }
    double area() { return (base * height)/2 ; }

};
