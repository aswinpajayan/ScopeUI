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
on_btnConnect_clicked( GtkWidget      *widget,
                 GdkEventExpose *event,
                 ChData         *data )
{
    int port_number;	
    g_print("from btnConnect");
    g_print("initialising UDP_server");
    //test initialisation of command buffer 
      	memcpy(out_buf,"TEST_COMMAND",12);
        
      	port_number = 50001;   //#define PORT_NUMBER 50001                                                         
	pthread_create(&server_t_id,NULL,&socketThread,&port_number);
        //pthread_join(server_t_id,NULL);
	return(TRUE);
	}
