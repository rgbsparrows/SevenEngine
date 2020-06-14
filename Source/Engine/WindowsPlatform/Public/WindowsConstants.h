#pragma once

#include "Macros/UtilMacros.h"
#include "Template/TemplateUtil.h"

#include <stdint.h>

enum class EWinMessageBoxFlag : uint32_t
{
	MB_OK					= 0x00000000L,
	MB_OKCANCEL				= 0x00000001L,
	MB_ABORTRETRYIGNORE		= 0x00000002L,
	MB_YESNOCANCEL			= 0x00000003L,
	MB_YESNO				= 0x00000004L,
	MB_RETRYCANCEL			= 0x00000005L,
	MB_CANCELTRYCONTINUE	= 0x00000006L,
	MB_HELP					= 0x00004000L,

	MB_ICONSTOP				= 0x00000010L,
	MB_ICONERROR			= 0x00000010L,
	MB_ICONHAND				= 0x00000010L,
	MB_ICONQUESTION			= 0x00000020L,
	MB_ICONEXCLAMATION		= 0x00000030L,
	MB_ICONWARNING			= 0x00000030L,
	MB_ICONINFORMATION		= 0x00000040L,
	MB_ICONASTERISK			= 0x00000040L,

	MB_DEFBUTTON1			= 0x00000000L,
	MB_DEFBUTTON2			= 0x00000100L,
	MB_DEFBUTTON3			= 0x00000200L,
	MB_DEFBUTTON4			= 0x00000300L,

	MB_APPLMODAL			= 0x00000000L,
	MB_SYSTEMMODAL			= 0x00001000L,
	MB_TASKMODAL			= 0x00002000L,

	MB_SETFOREGROUND		= 0x00010000L,
	MB_DEFAULT_DESKTOP_ONLY = 0x00020000L,
	MB_TOPMOST				= 0x00040000L,
	MB_RIGHT				= 0x00080000L,
	MB_RTLREADING			= 0x00100000L,
	MB_SERVICE_NOTIFICATION = 0x00200000L,

	Error = TErrorEnumValue<EWinMessageBoxFlag>
};
REGIST_ENUM_FLAG(EWinMessageBoxFlag)


enum class EWinMessboxResult : uint32_t
{
	IDOK		= 1,
	IDCANCEL	= 2,
	IDABORT		= 3,
	IDRETRY		= 4,
	IDIGNORE	= 5,
	IDYES		= 6,
	IDNO		= 7,
	IDTRYAGAIN	= 10,
	IDCONTINUE	= 11,

	Error = TErrorEnumValue<EWinMessboxResult>
};

enum class EWinClassStyle : uint32_t
{
	CS_VREDRAW          = 0x00001,
	CS_HREDRAW          = 0x00002,
	CS_DBLCLKS          = 0x00008,
	CS_OWNDC            = 0x00020,
	CS_CLASSDC          = 0x00040,
	CS_PARENTDC         = 0x00080,
	CS_NOCLOSE          = 0x00200,
	CS_SAVEBITS         = 0x00800,
	CS_BYTEALIGNCLIENT  = 0x01000,
	CS_BYTEALIGNWINDOW  = 0x02000,
	CS_GLOBALCLASS      = 0x04000,
	
	CS_IME              = 0x10000,
	CS_DROPSHADOW       = 0x20000,

	Error = TErrorEnumValue<EWinClassStyle>
};
REGIST_ENUM_FLAG(EWinClassStyle)

enum class EWinWindowStyle : uint64_t
{
	WS_OVERLAPPED		= 0x00000000ULL,
	WS_POPUP			= 0x80000000ULL,
	WS_CHILD			= 0x40000000ULL,
	WS_MINIMIZE			= 0x20000000ULL,
	WS_VISIBLE			= 0x10000000ULL,
	WS_DISABLED			= 0x08000000ULL,
	WS_CLIPSIBLINGS		= 0x04000000ULL,
	WS_CLIPCHILDREN		= 0x02000000ULL,
	WS_MAXIMIZE			= 0x01000000ULL,
	WS_BORDER			= 0x00800000ULL,
	WS_DLGFRAME			= 0x00400000ULL,
	WS_VSCROLL			= 0x00200000ULL,
	WS_HSCROLL			= 0x00100000ULL,
	WS_SYSMENU			= 0x00080000ULL,
	WS_THICKFRAME		= 0x00040000ULL,
	WS_GROUP			= 0x00020000ULL,
	WS_TABSTOP			= 0x00010000ULL,
	
	WS_MINIMIZEBOX		= 0x00020000ULL,
	WS_MAXIMIZEBOX		= 0x00010000ULL,
	
	
	WS_TILED			= WS_OVERLAPPED,
	WS_ICONIC			= WS_MINIMIZE,
	WS_SIZEBOX			= WS_THICKFRAME,
	
	WS_CAPTION			= WS_BORDER | WS_DLGFRAME,
	
	WS_OVERLAPPEDWINDOW	= (WS_OVERLAPPED     | \
						   WS_CAPTION        | \
						   WS_SYSMENU        | \
						   WS_THICKFRAME     | \
						   WS_MINIMIZEBOX    | \
						   WS_MAXIMIZEBOX),
	
	WS_POPUPWINDOW		= (WS_POPUP          | \
						   WS_BORDER         | \
						   WS_SYSMENU),
	
	WS_CHILDWINDOW		= WS_CHILD,
	WS_TILEDWINDOW		= WS_OVERLAPPEDWINDOW,
	
	WS_EX_DLGMODALFRAME			= 0x0000000100000000ULL,
	WS_EX_NOPARENTNOTIFY		= 0x0000000400000000ULL,
	WS_EX_TOPMOST				= 0x0000000800000000ULL,
	WS_EX_ACCEPTFILES			= 0x0000001000000000ULL,
	WS_EX_TRANSPARENT			= 0x0000002000000000ULL,
	
	WS_EX_MDICHILD				= 0x0000004000000000ULL,
	WS_EX_TOOLWINDOW			= 0x0000008000000000ULL,
	WS_EX_WINDOWEDGE			= 0x0000010000000000ULL,
	WS_EX_CLIENTEDGE			= 0x0000020000000000ULL,
	WS_EX_CONTEXTHELP			= 0x0000040000000000ULL,
	
	WS_EX_RIGHT					= 0x0000100000000000ULL,
	WS_EX_LEFT					= 0x0000000000000000ULL,
	WS_EX_RTLREADING			= 0x0000200000000000ULL,
	WS_EX_LTRREADING			= 0x0000000000000000ULL,
	WS_EX_LEFTSCROLLBAR			= 0x0000400000000000ULL,
	WS_EX_RIGHTSCROLLBAR		= 0x0000000000000000ULL,
	
	WS_EX_CONTROLPARENT			= 0x0001000000000000ULL,
	WS_EX_STATICEDGE			= 0x0002000000000000ULL,
	WS_EX_APPWINDOW				= 0x0004000000000000ULL,
	
	WS_EX_LAYERED				= 0x0008000000000000ULL,
	
	WS_EX_NOINHERITLAYOUT		= 0x0010000000000000ULL, // Disable inheritence of mirroring by children
	
	WS_EX_NOREDIRECTIONBITMAP	= 0x0020000000000000ULL,
	
	WS_EX_LAYOUTRTL				= 0x0040000000000000ULL, // Right to left mirroring
	
	WS_EX_COMPOSITED			= 0x0200000000000000ULL,
	
	WS_EX_NOACTIVATE			= 0x0800000000000000ULL,
	
	WS_EX_OVERLAPPEDWINDOW		= (WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE),
	WS_EX_PALETTEWINDOW			= (WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW | WS_EX_TOPMOST),

	Error = TErrorEnumValue<EWinWindowStyle>,
};
REGIST_ENUM_FLAG(EWinWindowStyle)

enum class EWinMessage : uint32_t
{
	WM_NULL								= 0x0000,
	WM_CREATE							= 0x0001,
	WM_DESTROY							= 0x0002,
	WM_MOVE								= 0x0003,
	WM_SIZE								= 0x0005,
	WM_ACTIVATE							= 0x0006,
	WM_SETFOCUS							= 0x0007,
	WM_KILLFOCUS						= 0x0008,
	WM_ENABLE							= 0x000A,
	WM_SETREDRAW						= 0x000B,
	WM_SETTEXT							= 0x000C,
	WM_GETTEXT							= 0x000D,
	WM_GETTEXTLENGTH					= 0x000E,
	WM_PAINT							= 0x000F,
	WM_CLOSE							= 0x0010,
	WM_QUERYENDSESSION					= 0x0011,
	WM_QUERYOPEN						= 0x0013,
	WM_ENDSESSION						= 0x0016,
	WM_QUIT								= 0x0012,
	WM_ERASEBKGND						= 0x0014,
	WM_SYSCOLORCHANGE					= 0x0015,
	WM_SHOWWINDOW						= 0x0018,
	WM_WININICHANGE						= 0x001A,
	WM_DEVMODECHANGE					= 0x001B,
	WM_ACTIVATEAPP						= 0x001C,
	WM_FONTCHANGE						= 0x001D,
	WM_TIMECHANGE						= 0x001E,
	WM_CANCELMODE						= 0x001F,
	WM_SETCURSOR						= 0x0020,
	WM_MOUSEACTIVATE					= 0x0021,
	WM_CHILDACTIVATE					= 0x0022,
	WM_QUEUESYNC						= 0x0023,
	WM_GETMINMAXINFO					= 0x0024,
	WM_PAINTICON						= 0x0026,
	WM_ICONERASEBKGND					= 0x0027,
	WM_NEXTDLGCTL						= 0x0028,
	WM_SPOOLERSTATUS					= 0x002A,
	WM_DRAWITEM							= 0x002B,
	WM_MEASUREITEM						= 0x002C,
	WM_DELETEITEM						= 0x002D,
	WM_VKEYTOITEM						= 0x002E,
	WM_CHARTOITEM						= 0x002F,
	WM_SETFONT							= 0x0030,
	WM_GETFONT							= 0x0031,
	WM_SETHOTKEY						= 0x0032,
	WM_GETHOTKEY						= 0x0033,
	WM_QUERYDRAGICON					= 0x0037,
	WM_COMPAREITEM						= 0x0039,
	WM_GETOBJECT						= 0x003D,
	WM_COMPACTING						= 0x0041,
	WM_COMMNOTIFY						= 0x0044,  /* no longer suported */
	WM_WINDOWPOSCHANGING				= 0x0046,
	WM_WINDOWPOSCHANGED					= 0x0047,
	WM_POWER							= 0x0048,
	WM_COPYDATA							= 0x004A,
	WM_CANCELJOURNAL					= 0x004B,
	WM_NOTIFY							= 0x004E,
	WM_INPUTLANGCHANGEREQUEST			= 0x0050,
	WM_INPUTLANGCHANGE					= 0x0051,
	WM_TCARD							= 0x0052,
	WM_HELP								= 0x0053,
	WM_USERCHANGED						= 0x0054,
	WM_NOTIFYFORMAT						= 0x0055,
	WM_CONTEXTMENU						= 0x007B,
	WM_STYLECHANGING					= 0x007C,
	WM_STYLECHANGED						= 0x007D,
	WM_DISPLAYCHANGE					= 0x007E,
	WM_GETICON							= 0x007F,
	WM_SETICON							= 0x0080,
	WM_NCCREATE							= 0x0081,
	WM_NCDESTROY						= 0x0082,
	WM_NCCALCSIZE						= 0x0083,
	WM_NCHITTEST						= 0x0084,
	WM_NCPAINT							= 0x0085,
	WM_NCACTIVATE						= 0x0086,
	WM_GETDLGCODE						= 0x0087,
	WM_NCMOUSEMOVE						= 0x00A0,
	WM_NCLBUTTONDOWN					= 0x00A1,
	WM_NCLBUTTONUP						= 0x00A2,
	WM_NCLBUTTONDBLCLK					= 0x00A3,
	WM_NCRBUTTONDOWN					= 0x00A4,
	WM_NCRBUTTONUP						= 0x00A5,
	WM_NCRBUTTONDBLCLK					= 0x00A6,
	WM_NCMBUTTONDOWN					= 0x00A7,
	WM_NCMBUTTONUP						= 0x00A8,
	WM_NCMBUTTONDBLCLK					= 0x00A9,
	WM_NCXBUTTONDOWN					= 0x00AB,
	WM_NCXBUTTONUP						= 0x00AC,
	WM_NCXBUTTONDBLCLK					= 0x00AD,
	WM_INPUT_DEVICE_CHANGE				= 0x00FE,
	WM_INPUT							= 0x00FF,
	WM_KEYFIRST							= 0x0100,
	WM_KEYDOWN							= 0x0100,
	WM_KEYUP							= 0x0101,
	WM_CHAR								= 0x0102,
	WM_DEADCHAR							= 0x0103,
	WM_SYSKEYDOWN						= 0x0104,
	WM_SYSKEYUP							= 0x0105,
	WM_SYSCHAR							= 0x0106,
	WM_SYSDEADCHAR						= 0x0107,
	WM_UNICHAR							= 0x0109,
	WM_IME_STARTCOMPOSITION				= 0x010D,
	WM_IME_ENDCOMPOSITION				= 0x010E,
	WM_IME_COMPOSITION					= 0x010F,
	WM_IME_KEYLAST						= 0x010F,
	WM_INITDIALOG						= 0x0110,
	WM_COMMAND							= 0x0111,
	WM_SYSCOMMAND						= 0x0112,
	WM_TIMER							= 0x0113,
	WM_HSCROLL							= 0x0114,
	WM_VSCROLL							= 0x0115,
	WM_INITMENU							= 0x0116,
	WM_INITMENUPOPUP					= 0x0117,
	WM_GESTURE							= 0x0119,
	WM_GESTURENOTIFY					= 0x011A,
	WM_MENUSELECT						= 0x011F,
	WM_MENUCHAR							= 0x0120,
	WM_ENTERIDLE						= 0x0121,
	WM_MENURBUTTONUP					= 0x0122,
	WM_MENUDRAG							= 0x0123,
	WM_MENUGETOBJECT					= 0x0124,
	WM_UNINITMENUPOPUP					= 0x0125,
	WM_MENUCOMMAND						= 0x0126,
	WM_CHANGEUISTATE					= 0x0127,
	WM_UPDATEUISTATE					= 0x0128,
	WM_QUERYUISTATE						= 0x0129,
	WM_CTLCOLORMSGBOX					= 0x0132,
	WM_CTLCOLOREDIT						= 0x0133,
	WM_CTLCOLORLISTBOX					= 0x0134,
	WM_CTLCOLORBTN						= 0x0135,
	WM_CTLCOLORDLG						= 0x0136,
	WM_CTLCOLORSCROLLBAR				= 0x0137,
	WM_CTLCOLORSTATIC					= 0x0138,
	MN_GETHMENU							= 0x01E1,
	WM_MOUSEFIRST						= 0x0200,
	WM_MOUSEMOVE						= 0x0200,
	WM_LBUTTONDOWN						= 0x0201,
	WM_LBUTTONUP						= 0x0202,
	WM_LBUTTONDBLCLK					= 0x0203,
	WM_RBUTTONDOWN						= 0x0204,
	WM_RBUTTONUP						= 0x0205,
	WM_RBUTTONDBLCLK					= 0x0206,
	WM_MBUTTONDOWN						= 0x0207,
	WM_MBUTTONUP						= 0x0208,
	WM_MBUTTONDBLCLK					= 0x0209,
	WM_MOUSEWHEEL						= 0x020A,
	WM_XBUTTONDOWN						= 0x020B,
	WM_XBUTTONUP						= 0x020C,
	WM_XBUTTONDBLCLK					= 0x020D,
	WM_MOUSEHWHEEL						= 0x020E,
	WM_PARENTNOTIFY						= 0x0210,
	WM_ENTERMENULOOP					= 0x0211,
	WM_EXITMENULOOP						= 0x0212,
	WM_NEXTMENU							= 0x0213,
	WM_SIZING							= 0x0214,
	WM_CAPTURECHANGED					= 0x0215,
	WM_MOVING							= 0x0216,
	WM_POWERBROADCAST					= 0x0218,
	WM_DEVICECHANGE						= 0x0219,
	WM_MDICREATE						= 0x0220,
	WM_MDIDESTROY						= 0x0221,
	WM_MDIACTIVATE						= 0x0222,
	WM_MDIRESTORE						= 0x0223,
	WM_MDINEXT							= 0x0224,
	WM_MDIMAXIMIZE						= 0x0225,
	WM_MDITILE							= 0x0226,
	WM_MDICASCADE						= 0x0227,
	WM_MDIICONARRANGE					= 0x0228,
	WM_MDIGETACTIVE						= 0x0229,
	WM_MDISETMENU						= 0x0230,
	WM_ENTERSIZEMOVE					= 0x0231,
	WM_EXITSIZEMOVE						= 0x0232,
	WM_DROPFILES						= 0x0233,
	WM_MDIREFRESHMENU					= 0x0234,
	WM_POINTERDEVICECHANGE				= 0x0238,
	WM_POINTERDEVICEINRANGE				= 0x0239,
	WM_POINTERDEVICEOUTOFRANGE			= 0x023A,
	WM_TOUCH							= 0x0240,
	WM_NCPOINTERUPDATE					= 0x0241,
	WM_NCPOINTERDOWN					= 0x0242,
	WM_NCPOINTERUP						= 0x0243,
	WM_POINTERUPDATE					= 0x0245,
	WM_POINTERDOWN						= 0x0246,
	WM_POINTERUP						= 0x0247,
	WM_POINTERENTER						= 0x0249,
	WM_POINTERLEAVE						= 0x024A,
	WM_POINTERACTIVATE					= 0x024B,
	WM_POINTERCAPTURECHANGED			= 0x024C,
	WM_TOUCHHITTESTING					= 0x024D,
	WM_POINTERWHEEL						= 0x024E,
	WM_POINTERHWHEEL					= 0x024F,
	DM_POINTERHITTEST					= 0x0250,
	WM_POINTERROUTEDTO					= 0x0251,
	WM_POINTERROUTEDAWAY				= 0x0252,
	WM_POINTERROUTEDRELEASED			= 0x0253,
	WM_IME_SETCONTEXT					= 0x0281,
	WM_IME_NOTIFY						= 0x0282,
	WM_IME_CONTROL						= 0x0283,
	WM_IME_COMPOSITIONFULL				= 0x0284,
	WM_IME_SELECT						= 0x0285,
	WM_IME_CHAR							= 0x0286,
	WM_IME_REQUEST						= 0x0288,
	WM_IME_KEYDOWN						= 0x0290,
	WM_IME_KEYUP						= 0x0291,
	WM_WTSSESSION_CHANGE				= 0x02B1,
	WM_TABLET_FIRST						= 0x02c0,
	WM_TABLET_LAST						= 0x02df,
	WM_DPICHANGED						= 0x02E0,
	WM_DPICHANGED_BEFOREPARENT			= 0x02E2,
	WM_DPICHANGED_AFTERPARENT			= 0x02E3,
	WM_GETDPISCALEDSIZE					= 0x02E4,
	WM_CUT								= 0x0300,
	WM_COPY								= 0x0301,
	WM_PASTE							= 0x0302,
	WM_CLEAR							= 0x0303,
	WM_UNDO								= 0x0304,
	WM_RENDERFORMAT						= 0x0305,
	WM_RENDERALLFORMATS					= 0x0306,
	WM_DESTROYCLIPBOARD					= 0x0307,
	WM_DRAWCLIPBOARD					= 0x0308,
	WM_PAINTCLIPBOARD					= 0x0309,
	WM_VSCROLLCLIPBOARD					= 0x030A,
	WM_SIZECLIPBOARD					= 0x030B,
	WM_ASKCBFORMATNAME					= 0x030C,
	WM_CHANGECBCHAIN					= 0x030D,
	WM_HSCROLLCLIPBOARD					= 0x030E,
	WM_QUERYNEWPALETTE					= 0x030F,
	WM_PALETTEISCHANGING				= 0x0310,
	WM_PALETTECHANGED					= 0x0311,
	WM_HOTKEY							= 0x0312,
	WM_PRINT							= 0x0317,
	WM_PRINTCLIENT						= 0x0318,
	WM_APPCOMMAND						= 0x0319,
	WM_THEMECHANGED						= 0x031A,
	WM_CLIPBOARDUPDATE					= 0x031D,
	WM_DWMCOMPOSITIONCHANGED			= 0x031E,
	WM_DWMNCRENDERINGCHANGED			= 0x031F,
	WM_DWMCOLORIZATIONCOLORCHANGED		= 0x0320,
	WM_DWMWINDOWMAXIMIZEDCHANGE			= 0x0321,
	WM_DWMSENDICONICTHUMBNAIL			= 0x0323,
	WM_DWMSENDICONICLIVEPREVIEWBITMAP	= 0x0326,
	WM_GETTITLEBARINFOEX				= 0x033F,
	WM_HANDHELDFIRST					= 0x0358,
	WM_HANDHELDLAST						= 0x035F,
	WM_AFXFIRST							= 0x0360,
	WM_AFXLAST							= 0x037F,
	WM_PENWINFIRST						= 0x0380,
	WM_PENWINLAST						= 0x038F,
	WM_APP								= 0x8000,
	WM_USER								= 0x0400,

	Error = TErrorEnumValue<EWinMessage>
};
