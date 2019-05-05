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


#define ZOOM_X  100.0
#define ZOOM_Y 100.0
/*definitions for plotting ends here*/

//#define DEBUG 1

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
static void do_drawing(cairo_t *cr,double WIDTH,double HEIGHT)
{
  	int i;
	/*___________painting a black background_______*/
  	cairo_set_source_rgba( cr, 0.0, 0.078, 0.039,1 );
 	cairo_paint( cr );

	
	cairo_translate(cr,0,0); 
	//setting coordinate axes origin, 
	
	
	cairo_scale(cr,WIDTH/BUFSIZE,(HEIGHT / 1024.0)); //sampling res is 10 bits
	//scale ratio, user to device(converting adc coordinates to pixel coordinates
	
	/*___________grid lines _______________________*/
	cairo_set_source_rgba(cr, 1, 1, 1,0.2);
	cairo_set_line_width(cr, 0.4);
	cairo_move_to(cr,0,512);
	cairo_line_to(cr,512,512);
	cairo_stroke(cr);

	for(i = 0 ; i < 512 ; i = i + 64){
		cairo_move_to(cr,0,i);
		cairo_line_to(cr,512,i);
		cairo_move_to(cr,0,i+512);
		cairo_line_to(cr,512,i+512);
		cairo_move_to(cr,i,0);
		cairo_line_to(cr,i,1024);
	}
	cairo_stroke(cr);
	cairo_set_source_rgba(cr, 1, 1, 1,0.8);
	cairo_set_line_width(cr, 0.6);
	cairo_move_to(cr,0,512);
	cairo_line_to(cr,512,512);
	cairo_move_to(cr,256,0);
	cairo_line_to(cr,256,1024);
	cairo_stroke(cr);

	/*___________move origin and plot adc data______*/
	cairo_move_to(cr,0,1024-in_buf[0]);
	for(i = 0; i < BUFSIZE; i ++){
		cairo_line_to(cr,i,1024-in_buf[i]);
	}

	cairo_set_source_rgba (cr, 0, 1, 0, 0.80);
	cairo_stroke(cr);

	/*__________for displaying trigger________________*/
	cairo_set_line_width(cr, 8);
	cairo_set_source_rgba (cr, 1, 0, 0, 1);
	cairo_move_to(cr,0,1024 - (out_buf[2] * 8));
	cairo_line_to(cr,8,1024 - (out_buf[2] * 8));
	cairo_move_to(cr,504,1024 - (out_buf[2] * 8));
	cairo_line_to(cr,512,1024 - (out_buf[2] * 8));
	cairo_stroke(cr);
	g_print("redraw complete  \n");
}
void on_draw_event(GtkWidget *widget, cairo_t *cr ,app_widgets *data){
  double WIDTH,HEIGHT;
  WIDTH = gtk_widget_get_allocated_width(data->w_drawing_area);
  HEIGHT = gtk_widget_get_allocated_height(data->w_drawing_area);
  do_drawing(cr,WIDTH,HEIGHT);
}

void on_scale_volt_value_changed(GtkWidget *widget , app_widgets *data){
	char *lbl_text;
     	gdouble pos=gtk_range_get_value(GTK_RANGE(widget));
	lbl_text = g_strdup_printf ("Amp : %0.f ",pos);
	gtk_label_set_markup(GTK_LABEL(data->w_lbl_marker),lbl_text);
	out_buf[0] = (char)gtk_range_get_value(GTK_RANGE(widget));
	g_free(lbl_text);
}


void on_scale_time_value_changed(GtkWidget *widget , app_widgets *data){
	char *lbl_text;
	gdouble pos=gtk_range_get_value(GTK_RANGE(widget));
	lbl_text = g_strdup_printf ("time : %0.f ",pos);
	gtk_label_set_markup(GTK_LABEL(data->w_lbl_marker),lbl_text);
	out_buf[1] = (char)gtk_range_get_value(GTK_RANGE(widget));
	g_free(lbl_text);
}

void on_scale_trig_value_changed(GtkWidget *widget , app_widgets *data){
	char *lbl_text;
	gdouble pos=gtk_range_get_value(GTK_RANGE(widget));
	lbl_text = g_strdup_printf ("trig : %0.f ",pos);
	gtk_label_set_markup(GTK_LABEL(data->w_lbl_marker),lbl_text);
	out_buf[2] = (char)gtk_range_get_value(GTK_RANGE(widget));
	g_free(lbl_text);
}

void on_da_clicked( GtkWidget *widget,GdkEventButton *event, app_widgets *data){
	g_print("drawing area clicked");
	if (event->button == 1) {
        	glob.coordx[glob.count] = event->x;
        	glob.coordy[glob.count++] = event->y;
 	}
}
void clicked(GtkWidget *widget, GdkEventButton *event,
    app_widgets *data)

{
	char *lbl_text;
	//g_print("test success clicked");
    if (event->button == 1) {
        glob.coordx[glob.count] = event->x;
        glob.coordy[glob.count++] = event->y;
	lbl_text = g_strdup_printf ("<span font=\"10\" color=\"black\">" "X: %2.f  Y: %2.f""</span>",event->x,event->y);
	//g_print(lbl_text);
	gtk_label_set_markup(GTK_LABEL(data->w_lbl_marker),lbl_text);
	//gtk_widget_queue_draw(data->w_lbl_marker);
    }
	

}


void on_DSO_screen_button_press_event( GtkWidget *widget,GdkEventButton *event, app_widgets *data){
	if (event->button == 1) {
		glob.coordx[glob.count] = event->x;
		glob.coordy[glob.count++] = event->y;
	}
}


void on_btnRedraw_clicked( GtkButton *widget, app_widgets *data ){
	g_print("from redraw button handler \n");

	/*____________sends a signal to invalidate the contents of drawing area
	 * ___________this forces the draw signal to be emitted from drawing area
	 * ___________handler for draw event (on_draw_event) runs and updates the
	 * ___________drawing area contents____________________________________*/
	gtk_widget_queue_draw(data->w_drawing_area);

	/*notice that the async_plotter_thread waits for new data and calls this
	 * function to automatically update ui on recieving data from MCU______*/


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

    pthread_create(&plotter_t_id,NULL,&async_plotter_thread,data);
    //pthread_join(server_t_id,NULL);
    return(TRUE);
}


void* async_plotter_thread(void *arg){
   int i = 0;
   data_pointer ui_pointer;
   ui_pointer = (data_pointer)arg;
   while(1){
        printf("starting to plot \n");
	pthread_mutex_lock(&lock_buf);
	printf("shared data locked by plotter thread\n");
	#if defined DEBUG
		for(i = 0;i<BUFSIZE;i++){
			g_print("%d\n",in_buf[i]);
		}
	#endif
	gtk_widget_queue_draw(ui_pointer->w_drawing_area);
	pthread_mutex_unlock(&lock_buf);
	printf("shared buf ready to recieve new data\n");
	//suspend plotting till new data arrives 
	//pthread_mutex_lock(&lock_plotter);
	printf("waiting for new data .....\n");
	pthread_cond_wait(&suspend_plotting,&lock_plotter);
   }
   pthread_exit(NULL);
}
