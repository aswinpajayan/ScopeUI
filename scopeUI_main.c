/*
 * =====================================================================================
 *
 *       Filename:  scopeUI_main.c
 *
 *    Description:  simple UI for a wireless DSO
 *
 *        Version:  1.0
 *        Created:  Thursday 04 April 2019 02:33:50  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aswin P Ajayan (), aswinp2008@gmail.com
 *   Organization:  iitb
 *
 * =====================================================================================
 */
//to compile use     gcc `pkg-config --cflags gtk+-3.0` -o start_ui scopeUI_main.c `pkg-config --libs gtk+-3.0
#include <gmodule.h>
#include <gtk/gtk.h>
#include "support.h"

int main (int   argc,char *argv[]){
  GtkBuilder *builder;
  GObject *window;
  GError *error = NULL;

  app_widgets *widgets = g_slice_new(app_widgets);

  gtk_init (&argc, &argv);

  /* Construct a GtkBuilder instance and load our UI description */
  builder = gtk_builder_new ();
  if (gtk_builder_add_from_file (builder, "DSO.glade", &error) == 0)
    {
      g_printerr ("Error loading file: %s\n", error->message);
      g_clear_error (&error);
      return 1;
    }

  /* Connect signal handlers to the constructed widgets. */
  //get pointer to the main window
  window = gtk_builder_get_object (builder, "main_window");
    /* Get objects from UI */
    widgets->w_drawing_area = GTK_WIDGET(gtk_builder_get_object(builder,"DSO_screen"));
    widgets->w_scale_volt = GTK_WIDGET(gtk_builder_get_object(builder,"scale_volt"));
    widgets->w_lbl_marker = GTK_WIDGET(gtk_builder_get_object(builder,"lbl_marker"));
 //   widgets->btn_test = GTK_WIDGET(gtk_builder_get_object(builder,"btnConnect"));
 //
    gtk_widget_add_events(widgets->w_drawing_area,GDK_BUTTON_PRESS_MASK);
gtk_widget_add_events(GTK_WIDGET(window),GDK_BUTTON_PRESS_MASK);
    /* Connect signals */
    gtk_builder_connect_signals( builder ,widgets);

    /* Destroy builder, since we don't need it anymore */
    g_object_unref( G_OBJECT( builder ) );
  gtk_widget_show(GTK_WIDGET(window));   
  gtk_main ();
  g_slice_free(app_widgets, widgets);

  return 0;
}
