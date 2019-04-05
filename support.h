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

/* Convenience macros for obtaining objects from UI file */
#define CH_GET_OBJECT( builder, name, type, data ) \
    data->name = type( gtk_builder_get_object( builder, #name ) )
#define CH_GET_WIDGET( builder, name, data ) \
    CH_GET_OBJECT( builder, name, GTK_WIDGET, data )

/* Main data structure definition */
typedef struct _ChData ChData;
struct _ChData
{
    /* Widgets */
    GtkWidget *main_window;  /* Main application window */
    GtkWidget *chart_area;   /* Chart drawing area */
};

#endif /* __SUPPORT_H__ */
