/*
 * props_ui.c - User interface object initialization functions.
 * This file was generated by `gxv' from `props.G'.
 * DO NOT EDIT BY HAND.
 */

#include <stdio.h>
#include <sys/param.h>
#include <sys/types.h>
#include <xview/xview.h>
#include <xview/canvas.h>
#include <xview/panel.h>
#include <xview/scrollbar.h>
#include <xview/svrimage.h>
#include <xview/termsw.h>
#include <xview/text.h>
#include <xview/tty.h>
#include <xview/xv_xrect.h>
#include <group.h>
#include "props_ui.h"

/*
 * Initialize an instance of object `popup'.
 */
props_popup_objects *
props_popup_objects_initialize(ip, owner)
	props_popup_objects	*ip;
	Xv_opaque	owner;
{
	if (!ip && !(ip = (props_popup_objects *) calloc(1, sizeof (props_popup_objects))))
		return (props_popup_objects *) NULL;
	if (!ip->popup)
		ip->popup = props_popup_popup_create(ip, owner);
	if (!ip->category_controls)
		ip->category_controls = props_popup_category_controls_create(ip, ip->popup);
	if (!ip->category)
		ip->category = props_popup_category_create(ip, ip->category_controls);
	if (!ip->mouse_controls)
		ip->mouse_controls = props_popup_mouse_controls_create(ip, ip->popup);
	if (!ip->scrollbar_jump)
		ip->scrollbar_jump = props_popup_scrollbar_jump_create(ip, ip->mouse_controls);
	if (!ip->popup_jump)
		ip->popup_jump = props_popup_popup_jump_create(ip, ip->mouse_controls);
	if (!ip->multi_click)
		ip->multi_click = props_popup_multi_click_create(ip, ip->mouse_controls);
	if (!ip->seconds)
		ip->seconds = props_popup_seconds_create(ip, ip->mouse_controls);
	if (!ip->group1)
		ip->group1 = props_popup_group1_create(ip, ip->mouse_controls);
	if (!ip->group2)
		ip->group2 = props_popup_group2_create(ip, ip->mouse_controls);
	window_fit(ip->mouse_controls);
	
	if (!ip->icon_controls)
		ip->icon_controls = props_popup_icon_controls_create(ip, ip->popup);
	if (!ip->icon_location)
		ip->icon_location = props_popup_icon_location_create(ip, ip->icon_controls);
	if (!ip->misc_controls)
		ip->misc_controls = props_popup_misc_controls_create(ip, ip->popup);
	if (!ip->beep)
		ip->beep = props_popup_beep_create(ip, ip->misc_controls);
	if (!ip->set_input)
		ip->set_input = props_popup_set_input_create(ip, ip->misc_controls);
	if (!ip->scrollbar)
		ip->scrollbar = props_popup_scrollbar_create(ip, ip->misc_controls);
	if (!ip->group)
		ip->group = props_popup_group_create(ip, ip->misc_controls);
	window_fit(ip->misc_controls);
	
	if (!ip->menu_controls)
		ip->menu_controls = props_popup_menu_controls_create(ip, ip->popup);
	if (!ip->drag_right)
		ip->drag_right = props_popup_drag_right_create(ip, ip->menu_controls);
	if (!ip->pixels)
		ip->pixels = props_popup_pixels_create(ip, ip->menu_controls);
	if (!ip->group3)
		ip->group3 = props_popup_group3_create(ip, ip->menu_controls);
	if (!ip->select)
		ip->select = props_popup_select_create(ip, ip->menu_controls);
	if (!ip->group4)
		ip->group4 = props_popup_group4_create(ip, ip->menu_controls);
	window_fit(ip->menu_controls);
	
	if (!ip->apply_reset_controls)
		ip->apply_reset_controls = props_popup_apply_reset_controls_create(ip, ip->popup);
	if (!ip->apply)
		ip->apply = props_popup_apply_create(ip, ip->apply_reset_controls);
	if (!ip->reset)
		ip->reset = props_popup_reset_create(ip, ip->apply_reset_controls);
	if (!ip->group5)
		ip->group5 = props_popup_group5_create(ip, ip->apply_reset_controls);
	window_fit(ip->apply_reset_controls);
	
	window_fit(ip->popup);
	
	xv_set(ip->category_controls, XV_WIDTH, WIN_EXTEND_TO_EDGE, NULL);
	xv_set(ip->mouse_controls, XV_WIDTH, WIN_EXTEND_TO_EDGE, NULL);
	xv_set(ip->icon_controls, XV_WIDTH, WIN_EXTEND_TO_EDGE, NULL);
	xv_set(ip->misc_controls, XV_WIDTH, WIN_EXTEND_TO_EDGE, NULL);
	xv_set(ip->menu_controls, XV_WIDTH, WIN_EXTEND_TO_EDGE, NULL);
	xv_set(ip->apply_reset_controls,
		XV_WIDTH, WIN_EXTEND_TO_EDGE,
		XV_HEIGHT, WIN_EXTEND_TO_EDGE,
		NULL);
	return ip;
}

/*
 * Create object `popup' in the specified instance.
 */
Xv_opaque
props_popup_popup_create(ip, owner)
	props_popup_objects	*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, FRAME_CMD,
		XV_KEY_DATA, INSTANCE, ip,
		XV_WIDTH, 427,
		XV_HEIGHT, 288,
		XV_LABEL, "Desktop Properties",
		FRAME_SHOW_FOOTER, FALSE,
		FRAME_SHOW_RESIZE_CORNER, FALSE,
		FRAME_CMD_PUSHPIN_IN, FALSE,
		NULL);
	xv_set(xv_get(obj, FRAME_CMD_PANEL), WIN_SHOW, FALSE, NULL);
	return obj;
}

/*
 * Create object `category_controls' in the specified instance.
 */
Xv_opaque
props_popup_category_controls_create(ip, owner)
	props_popup_objects	*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 0,
		XV_Y, 0,
		XV_WIDTH, WIN_EXTEND_TO_EDGE,
		XV_HEIGHT, 40,
		WIN_BORDER, FALSE,
		NULL);
	return obj;
}

/*
 * Create object `category' in the specified instance.
 */
Xv_opaque
props_popup_category_create(ip, owner)
	props_popup_objects	*ip;
	Xv_opaque	owner;
{
	extern void		props_popup_category_notify_callback();
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_CHOICE, PANEL_DISPLAY_LEVEL, PANEL_CURRENT,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 24,
		XV_Y, 8,
		PANEL_CHOICE_NROWS, 1,
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_LABEL_STRING, "Category:",
		PANEL_NOTIFY_PROC, props_popup_category_notify_callback,
		PANEL_CHOICE_STRINGS,
			"Icons",
			"Menus",
			"Miscellaneous",
			"Mouse Settings",
			NULL,
		PANEL_VALUE, 0,
		NULL);
	return obj;
}

/*
 * Create object `mouse_controls' in the specified instance.
 */
Xv_opaque
props_popup_mouse_controls_create(ip, owner)
	props_popup_objects	*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 0,
		XV_Y, 39,
		XV_WIDTH, WIN_EXTEND_TO_EDGE,
		XV_HEIGHT, 193,
		XV_SHOW, FALSE,
		WIN_BORDER, FALSE,
		NULL);
	return obj;
}

/*
 * Create object `scrollbar_jump' in the specified instance.
 */
Xv_opaque
props_popup_scrollbar_jump_create(ip, owner)
	props_popup_objects	*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_TOGGLE, PANEL_FEEDBACK, PANEL_MARKED,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 24,
		XV_Y, 40,
		PANEL_CHOICE_NCOLS, 1,
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_LABEL_STRING, "Scrollbar Pointer Jumping:",
		PANEL_CHOICE_STRING, 0, "",
		PANEL_VALUE, 0,
		NULL);
	return obj;
}

/*
 * Create object `popup_jump' in the specified instance.
 */
Xv_opaque
props_popup_popup_jump_create(ip, owner)
	props_popup_objects	*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_TOGGLE, PANEL_FEEDBACK, PANEL_MARKED,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 35,
		XV_Y, 73,
		PANEL_CHOICE_NCOLS, 1,
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_LABEL_STRING, "Pop-up Pointer Jumping:",
		PANEL_CHOICE_STRING, 0, "",
		PANEL_VALUE, 0,
		NULL);
	return obj;
}

/*
 * Create object `multi_click' in the specified instance.
 */
Xv_opaque
props_popup_multi_click_create(ip, owner)
	props_popup_objects	*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_SLIDER,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 65,
		XV_Y, 106,
		PANEL_SLIDER_WIDTH, 50,
		PANEL_TICKS, 0,
		PANEL_LABEL_STRING, "Multi-click Timeout:",
		PANEL_DIRECTION, PANEL_HORIZONTAL,
		PANEL_SLIDER_END_BOXES, FALSE,
		PANEL_SHOW_RANGE, TRUE,
		PANEL_SHOW_VALUE, TRUE,
		PANEL_MIN_VALUE, 2,
		PANEL_MAX_VALUE, 10,
		PANEL_VALUE, 0,
		NULL);
	return obj;
}

/*
 * Create object `seconds' in the specified instance.
 */
Xv_opaque
props_popup_seconds_create(ip, owner)
	props_popup_objects	*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_MESSAGE,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 361,
		XV_Y, 106,
		PANEL_LABEL_STRING, "sec/10",
		PANEL_LABEL_BOLD, TRUE,
		NULL);
	return obj;
}

/*
 * Create object `group1' in the specified instance.
 */
Xv_opaque
props_popup_group1_create(ip, owner)
	props_popup_objects	*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, GROUP,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 60,
		XV_Y, 106,
		GROUP_TYPE, GROUP_ROW,
		GROUP_MEMBERS,
			ip->multi_click,
			ip->seconds,
			NULL,
		GROUP_ROW_ALIGNMENT, GROUP_TOP_EDGES,
		GROUP_HORIZONTAL_SPACING, 10,
		NULL);
	return obj;
}

/*
 * Create object `group2' in the specified instance.
 */
Xv_opaque
props_popup_group2_create(ip, owner)
	props_popup_objects	*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, GROUP,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 24,
		XV_Y, 40,
		GROUP_TYPE, GROUP_COLUMN,
		GROUP_MEMBERS,
			ip->scrollbar_jump,
			ip->popup_jump,
			ip->group1,
			NULL,
		GROUP_COLUMN_ALIGNMENT, GROUP_LABELS,
		GROUP_VERTICAL_SPACING, 10,
		NULL);
	return obj;
}

/*
 * Create object `icon_controls' in the specified instance.
 */
Xv_opaque
props_popup_icon_controls_create(ip, owner)
	props_popup_objects	*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 0,
		XV_Y, 39,
		XV_WIDTH, WIN_EXTEND_TO_EDGE,
		XV_HEIGHT, 193,
		WIN_BORDER, FALSE,
		NULL);
	return obj;
}

/*
 * Create object `icon_location' in the specified instance.
 */
Xv_opaque
props_popup_icon_location_create(ip, owner)
	props_popup_objects	*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_CHOICE,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 80,
		XV_Y, 80,
		PANEL_CHOICE_NROWS, 1,
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_CHOOSE_NONE, FALSE,
		PANEL_LABEL_STRING, "Location:",
		PANEL_CHOICE_STRINGS,
			"Top",
			"Bottom",
			"Left",
			"Right",
			NULL,
		NULL);
	return obj;
}

/*
 * Create object `misc_controls' in the specified instance.
 */
Xv_opaque
props_popup_misc_controls_create(ip, owner)
	props_popup_objects	*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 0,
		XV_Y, 39,
		XV_WIDTH, WIN_EXTEND_TO_EDGE,
		XV_HEIGHT, 193,
		XV_SHOW, FALSE,
		WIN_BORDER, FALSE,
		NULL);
	return obj;
}

/*
 * Create object `beep' in the specified instance.
 */
Xv_opaque
props_popup_beep_create(ip, owner)
	props_popup_objects	*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_CHOICE,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 127,
		XV_Y, 40,
		PANEL_CHOICE_NROWS, 1,
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_CHOOSE_NONE, FALSE,
		PANEL_LABEL_STRING, "Beep:",
		PANEL_CHOICE_STRINGS,
			"Always",
			"Notices Only",
			"Never",
			NULL,
		NULL);
	return obj;
}

/*
 * Create object `set_input' in the specified instance.
 */
Xv_opaque
props_popup_set_input_create(ip, owner)
	props_popup_objects	*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_CHOICE,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 64,
		XV_Y, 73,
		PANEL_CHOICE_NROWS, 1,
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_CHOOSE_NONE, FALSE,
		PANEL_LABEL_STRING, "Set Input Area:",
		PANEL_CHOICE_STRINGS,
			"Click SELECT",
			"Move Pointer",
			NULL,
		NULL);
	return obj;
}

/*
 * Create object `scrollbar' in the specified instance.
 */
Xv_opaque
props_popup_scrollbar_create(ip, owner)
	props_popup_objects	*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_CHOICE,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 24,
		XV_Y, 106,
		PANEL_CHOICE_NROWS, 1,
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_CHOOSE_NONE, FALSE,
		PANEL_LABEL_STRING, "Scrollbar Placement:",
		PANEL_CHOICE_STRINGS,
			"Left",
			"Right",
			NULL,
		NULL);
	return obj;
}

/*
 * Create object `group' in the specified instance.
 */
Xv_opaque
props_popup_group_create(ip, owner)
	props_popup_objects	*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, GROUP,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 24,
		XV_Y, 40,
		GROUP_TYPE, GROUP_COLUMN,
		GROUP_MEMBERS,
			ip->beep,
			ip->set_input,
			ip->scrollbar,
			NULL,
		GROUP_COLUMN_ALIGNMENT, GROUP_LABELS,
		GROUP_VERTICAL_SPACING, 10,
		NULL);
	return obj;
}

/*
 * Create object `menu_controls' in the specified instance.
 */
Xv_opaque
props_popup_menu_controls_create(ip, owner)
	props_popup_objects	*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 0,
		XV_Y, 39,
		XV_WIDTH, WIN_EXTEND_TO_EDGE,
		XV_HEIGHT, 193,
		XV_SHOW, FALSE,
		WIN_BORDER, FALSE,
		NULL);
	return obj;
}

/*
 * Create object `drag_right' in the specified instance.
 */
Xv_opaque
props_popup_drag_right_create(ip, owner)
	props_popup_objects	*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_TEXT,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 40,
		XV_Y, 48,
		PANEL_VALUE_DISPLAY_LENGTH, 8,
		PANEL_VALUE_STORED_LENGTH, 80,
		PANEL_LABEL_STRING, "Drag-Right distance:",
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_READ_ONLY, FALSE,
		NULL);
	return obj;
}

/*
 * Create object `pixels' in the specified instance.
 */
Xv_opaque
props_popup_pixels_create(ip, owner)
	props_popup_objects	*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_MESSAGE,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 263,
		XV_Y, 48,
		PANEL_LABEL_STRING, "pixels",
		PANEL_LABEL_BOLD, TRUE,
		NULL);
	return obj;
}

/*
 * Create object `group3' in the specified instance.
 */
Xv_opaque
props_popup_group3_create(ip, owner)
	props_popup_objects	*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, GROUP,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 40,
		XV_Y, 48,
		GROUP_TYPE, GROUP_ROW,
		GROUP_MEMBERS,
			ip->drag_right,
			ip->pixels,
			NULL,
		GROUP_ROW_ALIGNMENT, GROUP_TOP_EDGES,
		GROUP_HORIZONTAL_SPACING, 10,
		NULL);
	return obj;
}

/*
 * Create object `select' in the specified instance.
 */
Xv_opaque
props_popup_select_create(ip, owner)
	props_popup_objects	*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_CHOICE,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 40,
		XV_Y, 73,
		PANEL_CHOICE_NROWS, 1,
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_CHOOSE_NONE, FALSE,
		PANEL_LABEL_STRING, "SELECT Mouse Press",
		PANEL_CHOICE_STRINGS,
			"Selects Default",
			"Displays Menu",
			NULL,
		NULL);
	return obj;
}

/*
 * Create object `group4' in the specified instance.
 */
Xv_opaque
props_popup_group4_create(ip, owner)
	props_popup_objects	*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, GROUP,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 40,
		XV_Y, 48,
		GROUP_TYPE, GROUP_COLUMN,
		GROUP_MEMBERS,
			ip->group3,
			ip->select,
			NULL,
		GROUP_COLUMN_ALIGNMENT, GROUP_LABELS,
		GROUP_VERTICAL_SPACING, 10,
		NULL);
	return obj;
}

/*
 * Create object `apply_reset_controls' in the specified instance.
 */
Xv_opaque
props_popup_apply_reset_controls_create(ip, owner)
	props_popup_objects	*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 0,
		XV_Y, (int)xv_get(ip->mouse_controls, XV_Y) +
		      (int)xv_get(ip->mouse_controls, XV_HEIGHT),
		XV_WIDTH, WIN_EXTEND_TO_EDGE,
		XV_HEIGHT, WIN_EXTEND_TO_EDGE,
		WIN_BORDER, FALSE,
		NULL);
	return obj;
}

/*
 * Create object `apply' in the specified instance.
 */
Xv_opaque
props_popup_apply_create(ip, owner)
	props_popup_objects	*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_BUTTON,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 156,
		XV_Y, 19,
		PANEL_LABEL_STRING, "Apply",
		NULL);
	return obj;
}

/*
 * Create object `reset' in the specified instance.
 */
Xv_opaque
props_popup_reset_create(ip, owner)
	props_popup_objects	*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_BUTTON,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 219,
		XV_Y, 19,
		PANEL_LABEL_STRING, "Reset",
		NULL);
	return obj;
}

/*
 * Create object `group5' in the specified instance.
 */
Xv_opaque
props_popup_group5_create(ip, owner)
	props_popup_objects	*ip;
	Xv_opaque	owner;
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, GROUP,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 156,
		XV_Y, 19,
		GROUP_TYPE, GROUP_ROW,
		GROUP_MEMBERS,
			ip->apply,
			ip->reset,
			NULL,
		GROUP_ROW_ALIGNMENT, GROUP_TOP_EDGES,
		GROUP_HORIZONTAL_SPACING, 10,
		NULL);
	return obj;
}

