#ifndef	deadline_HEADER
#define	deadline_HEADER

/*
 * deadline_ui.h - User interface object and function declarations.
 * This file was generated by `gxv' from `deadline.G'.
 * DO NOT EDIT BY HAND.
 */

extern Attr_attribute	INSTANCE;


typedef struct {
	Xv_opaque	deadline_frame;
	Xv_opaque	deadline_panel;
	Xv_opaque	deadline_date;
	Xv_opaque	deadline_actions;
	Xv_opaque	deadline_delete_units;
	Xv_opaque	deadline_delete_time;
	Xv_opaque	deadline_up_increment;
	Xv_opaque	deadline_down_increment;
	Xv_opaque	deadline_on_mail_to;
	Xv_opaque	deadline_after_mail_to;
	Xv_opaque	deadline_move_units;
	Xv_opaque	deadline_move_after;
	Xv_opaque	deadline_cancel;
	Xv_opaque	deadline_done;
} deadline_deadline_frame_objects;

extern deadline_deadline_frame_objects	*deadline_deadline_frame_objects_initialize();

extern Xv_opaque	deadline_deadline_frame_deadline_frame_create();
extern Xv_opaque	deadline_deadline_frame_deadline_panel_create();
extern Xv_opaque	deadline_deadline_frame_deadline_date_create();
extern Xv_opaque	deadline_deadline_frame_deadline_actions_create();
extern Xv_opaque	deadline_deadline_frame_deadline_delete_units_create();
extern Xv_opaque	deadline_deadline_frame_deadline_delete_time_create();
extern Xv_opaque	deadline_deadline_frame_deadline_up_increment_create();
extern Xv_opaque	deadline_deadline_frame_deadline_down_increment_create();
extern Xv_opaque	deadline_deadline_frame_deadline_on_mail_to_create();
extern Xv_opaque	deadline_deadline_frame_deadline_after_mail_to_create();
extern Xv_opaque	deadline_deadline_frame_deadline_move_units_create();
extern Xv_opaque	deadline_deadline_frame_deadline_move_after_create();
extern Xv_opaque	deadline_deadline_frame_deadline_cancel_create();
extern Xv_opaque	deadline_deadline_frame_deadline_done_create();
#endif
