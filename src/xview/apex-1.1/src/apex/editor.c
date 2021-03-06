#ifndef lint
static char    *RCSid = "$Header: /n/homeserver/i/gounares/pu/apex/src/apex/RCS/editor.c,v 1.1 93/01/06 03:27:28 gounares Exp Locker: gounares $";

#endif

/*
 * $Log:	editor.c,v $
 * Revision 1.1  93/01/06  03:27:28  gounares
 * Initial revision
 * 
 */

/*
 * editor.c
 * 
 * This file handles the creating of full-featured EDIT objects.  Actions for
 * various callbacks are handled elsewhere.
 * 
 * written by:
 * 
 * Alexander Gounares 9/18/92
 */
/*
 * Copyright 1993 Alexander Gounares
 * 
 * This source is covered by the GNU General Public License Version 2
 * 
 * see the apeX manual for more details
 */

#include <sys/param.h>		       /* MAXPATHLEN (include types.h if*/
#include <dirent.h>		       /* MAXNAMLEN */
#include <sys/stat.h>

#include <fcntl.h>
#include <stdio.h>
/* #include <strings.h> */
#include <X11/X.h>
#include <xview/defaults.h>
#include <xview/font.h>
#include <xview/notice.h>
#include <xview/frame.h>
#include <xview/xview.h>
#include <xview/scrollbar.h>
#include <xview/text.h>
#include <xview/panel.h>
#include <xview/selection.h>
#include <xview/sel_svc.h>
#include <xview/sel_attrs.h>
#include <xview/notify.h>
#include <xview/termsw.h>
#include "apex_shell.h"
#include "editor.h"
#include "file_io.h"
#include "alloc.h"
#include "object_list.h"
#include "apex.bmp"
#include "tb_apex_bmp"
#include "tb_create_bmp"
#include "tb_finish_bmp"
#include "tb_csh_bmp"
#include "tb_delete_bmp"
#include "tb_hide_bmp"
#include "tb_next_bmp"
#include "tb_prev_bmp"
#include "tb_run_bmp"
#include "tb_show_bmp"
#include "tb_desc_bmp"
#include "tb_nav_bmp"
#include "tb_sp_bmp"
#include "apex_title2_bmp"
#include "editor_qmark_bmp"
#include "object.h"
#include "navigator.h"
#include "state_server.h"
#include "file_browser.h"
#include "apex_options.h"

#define	GET_INT_ATTR_VAL(var)  if (argc > 0) {var = (caddr_t) atoi(argv[1]); argc--, argv++;}
#define STDERR  2
#define SAVE_NUM	2
#define STORE_NUM	3
#define INCLUDE_NUM	4
#define NEW_NUM		5
#define EXTRA_NUM	5

int             CLIENT_DATA_KEY = 0;
Notify_value    editor_done();
static void     new_editor_proc();
static void     add_filename_to_menu();
static void     file_name_proc();

/*
 * dismiss_proc -- equivalent to selecting 'quit' from the main window menu
 */
static void
dismiss_proc(item, event)
    Panel_item      item;
    Event          *event;
{
	Editor         *ped;

	ped = (Editor *) xv_get(item, PANEL_CLIENT_DATA);

	xv_destroy_safe(ped->frame);
}

/*
 * Return pointer to longest suffix not beginning with '/'
 */

static char    *
base_name(full_name)
    char           *full_name;
{
	extern char    *rindex();
	register char  *temp;

	if ((temp = rindex(full_name, '/')) == NULL)
		return (full_name);
	else
		return (temp + 1);
}


set_name_frame(textsw_local, attributes)
    Textsw          textsw_local;
    Attr_avlist     attributes;
{
	char            frame_label[50 + MAXNAMLEN + MAXPATHLEN],
	                buf[30];
	Icon            edit_icon;
	char            icon_text[sizeof(frame_label)];
	char           *ptr;
	int             len,
	                pass_on = 0,
	                repaint = 0;
	Attr_avlist     attrs;
	char           *attr_string;
	static int      current_lineno,
	                top_lineno,
	                bottom_lineno,
	                read_only,
	                was_read_only;
	int             temp_line,
	                temp_top,
	                temp_bottom;
	Editor         *ed;


	ed = (Editor *) xv_get(textsw_local, TEXTSW_CLIENT_DATA);

	icon_text[0] = '\0';
	ptr = icon_text;
	for (attrs = attributes; *attrs; attrs = attr_next(attrs)) {
		repaint++;	       /* Assume this attribute needs a
				        * repaint. */
		switch ((Textsw_action) (*attrs)) {
		case TEXTSW_ACTION_CAPS_LOCK:
			ed->fCaps_lock_on = (int) attrs[1];
			ATTR_CONSUME(*attrs);
			break;
		case TEXTSW_ACTION_CHANGED_DIRECTORY:
			attr_string = (char *) attrs[1];
			switch (attr_string[0]) {
			case '/':
				(void) strcpy(ed->szDirname, attrs[1]);
				break;
			case '.':
				if (attr_string[1] != '\0')
					(void) getwd(ed->szDirname);
				break;
			case '\0':
				break;
			default:
				(void) strcat(ed->szDirname, "/");
				(void) strcat(ed->szDirname, attrs[1]);
				break;
			}
			ATTR_CONSUME(*attrs);
			break;
		case TEXTSW_ACTION_USING_MEMORY:
			(void) strcpy(ed->szFilename, "(NONE)");
			(void) strcpy(icon_text, "NO FILE");
			ed->fEdited = read_only = 0;
			ATTR_CONSUME(*attrs);
			break;
		case TEXTSW_ACTION_LOADED_FILE:
			(void) strcpy(ed->szFilename, ed->pFile->szFilename);
			ed->fEdited = read_only = 0;
			goto Update_icon_text;
		case TEXTSW_ACTION_EDITED_FILE:
			ed->fEdited = 1;
			*ptr++ = '>';
		      Update_icon_text:
			len = (strlen(attrs[1]) > sizeof(icon_text) - 2) ?
				sizeof(icon_text) - 2 : strlen(attrs[1]);
			/* need 1 char for edit/not, 1 for null */
			(void) strncpy(ptr, attrs[1], len);
			ptr[len] = '\0';
			(void) strcpy(ptr, base_name(ptr));	/* strip path */
			ATTR_CONSUME(*attrs);
			break;
		default:
			pass_on = 1;
			repaint--;     /* Above assumption was wrong. */
			break;
		}
	}
	if (pass_on)
		(void) textsw_default_notify(textsw_local, attributes);
	if (repaint) {
		(void) sprintf(frame_label, "%s apeX Editor #%d - %s%s %s",
			(ed->fCaps_lock_on) ? "[CAPS] " : "",
			ed->id,
			ed->szFilename,
			(was_read_only) ? " (read only)"
			: (ed->fEdited) ? " (edited)" : "",
			ed->szDirname);
		(void) xv_set(ed->frame, FRAME_LABEL, frame_label, 0);

		/* default menu item stuff */
	}
	if (icon_text[0] != '\0') {
		struct rect     text_rect,
		               *icon_rect;
		struct pixfont *font;

		icon_rect = (Rect *) (icon_get(ed->icon, ICON_IMAGE_RECT));
		font = (struct pixfont *) (icon_get(ed->icon, ICON_FONT));
		ptr = icon_text;

		/* adjust icon text top/height to match font height */
		text_rect.r_height = xv_get(font, FONT_DEFAULT_CHAR_HEIGHT);
		text_rect.r_top =
			icon_rect->r_height - (text_rect.r_height + 2);

		/* center the icon text */
		text_rect.r_width = strlen(ptr) * (xv_get(font, FONT_DEFAULT_CHAR_WIDTH));
		if (text_rect.r_width > icon_rect->r_width)
			text_rect.r_width = icon_rect->r_width;
		text_rect.r_left = (icon_rect->r_width - text_rect.r_width) / 2;

		(void) xv_set(ed->icon,
			XV_LABEL, ptr,
			ICON_LABEL_RECT, &text_rect,
			0);
		/* xv_set actually makes a copy of all the icon fields */
		(void) xv_set(ed->frame, FRAME_ICON, ed->icon, 0);
	}
	/* now let's do the predictive interface stuff */

	if (ed->fEdited && ed->save_menu_item) {
		if (ed->pFile->szFilename[0] != '\0') {
			xv_set(ed->file_menu, MENU_DEFAULT_ITEM,
				ed->save_menu_item, NULL);
		} else {
			xv_set(ed->file_menu, MENU_DEFAULT_ITEM,
				ed->save_as_menu_item, NULL);
		}
	} else if (ed->save_menu_item) {
		xv_set(ed->file_menu, MENU_DEFAULT_ITEM,
			ed->load_menu_item, NULL);
	}
}

static void
button_notify_proc(item, event)
    Panel_item      item;
    Event          *event;
{
	Editor         *ed = (Editor *) xv_get(item, XV_KEY_DATA, CLIENT_DATA_KEY);
	Textsw          textsw;
	Menu            menu = (Menu) xv_get(item, PANEL_ITEM_MENU);
	Panel           p_menu = (Panel) xv_get(menu, MENU_PIN_WINDOW);
	Menu_item       menu_item;
	Menu            pullr_menu;
	int             num_items,
	                i;

	textsw = ed->textsw;

	xv_set(menu, XV_KEY_DATA, TEXTSW_MENU_DATA_KEY, textsw, 0);
	if (p_menu) {
		num_items = (int) xv_get(menu, MENU_NITEMS);
		for (i = 1; i <= num_items; i++) {
			menu_item = (Menu_item) xv_get(menu, MENU_NTH_ITEM, i);
			if (menu_item) {
				pullr_menu = (Menu) xv_get(menu_item, MENU_PULLRIGHT);
				if (pullr_menu) {
					xv_set(pullr_menu, XV_KEY_DATA, TEXTSW_MENU_DATA_KEY, textsw, 0);
				}
			}
		}
	}
}

static
textsw_split_proc(orgview, newview, pos)
    Xv_Window       orgview,
                    newview;
    int             pos;
{
	Textsw          textsw = textsw_first(orgview);
	Editor         *png = (Editor *) xv_get(textsw, XV_KEY_DATA, CLIENT_DATA_KEY);

	for (textsw = textsw_first(orgview); textsw; textsw = textsw_next(textsw)) {
		xv_set(textsw, XV_KEY_DATA, CLIENT_DATA_KEY, png, NULL);
	}
}

/*
 * info_proc -- create a popup with the title image and version number
 */
static void 
info_proc(menu, menu_item)
    Menu            menu;
    Menu_item       menu_item;
{
	Frame           frame;
	Panel           panel;
	Editor         *png = (Editor *) xv_get(menu, MENU_CLIENT_DATA);
	static Server_image title_image;
	int             width,
	                height;
	char            szVersion[25],
	                szTitle[256];
	Display        *dpy;
	Rect            rect;

	if (!title_image)
		title_image = (Server_image) xv_create(NULL, SERVER_IMAGE,
			SERVER_IMAGE_X_BITS, apex_title2_bmp_bits,
			SERVER_IMAGE_DEPTH, 1,
			XV_WIDTH, apex_title2_bmp_width,
			XV_HEIGHT, apex_title2_bmp_height,
			NULL);

	sscanf(RCSid, "%*s %*s %s", szVersion);
	sprintf(szTitle, "apeX Version %s", szVersion);

	frame = (Frame) xv_create(png->frame, FRAME_CMD,
		FRAME_LABEL, szTitle,
		NULL);

	panel = (Panel) xv_get(frame, FRAME_CMD_PANEL);

	xv_create(panel, PANEL_MESSAGE,
		PANEL_LABEL_IMAGE, title_image,
		NULL);


	frame_get_rect(frame, &rect);
	dpy = (Display *) xv_get(frame, XV_DISPLAY);
	width = DisplayWidth(dpy, DefaultScreen(dpy));
	height = DisplayHeight(dpy, DefaultScreen(dpy));

	rect.r_left = (width - apex_title2_bmp_width) / 2;
	rect.r_top = (height - apex_title2_bmp_height) / 2;

#ifdef hpux
	frame_set_rect(frame, &rect);
#else  hpux
	frame_set_rect(frame, rect);
#endif hpux

	window_fit(panel);
	window_fit(frame);

	xv_set(frame, XV_SHOW, TRUE, NULL);
}

static Server_image tb_apex_image;
static Server_image tb_create_image;
static Server_image tb_finish_image;
static Server_image tb_csh_image;
static Server_image tb_delete_image;
static Server_image tb_hide_image;
static Server_image tb_next_image;
static Server_image tb_prev_image;
static Server_image tb_run_image;
static Server_image tb_show_image;
static Server_image tb_nav_image;
static Server_image tb_desc_image;
static Server_image tb_sp_image;
static Server_image qmark_image;

/*
 * create_main_help -- create the popup for main editor help
 */
static void 
create_main_help(menu, menu_item)
    Menu            menu;
    Menu_item       menu_item;
{
	Frame           frame;
	Panel           panel;

	Editor         *ped = (Editor *) xv_get(menu, MENU_CLIENT_DATA);

	frame = (Frame) xv_create(ped->frame, FRAME_CMD,
		FRAME_LABEL, "apeX Instant-Help for the Editor",
		NULL);

	panel = (Panel) xv_get(frame, FRAME_CMD_PANEL);

	xv_set(panel, PANEL_LAYOUT, PANEL_VERTICAL, NULL);

	xv_create(panel, PANEL_MESSAGE,
		PANEL_LABEL_STRING, "Overview",
		PANEL_LABEL_BOLD, TRUE,
		NULL);

	xv_create(panel, PANEL_MESSAGE,
		PANEL_LABEL_STRING,
		"    apeX is a multi-featured developement environment.  It's",
		NULL);
	xv_create(panel, PANEL_MESSAGE,
		PANEL_LABEL_STRING,
		"principal features are a hypertext-like Object-Linking Editor",
		NULL);
	xv_create(panel, PANEL_MESSAGE,
		PANEL_LABEL_STRING,
		"and a source code Navigator.",
		NULL);
	xv_create(panel, PANEL_MESSAGE,
		PANEL_LABEL_STRING,
		"The Editor Tool Bar",
		PANEL_LABEL_BOLD, TRUE,
		NULL);

	xv_create(panel, PANEL_BUTTON,
		PANEL_LABEL_IMAGE, tb_run_image,
		NULL);

	xv_set(panel, PANEL_LAYOUT, PANEL_HORIZONTAL, NULL);

	xv_create(panel, PANEL_MESSAGE,
		PANEL_LABEL_STRING,
		"Execute an Object Link",
		PANEL_LABEL_BOLD, TRUE,
		NULL);

	xv_set(panel, PANEL_LAYOUT, PANEL_VERTICAL, NULL);

	xv_create(panel, PANEL_BUTTON,
		PANEL_LABEL_IMAGE, tb_desc_image,
		NULL);

	xv_set(panel, PANEL_LAYOUT, PANEL_HORIZONTAL, NULL);

	xv_create(panel, PANEL_MESSAGE,
		PANEL_LABEL_STRING,
		"Execute an Object Link",
		PANEL_LABEL_BOLD, TRUE,
		NULL);

	xv_set(panel, PANEL_LAYOUT, PANEL_VERTICAL, NULL);

	xv_create(panel, PANEL_BUTTON,
		PANEL_LABEL_IMAGE, tb_desc_image,
		NULL);

	xv_set(panel, PANEL_LAYOUT, PANEL_HORIZONTAL, NULL);

	xv_create(panel, PANEL_MESSAGE,
		PANEL_LABEL_STRING,
		"Execute (Run) an Object Link",
		PANEL_LABEL_BOLD, TRUE,
		NULL);

	xv_set(panel, PANEL_LAYOUT, PANEL_VERTICAL, NULL);

	xv_create(panel, PANEL_BUTTON,
		PANEL_LABEL_IMAGE, tb_desc_image,
		NULL);

	xv_set(panel, PANEL_LAYOUT, PANEL_HORIZONTAL, NULL);

	xv_create(panel, PANEL_MESSAGE,
		PANEL_LABEL_STRING,
		"Show an Object Description",
		PANEL_LABEL_BOLD, TRUE,
		NULL);

	xv_set(panel, PANEL_LAYOUT, PANEL_VERTICAL, NULL);

	xv_create(panel, PANEL_BUTTON,
		PANEL_LABEL_IMAGE, tb_show_image,
		NULL);

	xv_set(panel, PANEL_LAYOUT, PANEL_HORIZONTAL, NULL);

	xv_create(panel, PANEL_MESSAGE,
		PANEL_LABEL_STRING,
		"Display an Object Link",
		PANEL_LABEL_BOLD, TRUE,
		NULL);

	xv_set(panel, PANEL_LAYOUT, PANEL_VERTICAL, NULL);

	xv_create(panel, PANEL_BUTTON,
		PANEL_LABEL_IMAGE, tb_prev_image,
		NULL);

	xv_set(panel, PANEL_LAYOUT, PANEL_HORIZONTAL, NULL);

	xv_create(panel, PANEL_MESSAGE,
		PANEL_LABEL_STRING,
		"Show the Previous Link",
		PANEL_LABEL_BOLD, TRUE,
		NULL);

	xv_set(panel, PANEL_LAYOUT, PANEL_VERTICAL, NULL);

	xv_create(panel, PANEL_BUTTON,
		PANEL_LABEL_IMAGE, tb_next_image,
		NULL);

	xv_set(panel, PANEL_LAYOUT, PANEL_HORIZONTAL, NULL);

	xv_create(panel, PANEL_MESSAGE,
		PANEL_LABEL_STRING,
		"Show the Next Link",
		PANEL_LABEL_BOLD, TRUE,
		NULL);

	xv_set(panel, PANEL_LAYOUT, PANEL_VERTICAL, NULL);

	xv_create(panel, PANEL_BUTTON,
		PANEL_LABEL_IMAGE, tb_create_image,
		NULL);

	xv_set(panel, PANEL_LAYOUT, PANEL_HORIZONTAL, NULL);

	xv_create(panel, PANEL_MESSAGE,
		PANEL_LABEL_STRING,
		"Create a New Object Link",
		PANEL_LABEL_BOLD, TRUE,
		NULL);

	xv_set(panel, PANEL_LAYOUT, PANEL_VERTICAL, NULL);

	xv_create(panel, PANEL_BUTTON,
		PANEL_LABEL_IMAGE, tb_delete_image,
		NULL);

	xv_set(panel, PANEL_LAYOUT, PANEL_HORIZONTAL, NULL);

	xv_create(panel, PANEL_MESSAGE,
		PANEL_LABEL_STRING,
		"Delete the Selected Object Link",
		PANEL_LABEL_BOLD, TRUE,
		NULL);

	xv_set(panel, PANEL_LAYOUT, PANEL_VERTICAL, NULL);

	xv_create(panel, PANEL_BUTTON,
		PANEL_LABEL_IMAGE, tb_hide_image,
		NULL);

	xv_set(panel, PANEL_LAYOUT, PANEL_HORIZONTAL, NULL);

	xv_create(panel, PANEL_MESSAGE,
		PANEL_LABEL_STRING,
		"List All Object Links",
		PANEL_LABEL_BOLD, TRUE,
		NULL);

	xv_set(panel, PANEL_LAYOUT, PANEL_VERTICAL, NULL);

	xv_create(panel, PANEL_BUTTON,
		PANEL_LABEL_IMAGE, tb_csh_image,
		NULL);

	xv_set(panel, PANEL_LAYOUT, PANEL_HORIZONTAL, NULL);

	xv_create(panel, PANEL_MESSAGE,
		PANEL_LABEL_STRING,
		"Start the apeX Shell",
		PANEL_LABEL_BOLD, TRUE,
		NULL);

	xv_set(panel, PANEL_LAYOUT, PANEL_VERTICAL, NULL);

	xv_create(panel, PANEL_BUTTON,
		PANEL_LABEL_IMAGE, tb_nav_image,
		NULL);

	xv_set(panel, PANEL_LAYOUT, PANEL_HORIZONTAL, NULL);

	xv_create(panel, PANEL_MESSAGE,
		PANEL_LABEL_STRING,
		"Start the apeX Navigator",
		PANEL_LABEL_BOLD, TRUE,
		NULL);

	xv_set(panel, PANEL_LAYOUT, PANEL_VERTICAL, NULL);

	xv_create(panel, PANEL_BUTTON,
		PANEL_LABEL_IMAGE, tb_apex_image,
		NULL);

	xv_set(panel, PANEL_LAYOUT, PANEL_HORIZONTAL, NULL);

	xv_create(panel, PANEL_MESSAGE,
		PANEL_LABEL_STRING,
		"Start Another Editor",
		PANEL_LABEL_BOLD, TRUE,
		NULL);

	xv_set(panel, PANEL_LAYOUT, PANEL_VERTICAL, NULL);

	xv_create(panel, PANEL_BUTTON,
		PANEL_LABEL_IMAGE, tb_sp_image,
		NULL);

	xv_set(panel, PANEL_LAYOUT, PANEL_HORIZONTAL, NULL);

	xv_create(panel, PANEL_MESSAGE,
		PANEL_LABEL_STRING,
		"Start a Scratch Pad",
		PANEL_LABEL_BOLD, TRUE,
		NULL);

	window_fit(panel);
	window_fit(frame);
	xv_set(frame, XV_SHOW, TRUE, NULL);

}

/*
 * create_editor -- create the editor object
 */
Editor         *
create_editor(fType)
    int             fType;
{
	Editor         *ed;

	extern struct pixfont *pw_pfsysopen();

	int             checkpoint = 0;
	int             margin;
	Menu            edit_panel_menu,
	                extra_menu,
	                textsw_menu,
	                display_panel_menu,
	                find_panel_menu,
	                tool_panel_menu,
	                working_group_menu,
	                file_panel_menu,
	                help_menu;
	Menu_item       menu_item,
	                store_menu_item,
	                include_menu_item,
	                extra_menu_item,
	                new_menu_item,
	                save_menu_item,
	                load_menu_item;
	Server_image    icon_image;
	Server_image    mask_image;

	char            szBuf[100];

	ed = (Editor *) acalloc(1, sizeof(Editor));
	ed->pFile = (File *) acalloc(1, sizeof(File));

	getwd(ed->szDirname);

	icon_image = xv_create(0, SERVER_IMAGE,
		SERVER_IMAGE_X_BITS, apex_bmp_bits,
		SERVER_IMAGE_DEPTH, 1,
		XV_WIDTH, 64,
		XV_HEIGHT, 64,
		0);

	mask_image = xv_create(0, SERVER_IMAGE,
		SERVER_IMAGE_X_BITS, apex_bmp_bits,
		SERVER_IMAGE_DEPTH, 1,
		XV_WIDTH, 64,
		XV_HEIGHT, 64,
		0);

	if (!tb_apex_image) {
		tb_apex_image = (Server_image) xv_create(NULL, SERVER_IMAGE,
			SERVER_IMAGE_X_BITS, tb_apex_bmp_bits,
			SERVER_IMAGE_DEPTH, 1,
			XV_WIDTH, tb_apex_bmp_width,
			XV_HEIGHT, tb_apex_bmp_height,
			NULL);
	}
	if (!tb_sp_image) {
		tb_sp_image = (Server_image) xv_create(NULL, SERVER_IMAGE,
			SERVER_IMAGE_X_BITS, tb_sp_bmp_bits,
			SERVER_IMAGE_DEPTH, 1,
			XV_WIDTH, tb_sp_bmp_width,
			XV_HEIGHT, tb_sp_bmp_height,
			NULL);
	}
	if (!tb_create_image) {
		tb_create_image = (Server_image) xv_create(NULL, SERVER_IMAGE,
			SERVER_IMAGE_X_BITS, tb_create_bmp_bits,
			SERVER_IMAGE_DEPTH, 1,
			XV_WIDTH, tb_create_bmp_width,
			XV_HEIGHT, tb_create_bmp_height,
			NULL);
	}
	ed->create_image = tb_create_image;

	if (!tb_finish_image) {
		tb_finish_image = (Server_image) xv_create(NULL, SERVER_IMAGE,
			SERVER_IMAGE_X_BITS, tb_finish_bmp_bits,
			SERVER_IMAGE_DEPTH, 1,
			XV_WIDTH, tb_finish_bmp_width,
			XV_HEIGHT, tb_finish_bmp_height,
			NULL);
	}
	ed->finish_image = tb_finish_image;

	if (!tb_csh_image) {
		tb_csh_image = (Server_image) xv_create(NULL, SERVER_IMAGE,
			SERVER_IMAGE_X_BITS, tb_csh_bmp_bits,
			SERVER_IMAGE_DEPTH, 1,
			XV_WIDTH, tb_csh_bmp_width,
			XV_HEIGHT, tb_csh_bmp_height,
			NULL);
	}
	if (!tb_delete_image) {
		tb_delete_image = (Server_image) xv_create(NULL, SERVER_IMAGE,
			SERVER_IMAGE_X_BITS, tb_delete_bmp_bits,
			SERVER_IMAGE_DEPTH, 1,
			XV_WIDTH, tb_delete_bmp_width,
			XV_HEIGHT, tb_delete_bmp_height,
			NULL);
	}
	if (!tb_hide_image) {
		tb_hide_image = (Server_image) xv_create(NULL, SERVER_IMAGE,
			SERVER_IMAGE_X_BITS, tb_hide_bmp_bits,
			SERVER_IMAGE_DEPTH, 1,
			XV_WIDTH, tb_hide_bmp_width,
			XV_HEIGHT, tb_hide_bmp_height,
			NULL);
	}
	if (!tb_desc_image) {
		tb_desc_image = (Server_image) xv_create(NULL, SERVER_IMAGE,
			SERVER_IMAGE_X_BITS, tb_desc_bmp_bits,
			SERVER_IMAGE_DEPTH, 1,
			XV_WIDTH, tb_desc_bmp_width,
			XV_HEIGHT, tb_desc_bmp_height,
			NULL);
	}
	if (!tb_next_image) {
		tb_next_image = (Server_image) xv_create(NULL, SERVER_IMAGE,
			SERVER_IMAGE_X_BITS, tb_next_bmp_bits,
			SERVER_IMAGE_DEPTH, 1,
			XV_WIDTH, tb_next_bmp_width,
			XV_HEIGHT, tb_next_bmp_height,
			NULL);
	}
	if (!tb_prev_image) {
		tb_prev_image = (Server_image) xv_create(NULL, SERVER_IMAGE,
			SERVER_IMAGE_X_BITS, tb_prev_bmp_bits,
			SERVER_IMAGE_DEPTH, 1,
			XV_WIDTH, tb_prev_bmp_width,
			XV_HEIGHT, tb_prev_bmp_height,
			NULL);
	}
	if (!tb_run_image) {
		tb_run_image = (Server_image) xv_create(NULL, SERVER_IMAGE,
			SERVER_IMAGE_X_BITS, tb_run_bmp_bits,
			SERVER_IMAGE_DEPTH, 1,
			XV_WIDTH, tb_run_bmp_width,
			XV_HEIGHT, tb_run_bmp_height,
			NULL);
	}
	if (!tb_show_image) {
		tb_show_image = (Server_image) xv_create(NULL, SERVER_IMAGE,
			SERVER_IMAGE_X_BITS, tb_show_bmp_bits,
			SERVER_IMAGE_DEPTH, 1,
			XV_WIDTH, tb_show_bmp_width,
			XV_HEIGHT, tb_show_bmp_height,
			NULL);
	}
	if (!tb_nav_image) {
		tb_nav_image = (Server_image) xv_create(NULL, SERVER_IMAGE,
			SERVER_IMAGE_X_BITS, tb_nav_bmp_bits,
			SERVER_IMAGE_DEPTH, 1,
			XV_WIDTH, tb_nav_bmp_width,
			XV_HEIGHT, tb_nav_bmp_height,
			NULL);
	}
	if (!qmark_image) {
		qmark_image = (Server_image) xv_create(NULL, SERVER_IMAGE,
			SERVER_IMAGE_X_BITS, editor_qmark_bmp_bits,
			SERVER_IMAGE_DEPTH, 1,
			XV_WIDTH, editor_qmark_bmp_width,
			XV_HEIGHT, editor_qmark_bmp_height,
			NULL);
	}
	ed->icon = xv_create(0, ICON,
		ICON_IMAGE, icon_image,
		ICON_MASK_IMAGE, mask_image,
		ICON_TRANSPARENT, TRUE,
		0);

	xv_set(ed->icon,
		WIN_CONSUME_EVENTS,
		LOC_WINENTER,
		LOC_WINEXIT,
		LOC_DRAG,
		0,
		0);

	checkpoint = defaults_get_integer_check("text.checkpointFrequency",
		"Text.CheckpointFrequency", 0, 0, (int) TEXTSW_INFINITY);

	ed->id = get_next_editor_id();
	sprintf(szBuf, "apeX Editor #%d", ed->id);

	ed->frame = xv_create((Xv_window) NULL, FRAME,
		FRAME_NO_CONFIRM, FALSE,
		FRAME_ICON, ed->icon,
		FRAME_LABEL, szBuf,
		FRAME_SHOW_LABEL, TRUE,
		FRAME_SHOW_FOOTER, TRUE,
		FRAME_LEFT_FOOTER, "No Objects Selected",
	/* FRAME_RIGHT_FOOTER, "Destination Editor: new", */
		XV_KEY_DATA, CLIENT_DATA_KEY, ed,
		XV_X, 0,
		XV_Y, 0,
		0);


	/*
	 * Create subwindows
	 */

	ed->panel = xv_create(ed->frame, PANEL,
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		XV_HEIGHT, 60,
		0);


	ed->textsw = (Textsw) xv_create(ed->frame, TEXTSW,
		WIN_IS_CLIENT_PANE,
		TEXTSW_NOTIFY_PROC, set_name_frame,
		TEXTSW_CHECKPOINT_FREQUENCY, checkpoint,
		TEXTSW_CLIENT_DATA, (caddr_t) ed,
		TEXTSW_TAB_WIDTH, get_tabsize(),
		WIN_COLUMNS, 80,
		WIN_ROWS, fType,
		OPENWIN_SPLIT,
		OPENWIN_SPLIT_INIT_PROC, textsw_split_proc,
		NULL,
		NULL,
		0);

	/*
	 * force the panel to be some reasonable size, so that the buttons
	 * lay out reasonably.
	 */


	/*
	 * Install us in tree of windows
	 */

	if (!CLIENT_DATA_KEY) {
		CLIENT_DATA_KEY = xv_unique_key();
	}
	file_panel_menu = (Menu) xv_get(ed->textsw, TEXTSW_SUBMENU_FILE);

	save_menu_item = (Menu_item) xv_get(file_panel_menu,
		MENU_NTH_ITEM, SAVE_NUM);
	store_menu_item = (Menu_item) xv_get(file_panel_menu,
		MENU_NTH_ITEM, STORE_NUM);
	include_menu_item = (Menu_item) xv_get(file_panel_menu,
		MENU_NTH_ITEM, INCLUDE_NUM);
	new_menu_item = (Menu_item) xv_get(file_panel_menu,
		MENU_NTH_ITEM, NEW_NUM);
	load_menu_item = (Menu_item) xv_get(file_panel_menu, MENU_NTH_ITEM, 1);

	display_panel_menu = (Menu) xv_get(ed->textsw, TEXTSW_SUBMENU_VIEW);
	edit_panel_menu = (Menu) xv_get(ed->textsw, TEXTSW_SUBMENU_EDIT);
	find_panel_menu = (Menu) xv_get(ed->textsw, TEXTSW_SUBMENU_FIND);
	textsw_menu = (Menu) xv_get(ed->textsw, MENU);
	extra_menu = (Menu) xv_get(ed->textsw, TEXTSW_EXTRAS_CMD_MENU);

	ed->load_menu_item = (Menu_item) xv_create(NULL, MENUITEM,
		MENU_STRING, "Load File...",
		MENU_NOTIFY_PROC, create_load_browser_proc,
		MENU_RELEASE,
		MENU_CLIENT_DATA, ed,
		NULL);

	ed->save_menu_item = (Menu_item) xv_create(NULL, MENUITEM,
		MENU_STRING, "Save File",
		MENU_NOTIFY_PROC, save_existing_file_proc,
		MENU_RELEASE,
		MENU_CLIENT_DATA, ed,
		NULL);

	ed->save_as_menu_item = (Menu_item) xv_create(NULL, MENUITEM,
		MENU_STRING, "Save As New File...",
		MENU_NOTIFY_PROC, create_save_browser_proc,
		MENU_RELEASE,
		MENU_CLIENT_DATA, ed,
		NULL);


	ed->file_menu = file_panel_menu = (Menu) xv_create(NULL, MENU,
		MENU_GEN_PIN_WINDOW, ed->frame, "apeX: File",
		MENU_APPEND_ITEM, ed->load_menu_item,
		MENU_APPEND_ITEM, ed->save_menu_item,
		MENU_APPEND_ITEM, ed->save_as_menu_item,
		MENU_APPEND_ITEM, store_menu_item,
		MENU_APPEND_ITEM, include_menu_item,
		MENU_APPEND_ITEM, new_menu_item,
		MENU_CLIENT_DATA, ed,
		NULL);

	ed->file_button = xv_create(ed->panel, PANEL_BUTTON,
		PANEL_LABEL_STRING, "File",
		PANEL_ITEM_MENU, file_panel_menu,
		PANEL_NOTIFY_PROC, button_notify_proc,
		0);

	/* This set has to be done before the other panel items are created */
	xv_set(ed->file_button,
		XV_KEY_DATA, CLIENT_DATA_KEY, ed,
		0);

	menu_item = (Menu_item) xv_find(display_panel_menu, MENUITEM,
		MENU_STRING, "Destination Editor",
		XV_AUTO_CREATE, FALSE,
		NULL);

	/*
	 * if( !menu_item ) { menu_item = (Menu_item)xv_create(NULL,
	 * MENUITEM, MENU_STRING, "Destination Editor", MENU_GEN_PULLRIGHT,
	 * editor_menu_gen_proc, NULL );
	 * 
	 * xv_set( display_panel_menu, MENU_APPEND_ITEM, menu_item, NULL ); }
	 */
	ed->view_menu = display_panel_menu;

	ed->view_button = xv_create(ed->panel, PANEL_BUTTON,
		PANEL_LABEL_STRING, "View",
		PANEL_NOTIFY_PROC, button_notify_proc,
		PANEL_ITEM_MENU, display_panel_menu,
		XV_KEY_DATA, CLIENT_DATA_KEY, ed,
		0);

	ed->edit_button = xv_create(ed->panel, PANEL_BUTTON,
		PANEL_LABEL_STRING, "Edit",
		PANEL_NOTIFY_PROC, button_notify_proc,
		PANEL_ITEM_MENU, edit_panel_menu,
		XV_KEY_DATA, CLIENT_DATA_KEY, ed,
		0);

	ed->find_button = xv_create(ed->panel, PANEL_BUTTON,
		PANEL_LABEL_STRING, "Find",
		PANEL_NOTIFY_PROC, button_notify_proc,
		PANEL_ITEM_MENU, find_panel_menu,
		XV_KEY_DATA, CLIENT_DATA_KEY, ed,
		0);

	ed->extras_button = xv_create(ed->panel, PANEL_BUTTON,
		PANEL_LABEL_STRING, "Extras",
		PANEL_ITEM_MENU, extra_menu,
		PANEL_NOTIFY_PROC, button_notify_proc,
		XV_KEY_DATA, CLIENT_DATA_KEY, ed,
		NULL);

	ed->help_menu = (Menu) xv_create(NULL, MENU,
		MENU_ACTION_ITEM, "Help...", create_main_help,
		MENU_ACTION_ITEM, "Info...", info_proc,
		MENU_CLIENT_DATA, ed,
		NULL);

	ed->tools_menu = (Menu) xv_create(NULL, MENU,
		MENU_TITLE_ITEM, "apeX: Tools",
		MENU_GEN_PIN_WINDOW, ed->frame, "apeX: Tools Menu",
		MENU_ACTION_ITEM, "apeX Shell", show_shell,
		MENU_ACTION_ITEM, "apeX Navigator", create_navigator_proc,
		MENU_ACTION_ITEM, "apeX Editor", new_editor_proc,
		MENU_ACTION_ITEM, "apeX Scratch Pad", show_sp_proc,
		MENU_ITEM,
		MENU_STRING, "apeX Help",
		MENU_PULLRIGHT, ed->help_menu,
		NULL,
		XV_KEY_DATA, CLIENT_DATA_KEY, ed,
		NULL);

	ed->tools_button = xv_create(ed->panel, PANEL_BUTTON,
		PANEL_LABEL_STRING, "Tools",
		PANEL_ITEM_MENU, ed->tools_menu,
		XV_KEY_DATA, CLIENT_DATA_KEY, ed,
	/* XV_X, 400, */
		NULL);

	ed->dismiss_button = xv_create(ed->panel, PANEL_BUTTON,
		PANEL_LABEL_STRING, "Dismiss",
		PANEL_CLIENT_DATA, ed,
		PANEL_NOTIFY_PROC, dismiss_proc,
		NULL);

	textsw_menu = (Menu) xv_create(NULL, MENU,
		MENU_TITLE_ITEM, "apeX",
		MENU_GEN_PIN_WINDOW, ed->frame, "apeX",
		MENU_PULLRIGHT_ITEM, "File", file_panel_menu,
		MENU_PULLRIGHT_ITEM, "View", display_panel_menu,
		MENU_PULLRIGHT_ITEM, "Edit", edit_panel_menu,
		MENU_PULLRIGHT_ITEM, "Find", find_panel_menu,
		MENU_PULLRIGHT_ITEM, "Extras", extra_menu,
		MENU_ITEM,
		MENU_STRING, "",
		MENU_FEEDBACK, FALSE,
		NULL,
		MENU_PULLRIGHT_ITEM, "Tools", ed->tools_menu,
		NULL);

	xv_set(ed->textsw, TEXTSW_MENU, textsw_menu, NULL);

	xv_set(ed->panel, XV_WIDTH, (int) xv_get(ed->textsw, XV_WIDTH), NULL);

	xv_set(ed->panel, PANEL_LAYOUT, PANEL_VERTICAL, NULL);

	ed->tb_run_button = (Panel_item) xv_create(ed->panel, PANEL_BUTTON,
		PANEL_LABEL_IMAGE, tb_run_image,
		PANEL_CLIENT_DATA, ed,
		PANEL_ITEM_Y_GAP, 5,
		PANEL_NOTIFY_PROC, obj_run_proc,
		NULL);

	xv_set(ed->panel, PANEL_LAYOUT, PANEL_HORIZONTAL, NULL);

	ed->tb_desc_button = (Panel_item) xv_create(ed->panel, PANEL_BUTTON,
		PANEL_LABEL_IMAGE, tb_desc_image,
		PANEL_CLIENT_DATA, ed,
		PANEL_NOTIFY_PROC, obj_desc_proc,
		NULL);

	ed->tb_show_button = (Panel_item) xv_create(ed->panel, PANEL_BUTTON,
		PANEL_LABEL_IMAGE, tb_show_image,
		PANEL_CLIENT_DATA, ed,
		PANEL_NOTIFY_PROC, obj_show_proc,
		NULL);

	ed->tb_prev_button = (Panel_item) xv_create(ed->panel, PANEL_BUTTON,
		PANEL_LABEL_IMAGE, tb_prev_image,
		PANEL_CLIENT_DATA, ed,
		PANEL_NOTIFY_PROC, obj_prev_proc,
		NULL);

	ed->tb_next_button = (Panel_item) xv_create(ed->panel, PANEL_BUTTON,
		PANEL_LABEL_IMAGE, tb_next_image,
		PANEL_CLIENT_DATA, ed,
		PANEL_NOTIFY_PROC, obj_next_proc,
		NULL);

	ed->tb_create_button = (Panel_item) xv_create(ed->panel, PANEL_BUTTON,
		PANEL_LABEL_IMAGE, tb_create_image,
		PANEL_CLIENT_DATA, ed,
		PANEL_NOTIFY_PROC, show_oc,
		NULL);

	ed->tb_delete_button = (Panel_item) xv_create(ed->panel, PANEL_BUTTON,
		PANEL_LABEL_IMAGE, tb_delete_image,
		PANEL_CLIENT_DATA, ed,
		PANEL_NOTIFY_PROC, obj_delete_proc,
		NULL);

	ed->tb_hide_button = (Panel_item) xv_create(ed->panel, PANEL_BUTTON,
		PANEL_LABEL_IMAGE, tb_hide_image,
		PANEL_CLIENT_DATA, ed,
		PANEL_NOTIFY_PROC, create_olp_proc,
		NULL);

	ed->tb_csh_button = (Panel_item) xv_create(ed->panel, PANEL_BUTTON,
		PANEL_LABEL_IMAGE, tb_csh_image,
		PANEL_ITEM_X_GAP, 20,
		PANEL_CLIENT_DATA, ed,
		PANEL_NOTIFY_PROC, show_shell,
		NULL);

	ed->tb_nav_button = (Panel_item) xv_create(ed->panel, PANEL_BUTTON,
		PANEL_LABEL_IMAGE, tb_nav_image,
		XV_KEY_DATA, CLIENT_DATA_KEY, ed,
		PANEL_NOTIFY_PROC, create_navigator_proc,
		NULL);

	ed->tb_apex_button = (Panel_item) xv_create(ed->panel, PANEL_BUTTON,
		PANEL_LABEL_IMAGE, tb_apex_image,
		PANEL_CLIENT_DATA, ed,
		PANEL_NOTIFY_PROC, new_editor_proc,
		NULL);

	ed->tb_sp_button = (Panel_item) xv_create(ed->panel, PANEL_BUTTON,
		PANEL_LABEL_IMAGE, tb_sp_image,
		XV_KEY_DATA, CLIENT_DATA_KEY, ed,
		PANEL_NOTIFY_PROC, show_sp_proc,
		NULL);

	ed->help_button = xv_create(ed->panel, PANEL_BUTTON,
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_LABEL_IMAGE, qmark_image,
		PANEL_ITEM_X_GAP, 20,
		PANEL_ITEM_MENU, ed->help_menu,
		NULL);

	window_fit_height(ed->panel);

	xv_set(ed->textsw, WIN_BELOW, ed->panel, 0);


	window_fit(ed->frame);

	/*
	 * now we set the function to trap the 'quit' option on the main
	 * window bar
	 */
	notify_interpose_destroy_func(ed->frame, editor_done);

	return ed;
}

/*
 * new_editor_proc -- create a new editor and tell the state server about it
 */
static void
new_editor_proc()
{
	Editor         *ped;

	new_editor(NULL, 0, EDITOR_MAIN);
}

/*
 * new_editor -- will create a new editor of type type displaying the file at
 * lineno
 */
Editor         *
new_editor(szFilename, line, fType)
    char           *szFilename;
    int             line,
                    fType;
{
	Editor         *ped;

	ped = create_editor(fType);
	add_editor(ped);

	set_editor(ped, szFilename, line);

	xv_set(ped->frame, XV_SHOW, TRUE, NULL);

	return ped;
}

/*
 * set_editor -- set a given editor to load a file and focus at line
 */
void
set_editor(ped, szFullname, line)
    Editor         *ped;
    char           *szFullname;
    int             line;
{
	int             answer;
	char            szBuf[1024];

	if (szFullname == NULL) {
		return;
	}
	destroy_file(ped->pFile);

	if (ped->fEdited) {
		answer = notice_prompt(ped->frame, NULL,
			NOTICE_MESSAGE_STRINGS,
			"Warning! Your edits have not been saved.",
			NULL,
			NOTICE_BUTTON_YES, "Cancel",
			NOTICE_BUTTON_NO, "Discard Edits",
			NULL);
		if (answer != NOTICE_NO)
			return;
	}
	if (!(ped->pFile = load_file(szFullname))) {
		sprintf(szBuf, "Trouble loading file '%s'.", szFullname);
		answer = notice_prompt(ped->frame, NULL,
			NOTICE_MESSAGE_STRINGS,
			szBuf,
			"File may not exist or may not be readable",
			NULL,
			NOTICE_BUTTON_YES, "OK",
			NULL);
		return;
	}
	if (!fFiletoTextsw(ped->pFile, ped)) {
		sprintf(szBuf, "Trouble loading file '%s'.", szFullname);
		answer = notice_prompt(ped->frame, NULL,
			NOTICE_MESSAGE_STRINGS,
			szBuf,
			"File may not exist or may not be readable",
			NULL,
			NOTICE_BUTTON_YES, "OK",
			NULL);
		return;
	}
	textsw_normalize_view(ped->textsw,
		textsw_index_for_file_line(ped->textsw, line));

	/* now we want to add the filename to the menu */

	add_filename_to_menu(ped, szFullname);

	/* now update the editor view for object links and such */

	update_object_status(ped);
}

/*
 * add the file name to the File menu ala MS word
 */
static void
add_filename_to_menu(ped, szFullname)
    Editor         *ped;
    char           *szFullname;
{
	Menu_item       mi;

	mi = (Menu_item) xv_find(ped->file_menu, MENUITEM,
		MENU_STRING, szFullname,
		XV_AUTO_CREATE, FALSE,
		NULL);

	if (!mi) {
		mi = (Menu_item) xv_create(NULL, MENUITEM,
			MENU_STRING, strdup(szFullname),
			MENU_NOTIFY_PROC, file_name_proc,
			MENU_CLIENT_DATA, ped,
			NULL);

		xv_set(ped->file_menu, MENU_APPEND_ITEM, mi, NULL);
	}
}

/*
 * file_name_proc -- called when the menu item added in add_filename_to_menu
 * is pushed
 */
static void
file_name_proc(menu, mi)
    Menu            menu;
    Menu_item       mi;
{
	Editor         *ped;

	ped = (Editor *) xv_get(mi, MENU_CLIENT_DATA);

	set_editor(ped, (char *) xv_get(mi, MENU_STRING), 0);
}


/*
 * shut_subframes -- go through popups and set them to NULL
 */
void 
shut_subframes(frame)
    Frame           frame;
{
	Frame           subframe;
	int             i = 0;

	/* well, this is not too important to do yet...someday make it work */

	/*
	 * while( (subframe = (Frame)xv_get(frame, FRAME_NTH_SUBFRAME, i)) ){
	 * xv_set(subframe, XV_SHOW, FALSE, NULL ); }
	 */
}

/*
 * editor_done -- called when the editor window is closed so that we can
 * update our state accordingly
 */
Notify_value
editor_done(client, status)
    Notify_client   client;
    Destroy_status  status;
{
	Editor         *ped;


	if (client == get_shell_frame() && status == DESTROY_CHECKING) {
		xv_set(client, XV_SHOW, FALSE, NULL);
		shut_subframes(client);
		notify_veto_destroy(client);
		return NOTIFY_DONE;
	}
	if (status == DESTROY_CHECKING) {
		if ((ped = get_editor_by_client(client)) && ped->fEdited) {
			int             answer = notice_prompt(client, NULL,
				NOTICE_MESSAGE_STRINGS,
				"Warning! Your edits have not been saved.",
				NULL,
				NOTICE_BUTTON_YES, "Cancel",
				NOTICE_BUTTON_NO, "Discard Edits",
				NULL);

			if (answer != NOTICE_NO) {
				notify_veto_destroy(client);
				return NOTIFY_DONE;
			}
		}
		if (fLast_Editor()) {
			int             answer = notice_prompt(client, NULL,
				NOTICE_MESSAGE_STRINGS,
				"Do you really want to quit apeX?", NULL,
				NOTICE_BUTTON_YES, "Hell Yes!",
				NOTICE_BUTTON_NO, "Ooops!",
				NULL);

			if (answer != NOTICE_YES) {
				notify_veto_destroy(client);
				return NOTIFY_DONE;
			} else {
				exit(0);
			}
		}
		xv_set(client, XV_SHOW, FALSE, NULL);
		shut_subframes(client);
		remove_editor_by_client(client);
		notify_veto_destroy(client);
	} else if (status == DESTROY_CLEANUP) {
		xv_set(client, XV_SHOW, FALSE, NULL);
		shut_subframes(client);
		remove_editor_by_client(client);
		/* notify_next_destroy_func(client, status); */
	} else if (status == DESTROY_SAVE_YOURSELF) {
		printf("got save\n");
	} else {
		printf("ahhhhhhhh! Somebody is trying to kill us...\n");
	}

	return NOTIFY_DONE;
}

/*
 * editor_menu_gen_proc -- generate a menu listing available editors
 */
Menu 
editor_menu_gen_proc(menu_item, op)
    Menu_item       menu_item;
    Menu_generate   op;
{
	Menu            menu;
	Menu_item       mi;
	Editor        **rgped,
	              **rgped_head,
	               *ped;
	char            szBuf[100];
	void           *notify_proc;

	if (op == MENU_DISPLAY) {
		menu = (Menu) xv_get(menu_item, XV_OWNER);
		ped = (Editor *) xv_get(menu_item, MENU_CLIENT_DATA);
		notify_proc = (void *) xv_get(menu_item, MENU_NOTIFY_PROC);
		/* get old menu and free it -- we're going to build another */
		if (menu = (Menu) xv_get(menu_item, MENU_PULLRIGHT)) {
			xv_destroy(menu);
		}
		if (ped && get_editor_by_client(ped->frame)) {
			sprintf(szBuf, "Editor #%d: %s", ped->id, ped->szFilename);
			menu = (Menu) xv_create(NULL, MENU,
				MENU_ITEM,
				MENU_STRING, strdup(szBuf),
				MENU_CLIENT_DATA, ped,
				NULL,
				MENU_ITEM,
				MENU_STRING, "New Editor",
				MENU_CLIENT_DATA, 0,
				NULL,
				MENU_NOTIFY_PROC, notify_proc,
				NULL);

		} else
			menu = (Menu) xv_create(NULL, MENU,
				MENU_ITEM,
				MENU_STRING, "New Editor",
				MENU_CLIENT_DATA, 0,
				NULL,
				NULL);

		rgped = rgped_head = get_editor_array();
		for (; *rgped != NULL; rgped++) {
			if (ped != *rgped) {
				sprintf(szBuf, "Editor #%d: %s", (*rgped)->id,
					(*rgped)->szFilename);
				mi = (Menu_item) xv_create(NULL, MENUITEM,
					MENU_STRING, strdup(szBuf),
					MENU_CLIENT_DATA, *rgped,
					NULL);
				xv_set(menu, MENU_APPEND_ITEM, mi, NULL);
			}
		}
		xv_set(menu_item, MENU_PULLRIGHT, menu, NULL);

	}
	if (!(menu = (Menu) xv_get(menu_item, MENU_PULLRIGHT)))
		menu = (Menu) xv_create(NULL, MENU,
			MENU_STRINGS, "Error building menu...", NULL,
			NULL);
	return menu;
}
