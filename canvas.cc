#include <iostream>
#include <algorithm>
#include <canvas.h>

#include <gtkmm/drawingarea.h>

void Shape::show_points() 
    {
        for( unsigned j=0; j<points.size(); j++ )
            {
                std::cout << "{" << points[j].x << "," << points[j].y << "}";
            }
        std::cout << std::endl;
    }

void Shape::str_to_points( std::string str )
    {
       double d;
       std::stringstream iss(str);
       std::vector <double> vec;    // временный вектор с числами типа double
       while (true)
       {
            if (iss >> d) vec.push_back(d);
            if (iss.eof() || iss.bad())     // проверка на конец строки
            {
                break;
            }
            else if (iss.fail())    // обработка ошибок 
            {
                iss.clear();    // очистка флагов ошибки
                iss.ignore(1, EOF);     // выбрасываем из строки неверный символ 
            }
       }
       //std::cout << str << "====> ";
       for (unsigned i=0; i+1<vec.size(); i+=2) 
       {
           points.push_back(CoordinatePoint(vec[i],vec[i+1]));
           //std::cout << "[" << vec[i] << "," << vec[i+1] << "] ";
       }
       //std::cout << std::endl;
    }


Shape::Shape( Shape* source )
    {
        type_of_shape = source->type_of_shape;
        for ( unsigned i=0; i<source->points.size(); i++ ) points.push_back(source->points[i]);
        line_color = source->line_color;
        filled = source->filled;
        fill_color = source->fill_color;
    }

Shape::Shape( std::string input_type_of_shape, std::string input_coordinates, Gdk::RGBA input_line_color, bool input_filled, Gdk::RGBA input_fill_color )
    {
        type_of_shape = input_type_of_shape;
        this->str_to_points(input_coordinates);
        line_color = input_line_color;
        filled = input_filled;
        fill_color = input_fill_color;
    }


void Canvas::add(Shape *source)
    {
        Shape* s = new Shape(source);
        shapes.push_back(s);
    }

void Canvas::remove(Shape *value)
    {
        // идиома "erase-remove" :
        // стандартная ф-ция std::remove сдвигает искомый элемент в конец ряда для последующего удаления ф-цией vector::erase
        shapes.erase( std::remove(shapes.begin(),shapes.end(),value), shapes.end() );
    }

void Canvas::remove_all()
    {
        shapes.clear();  
    }

void Canvas::info()
    {

    }

void Canvas::show_canvas()
    {
        for (unsigned i=0; i<shapes.size(); i++) 
            {
                std::cout << shapes[i]->get_type() << std::endl;
                shapes[i]->show_points();
            } 
    }

bool Canvas::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)    
    {
        for (unsigned i=0; i<shapes.size(); i++) 
            {
                if(this->type(i)== "Point") 
                {
                    auto P = Point(this->get(i));
                    P.draw(cr);
                }
                else if (this->type(i) == "Line")
                {
                    auto L = Line(this->get(i));
                    L.draw(cr);
                }
                else if (this->type(i) == "Rectangle")
                {
                    auto R = Rectangle(this->get(i));
                    R.draw(cr);
                }
                else if (this->type(i) == "Ellipse")
                {
                    auto E = Ellipse(this->get(i));
                    E.draw(cr);
                }
                else
                {
                    auto Po = Polygon(this->get(i));
                    Po.draw(cr);
                }
            }
        return true;
    }


Point::Point ( Shape * source )
    {
        type_of_shape = source->get_type();
        for ( unsigned i=0; i<source->get_size(); i++ ) points.push_back(source->get_point(i));
        line_color = source->get_line_color();
        filled = source->is_filled();
        fill_color = source->get_fill_color();        
    }

Line::Line ( Shape * source )
    {
        type_of_shape = source->get_type();
        for ( unsigned i=0; i<source->get_size(); i++ ) points.push_back(source->get_point(i));
        line_color = source->get_line_color();
        filled = source->is_filled();
        fill_color = source->get_fill_color();        
    }

bool Line::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
    {
        std::cout << "drawing line" << std::endl;
        
        line_color.set_alpha( 1.0 );
        fill_color.set_alpha( 1.0 );

        cr->save();
        
        cr->move_to(points[0].x, points[0].y);
        cr->line_to(points[1].x, points[1].y);

        Gdk::Cairo::set_source_rgba(cr, line_color);
        cr->stroke();        

        cr->restore();
        return true;
    }

bool Point::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
    {
        std::cout << "drawing point" << std::endl;

        line_color.set_alpha( 1.0 );
        fill_color.set_alpha( 1.0 );

        cr->save();

        cr->set_line_width(5.0);
        cr->set_line_cap(Cairo::LINE_CAP_ROUND);
        cr->move_to(points[0].x, points[0].y);
        cr->line_to(points[0].x, points[0].y);

        Gdk::Cairo::set_source_rgba(cr, line_color);
        cr->stroke();        

        cr->restore();

        return true;
    }

Ellipse::Ellipse ( Shape * source )
    {
        type_of_shape = source->get_type();
        for ( unsigned i=0; i<source->get_size(); i++ ) points.push_back(source->get_point(i));
        line_color = source->get_line_color();
        filled = source->is_filled();
        fill_color = source->get_fill_color();        
    }

bool Ellipse::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
    {
        std::cout << "drawing ellipse" << std::endl;

        line_color.set_alpha( 1.0 );
        fill_color.set_alpha( 1.0 );

        // вписываем эллипс в прямоугольник, заданный координатами левой верхней и правой нижней координатой
        double ex, ey, ew, eh;
        // находим центр эллипса
        ex = points[0].x + (points[1].x-points[0].x)/2;
        ey = points[0].y + (points[1].y-points[0].y)/2;
        // ширина и высота эллипса
        ew = points[1].x-points[0].x;
        eh = points[1].y-points[0].y;

        cr->save(); // рекомендуется сохранить контекст, т.к. для построения эллипса потребуется изменить пропорции и это может повлиять на ширину линии

        cr->translate(ex, ey);  // совмещаем с центром координат (ex, ey) == (0, 0)
        cr->scale(ew / 2.0, eh / 2.0);  // для ширины: ew / 2.0 == 1.0
                                        // для высоты: eh / 2.0 == 1.0
        cr->arc(0.0, 0.0, 1.0, 0.0, 2 * M_PI);  // 'дуга' с центром в (0, 0)
                                                // с 'радиусом' равным 1.
                                                // от 0 до 2*pi
        cr->restore();

        if (filled)
            {
                Gdk::Cairo::set_source_rgba(cr, fill_color);
                cr->fill_preserve();
            }
        
        Gdk::Cairo::set_source_rgba(cr, line_color);
        cr->stroke();

        return true;
    }

Rectangle::Rectangle ( Shape * source )
    {
        type_of_shape = source->get_type();
        for ( unsigned i=0; i<source->get_size(); i++ ) points.push_back(source->get_point(i));
        line_color = source->get_line_color();
        filled = source->is_filled();
        fill_color = source->get_fill_color();        
    }

bool Rectangle::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
    {
        std::cout << "drawing rectangle" << std::endl;

        line_color.set_alpha( 1.0 );
        fill_color.set_alpha( 1.0 );

        double width, height; 

        width = points[1].x-points[0].x;
        height = points[1].y-points[0].y;

        cr->save();

        cr->set_line_width(5.0);

        cr->rectangle(points[0].x, points[0].y, width, height);

        if (filled)
            {
                Gdk::Cairo::set_source_rgba(cr, fill_color);
                cr->fill_preserve();
            }
        
        Gdk::Cairo::set_source_rgba(cr, line_color);
        cr->stroke();

        cr->restore();
        
        return true;
    }

Polygon::Polygon ( Shape * source )
    {
        type_of_shape = source->get_type();
        for ( unsigned i=0; i<source->get_size(); i++ ) points.push_back(source->get_point(i));
        line_color = source->get_line_color();
        filled = source->is_filled();
        fill_color = source->get_fill_color();        
    }

bool Polygon::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)    
    {
        std::cout << "drawing polygon" << std::endl;

        line_color.set_alpha( 1.0 );
        fill_color.set_alpha( 1.0 );
        
        cr->save();

        cr->move_to(points[0].x, points[0].y);
        for (unsigned i=1; i<points.size(); i++) 
            {
                cr->line_to(points[i].x, points[i].y);
            }
        cr->close_path();

        if(filled) 
            {
                Gdk::Cairo::set_source_rgba(cr, fill_color);
                cr->fill_preserve();
            }
        
        Gdk::Cairo::set_source_rgba(cr, line_color);
        cr->stroke();

        cr->restore();

        return true;
    }