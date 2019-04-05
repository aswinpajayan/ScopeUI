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
    g_print("from btnConnect");
	/* Return TRUE, since we handled this event */
    return( TRUE );
}
