/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 4;        /* border pixel of windows */
/* static const unsigned int borderpx  = 0;        /1* border pixel of windows *1/ */
static const unsigned int snap      = 32;       /* snap pixel */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
/* static const char *fonts[]          = { "monospace:size=14" }; */
/* static const char dmenufont[]       = "monospace:size=14"; */
/* static const char *fonts[]          = { "monospace:size=12" }; */
/* static const char dmenufont[]       = "monospace:size=12"; */
static const char *fonts[]          = { "monospace:size=14" };
static const char dmenufont[]       = "monospace:size=14";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#222222";
static const char col_cyan[]        = "#d8bb6d";
static const char col_blue[]        = "#d8bb6d";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_blue  },
};

/* tagging */
#define MAX_TAGNAME_LEN 14		/* excludes TAG_PREPEND */
#define TAG_PREPEND "%1i:"		/* formatted as 2 chars */
#define MAX_TAGLEN 16			/* altogether */
static char tags[][MAX_TAGLEN] = { "1:", "2:", "3:", "4:", "5:", "6:", "7:", "8:", "9:" };
/* static char tags[][MAX_TAGLEN] = { "1:", "2:", "3:", "4:", "5:"}; */

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	/* { "Gimp",            NULL,       NULL,       0,            1,           -1 }, */
	{ "clonemon",            NULL,       NULL,       0,            1,           -1 },
	/* { "ncal",            NULL,       "ncal",       0,            1,           -1 }, */
	{NULL,            NULL,       "ncal",       0,            1,           -1 },
	{NULL,            NULL,       "SpeedCrunch",       0,            1,           -1 },
	{NULL,            NULL,       "VisualBoyAdvance",       0,            1,           -1 },
	/* { "SpeedCrunch",     NULL,       NULL,       0,            1,           -1 }, */
	{ "Firefox",         NULL,       NULL,       1 << 8,       0,           -1 },
	{ "urxvt",            NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
};

/* key definitions */
#define AltMask Mod1Mask
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
/* static const char *termurxvt[]  = { "urxvtc", "-pe", "tabbedex", NULL }; */
/* static const char *termcmd[]  = { "urxvtc", "-pe", "tabbedex", NULL }; */
/* static const char *termcmd[]  = { "tabbed", "-r", "2", "st", "-w", "'1'", "-e", "zsh", NULL }; */
/* static const char *termcmd[]  = { "tabbed", "-r", "2", "st", "-e", "zsh", NULL }; */
/* static const char *termcmd[]  = {"st-open", NULL}; */
/* static const char *termcmd[]  = {"st", NULL}; */
static const char *termcmd[]  = {"/usr/bin/alacritty", NULL};
/* static const char *termcmd[]  = {"gnome-terminal", NULL}; */
/* static const char *termcmd[]  = {" qterminal", NULL}; */
/* static const char *termcmd[]  = {"stterm", NULL}; */
/* static const char *termcmd[]  = { "tabbed", "st", "-w", "''", "-e", "zsh", NULL }; */
/* static const char *termcmd[]  = {"alacritty", NULL}; */
/* static const char *termcmd[]  = {"qterminal", NULL}; */
static const char *mountcmd[]  = { "mount-dev", NULL };
static const char *volupcmd[]  = { "dwm_set_volume_up" , NULL };
static const char *voldowncmd[]  = { "dwm_set_volume_down" , NULL };
static const char *volmutecmd[]  = { "dwm_set_volume_mute_toggle" , NULL };
static const char *flameshotcmd[]  = { "flameshot", "gui", NULL };
static const char *comptontogglecmd[]  = { "comptontoggle", NULL };
static const char *firefoxcmd[]  = { "firefox", NULL };
/* static const char *surfcmd[]  = { "tabbed",  "surf", "-e", NULL }; */
static const char *surfcmd[]  = { "surf-open", NULL };
static const char *chromiumcmd[]  = { "brave", NULL };
static const char *blanktermcmd[]  = { "blank_urxvt", NULL };
static const char *mpdnextcmd[]  = { "mpdctl", "next", NULL };
static const char *mpdprevcmd[]  = { "mpdctl", "prev", NULL };
static const char *mpdtogglecmd[]  = { "mpdctl", "toggle", NULL };
static const char *mpdstopcmd[]  = { "mpdctl", "stop", NULL };
static const char *mpdstartcmd[]  = { "mpd", NULL };
static const char *mpdkillcmd[]  = { "killall", "mpd", NULL };
static const char *brightnessupcmd[]  = {"/home/nyquist/bin/brightness", "up", "10", NULL };
static const char *brightnessdowncmd[]  = {"/home/nyquist/bin/brightness", "down", "10", NULL };
static const char *speedcmd[]  = { "systemctl", "suspend", NULL };
static const char *audioNext[]  = { "playerctl", "next", NULL };
static const char *audioPrev[]  = { "playerctl", "previous", NULL };
static const char *audioPlay[]  = { "playerctl", "play-pause", NULL };
static const char *audioStop[]  = { "playerctl", "stop", NULL };
static const char *spaawncalculator[]  = { "/usr/bin/speedcrunch", NULL };
/* static const char *audioPause[]  = { "playerctl", "pause", NULL }; */

#include "movestack.c"

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ 0,             XF86XK_AudioRaiseVolume,  spawn,          {.v = volupcmd } },
	{ 0,             XF86XK_AudioLowerVolume,  spawn,          {.v = voldowncmd } },
	{ 0,             XF86XK_Calculator,  spawn,          {.v = spaawncalculator} },
	{ 0,        XF86XK_MonBrightnessUp,   spawn,          {.v = brightnessupcmd } },
	{ 0,        XF86XK_MonBrightnessDown, spawn,          {.v = brightnessdowncmd } },
	/* { MODKEY|ShiftMask|ControlMask,	XK_m,      spawn,          {.v = volmutecmd } }, */
	{ 0,	         XF86XK_AudioMute,         spawn,          {.v = volmutecmd } },
	{ MODKEY|AltMask,               XK_m,      spawn,          {.v = mountcmd } },
	{ MODKEY|ControlMask,           XK_s,      spawn,          {.v = flameshotcmd } },
	{ MODKEY|ControlMask,           XK_f,      spawn,          {.v = firefoxcmd } },
	{ MODKEY|ControlMask,           XK_b,      spawn,          {.v = surfcmd } },
	{ MODKEY|ControlMask,           XK_g,      spawn,          {.v = chromiumcmd } },
	{ MODKEY|AltMask|ControlMask,   XK_c,      spawn,          {.v = comptontogglecmd } },
	{ MODKEY|AltMask|ControlMask,   XK_b,      spawn,          {.v = blanktermcmd } },
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ 0,                     XF86XK_AudioNext, spawn,          {.v = audioNext } },
	{ 0,                     XF86XK_AudioPrev, spawn,          {.v = audioPrev } },
	{ 0,                     XF86XK_AudioPlay,spawn,          {.v = audioPlay } },
	{ 0,                     XF86XK_AudioStop, spawn,          {.v = audioStop } },
	/* { 0,                     XF86XK_AudioPause,spawn,          {.v = audioPause } }, */
	/* { MODKEY|ControlMask,           XK_Return, spawn,          {.v = termurxvt } }, */
	/* { 0,                     XF86XK_AudioNext, spawn,          {.v = mpdnextcmd } }, */
	{ MODKEY,                XK_Right,         spawn,          {.v = mpdnextcmd } },
	/* { 0,                     XF86XK_AudioPrev, spawn,          {.v = mpdprevcmd } }, */
	{ MODKEY,                XK_Left,          spawn,          {.v = mpdprevcmd } },
	/* { 0,                     XF86XK_AudioPlay, spawn,          {.v = mpdtogglecmd } }, */
	{ MODKEY,                XK_Up,            spawn,          {.v = mpdtogglecmd } },
	/* { 0,                     XF86XK_AudioStop, spawn,          {.v = mpdstopcmd } }, */
	{ MODKEY,                   XK_Down,       spawn,          {.v = mpdstopcmd } },
	{ MODKEY|AltMask,            XK_Up,       spawn,          {.v = mpdstartcmd } },
	{ MODKEY|AltMask,            XK_Down,     spawn,          {.v = mpdkillcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.01} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.01} },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
 	{ MODKEY,                       XK_n,      nametag,        {0} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

