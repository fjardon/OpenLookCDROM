#ifndef	print_HEADER
#define	print_HEADER

/*
 * print_ui.h - User interface object and function declarations.
 * This file was generated by `gxv' from `print.G'.
 * DO NOT EDIT BY HAND.
 */

extern Attr_attribute	INSTANCE;


typedef struct {
	Xv_opaque	print_base;
	Xv_opaque	controls1;
	Xv_opaque	print_or_file;
	Xv_opaque	postscript;
	Xv_opaque	printer;
	Xv_opaque	filename;
	Xv_opaque	categories;
	Xv_opaque	scale;
	Xv_opaque	incl_checked_items;
	Xv_opaque	cancel_print;
	Xv_opaque	exec_print;
} print_print_base_objects;

extern print_print_base_objects	*print_print_base_objects_initialize();

extern Xv_opaque	print_print_base_print_base_create();
extern Xv_opaque	print_print_base_controls1_create();
extern Xv_opaque	print_print_base_print_or_file_create();
extern Xv_opaque	print_print_base_postscript_create();
extern Xv_opaque	print_print_base_printer_create();
extern Xv_opaque	print_print_base_filename_create();
extern Xv_opaque	print_print_base_categories_create();
extern Xv_opaque	print_print_base_scale_create();
extern Xv_opaque	print_print_base_incl_checked_items_create();
extern Xv_opaque	print_print_base_cancel_print_create();
extern Xv_opaque	print_print_base_exec_print_create();
#endif
