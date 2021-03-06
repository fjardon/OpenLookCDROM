#ifndef	timer_HEADER
#define	timer_HEADER

/*
 * timer_ui.h - User interface object and function declarations.
 * This file was generated by `gxv' from `timer.G'.
 * DO NOT EDIT BY HAND.
 */

extern Attr_attribute	INSTANCE;


typedef struct {
	Xv_opaque	baseFrame;
	Xv_opaque	controls1;
	Xv_opaque	topicList;
	Xv_opaque	startButton;
	Xv_opaque	stopButton;
	Xv_opaque	newTextfield;
	Xv_opaque	addButton;
	Xv_opaque	rereadButton;
	Xv_opaque	deleteButton;
	Xv_opaque	group1;
} timer_baseFrame_objects;

extern timer_baseFrame_objects	*timer_baseFrame_objects_initialize();

extern Xv_opaque	timer_baseFrame_baseFrame_create();
extern Xv_opaque	timer_baseFrame_controls1_create();
extern Xv_opaque	timer_baseFrame_topicList_create();
extern Xv_opaque	timer_baseFrame_startButton_create();
extern Xv_opaque	timer_baseFrame_stopButton_create();
extern Xv_opaque	timer_baseFrame_newTextfield_create();
extern Xv_opaque	timer_baseFrame_addButton_create();
extern Xv_opaque	timer_baseFrame_rereadButton_create();
extern Xv_opaque	timer_baseFrame_deleteButton_create();
extern Xv_opaque	timer_baseFrame_group1_create();
#endif
