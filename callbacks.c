/*
 * =====================================================================================
 *
 *       Filename:  callbacks.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  Thursday 04 April 2019 03:32:43  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aswin P Ajayan (), aswinp2008@gmail.com
 *   Organization:  iitb
 *
 * =====================================================================================
 */
#include "support.h"
#include <gmodule.h>
#include <gtk/gtk.h>
#include "UDP_server.c"
#include <pthread.h>
/*includes for plotting*/
#include <math.h>
#include <cairo.h>

#define WIDTH   640
#define HEIGHT  480

#define ZOOM_X  100.0
#define ZOOM_Y 100.0
/*definitions for plotting ends here*/

pthread_t plotter_t_id;

struct {
  int count;
  double coordx[100];
  double coordy[100];
} glob;


void* async_plotter_thread(void *arg);

void on_btnExport_clicked( GtkButton *widget, app_widgets *data ){
	g_print("from btnExport's handler\n");
}
static void do_drawing(cairo_t *cr)
{
//	cairo_t *cr;
//	cr = cairo_create(surface);
	//cairo_set_source_surface(cr,surface,0.0,0.0);
	 //cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
//cairo_paint (cr);
  int i;
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_set_line_width(cr, 1);
 // cairo_translate (cr, 720/ 2, 480 / 2);
 //   cairo_scale (cr, ZOOM_X, -ZOOM_Y);


// int i, j;
//  for (i = 0; i <= glob.count - 1; i++ ) {
//      for (j = 0; j <= glob.count - 1; j++ ) {
//          cairo_move_to(cr, glob.coordx[i], glob.coordy[i]);
//          cairo_line_to(cr, glob.coordx[j], glob.coordy[j]);
//      }
//  }
//
//  glob.count = 0;
////  cairo_move_to(cr, 0, 10);
////cairo_move_to(cr, 30, 100);
////cairo_move_to(cr,90, 10);
////ndow_begin_draw_framecairo_move_to(cr, 0, 10);
////cairo_rel_curve_to (cr, 0, 10, 20, 50, 5, 0);
// //cairo_set_source_rgb( cr, 0, 1, 0 );
//    //cairo_paint( cr );
//
////cairo_set_source_rgb(cr, 0, 0, 0);
////cairo_rel_curve_to (cr, 10, 10, 40, 60, 25, 35);
////cairo_set_line_width (cr, 0.2);
////  cairo_stroke(cr);    
////    cairo_set_source_rgba (cr, 1, 0.2, 0.2, 0.6);
//    cairo_stroke (cr);
// cairo_save (cr);
//      cairo_set_font_size (cr, 40);
//      cairo_move_to (cr, 40, 60);
//      cairo_set_source_rgb (cr, 0,0,0);
//      cairo_show_text (cr, "Hello World");
//   cairo_restore (cr);
//  
//     // cairo_destroy(cr);
//     //cairo_save(cr);
//     cairo_rectangle (cr, 0, 0, 100, 100);
//cairo_set_source_rgba (cr, 0, 1, 0, 0.80);
//cairo_set_source_rgb(cr, 0, 0, 0);
//cairo_set_line_width(cr, 0.5);
//cairo_fill (cr);
//cairo_restore(cr);

//cairo_set_line_width(cr, 5);
//cairo_move_to (cr, 0, 0);
//cairo_line_to (cr, 300, 250);
//cairo_rel_line_to (cr, 200, 200);
//cairo_curve_to (cr, 250, 275,300,400,400,400);
//cairo_stroke(cr);

cairo_translate(cr,0,0);
cairo_scale(cr,1,0.5);
cairo_move_to(cr,0,512);
cairo_line_to(cr,512,512);
cairo_stroke(cr);
cairo_move_to(cr,0,512);
//cairo_line_to(cr,200,230);
//cairo_line_to(cr,400,-230);
//cairo_line_to(cr,450,200);
for(i = 0; i < BUFSIZE; i ++){
	cairo_line_to(cr,i,1024-in_buf[i]);
}

cairo_set_source_rgba (cr, 0, 1, 0, 0.80);
cairo_stroke(cr);
g_print("yayyy i redrew it  \n");
}
void on_draw_event(GtkWidget *widget, cairo_t *cr ,app_widgets *data)

{
//	GtkWidget *widget_da;
//	widget_da = GTK_WIDGET(data->w_drawing_area);
//	surface = gdk_window_create_similar_surface (gtk_widget_get_window(widget_da),
//			CAIRO_CONTENT_COLOR,
//			gtk_widget_get_allocated_width(widget_da),
//			gtk_widget_get_allocated_height(widget_da));
  do_drawing(cr);

}

void clicked_da( GtkWidget *widget,GdkEventButton *event, app_widgets *data){
	
if (event->button == 1) {
        glob.coordx[glob.count] = event->x;
        glob.coordy[glob.count++] = event->y;
 }
}


void on_btnRedraw_clicked( GtkButton *widget, app_widgets *data ){
	g_print("from redraw button handler \n");
    //cairo_t *cr = gdk_cairo_create (data->w_drawing_area);
    //GdkRectangle da;            /* GtkDrawingArea size */
    //gdouble dx = 5.0, dy = 5.0; /* Pixels between each point */
    //gdouble i, clip_x1 = 0.0, clip_y1 = 0.0, clip_x2 = 0.0, clip_y2 = 0.0;
    //gint unused = 0;
    
    g_print("from redraw button handler \n");

    /* Define a clipping zone to improve performance */
    //cairo_rectangle (cr,
    //        event->area.x,
    //        event->area.y,
    //        event->area.width,
    //        event->area.height);
    //cairo_clip (cr);

    /* Determine GtkDrawingArea dimensions */

    /* Draw on a black background */
    //cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
    //cairo_paint (cr);

    ///* Change the transformation matrix */
    //cairo_translate (cr, 300 / 2, 500 / 2);
    //cairo_scale (cr, ZOOM_X, -ZOOM_Y);

    ///* Determine the data points to calculate (ie. those in the clipping zone */
    //cairo_device_to_user_distance (cr, 10, 20);
    //cairo_clip_extents (cr, &clip_x1, &clip_y1, &clip_x2, &clip_y2);
    //cairo_set_line_width (cr, dx);

    ///* Draws x and y axis */
    //cairo_set_source_rgb (cr, 0.0, 1.0, 0.0);
    //cairo_move_to (cr, clip_x1, 0.0);
    //cairo_line_to (cr, clip_x2, 0.0);
    //cairo_move_to (cr, 0.0, clip_y1);
    //cairo_line_to (cr, 0.0, clip_y2);
    //cairo_stroke (cr);

    ///* Link each data point */
    //for (i = clip_x1; i < clip_x2; i += dx)
    //    cairo_line_to (cr, i, 0.03 * pow (i, 3));

    ///* Draw the curve */
    //cairo_set_source_rgba (cr, 1, 0.2, 0.2, 0.6);
    //cairo_stroke (cr);
    //cairo_destroy (cr);
	/* Return TRUE, since we handled this event */
    //gchar *lbl;
    //lbl = g_strdup_printf("test");
    //gtk_button_set_label(GTK_BUTTON(data->btn_test),lbl);
    //g_free(lbl)
    gtk_widget_queue_draw(data->w_drawing_area);
g_print("from redraw button handler \n");
}


G_MODULE_EXPORT gboolean
on_btnConnect_clicked( GtkWidget *widget, GdkEventExpose *event, app_widgets *data ){
    char port_number[5];	
    g_print("from btnConnect");


    g_print("initialising UDP_server");
    //test initialisation of command buffer 
    memcpy(out_buf,"TEST_COMMAND",12);
    memcpy(port_number,"50001",5);   //#define PORT_NUMBER 50001  
    pthread_create(&server_t_id,NULL,&socketThread,&port_number);

    pthread_create(&plotter_t_id,NULL,&async_plotter_thread,NULL);
    //pthread_join(server_t_id,NULL);
    return(TRUE);
}


void* async_plotter_thread(void *arg){
   int i = 0;
   while(1){
        printf("starting to plot \n");
	pthread_mutex_lock(&lock_buf);
	printf("shared data locked by plotter thread\n");
	for(i = 0;i<BUFSIZE;i++){
		g_print("%d \t",in_buf[i]);
	}
	g_print("\n\n");
	pthread_mutex_unlock(&lock_buf);
	printf("shared buf ready to recieve new data\n");
	//suspend plotting till new data arrives 
	//pthread_mutex_lock(&lock_plotter);
	printf("waiting for new data .....\n");
	pthread_cond_wait(&suspend_plotting,&lock_plotter);
   }
   pthread_exit(NULL);
}

