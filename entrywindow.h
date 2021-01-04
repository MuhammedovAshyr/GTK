#ifndef GTKMM_ENTRYWINDOW_H
#define GTKMM_ENTRYWINDOW_H

#include <canvas.h>
#include <gtkmm.h>


class EntryWindow : public Gtk::Window
{
public:
  EntryWindow();
  virtual ~EntryWindow();

protected:
  Gtk::Window DrawWnd;
  Polygon Po;
  Rectangle R;
  Ellipse E;
  Line L;
  Point P;
  Canvas myCanvas, drwCanvas;
  Gtk::TreeModel::Row row;
  
  class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
    public:

    ModelColumns() { add(m_col_shape); add(m_col_points); }
    Gtk::TreeModelColumn<Glib::ustring> m_col_shape;
    Gtk::TreeModelColumn<Glib::ustring> m_col_points;
  };

  ModelColumns m_Columns;

  Gtk::Frame m_Frame_ScrolledWindow;
  Gtk::ScrolledWindow m_ScrolledWindow;
  Gtk::TreeView m_TreeView;
  Glib::RefPtr<Gtk::ListStore> m_refTreeModel;

  //Signal handlers:
  std::string type_of_shape;
  void on_button_close();
  void on_button_add();
  void on_button_remove();
  void on_button_clear();
  void on_button_show();

  void on_color_button_line_color_set();
  void on_color_button_fill_color_set();

  //Child widgets:
  Gtk::Frame m_Frame_Drawing, m_Frame_DrwArea; //
  Gtk::Box m_VBox_Drawing, m_HBox_Drawing; //
  Gtk::Box m_VBox_SizeX, m_VBox_SizeY; //
  Gtk::Label m_Label_SizeX, m_Label_SizeY; //
  Glib::RefPtr<Gtk::Adjustment> m_adjustment_SizeX, m_adjustment_SizeY; //
  Gtk::SpinButton m_SpinButton_SizeX, m_SpinButton_SizeY; //

  Gtk::Frame m_Frame_Shape; //
  Gtk::Box RB_Shapes; //
  Gtk::RadioButton m_RB_Point, m_RB_Line, m_RB_Rectangle, m_RB_Ellipse, m_RB_Polygon; //

  Gtk::Box m_VBox_Shape, m_HBox_Shape; //
  Gtk::Box m_VBox_Coordinates;
  Gtk::Box m_HBox_Coordinates;
  Gtk::Entry m_Entry_Coordinates;
  Gtk::Label m_Label_Coordinates;

  Gtk::Button m_Button_Add; //
  Gtk::Button m_Button_Remove;
  Gtk::Button m_Button_Clear;
  Gtk::Button m_Button_Show;

  Gtk::Box m_VBox_Color, m_HBox_Color; //
  Gtk::ColorButton m_Button_LineColor, m_Button_FillColor; //
  Gtk::Label m_Label_LineColor, m_Label_FillColor;
  bool m_FillColorSet;

  Gdk::RGBA m_Color_Line, m_Color_Fill; //

  Gtk::Box m_VBox_Buttons, m_HBox_Buttons;
  Gtk::Box m_VBox_Main, m_VBox;
  Gtk::Box m_HBox_Main;

  Gtk::Button m_Button_Close;
  
};

#endif //GTKMM_ENTRYWINDOW_H