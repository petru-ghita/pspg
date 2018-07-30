/*-------------------------------------------------------------------------
 *
 * pspg.h
 *	  themes initialization
 *
 * Portions Copyright (c) 2017-2018 Pavel Stehule
 *
 * IDENTIFICATION
 *	  src/pspg.h
 *
 *-------------------------------------------------------------------------
 */

#ifndef PSPG_PSPG_H
#define PSPG_PSPG_H

#include "config.h"
#include "themes.h"
#include "st_menu.h"

#define LINEINFO_NONE				0
#define LINEINFO_BOOKMARK			1
#define LINEINFO_FOUNDSTR			2
#define LINEINFO_FOUNDSTR_MULTI		4
#define LINEINFO_UNKNOWN			8

#define MAX_STYLE					16

typedef struct LineInfo
{
	char			mask;
	short int		start_char;
} LineInfo;

typedef struct LineBuffer
{
	int		first_row;
	int		nrows;
	char   *rows[1000];
	LineInfo	   *lineinfo;
	struct LineBuffer *next;
	struct LineBuffer *prev;
} LineBuffer;

/*
 * Available formats of headline chars
 *
 *  L, R   .. outer border
 *  I      .. inner border
 *  d      .. data
 */

/*
 * This structure should be immutable
 */
typedef struct
{
	int		border_top_row;			/* nrow of bottom outer border or -1 */
	int		border_head_row;		/* nrow of head outer (required) */
	int		border_bottom_row;		/* nrow of bottom outer border or -1 */
	int		border_type;			/* detected type of border: 0, 1, 2 */
	char	linestyle;				/* detected linestyle: a, u */
	bool	is_expanded_mode;		/* true when data are in expanded mode */
	bool	is_pgcli_fmt;			/* table was generated by pgcli */
	int		expanded_info_minx;		/* begin of info in \x mode .. RECORD x */
	char	title[65];				/* detected title (trimmed) or NULL */
	int		title_rows;				/* number of rows used as table title (skipped later) */
	char	filename[65];			/* filename (printed on top bar) */
	LineBuffer rows;				/* list of rows buffers */
	int		maxy;					/* maxy of used pad area with data */
	int		maxx;					/* maxx of used pad area with data */
	int		maxbytes;				/* max length of line in bytes */
	char   *headline;				/* header separator line */
	int		headline_size;			/* size of headerline in bytes */
	char   *headline_transl;		/* translated headline */
	int		headline_char_size;		/* size of headerline in chars */
	int		first_data_row;			/* fist data row line (starts by zero) */
	int		last_data_row;			/* last line of data row */
	int		footer_row;				/* nrow of first footer row or -1 */
	int		alt_footer_row;			/* alternative footer row (used when border = 1) */
	int		footer_char_size;		/* width of footer */
	int		last_row;				/* last not empty row */
	int		fixed_rows;				/* number of fixed rows */
	int		fixed_columns;			/* number of fixed columns */
	int		data_rows;				/* number of data rows */
	int		footer_rows;			/* number of footer rows */
} DataDesc;

/*
 * This structure can be mutable - depends on displayed data
 */
typedef struct
{
	int		fix_rows_rows;			/* current number of fixed rows in window rows */
	int		fix_cols_cols;			/* current number of fixed columns in window rows */
	int		rows_rows;				/* current number of data rows */
	int		footer_rows;			/* current number of footer rows */
	int		maxy;					/* max y size of screen */
	int		maxx;					/* max x size of screen */
	int		main_maxy;				/* max y size of main place (fixed + data + footer rows) */
	int		main_maxx;				/* max x size of main place (should be same like maxx) */
	int		main_start_y;			/* y position of first row of main place */
	int		main_start_x;			/* x position of first row of main place */
	int		top_bar_rows;			/* 1 or 0 when top bar is not used */
	WINDOW	*wins[7];
	Theme	themes[7];
	char	searchterm[256];		/* currently active search input */
	int		searchterm_char_size;	/* size of searchterm in chars */
	int		searchterm_size;		/* size of searchterm in bytes */
	bool	has_upperchr;			/* true, when search term has upper char */
	bool	found;					/* true, when last search was successfull */
	int		found_start_x;			/* x position of found pattern */
	int		found_start_bytes;		/* bytes position of found pattern */
	int		found_row;				/* row of found pattern */
	int		first_rec_title_y;		/* y of first displayed record title in expanded mode */
	int		last_rec_title_y;		/* y of last displayed record title in expanded mode */
	char   *fmt;					/* format string for info when refresh first is required */
	char   *par;					/* parameter for info when refresh first is required */
	bool	beep;					/* beep for info when refresh is required */
	bool	applytimeout;			/* true, when saved info should be close after timeout */
	bool	refresh_scr;			/* force rewrite screen */
} ScrDesc;

#define		w_luc(scrdesc)			((scrdesc)->wins[WINDOW_LUC])
#define		w_fix_rows(scrdesc)		((scrdesc)->wins[WINDOW_FIX_ROWS])
#define		w_fix_cols(scrdesc)		((scrdesc)->wins[WINDOW_FIX_COLS])
#define		w_rows(scrdesc)			((scrdesc)->wins[WINDOW_ROWS])
#define		w_footer(scrdesc)		((scrdesc)->wins[WINDOW_FOOTER])
#define		w_top_bar(scrdesc)		((scrdesc)->wins[WINDOW_TOP_BAR])
#define		w_bottom_bar(scrdesc)	((scrdesc)->wins[WINDOW_BOTTOM_BAR])

/* from print.c */
extern void window_fill(int window_identifier, int srcy, int srcx, int cursor_row, DataDesc *desc, ScrDesc *scrdesc, Options *opts);
extern void draw_data(Options *opts, ScrDesc *scrdesc, DataDesc *desc, int first_data_row, int first_row, int cursor_col, int footer_cursor_col, int fix_rows_offset);

/* from pspg.c */
extern void leave_ncurses(const char *str);
extern bool is_expanded_header(Options *opts, char *str, int *ei_minx, int *ei_maxx);
extern int min_int(int a, int b);
extern const char *nstrstr(const char *haystack, const char *needle);
extern const char *nstrstr_ignore_lower_case(const char *haystack, const char *needle);

extern const char *pspg_search(Options *opts, ScrDesc *scrdesc, const char *str);

/* from menu.c */
extern struct ST_MENU *init_menu(Options *opts, struct ST_MENU *current_menu);
extern void post_menu(Options *opts, struct ST_MENU *current_menu);

#endif