/* xplan - project planning tool
 * Copyright (C) 1992 Brian Gaubert, Mark M. Lacey, Richard Malingkas,
 * and Mike Marlow.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License (distributed with this program in the file
 * COPYING) for more details.
 * 
 * If you did not received a copy of the GNU General Public License
 * along with this program, write to the Free Software Foundation,
 * Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 * Since this was a project for a one semester software engineering
 * course, the authors will not be offering support for the product
 * after its release
 *-----------------------------------------------------------------------
 * FILE gantt_Xstubs.c *************************************************
 *  
 *  DESCRIPTION OF CONTENTS
 *  
 *   This file contains the functions developed by guide, gantt_resize
 *   and gantt_repaint, but it also contains the function gantt_chart
 *   draw. Since guide suplies the comments for its code I will only
 *   comment my code 
 *  
 * FILE gantt_Xstubs.c */   


/*
 * gantt_stubs.c - Notify and event callback function stubs.
 * This file was generated by `gxv' from `gantt.G'.
 *
 *  Include all of the files needed for the gantt_chart_draw function
 */

#include <stdio.h>
#include <sys/param.h>
#include <sys/types.h>
#include <xview/xview.h>
#include <xview/panel.h>
#include <xview/textsw.h>
#include <xview/xv_xrect.h>
#include "db.h"
#include "xplan.h"

/*
 * Define all of the constants that will be used in the
 * gantt_chart_draw function
 *
 */
#define G_SETUP_L 30
#define G_SETUP_W 20
#define START_WEEK 50
#define TASK_N 50
#define XOFFSET 5;
#define YOFFSET 10;
#define BAR_HEIGHT 15;
void gantt_chart_draw(Display *display, Window xid);

char *julian_to_str_date(int);

/*
 * Repaint callback function for `canvas'.
 */
void
gantt_repaint(Canvas canvas, Xv_window paint_window, Display *display, Window xid, Xv_xrectlist *rects)
{
	gantt_chart_draw(display, xid);
	
	/* gxv_start_connections DO NOT EDIT THIS SECTION */

	/* gxv_end_connections */

}

/*
 * Resize callback function for `canvas'.
 */
void
gantt_resize(Canvas canvas, int width, int height)
{
	
	/* gxv_start_connections DO NOT EDIT THIS SECTION */

	/* gxv_end_connections */

}


/* FUNCTION gantt_chart_draw() ********************************************
 *
 *  PURPOSE
 *
 *   This function, like the generate_gant_chart will produce gantt charts
 *   using xlib functions and will be graphically displayed.
 *
 *  SAMPLE CALL
 *
 *  gantt = gantt_chart_draw();
 *   
 *  INPUTS
 *
 *   none.
 *
 *  OUTPUTS
 *
 *   A graphical representation of the gantt chart on the canvis that it
 *   is assigned to.
 *
 *  AUTHOR/AUDITOR/TESTER
 *
 *  Author.... Brian Gaubert Dec, 2, 1992
 *  Tester.... Brian Gaubert Dec, 2, 1992
 *
 *  MODIFICATIONS (most recent to least)
 *
 * FUNCTION gantt_chart_draw() */

void gantt_chart_draw(Display *display, Window xid)
{


   /* initialize all of the variables that are to be used in this */
   /* function */
   GC gc;
   struct task_node *temp;
   struct task_node *temp2;
   int task_num = 0;
   int number_of_task = 0;
   int x = 0;
   int y = 0;
   int total_length = 0;
   int i;
   int j;
   int day_of_week = 0;
   char *temp_date;
   int start_of_task = 0;
   int end_of_task = 0;
   int next_date = 0;
   char tmp[10];
   int temp_x = 0;
   int temp_y = 40;
   int length = 50;
   int horizonal_length = 90000;
   int first_date = 0;
   int last_date = 0;
   
   /* This initialization of the two dem. array will be used in printing
      the days of the week to a page in the charts. The reason for the
      same data twice in a row is because when the modulus command is used
      on the julian date, the the range of values is from 0 to 6, and
      if the number that you get when you do the modulus is six then
      six + 1 etc. will give the next date in the week */      
   char weekday[14][4] = {"Mon","Tue","Wed","Thr","Fri","Sat","Sun",\
			   "Mon","Tue","Wed","Thr","Fri","Sat","Sun"};
   char crit[5];

   /* Get the default graphics content */
   gc = DefaultGC(display, DefaultScreen(display));

   temp = get_main_task_list()->head;
   if (temp == NULL) return;

   /* Set x to the start of the week */
   x = START_WEEK;

   /* Get the first date, the last date and the total days of the */
   /* project */   
   first_date = temp->data->planned_start_date;
   while( temp != NULL ) {
	last_date = temp->data->planned_end_date;
	number_of_task++;
        temp = temp->next;
     } 
   total_length = abs(last_date - first_date);

   /* Find out what day the first task starts on */
   day_of_week = first_date % 7;

   strcpy(crit,"C");
   
   /* draw the rectangles for the top of the screen  and fill in the */
   /* days of the week, and fill in the date that the project started */
   /* and all dates that are seven + days after the start of the */
   /* project Show this on the screen within boxes */
   for(i=0;i<=total_length/7 + 1;i++) {
      temp_date = julian_to_str_date(first_date+next_date);
      XDrawString(display, xid, gc, x,y + 10,\
		  temp_date,\
		  strlen(temp_date));
      next_date = next_date + 7;
      for(j=0;j<=6;j++) {
	 XDrawRectangle(display, xid, gc, x,y + 10, 30, 30);
	 XDrawString(display, xid, gc, (x+3),(y+27),\
		     weekday[(day_of_week+j)],\
		     strlen(weekday[(day_of_week+j)]));
	 x = x + 30;
      }
   }

   /* Find the critical path */
   calc_critical();
   

   /* This section of code will determine what information */
   /* goes on the screen and will write that information to */
   /* the screen. If the */
   /* start and end dates of the task are equal then ist is */
   /* assumed to be a milestone and a diamond will be written */
   /* to the screen. A diamond will also be written to the screen */
   /* if the milestone was selected by the user, but all */
   /* other information will be xlib format to generate bars */
   /* on the screen */
   temp2 = get_main_task_list()->head;
   while(temp2 != NULL) {
      start_of_task = temp2->data->planned_start_date - first_date;
      end_of_task = temp2->data->planned_end_date - first_date;
 
      /* If milestone display diamond */
      if ((end_of_task - start_of_task) == 0) {
	 XDrawLine(display, xid, gc, start_of_task*30 + 50,\
		   task_num*30 + 70,start_of_task*30 + 43,\
		   task_num*30 + 63);
	 XDrawLine(display, xid, gc, start_of_task*30 + 50,\
		   task_num*30 + 70,start_of_task*30 + 57,\
		   task_num*30 + 63);
	 XDrawLine(display, xid, gc, start_of_task*30 + 50,\
		   task_num*30 + 56,start_of_task*30 + 43,\
		   task_num*30 + 63);
	 XDrawLine(display, xid, gc, start_of_task*30 + 50,\
		   task_num*30 + 56,start_of_task*30 + 57,\
		   task_num*30 + 63);
      }
      else {

	 /* Display the gantt bar for the task */
	 XDrawRectangle(display, xid, gc, start_of_task*30 + 50,
			(task_num*30) + 40 + 15,
			(end_of_task-start_of_task)*30, 15);
	 
	 /* If milestone display diamond */
	 if (temp2->data->milestone == 1) {
	    XDrawLine(display, xid, gc, end_of_task*30 + 50,\
		      task_num*30 + 70,end_of_task*30 + 43,\
		      task_num*30 + 63);
	    XDrawLine(display, xid, gc, end_of_task*30 + 50,\
		      task_num*30 + 70,end_of_task*30 + 57,\
		      task_num*30 + 63);
	    XDrawLine(display, xid, gc, end_of_task*30 + 50,\
		      task_num*30 + 56,end_of_task*30 + 43,\
		      task_num*30 + 63);
	    XDrawLine(display, xid, gc, end_of_task*30 + 50,\
		      task_num*30 + 56,end_of_task*30 + 57,\
		      task_num*30 + 63);
	    
	 }

	 /* Put a C in hte bar if it is part of the critical path */
	 if (temp2->data->critical_path) {
	    XDrawString(display, xid, gc, ((start_of_task*30 + 50) +
					   (end_of_task -start_of_task)*30 -
					   15),task_num*30 + 68,crit,strlen(crit));	    
	 }
      }
         /* Display the tasks and numbers on the left side of the */
	 /* screen */
	 task_num++;
	 sprintf(tmp,"Task %d",task_num);   
	 XDrawRectangle(display, xid, gc, temp_x,temp_y,\
			TASK_N, 30);
	 XDrawString(display, xid, gc, temp_x + 3,temp_y+20,
		     tmp,strlen(tmp));
	 temp_y = temp_y + 30;
	 temp2 = temp2->next;
   }
 
}

   


