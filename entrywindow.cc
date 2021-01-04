#include "entrywindow.h"
#include "canvas.h"
#include <iostream>
#include <cstdio>

#include <limits>

EntryWindow::EntryWindow()
:
  m_Frame_Drawing("Задайте размеры окна для рисования "), //
  m_VBox_Drawing(Gtk::ORIENTATION_VERTICAL), //
  m_VBox_SizeX(Gtk::ORIENTATION_VERTICAL), //
  m_VBox_SizeY(Gtk::ORIENTATION_VERTICAL), //
  m_Label_SizeX("Ширина X: ", Gtk::ALIGN_START), //
  m_Label_SizeY("Высота Y: ", Gtk::ALIGN_START), //
  m_SpinButton_SizeX(m_adjustment_SizeX), //
  m_SpinButton_SizeY(m_adjustment_SizeY), //
  m_adjustment_SizeX( Gtk::Adjustment::create(300, 100, 1000, 1, 10, 0) ), // (value, lower, uppper, step_increment, page_increment, page_size: unused)
  m_adjustment_SizeY( Gtk::Adjustment::create(300, 100, 1000, 1, 10, 0) ), // (value, lower, uppper, step_increment, page_increment, page_size: unused)

  m_Frame_Shape("Выберите фигуру для построения "),
  m_VBox_Shape(Gtk::ORIENTATION_VERTICAL), //
  m_VBox_Coordinates(Gtk::ORIENTATION_VERTICAL), //
  m_Label_Coordinates("Координаты фигуры : ", Gtk::ALIGN_START),

  m_RB_Point("Точка"),
  m_RB_Line("Отрезок"),
  m_RB_Rectangle("Прямоугольник"),
  m_RB_Ellipse("Эллипс"),
  m_RB_Polygon("Полигон"),
  m_Button_Add("Добавить фигуру"),
  m_Button_Remove("Удалить фигуру"),
  m_Button_Clear("Очистить список"),
  m_Button_Show("Построить фигуру"),
  m_Label_LineColor("Цвет контура", Gtk::ALIGN_START), //
  m_Label_FillColor("Цвет заполнения", Gtk::ALIGN_END), //
  m_FillColorSet(false),  
 
  m_VBox_Main(Gtk::ORIENTATION_VERTICAL, 5),
  m_VBox(Gtk::ORIENTATION_VERTICAL),
  m_Button_Close("Close")
{
  set_title("Shape Control Panel");

  m_HBox_Main.set_border_width(10);
  add(m_HBox_Main);
  m_HBox_Main.pack_start(m_VBox_Main, Gtk::PACK_EXPAND_WIDGET, 10);

  m_ScrolledWindow.set_min_content_width(200);
  m_HBox_Main.pack_start(m_Frame_ScrolledWindow);
  
  m_Frame_ScrolledWindow.set_border_width(8);
  m_Frame_ScrolledWindow.add(m_ScrolledWindow);
  //Add the TreeView, inside a ScrolledWindow, with the button underneath:
  m_ScrolledWindow.add(m_TreeView);
  //Only show the scrollbars when they are necessary:
  m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

  //Create the Tree model:
  m_refTreeModel = Gtk::ListStore::create(m_Columns);
  m_TreeView.set_model(m_refTreeModel);

  //Fill the TreeView's model
  //Gtk::TreeModel::Row row = *(m_refTreeModel->append());

  //Add the TreeView's view columns:
  //This number will be shown with the default numeric formatting.
  m_TreeView.append_column("Shape", m_Columns.m_col_shape);
  m_TreeView.append_column("Points", m_Columns.m_col_points);  

  // Frame Drawing Area
  m_VBox_Main.pack_start(m_Frame_Drawing); //
  m_VBox_Drawing.set_border_width(5); //
  m_Frame_Drawing.add(m_VBox_Drawing); //

  m_VBox_Drawing.pack_start(m_HBox_Drawing, Gtk::PACK_EXPAND_WIDGET, 5); //
  m_VBox_SizeX.pack_start(m_Label_SizeX); //
  m_VBox_SizeX.pack_start(m_SpinButton_SizeX); //
  m_HBox_Drawing.pack_start(m_VBox_SizeX, Gtk::PACK_EXPAND_WIDGET, 5); //

  m_VBox_SizeY.pack_start(m_Label_SizeY); //
  m_VBox_SizeY.pack_start(m_SpinButton_SizeY); //
  m_HBox_Drawing.pack_start(m_VBox_SizeY, Gtk::PACK_EXPAND_WIDGET, 5); //

  // Frame Shape
  m_VBox_Main.pack_start(m_Frame_Shape); //
  m_VBox_Shape.set_border_width(5); //
  m_Frame_Shape.add(m_VBox_Shape); //

  m_RB_Line.join_group(m_RB_Point); // All radio buttons to the same group
  m_RB_Rectangle.join_group(m_RB_Point);
  m_RB_Ellipse.join_group(m_RB_Point);
  m_RB_Polygon.join_group(m_RB_Point);
  m_RB_Line.set_active();

  m_VBox_Shape.pack_start(m_HBox_Shape, Gtk::PACK_EXPAND_WIDGET, 5); //
  m_HBox_Shape.pack_start(m_RB_Point);
  m_HBox_Shape.pack_start(m_RB_Line);
  m_HBox_Shape.pack_start(m_RB_Rectangle);
  m_HBox_Shape.pack_start(m_RB_Ellipse);
  m_HBox_Shape.pack_start(m_RB_Polygon);

  m_VBox_Shape.add(m_VBox_Coordinates);

  m_VBox_Coordinates.pack_start(m_Label_Coordinates);
  m_Entry_Coordinates.set_text("{25,25}{275,25}{150,150}{275,275}{25,275},...");
  m_VBox_Coordinates.pack_start(m_Entry_Coordinates, Gtk::PACK_EXPAND_WIDGET, 5);

  m_VBox_Shape.add(m_VBox_Color);

  m_VBox_Color.pack_start(m_Button_LineColor, Gtk::PACK_SHRINK, 5);
  m_VBox_Color.pack_start(m_Label_LineColor);
  m_Button_LineColor.signal_color_set().connect(sigc::mem_fun(*this,
    &EntryWindow::on_color_button_line_color_set) );

  m_VBox_Color.pack_end(m_Button_FillColor, Gtk::PACK_SHRINK, 5);
  m_VBox_Color.pack_end(m_Label_FillColor);
  m_Button_FillColor.signal_color_set().connect(sigc::mem_fun(*this,
    &EntryWindow::on_color_button_fill_color_set) );

  //Set start color:
  m_Color_Line.set_red(1.0);
  m_Color_Line.set_green(0.0);
  m_Color_Line.set_blue(0.0);
  m_Color_Line.set_alpha(1.0); //opaque

  m_Button_LineColor.set_rgba(m_Color_Line);
  m_Button_LineColor.set_title("Выберите цвет линии");
  m_Button_FillColor.set_title("Выберите цвет заполнения");

  //Show button
  m_VBox_Shape.add(m_VBox_Buttons);
  m_VBox_Buttons.set_border_width(5);
  m_VBox_Buttons.pack_start(m_HBox_Buttons, Gtk::PACK_EXPAND_WIDGET, 5);
  
  m_Button_Add.signal_clicked().connect( sigc::mem_fun(*this,
              &EntryWindow::on_button_add) );
  m_Button_Show.signal_clicked().connect( sigc::mem_fun(*this,
              &EntryWindow::on_button_show) );

  m_HBox_Buttons.pack_start(m_Button_Add, Gtk::PACK_SHRINK, 5);
  m_HBox_Buttons.pack_start(m_Button_Remove, Gtk::PACK_SHRINK, 5);
  m_HBox_Buttons.pack_start(m_Button_Clear, Gtk::PACK_SHRINK, 5);
  m_HBox_Buttons.pack_start(m_Button_Show, Gtk::PACK_SHRINK, 5);

  //Close button:
  m_Button_Close.signal_clicked().connect( sigc::mem_fun(*this,
              &EntryWindow::on_button_close) );
  m_VBox_Main.pack_start(m_Button_Close, Gtk::PACK_SHRINK, 5);

  show_all_children();
}

EntryWindow::~EntryWindow()
{
}

void EntryWindow::on_button_add()
{
  m_Color_Line.set_alpha(1.0);
  m_Color_Fill.set_alpha(1.0);
  

  if (m_RB_Line.get_active()) { 
    type_of_shape = "Line";
    }
  else if (m_RB_Point.get_active()) {
    type_of_shape = "Point";
    }
  else if (m_RB_Rectangle.get_active()) {
    type_of_shape = "Rectangle";
    }
  else if (m_RB_Ellipse.get_active()) {
    type_of_shape = "Ellipse";
    } 
  else {
    type_of_shape = "Polygon";
    }
  
  Shape S( type_of_shape, m_Entry_Coordinates.get_text(), m_Color_Line, m_FillColorSet, m_Color_Fill );
  myCanvas.add(&S);

  row = *(m_refTreeModel->append());
  row[m_Columns.m_col_shape]=type_of_shape;
  row[m_Columns.m_col_points]=m_Entry_Coordinates.get_text();
  
  myCanvas.show_canvas();
}

void EntryWindow::on_button_close()
{
  hide();
}

void EntryWindow::on_color_button_line_color_set() 
{
  m_Color_Line = m_Button_LineColor.get_rgba();
}

void EntryWindow::on_color_button_fill_color_set()
{
  m_Color_Fill = m_Button_FillColor.get_rgba();
  m_FillColorSet = true;
}

void EntryWindow::on_button_remove() 
{

}
void EntryWindow::on_button_clear()
{
  myCanvas.remove_all();
  
}
void EntryWindow::on_button_show()
{
  DrawWnd.set_default_size(m_SpinButton_SizeX.get_value_as_int(), m_SpinButton_SizeY.get_value_as_int());
  DrawWnd.set_title("Окно для рисования");
  
  if (myCanvas.size()==0) return;

  if (DrawWnd.get_child()==NULL) DrawWnd.add(myCanvas);
  
  //DrawWnd.add(myCanvas);
  myCanvas.show();
  DrawWnd.show();
  DrawWnd.queue_draw();
}

