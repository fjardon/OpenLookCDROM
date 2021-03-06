static void CreateContinueDialog_cb P_((Widget, XtPointer, XtPointer));
static void CreateGlobDialog_cb P_((Widget, XtPointer, XtPointer));
static void CreateSearchDialog_cb P_((Widget, XtPointer, XtPointer));
static void CreateSearchHostDialog_cb P_((Widget, XtPointer, XtPointer));
static void PopDialog P_((Widget, char*));
static void continue_cb P_((Widget, XtPointer, XtPointer));
static void glob_cb P_((Widget, XtPointer, XtPointer));
static char * makeglob P_((char*));
static void restart_cb P_((DATA, int, CB*));
static void search_cb P_((Widget, XtPointer, XtPointer));
static void search_clear_act P_((Widget, XEvent*, String*, Cardinal*));
static void search_host_cb P_((Widget, XtPointer, XtPointer));
static void set_glob_text_act P_((Widget, XEvent*, String*, Cardinal*));
static void set_search_host_act P_((Widget, XEvent*, String*, Cardinal*));
static void set_search_text_act P_((Widget, XEvent*, String*, Cardinal*));
