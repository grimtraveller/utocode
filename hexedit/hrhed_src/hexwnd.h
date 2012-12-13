#include <windows.h>
#include "simparr.h"
#include <stdio.h>
// This is frhed v1.0.SUB_RELEASE_NO
#define SUB_RELEASE_NO "155 beta 3"

//--------------------------------------------------------------------------------------------
// Callback functions for dialogue boxes.
//Pabs changed - line insert
BOOL CALLBACK FillWithDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);//msg handler for fill with dialog 
//end
BOOL CALLBACK GoToDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK FindDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK CopyHexdumpDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK EnterDecimalValueDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK PasteDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK CutDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK CopyDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ViewSettingsDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AppendDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK BitManipDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK CharacterSetDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ChooseDiffDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK BinaryModeDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SelectBlockDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AddBmkDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK RemoveBmkDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OpenPartiallyDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK FastPasteDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK TmplDisplayDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ReplaceDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

//--------------------------------------------------------------------------------------------
// Functions to find out which instance of frhed is being started right now.
int g_iInstCount;

//--------------------------------------------------------------------------------------------
#define bitval(base,pos) ((base)[(pos)/8]&(1<<((pos)%8)))
#define CLIENT_BORDER_WIDTH 2
#define ANSI_SET ANSI_FIXED_FONT
#define OEM_SET  OEM_FIXED_FONT
#define LITTLEENDIAN_MODE    0
#define BIGENDIAN_MODE 1
#define TIMERID 1
#define TIMERDURATION 10
#define MRUMAX 9
#define BMKMAX 9
#define BMKTEXTMAX 256
#define TPL_TYPE_MAXLEN 16
#define TPL_NAME_MAXLEN 128

typedef struct
{
	int one;
	int two;
} intpair;

typedef struct
{
	int offset;
	char* name;
} bookmark;

//--------------------------------------------------------------------------------------------
// Global variables.

// String containing data to replace.
SimpleString strToReplaceData;
// String containing data to replace with.
SimpleString strReplaceWithData;

SimpleString TxtEditName;
char *pcGotoDlgBuffer;
int iGotoDlgBufLen;
int bOpenReadOnlySetting;
int iStartPL, iNumBytesPl, iPLFileLen;
bookmark *pbmkRemove;
int iRemBmk;
int iBmkOffset;
char pcBmkTxt[BMKTEXTMAX];
int iFindDlgBufLen, iFindDlgMatchCase, iFindDlgDirection, iFindDlgLastLen;
char *pcFindDlgBuffer;
int iCopyHexdumpDlgStart, iCopyHexdumpDlgEnd, iCopyHexdumpMode = BST_CHECKED;
int iDecValDlgOffset, iDecValDlgValue, iDecValDlgSize, iDecValDlgTimes;
//Pabs changed " = 1, iPasteSkip" inserted
int iPasteMode, iPasteMaxTxtLen, iPasteTimes = 1, iPasteSkip;
//end
char *pcPasteText;
int iCutOffset, iCutNumberOfBytes, iCutMode = BST_CHECKED;
int iCopyStartOffset, iCopyEndOffset;
int iAutomaticXAdjust = BST_CHECKED, iBPLSetting, iOffsetLenSetting;
int iAppendbytes;
int iManipPos;
unsigned char cBitValue;
int iCharacterSetting, iFontSizeSetting;
int bUnsignedViewSetting;
char szFileName[_MAX_PATH];
int iDestFileLen, iSrcFileLen;
intpair* pdiffChoice;
int iDiffNum;
int iBinaryModeSetting;
int iStartOfSelSetting, iEndOfSelSetting;
int iPasteAsText;
char* pcTmplText;
SimpleString BrowserName;

HWND hMainWnd;

int find_bytes (char* ps, int ls, char* pb, int lb, int mode, char (*cmp) (char));
HRESULT ResolveIt( HWND hwnd, LPCSTR lpszLinkFile, LPSTR lpszPath );

//Pabs changed - line insert
#define FW_MAX 1024//max bytes to fill with
char pcFWText[FW_MAX];//hex representation of bytes to fill with
char buf[FW_MAX];//bytes to fill with
int buflen;//number of bytes to fill with
char szFWFileName[_MAX_PATH];//fill with file name
int FWFile,FWFilelen;//fill with file and len
LONG oldproc;//old hex box proc
LONG cmdoldproc;//old command box proc
HFONT hfon;//needed so possible to display infinity char in fill with dlg box
HFONT hfdef;//store default text font for text boxes here
char curtyp;//filling with input-0 or file-1
int tmpstart,tmpend;//temporary storage for start and end of selection
char asstyp;
//end

// RK: function by pabs.
void MessageCopyBox(HWND hWnd, LPTSTR lpText, LPCTSTR lpCaption, UINT uType, HWND hwnd);

//--------------------------------------------------------------------------------------------
class HexEditorWindow
{
public:
	int transl_text_to_binary( SimpleString& in, SimpleArray<char>& out );
	int iGetStartOfSelection()
	{
		if( iStartOfSelection < iEndOfSelection )
			return iStartOfSelection;
		else
			return iEndOfSelection;
	}

	int iGetEndOfSelection()
	{
		if( iStartOfSelection < iEndOfSelection )
			return iEndOfSelection;
		else
			return iStartOfSelection;
	}

//Pabs changed - line insert
	void CMD_fw();//fill selection with command
	void CMD_revert();
	void CMD_saveselas();
	void CMD_deletefile();
	void CMD_insertfile();
//end
	int replace_selected_data( SimpleString& replacedata, int do_repaint = TRUE );
	int find_and_select_data( SimpleString& finddata, int finddir, int do_repaint, char (*cmp) (char) );
	int	transl_binary_to_text( SimpleString& dest, char* src, int len );
	void CMD_replace();
	void CMD_explorersettings();
	inline int OnWndMsg( HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam );
	int CMD_summon_text_edit();
	int CMD_findprev();
	int CMD_findnext();
	void CMD_colors_to_default();
	int CMD_goto();
	int read_tpl_token( char* pcTpl, int tpl_len, int& index, char* name );
	int ignore_non_code( char* pcTpl, int tpl_len, int& index );
	int apply_template_on_memory( char* pcTpl, int tpl_len, SimpleArray<char>& ResultArray );
	int apply_template( char* pcTemplate );
	int CMD_apply_template();
	int dropfiles( HANDLE hDrop );
	int CMD_fast_paste ();
	int CMD_open_partially ();
	int CMD_clear_all_bmk ();
	int CMD_remove_bkm ();
	int CMD_goto_bookmark (int cmd);
	int make_bookmark_list (HMENU menu);
	int CMD_add_bookmark ();
	int CMD_MRU_selected (int cmd);
	int make_MRU_list (HMENU menu);
	int update_MRU ();
	int CMD_select_block ();
	int timer (WPARAM w, LPARAM l);
	int CMD_binarymode ();
	inline char TranslateAnsiToOem (char c);
	int get_diffs (char* ps, int sl, char* pd, int dl, intpair* pdiff);
	int compare_arrays (char* ps, int sl, char* pd, int dl);
	int CMD_compare ();
	int CMD_properties ();
	int make_font ();
	int CMD_select_all ();
	int CMD_on_backspace ();
	int CMD_toggle_insertmode ();
	int CMD_on_deletekey ();
	int CMD_character_set ();
	void draw_client_border (HDC hdc);
	void destroy_backbuffer ();
	int CMD_manipulate_bits ();
	int CMD_edit_append ();
	int save_ini_data ();
	int read_ini_data ();
	int CMD_color_settings (COLORREF* pColor);
	int CMD_view_settings ();
	void adjust_view_for_selection ();
	int CMD_select_with_arrowkeys (int key);
	int CMD_open ();
	int CMD_save ();
	int CMD_save_as ();
	int CMD_new ();
	int CMD_edit_cut ();
	void update_for_new_datasize ();
	int byte_to_BC_destlen (char* src, int srclen);
	int translate_bytes_to_BC (char* pd, unsigned char* src, int srclen);
	int is_bytecode (char* src, int len);
	int calc_bctrans_destlen (char* src, int srclen);
	int translate_bytecode (char* dest, char* src, int srclen, int binmode=LITTLEENDIAN_MODE);
	int create_bc_translation (char* dest, char* src, int srclen, int charmode=ANSI_SET, int binmode=LITTLEENDIAN_MODE);
	int find_byte_pos (char* src, char c);
	int create_bc_translation (char** ppd, char* src, int srclen, int charmode=ANSI_SET, int binmode=LITTLEENDIAN_MODE);
	int CMD_edit_enterdecimalvalue ();
	int CMD_edit_paste ();
	int CMD_copy_hexdump ();
	int CMD_edit_copy ();
	int CMD_find ();
	int mousemove (int xPos, int yPos);
	int lbuttonup (int xPos, int yPos);
	int close ();
	int initmenupopup (WPARAM w, LPARAM l);
	void adjust_view_for_caret ();
	void print_line( HDC hdc, int line, char* linebuffer, HBRUSH hbr );
	void mark_char (HDC hdc);
	void adjust_hscrollbar ();
	void adjust_vscrollbar ();
	void clear_all ();
	int repaint (int line=-1);
	HexEditorWindow();
	~HexEditorWindow();
	int load_file (char* fname);
	int file_is_loadable (char* fname);
	int at_window_create (HWND hw, HINSTANCE hI);
	int resize_window (int cx, int cy);
	int set_focus ();
	int kill_focus ();
	int lbuttondown (int xPos, int yPos);
	int keydown (int key);
	int character (char ch);
	int vscroll (int cmd, int pos);
	int hscroll (int cmd, int pos);
	int paint ();
	int command (int cmd);
	int destroy_window ();
	void set_wnd_title ();
	void set_caret_pos ();

public:
	int iWindowShowCmd, iWindowX, iWindowY, iWindowWidth, iWindowHeight;

private:
	int bDontMarkCurrentPos;
	int bInsertingHex;
	SimpleString TexteditorName;
	SimpleArray<char> Linebuffer;
	int iHexWidth;
	int bReadOnly, bOpenReadOnly;
	int iPartialOffset, bPartialOpen;
	int iBmkCount;
	bookmark pbmkList[BMKMAX];
	int iMRU_count;
	char strMRU[MRUMAX][_MAX_PATH+1];
	int bFilestatusChanged;
	int iUpdateLine;
	int bTimerSet;
	int iMouseX, iMouseY;
	int iBinaryMode;
	int bUnsignedView;
	int iFontSize;
	HFONT hFont;
	int iInsertMode;
	int iCharacterSet;
	int iTextColor, iBkColor;
	COLORREF iTextColorValue, iBkColorValue, iSepColorValue;
	COLORREF iSelBkColorValue, iSelBkColor, iSelTextColorValue, iSelTextColor;
	COLORREF iBmkColor;
	int iAutomaticBPL;
	int bFileNeverSaved;
	SimpleArray<unsigned char> DataArray;
	int bSelected;
	int bLButtonIsDown, iLBDownX, iLBDownY;
	int iStartOfSelection, iEndOfSelection;
	int m_iEnteringMode, m_iFileChanged;
	int cxChar, cxCaps, cyChar, cxClient, cyClient, cxBuffer, cyBuffer, iNumlines,
		iVscrollMax, iVscrollPos, iVscrollInc,
		iHscrollMax, iHscrollPos, iHscrollInc,
		iCurLine, iCurByte, iCurNibble;
	int iOffsetLen,	iByteSpace,	iBytesPerLine, iCharSpace, iCharsPerLine;
	char *filename;
	HWND hwnd, hwndStatusBar, hwndHBar, hwndVBar;
	HINSTANCE hInstance;
};

//--------------------------------------------------------------------------------------------
// MAKROS
#define CHARSTART (iOffsetLen + iByteSpace + iBytesPerLine * 3 + iCharSpace)
#define BYTEPOS (iCurByte % iBytesPerLine)
#define BYTELINE (iCurByte / iBytesPerLine)
#define BYTES_LOGICAL_COLUMN (iOffsetLen + iByteSpace + BYTEPOS * 3 + iCurNibble)
#define CHARS_LOGICAL_COLUMN (CHARSTART + BYTEPOS)
#define LAST_LOG_COLUMN (iHscrollPos + cxBuffer - 1) // Last visible logical column.
#define CURSOR_TOO_HIGH (iCurLine > BYTELINE)
#define CURSOR_TOO_LOW (iCurLine+cyBuffer-1 < BYTELINE)
#define LAST_VISIBLE_LINE (iCurLine+cyBuffer-1)
#define LASTLINE (iNumlines-1)
#define LASTBYTE (DataArray.GetUpperBound ())
#define STARTSELECTION_LINE (iStartOfSelection / iBytesPerLine)
#define ENDSELECTION_LINE (iEndOfSelection / iBytesPerLine)
#define IN_BOUNDS( i, a, b ) ( ( i >= a && i <= b ) || ( i >= b && i <= a ) )
#define NO_FILE (filename[0] == '\0')
#define BYTES 0 // for EnteringMode
#define CHARS 1
#define WM_F1DOWN (WM_USER+1)

//============================================================================================
// The main window object.
HexEditorWindow hexwnd;

