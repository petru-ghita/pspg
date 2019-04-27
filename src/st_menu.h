#if defined HAVE_NCURSESW_CURSES_H
#include <ncursesw/curses.h>
#elif defined HAVE_NCURSESW_H
#include <ncursesw.h>
#elif defined HAVE_NCURSES_CURSES_H
#include <ncurses/curses.h>
#elif defined HAVE_NCURSES_H
#include <ncurses.h>
#elif defined HAVE_CURSES_H
#include <curses.h>
#else
/* fallback */
#include <ncurses/ncurses.h>
#endif

#ifndef _ST_MENU_H

#define _ST_MENU_H

#define ST_MENU_STYLE_MCB			0
#define ST_MENU_STYLE_MC			1
#define ST_MENU_STYLE_VISION		2
#define ST_MENU_STYLE_DOS			3
#define ST_MENU_STYLE_FAND_1		4
#define ST_MENU_STYLE_FAND_2		5
#define ST_MENU_STYLE_FOXPRO		6
#define ST_MENU_STYLE_PERFECT		7
#define ST_MENU_STYLE_NOCOLOR		8
#define ST_MENU_STYLE_ONECOLOR		9
#define ST_MENU_STYLE_TURBO			10
#define ST_MENU_STYLE_PDMENU		11
#define ST_MENU_STYLE_OLD_TURBO		12
#define ST_MENU_STYLE_FREE_DOS		13
#define ST_MENU_STYLE_FREE_DOS_P	14
#define ST_MENU_STYLE_MC46			15
#define ST_MENU_STYLE_DBASE			16

#define ST_MENU_LAST_STYLE			ST_MENU_STYLE_DBASE

#define	ST_MENU_ESCAPE		27

#define ST_MENU_OPTION_DEFAULT		1
#define ST_MENU_OPTION_DISABLED		2
#define ST_MENU_OPTION_MARKED		4

#define ST_MENU_FOCUS_FULL			0		/* all possible events can be processed */
#define ST_MENU_FOCUS_ALT_MOUSE		1		/* only mouse, ALT key events */
#define ST_MENU_FOCUS_MOUSE_ONLY	2		/* only mouse events are processed */
#define ST_MENU_FOCUS_NONE			3		/* menu has not any focus */

/*
 * Uncomment it and set for your environment when you would to
 * use named pipe for debugging.
 *
#define DEBUG_PIPE		"/home/pavel/debug"
 */

#ifdef DEBUG_PIPE

/*
 * When you would to use named pipe for debugging, then there should
 * be active reader from this pipe before start demo application.
 * In this case "tail -f ~/debug" inside other terminal.
 */

extern FILE   *debug_pipe;
extern int		debug_eventno;

#endif

typedef struct _ST_MENU_ITEM
{
	char	*text;						/* text of menu item, possible specify accelerator by ~ */
	int		code;						/* code of menu item (optional) */
	char	*shortcut;					/* shortcut text, only printed (optional) */
	int		data;						/* allow to assign some value to menu item (optional) */
	char	group;						/* specify semantics of data value (optional) */
	int		options;					/* locked, marked, ... (optional) */
	struct _ST_MENU_ITEM *submenu;		/* reference to nested menu (optional) */
} ST_MENU_ITEM;

typedef struct
{
	bool	force8bit;
	char   *encoding;
	const char   *language;
	bool	wide_vborders;			/* wide vertical menu borders like Turbo Vision */
	bool	wide_hborders;			/* wide horizontal menu borders like custom menu mc */
	bool	draw_box;				/* when true, then box is created */
	bool	left_alligned_shortcuts;	/* when true, a shortcuts are left alligned */
	bool	funckey_bar_style;		/* when true, menu bar is displayed as mc toolbar */
	bool	extra_inner_space;		/* when true, then there 2 spaces between text and border */
	bool	force_ascii_art;		/* use ascii chars for borders */
	int		shadow_width;			/* when shadow_width is higher than zero, shadow is visible */
	int		menu_background_cpn;	/* draw area color pair number */
	int		menu_background_attr;	/* draw area attributte */
	int		menu_unfocused_cpn;		/* draw area color pair number when menu has not focus */
	int		menu_unfocused_attr;	/* draw area attribute when menu has not focus */
	int		menu_shadow_cpn;		/* menu shadow color pair number */
	int		menu_shadow_attr;		/* menu shadow area attributte */
	int		accelerator_cpn;		/* color pair of accelerators */
	int		accelerator_attr;		/* accelerator attributes */
	int		cursor_cpn;				/* cursor color pair */
	int		cursor_attr;			/* cursor attributte */
	int		cursor_accel_cpn;		/* color pair of accelerator on cursor row */
	int		cursor_accel_attr;		/* attributte of of accelerator on cursor row */
	int		disabled_cpn;			/* color of disabled menu fields */
	int		disabled_attr;			/* attributes of disabled menu fields */
	int		shortcut_space;			/* spaces between text and shortcut */
	int		text_space;				/* spaces between text fields (menubar), when it is -1, then dynamic spaces (FAND) */
	int		init_text_space;		/* initial space for menu bar */
	int		menu_bar_menu_offset;	/* offset between menu bar and menu */
	int		inner_space;			/* space between draw area and border, FAND uses 2 spaces */
	int		extern_accel_text_space;	/* space between external accelerator and menu item text */
	int		submenu_tag;			/* symbol used for submenu tag */
	int		submenu_offset_y;		/* offset for submenu related to right border of parent menu window */
	int		submenu_offset_x;		/* offset for submenu related to cursor in parent menu window */
	int		mark_tag;				/* symbol used for mark tag */
} ST_MENU_CONFIG;

struct ST_MENU;

typedef struct
{
	char	   *text;				/* text of command bar field */
	bool		alt;				/* should be used like Alt+FX */
	int			fkey;				/* Func key number */
	int			code;				/* code of command bar item */
	int			option;				/* locked, marked, ... (optional) */
} ST_CMDBAR_ITEM;

struct ST_CMDBAR;

extern int st_menu_load_style(ST_MENU_CONFIG *config, int style, int start_from_cpn);

extern void st_menu_set_desktop_window(WINDOW *win);
extern struct ST_MENU *st_menu_new(ST_MENU_CONFIG *config, ST_MENU_ITEM *items, int begin_y, int begin_x, char *title);
extern struct ST_MENU *st_menu_new_menubar(ST_MENU_CONFIG *config, ST_MENU_ITEM *items);
extern struct ST_MENU *st_menu_new_menubar2(ST_MENU_CONFIG *barcfg, ST_MENU_CONFIG *pdcfg, ST_MENU_ITEM *items);

extern void st_menu_post(struct ST_MENU *menu);
extern void st_menu_unpost(struct ST_MENU *menu, bool close_active_submenu);
extern bool st_menu_driver(struct ST_MENU *menu, int c, bool alt, MEVENT *mevent);
extern void st_menu_free(struct ST_MENU *menu);
extern void st_menu_save(struct ST_MENU *menu, int *cursor_rows, int max_rows);
extern void st_menu_load(struct ST_MENU *menu, int *cursor_rows);

extern ST_MENU_ITEM *st_menu_selected_item(bool *activated);
extern ST_CMDBAR_ITEM *st_menu_selected_command(bool *activated);

extern bool st_menu_enable_option(struct ST_MENU *menu, int code, int option);
extern bool st_menu_reset_option(struct ST_MENU *menu, int code, int option);
extern bool st_menu_set_option(struct ST_MENU *menu, int code, int option, bool value);
extern bool st_menu_reset_all_submenu_options(struct ST_MENU *menu, int menu_code, int option);
extern bool st_menu_reset_all_options(struct ST_MENU *menu, int option);
extern void st_menu_set_focus(struct ST_MENU *menu, int focus);

extern struct ST_CMDBAR *st_cmdbar_new(ST_MENU_CONFIG *config, ST_CMDBAR_ITEM *cmdbar_items);
extern void st_cmdbar_post(struct ST_CMDBAR *cmdbar);
extern void st_cmdbar_unpost(struct ST_CMDBAR *cmdbar);
extern void st_cmdbar_free(struct ST_CMDBAR *cmdbar);

#endif
