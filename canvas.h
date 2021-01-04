#ifndef CANVAS_H
#define CANVAS_H

#include <iostream>
#include <vector>
#include <gtkmm.h>

#include <gtkmm/drawingarea.h>
#include <gdkmm/rgba.h>

class CoordinatePoint
{
public:
    double x,y;

    CoordinatePoint() : x(0.0), y(0.0) {}
    CoordinatePoint(double x, double y) : x(x), y(y) {}
    ~CoordinatePoint() {}
};

class Shape 
{
protected:
    std::string type_of_shape;
    Gdk::RGBA line_color;
    Gdk::RGBA fill_color;
    std::vector <CoordinatePoint> points; 
    bool filled;
public:
    void set_lcolor( float r, float g, float b, float a) { line_color.set_rgba(r, g, b, a); }
    void set_fcolor( float r, float g, float b, float a) { fill_color.set_rgba(r, g, b, a); }
    void str_to_points ( std::string );
    void show_points();
    Shape() {}
    Shape( Shape *shape);
    Shape( std::string input_type_of_shape, std::string input_coordinates, Gdk::RGBA input_line_color, bool input_filled, Gdk::RGBA input_fill_color );
    ~Shape() {}
    std::string get_type() { return type_of_shape; }
    Gdk::RGBA get_line_color () { return line_color; }
    Gdk::RGBA get_fill_color () { return fill_color; }
    bool is_filled() { return filled; }
    CoordinatePoint get_point( unsigned i ) { return points[i]; }
    int get_size() { return points.size(); }
};

class Point : public Shape, public Gtk::DrawingArea
{
public:
    Point ( Shape * );
    Point () {};
    bool draw(const Cairo::RefPtr<Cairo::Context>& cr) { return on_draw(cr); };
protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;   
};

class Line : public Shape, public Gtk::DrawingArea
{
public:
    Line ( Shape * );
    Line () {};
    bool draw(const Cairo::RefPtr<Cairo::Context>& cr) { return on_draw(cr); };
protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;    
};

class Ellipse : public Shape, public Gtk::DrawingArea
{
public:
    Ellipse ( Shape * );
    Ellipse () {};
    bool draw(const Cairo::RefPtr<Cairo::Context>& cr) { return on_draw(cr); };
protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
};

class Rectangle : public Shape, public Gtk::DrawingArea
{
public:
    Rectangle ( Shape * );
    Rectangle () {};
    bool draw(const Cairo::RefPtr<Cairo::Context>& cr) { return on_draw(cr); };
protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
};

class Polygon : public Shape, public Gtk::DrawingArea
{
public:
    Polygon ( Shape * );
    Polygon() {};
    bool draw(const Cairo::RefPtr<Cairo::Context>& cr) { return on_draw(cr); };
protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
};

class Canvas : public Gtk::DrawingArea
{
    
protected:    
    std::vector<Shape *> shapes;
    
public:
    void add(Shape *shape);
    void remove(Shape *shape);
    void remove_all();
    void info();
    void show_canvas();
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr); 
    int size() { return shapes.size(); }
    Shape * get ( unsigned i ) { return shapes[i]; }
    std::string type( unsigned i) { return shapes[i]->get_type(); }
};

#endif // CANVAS_H