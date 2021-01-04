#include "canvas.h"
#include "entrywindow.h"
#include <gtkmm/application.h>

int main(int argc, char** argv)
{
   auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

   EntryWindow EntryWnd;

   return app->run(EntryWnd);
}