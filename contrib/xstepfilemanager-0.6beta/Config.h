#define LABEL    "XstepFileManager 0.6beta"
/* Here you can define your preferred helper apps */

#define TERMINAL "aterm"
#define TERM_EXE "aterm -e"
#define EDITOR   "aterm -e e3"
/*#define VIEWER   "aterm -e less"*/

/* to enable internal viewer */
#define INTERNAL_VIEWER
	#ifdef INTERNAL_VIEWER
	#undef VIEWER
	#endif

/* to enable right-click menus */
/*#define RIGHT_CLICK */

/* for debugging */
/*#define DEBUG*/

