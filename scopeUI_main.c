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

static void
print_hello (GtkWidget *widget,
             gpointer   data)
{
  g_print ("Hello World\n");

}


int
main (int   argc,
      char *argv[])
{
  GtkBuilder *builder;
  GObject *window;
  GObject *button;
  GError *error = NULL;
  ChData     *data;

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
  window = gtk_builder_get_object (builder, "main_window");

  /* Allocate data structure */
    data = g_slice_new( ChData );

    /* Get objects from UI */
#define GW( name ) CH_GET_WIDGET( builder, name, data )
    GW( main_window );
    GW( chart_area );
#undef GW

    /* Connect signals */
    gtk_builder_connect_signals( builder, data );

    /* Destroy builder, since we don't need it anymore */
    g_object_unref( G_OBJECT( builder ) );
  
  gtk_main ();

  return 0;
}
