
#define MY_PLUGIN_NAME      "Custom Tag App"
#define MY_PLUGIN_VERSION   "1.1"
#define MY_PLUGIN_DEVELOPER "Hendrik Peter, Chris Gutierrez"
#define MY_PLUGIN_COPYRIGHT "Free to be distributed"
#define MY_PLUGIN_VIEW      "Standard ES radar screen"

const COLORREF	COLOR_BLACK = RGB(1, 1, 1); //Window Framing
const COLORREF	COLOR_WHITE = RGB(255, 254, 255); //Header Button Framing
const COLORREF	COLOR_ERROR = RGB(137, 19, 19); //
const COLORREF	COLOR_GREEN = RGB(155, 255, 148); //Response Pending Color
const COLORREF	COLOR_BLUE = RGB(14, 1, 54);	  //Header & Button Inner-Background
const COLORREF	COLOR_BLUE_HIGHLIGHT = RGB(145, 180, 203); //Default Data Text Color
const COLORREF	COLOR_BUTTON = RGB(165, 165, 165); // Header Text & Button Symbol & Text Color
const COLORREF	COLOR_BUTTON_HIGHLIGHT = RGB(210, 193, 49); //
const COLORREF	COLOR_FRAME = RGB(42, 42, 42); //Frame Background & Selected AC Background
const COLORREF	COLOR_FRAME_HIGHLIGHT = RGB(74, 75, 74); //AC List Border & Button Outer-Background & Non-Selected AC Background

const int		FRAME = 499;
const int		ITEM_AIRCRAFT = 500;
const int		AREA_HEADER = 0;
const int		AREA_AIRCRAFT = 1;
const int		AREA_DATA = 2;

const int		BUTTON_ICONIFY = 3;
const int		BUTTON_CLOSE = 4;
const int		BUTTON_LVL = 5;
const int		BUTTON_ROUTE = 6;
const int		BUTTON_VCI = 7;
const int		BUTTON_SSR = 8;
const int		BUTTON_RST = 9;
const int		BUTTON_MIKE = 10;
const int		BUTTON_EXECUTE = 11;
const int		BUTTON_CLIMB = 12;
const int		BUTTON_DESCEND = 13;
const int		BUTTON_MAINTN = 14;
const int		BUTTON_MAXROC = 15;
const int		BUTTON_MINROC = 16;
const int		BUTTON_CONTHDG = 17;
const int		BUTTON_FLYHDG = 18;
const int		BUTTON_DCTTO = 19;
const int		BUTTON_TURNLDEG = 20;
const int		BUTTON_TURNRDEG = 21;
const int		BUTTON_ACC = 22;
const int		BUTTON_SBY = 23;
const int		BUTTON_UNA = 24;
const int		BUTTON_UDT = 25;
const int		BUTTON_UDA = 26;
const int		BUTTON_NSSR = 27;
const int		BUTTON_SQIDENT = 28;
const int		BUTTON_AUTO = 29;

const int		BUTTON_LVL_EX = 31;
const int		BUTTON_ROUTE_EX = 32;

const   int     BUTTON_LEFT = 1;
const   int     BUTTON_MIDDLE = 2;
const   int     BUTTON_RIGHT = 3;

const   int		MSG_TELEX = 0;
const   int		MSG_tag = 1;
const   int		MSG_ADSC = 2;
const   int		MSG_POLL = 3;
const   int		MSG_PEK = 4;
const   int		MSG_PING = 5;
const   bool	MSG_SEND = false;
const   bool	MSG_RECEIVE = true;

const   int		VICINITY_LIST = 1;
const   int		HOPPIE_LIST = 2;