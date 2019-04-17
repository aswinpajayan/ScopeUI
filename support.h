/*
 * =====================================================================================
 *
 *       Filename:  support.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  Thursday 04 April 2019 03:32:58  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aswin P Ajayan (), aswinp2008@gmail.com
 *   Organization:  iitb
 *
 * =====================================================================================
 */
#ifndef __SUPPORT_H__
#define __SUPPORT_H__

#include <gtk/gtk.h>


//data structure to pass widgets to callback functions
typedef struct{
	GtkWidget *w_drawing_area;
	GtkWidget *w_scale;
}app_widgets;

/* Convenience macros for obtaining objects from UI file */
#define CH_GET_OBJECT( builder, name, type, data ) \
    data->name = type( gtk_builder_get_object( builder, #name ) )
#define CH_GET_WIDGET( builder, name, data ) \
    CH_GET_OBJECT( builder, name, GTK_WIDGET, data )


#endif /* __SUPPORT_H__ */
