/*
 * $Id: calentool.c,v 2.4 1995/07/02 22:13:06 ian Exp $
 */
/*
 * calentool.c
 *
 * calentool - day/week/month/year-at-a-glance calendar for XView/Open Look
 * 
 * Original suntool source Copyright (C) 1987, Sun Microsystems, Inc.
 * 	All Rights Reserved
 * Permission is hereby granted to use and modify this program in source
 * or binary form as long as it is not sold for profit and this copyright
 * notice remains intact.
 * Original author: Philip Heller, Sun Microsystems, Inc.
 * 
 * All additional software, enhancements and modifications are
 * Copyright 1988, 1989, 1991, 1994 by Tektronix, Inc. - All Rights Reserved.
 * 
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Tektronix, Inc. not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.
 * 
 * TEKTRONIX INCORPORATED MAKES NO REPRESENTATIONS ABOUT THE
 * SUITABILITY OF THIS SOFTWARE FOR ANY PURPOSE.  IT IS PROVIDED "AS IS"
 * WITHOUT EXPRESS OR IMPLIED WARRANTY.  TEKTRONIX INCORPORATED
 * DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO
 * EVENT SHALL TEKTRONIX INCORPORATED BE LIABLE FOR ANY SPECIAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 * 
 * Author: Bill Randle, Tektronix, Inc. <billr@saab.cna.tek.com>
 */ 
#include <stdio.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <xview/xview.h>
#include <xview/panel.h>
#undef ROUNDUP  /* to avoid multiply declared define */
#include <xview/seln.h>
#include <xview/svrimage.h>
#include <xview/font.h>
#include <xview/cursor.h>
#include <xview/cms.h>
#include <xview/defaults.h>
#include <xview/rect.h>
#include <sys/time.h>
#include <sys/file.h>
#include <signal.h>
#include <ctype.h>
#include "ct.h"

extern Xv_Font font, sfont;
extern char apts_pathname[], tmpapts_pathname[];
extern char clockstr[];
extern char *smonthnames[];
extern Panel_item clock_pi;
extern Frame fframe;
extern Panel panel;
extern int day_is_open;
#ifndef NO_SUN_MOON
extern Frame sframe;
#endif

Frame frame = 0;
struct tm today, current;
struct tm First, Last;
struct tm olddate, closedate;
int mainsw_state, selected_type;
int x_coord, y_coord, startx, starty;
int read_only;
int nr_weekdays, dayslot_width, day_message_size, n_tslots, n_slots;
int week_message_size, weekslot_width, weekslot_height, dayslot_height;
int ybox_height, ybox_width;
struct dayslot *slots;
struct rect_limits boxlims[31];
struct rect_limits mboxlims[12];
struct week_arrow week_arrows[6];
struct weekrect week_boxes[7];
Rect *closed_rect, image_rect, label_rect;
char *mailto = NULL;
int one_based = 0, new_entry = 0, version2 = 0;
int otherfile = 0;
char *othername;
int beep = 0, show_time = 0, beep_open = 0;
int include_old = 0, save_old = 0;
#ifndef NO_HOLIDAYS
int holiday_a = 0, holiday_c = 0, holiday_i = 0;
int holiday_j = 0, holiday_s = 0;
#endif
char *progname;
struct itimerval cal_timer;
int update_interval = 0;
int show_future = 1;
Server_image year_cursor_image, month_cursor_image;
Server_image week_cursor_image, day_cursor_image, wait_cursor_image;
Xv_Cursor year_cursor, month_cursor, week_cursor, day_cursor;
Xv_Cursor wait_cursor;
char orig_apts_pathname[160];
char timestr[16], datestr_day[3];
char printer[64];
struct pr_prpos where;
int working_msg;
int hour24 = 0, monday_first = 0, day_first = 0;
int expire_days = 0;
int start_hour, end_hour, num_notes;
int print_dev = PR_DEFAULT;
int appt_check_limit, week_ofs = 0;
int locked = 0;
int limitcheck = -1;
int orig_ro;
Seln_client s_client;
Xv_server server;
Display *mydisplay;
Drawable drawable, drawable2;
int screen;
GC gc, gcc, gccs;
XGCValues gc_val;
unsigned long foregr, backgr;
XFontStruct *xfont;
Window m_root;
Pixmap iconPixmap, icon, rev_icon, na_icon;
int m_x, m_y;
unsigned int m_width, m_height;
unsigned int m_border, m_depth;
unsigned int g_width, g_height;
XClassHint WMClassHints;
XWMHints *myWMHints;
XPoint ilabel = {0, 74};
XSetWindowAttributes frame_xattr;
Colormap def_cmap;
XColor def_color;
Xv_singlecolor ct_back, ct_fore;
unsigned long black_pix, white_pix;
Cms cms;
Xv_cmsdata *ocms_data;
Xv_singlecolor cmscolors[CMS_CONTROL_COLORS+5];
Defaults_pairs holiday_pairs[] = { {"all",1}, {"day",2}, {NULL,0} };
Defaults_pairs displayMode_pairs[] = {
	{"day",DISPLAYING_DAY}, {"week",DISPLAYING_WEEK},
	{"month",DISPLAYING_MONTH}, {"year",DISPLAYING_YEAR},
	{NULL,DISPLAYING_DAY}
};
Defaults_pairs prfile_pairs[] = { {"printer",0}, {"file",1}, {NULL,0} };
int xposition, yposition;
XSizeHints sizehints;
XWindowAttributes attr;
XSetWindowAttributes icon_attr;
Window icon_window;
char userStdPixmap[256] = "\0";
char userNapPixmap[256] = "\0";
char userRevPixmap[256] = "\0";
int xicon, yicon, icon_pos = 0;
int stipple_day = 1;
int print_trailer = 0;

#include "std_icon.xbm"
#include "rev_icon.xbm"
#include "nap_icon.xbm"

static short year_cursor_data[] = {
#include "year.cursor"
};
static short month_cursor_data[] = {
#include "month.cursor"
};
static short week_cursor_data[] = {
#include "week.cursor"
};
static short day_cursor_data[] = { 
#include "day.cursor"
};
static short wait_cursor_data[] = {
#include <images/hglass.cursor>
};

#if 0
#ifndef NO_SUN_MOON
static short moon_icon_data[] = {
#include "moony.icon"
};
static short sun_icon_data[] = {
#include "sunny.icon"
};
#endif
#endif

#ifdef __STDC__
static Notify_value itimer_handler(Notify_client me, int which);
Notify_value leave(Notify_client me, int signal, Notify_signal_mode when);
Notify_value myframe_interposer(Notify_client client, Event *event, Notify_arg arg, Notify_event_type type);
void sel_func_key_proc(Xv_opaque client_data, Seln_function_buffer *args);
Seln_result sel_reply_proc(Seln_attribute item, Seln_replier_data *context, int length);
int XIOError(Display *mydisplay), XError(Display *mydisplay, XErrorEvent *myerror);
void usage(void);
void def_to_xvsc(char *def, Xv_singlecolor *sc, long unsigned int *pixel);
void get_user_defaults(void);
void set_fgbg (void);
void create_icon (char *filename, Pixmap iconPix);
void set_cms(void);
#else
static Notify_value itimer_handler();
Notify_value leave();
Notify_value myframe_interposer();
void sel_func_key_proc();
Seln_result sel_reply_proc();
int XIOError(), XError();
void usage();
void def_to_xvsc();
void get_user_defaults();
void set_fgbg ();
void create_icon ();
void set_cms();
#endif

void main(argc,argv)
int argc;
char *argv[];
{
	int  flag, i;
	int  printit = 0, toolrunning = 1;
	int  printit_dst = 0;
	int  df_set = 0;
	int  win_height;
	char buff[128];
	extern char *optarg;
	
	if ((progname = rindex(*argv, '/')))
		progname++;
	else
		progname = *argv;

	get_today();	/* initial day is today */
	current = today;
	closedate = today; /* in case we started iconic */
	read_only = 0;
	working_msg = 0;

	/*
	 * Check for -p, -P, -m or -M option, which means we don't want
	 * to create the tool.
	 */
	i = argc;
	while (--i > 0) {
		if (!strncmp(argv[i], "-p", 2) || !strncmp(argv[i], "-m", 2)
		    || !strncmp(argv[i], "-P", 2) || !strncmp(argv[i], "-M", 2)) {
			/* standard getopt doesn't allow optional
			 * arguments, so we do a check here for
			 * obsolete usage of the -p option
			 */
			if ((argv[i][1] == 'p' || argv[i][1] == 'P') &&
				argv[i][2] == '\0') {
				fprintf(stderr, "%s: -p/-P requires a 'd', 'w' or 'm' argument\n",
					progname);
				exit(1);
			}
			toolrunning = 0;
		}
	}
	if (toolrunning) {
		/* user did not specify a -[pP] or -[mM] option */
		/*
		 * hack alert: it should be possible to use the return from 'xv_init'
		 * to determine if X is running, but it dies instead. So, we try and
		 * open the display using a direct Xlib call to see if the server
		 * can be accessed.
		 */
		if ((mydisplay = XOpenDisplay(NULL)) == NULL) {
			/* can't open up the display */
			if (argc > 1 && strncmp(argv[1], "-?", 2)==0)
				usage();
			else {
				fprintf(stderr, "%s: X server not available.\n", progname);
				fprintf(stderr, "%s: one of -p, -P, -m or -M options must be specified. Use -? for help.\n",
					progname);
				exit(1);
			}
		} else {
			/* it worked */
			XCloseDisplay(mydisplay);
		}
		/* 
		** Parse args: xv_init must be called before getopt, so
		** it can interpret and extract flags for XView.
		*/
		server = (Xv_server)xv_init(XV_INIT_ARGC_PTR_ARGV, &argc, argv,
				XV_USAGE_PROC, usage, NULL);
		if (server == 0) {
			fprintf(stderr, "%s: can't conect to server, bailing out.\n", progname);
			exit(1);
		}
		mydisplay = (Display *)xv_get(server, XV_DISPLAY);
		screen = DefaultScreen(mydisplay);
		/* set foreground and background colors */
		set_fgbg();

		frame = xv_create(0, FRAME,
				XV_NAME,	"calentool",
				FRAME_LABEL, version(),
				FRAME_BACKGROUND_COLOR, &ct_back,
				FRAME_FOREGROUND_COLOR, &ct_fore,
				FRAME_INHERIT_COLORS, TRUE,
				WIN_ERROR_MSG,
				"Error opening display. Is DISPLAY set?",
				0);
		if (frame == 0)
			exit(1);
		err2console(TRUE); /* force error msgs to console */
		/* pick out some X data from the base frame for later use */
		drawable = (Drawable)xv_get(frame, XV_XID);
		frame_xattr.backing_store = WhenMapped;
		XChangeWindowAttributes(mydisplay, drawable,
				CWBackingStore, &frame_xattr);
		/* DEBUG */
		/**
		XSynchronize(mydisplay, 1);
		**/
		if ((s_client = selection_create(server, sel_func_key_proc, sel_reply_proc, NULL)) == NULL)
			err_rpt("Can't create selection svc client", NON_FATAL);
	}

	get_user_defaults();	/* these can be overriden by the command line */
	while ((flag = getopt(argc, argv, "1:2:567bBd:D:eEf:iI:l:m:M:op:P:rtu:zH:h:wx:T:s:S:N:")) != EOF)
		switch (flag) {   
		    case 'f':	/* use this file */
			otherfile = 1;
			othername = optarg;
			break;
			
		    case 'd':	/* starting date */
			(void)parse_date(optarg, TRUE);
			break;

		    case 'D':	/* date format */
			switch (*optarg) {
				case '0':
					day_first = 0;
					df_set = 1;
					break;
				case '1':
					day_first = 1;
					df_set = 1;
					break;
				case '2':
					day_first = 2;
					df_set = 1;
					break;
			}
			break;
		    case 'r':	/* read only file */
			read_only = 1;
			break;
			
		    case '1':	/* -12 -- 12 hour time */
			if (*optarg == '2')
				hour24 = FALSE;
			break;

		    case '2':	/* -24 -- 24 hour time */
			if (*optarg == '4')
				hour24 = TRUE;
			break;

		    case 'e':	/* European calendar Mon-Sun */
			monday_first = TRUE;
			break;

		    case 'E':	/* European style */
			monday_first = TRUE;
			hour24 = TRUE;
			if (!df_set)
				day_first = TRUE;
			break;

		    case 'I':	/* initial state */
			switch (*optarg) {
				case 'd':
				case 'D':
					mainsw_state = DISPLAYING_DAY;
					break;
				case 'w':
				case 'W':
					mainsw_state = DISPLAYING_WEEK;
					break;
				case 'm':
				case 'M':
					mainsw_state = DISPLAYING_MONTH;
					break;
				case 'y':
				case 'Y':
					mainsw_state = DISPLAYING_YEAR;
					break;
				default:
					fprintf(stderr, "%s: unknown startup option.", progname);
					break;
			}
			break;

		    case 'p':	/* print and exit */
			read_only |= 0x2;
			switch (*optarg) {
				case 'd':
				case 'D':
					printit = PRI_DAY;
					break;
				case 'w':
					printit = PRI_WEEK;
					week_ofs = 0;
					break;
				case 'W':
					printit = PRI_WEEK;
					week_ofs = 1;
					break;
				case 'm':
				case 'M':
					printit = PRI_MONTH;
					break;
				default:
					fprintf(stderr, "%s: unknown print option.", progname);
					break;
			}
			if (!printit_dst)
				printit_dst = DST_STDOUT;
			break;

		    case 'P':	/* print and exit (ignore some notes) */
			read_only |= 0x2;
			switch (*optarg) {
				case 'd':
				case 'D':
					printit = PRI_DAY_XNOTES;
					break;
				case 'w':
					printit = PRI_WEEK_XNOTES;
					week_ofs = 0;
					break;
				case 'W':
					printit = PRI_WEEK_XNOTES;
					week_ofs = 1;
					break;
				case 'm':
				case 'M':
					printit = PRI_MONTH_XNOTES;
					break;
				default:
					fprintf(stderr, "%s: unknown print option.", progname);
					break;
			}
			if (!printit_dst)
				printit_dst = DST_STDOUT;
			break;

		    case 'l':	/* appt check limit */
			if (optarg)
				limitcheck = atoi(optarg);
			else
				limitcheck = 0;
			break;

		    case 'm':	/* mail today's appts and exit */
			read_only = 1;
			if (!printit)
				printit = PRI_DAY;
			printit_dst = DST_MAIL;
			mailto = optarg;
			break;

		    case 'M':	/* mail today's appts and exit (ignore
				   some notes) */
			read_only |= 0x2;
			if (!printit)
				printit = PRI_DAY_XNOTES;
			printit_dst = DST_MAIL;
			mailto = optarg;
			break;

		    case 'b':	/* beep to console for pending appt */
			beep = 1;
			break;

		    case 'B':	/* beep then open wondow */
			beep_open = 1;
			break;

		    case 'i':	/* include old appt files */
			include_old = 1;
			break;

		    case 'o':	/* save outdated appts to another file */
			save_old = 1;
			break;

		    case 't':	/* display current time under icon */
			show_time = 1;
			break;

		    case '5':	/* Mon - Fri week display */
			nr_weekdays = 5;
			break;

		    case '6':	/* Mon - Sat week display */
			nr_weekdays = 6;
			break;

		    case '7':	/* Sun - Sat (or Mon-Sun) week display */
			nr_weekdays = 7;
			break;

		    case 'u':	/* update interval (in seconds) */
			update_interval = atoi(optarg);
			break;

		    case 'w':	/* display Working! message */
			working_msg = 1;
			break;

		    case 'x':	/* eXpire appts after so many days */
			expire_days = atoi(optarg);
			break;

		    case 'z':	/* zero offset -- new style appts file */
			one_based = 1;
			break;

#ifndef NO_HOLIDAYS
		    case 'h':
			/* show certain holidays */
			switch (*optarg) {
				case 'A':
					/* all holidays */
					holiday_a = holiday_c = holiday_i = 1;
					holiday_j = holiday_s = 1;
					break;
				case 'a':
					/* astronomical events */
					holiday_a = 1;
					break;
				case 'c':
					/* Christian holidays */
					holiday_c = 1;
					break;
				case 'i':
					/* Islamic holidays */
					holiday_i = 1;
					break;
				case 'j':
					/* Jewish holidays */
					holiday_j = 1;
					break;
				case 's':
					/* secular holidays */
					holiday_s = 1;
					break;
				default:
					fprintf(stderr, "%s: unknown holiday option (must be one of \"Aacijs\")", progname);
					break;
			}
			break;

		    case 'H':
			/* show certain holidays day/week display only */
			switch (*optarg) {
				case 'A':
					/* all holidays */
					holiday_a = holiday_c = holiday_i = 2;
					holiday_j = holiday_s = 2;
					break;
				case 'a':
					/* astronomical events */
					holiday_a = 2;
					break;
				case 'c':
					/* Christian holidays */
					holiday_c = 2;
					break;
				case 'i':
					/* Islamic holidays */
					holiday_i = 2;
					break;
				case 'j':
					/* Jewish holidays */
					holiday_j = 2;
					break;
				case 's':
					/* secular holidays */
					holiday_s = 2;
					break;
				default:
					fprintf(stderr, "%s: unknown holiday option (must be one of \"Aacijs\")", progname);
					break;
			}
			break;
#else
		    case 'h':
		    case 'H':
			    fprintf(stderr, "%s: -h and -H options not available\n", progname);
			    break;
#endif
		    case 'T':
			if (!strncmp(optarg, "ps", 2))
				print_dev = PR_POSTSCRIPT;
			else
				print_dev = PR_ASCII;
			break;

		    case 's':	/* start hour */
			start_hour = atoi(optarg);
			if (start_hour < 0 || start_hour > 23)
				fprintf(stderr, "%s: start hour must be in the range 0 - 23\n", progname);
			break;

		    case 'S':	/* Stop (end) hour */
			end_hour = atoi(optarg);
			if (end_hour < 1 || end_hour > 24)
				fprintf(stderr, "%s: end hour must be in the range 1 - 24\n", progname);
			break;

		    case 'N':	/* number of note slots */
			num_notes = atoi(optarg);
			break;

		    case '?':
		    default:
			usage();
		}

	if (toolrunning) {
		(void) notify_set_signal_func(frame, leave, SIGHUP, NOTIFY_ASYNC);
		(void) notify_set_signal_func(frame, leave, SIGINT, NOTIFY_ASYNC);
		(void) notify_set_signal_func(frame, leave, SIGTERM, NOTIFY_ASYNC);
		/* slimy - capture I/O errors so we can clean up! */
		XSetIOErrorHandler(XIOError);
		XSetErrorHandler(XError);
	}

	/* set default printer to something useful */
	if (print_dev == PR_DEFAULT) {
		if (printit == PRI_MONTH || printit == PRI_MONTH_XNOTES)
			print_dev = PR_POSTSCRIPT;
		else
			print_dev = PR_ASCII;
	}
	/*
	** setup number of slots and allocate memory for them
	*/
	if ( start_hour >= end_hour )
		err_rpt("Start Hour must be less than Stop Hour", FATAL);
	n_tslots = (end_hour - start_hour) * 2;
	n_slots = n_tslots + num_notes;
	appt_check_limit = APPT_CHECK_LIMIT;  /* set default */
	if (limitcheck == 0)
		appt_check_limit = n_tslots;
	else if (limitcheck == 1)
		appt_check_limit = n_slots;
	/* make room for n_slots dayslot entries and week entries */
	if ((slots = (struct dayslot *)malloc(n_slots* sizeof(struct dayslot))) == NULL)
		err_rpt("Can't get enough storage for day slots", FATAL);
	for (i=0; i<nr_weekdays; i++)
		if ((week_boxes[i].weekslots = 
		    (struct dayslot *)malloc(n_slots*sizeof(struct dayslot))) == NULL)
			err_rpt("Can't get enough storage for week slots", FATAL);

	/* 
	** Find the calendar file, etc.
	*/
	if (do_files(toolrunning))
		/* can't open (or create) appts file */
		exit(1);
	strcpy(orig_apts_pathname, apts_pathname);
	orig_ro = read_only;
	
	if (printit) {
		print_apts(printit, printit_dst);
		if ((save_old || expire_days) && !(read_only & 0x1)) {
			read_only = 0;  /* allow updates */
			expire(expire_days);
		}
		exit(0);
	}

	get_fonts();

	if (!update_interval) {
		if (!strcmp(UPDATE_RATE, "minute"))
			update_interval = 60;
		else if (!strcmp(UPDATE_RATE, "second"))
			update_interval = 1; /* check in 1 sec */
		else {
			err_rpt("unknown update interval, defaulting to seconds", NON_FATAL);
			update_interval = 1; /* check in 1 sec */
		}
	}

	/* create the cursors */
	year_cursor_image = xv_create(XV_NULL, SERVER_IMAGE,
		XV_WIDTH,	16,
		XV_HEIGHT,	16,
		SERVER_IMAGE_BITS,	year_cursor_data,
		0);
	year_cursor = xv_create(XV_NULL, CURSOR,
			CURSOR_IMAGE, year_cursor_image,
			CURSOR_XHOT, 7, CURSOR_YHOT, 7,
			CURSOR_OP, PIX_SRC ^ PIX_DST,
			0);

	month_cursor_image = xv_create(XV_NULL, SERVER_IMAGE,
		XV_WIDTH,	16,
		XV_HEIGHT,	16,
		SERVER_IMAGE_BITS,	month_cursor_data,
		0);
	month_cursor = xv_create(XV_NULL, CURSOR,
			CURSOR_IMAGE, month_cursor_image,
			CURSOR_XHOT, 7, CURSOR_YHOT, 7,
			CURSOR_OP, PIX_SRC ^ PIX_DST,
			0);

	week_cursor_image = xv_create(XV_NULL, SERVER_IMAGE,
		XV_WIDTH,	16,
		XV_HEIGHT,	16,
		SERVER_IMAGE_BITS,	week_cursor_data,
		0);
	week_cursor = xv_create(XV_NULL, CURSOR,
			CURSOR_IMAGE, week_cursor_image,
			CURSOR_XHOT, 7, CURSOR_YHOT, 7,
			CURSOR_OP, PIX_SRC ^ PIX_DST,
			0);

	day_cursor_image = xv_create(XV_NULL, SERVER_IMAGE,
		XV_WIDTH,	16,
		XV_HEIGHT,	16,
		SERVER_IMAGE_BITS,	day_cursor_data,
		0);
	day_cursor = xv_create(XV_NULL, CURSOR,
			CURSOR_IMAGE, day_cursor_image,
			CURSOR_XHOT, 7, CURSOR_YHOT, 7,
			CURSOR_OP, PIX_SRC ^ PIX_DST,
			0);

	wait_cursor_image = xv_create(XV_NULL, SERVER_IMAGE,
		XV_WIDTH,	16,
		XV_HEIGHT,	16,
		SERVER_IMAGE_BITS,	wait_cursor_data,
		0);
	wait_cursor = xv_create(XV_NULL, CURSOR,
			CURSOR_IMAGE, wait_cursor_image,
			CURSOR_XHOT, 7, CURSOR_YHOT, 7,
			CURSOR_OP, PIX_SRC ^ PIX_DST,
			0);

	/* get window depth for creating icons */
	XGetWindowAttributes(mydisplay, xv_get(frame, XV_XID), &attr);

	if ((icon = XCreatePixmapFromBitmapData(mydisplay, DefaultRootWindow(mydisplay),
			std_icon_bits, 64, 64,
			foregr, backgr, attr.depth)) == None)
		err_rpt("Can't create icon Pixmap", FATAL);

	if ((rev_icon = XCreatePixmapFromBitmapData(mydisplay, DefaultRootWindow(mydisplay),
			rev_icon_bits, 64, 64,
			foregr, backgr, attr.depth)) == None)
		err_rpt("Can't create rev_icon Pixmap", FATAL);

	if ((na_icon = XCreatePixmapFromBitmapData(mydisplay, DefaultRootWindow(mydisplay),
			nap_icon_bits, 64, 64,
			foregr, backgr, attr.depth)) == None)
		err_rpt("Can't create na_icon Pixmap", FATAL);

	/* create a generic Pixmap for the current icon image */
	iconPixmap = XCreatePixmap(mydisplay, DefaultRootWindow(mydisplay),
		64, (show_time ? 77 : 64), attr.depth);

	/* build a GC for the icon */
	xfont = (XFontStruct *)xv_get(sfont, FONT_INFO);
	gc_val.foreground = foregr;
	gc_val.background = backgr;
	gc_val.function = GXcopy;
	gc_val.font = xfont->fid;
	gc = XCreateGC(mydisplay, iconPixmap,
		GCBackground|GCForeground|GCFunction|GCFont,
		&gc_val);

	/* change default values for future gc's */
	xfont = (XFontStruct *)xv_get(font, FONT_INFO);
	gc_val.font = xfont->fid;
	gc_val.fill_style = FillOpaqueStippled;

	/* maybe overwrite default icon pixmaps */
	if (userStdPixmap[0])
		create_icon(userStdPixmap, icon);
	if (userNapPixmap[0])
		create_icon(userNapPixmap, na_icon);
	if (userRevPixmap[0])
		create_icon(userRevPixmap, rev_icon);

	/* create a window for the icon */
	icon_attr.border_pixel = foregr;
	icon_attr.background_pixmap = iconPixmap;
	icon_window = XCreateWindow(mydisplay, DefaultRootWindow(mydisplay),
		0, 0, 64, (show_time ? 77 : 64), 0, attr.depth, InputOutput,
		CopyFromParent, CWBackPixmap, &icon_attr);
	
	if (show_time) {
		/* time string displayed below icon */
		ilabel.x = xv_get(sfont, FONT_DEFAULT_CHAR_WIDTH) +
			xv_get(sfont, FONT_DEFAULT_CHAR_WIDTH)/2;
		if (update_interval >= 60)
			/* no seconds displayed, adjust label position */
			ilabel.x += 2*xv_get(sfont, FONT_DEFAULT_CHAR_WIDTH) - 2;
		if (!hour24)
			/* am/pm, adjust position */
			ilabel.x -= xv_get(sfont, FONT_DEFAULT_CHAR_WIDTH);
	}

	week_message_size = WEEK_MESSAGE_SIZE + 3;
	weekslot_width = (xv_get(font, FONT_DEFAULT_CHAR_WIDTH) *
		week_message_size);
	dayslot_width = (nr_weekdays - 1) * weekslot_width;
	day_message_size = dayslot_width / xv_get(font, FONT_DEFAULT_CHAR_WIDTH);
	ybox_height = dayslot_height = weekslot_height = 
		xv_get(font, FONT_DEFAULT_CHAR_HEIGHT) + 6;
	ybox_width = 3 * xv_get(font, FONT_DEFAULT_CHAR_WIDTH);
	win_height = MAX(36*ybox_height, (n_slots+5)*weekslot_height) + 72;

	/* set up the rest of the tool */
	set_cms();
	xv_set(frame,
		XV_WIDTH, (nr_weekdays+1)*weekslot_width+60,
		XV_HEIGHT, win_height,
		XV_X, xposition, XV_Y, yposition,
		WIN_CMS, cms,
		0);

	init_pixrects();	/* Initialize pictures. */

	/* set this applications WM_CLASS attributes */
	WMClassHints.res_name = progname;
	WMClassHints.res_class = "Calentool";

	myWMHints = XGetWMHints(mydisplay, xv_get(frame, XV_XID));
	if (myWMHints == NULL) {
		myWMHints = XAllocWMHints();
		};
	myWMHints->flags = InputHint | IconWindowHint | StateHint | IconPixmapHint;
	myWMHints->input = True;
	myWMHints->icon_window = icon_window;
	myWMHints->icon_pixmap = iconPixmap;
	if (icon_pos) {
		/* user specified icon position */
		myWMHints->icon_x = xicon;
		myWMHints->icon_y = yicon;
		myWMHints->flags |= IconPositionHint;
	}
	sprintf(buff, "%s.iconic", progname);
	if (defaults_exists("window.iconic", NULL)) {
		if (defaults_get_boolean("window.iconic", NULL, False))
			myWMHints->initial_state = IconicState;
		else
			myWMHints->initial_state = NormalState;
	} else if (defaults_get_boolean(buff, "Calentool.iconic", False)) {
		myWMHints->initial_state = IconicState;
	} else {
		myWMHints->initial_state = NormalState;
	}

	/* do this to make sure positioning hints get set */
	/* (doing XV_X and XV_Y on the frame doesn't always work) */
	sizehints.x = xposition;
	sizehints.y = yposition;
	sizehints.flags = USPosition;
	XSetWMProperties(mydisplay, xv_get(frame, XV_XID), NULL, NULL,
		NULL, 0, &sizehints, myWMHints, &WMClassHints);

	/* this must be done AFTER myWMHints is set up */
	set_icon();
	closed_rect = (Rect *)xv_get(frame, FRAME_CLOSED_RECT);
	create_panel();
	create_main_window();
	check_calendar();	/* set proper icon */

	/* interpose on frame events (specifically open/close) */
	(void) notify_interpose_event_func(frame, myframe_interposer, NOTIFY_SAFE);

	timerclear(&(cal_timer.it_interval));
	timerclear(&(cal_timer.it_value));
	cal_timer.it_value.tv_sec = (long)update_interval;
	cal_timer.it_value.tv_usec = 0L;
	(void) notify_set_itimer_func(frame, itimer_handler, ITIMER_REAL, &cal_timer, NULL);
	err2console(FALSE); /* error msgs to notice window */

	xv_main_loop(frame);

	(void)leave((Notify_client)0, 0, (Notify_signal_mode)0);
}
             

static Notify_value
itimer_handler(me, which)
Notify_client me;
int which;
{
	static int timeout_cntr = 0;

	timerclear(&(cal_timer.it_interval));
	timerclear(&(cal_timer.it_value));
	cal_timer.it_value.tv_sec = (long)update_interval;
	cal_timer.it_value.tv_usec = 0L;
	(void) notify_set_itimer_func(frame, itimer_handler, ITIMER_REAL, &cal_timer, NULL);
	get_today();
	/* update date/time displayed in the panel */
	xv_set(clock_pi, PANEL_LABEL_STRING, clockstr, 0);
	if (show_time && (int)xv_get(frame, FRAME_CLOSED))
		update_icon_time();
#ifndef NO_SUN_MOON
	/* update data in Sun Data frame (if it's open) */
	if (sframe)
		write_times();
#endif
	/* only check appointments every TIME_OUT minutes */
	timeout_cntr += update_interval;
	if (timeout_cntr/60 < TIME_OUT)
		return(NOTIFY_DONE);
	timeout_cntr = 0;
	/* check todays appointments */
	check_calendar();
	return(NOTIFY_DONE);
}

Notify_value
leave(me, signal, when)
Notify_client me;
int signal;
Notify_signal_mode when;
{
	if (day_is_open)
		close_day();

	/* create outdated include files (if necessary) */
	if (save_old || expire_days)
		expire(expire_days);

	/* delete tmp file */
	if (access(tmpapts_pathname, R_OK) == 0 && unlink(tmpapts_pathname) < 0)
		perror(tmpapts_pathname);

	seln_destroy(s_client);
	exit(0);
	/* NOTREACHED */
	return(NOTIFY_DONE);
}

int
XIOError(mydisplay)
Display *mydisplay;
{
	leave((Notify_client)0, 0, (Notify_signal_mode)0);
	/* NOTREACHED */
	return 0;
}

int
XError(mydisplay, myerror)
Display *mydisplay;
XErrorEvent *myerror;
{
	char error_description[1024], error_mesg[1024], number[32];

	/* UGLY HACK! Xview generates BadMatch errors on X_SetInputFocus
	 * events when you close a window before the tool has a chance to
	 * draw in it - so, ignore those without comment.
	 */
	if ((myerror->error_code == BadMatch) &&
	    ((int) myerror->request_code == 42)
	    /*
	     ** minor code is not always 0 ...
	    && ((int) myerror->minor_code == 0)
	    */
	    )
		return 0;

	/* okay, these are errors we should report - but don't die, because
	 * X says these are non-fatal.
	 */
	XGetErrorText(mydisplay, myerror->error_code, error_description, 1024);
	XGetErrorDatabaseText(mydisplay, "XlibMessage", "XError",
		"X Error", error_mesg, 1024);
	fprintf(stderr, "%s: %s\n ", error_mesg, error_description);
	XGetErrorDatabaseText(mydisplay, "XlibMessage", "MajorCode",
		"Request Major code %d", error_mesg, 1024);
	fprintf(stderr, error_mesg, myerror->request_code);
	if (myerror->request_code < 128) {
		sprintf(number, "%d", myerror->request_code);
		XGetErrorDatabaseText(mydisplay, "XRequest", number,
			"", error_mesg, 1024);
	} else {
		strcpy(error_mesg, "X Extension Protocol Request");
	}
	fprintf(stderr, " (%s)\n  ", error_mesg);
	XGetErrorDatabaseText(mydisplay, "XlibMessage", "MinorCode",
		"Request Minor code %d", error_mesg, 1024);
	fprintf(stderr, error_mesg, myerror->minor_code);
	fputs("\n  ", stderr);
	XGetErrorDatabaseText(mydisplay, "XlibMessage", "ResourceID",
		"ResourceID 0x%x", error_mesg, 1024);
	fprintf(stderr, error_mesg, myerror->resourceid);
	fputs("\n  ", stderr);
	XGetErrorDatabaseText(mydisplay, "XlibMessage", "ErrorSerial",
		"Error Serial #%d", error_mesg, 1024);
	fprintf(stderr, error_mesg, myerror->serial);
	fputs("\n  ", stderr);
	XGetErrorDatabaseText(mydisplay, "XlibMessage", "CurrentSerial",
		"Current Serial #%d", error_mesg, 1024);
	fprintf(stderr, error_mesg, LastKnownRequestProcessed(mydisplay));
	fputs("\n", stderr);

	/** If we were debugging, this would be useful.... 
	abort();
	**/

	return 0;
}

void
create_icon(filename, iconPix)
char *filename;
Pixmap iconPix;
{
	Pixmap pix;
	int width, height, x_hot, y_hot;
	int status;

	status = XReadBitmapFile(mydisplay, DefaultRootWindow(mydisplay),
		filename, &width, &height, &pix, &x_hot, &y_hot);
	if (status != BitmapSuccess)
		err_rpt("Error reading iconPixmap", NON_FATAL);
	else
		XCopyPlane(mydisplay, pix, iconPix, gc, 0, 0, width, height,
			0, 0, 1L);
	XFreePixmap(mydisplay, pix);
}

/*
 * find the default background and foreground
 * colors of the screen then pass them on to
 * the frame (since the frame colors in XView
 * are, by definition, undefined).
 */
void
set_fgbg()
{
	char *def_str;
	char buff[128];

	/* get default color map */
	def_cmap = DefaultColormap(mydisplay, screen);
	/*
	 * check command line, then calentool defaults, then
	 * default black & white
	 */
	sprintf(buff, "%s.color.foreground", progname);
	if (defaults_exists(buff, "Calentool.color.foreground")) {
		def_str = defaults_get_string(buff, "Calentool.color.foreground", NULL);
		def_to_xvsc(def_str, &ct_fore, &foregr);
	} else if (defaults_exists("window.color.foreground", NULL)) {
		def_str = defaults_get_string("window.color.foreground", NULL, NULL);
		def_to_xvsc(def_str, &ct_fore, &foregr);
	} else {
		foregr = BlackPixel(mydisplay, screen);
		def_color.pixel = foregr;
		XQueryColor(mydisplay, def_cmap, &def_color);
		ct_fore.red = def_color.red / 257;
		ct_fore.green = def_color.green / 257;
		ct_fore.blue = def_color.blue / 257;
	}
	sprintf(buff, "%s.color.background", progname);
	if (defaults_exists(buff, "Calentool.color.background")) {
		def_str = defaults_get_string(buff, "Calentool.color.background", NULL);
		def_to_xvsc(def_str, &ct_back, &backgr);
	} else if (defaults_exists("window.color.background", NULL)) {
		def_str = defaults_get_string("window.color.background", NULL, NULL);
		def_to_xvsc(def_str, &ct_back, &backgr);
	} else {
		backgr = WhitePixel(mydisplay, screen);
		def_color.pixel = backgr;
		XQueryColor(mydisplay, def_cmap, &def_color);
		ct_back.red = def_color.red / 257;
		ct_back.green = def_color.green / 257;
		ct_back.blue = def_color.blue / 257;
	}
}

void set_cms()
{
	int i;

	ocms_data = (Xv_cmsdata *)xv_get(frame, WIN_CMS_DATA);

	for (i=0; i<CMS_CONTROL_COLORS; i++) {
		cmscolors[i].red = ocms_data->red[i];
		cmscolors[i].green = ocms_data->green[i];
		cmscolors[i].blue = ocms_data->blue[i];
	}

	/* white and black for the moon bitmap */
	cmscolors[CMS_CONTROL_COLORS+2].red = 255;
	cmscolors[CMS_CONTROL_COLORS+2].green = 255;
	cmscolors[CMS_CONTROL_COLORS+2].blue = 255;
	cmscolors[CMS_CONTROL_COLORS+3].red = 0;
	cmscolors[CMS_CONTROL_COLORS+3].green = 0;
	cmscolors[CMS_CONTROL_COLORS+3].blue = 0;
	black_pix = 0;
	white_pix = 0xffffffff;

	/* our background and foreground as specified by the user */
	cmscolors[CMS_CONTROL_COLORS+0].red = ct_back.red;
	cmscolors[CMS_CONTROL_COLORS+0].green = ct_back.green;
	cmscolors[CMS_CONTROL_COLORS+0].blue = ct_back.blue;
	cmscolors[CMS_CONTROL_COLORS+1].red = ct_fore.red;
	cmscolors[CMS_CONTROL_COLORS+1].green = ct_fore.green;
	cmscolors[CMS_CONTROL_COLORS+1].blue = ct_fore.blue;

	/* now make sure the control colors are set right */
	/* we set the control bg to the user's bg, but leave the
	 * control fg set to its original value.
	 */
	cmscolors[0].red = ct_back.red;
	cmscolors[0].green = ct_back.green;
	cmscolors[0].blue = ct_back.blue;
	/****
	cmscolors[CMS_CONTROL_COLORS+4].red = cmscolors[CMS_CONTROL_COLORS-1].red;
	cmscolors[CMS_CONTROL_COLORS+4].green = cmscolors[CMS_CONTROL_COLORS-1].green;
	cmscolors[CMS_CONTROL_COLORS+4].blue = cmscolors[CMS_CONTROL_COLORS-1].blue;
	****/

	cms = (Cms) xv_create(0, CMS,
			CMS_NAME, "palette",
			CMS_TYPE, XV_STATIC_CMS,
			CMS_CONTROL_CMS, TRUE,
			CMS_SIZE, CMS_CONTROL_COLORS+4,
			CMS_COLORS, cmscolors,
			0);
}

void
def_to_xvsc(def, sc, pixel)
char *def;
Xv_singlecolor *sc;
unsigned long *pixel;
{

	int r, g, b;

	/*
	 * we need to convert the defaults data base entry because
	 * it could be either a X color name or a string consisting
	 * of three space seperated integers.
	 * The defaults entry is converted to a Xv_singlecolor struct
	 * and a X pixel value.
	 */
	 if (isdigit(*def)) {
		sscanf(def, "%d %d %d", &r, &g, &b);
		sc->red = r;
		sc->green = g;
		sc->blue = b;
		/* approx scaling 0->255 to 0->65535 */
		def_color.red = r * 257;
		def_color.green = g * 257;
		def_color.blue = b * 257;
		XAllocColor(mydisplay, def_cmap, &def_color);
		*pixel = def_color.pixel;
	} else {
		(void) XParseColor(mydisplay, def_cmap, def, &def_color);
		/* approx scaling 0->65535 to 0->255 */
		sc->red = def_color.red / 257;
		sc->green = def_color.green / 257;
		sc->blue = def_color.blue / 257;
		XAllocColor(mydisplay, def_cmap, &def_color);
		*pixel = def_color.pixel;
	}
}

void
get_user_defaults()
{
	char *getenv(), *def;
	unsigned int result, w, h;
	char buff[128];
	extern int print_to_file;
	extern char psfile[];

	sprintf(buff, "%s.mainWin.geometry", progname);
	if (defaults_exists("window.geometry", NULL)) {
		def = defaults_get_string("window.geometry", NULL, "+100+56");
		result = XGeometry(mydisplay, screen, def, "+100+56",
				6, 1, 1, 0, 0,
				&xposition, &yposition, &w, &h);
	} else if (defaults_exists("window.x", NULL)) {
		xposition = defaults_get_integer("window.x", NULL, 100);
		yposition = defaults_get_integer("window.y", NULL, 56);
	} else if (defaults_exists(buff, "Calentool.mainWin.geometry")) {
		def = defaults_get_string(buff, "Calentool.mainWin.geometry", "+100+56");
		result = XGeometry(mydisplay, screen, def, "+100+56",
				6, 1, 1, 0, 0,
				&xposition, &yposition, &w, &h);
	} else {
		xposition = 100;
		yposition = 56;
	}
	sprintf(buff, "%s.icon.geometry", progname);
	if (defaults_exists("icon.x", NULL)) {
		/* specified on the command line */
		xicon = defaults_get_integer("icon.x", NULL, 100);
		yicon = defaults_get_integer("icon.y", NULL, 56);
		icon_pos = 1;
	} else if (defaults_exists(buff, "Calentool.icon.geometry")) {
		/* specified in the resource file */
		def = defaults_get_string(buff, "Calentool.icon.geometry", "+0+0");
		result = XGeometry(mydisplay, screen, def, "+0+0",
				6, 1, 1, 0, 0,
				&xicon, &yicon, &w, &h);
		icon_pos = 1;
	}
	sprintf(buff, "%s.hour24", progname);
	hour24 = defaults_get_boolean(buff, "Calentool.hour24", HOUR_24);
	sprintf(buff, "%s.weekdays", progname);
	nr_weekdays = defaults_get_integer_check(buff, "Calentool.weekdays", NR_WEEKDAYS, 5, 7);
	sprintf(buff, "%s.mondayFirst", progname);
	monday_first = defaults_get_boolean(buff, "Calentool.mondayFirst", MON_FIRST);
	sprintf(buff, "%s.dayFirst", progname);
	day_first = defaults_get_integer_check(buff, "Calentool.dayFirst", DAY_FIRST, 0, 2);
	sprintf(buff, "%s.startHour", progname);
	start_hour = defaults_get_integer_check(buff, "Calentool.startHour", START_HOUR, 0, 23);
	sprintf(buff, "%s.endHour", progname);
	end_hour = defaults_get_integer_check(buff, "Calentool.endHour", END_HOUR, start_hour+1, 23);
	sprintf(buff, "%s.numberOfNotes", progname);
	num_notes = defaults_get_integer_check(buff, "Calentool.numberOfNotes", N_NOTESLOTS, 1, 20);
	sprintf(buff, "%s.printer.type", progname);
	if (defaults_exists(buff, "Calentool.printer.type")) {
		def = defaults_get_string(buff, "Calentool.printer.type", "ps");
		if (!strncmp(def, "ps", 2))
			print_dev = PR_POSTSCRIPT;
		else
			print_dev = PR_ASCII;
	}
#ifndef NO_PRINTER
	sprintf(buff, "%s.printer.name", progname);
	strcpy(printer, "lpr -P");
	if (defaults_exists(buff, "Calentool.printer.name")) {
		def = defaults_get_string(buff, "Calentool.printer.name", NULL);
		strcat(printer, def);
	} else if ((def = getenv("PRINTER")) != NULL)
		strcat(printer, def);
	else
		strcpy(printer, PRINT_CMD);
	sprintf(buff, "%s.printer.dest", progname);
	if (defaults_exists(buff, "Calentool.printer.dest"))
		print_to_file = defaults_get_enum(buff, "Calentool.printer.dest", prfile_pairs);
	sprintf(buff, "%s.printer.filename", progname);
	if (defaults_exists(buff, "Calentool.printer.filename")) {
		def = defaults_get_string(buff, "Calentool.printer.filename", NULL);
		strcpy(psfile, def);
	}
	sprintf(buff, "%s.printer.printTrailer", progname);
	print_trailer = defaults_get_boolean(buff, "Calentool.printer.printTrailer", FALSE);
#endif
	sprintf(buff, "%s.beep", progname);
	beep = defaults_get_boolean(buff, "Calentool.beep", FALSE);
	sprintf(buff, "%s.beepOpen", progname);
	beep_open = defaults_get_boolean(buff, "Calentool.beepOpen", FALSE);
	sprintf(buff, "%s.includeOld", progname);
	include_old = defaults_get_boolean(buff, "Calentool.includeOld", FALSE);
	sprintf(buff, "%s.saveOld", progname);
	save_old = defaults_get_boolean(buff, "Calentool.saveOld", FALSE);
	sprintf(buff, "%s.showIconTime", progname);
	show_time = defaults_get_boolean(buff, "Calentool.showIconTime", FALSE);
	sprintf(buff, "%s.updateInterval", progname);
	if (defaults_exists(buff, "Calentool.updateInterval"))
		update_interval = defaults_get_integer_check(buff, "Calentool.updateInterval", 1, 1, 300);
	sprintf(buff, "%s.expireInterval", progname);
	if (defaults_exists(buff, "Calentool.expireInterval"))
		expire_days = defaults_get_integer(buff, "Calentool.expireInterval", 7);
	sprintf(buff, "%s.checkNotes", progname);
	if (defaults_exists(buff, "Calentool.checkNotes")) {
		limitcheck = !defaults_get_boolean(buff, "Calentool.checkNotes", FALSE);
		if (limitcheck > 1)
			limitcheck = 1;
	}
	sprintf(buff, "%s.holidays.astronomical", progname);
	if (defaults_exists(buff, "Calentool.holidays.astronomical"))
		holiday_a = defaults_get_enum(buff, "Calentool.holidays.astronomical", holiday_pairs);
	sprintf(buff, "%s.holidays.christian", progname);
	if (defaults_exists(buff, "Calentool.holidays.christian"))
		holiday_c = defaults_get_enum(buff, "Calentool.holidays.christian", holiday_pairs);
	sprintf(buff, "%s.holidays.islamic", progname);
	if (defaults_exists(buff, "Calentool.holidays.islamic"))
		holiday_i = defaults_get_enum(buff, "Calentool.holidays.islamic", holiday_pairs);
	sprintf(buff, "%s.holidays.jewish", progname);
	if (defaults_exists(buff, "Calentool.holidays.jewish"))
		holiday_j = defaults_get_enum(buff, "Calentool.holidays.jewish", holiday_pairs);
	sprintf(buff, "%s.holidays.secular", progname);
	if (defaults_exists(buff, "Calentool.holidays.secular"))
		holiday_s = defaults_get_enum(buff, "Calentool.holidays.secular", holiday_pairs);
	sprintf(buff, "%s.initialDisplay", progname);
	if (defaults_exists(buff, "Calentool.initialDisplay"))
		mainsw_state = defaults_get_enum(buff, "Calentool.initialDisplay", displayMode_pairs);
	else
		mainsw_state = DISPLAYING_DAY;
	sprintf(buff, "%s.iconPixmap.standard", progname);
	if (defaults_exists(buff, "Calentool.iconPixmap.standard")) {
		def = defaults_get_string(buff, "Calentool.iconPixmap.standard", "");
		strcpy(userStdPixmap, def);
	}
	sprintf(buff, "%s.iconPixmap.noAppts", progname);
	if (defaults_exists(buff, "Calentool.iconPixmap.noAppts")) {
		def = defaults_get_string(buff, "Calentool.iconPixmap.noAppts", "");
		strcpy(userNapPixmap, def);
	}
	sprintf(buff, "%s.iconPixmap.apptPending", progname);
	if (defaults_exists(buff, "Calentool.iconPixmap.apptPending")) {
		def = defaults_get_string(buff, "Calentool.iconPixmap.apptPending", "");
		strcpy(userRevPixmap, def);
	}
	sprintf(buff, "%s.stippleDay", progname);
	stipple_day = defaults_get_boolean(buff, "Calentool.stippleDay", TRUE);
}

void
usage()
{
	fprintf(stderr, "usage: %s [options...]\nwhere <options> are:\n", progname);
	fprintf(stderr, " -f <appointment_file>\n");
	fprintf(stderr, " -r		// readonly\n");
	fprintf(stderr, " -I <dwmy>	// initial startup with day [d], week [w], month [m] or year [y] display\n");
	fprintf(stderr, " -p <dwWm>	// print selected day [d], week [w] or month [m] appts and exit\n");
	fprintf(stderr, " -P <dwWm>	// like -p, only don't print marked notes\n");
	fprintf(stderr, " -m <user>	// mail selected days appts to <user>\n");
	fprintf(stderr, " -M <user>	// like -m, only don't mail marked notes\n");
	fprintf(stderr, " [-5|-6|-7]	// 5, 6 or 7-day week display\n");
	fprintf(stderr, " [-12|-24]	// 12 or 24 hour time format\n");
	fprintf(stderr, " -d <date>	// display appts for <date>\n");
	fprintf(stderr, " -b		// beep and display message when appt is pending\n");
	fprintf(stderr, " -B		// beep and open window when appt is pending\n");
	fprintf(stderr, " -i		// auto-include outdated appts files\n");
	fprintf(stderr, " -l [0|1]	// set limit check for appts to include notes\n");
	fprintf(stderr, " -o		// create outdated include files\n");
	fprintf(stderr, " -t		// display time below icon\n");
	fprintf(stderr, " -u <interval>	// time update interval (seconds)\n");
	fprintf(stderr, " -w		// display 'Working!' message\n");
	fprintf(stderr, " -D <0-2>	// date display format\n");
	fprintf(stderr, " -e		// Mon-Sun week display\n");
	fprintf(stderr, " -E		// European options (-e, -24, -D1)\n");
#ifndef NO_HOLIDAYS
	fprintf(stderr, " -h <Aacijs>	// display selected computed holidays\n");
	fprintf(stderr, "		// A=All, a=astronomical, c=Christian,\n");
	fprintf(stderr, "		// i=Islamic, j=Jewish, s=secular\n");
	fprintf(stderr, " -H <Aacijs>	// like -h, only flag them as marked\n");
#endif
	fprintf(stderr, " -T <printdev>	// printer output type for -p and -m options\n");
	fprintf(stderr, " -s <hour>	// start hour\n");
	fprintf(stderr, " -S <hour>	// stop hour\n");
	fprintf(stderr, " -N <num>	// number of note slots\n");
	fprintf(stderr, " -x <days>	// expire appts after <days> days\n");
	fprintf(stderr, " -z		// conversion flag (see INSTALL)\n");
	fprintf(stderr, " <window_opts>	// XView options\n");
	exit(0);
}

