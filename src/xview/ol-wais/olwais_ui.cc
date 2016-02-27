//
// olwais_ui.cc - User interface object initialization functions.
// This file was generated by `gxv++' from `olwais.G'.
// DO NOT EDIT BY HAND.
//

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
#include <xview/dragdrop.h>
#include <xview/cursor.h>
#include <gdd.h>
#include <gcm.h>
#include "olwais_ui.h"

//
// Create object `FileMenu' in the specified instance.
//
Xv_opaque
olwais_FileMenu_create(caddr_t ip, Xv_opaque owner)
{
	extern Menu_item	olwais_FileMenu_item0_callback(Menu_item, Menu_generate);
	extern Menu_item	olwais_FileMenu_item2_callback(Menu_item, Menu_generate);
	Xv_opaque	obj;
	
	obj = xv_create(XV_NULL, MENU_COMMAND_MENU,
		XV_KEY_DATA, INSTANCE, ip,
		MENU_ITEM,
			XV_KEY_DATA, INSTANCE, ip,
			MENU_STRING, "Load Question.....",
			MENU_GEN_PROC, olwais_FileMenu_item0_callback,
			NULL,
		MENU_ITEM,
			XV_KEY_DATA, INSTANCE, ip,
			MENU_STRING, "Save Question....",
			NULL,
		MENU_ITEM,
			XV_KEY_DATA, INSTANCE, ip,
			MENU_STRING, "Quit",
			MENU_GEN_PROC, olwais_FileMenu_item2_callback,
			NULL,
		MENU_DEFAULT, 1,
		NULL);
	return obj;
}

//
// Initialize an instance of object `olw'.
//
void
olwais_olw_objects::objects_initialize(Xv_opaque owner)
{
	olw = olw_create(owner);
	MenusPanel = MenusPanel_create(olw);
	FileMenuButton = FileMenuButton_create(MenusPanel);
	SearchPanel = SearchPanel_create(olw);
	KeywordsText = KeywordsText_create(SearchPanel);
	SearchButton = SearchButton_create(SearchPanel);
	RelevanceFeedbackList = RelevanceFeedbackList_create(SearchPanel);
	SelectSourcesButton = SelectSourcesButton_create(SearchPanel);
}

//
// Create object `olw' in the specified instance.
//
Xv_opaque
olwais_olw_objects::olw_create(Xv_opaque owner)
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, FRAME,
		XV_KEY_DATA, INSTANCE, this,
		XV_WIDTH, 894,
		XV_HEIGHT, 230,
		XV_LABEL, "Open Look Wais Version 2",
		FRAME_SHOW_FOOTER, FALSE,
		FRAME_SHOW_RESIZE_CORNER, TRUE,
		NULL);
	return obj;
}

//
// Create object `MenusPanel' in the specified instance.
//
Xv_opaque
olwais_olw_objects::MenusPanel_create(Xv_opaque owner)
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL,
		XV_KEY_DATA, INSTANCE, this,
		XV_X, 0,
		XV_Y, 0,
		XV_WIDTH, WIN_EXTEND_TO_EDGE,
		XV_HEIGHT, 35,
		WIN_BORDER, TRUE,
		NULL);
	return obj;
}

//
// Create object `FileMenuButton' in the specified instance.
//
Xv_opaque
olwais_olw_objects::FileMenuButton_create(Xv_opaque owner)
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_BUTTON,
		XV_KEY_DATA, INSTANCE, this,
		XV_X, 8,
		XV_Y, 8,
		PANEL_LABEL_STRING, "File",
		PANEL_ITEM_MENU, olwais_FileMenu_create((caddr_t) this, olw),
		NULL);
	return obj;
}

//
// Create object `SearchPanel' in the specified instance.
//
Xv_opaque
olwais_olw_objects::SearchPanel_create(Xv_opaque owner)
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL,
		XV_KEY_DATA, INSTANCE, this,
		XV_X, 0,
		XV_Y, (int)xv_get(MenusPanel, XV_Y) +
		      (int)xv_get(MenusPanel, XV_HEIGHT) + 2,
		XV_WIDTH, WIN_EXTEND_TO_EDGE,
		XV_HEIGHT, WIN_EXTEND_TO_EDGE,
		WIN_BORDER, FALSE,
		NULL);
	notify_interpose_event_func(obj,
		(Notify_func) gdd_load_event_proc, NOTIFY_SAFE);
	return obj;
}

//
// Create object `KeywordsText' in the specified instance.
//
Xv_opaque
olwais_olw_objects::KeywordsText_create(Xv_opaque owner)
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_TEXT,
		XV_KEY_DATA, INSTANCE, this,
		XV_X, 16,
		XV_Y, 16,
		PANEL_VALUE_DISPLAY_LENGTH, 75,
		PANEL_VALUE_STORED_LENGTH, 80,
		PANEL_LABEL_STRING, "Tell me about",
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_READ_ONLY, FALSE,
		NULL);
	return obj;
}

//
// Create object `SearchButton' in the specified instance.
//
Xv_opaque
olwais_olw_objects::SearchButton_create(Xv_opaque owner)
{
	extern void		DoSearch(Panel_item, Event *);
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_BUTTON,
		XV_KEY_DATA, INSTANCE, this,
		XV_X, 768,
		XV_Y, 32,
		PANEL_LABEL_STRING, "   Search   ",
		PANEL_NOTIFY_PROC, DoSearch,
		NULL);
	return obj;
}

//
// Create object `RelevanceFeedbackList' in the specified instance.
//
Xv_opaque
olwais_olw_objects::RelevanceFeedbackList_create(Xv_opaque owner)
{
	extern void	olwais_olw_RelevanceFeedbackList_drop_callback(Xv_opaque, Event *, GDD_DROP_INFO *);
	Xv_opaque	obj;
	Xv_drop_site	drop_site;
	Rect		rectlist[2];
	
	obj = xv_create(owner, PANEL_LIST,
		XV_KEY_DATA, INSTANCE, this,
		XV_X, 16,
		XV_Y, 40,
		PANEL_LIST_WIDTH, 700,
		PANEL_LIST_DISPLAY_ROWS, 5,
		PANEL_LIST_TITLE, "In Documents Similar To",
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_READ_ONLY, TRUE,
		PANEL_CHOOSE_ONE, TRUE,
		PANEL_CHOOSE_NONE, TRUE,
		NULL);
	
	drop_site = xv_create(owner, DROP_SITE_ITEM,
		DROP_SITE_ID, 1,
		DROP_SITE_EVENT_MASK, DND_ENTERLEAVE | DND_MOTION,
		NULL);
		
	xv_set(obj,
		XV_KEY_DATA, DROP_SITE_ITEM, drop_site,
		NULL);
	
	rectlist[0] = *(Rect *) xv_get(obj, XV_RECT);
	rectlist[1].r_width = 0;
	rectlist[1].r_height = 0;
	gdd_register_drop_site(drop_site, olwais_olw_RelevanceFeedbackList_drop_callback);
	gdd_activate_drop_site(drop_site, rectlist);
	return obj;
}

//
// Create object `SelectSourcesButton' in the specified instance.
//
Xv_opaque
olwais_olw_objects::SelectSourcesButton_create(Xv_opaque owner)
{
	extern void		olwais_olw_SelectSourcesButton_notify_callback(Panel_item, Event *);
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_BUTTON,
		XV_KEY_DATA, INSTANCE, this,
		XV_X, 760,
		XV_Y, 80,
		PANEL_LABEL_STRING, "Select Sources...",
		PANEL_NOTIFY_PROC, olwais_olw_SelectSourcesButton_notify_callback,
		NULL);
	return obj;
}

//
// Initialize an instance of object `ResultsPopup'.
//
void
olwais_ResultsPopup_objects::objects_initialize(Xv_opaque owner)
{
	ResultsPopup = ResultsPopup_create(owner);
	controls4 = controls4_create(ResultsPopup);
	ResultsList = ResultsList_create(controls4);
}

//
// Create object `ResultsPopup' in the specified instance.
//
Xv_opaque
olwais_ResultsPopup_objects::ResultsPopup_create(Xv_opaque owner)
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, FRAME_CMD,
		XV_KEY_DATA, INSTANCE, this,
		XV_WIDTH, 793,
		XV_HEIGHT, 433,
		XV_LABEL, "Results of Search for \"%s\"",
		XV_SHOW, FALSE,
		FRAME_SHOW_FOOTER, TRUE,
		FRAME_SHOW_RESIZE_CORNER, FALSE,
		FRAME_CMD_PUSHPIN_IN, FALSE,
		NULL);
	gcm_initialize_colors(obj, "BG1", "Black");
	xv_set(xv_get(obj, FRAME_CMD_PANEL), WIN_SHOW, FALSE, NULL);
	return obj;
}

//
// Create object `controls4' in the specified instance.
//
Xv_opaque
olwais_ResultsPopup_objects::controls4_create(Xv_opaque owner)
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL,
		XV_KEY_DATA, INSTANCE, this,
		XV_X, 0,
		XV_Y, 0,
		XV_WIDTH, WIN_EXTEND_TO_EDGE,
		XV_HEIGHT, WIN_EXTEND_TO_EDGE,
		WIN_BORDER, FALSE,
		NULL);
	return obj;
}

//
// Create object `ResultsList' in the specified instance.
//
Xv_opaque
olwais_ResultsPopup_objects::ResultsList_create(Xv_opaque owner)
{
	extern int		ViewDoc(Panel_item, char *, Xv_opaque, Panel_list_op, Event *, int);
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_LIST,
		XV_KEY_DATA, INSTANCE, this,
		XV_X, 8,
		XV_Y, 16,
		PANEL_LIST_WIDTH, 730,
		PANEL_LIST_DISPLAY_ROWS, 20,
		PANEL_LIST_TITLE, "Results",
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_READ_ONLY, TRUE,
		PANEL_CHOOSE_ONE, TRUE,
		PANEL_CHOOSE_NONE, TRUE,
		PANEL_NOTIFY_PROC, ViewDoc,
		NULL);
	return obj;
}

//
// Initialize an instance of object `SourceSelectPopup'.
//
void
olwais_SourceSelectPopup_objects::objects_initialize(Xv_opaque owner)
{
	SourceSelectPopup = SourceSelectPopup_create(owner);
	controls5 = controls5_create(SourceSelectPopup);
	SourcesList = SourcesList_create(controls5);
	ChosenSourcesList = ChosenSourcesList_create(controls5);
	AddSources = AddSources_create(controls5);
	AutopickSourcesButton = AutopickSourcesButton_create(controls5);
}

//
// Create object `SourceSelectPopup' in the specified instance.
//
Xv_opaque
olwais_SourceSelectPopup_objects::SourceSelectPopup_create(Xv_opaque owner)
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, FRAME_CMD,
		XV_KEY_DATA, INSTANCE, this,
		XV_WIDTH, 824,
		XV_HEIGHT, 260,
		XV_LABEL, "Sources for \"%s\"",
		XV_SHOW, FALSE,
		FRAME_SHOW_FOOTER, TRUE,
		FRAME_SHOW_RESIZE_CORNER, TRUE,
		FRAME_CMD_PUSHPIN_IN, TRUE,
		NULL);
	xv_set(xv_get(obj, FRAME_CMD_PANEL), WIN_SHOW, FALSE, NULL);
	return obj;
}

//
// Create object `controls5' in the specified instance.
//
Xv_opaque
olwais_SourceSelectPopup_objects::controls5_create(Xv_opaque owner)
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL,
		XV_KEY_DATA, INSTANCE, this,
		XV_X, 0,
		XV_Y, 0,
		XV_WIDTH, WIN_EXTEND_TO_EDGE,
		XV_HEIGHT, WIN_EXTEND_TO_EDGE,
		WIN_BORDER, FALSE,
		NULL);
	return obj;
}

//
// Create object `SourcesList' in the specified instance.
//
Xv_opaque
olwais_SourceSelectPopup_objects::SourcesList_create(Xv_opaque owner)
{
	extern int		olwais_SourceSelectPopup_SourcesList_notify_callback(Panel_item, char *, Xv_opaque, Panel_list_op, Event *, int);
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_LIST,
		XV_KEY_DATA, INSTANCE, this,
		XV_X, 16,
		XV_Y, 12,
		PANEL_LIST_WIDTH, 500,
		PANEL_LIST_DISPLAY_ROWS, 7,
		PANEL_LIST_TITLE, "Select sources",
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_READ_ONLY, TRUE,
		PANEL_CHOOSE_ONE, FALSE,
		PANEL_CHOOSE_NONE, TRUE,
		PANEL_NOTIFY_PROC, olwais_SourceSelectPopup_SourcesList_notify_callback,
		NULL);
	return obj;
}

//
// Create object `ChosenSourcesList' in the specified instance.
//
Xv_opaque
olwais_SourceSelectPopup_objects::ChosenSourcesList_create(Xv_opaque owner)
{
	extern int		olwais_SourceSelectPopup_ChosenSourcesList_notify_callback(Panel_item, char *, Xv_opaque, Panel_list_op, Event *, int);
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_LIST,
		XV_KEY_DATA, INSTANCE, this,
		XV_X, 592,
		XV_Y, 16,
		PANEL_LIST_WIDTH, 200,
		PANEL_LIST_DISPLAY_ROWS, 7,
		PANEL_LIST_TITLE, "Use these Sources",
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_READ_ONLY, TRUE,
		PANEL_CHOOSE_ONE, FALSE,
		PANEL_CHOOSE_NONE, TRUE,
		PANEL_NOTIFY_PROC, olwais_SourceSelectPopup_ChosenSourcesList_notify_callback,
		NULL);
	return obj;
}

//
// Create object `AddSources' in the specified instance.
//
Xv_opaque
olwais_SourceSelectPopup_objects::AddSources_create(Xv_opaque owner)
{
	extern void		AddOrDeleteSources(Panel_item, Event *);
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_BUTTON,
		XV_KEY_DATA, INSTANCE, this,
		XV_X, 544,
		XV_Y, 88,
		PANEL_LABEL_STRING, "<-",
		PANEL_NOTIFY_PROC, AddOrDeleteSources,
		NULL);
	return obj;
}

//
// Create object `AutopickSourcesButton' in the specified instance.
//
Xv_opaque
olwais_SourceSelectPopup_objects::AutopickSourcesButton_create(Xv_opaque owner)
{
	extern void		olwais_SourceSelectPopup_AutopickSourcesButton_notify_callback(Panel_item, Event *);
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_BUTTON,
		XV_KEY_DATA, INSTANCE, this,
		XV_X, 608,
		XV_Y, 208,
		PANEL_LABEL_STRING, "Pick sources automatically",
		PANEL_NOTIFY_PROC, olwais_SourceSelectPopup_AutopickSourcesButton_notify_callback,
		NULL);
	return obj;
}

//
// Initialize an instance of object `Alert'.
//
void
olwais_Alert_objects::objects_initialize(Xv_opaque owner)
{
	Alert = Alert_create(owner);
	controls1 = controls1_create(Alert);
	Message = Message_create(controls1);
	Dismiss = Dismiss_create(controls1);
}

//
// Create object `Alert' in the specified instance.
//
Xv_opaque
olwais_Alert_objects::Alert_create(Xv_opaque owner)
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, FRAME_CMD,
		XV_KEY_DATA, INSTANCE, this,
		XV_WIDTH, 399,
		XV_HEIGHT, 156,
		XV_LABEL, "Message",
		XV_SHOW, FALSE,
		FRAME_SHOW_FOOTER, FALSE,
		FRAME_SHOW_RESIZE_CORNER, FALSE,
		FRAME_CMD_PUSHPIN_IN, FALSE,
		NULL);
	xv_set(xv_get(obj, FRAME_CMD_PANEL), WIN_SHOW, FALSE, NULL);
	return obj;
}

//
// Create object `controls1' in the specified instance.
//
Xv_opaque
olwais_Alert_objects::controls1_create(Xv_opaque owner)
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL,
		XV_KEY_DATA, INSTANCE, this,
		XV_X, 0,
		XV_Y, 0,
		XV_WIDTH, WIN_EXTEND_TO_EDGE,
		XV_HEIGHT, WIN_EXTEND_TO_EDGE,
		WIN_BORDER, FALSE,
		NULL);
	return obj;
}

//
// Create object `Message' in the specified instance.
//
Xv_opaque
olwais_Alert_objects::Message_create(Xv_opaque owner)
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_MULTILINE_TEXT,
		XV_KEY_DATA, INSTANCE, this,
		XV_X, 16,
		XV_Y, 16,
		PANEL_VALUE_DISPLAY_LENGTH, 50,
		PANEL_VALUE_STORED_LENGTH, 80,
		PANEL_DISPLAY_ROWS, 8,
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_READ_ONLY, TRUE,
		NULL);
	return obj;
}

//
// Create object `Dismiss' in the specified instance.
//
Xv_opaque
olwais_Alert_objects::Dismiss_create(Xv_opaque owner)
{
	extern void		olwais_Alert_Dismiss_notify_callback(Panel_item, Event *);
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_BUTTON,
		XV_KEY_DATA, INSTANCE, this,
		XV_X, 312,
		XV_Y, 136,
		PANEL_LABEL_STRING, "Dismiss",
		PANEL_NOTIFY_PROC, olwais_Alert_Dismiss_notify_callback,
		NULL);
	return obj;
}
