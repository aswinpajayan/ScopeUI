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
#include <semaphore.h>
#include <pthread.h>

pthread_t plotter_t_id;

void* async_plotter_thread(void *arg);
G_MODULE_EXPORT gboolean
on_btnRedraw_clicked( GtkWidget      *widget,
                 GdkEventExpose *event,
                 ChData         *data )
{
    g_print("from chart_expose");
	/* Return TRUE, since we handled this event */
    return( TRUE );
}


G_MODULE_EXPORT gboolean
on_btnConnect_clicked( GtkWidget *widget, GdkEventExpose *event, ChData *data ){
    char port_number[5];	
    g_print("from btnConnect");

    sem_init(&new_data,0,1);  //initialise the semaphore 
    /*this semaphore will report presence of new data*/

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
   while(1){
	if(plotting_completed)
		wait();
	int i = 0;
	pthread_mutex_lock(&lock_buf);
	for(i = 0;i<BUFSIZE;i++){
		g_print("%d \t",in_buf[i]);
	}
	pthread_mutex_unlock(&lock_buf);
	plotting_completed = 1;
   }
   pthread_exit(NULL);
}

