//============================================================================================
// Frhed main definition file.

#include "hexwnd.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <io.h>
#include <commctrl.h>
#include "resource.h"
#include <windows.h>
#include <shobjidl.h>
#include <shlobj.h>

extern HINSTANCE hMainInstance;
extern void TextToClipboard( char* pcText );

BOOL CALLBACK EnumWindowsProc( HWND hwnd, LPARAM lParam )
{
	char buf[64];
	if( GetClassName( hwnd, buf, 64 ) != 0 )
	{
		if( strcmp( buf, "frhed wndclass" ) == 0 )
		{
			g_iInstCount++;
		}
	}
	return TRUE;
}

void count_instances()
{
	g_iInstCount = 0;
	EnumWindows( (WNDENUMPROC) EnumWindowsProc, 0 );
}

class WaitCursor
{
public:
	WaitCursor()
	{
		SetCursor( LoadCursor( NULL, IDC_WAIT ) );
	}

	~WaitCursor()
	{
		SetCursor( LoadCursor( NULL, IDC_ARROW ) );
	}
};

//--------------------------------------------------------------------------------------------
// Required for the find function.
inline char equal (char c)
{
	return c;
}

inline char lower_case (char c)
{
	if (c >= 'A' && c <= 'Z')
		return 'a' + c - 'A';
	else
		return c;
}

//Pabs changed - line insert
//used to swap tmpstart and tmpend if start>end
template<class T>void swap(T& x, T& y){
	T temp = x;
	x = y;
	y = temp;
}

//--------------------------------------------------------------------------------------------
HexEditorWindow::HexEditorWindow ()
{
	bDontMarkCurrentPos = FALSE;

	bInsertingHex = FALSE;

	BrowserName = "iexplore";
	TexteditorName.SetToString( "notepad.exe" );

	iWindowX = CW_USEDEFAULT;
	iWindowY = CW_USEDEFAULT;
	iWindowWidth = CW_USEDEFAULT;
	iWindowHeight = CW_USEDEFAULT;
	iWindowShowCmd = SW_SHOW;

	iHexWidth = 3;

	// iClipboardEncode = TRUE;
	iBmkColor = RGB( 255, 0, 0 );
	iSelBkColorValue = RGB( 255, 255, 0 );
	iSelTextColorValue = RGB( 0, 0, 0 );

	pcGotoDlgBuffer = NULL;
	iGotoDlgBufLen = 0;
	bOpenReadOnly = bReadOnly = FALSE;
	iPartialOffset=0;
	bPartialOpen=FALSE;
	iBmkCount=0;
	int i;
	for (i=1; i<=MRUMAX; i++)
		sprintf (&(strMRU[i-1][0]), "dummy%d", i);
	iMRU_count = 0;
	bFilestatusChanged = TRUE;
	iBinaryMode = LITTLEENDIAN_MODE;
	szFileName[0] = '\0';
	bUnsignedView = TRUE;
	iFontSize = 10;
	iInsertMode = FALSE;
	iTextColorValue = RGB (0,0,0);
	iBkColorValue = RGB (255,255,255);
	iSepColorValue = RGB (192,192,192);
	iAutomaticBPL = BST_CHECKED;
	bSelected = FALSE;
	bLButtonIsDown = FALSE;
	iStartOfSelection = 0;
	iEndOfSelection = 0;
	hwnd = 0;
	iOffsetLen = 6;
	iByteSpace = 2;
	iBytesPerLine = 16;
	iCharSpace = 1;
	iCharsPerLine = iOffsetLen + iByteSpace + iBytesPerLine*3 + iCharSpace + iBytesPerLine;
	filename = new char[_MAX_PATH];
	filename[0] = '\0';
	m_iEnteringMode = BYTES;
	m_iFileChanged = FALSE;
	bFileNeverSaved = TRUE;

	iFindDlgLastLen = 0;
	pcFindDlgBuffer = NULL;
	iFindDlgBufLen = 0;

	iCopyHexdumpDlgStart = 0;
	iCopyHexdumpDlgEnd = 0;
	iCharacterSet = ANSI_FIXED_FONT;
	
	// Read in the last saved preferences.
	count_instances();
	read_ini_data ();

	bFileNeverSaved = TRUE;
	bSelected = FALSE;
	bLButtonIsDown = FALSE;
	m_iFileChanged = FALSE;
	iVscrollMax = 0;
	iVscrollPos = 0;
	iVscrollInc = 0;
	iHscrollMax = 0;
	iHscrollPos = 0;
	iHscrollInc = 0;
	iCurLine = 0;
	iCurByte = 0;
	DataArray.ClearAll ();
	DataArray.SetGrowBy (100);
	sprintf (filename, "Untitled");
}

//--------------------------------------------------------------------------------------------
HexEditorWindow::~HexEditorWindow ()
{
	if (hFont != NULL)
		DeleteObject (hFont);
	if (filename != NULL)
		delete [] filename;
	if (pcFindDlgBuffer != NULL)
		delete [] pcFindDlgBuffer;
	if( pcGotoDlgBuffer != NULL )
		delete [] pcGotoDlgBuffer;
}

//--------------------------------------------------------------------------------------------
int HexEditorWindow::load_file (char* fname)
{
	if (file_is_loadable (fname))
	{		
		int filehandle;
		if ((filehandle = _open (fname,_O_RDONLY|_O_BINARY,_S_IREAD|_S_IWRITE)) != -1)
		{
			int filelen = _filelength (filehandle);
			DataArray.ClearAll ();
			// Try to allocate memory for the file.
			if (DataArray.SetSize (filelen) == TRUE)
			{
				// If read-only mode on opening is enabled:
				if( bOpenReadOnly )
					bReadOnly = TRUE;
				else
					bReadOnly = FALSE;

				if( filelen == 0)
				{
					// This is an empty file. Don't need to read anything.
					_close( filehandle );
					strcpy( filename, fname );
					bFileNeverSaved = FALSE;
					bPartialOpen=FALSE;
					// Update MRU list.
					update_MRU ();
					bFilestatusChanged = TRUE;
					update_for_new_datasize();
					return TRUE;
				}
				else
				{
					// Load the file.
					SetCursor (LoadCursor (NULL, IDC_WAIT));
					DataArray.SetUpperBound (filelen-1);
					if (_read (filehandle, DataArray, DataArray.GetLength ()) != -1)
					{
						_close (filehandle);
						strcpy (filename, fname);
						bFileNeverSaved = FALSE;
						bPartialOpen=FALSE;
						// Update MRU list.
						update_MRU ();
						bFilestatusChanged = TRUE;
						update_for_new_datasize();
						SetCursor (LoadCursor (NULL, IDC_ARROW));
						return TRUE;
					}
					else
					{
						_close (filehandle);
						SetCursor (LoadCursor (NULL, IDC_ARROW));
						MessageBox (NULL, "Error while reading from file.", "Load error", MB_OK | MB_ICONERROR);
						return FALSE;
					}
				}
			}
			else
			{
				MessageBox (NULL, "Not enough memory to load file.", "Load error", MB_OK | MB_ICONERROR);
				return FALSE;
			}
		}
		else
		{
			char buf[500];
			sprintf (buf, "Error code 0x%x occured while opening file %s.", errno, fname);
			MessageBox (NULL, buf, "Load error", MB_OK | MB_ICONERROR);
			return FALSE;
		}
	}
	else
		return FALSE;
}

//--------------------------------------------------------------------------------------------
int HexEditorWindow::file_is_loadable (char* fname)
{
	int filehandle;
	if ((filehandle = _open (fname,_O_RDONLY|_O_BINARY,_S_IREAD|_S_IWRITE)) != -1)
	{
		_close (filehandle);
		return TRUE;
	}
	else
		return FALSE;
}

//--------------------------------------------------------------------------------------------
int HexEditorWindow::at_window_create (HWND hw, HINSTANCE hI)
{
	hwnd = hw;
	hMainWnd = hw;
	hInstance = hI;

	// Create Statusbar.
	InitCommonControls ();
	hwndStatusBar = CreateStatusWindow (
		 CCS_BOTTOM | WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
		"Ready", hwnd, 2);

	// Create Scrollbars.
	hwndHBar = CreateWindow ("scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ,
		0,0,0,0, hwnd, (HMENU) 1, hInstance, NULL);
	hwndVBar = CreateWindow ("scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_VERT,
		0,0,0,0, hwnd, (HMENU) 2, hInstance, NULL);

	iVscrollMax = 0;
	iVscrollPos = 0;
	iVscrollInc = 0;
	iHscrollMax = 0;
	iHscrollPos = 0;
	iHscrollInc = 0;

	iCurLine = 0;
	iCurByte = 0;
	iCurNibble = 0;
	
	DragAcceptFiles( hwnd, TRUE ); // Accept files dragged into window.
	return TRUE;
}

//--------------------------------------------------------------------------------------------
// Window was resized to new width of cx and new height of cy.
int HexEditorWindow::resize_window (int cx, int cy)
{
	// Get font data.
	HDC hdc = GetDC (hwnd);
	make_font ();
	HFONT of = (HFONT) SelectObject( hdc, hFont );
	TEXTMETRIC tm;
	GetTextMetrics (hdc, &tm);
	cxChar = tm.tmAveCharWidth;
	cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
	cyChar = tm.tmHeight + tm.tmExternalLeading;
	SelectObject (hdc, of);
	ReleaseDC (hwnd, hdc);
	
	//--------------------------------------------
	// Resize statusbar.
	int x, y, x2, y2;
	RECT rWindow;
	GetWindowRect (hwndStatusBar, &rWindow);
	y2 = rWindow.bottom - rWindow.top;
	x = 0;
	y = cy - y2;
	x2 = cx;
	MoveWindow (hwndStatusBar, x, y, x2, y2, TRUE);

	// Get client size.
	cxClient = cx;
	cyClient = cy - y2;

	// Resize scrollbars.
	int cyh = GetSystemMetrics (SM_CYHSCROLL),
		cxv = GetSystemMetrics (SM_CXVSCROLL);
	MoveWindow (hwndHBar, 0, cyClient - cyh, cxClient - cxv, cyh, FALSE);
	MoveWindow (hwndVBar, cxClient - cxv, 0, cxv, cyClient, FALSE);
	RedrawWindow (hwndHBar, NULL, NULL, RDW_INVALIDATE | RDW_ERASENOW | RDW_ALLCHILDREN);
	RedrawWindow (hwndVBar, NULL, NULL, RDW_INVALIDATE | RDW_ERASENOW | RDW_ALLCHILDREN);

	// Resize client area.
	cxClient -= cxv;
	cyClient -= cyh;

	//--------------------------------------------
	// Set statusbar divisions.
	int statbarw;
	if (IsZoomed (hwnd))
		statbarw = cx;
	else
		statbarw = cxClient;
    // Allocate an array for holding the right edge coordinates.
    HLOCAL hloc = LocalAlloc (LHND, sizeof(int) * 3);
    int* lpParts = (int*) LocalLock(hloc); 
 
    // Calculate the right edge coordinate for each part, and
    // copy the coordinates to the array.
	lpParts[0] = statbarw*4/6;
	lpParts[1] = statbarw*5/6;
	lpParts[2] = statbarw;

    // Tell the status window to create the window parts. 
    SendMessage (hwndStatusBar, SB_SETPARTS, (WPARAM) 3,
        (LPARAM) lpParts);
 
    // Free the array, and return. 
    LocalUnlock(hloc); 
    LocalFree(hloc);
	//--------------------------------------------

	cxBuffer = max (1, cxClient / cxChar);
	cyBuffer = max (1, cyClient / cyChar);
	// Adjust bytes per line to width of window.
	// cxBuffer = maximal width of client-area in chars.
	if( iAutomaticBPL )
	{
		int bytemax = cxBuffer-iOffsetLen-iByteSpace-iCharSpace;
		iBytesPerLine = bytemax / 4;
		if (iBytesPerLine < 1)
			iBytesPerLine = 1;
	}
	// Caret or end of selection will be vertically centered if line not visible.
	if( bSelected )
	{
		if( iEndOfSelection / iBytesPerLine < iCurLine || iEndOfSelection / iBytesPerLine > iCurLine + cyBuffer )
			iCurLine = max( 0, iEndOfSelection / iBytesPerLine - cyBuffer / 2 );
	}
	else
	{
		if( iCurByte/iBytesPerLine < iCurLine || iCurByte/iBytesPerLine > iCurLine + cyBuffer )
			iCurLine = max( 0, iCurByte/iBytesPerLine-cyBuffer/2 );
	}
	adjust_vscrollbar();

	iCharsPerLine = iOffsetLen + iByteSpace + iBytesPerLine*3 + iCharSpace + iBytesPerLine;

	// Get number of lines to display.
	if ((DataArray.GetLength()+1) % iBytesPerLine == 0)
		iNumlines = (DataArray.GetLength()+1) / iBytesPerLine;
	else
		iNumlines = (DataArray.GetLength()+1) / iBytesPerLine + 1;
	
	if (iNumlines <= 0xffff)
		iVscrollMax = iNumlines-1;
	else
		iVscrollMax = 0xffff;

	SetScrollRange (hwndVBar, SB_CTL, 0, iVscrollMax, FALSE);
	SetScrollPos (hwndVBar, SB_CTL, iVscrollPos, TRUE);

	iHscrollMax = iCharsPerLine - 1;
	iHscrollPos = 0;
	SetScrollRange (hwndHBar, SB_CTL, 0, iHscrollMax, FALSE);
	SetScrollPos (hwndHBar, SB_CTL, iHscrollPos, TRUE);

	set_wnd_title ();
	if (hwnd == GetFocus ())
		set_caret_pos ();
	repaint ();
	return TRUE;
}

//--------------------------------------------------------------------------------------------
// When focus is gained.
int HexEditorWindow::set_focus ()
{
	if (cxChar == 0 || cyChar == 0)
	{
		make_font ();
		HDC hdc = GetDC (hwnd);
		HFONT of = (HFONT) SelectObject (hdc, hFont);
		TEXTMETRIC tm;
		GetTextMetrics (hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		SelectObject (hdc, of);
		ReleaseDC (hwnd, hdc);
	}
	update_for_new_datasize ();
	CreateCaret (hwnd, NULL, cxChar, cyChar);
	set_caret_pos ();
	ShowCaret (hwnd);
	return 0;
}

//--------------------------------------------------------------------------------------------
// What to do when focus is lost.
int HexEditorWindow::kill_focus ()
{
	HideCaret (hwnd);
	DestroyCaret ();
	return TRUE;
}

//--------------------------------------------------------------------------------------------
// Handler for non-character keys (arrow keys, page up/down etc.)
int HexEditorWindow::keydown (int key)
{
	if (filename[0] == '\0' || iCurByte<0)
		return 0;

	int log_column, scrollflag;

	if (bSelected)
		switch (key)
		{
		case VK_END:
		case VK_HOME:
		case VK_LEFT:
		case VK_RIGHT:
		case VK_UP:
		case VK_DOWN:
		case VK_PRIOR:
		case VK_NEXT:
			iCurByte = iEndOfSelection;
			bSelected = FALSE;
			adjust_vscrollbar ();
			repaint ();
		default:
			break;
		}

	switch (key)
	{
	case VK_HOME:
	{
		iCurByte = BYTELINE * iBytesPerLine;
		iCurNibble = 0;
		if (m_iEnteringMode == BYTES)
			log_column = BYTES_LOGICAL_COLUMN;
		else
			log_column = CHARS_LOGICAL_COLUMN;
		if (log_column < iHscrollPos) // Cursor too far to the left?
		{
			iHscrollPos = log_column;
			adjust_hscrollbar ();
			repaint ();
		}
		else
		{
			repaint (BYTELINE);
		}
		if (CURSOR_TOO_HIGH || CURSOR_TOO_LOW)
		{
			iCurLine = max (0, iCurByte/iBytesPerLine-cyBuffer/2);
			adjust_vscrollbar ();
			repaint ();
		}
		break;
	}
	
	case VK_END:
	{
		// Set position:
		iCurByte = (BYTELINE + 1) * iBytesPerLine - 1;
		if (iCurByte > LASTBYTE+1)
			iCurByte = LASTBYTE+1;
		iCurNibble = 1;

		if (m_iEnteringMode == BYTES)
			log_column = BYTES_LOGICAL_COLUMN;
		else
			log_column = CHARS_LOGICAL_COLUMN;
		
		// If logical column too far to right:
		if (log_column > LAST_LOG_COLUMN)
		{
			iHscrollPos = log_column - cxBuffer + 1;
			adjust_hscrollbar ();
			repaint ();
		}
		else
		{
			repaint (BYTELINE);
		}
		
		if (CURSOR_TOO_HIGH || CURSOR_TOO_LOW)
		{
			iCurLine = max (0, iCurByte/iBytesPerLine-cyBuffer/2);
			adjust_vscrollbar ();
			repaint ();
		}
		break;
	}

	case VK_UP:
		// If possible, cursor one line up.
		if (iCurByte >= iBytesPerLine)
			iCurByte -= iBytesPerLine;
		// If cursor pos. before or behind visible area...
		scrollflag = FALSE;
		if (CURSOR_TOO_HIGH || CURSOR_TOO_LOW)
		{
			adjust_vscrollbar ();
			scrollflag = TRUE;
		}

		if (m_iEnteringMode == BYTES)
			log_column = BYTES_LOGICAL_COLUMN;
		else
			log_column = CHARS_LOGICAL_COLUMN;

		// If cursor too far horiz.
		if (log_column < iHscrollPos || log_column > LAST_LOG_COLUMN)
		{
			iHscrollPos = log_column;
			adjust_hscrollbar ();
			repaint ();
			break;
		}
		else if (!scrollflag)
		{
			repaint (BYTELINE+1);
			repaint (BYTELINE);
		}
		else // scrollflag
		{
			if (iCurLine-BYTELINE==1) // One line up.
			{
				iCurLine = BYTELINE;
				RECT r;
				r.left = CLIENT_BORDER_WIDTH;
				r.top = CLIENT_BORDER_WIDTH;
				r.right = cxClient - CLIENT_BORDER_WIDTH;
				r.bottom = cyClient - CLIENT_BORDER_WIDTH;
				ScrollWindow (hwnd, 0, cyChar, &r, &r);
				repaint (BYTELINE+1);
				repaint (BYTELINE);
			}
			else
			{
				iCurLine = BYTELINE;
				repaint ();
			}
		}
		break;

	case VK_DOWN:
		// If possible cursor one line down.
		if (iCurByte+iBytesPerLine <= DataArray.GetLength())
			iCurByte += iBytesPerLine;
		
		scrollflag = FALSE;
		if (CURSOR_TOO_HIGH || CURSOR_TOO_LOW)
		{
			adjust_vscrollbar ();
			scrollflag = TRUE;
		}

		if (m_iEnteringMode == BYTES)
			log_column = BYTES_LOGICAL_COLUMN;
		else
			log_column = CHARS_LOGICAL_COLUMN;

		if (log_column < iHscrollPos || log_column > LAST_LOG_COLUMN)
		{
			iHscrollPos = log_column;
			adjust_hscrollbar ();
			repaint ();
			break;
		}
		else if (!scrollflag)
		{
			repaint (BYTELINE-1);
			repaint (BYTELINE);
		}
		else // scrollflag
		{
			if (BYTELINE==iCurLine+cyBuffer) // Cursor one line down.
			{
				iCurLine = max (0, BYTELINE-(cyBuffer-1)); // Cursor at bottom of screen.
				RECT r;
				r.left = CLIENT_BORDER_WIDTH;
				r.top = CLIENT_BORDER_WIDTH;
				r.right = cxClient - CLIENT_BORDER_WIDTH;
				r.bottom = cyClient - CLIENT_BORDER_WIDTH;
				ScrollWindow (hwnd, 0, -cyChar, &r, &r);
				repaint (BYTELINE-1);
				repaint (BYTELINE);
				repaint (BYTELINE+1);
				mark_char( 0 );
			}
			else
			{
				iCurLine = max (0, BYTELINE-(cyBuffer-1));
				repaint ();
			}
		}
		break;
	
	case VK_LEFT:
	{
		if (m_iEnteringMode == BYTES) // BYTE-Mode.
		{
			if (iCurNibble == 1) // 1 nibble to the left...
				iCurNibble = 0;
			else if (iCurByte != 0) // Or 1 byte to the left.
			{
				iCurByte--;
				iCurNibble = 1;
			}
		}
		else // CHAR-Mode.
		{
			if (iCurByte != 0)
				iCurByte--; // 1 Byte to the left.
		}

		// If new line not visible, then scroll.
		scrollflag = FALSE;
		if (CURSOR_TOO_HIGH || CURSOR_TOO_LOW)
		{
			iCurLine = BYTELINE;
			adjust_vscrollbar ();
			scrollflag = TRUE;
		}
		
		if (m_iEnteringMode == BYTES)
			log_column = BYTES_LOGICAL_COLUMN;
		else
			log_column = CHARS_LOGICAL_COLUMN;

		if (log_column < iHscrollPos)
		{
			iHscrollPos = log_column;
			adjust_hscrollbar ();
			repaint ();
			break;
		}
		else if (log_column > LAST_LOG_COLUMN)
		{
			iHscrollPos = log_column-(cxBuffer-1);
			adjust_hscrollbar ();
			repaint ();
			break;
		}
		else if (!scrollflag)
		{
			if (iCurByte%iBytesPerLine==iBytesPerLine-1) // Just got to previous line.
				repaint (BYTELINE+1);
			repaint (BYTELINE);
		}
		else
			repaint ();
		break;
	}

	case VK_RIGHT:
	{
		if (m_iEnteringMode == BYTES)
		{
			if (iCurNibble == 0)
				iCurNibble = 1;
			else if (iCurByte <= LASTBYTE)
			{
				iCurNibble = 0;
				iCurByte++;
			}
		}
		else
		{
			if (iCurByte <= LASTBYTE)
				iCurByte++;
		}
		
		scrollflag = FALSE;
		if (CURSOR_TOO_HIGH || CURSOR_TOO_LOW)
		{
			iCurLine = max (BYTELINE-cyBuffer+1, 0);
			adjust_vscrollbar ();
			scrollflag = TRUE;
		}

		if (m_iEnteringMode == BYTES)
			log_column = BYTES_LOGICAL_COLUMN;
		else
			log_column = CHARS_LOGICAL_COLUMN;

		if (log_column >= iHscrollPos+cxBuffer)
		{
			iHscrollPos = log_column-(cxBuffer-1);
			adjust_hscrollbar ();
			repaint ();
			break;
		}
		else if (log_column < iHscrollPos)
		{
			iHscrollPos = log_column;
			adjust_hscrollbar ();
			repaint ();
			break;
		}
		else if (scrollflag != TRUE) // If one line down but cursor not too far horiz.
		{
			if (iCurByte%iBytesPerLine==0)
				repaint (BYTELINE-1);
			repaint (BYTELINE);
		}
		else
			repaint ();
		break;
	}

	case VK_PRIOR:
		if (BYTELINE >= cyBuffer)
		{
			iCurByte -= cyBuffer * iBytesPerLine; // 1 page up.
			iCurLine -= cyBuffer;
			if (iCurLine < 0)
				iCurLine = BYTELINE;
		}
		else
		{
			iCurLine = 0; // To top.
			iCurByte = BYTEPOS;
		}
		if (CURSOR_TOO_HIGH || CURSOR_TOO_LOW)
			iCurLine = iCurByte/iBytesPerLine;
		adjust_vscrollbar ();
		repaint ();
		break;

	case VK_NEXT:
		iCurByte += cyBuffer*iBytesPerLine;
		if (iCurByte > LASTBYTE+1)
		{
			iCurByte = (LASTBYTE+1)/iBytesPerLine*iBytesPerLine + BYTEPOS;
			if (iCurByte > LASTBYTE+1)
				iCurByte = LASTBYTE+1;
			iCurLine = BYTELINE;
			adjust_view_for_caret ();
		}
		else
		{
			iCurLine += cyBuffer;
			if (iCurLine > LASTLINE)
				iCurLine = BYTELINE;
		}
		if (CURSOR_TOO_HIGH || CURSOR_TOO_LOW)
			iCurLine = max (BYTELINE-cyBuffer+1, 0);
		adjust_vscrollbar ();
		repaint ();
		break;
	}
	return 0;
}

//--------------------------------------------------------------------------------------------
// Handler for character keys.
int HexEditorWindow::character (char ch)
{
	// If there is selection return now.
	if (bSelected)
		return 0;

	// If we are in read-only mode, give a warning and return,
	// except if TAB was pressed.
	if( bReadOnly && ch != '\t' )
	{
		MessageBox( hwnd, "Can't change file because read-only mode is engaged!", "Keyboard editing", MB_OK | MB_ICONERROR );
		return 0;
	}

	char x, c = tolower (ch);
	if (ch == '\t') // TAB => change EnteringMode.
	{
		if (m_iEnteringMode == BYTES)
			m_iEnteringMode = CHARS;
		else
			m_iEnteringMode = BYTES;
		
		int log_column;
		if (m_iEnteringMode == BYTES)
			log_column = BYTES_LOGICAL_COLUMN;
		else
			log_column = CHARS_LOGICAL_COLUMN;

		if (log_column >= iHscrollPos+cxBuffer) // Cursor too far right...
			iHscrollPos = log_column-(cxBuffer-1); // Cursor to right border.
		else if (log_column < iHscrollPos) // Cursor too far left.
			iHscrollPos = log_column; // Cursor to left border.
		adjust_hscrollbar ();
		repaint ();
		return 0;
	}

	// If read-only mode, return.
	if( bReadOnly )
		return 1;
	
	// If in bytes and char is not a hex digit, return.
	if (m_iEnteringMode==BYTES && !((c>='a'&&c<='f')||(c>='0'&&c<='9')))
		return 1;

	// If caret at EOF.
	if (iCurByte == DataArray.GetLength())
	{
		if (DataArray.InsertAtGrow(iCurByte, 0, 1) == TRUE)
		{
			iCurNibble = 0;
			iInsertMode = FALSE;
			character (ch);
			update_for_new_datasize ();
			return 1;
		}
		else
		{
			MessageBox (NULL, "Not enough memory for inserting character.", "Insert mode error", MB_OK | MB_ICONERROR);
		}
		return 0;
	}

	if( iInsertMode )
	{
		// INSERT
		if( m_iEnteringMode == BYTES )
		{
			if( ( c >= 'a' && c <= 'f' ) || ( c >= '0' && c <= '9' ) )
			{
				if( bInsertingHex )
				{
					// Expecting the lower nibble of the recently inserted byte now.
					// The bInsertingHex mode must be turned off if anything other is done
					// except entering a valid hex digit. This is checked for in the
					// HexEditorWindow::OnWndMsg() method.
					bInsertingHex = FALSE;
					if (c >= 'a' && c <= 'f')
						x = c - 0x61 + 0x0a;
					else
						x = c - 0x30;
					DataArray[iCurByte] = (DataArray[iCurByte] & 0xf0) | x;
					m_iFileChanged = TRUE;
					bFilestatusChanged = TRUE;
					iCurByte++;
					iCurNibble = 0;
					update_for_new_datasize();
				}
				else
				{
					// Insert a new byte with the high nibble set to value just typed.
					if( DataArray.InsertAtGrow( iCurByte, 0, 1 ) == TRUE )
					{
						bInsertingHex = TRUE;
						if (c >= 'a' && c <= 'f')
							x = c - 0x61 + 0x0a;
						else
							x = c - 0x30;
						DataArray[iCurByte] = (DataArray[iCurByte] & 0x0f) | (x << 4);
						m_iFileChanged = TRUE;
						bFilestatusChanged = TRUE;
						iCurNibble = 1;
						update_for_new_datasize();
					}
					else
					{
						MessageBox (NULL, "Not enough memory for inserting.", "Insert mode error", MB_OK | MB_ICONERROR);
						return 0;
					}
				}
			}
			return 1;
		}
		else if (m_iEnteringMode == CHARS)
		{
			if (DataArray.InsertAtGrow(iCurByte, 0, 1) == TRUE)
			{
				iCurNibble = 0;
				iInsertMode = FALSE;
				character (ch);
				iInsertMode = TRUE;
				iCurNibble = 0;
				update_for_new_datasize ();
			}
			else
			{
				MessageBox (NULL, "Not enough memory for inserting.", "Insert mode error", MB_OK | MB_ICONERROR);
				return 0;
			}
		}
		return 1;
	}
	else
	{
		// OVERWRITE
		// TAB => change mode.
		// Byte-mode: only a-f, 0-9 allowed.
		if ((m_iEnteringMode == BYTES) && ((c >= 'a' && c <= 'f') || (c >= '0' && c <= '9')))
		{
			if (c >= 'a' && c <= 'f')
				x = c - 0x61 + 0x0a;
			else
				x = c - 0x30;
			if (iCurNibble == 0)
				DataArray[iCurByte] = (DataArray[iCurByte] & 0x0f) | (x << 4);
			else
				DataArray[iCurByte] = (DataArray[iCurByte] & 0xf0) | x;
			m_iFileChanged = TRUE;
			bFilestatusChanged = TRUE;
			keydown (VK_RIGHT);
		}
		// Char-mode.
		else if (m_iEnteringMode == CHARS)
		{
			switch (iCharacterSet)
			{
			case ANSI_FIXED_FONT:
				DataArray[iCurByte] = ch;
				break;

			case OEM_FIXED_FONT:
				{
					char src[2], dst[2];
					src[0] = ch;
					src[1] = 0;
					CharToOem (src, dst);
					DataArray[iCurByte] = dst[0];
				}
				break;
			}
			m_iFileChanged = TRUE;
			bFilestatusChanged = TRUE;
			keydown (VK_RIGHT);
		}
	}
	return 0;
}

//--------------------------------------------------------------------------------------------
// Handler for vertical scrollbar.
int HexEditorWindow::vscroll (int cmd, int pos)
{
	if (NO_FILE || DataArray.GetLength()==0)
		return 0;

	iVscrollInc = 0;
	switch (cmd)
	{
	case SB_TOP:
		iCurLine = 0;
		break;
	case SB_BOTTOM:
		iCurLine = iNumlines-1;
		break;
	case SB_LINEUP:
		if (iCurLine > 0)
			iCurLine -= 1;
		break;
	case SB_LINEDOWN:
		if (iCurLine < iNumlines-1)
			iCurLine += 1;
		break;
	case SB_PAGEUP:
		if (iCurLine >= cyBuffer)
			iCurLine -= cyBuffer;
		else
			iCurLine = 0;
		break;
	case SB_PAGEDOWN:
		if (iCurLine <= iNumlines-1-cyBuffer)
			iCurLine += cyBuffer;
		else
			iCurLine = iNumlines-1;
		break;
	case SB_THUMBTRACK:
		iCurLine = (int) (pos * ((float)(iNumlines-1)/(float)iVscrollMax));
		SetScrollPos (hwndVBar, SB_CTL, pos, TRUE);
		if (iCurLine > iNumlines-1)
			iCurLine = iNumlines-1;
		// Make sure the number of empty lines is as small as possible.
		if( iNumlines - iCurLine < cyBuffer )
		{
			iCurLine = ( ( DataArray.GetUpperBound() + 1 ) / iBytesPerLine ) - ( cyBuffer - 1 );
			if( iCurLine < 0 )
				iCurLine = 0;
		}
		repaint();
		return 0;
	default:
		break;
	}
	iVscrollPos = (int) ((float)iCurLine * ((float)iVscrollMax)/(float)(iNumlines-1));
	SetScrollPos (hwndVBar, SB_CTL, iVscrollPos, TRUE);
	if (iCurLine > iNumlines-1)
		iCurLine = iNumlines-1;
	repaint ();
	return 0;
}

//--------------------------------------------------------------------------------------------
// Handler for horizontal scrollbar.
int HexEditorWindow::hscroll (int cmd, int pos)
{
	if (NO_FILE || DataArray.GetLength()==0)
		return 0;

	iHscrollInc = 0;
	switch (cmd)
	{
	case SB_TOP:
		iHscrollInc = -iHscrollPos;
		break;
	case SB_BOTTOM:
		iHscrollInc = iHscrollMax - iHscrollPos;
		break;
	case SB_LINEUP:
		if (iHscrollPos > 0)
			iHscrollInc = -1;
		break;
	case SB_LINEDOWN:
		if (iHscrollPos < iHscrollMax)
			iHscrollInc = 1;
		break;
	case SB_PAGEUP:
		if (iHscrollPos >= cxBuffer)
			iHscrollInc = -cxBuffer;
		else
			iHscrollInc = -iHscrollPos;
		break;
	case SB_PAGEDOWN:
		if (iHscrollPos <= iHscrollMax-cxBuffer)
			iHscrollInc = cxBuffer;
		else
			iHscrollInc = iHscrollMax - iHscrollPos;
		break;
	case SB_THUMBTRACK:
		iHscrollInc = pos - iHscrollPos;
		break;
	default:
		break;
	}
	iHscrollPos += iHscrollInc;
	SetScrollPos (hwndHBar, SB_CTL, iHscrollPos, TRUE);
	InvalidateRect (hwnd, NULL, FALSE);
	UpdateWindow (hwnd);
	return 0;
}

//--------------------------------------------------------------------------------------------
// WM_PAINT handler.
int HexEditorWindow::paint()
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint (hwnd, &ps);
	//-------------------------------------------------------
	HideCaret (hwnd);
	// Delete remains of last position.
	int a, b;
	b = min (iCurLine + cyBuffer, iNumlines-1);
	iBkColor = PALETTERGB (GetRValue(iBkColorValue),GetGValue(iBkColorValue),GetBValue(iBkColorValue));
	iTextColor = PALETTERGB (GetRValue(iTextColorValue),GetGValue(iTextColorValue),GetBValue(iTextColorValue));
	SetTextColor (hdc, iTextColor);
	SetBkColor (hdc, iBkColor);
	HPEN pen1 = CreatePen (PS_SOLID, 1, iBkColor);
	HPEN oldpen = (HPEN) SelectObject (hdc, pen1);
	HBRUSH brush1 = CreateSolidBrush (iBkColor);
	HBRUSH oldbrush = (HBRUSH) SelectObject (hdc, brush1);
	// Delete lower border if there are empty lines on screen.
	if ((b-iCurLine+1)*cyChar+CLIENT_BORDER_WIDTH < cyClient-CLIENT_BORDER_WIDTH)
		Rectangle (hdc, CLIENT_BORDER_WIDTH, (b-iCurLine+1)*cyChar+CLIENT_BORDER_WIDTH, cxClient-CLIENT_BORDER_WIDTH,
			cyClient-CLIENT_BORDER_WIDTH);
	// Delete right border.
	Rectangle (hdc, ((iHscrollMax+1)-iHscrollPos)*cxChar+CLIENT_BORDER_WIDTH, CLIENT_BORDER_WIDTH,
		cxClient-CLIENT_BORDER_WIDTH, cyClient-CLIENT_BORDER_WIDTH);
	SelectObject (hdc, oldpen);
	SelectObject (hdc, oldbrush);
	DeleteObject (pen1);
	DeleteObject (brush1);
	
	// Get font.
	HFONT oldfont = (HFONT) SelectObject (hdc, hFont);
	HPEN sep_pen = CreatePen (PS_SOLID, 1, iSepColorValue);
	oldpen = (HPEN) SelectObject (hdc, sep_pen);

	if( Linebuffer.GetSize() < iCharsPerLine )
	{
		// Linebuffer too small.
		if( Linebuffer.SetSize( iCharsPerLine ) )
		{
			Linebuffer.ExpandToSize();
			// Linebuffer successfully resized.
			HBRUSH hbr = CreateSolidBrush( iBmkColor );
			if (iUpdateLine == -1)
			{
				for (a = iCurLine; a <= b; a++)
					print_line( hdc, a, Linebuffer, hbr );
			}
			else
			{
				print_line( hdc, iUpdateLine, Linebuffer, hbr );
			}
			DeleteObject( hbr );
			SelectObject (hdc, oldpen);
			DeleteObject (sep_pen);
			SelectObject (hdc, oldfont);
			// Mark character.
			mark_char (hdc);
			// Draw client-border.
			draw_client_border (hdc);
			ShowCaret (hwnd);
			EndPaint (hwnd, &ps);
			set_caret_pos ();
			set_wnd_title ();
			iUpdateLine = -1;
			return 0;
		}
		else
		{
			// Could not allocate line buffer.
			Rectangle( hdc, 0, 0, cxClient, cyClient );
			char buf[] = "Error: could not allocate line buffer.\nPlease save your changes and restart frhed.";
			RECT r;
			r.top = CLIENT_BORDER_WIDTH;
			r.left = CLIENT_BORDER_WIDTH;
			r.right = cxClient;
			r.bottom = cyClient;
			DrawText( hdc, buf, -1, &r, DT_LEFT );
		}
		
	}
	else
	{
		// Linebuffer large enough.
			HBRUSH hbr = CreateSolidBrush( iBmkColor );
			if (iUpdateLine == -1)
			{
				for (a = iCurLine; a <= b; a++)
					print_line( hdc, a, Linebuffer, hbr );
			}
			else
			{
				print_line( hdc, iUpdateLine, Linebuffer, hbr );
			}
			DeleteObject( hbr );
			SelectObject (hdc, oldpen);
			DeleteObject (sep_pen);
			SelectObject (hdc, oldfont);
			// Mark character.
			mark_char (hdc);
			// Draw client-border.
			draw_client_border (hdc);
			ShowCaret (hwnd);
			EndPaint (hwnd, &ps);
			set_caret_pos ();
			set_wnd_title ();
			iUpdateLine = -1;
			return 0;
	}
	return 0;
}

//--------------------------------------------------------------------------------------------
// Receives WM_COMMAND messages and passes either them to their handler functions or
// processes them here.
int HexEditorWindow::command (int cmd)
{
	HMENU hMenu = GetMenu (hwnd);
	switch( cmd )
	{
//Pabs changed - line insert
	case IDM_FILL_WITH:
		CMD_fw();
		break;
	case IDM_REVERT:
		CMD_revert();
		break;
	case IDM_SAVESELAS:
		CMD_saveselas();
		break;
	case IDM_DELETEFILE:
		CMD_deletefile();
		break;
	case IDM_INSERTFILE:
		CMD_insertfile();
		break;
//end
	case IDM_REPLACE:
		CMD_replace();
		break;

	case IDM_EXPLORERSETTINGS:
		CMD_explorersettings();
		break;

	case IDM_OPEN_TEXT:
		CMD_summon_text_edit();
		break;

	case IDM_FINDPREV:
		CMD_findprev();
		break;

	case IDM_FINDNEXT:
		CMD_findnext();
		break;

	case IDM_BMK_COLOR:
		CMD_color_settings( &iBmkColor );
		break;

	case IDM_RESET_COLORS:
		CMD_colors_to_default();
		break;

	case IDM_EDIT_READONLYMODE:
		if( bReadOnly == FALSE )
			bReadOnly = TRUE;
		else
			bReadOnly = FALSE;
		update_for_new_datasize();
		break;

	case IDM_APPLYTEMPLATE:
		CMD_apply_template();
		break;

	case IDM_PARTIAL_OPEN:
		CMD_open_partially ();
		break;

	case IDM_CLEARALL_BMK:
		CMD_clear_all_bmk ();
		break;

	case IDM_REMOVE_BKM:
		CMD_remove_bkm ();
		break;

	case IDM_BOOKMARK1: case IDM_BOOKMARK2: case IDM_BOOKMARK3: 
		case IDM_BOOKMARK4: case IDM_BOOKMARK5: case IDM_BOOKMARK6: 
		case IDM_BOOKMARK7: case IDM_BOOKMARK8: case IDM_BOOKMARK9: 
			CMD_goto_bookmark (cmd);
		break;

	case IDM_ADDBOOKMARK:
		CMD_add_bookmark ();
		break;

	case IDM_MRU1: case IDM_MRU2: case IDM_MRU3: case IDM_MRU4: case IDM_MRU5:
		case IDM_MRU6: case IDM_MRU7: case IDM_MRU8: case IDM_MRU9:
		CMD_MRU_selected (cmd);
		break;

	case IDM_SELECT_BLOCK:
		CMD_select_block ();
		break;

	case IDM_BINARYMODE:
		CMD_binarymode ();
		break;

	case IDM_COMPARE:
		CMD_compare ();
		break;

	case IDM_READFLOAT:
		{
			char buf[500], buf2[50];
			buf[0] = 0;
			float floatval;
			if (DataArray.GetLength()-iCurByte >= 4)
			{
				// Space enough for float.
				if (iBinaryMode == LITTLEENDIAN_MODE)
				{
					floatval = *((float*)&(DataArray[iCurByte]));
				}
				else // BIGENDIAN_MODE
				{
					char* pf = (char*) &floatval;
					int i;
					for (i=0; i<4; i++)
						pf[i] = DataArray[iCurByte+3-i];
				}
				sprintf (buf, "float size value:\n%f\n", floatval);
			}
			else
				sprintf (buf, "Not enough space for float size value.\n");
			double dval;
			if (DataArray.GetLength()-iCurByte >= 8)
			{
				// Space enough for double.
				if (iBinaryMode == LITTLEENDIAN_MODE)
				{
					dval = *((double*)&(DataArray[iCurByte]));
				}
				else // BIGENDIAN_MODE
				{
					char* pd = (char*) &dval;
					int i;
					for (i=0; i<8; i++)
						pd[i] = DataArray[iCurByte+7-i];
				}
				sprintf (buf2, "\ndouble size value:\n%g\n", dval);
				strcat (buf, buf2);
			}
			else
			{
				sprintf (buf2, "\nNot enough space for double size value.\n");
				strcat (buf, buf2);
			}
			// MessageBox (NULL, buf, "Floating point values", MB_OK | MB_ICONINFORMATION);
			MessageCopyBox (NULL, buf, "Floating point values", MB_ICONINFORMATION, hwnd);
			break;
		}

	case IDM_PROPERTIES:
		CMD_properties ();
		break;

	case IDM_SELECT_ALL:
		CMD_select_all ();
		break;

	case IDA_BACKSPACE:
		CMD_on_backspace ();
		break;

	case IDA_INSERTMODETOGGLE:
		CMD_toggle_insertmode ();
		break;

	case IDA_DELETEKEY:
		CMD_on_deletekey ();
		break;

	case IDM_CHARACTER_SET:
		CMD_character_set ();
		break;

	case IDM_EDIT_MANIPULATEBITS:
		CMD_manipulate_bits ();
		break;

	case IDM_EDIT_APPEND:
		CMD_edit_append ();
		break;

	case IDM_SELTEXT_COLOR:
		CMD_color_settings( &iSelTextColorValue );
		break;

	case IDM_SELBACK_COLOR:
		CMD_color_settings( &iSelBkColorValue );
		break;

	case IDM_SEP_COLOR:
		CMD_color_settings (&iSepColorValue);
		break;

	case IDM_TEXT_COLOR:
		CMD_color_settings (&iTextColorValue);
		break;

	case IDM_BK_COLOR:
		CMD_color_settings (&iBkColorValue);
		break;

	case IDM_VIEW_SETTINGS:
		CMD_view_settings ();
		break;

	case IDA_SELECTPAGEDOWN:
		CMD_select_with_arrowkeys (IDA_SELECTPAGEDOWN);
		break;

	case IDA_SELECTPAGEUP:
		CMD_select_with_arrowkeys (IDA_SELECTPAGEUP);
		break;

	case IDA_SELECTSTARTOFLINE:
		CMD_select_with_arrowkeys (IDA_SELECTSTARTOFLINE);
		break;

	case IDA_SELECTENDOFLINE:
		CMD_select_with_arrowkeys (IDA_SELECTENDOFLINE);
		break;

	case IDA_SELECTSTARTOFFILE:
		CMD_select_with_arrowkeys (IDA_SELECTSTARTOFFILE);
		break;

	case IDA_SELECTENDOFFILE:
		CMD_select_with_arrowkeys (IDA_SELECTENDOFFILE);
		break;

	case IDA_SELECTLEFT:
		CMD_select_with_arrowkeys (IDA_SELECTLEFT);
		break;

	case IDA_SELECTRIGHT:
		CMD_select_with_arrowkeys (IDA_SELECTRIGHT);
		break;

	case IDA_SELECTUP:
		CMD_select_with_arrowkeys (IDA_SELECTUP);
		break;

	case IDA_SELECTDOWN:
		CMD_select_with_arrowkeys (IDA_SELECTDOWN);
		break;

	case IDM_INTERNALSTATUS:
		{
			// Remove break for internal information on F2.
			// break;

			char buf[4000], buf2[300];
			sprintf (buf, "Data length: %d\n", DataArray.GetLength ());
			sprintf (buf, "Upper Bound: %d\n", DataArray.GetUpperBound ());
			sprintf (buf2, "Data size: %d\n", DataArray.GetSize ());
			strcat (buf, buf2);
			sprintf (buf2, "iNumlines: %d\n", iNumlines);
			strcat (buf, buf2);
			sprintf (buf2, "iCurLine: %d\n", iCurLine);
			strcat (buf, buf2);
			sprintf (buf2, "iCurByte: %d\n", iCurByte);
			strcat (buf, buf2);
			sprintf( buf2, "cyBuffer: %d\n", cyBuffer );
			strcat( buf, buf2 );

			sprintf (buf2, "iMRU_count: %d\n", iMRU_count);
			strcat (buf, buf2);
			int i;
			for (i=0; i<MRUMAX; i++)
			{
				sprintf (buf2, "MRU %d=%s\n", i, &(strMRU[i][0]));
				strcat (buf, buf2);
			}
			MessageBox (NULL, buf, "Internal status", MB_OK);
			break;
		}

	case IDM_EDIT_CUT:
		iCutMode = BST_CHECKED;
		CMD_edit_cut ();
		break;

	case IDM_HELP_TOPICS:
		{
			char path[500];
			strcpy (path, _pgmptr);
			int len = strlen (path);
			path[len-3] = 'h';
			path[len-2] = 'l';
			path[len-1] = 'p';
			WinHelp (hwnd, path, HELP_CONTENTS, 0);
		}
		break;

	case IDM_EDIT_ENTERDECIMALVALUE:
		CMD_edit_enterdecimalvalue ();
		break;

	case IDM_COPY_HEXDUMP:
		CMD_copy_hexdump ();
		break;

	case IDM_EDIT_COPY:
		CMD_edit_copy ();
		break;

	case IDM_PASTE_WITH_DLG:
		CMD_edit_paste ();
		break;

	case IDM_EDIT_PASTE:
		CMD_fast_paste ();
		break;

	case IDM_ABOUT:
		DialogBox (hInstance, MAKEINTRESOURCE (IDD_ABOUTDIALOG), hwnd, (DLGPROC) AboutDlgProc);
		break;
	
	case IDM_FIND:
		CMD_find ();
		break;

	case IDM_GO_TO:
		CMD_goto();
		break;

	case IDM_CHANGE_MODE:
		character ('\t');
		break;

	case IDM_SAVE_AS:
		CMD_save_as ();
		break;

	case IDM_SAVE:
		CMD_save ();
		break;

	case IDM_EXIT:
		SendMessage (hwnd, WM_CLOSE, 0, 0);
		break;

	case IDM_SCROLL_UP:
		if (iCurLine > 0)
		{
			iCurLine--;
			adjust_vscrollbar ();
			repaint ();
		}
		break;

	case IDM_SCROLL_DOWN:
		if (iCurLine < iNumlines-1)
		{
			iCurLine++;
			adjust_vscrollbar ();
			repaint ();
		}
		break;

	case IDM_SCROLL_LEFT:
		if (iHscrollPos > 0)
		{
			iHscrollPos--;
			adjust_hscrollbar ();
			repaint ();
		}
		break;
	
	case IDM_SCROLL_RIGHT:
		if (iHscrollPos < iHscrollMax)
		{
			iHscrollPos++;
			adjust_hscrollbar ();
			repaint();
		}
		break;

	case IDM_BOTTOM:
	{
		iCurByte = DataArray.GetUpperBound()+1;
		iCurNibble = 1;
		iCurLine = BYTELINE-(cyBuffer-1); // Cursor visible on lower border.
		if (iCurLine < 0)
			iCurLine = 0;
		adjust_view_for_caret ();
		adjust_vscrollbar ();
		bSelected = FALSE;
		repaint();
		break;
	}

	case IDM_TOP:
		iCurByte = iCurLine = iCurNibble = 0;
		adjust_view_for_caret ();
		adjust_vscrollbar ();
		bSelected = FALSE;
		repaint();
		break;

	case IDM_OPEN:
		CMD_open ();
		break;
	
	case IDM_NEW:
		CMD_new ();
		break;
	
	default:
		{
			char buf[500];
			sprintf (buf, "Unknown COMMAND-ID %d.", cmd);
			MessageBox (NULL, buf, "frhed ERROR", MB_OK);
		}
		break;
	}
	return 0;
}

//--------------------------------------------------------------------------------------------
int HexEditorWindow::destroy_window ()
{
	return 0;
}

//--------------------------------------------------------------------------------------------
// Set the window title and the statusbar text.
void HexEditorWindow::set_wnd_title()
{
	char buf[512];
	if (strlen (filename) != 0)
	{
		// Change window title.
		if (bFilestatusChanged)
		{
			sprintf (buf, "[%s", filename);
			if (m_iFileChanged == TRUE)
				strcat (buf, " *");
			strcat (buf, "]");
			if (bPartialOpen==TRUE)
				strcat (buf, " - P");
			strcat (buf, " - frhed");
			SetWindowText (hwnd, buf);
			bFilestatusChanged = FALSE;
		}
		// Selection going on.
		if (bSelected == TRUE)
		{
			if (iEndOfSelection >= iStartOfSelection)
			{
				sprintf (buf, "Selected: Offset %d=0x%x to %d=0x%x (%d byte(s))", iStartOfSelection, iStartOfSelection,
					iEndOfSelection, iEndOfSelection, iEndOfSelection-iStartOfSelection+1);
			}
			else
			{
				sprintf (buf, "Selected: Offset %d=0x%x to %d=0x%x (%d byte(s))", iEndOfSelection, iEndOfSelection,
					iStartOfSelection, iStartOfSelection, iStartOfSelection-iEndOfSelection+1);
			}
			SendMessage (hwndStatusBar, SB_SETTEXT, 0, (LPARAM) buf);
		}
		else // Normal display.
		{
//Pabs changed - \t removed from end of string literal ("Offset %d=0x%x\t" -> "Offset %d=0x%x")
			sprintf (buf, "Offset %d=0x%x", iCurByte, iCurByte);
//end
			int wordval, longval;
			char buf2[80];
//Pabs changed - line insert
			if (DataArray.GetLength()-iCurByte > 0){//if we are not looking at the End byte
				// R. Kibria: changed the output slightly (used to be "Bits = 0b").
				strcat (buf, "   Bits=");//append stuff to status text
				unsigned char zzz=DataArray[iCurByte];//quicker to have a tmp var than to call operator[] 8 times
				for(int i=0;i<8;i++)buf2[i]=((zzz>>i)&0x1?'1':'0');//Extract bits
				for(int i=0;i<4;i++)swap(buf2[i],buf2[7-i]);//flip order-for some reason it doesn't display correctly going i-- or i++ in for loop
				buf2[8]='\0';//terminate string
				strcat (buf, buf2);//append to status text
			}
			strcat (buf, "\t");//add that \t back on to the status text
//end
			if (bUnsignedView) // Values signed/unsigned?
			{
				// UNSIGNED
				if (iBinaryMode == LITTLEENDIAN_MODE)
				{
					// UNSIGNED LITTLEENDIAN_MODE
					// Decimal value of byte.
					if (DataArray.GetLength ()-iCurByte >= 1)
					{
						sprintf (buf2, "\tUnsigned: B:%u", (unsigned int) DataArray[iCurByte]);
						strcat (buf, buf2);
					}
					else
					{
						sprintf (buf2, "\tEND");
						strcat (buf, buf2);
					}
					// Space enough for a word?
					if (DataArray.GetLength ()-iCurByte >= 2)
					{
						// Space enough for a word.
						wordval = (DataArray[iCurByte+1] << 8) | DataArray[iCurByte];
						sprintf (buf2, ",W:%u", (unsigned int) wordval);
						strcat (buf, buf2);
					}
					if (DataArray.GetLength ()-iCurByte >= 4)
					{
						// Space enough for a longword.
						longval = wordval | (((DataArray[iCurByte + 3] << 8) | DataArray[iCurByte + 2]) << 16);
						sprintf (buf2, ",L:%u", (unsigned int) longval);
						strcat (buf, buf2);
					}
				}
				else
				{
					// UNSIGNED BIGENDIAN_MODE
					// Decimal value of byte.
					if (DataArray.GetLength ()-iCurByte >= 1)
					{
						sprintf (buf2, "\tUnsigned: B:%u", (unsigned int) DataArray[iCurByte]);
						strcat (buf, buf2);
					}
					else
					{
						sprintf (buf2, "\tEND");
						strcat (buf, buf2);
					}
					// Space enough for a word?
					if (DataArray.GetLength ()-iCurByte >= 2)
					{
						// Space enough for a word.
						wordval = (DataArray[iCurByte] << 8) | DataArray[iCurByte+1];
						sprintf (buf2, ",W:%u", (unsigned int) wordval);
						strcat (buf, buf2);
					}
					if (DataArray.GetLength ()-iCurByte >= 4)
					{
						// Space enough for a longword.
						longval = (wordval<<16) | (DataArray[iCurByte+2]<<8) | (DataArray[iCurByte+3]);
						sprintf (buf2, ",L:%u", (unsigned int) longval);
						strcat (buf, buf2);
					}
				}
			}
			else // SIGNED
			{
				if (iBinaryMode == LITTLEENDIAN_MODE)
				{
					// SIGNED LITTLEENDIAN_MODE
					// Decimal value of byte.
					if (DataArray.GetLength ()-iCurByte >= 1)
					{
						sprintf (buf2, "\tSigned: B:%d", (int) (signed char) DataArray[iCurByte]);
						strcat (buf, buf2);
					}
					else
					{
						sprintf (buf2, "\tEND");
						strcat (buf, buf2);
					}
					// Space enough for a word?
					if (DataArray.GetLength ()-iCurByte >= 2)
					{
						// Space enough for a word.
						wordval = (DataArray[iCurByte + 1] << 8) | DataArray[iCurByte];
						sprintf (buf2, ",W:%d", (int) (signed short) wordval);
						strcat (buf, buf2);
					}
					if (DataArray.GetLength ()-iCurByte >= 4)
					{
						// Space enough for a longword.
						longval = wordval | (((DataArray[iCurByte + 3] << 8) | DataArray[iCurByte + 2]) << 16);
						sprintf (buf2, ",L:%d", (signed int) longval);
						strcat (buf, buf2);
					}
				}
				else
				{
					// SIGNED BIGENDIAN_MODE
					// Decimal value of byte.
					if (DataArray.GetLength ()-iCurByte >= 1)
					{
						sprintf (buf2, "\tSigned: B:%d", (signed char) DataArray[iCurByte]);
						strcat (buf, buf2);
					}
					else
					{
						sprintf (buf2, "\tEND");
						strcat (buf, buf2);
					}
					// Space enough for a word.
					if (DataArray.GetLength ()-iCurByte >= 2)
					{
						// Space enough for a longword.
						wordval = (DataArray[iCurByte] << 8) | DataArray[iCurByte+1];
						sprintf (buf2, ",W:%d", (int) (signed short) wordval);
						strcat (buf, buf2);
					}
					if (DataArray.GetLength ()-iCurByte >= 4)
					{
						// Space enough for a longword.
						longval = (wordval<<16) | (DataArray[iCurByte+2]<<8) | (DataArray[iCurByte+3]);
						sprintf (buf2, ",L:%d", (signed int) longval);
						strcat (buf, buf2);
					}
				}
			}
			SendMessage (hwndStatusBar, SB_SETTEXT, 0, (LPARAM) buf);

			// Character set, input mode or read-only, binary mode.
			switch (iCharacterSet)
			{
			case ANSI_FIXED_FONT:
				sprintf (buf, "\tANSI");
				break;

			case OEM_FIXED_FONT:
				sprintf (buf, "\tOEM");
				break;
			}

			if( bReadOnly )
			{
				sprintf (buf2, " / READ");
				strcat (buf, buf2);
			}
			else if( iInsertMode )
			{
				sprintf (buf2, " / INS");
				strcat (buf, buf2);
			}
			else
			{
				sprintf (buf2, " / OVR");
				strcat (buf, buf2);
			}
			if (iBinaryMode == LITTLEENDIAN_MODE)
			{
				sprintf (buf2, " / L"); // Intel
				strcat (buf, buf2);
			}
			else if (iBinaryMode == BIGENDIAN_MODE)
			{
				sprintf (buf2, " / B"); // Motorola
				strcat (buf, buf2);
			}
			SendMessage (hwndStatusBar, SB_SETTEXT, 1, (LPARAM) buf);
			
			// File size.
			sprintf (buf, "\tSize: %u", DataArray.GetLength ());
			SendMessage (hwndStatusBar, SB_SETTEXT, 2, (LPARAM) buf);
		}
	}
	else
	{
		SetWindowText (hwnd, "frhed");
		SendMessage (hwndStatusBar, WM_SETTEXT, 0, (LPARAM) "No file loaded");
	}
}

//--------------------------------------------------------------------------------------------
// Set Caret position.
void HexEditorWindow::set_caret_pos ()
{
	if (bSelected)
	{
		SetCaretPos (-cxChar, -cyChar);
		return;
	}

	int iCaretLine = iCurByte / iBytesPerLine,
		iBottomLine = iCurLine + cyBuffer - 1;

	switch (m_iEnteringMode)	
	{
	case CHARS:
		if (iCaretLine >= iCurLine && iCaretLine <= iBottomLine && filename[0] != '\0')
		{
			int y = iCaretLine - iCurLine,
				x = iOffsetLen+iByteSpace+iBytesPerLine*3+iCharSpace
				- iHscrollPos + (iCurByte%iBytesPerLine);
			SetCaretPos (CLIENT_BORDER_WIDTH + x*cxChar, CLIENT_BORDER_WIDTH + y*cyChar);
		}
		else
			SetCaretPos (-cxChar, -cyChar);
		break;
	case BYTES:
		// If caret in vertical visible area...
		if (iCaretLine >= iCurLine && iCaretLine <= iBottomLine && filename[0] != '\0')
		{
			int y = iCaretLine - iCurLine,
				x = iOffsetLen+iByteSpace + (iCurByte%iBytesPerLine)*3 - iHscrollPos + iCurNibble;
			SetCaretPos (CLIENT_BORDER_WIDTH + x*cxChar, CLIENT_BORDER_WIDTH + y*cyChar);
		}
		else
			SetCaretPos (-cxChar, -cyChar);
	}
}

//--------------------------------------------------------------------------------------------
// Repaints the whole window.
int HexEditorWindow::repaint( int line )
{
	HideCaret( hwnd );
	iUpdateLine = line;
	InvalidateRect( hwnd, NULL, FALSE );
	UpdateWindow( hwnd );
	ShowCaret( hwnd );
	return 0;
}

//--------------------------------------------------------------------------------------------
// Clear everything up.
void HexEditorWindow::clear_all ()
{
	iOffsetLen = 8;
	iByteSpace = 2;
	iBytesPerLine = 16;
	iCharSpace = 1;
	iCharsPerLine = iOffsetLen + iByteSpace + iBytesPerLine*3 + iCharSpace + iBytesPerLine;
	DataArray.ClearAll ();
	filename[0] = '\0';
	iVscrollMax = 0;
	iVscrollPos = 0;
	iVscrollInc = 0;
	iHscrollMax = 0;
	iHscrollPos = 0;
	iHscrollInc = 0;
	iCurLine = 0;
	iCurByte = 0;
	iCurNibble = 0;
}

//--------------------------------------------------------------------------------------------
// Set the vertical scrollbar position.
void HexEditorWindow::adjust_vscrollbar ()
{
	iVscrollPos = (int) ((float)iCurLine * ((float)iVscrollMax)/(float)(iNumlines-1));
	SetScrollPos (hwndVBar, SB_CTL, iVscrollPos, TRUE);
}

//--------------------------------------------------------------------------------------------
// Set the horizontal scrollbar position.
void HexEditorWindow::adjust_hscrollbar ()
{
	SetScrollPos (hwndHBar, SB_CTL, iHscrollPos, TRUE);
}

//--------------------------------------------------------------------------------------------
// Highlight (invert) the character/byte at the current offset.
void HexEditorWindow::mark_char (HDC hdc)
{
	if( bDontMarkCurrentPos )
		return;

	if (bSelected)
	{
		SetCaretPos (-cxChar, -cyChar);
		return;
	}

	int DC_was_allocated = FALSE;
	if (hdc == 0)
	{
		hdc = GetDC (hwnd);
		DC_was_allocated = TRUE;		
	}

	int chpos;
	RECT r;
	switch (m_iEnteringMode)
	{
	case CHARS:
		chpos = iOffsetLen + iByteSpace + (iCurByte%iBytesPerLine)*3 - iHscrollPos;
		r.left = CLIENT_BORDER_WIDTH + chpos * cxChar;
		r.top = CLIENT_BORDER_WIDTH + (iCurByte/iBytesPerLine-iCurLine)*cyChar;
		r.right = CLIENT_BORDER_WIDTH + r.left + 2*cxChar;
		r.bottom = CLIENT_BORDER_WIDTH + (iCurByte/iBytesPerLine-iCurLine+1)*cyChar;
		InvertRect (hdc, &r);
		break;
	case BYTES:
		chpos = iOffsetLen + iByteSpace + iBytesPerLine*3 + iCharSpace
			+ (iCurByte % iBytesPerLine) - iHscrollPos;
		r.left = CLIENT_BORDER_WIDTH + chpos * cxChar;
		r.top = CLIENT_BORDER_WIDTH + (iCurByte/iBytesPerLine-iCurLine)*cyChar;
		r.right = CLIENT_BORDER_WIDTH + (chpos+1)*cxChar;
		r.bottom = CLIENT_BORDER_WIDTH + (iCurByte/iBytesPerLine-iCurLine+1)*cyChar;
		InvertRect (hdc, &r);
	}

	if (DC_was_allocated)
		ReleaseDC (hwnd, hdc);
}

//--------------------------------------------------------------------------------------------
// Repaint one line in the window.
void HexEditorWindow::print_line (HDC hdc, int line, char* linbuf, HBRUSH hbr )
{
	// line = absolute line number.
	// if line not visible:
	if (line < iCurLine || line > iCurLine + cyBuffer)
		return;
	int startpos = line * iBytesPerLine, endpos, i = 0, m;
	char buf[80], c;

	// Return if this line does not even contain the end-of-file double
	// underscore (at index upperbound+1).
	if( startpos > DataArray.GetUpperBound() + 1 )
	{
		return;
	}

	// Write offset.
	sprintf (buf, "%%%d.%dx", iOffsetLen, iOffsetLen);
	for (m=0; m<iByteSpace; m++)
		buf[5+m] = ' ';
	buf[5+m] = '\0';
	sprintf (linbuf, buf, startpos);
	
	// Last line reached? Then only write rest of bytes.
	// startpos+iBytesPerLine-1 = Last byte in current line.
	if (startpos+iBytesPerLine > DataArray.GetLength ())
	{
		// If the first byte of the next line would not be valid, then
		// only print the bytes up to and including the last byte of the file.
		endpos = DataArray.GetUpperBound()+1;
	}
	else
	{
		// Print the bytes up to the end of this line, they are all valid.
		endpos = startpos+iBytesPerLine-1;
	}
	// Could happen on arrow down, so that last line is on bottom of window:
	if( endpos < startpos )
	{
		endpos = startpos;
	}
	// Write bytes.
	m = iOffsetLen+iByteSpace; // Index of first byte in line.
	for (i=startpos; i<=endpos; i++)
	{
		if (i == DataArray.GetLength())
		{
			linbuf[m++] = '_';
			linbuf[m++] = '_';
			linbuf[m++] = ' ';
		}
		else
		{
			c = (DataArray[i] >> 4);
			if( c < 10 )
				c += '0';
			else
				c = c - 10 + 'a';
			linbuf[m++] = c;
			c = (DataArray[i] & 0x0f);
			if( c < 10 )
				c += '0';
			else
				c = c - 10 + 'a';
			linbuf[m++] = c;
			linbuf[m++] = ' ';
		}
	}
	// Write spaces for non-existant bytes.
	if (endpos-startpos < iBytesPerLine-1)
	{
		for (i=0; i<iBytesPerLine-1-(endpos-startpos); i++)
		{
			linbuf[m++] = ' ';
			linbuf[m++] = ' ';
			linbuf[m++] = ' ';
		}
	}

	// Write offset to chars.
	for (i=0; i<iCharSpace; i++)
		linbuf[m++] = ' ';
	
	// Write ASCIIs.
	for (i=startpos; i<=endpos; i++)
	{
		if (i == DataArray.GetLength())
		{
			linbuf[m++] = ' ';
		}
		else if (iCharacterSet == OEM_FIXED_FONT && DataArray[i]!=0)
		{
			linbuf[m++] = DataArray[i];
		}
		else if ((DataArray[i]>=32 && DataArray[i]<=126) || (DataArray[i]>=160 && DataArray[i]<=255) || (DataArray[i]>=145 && DataArray[i]<=146))
		{
			linbuf[m++] = DataArray[i];
		}
		else
		{
			linbuf[m++] = '.';
		}
	}

	// Write spaces for nonexisting chars.
	if (endpos-startpos < iBytesPerLine-1)
		for (i=0; i<iBytesPerLine-1-(endpos-startpos); i++)
			linbuf[m++] = ' ';
	
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	int line_len = m; // Length of the line in chars.

	// Set normal text colors.
	iBkColor = PALETTERGB (GetRValue(iBkColorValue),GetGValue(iBkColorValue),GetBValue(iBkColorValue));
	iTextColor = PALETTERGB (GetRValue(iTextColorValue),GetGValue(iTextColorValue),GetBValue(iTextColorValue));
	SetTextColor (hdc, iTextColor);
	SetBkColor (hdc, iBkColor);

	// How much of offset and byte-space is visible? Print it in normal text colors.
	if( iHscrollPos < iOffsetLen + iByteSpace )
	{
		// A part of offset+byte-space is visible.
		// Write offset to screen.
		TextOut( hdc, CLIENT_BORDER_WIDTH, CLIENT_BORDER_WIDTH + ( line - iCurLine ) * cyChar, linbuf + iHscrollPos, iOffsetLen + iByteSpace - iHscrollPos );
	}

	int iHexStart = iOffsetLen + iByteSpace;
	int iHexXStart = iHexStart * cxChar;

	// Write char-space, if it is visible.
	if( iHscrollPos < iOffsetLen + iByteSpace + iBytesPerLine * iHexWidth )
	{
		// Char-space is visible.
		TextOut( hdc,
			CLIENT_BORDER_WIDTH + ( iHexStart + iBytesPerLine * iHexWidth - iHscrollPos ) * cxChar,
			CLIENT_BORDER_WIDTH + ( line - iCurLine ) * cyChar,
			linbuf + iHexStart + iBytesPerLine * iHexWidth,
			iCharSpace );
	}

	iSelBkColor = PALETTERGB (GetRValue(iSelBkColorValue),GetGValue(iSelBkColorValue),GetBValue(iSelBkColorValue));
	iSelTextColor = PALETTERGB (GetRValue(iSelTextColorValue),GetGValue(iSelTextColorValue),GetBValue(iSelTextColorValue));
	BOOL last_normtext = TRUE;

	int p, el = startpos + iBytesPerLine - 1 - 1, s, e;
	for( p = startpos; p <= el; p++ )
	{
		// Write hex, if it is visible.
		// s = Position in string of last character of current hex.
		s = iOffsetLen + iByteSpace + ( p - startpos + 1 ) * iHexWidth;
		e = s - iHexWidth;
		// Print only if at least a part of the hex is visible.
		if( iHscrollPos < s && iHscrollPos + cxBuffer >= e )
		{
			// Selected bytes must be printed in selection colors.
			if( bSelected && IN_BOUNDS( p, iStartOfSelection, iEndOfSelection ) )
			{
				if( last_normtext )
				{
					// Set selection colors.
					SetTextColor (hdc, iSelTextColor);
					SetBkColor (hdc, iSelBkColor);
					last_normtext = FALSE;
				}
			}
			else
			{
				if( !last_normtext )
				{
					// Set normal text colors.
					SetTextColor (hdc, iTextColor);
					SetBkColor (hdc, iBkColor);
					last_normtext = TRUE;
				}
			}
			// Hex is visible.
			TextOut( hdc,
				CLIENT_BORDER_WIDTH + ( iHexStart + ( p - startpos ) * iHexWidth - iHscrollPos ) * cxChar,
				CLIENT_BORDER_WIDTH + ( line - iCurLine ) * cyChar,
				linbuf + iHexStart + ( p - startpos ) * iHexWidth,
				iHexWidth );
		}

		s = iOffsetLen + iByteSpace + iBytesPerLine * iHexWidth + iCharSpace + ( p - startpos + 1);
		// Write char, if it is visible.
		if( iHscrollPos < s && iHscrollPos + cxBuffer >= s - 1 )
		{
			// Selected bytes must be printed in selection colors.
			if( bSelected && IN_BOUNDS( p, iStartOfSelection, iEndOfSelection ) )
			{
				if( last_normtext )
				{
					// Set selection colors.
					SetTextColor (hdc, iSelTextColor);
					SetBkColor (hdc, iSelBkColor);
					last_normtext = FALSE;
				}
			}
			else
			{
				if( !last_normtext )
				{
					// Set normal text colors.
					SetTextColor (hdc, iTextColor);
					SetBkColor (hdc, iBkColor);
					last_normtext = TRUE;
				}
			}
			// Char is visible.
			TextOut( hdc,
				CLIENT_BORDER_WIDTH + ( iHexStart + iBytesPerLine * iHexWidth + iCharSpace + ( p - startpos ) - iHscrollPos ) * cxChar,
				CLIENT_BORDER_WIDTH + ( line - iCurLine ) * cyChar,
				linbuf + iHexStart + iBytesPerLine * iHexWidth + iCharSpace + ( p - startpos ),
				1 );
		}
	}
	
	// The last hex in the line is not completely in selection colors. It's
	// succeding space must be printed in normal text colors (visually more
	// appealing).
	// Write hex, if it is visible.
	// s = Position in string of last character of current hex.
	s = iOffsetLen + iByteSpace + ( p - startpos + 1 ) * iHexWidth;
	e = s - iHexWidth;
	// Print only if at least a part of the hex is visible.
	if( iHscrollPos < s && iHscrollPos + cxBuffer >= e )
	{
		// Selected bytes must be printed in selection colors.
		if( bSelected && IN_BOUNDS( p, iStartOfSelection, iEndOfSelection ) )
		{
			if( last_normtext )
			{
				// Output the last space first.
				TextOut( hdc,
					CLIENT_BORDER_WIDTH + ( iHexStart + ( p - startpos ) * iHexWidth - iHscrollPos ) * cxChar + 2 * cxChar,
					CLIENT_BORDER_WIDTH + ( line - iCurLine ) * cyChar,
					" ",
					1 );
				// Set selection colors.
				SetTextColor (hdc, iSelTextColor);
				SetBkColor (hdc, iSelBkColor);
				last_normtext = FALSE;
				last_normtext = TRUE;
				// Write hex.
				TextOut( hdc,
					CLIENT_BORDER_WIDTH + ( iHexStart + ( p - startpos ) * iHexWidth - iHscrollPos ) * cxChar,
					CLIENT_BORDER_WIDTH + ( line - iCurLine ) * cyChar,
					linbuf + iHexStart + ( p - startpos ) * iHexWidth,
					iHexWidth - 1 );
			}
			else
			{
				// Write hex.
				TextOut( hdc,
					CLIENT_BORDER_WIDTH + ( iHexStart + ( p - startpos ) * iHexWidth - iHscrollPos ) * cxChar,
					CLIENT_BORDER_WIDTH + ( line - iCurLine ) * cyChar,
					linbuf + iHexStart + ( p - startpos ) * iHexWidth,
					iHexWidth - 1 );
				// Set normal text colors.
				SetTextColor (hdc, iTextColor);
				SetBkColor (hdc, iBkColor);
				last_normtext = TRUE;
				// Output the last space.
				TextOut( hdc,
					CLIENT_BORDER_WIDTH + ( iHexStart + ( p - startpos ) * iHexWidth - iHscrollPos ) * cxChar + 2 * cxChar,
					CLIENT_BORDER_WIDTH + ( line - iCurLine ) * cyChar,
					" ",
					1 );
			}
		}
		else
		{
			// Non-selected hex.
			if( !last_normtext )
			{
				// Set normal text colors.
				SetTextColor (hdc, iTextColor);
				SetBkColor (hdc, iBkColor);
				last_normtext = TRUE;
			}
			// Hex is visible.
			TextOut( hdc,
				CLIENT_BORDER_WIDTH + ( iHexStart + ( p - startpos ) * iHexWidth - iHscrollPos ) * cxChar,
				CLIENT_BORDER_WIDTH + ( line - iCurLine ) * cyChar,
				linbuf + iHexStart + ( p - startpos ) * iHexWidth,
				iHexWidth );
		}
	}

	s = iOffsetLen + iByteSpace + iBytesPerLine * iHexWidth + iCharSpace + ( p - startpos + 1);
	// Write char, if it is visible.
	if( iHscrollPos < s && iHscrollPos + cxBuffer >= s - 1 )
	{
		// Selected bytes must be printed in selection colors.
		if( bSelected && IN_BOUNDS( p, iStartOfSelection, iEndOfSelection ) )
		{
			if( last_normtext )
			{
				// Set selection colors.
				SetTextColor (hdc, iSelTextColor);
				SetBkColor (hdc, iSelBkColor);
			}
		}
		else
		{
			if( !last_normtext )
			{
				// Set normal text colors.
				SetTextColor (hdc, iTextColor);
				SetBkColor (hdc, iBkColor);
			}
		}
		// Char is visible.
		TextOut( hdc,
			CLIENT_BORDER_WIDTH + ( iHexStart + iBytesPerLine * iHexWidth + iCharSpace + ( p - startpos ) - iHscrollPos ) * cxChar,
			CLIENT_BORDER_WIDTH + ( line - iCurLine ) * cyChar,
			linbuf + iHexStart + iBytesPerLine * iHexWidth + iCharSpace + ( p - startpos ),
			1 );
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Separators.
	for (i = 0; i < (iBytesPerLine / 4) + 1; i++)
	{
		m = (iOffsetLen+iByteSpace)*cxChar - cxChar/2 + 3*cxChar*4*i - cxChar*iHscrollPos;
		MoveToEx (hdc, CLIENT_BORDER_WIDTH+m, CLIENT_BORDER_WIDTH+(line-iCurLine)*cyChar, NULL);
		LineTo (hdc, CLIENT_BORDER_WIDTH+m, CLIENT_BORDER_WIDTH+(line-iCurLine+1)*cyChar);
	}
	// Separator for chars.
	m = CHARSTART*cxChar - cxChar*iHscrollPos - 2;
	MoveToEx (hdc, CLIENT_BORDER_WIDTH+m, CLIENT_BORDER_WIDTH+(line-iCurLine)*cyChar, NULL);
	LineTo (hdc, CLIENT_BORDER_WIDTH+m, CLIENT_BORDER_WIDTH+(line-iCurLine+1)*cyChar);
	// Second separator.
	MoveToEx (hdc, CLIENT_BORDER_WIDTH+m+2, CLIENT_BORDER_WIDTH+(line-iCurLine)*cyChar, NULL);
	LineTo (hdc, CLIENT_BORDER_WIDTH+m+2, CLIENT_BORDER_WIDTH+(line-iCurLine+1)*cyChar);

	// Print bookmark indicators.
	// Are there bookmarks in this line?
	el = startpos + iBytesPerLine - 1;
	int chpos;
	RECT r;
	// Brush for bookmark borders.
	for( i = 0; i < iBmkCount; i++ )
	{
		// Print the bookmark if it is within the file.
		if( IN_BOUNDS( pbmkList[i].offset, startpos, el ) && pbmkList[i].offset <= DataArray.GetUpperBound() )
		{
			// Found a bookmark in this line.
			// Mark hex.
			chpos = iOffsetLen + iByteSpace + ( pbmkList[i].offset % iBytesPerLine )*3 - iHscrollPos;
			r.left = CLIENT_BORDER_WIDTH + chpos * cxChar;
			r.top = CLIENT_BORDER_WIDTH + ( pbmkList[i].offset / iBytesPerLine - iCurLine ) * cyChar;
			r.right = CLIENT_BORDER_WIDTH + r.left + 2*cxChar;
			r.bottom = CLIENT_BORDER_WIDTH + ( pbmkList[i].offset / iBytesPerLine - iCurLine + 1 ) * cyChar;
			FrameRect( hdc, &r, hbr );

			// Mark char.
			chpos = iOffsetLen + iByteSpace + iBytesPerLine*3 + iCharSpace
				+ ( pbmkList[i].offset % iBytesPerLine ) - iHscrollPos;
			r.left = CLIENT_BORDER_WIDTH + chpos * cxChar;
			r.top = CLIENT_BORDER_WIDTH + ( pbmkList[i].offset / iBytesPerLine - iCurLine) * cyChar;
			r.right = CLIENT_BORDER_WIDTH + ( chpos + 1 ) * cxChar;
			r.bottom = CLIENT_BORDER_WIDTH + ( pbmkList[i].offset / iBytesPerLine - iCurLine + 1 ) * cyChar;
			FrameRect( hdc, &r, hbr );
		}
	}
	return;
}

//--------------------------------------------------------------------------------------------
// Dialogue procedure for Go-to dialogue.
BOOL CALLBACK GoToDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{	
	switch (iMsg)
	{
	case WM_INITDIALOG:
		if( iGotoDlgBufLen == 0 )
		{
			// Maximal length of text in Edit-Control:
			iGotoDlgBufLen = SendMessage( GetDlgItem( hDlg, IDC_EDIT1 ), EM_GETLIMITTEXT, 0, 0 );
			// Memory for returning text to caller.
			pcGotoDlgBuffer = new char[iGotoDlgBufLen];
			if( pcGotoDlgBuffer == NULL )
				MessageBox( NULL, "Could not allocate Goto buffer.", "Go to error", MB_OK | MB_ICONERROR );
			memset( pcGotoDlgBuffer, 0, iGotoDlgBufLen );
		}
		if( pcGotoDlgBuffer != NULL )
			SetWindowText( GetDlgItem (hDlg, IDC_EDIT1), pcGotoDlgBuffer );
		SetFocus (GetDlgItem (hDlg, IDC_EDIT1));
		return FALSE;

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			// Copy text in Edit-Control.
			EndDialog( hDlg, GetDlgItemText( hDlg, IDC_EDIT1, pcGotoDlgBuffer, iGotoDlgBufLen ) );
			return TRUE;

		case IDCANCEL:
			EndDialog( hDlg, 0 );
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//--------------------------------------------------------------------------------------------
// Dialogue procedure for Find dialogue.
BOOL CALLBACK FindDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{	
	switch (iMsg)
	{
	case WM_INITDIALOG:
		// Is the buffer containing the data to find from the last find command empty?
		if (iFindDlgBufLen == 0)
		{
			// Maximal Length of text in Edit-Control:
			iFindDlgBufLen = SendMessage (GetDlgItem (hDlg, IDC_EDIT1), EM_GETLIMITTEXT, 0, 0);
			// Memory for returning text to caller.
			pcFindDlgBuffer = new char[iFindDlgBufLen];
		}
		SetFocus (GetDlgItem (hDlg, IDC_EDIT1));
		if (iFindDlgLastLen != 0)
			SetWindowText (GetDlgItem (hDlg, IDC_EDIT1), pcFindDlgBuffer);
		CheckDlgButton (hDlg, IDC_RADIO2, BST_CHECKED);
		return FALSE;

	case WM_COMMAND:
		iFindDlgMatchCase = IsDlgButtonChecked (hDlg, IDC_CHECK1);
		if (IsDlgButtonChecked (hDlg, IDC_RADIO1) == BST_CHECKED)
			iFindDlgDirection = -1;
		else
			iFindDlgDirection = 1;

		switch (LOWORD (wParam))
		{
		case IDOK:
			// Copy text in Edit-Control. Return the number of characters
			// in the Edit-control minus the zero byte at the end.
			EndDialog (hDlg, GetDlgItemText (hDlg, IDC_EDIT1, pcFindDlgBuffer, iFindDlgBufLen));
			return TRUE;
		case IDCANCEL:
			EndDialog (hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//--------------------------------------------------------------------------------------------
// Set horizontal scroll position so that caret is visible.
void HexEditorWindow::adjust_view_for_caret ()
{
	int log_column;
	if (m_iEnteringMode == BYTES)
		log_column = BYTES_LOGICAL_COLUMN;
	else
		log_column = CHARS_LOGICAL_COLUMN;

	if (log_column >= iHscrollPos+cxBuffer)
		iHscrollPos = log_column-(cxBuffer-1);
	else if (log_column < iHscrollPos)
		iHscrollPos = log_column;
	adjust_hscrollbar ();
}

//--------------------------------------------------------------------------------------------
// Initialize main menu items.
int HexEditorWindow::initmenupopup( WPARAM w, LPARAM l )
{
	// Submenu "File":
	if( l == 0 )
	{
//Pabs changed - line insert
		//Revert is allowed when the file has been saved, changed and is not in partial open mode
		EnableMenuItem( (HMENU) w, IDM_REVERT, ((!bFileNeverSaved && m_iFileChanged && !bPartialOpen)?MF_ENABLED:MF_GRAYED));
		//Save selection as is allowed when selection is going on
		EnableMenuItem( (HMENU) w, IDM_SAVESELAS, (bSelected?MF_ENABLED:MF_GRAYED) );
		//Delete file is allowed when the file has been saved and read only mode is off
		EnableMenuItem( (HMENU) w, IDM_DELETEFILE, ((!bFileNeverSaved && !bReadOnly)?MF_ENABLED:MF_GRAYED) );
		//Insert file is allowed when read only mode is off
		// RK: ...and there is no selection going on.
		EnableMenuItem( (HMENU) w, IDM_INSERTFILE, ((!bReadOnly && !bSelected)?MF_ENABLED:MF_GRAYED));
//end
		// "New file" is always possible.
		EnableMenuItem( (HMENU) w, IDM_NEW, MF_ENABLED );

		// "Open" and "Open Partially" are always allowed.
		EnableMenuItem( (HMENU) w, IDM_OPEN, MF_ENABLED );
		EnableMenuItem( (HMENU) w, IDM_PARTIAL_OPEN, MF_ENABLED );

		// "Save" is only possible if the current file has a name.
		if( !bFileNeverSaved )
			EnableMenuItem( (HMENU) w, IDM_SAVE, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_SAVE, MF_GRAYED );

		// "Save as" is always possible.
		EnableMenuItem( (HMENU) w, IDM_SAVE_AS, MF_ENABLED );

		// Hexdump is only possible if the file isn't empty.
		if( DataArray.GetLength() > 0 )
			EnableMenuItem( (HMENU) w, IDM_COPY_HEXDUMP, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_COPY_HEXDUMP, MF_GRAYED );

		// "Exit" is always allowed.
		EnableMenuItem( (HMENU) w, IDM_EXIT, MF_ENABLED );

		// Create the MRU list.
		make_MRU_list( (HMENU) w );
	}
	// Submenu "Edit":
	else if( l == 1 )
	{
//Pabs changed - line insert
		//"Fill with" is allowed if read-only is disabled, there is selection
		//going on and the file is not in partial-open mode
		EnableMenuItem( (HMENU) w, IDM_FILL_WITH, ((!bReadOnly && bSelected && !bPartialOpen)?MF_ENABLED:MF_GRAYED) );
//end
		// "Cut" is allowed if there is a selection or the caret is on a byte.
		// It is not allowed in read-only and partial-open mode.
		if( ( bSelected || iCurByte <= DataArray.GetUpperBound() ) && !bReadOnly && !bPartialOpen )
			EnableMenuItem( (HMENU) w, IDM_EDIT_CUT, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_EDIT_CUT, MF_GRAYED );

		// "Copy" is allowed if there is a selection or the caret is on a byte.
		if( bSelected || iCurByte <= DataArray.GetUpperBound() )
			EnableMenuItem( (HMENU) w, IDM_EDIT_COPY, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_EDIT_COPY, MF_GRAYED );

		// "Paste" is allowed if the clipboard contains text,
		// there is no selection going on and read-only is disabled.
		if( !bReadOnly && !bSelected )
		{
			if( OpenClipboard( NULL ) )
			{
				HGLOBAL hClipMemory = GetClipboardData( CF_TEXT );
				if( hClipMemory != NULL )
					EnableMenuItem( (HMENU) w, IDM_EDIT_PASTE, MF_ENABLED );
				else
					EnableMenuItem( (HMENU) w, IDM_EDIT_PASTE, MF_GRAYED );
				CloseClipboard ();
			}
			else
			{
				// Clipboard could not be opened => can't paste.
				EnableMenuItem( (HMENU) w, IDM_EDIT_PASTE, MF_GRAYED );
			}
		}
		else
			EnableMenuItem( (HMENU) w, IDM_EDIT_PASTE, MF_GRAYED );

		// "Paste with dialogue" is allowed if read-only is disabled and.
		// there is no selection going on.
		if( !bReadOnly && !bSelected )
			EnableMenuItem( (HMENU) w, IDM_PASTE_WITH_DLG, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_PASTE_WITH_DLG, MF_GRAYED );

		// "Append" is allowed if read-only is disabled and file is not
		// in partial-open mode and there is no selection going on.
		if( !bReadOnly && !bPartialOpen && !bSelected )
			EnableMenuItem( (HMENU) w, IDM_EDIT_APPEND, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_EDIT_APPEND, MF_GRAYED );

		// "Delete" is allowed if there is a selection or the caret is on a byte.
		// It is not allowed in read-only and partial-open mode.
		if( ( bSelected || iCurByte <= DataArray.GetUpperBound() ) && !bReadOnly && !bPartialOpen )
			EnableMenuItem( (HMENU) w, IDA_DELETEKEY, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDA_DELETEKEY, MF_GRAYED );
		
		// "Select All" is allowed if file is not empty.
		if( DataArray.GetLength() > 0 )
			EnableMenuItem( (HMENU) w, IDM_SELECT_ALL, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_SELECT_ALL, MF_GRAYED );

		// "Select block" is allowed if file is not empty.
		if( DataArray.GetLength() > 0 )
			EnableMenuItem( (HMENU) w, IDM_SELECT_BLOCK, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_SELECT_BLOCK, MF_GRAYED );

		// "Change editing mode" is always allowed.
		EnableMenuItem( (HMENU) w, IDM_CHANGE_MODE, MF_ENABLED );

		// "Toggle entering mode" is allowed if read-only is disabled.
		if( !bReadOnly )
			EnableMenuItem( (HMENU) w, IDA_INSERTMODETOGGLE, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDA_INSERTMODETOGGLE, MF_GRAYED );

		// "Read-only mode" is always allowed.
		EnableMenuItem( (HMENU) w, IDM_EDIT_READONLYMODE, MF_ENABLED );
		// Check or uncheck this item.
		if( bReadOnly )
			CheckMenuItem( (HMENU) w, IDM_EDIT_READONLYMODE, MF_CHECKED );
		else
			CheckMenuItem( (HMENU) w, IDM_EDIT_READONLYMODE, MF_UNCHECKED );

		// "Find" is allowed if the file is not empty.
		if( DataArray.GetLength() > 0 )
			EnableMenuItem( (HMENU) w, IDM_FIND, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_FIND, MF_GRAYED );

		// "Replace" is allowed if the file is not empty and read-only is disabled.
		if( DataArray.GetLength() > 0 && !bReadOnly )
			EnableMenuItem( (HMENU) w, IDM_REPLACE, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_REPLACE, MF_GRAYED );

		// "Find next" is allowed if the file is not empty,
		// and there is a findstring OR there is a selection
		// (which will be searched for).
		if( DataArray.GetLength() > 0 && ( pcFindDlgBuffer != NULL || bSelected ) )
			EnableMenuItem( (HMENU) w, IDM_FINDNEXT, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_FINDNEXT, MF_GRAYED );

		// "Find previous" is allowed if the file is not empty,
		// and there is a findstring OR there is a selection
		// (which will be searched for).
		if( DataArray.GetLength() > 0 && ( pcFindDlgBuffer != NULL || bSelected ) )
			EnableMenuItem( (HMENU) w, IDM_FINDPREV, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_FINDPREV, MF_GRAYED );

		// "Go to" is allowed if the file isn't empty.
		if( DataArray.GetLength() > 0 )
			EnableMenuItem( (HMENU) w, IDM_GO_TO, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_GO_TO, MF_GRAYED );

		// "Enter decimal value" is allowed if read-only is disabled, the file is not empty,
		// the caret is on a byte and there is no selection going on.
		if( !bReadOnly && DataArray.GetLength() > 0 && iCurByte <= DataArray.GetUpperBound() && !bSelected )
			EnableMenuItem( (HMENU) w, IDM_EDIT_ENTERDECIMALVALUE, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_EDIT_ENTERDECIMALVALUE, MF_GRAYED );

		// "Manipulate bits" is allowed if the caret is on a byte, read-only is disabled
		// and there is no selection going on.
		if( !bReadOnly && iCurByte <= DataArray.GetUpperBound() && !bSelected )
			EnableMenuItem( (HMENU) w, IDM_EDIT_MANIPULATEBITS, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_EDIT_MANIPULATEBITS, MF_GRAYED );

		// "Compare from current offset" is allowed if the caret is on a byte
		// and there is no selection going on.
		if( iCurByte <= DataArray.GetUpperBound() && !bSelected )
			EnableMenuItem( (HMENU) w, IDM_COMPARE, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_COMPARE, MF_GRAYED );

		// "Get floating point value" is allowed if the caret is on a byte
		// and there is no selection going on.
		if( iCurByte <= DataArray.GetUpperBound() && !bSelected )
			EnableMenuItem( (HMENU) w, IDM_READFLOAT, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_READFLOAT, MF_GRAYED );

		// "File properties" is always allowed.
		EnableMenuItem( (HMENU) w, IDM_PROPERTIES, MF_ENABLED );

		// "Apply template" is allowed if the caret is on a byte
		// and there is no selection going on.
		if( iCurByte <= DataArray.GetUpperBound() && !bSelected )
			EnableMenuItem( (HMENU) w, IDM_APPLYTEMPLATE, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_APPLYTEMPLATE, MF_GRAYED );
		
		// "Open in text editor" is allowed if file has been saved before.
		if( !bFileNeverSaved )
			EnableMenuItem( (HMENU) w, IDM_OPEN_TEXT, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_OPEN_TEXT, MF_GRAYED );
	}
	// Submenu "View":
	else if( l == 2 )
	{
		// These items are always enabled.
		EnableMenuItem( (HMENU) w, IDM_TOP, MF_ENABLED );
		EnableMenuItem( (HMENU) w, IDM_BOTTOM, MF_ENABLED );
		EnableMenuItem( (HMENU) w, IDM_SCROLL_LEFT, MF_ENABLED );
		EnableMenuItem( (HMENU) w, IDM_SCROLL_RIGHT, MF_ENABLED );
		EnableMenuItem( (HMENU) w, IDM_SCROLL_UP, MF_ENABLED );
		EnableMenuItem( (HMENU) w, IDM_SCROLL_DOWN, MF_ENABLED );
		
	}
	// Submenu "Options":
	else if( l == 3 )
	{
		// These items are always enabled.
		EnableMenuItem( (HMENU) w, IDM_VIEW_SETTINGS, MF_ENABLED );
		EnableMenuItem( (HMENU) w, IDM_TEXT_COLOR, MF_ENABLED );
		EnableMenuItem( (HMENU) w, IDM_BK_COLOR, MF_ENABLED );
		EnableMenuItem( (HMENU) w, IDM_SEP_COLOR, MF_ENABLED );
		EnableMenuItem( (HMENU) w, IDM_SELTEXT_COLOR, MF_ENABLED );
		EnableMenuItem( (HMENU) w, IDM_SELBACK_COLOR, MF_ENABLED );
		EnableMenuItem( (HMENU) w, IDM_BMK_COLOR, MF_ENABLED );
		EnableMenuItem( (HMENU) w, IDM_RESET_COLORS, MF_ENABLED );
		EnableMenuItem( (HMENU) w, IDM_CHARACTER_SET, MF_ENABLED );
		EnableMenuItem( (HMENU) w, IDM_BINARYMODE, MF_ENABLED );
	}
	// Submenu "Bookmarks":
	else if( l == 4 )
	{
		// "Add bookmark" is allowed if the file is not
		// empty and there is no selection going on.
		if( !bSelected && DataArray.GetLength() > 0 )
			EnableMenuItem( (HMENU) w, IDM_ADDBOOKMARK, MF_ENABLED );
		else
			EnableMenuItem( (HMENU) w, IDM_ADDBOOKMARK, MF_GRAYED );

		// "Remove bookmark" and "Clear all bookmarks" are allowed if there are bookmarks set.
		if( iBmkCount > 0 )
		{
			EnableMenuItem( (HMENU) w, IDM_REMOVE_BKM, MF_ENABLED );
			EnableMenuItem( (HMENU) w, IDM_CLEARALL_BMK, MF_ENABLED );
		}
		else
		{
			EnableMenuItem( (HMENU) w, IDM_REMOVE_BKM, MF_GRAYED );
			EnableMenuItem( (HMENU) w, IDM_CLEARALL_BMK, MF_GRAYED );
		}
		// Create the bookmark list.
		make_bookmark_list( (HMENU) w );
	}
	// Submenu "Help":
	else if( l == 5 )
	{
		// These items are always enabled.
		EnableMenuItem( (HMENU) w, IDM_HELP_TOPICS, MF_ENABLED );
		EnableMenuItem( (HMENU) w, IDM_ABOUT, MF_ENABLED );
	}
	return 0;
}

//--------------------------------------------------------------------------------------------
// Handler on window closing.
int HexEditorWindow::close ()
{
	if( m_iFileChanged == TRUE )
	{
		if( MessageBox (hwnd, "File was changed! Exit anyway?", "Exit", MB_YESNO | MB_ICONQUESTION) == IDNO )
			return 0;
	}
	else
	{
		// If help was open close it.
		char path[500];
		strcpy (path, _pgmptr);
		int len = strlen (path);
		path[len-3] = 'h';
		path[len-2] = 'l';
		path[len-1] = 'p';
		WinHelp (hwnd, path, HELP_QUIT, 0);
	}

	// Store window position for next startup.
	WINDOWPLACEMENT wndpl;
	wndpl.length = sizeof( WINDOWPLACEMENT );
	GetWindowPlacement( hwnd, &wndpl );
	iWindowShowCmd = wndpl.showCmd;
	iWindowX = wndpl.rcNormalPosition.left;
	iWindowY = wndpl.rcNormalPosition.top;
	iWindowWidth = wndpl.rcNormalPosition.right - iWindowX;
	iWindowHeight = wndpl.rcNormalPosition.bottom - iWindowY;
	save_ini_data();

	// Destroy window.
	DestroyWindow( hwnd );
	return 0;
}

//--------------------------------------------------------------------------------------------
BOOL CALLBACK AboutDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			// Set the version information.
			SetWindowText( GetDlgItem( hDlg, IDC_STATIC1 ),
				"frhed - free hex editor for 32-bit Windows\nVersion 1.0."
				SUB_RELEASE_NO "\n(c) Raihan Kibria 2000"
				"\nFill with by Pabs Dec 1999");
			// Set the email-addresses.
			SetWindowText( GetDlgItem( hDlg, IDC_EDIT1 ),
				"rkibria@hrz1.hrz.tu-darmstadt.de"
				"\r\nPabs: pabs3@hotmail.com");
			// Set the homepage URL.
			SetWindowText( GetDlgItem( hDlg, IDC_EDIT2 ), "http://www.tu-darmstadt.de/~rkibria" );
			return TRUE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
		case IDCANCEL:
			EndDialog (hDlg, 0);
			return TRUE;
		
		case IDC_BUTTON1:
			{
				HINSTANCE hi = ShellExecute( hMainWnd, "open", BrowserName, "http://www.tu-darmstadt.de/~rkibria", NULL, SW_SHOWNORMAL );
				if( (int) hi <= 32 )
					MessageBox( hMainWnd, "Could not call browser.", "Go to homepage", MB_OK | MB_ICONERROR );
			}
		}
		break;
	}
	return FALSE;
}

//--------------------------------------------------------------------------------------------
// FIND_BYTES
// Arguments:
// ps   = Start position.
// ls   = Length of src array.
// pb   = Start of searchstring.
// lb   = Length searchstring.
// mode = -1 : backwards search.
//         1 : forward search.
// cmp  = pointer to function that is applied to data before comparing.
//
// Return:
// Position of found string or -1 if not there.

int find_bytes (char* ps, int ls, char* pb, int lb, int mode, char (*cmp) (char))
{
	int start, inc, end, i;
	if (mode == 1)
	{
		start = 0;
		inc = 1;
		end = ls - lb + 1;
	}
	else
	{
		start = ls - lb;
		inc = -1;
		end = 1;
	}

	for (; mode * start < end; start += inc)
	{
		for (i = start; i < start + lb; i++)
		{
			if (cmp (ps[i]) != cmp (pb[i - start]))
				break;
		}
		if (i == start + lb)
			return start;
	}

	return -1;
}

//--------------------------------------------------------------------------------------------
// WM_LBUTTONUP handler.
int HexEditorWindow::lbuttonup (int xPos, int yPos)
{
	// Kill timer.
	KillTimer (hwnd, TIMERID);
	bTimerSet = FALSE;
	// Reset cursor from I-beam to arrow.
	SetCursor( LoadCursor( NULL, IDC_ARROW ) );
	// Release mouse.
	ReleaseCapture ();
	bLButtonIsDown = FALSE;
	// If button released where it was pressed (set caret).
	if (iLBDownX == xPos && iLBDownY == yPos)
	{
		int scr_column = xPos / cxChar, // Column on the screen.
			scr_row = yPos / cyChar,	// Line on the screen.
			log_column = scr_column + iHscrollPos,	// Logical column.
			line = iCurLine + scr_row; // Absolute line number.

		// If click on bytes: set caret there.
		if (log_column >= iOffsetLen + iByteSpace && log_column <= iCharsPerLine - 1 - iBytesPerLine - iCharSpace - 1)
		{
			// If not clicked on empty place:
			if ((log_column-iOffsetLen-(iByteSpace-1))%3 != 0)
			{
				int bytenum = (log_column-iOffsetLen-(iByteSpace-1))/3,
					nibblenum = ((log_column-iOffsetLen-(iByteSpace-1))%3)-1;
				int newpos = line * iBytesPerLine + bytenum;
				if( newpos <= LASTBYTE + 1 )
				{
					iCurByte = newpos;
					iCurNibble = nibblenum;
					m_iEnteringMode = BYTES;
					repaint ();
				}
			}
		}
		// If click on chars: set caret there.
		else if (log_column > iCharsPerLine - 1 - iBytesPerLine	&& log_column < iCharsPerLine)
		{
			int newpos = line * iBytesPerLine + (log_column - (iCharsPerLine - iBytesPerLine));
			if( newpos <= LASTBYTE + 1 )
			{
				iCurByte = newpos;
				m_iEnteringMode = CHARS;
				repaint ();
			}
		}
	}
	return 0;
}

//--------------------------------------------------------------------------------------------
// WM_MOUSEMOVE handler.
int HexEditorWindow::mousemove (int xPos, int yPos)
{
	iMouseX = xPos;
	iMouseY = yPos;
	// If not selecting at the moment.
	if (bLButtonIsDown == FALSE)
		return 0;

	bSelected = TRUE;
	// Capture mouse.
	if (GetCapture() == NULL)
	{
		SetCapture (hwnd);
		SetCursor( LoadCursor( NULL, IDC_IBEAM ) );
	}

	int scr_column = xPos / cxChar,
		scr_row = yPos / cyChar,
		log_column = scr_column + iHscrollPos,
		line = iCurLine + scr_row;

	// Set end of selection according to cursor pos.
	// Scroll if cursor above or under window.
	if (yPos>cyBuffer*cyChar || yPos<0)
	{
		if (bTimerSet==FALSE)
		{
			// Set timer for scrolling.
			SetTimer (hwnd, TIMERID, TIMERDURATION, NULL);
			bTimerSet = TRUE;
		}
	}
	// If cursor on bytes.
	else if (log_column >= iOffsetLen + iByteSpace && log_column <= iCharsPerLine - 1 - iBytesPerLine - iCharSpace - 1)
	{
		KillTimer (hwnd, TIMERID);
		bTimerSet = FALSE;
		// If not clicked on empty place.
		if ((log_column-iOffsetLen-(iByteSpace-1))%3 != 0)
		{
			int bytenum = (log_column-iOffsetLen-(iByteSpace-1))/3,
				nibblenum = ((log_column-iOffsetLen-(iByteSpace-1))%3)-1;
			int newpos = line * iBytesPerLine + bytenum;
			if (newpos <= LASTBYTE)
			{
				iEndOfSelection = newpos;
				repaint ();
			}
		}
	}
	// If cursor on chars.
	else if (log_column > iCharsPerLine - 1 - iBytesPerLine	&& log_column < iCharsPerLine)
	{
		KillTimer (hwnd, TIMERID);
		bTimerSet = FALSE;
		int newpos = line * iBytesPerLine + (log_column - (iCharsPerLine - iBytesPerLine));
		if (newpos <= LASTBYTE)
		{
			iEndOfSelection = newpos;
			repaint ();
		}
	}
	return 0;
}

//--------------------------------------------------------------------------------------------
// WM_LBUTTONDOWN handler.
int HexEditorWindow::lbuttondown (int xPos, int yPos)
{
	if (NO_FILE)
		return 0;

	int scr_column = xPos / cxChar,
		scr_row = yPos / cyChar,
		log_column = scr_column + iHscrollPos,
		line = iCurLine + scr_row;
	iLBDownX = xPos;
	iLBDownY = yPos;

	// Set start of selection.
	// Click on bytes.
	if (log_column >= iOffsetLen + iByteSpace && log_column <= iCharsPerLine - 1 - iBytesPerLine - iCharSpace - 1)
	{
		// Not clicked on empty place:
		if ((log_column-iOffsetLen-(iByteSpace-1))%3 != 0)
		{
			bLButtonIsDown = TRUE;
			bSelected = FALSE;
			int bytenum = (log_column-iOffsetLen-(iByteSpace-1))/3,
				nibblenum = ((log_column-iOffsetLen-(iByteSpace-1))%3)-1;
			int newpos = line * iBytesPerLine + bytenum;
			if( newpos <= LASTBYTE )
			{
				iStartOfSelection = newpos;
				iEndOfSelection = newpos;
			}
			else if( newpos == LASTBYTE + 1 )
			{
				bLButtonIsDown = FALSE; // Don't select, this can only set caret to END.
			}
		}
	}
	// Click on chars.
	else if (log_column > iCharsPerLine - 1 - iBytesPerLine	&& log_column < iCharsPerLine)
	{
		bLButtonIsDown = TRUE;
		bSelected = FALSE;
		int newpos = line * iBytesPerLine + (log_column - (iCharsPerLine - iBytesPerLine));
		if( newpos <= LASTBYTE )
		{
			iStartOfSelection = newpos;
			iEndOfSelection = newpos;
		}
		else if( newpos == LASTBYTE + 1 )
		{
			bLButtonIsDown = FALSE; // Don't select, this can only set caret to END.
		}
	}
	return 0;
}

//-------------------------------------------------------------------
// On find command.
int HexEditorWindow::CMD_find ()
{
	if (filename[0] == '\0')
		return 0;

	// If there is selected data then make it the data to find.
	if( bSelected )
	{
		int sel_start, select_len;
		if( iEndOfSelection < iStartOfSelection )
		{
			sel_start = iEndOfSelection;
			select_len = iStartOfSelection - iEndOfSelection + 1;
		}
		else
		{
			sel_start = iStartOfSelection;
			select_len = iEndOfSelection - iStartOfSelection + 1;
		}

		iFindDlgBufLen = byte_to_BC_destlen( (char*) &DataArray[sel_start], select_len );
		iFindDlgLastLen = iFindDlgBufLen;
		if( pcFindDlgBuffer != NULL )
			delete [] pcFindDlgBuffer;
		pcFindDlgBuffer = new char[iFindDlgBufLen];
		translate_bytes_to_BC( pcFindDlgBuffer, &DataArray[sel_start], select_len );
	}

	int srclen = DialogBox (hInstance, MAKEINTRESOURCE (IDD_FINDDIALOG), hwnd, (DLGPROC) FindDlgProc);
	char* pcFindstring;
	if (srclen != 0)
	{
		// Create findstring.
		int destlen = create_bc_translation (&pcFindstring, pcFindDlgBuffer, srclen, iCharacterSet, iBinaryMode);
		if (destlen > 0)
		{
			int i;
			char (*cmp) (char);

			if (iFindDlgMatchCase == BST_CHECKED)
				cmp = equal;
			else
				cmp = lower_case;

			SetCursor (LoadCursor (NULL, IDC_WAIT));
			// Find forward.
			if (iFindDlgDirection == 1)
			{
				i = find_bytes ((char*) &(DataArray[iCurByte + 1]), DataArray.GetLength() - iCurByte - 1, pcFindstring, destlen, 1, cmp);
				if (i != -1)
					iCurByte += i + 1;
			}
			// Find backward.
			else
			{
				i = find_bytes( (char*) &(DataArray[0]),
					min( iCurByte + (destlen - 1), DataArray.GetLength() ),
					pcFindstring, destlen, -1, cmp );
				if (i != -1)
					iCurByte = i;
			}
			SetCursor (LoadCursor (NULL, IDC_ARROW));

			if (i != -1)
			{
				// Caret will be vertically centered if line of found string is not visible.
				/* OLD: ONLY SET CURSOR POSITION
				if( iCurByte/iBytesPerLine < iCurLine || iCurByte/iBytesPerLine > iCurLine + cyBuffer )
					iCurLine = max( 0, iCurByte/iBytesPerLine-cyBuffer/2 );
				adjust_vscrollbar();
				*/

				// NEW: Select found interval.
				bSelected = TRUE;
				iStartOfSelection = iCurByte;
				iEndOfSelection = iCurByte + destlen - 1;
				adjust_view_for_selection();

				repaint();
			}
			else
				MessageBox (hwnd, "Could not find data.", "Find", MB_OK | MB_ICONERROR);
			iFindDlgLastLen = srclen;
			delete [] pcFindstring;
		}
		else
			MessageBox (hwnd, "Findstring is zero bytes long.", "Find", MB_OK | MB_ICONERROR);
	}
	return 1;
}

//-------------------------------------------------------------------
// On copy command.
int HexEditorWindow::CMD_edit_copy ()
{
	if (!bSelected)
	{
		// No selection: copy current byte.
		iCopyStartOffset = iCurByte;
		iCopyEndOffset = iCurByte;
	}
	else
	{
		// Copy selection.
		if (iEndOfSelection >= iStartOfSelection)
		{
			iCopyStartOffset = iStartOfSelection;
			iCopyEndOffset = iEndOfSelection;
		}
		else
		{
			iCopyStartOffset = iEndOfSelection;
			iCopyEndOffset = iStartOfSelection;
		}
	}

	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_COPYDIALOG), hwnd, (DLGPROC) CopyDlgProc) != FALSE)
	{
		// Get dialogue values.
		iStartOfSelection = iCopyStartOffset;
		iEndOfSelection = iCopyEndOffset;
		if (iEndOfSelection >= iStartOfSelection)
		{
			int destlen = byte_to_BC_destlen ((char*) &(DataArray[iStartOfSelection]), iEndOfSelection-iStartOfSelection+1);
			HGLOBAL hGlobal = GlobalAlloc (GHND, destlen);
			if (hGlobal != NULL)
			{
				SetCursor (LoadCursor (NULL, IDC_WAIT));
				char* pd = (char*) GlobalLock (hGlobal);
				translate_bytes_to_BC (pd, &(DataArray[iStartOfSelection]), iEndOfSelection-iStartOfSelection+1);
				GlobalUnlock (hGlobal);
				OpenClipboard (hwnd);
				EmptyClipboard ();
				SetClipboardData (CF_TEXT, hGlobal);
				CloseClipboard ();
				SetCursor (LoadCursor (NULL, IDC_ARROW));
			}
			else
				MessageBox (hwnd, "Not enough memory for copying.", "Copy", MB_OK | MB_ICONERROR);
		}
		else
		{
			MessageBox (hwnd, "End offset is smaller than start offset.", "Copy", MB_OK | MB_ICONERROR);
		}
	}
	adjust_view_for_selection ();
	return 1;
}

//-------------------------------------------------------------------
// On hexdump to file/clipboard command.
int HexEditorWindow::CMD_copy_hexdump ()
{
	if (DataArray.GetLength() <= 0)
	{
		MessageBox (hwnd, "Can't hexdump empty file.", "Export hexdump", MB_OK | MB_ICONERROR);
		return 0;
	}
	if( !bSelected )
	{
		// Assume whole file is to be hexdumped.
		iCopyHexdumpDlgStart = 0;
		iCopyHexdumpDlgEnd = ((DataArray.GetUpperBound())/iBytesPerLine)*iBytesPerLine;
	}
	else
	{
		// Assume selected area is to be hexdumped.
		// Make sure end of selection is greater than start of selection.
		if( iEndOfSelection < iStartOfSelection )
		{
			int i = iEndOfSelection;
			iEndOfSelection = iStartOfSelection;
			iStartOfSelection = i;
		}
		iCopyHexdumpDlgStart = ( iStartOfSelection / iBytesPerLine ) * iBytesPerLine;
		iCopyHexdumpDlgEnd = ( iEndOfSelection / iBytesPerLine ) * iBytesPerLine;
	}

	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_HEXDUMPDIALOG), hwnd, (DLGPROC) CopyHexdumpDlgProc) != FALSE)
	{
		// Show wait cursor.
		SetCursor (LoadCursor (NULL, IDC_WAIT));
		if (iCopyHexdumpDlgStart % iBytesPerLine != 0 || iCopyHexdumpDlgEnd % iBytesPerLine != 0)
		{
			MessageBox (hwnd, "Offsets must be multiples of\nthe number of bytes per line.", "Export hexdump", MB_OK | MB_ICONERROR);
			return 0;
		}
		// Number of lines to copy:
		int linecount = (iCopyHexdumpDlgEnd - iCopyHexdumpDlgStart) / iBytesPerLine + 1;
		// Req'd mem for lines:
		// (Every line ended with CR+LF ('\r'+'\n'))
		int buflen = linecount * (iCharsPerLine+2) + 1;
		// Create hexdump.
		int a,b,k,m,n,j,l;
		char buf1[128], buf2[128];
		char* pMem = new char[buflen];
		for (n=0; n < buflen; n++)
			pMem[n] = ' ';
		// Write hexdump.
		// a = first byte of first line of hexdump.
		// b = first byte of last line of hexdump.
		b = iCopyHexdumpDlgEnd;
		// a = Offset of current line.
		// k = Offset in text array.
		for (k = 0, a = iCopyHexdumpDlgStart; a <= b; a += iBytesPerLine, k += iCharsPerLine + 2)
		{
			// Write offset.
			sprintf (buf1, "%%%d.%dx", iOffsetLen, iOffsetLen);
			for (m = 0; m < iByteSpace; m++)
				strcat (buf1, " ");
			sprintf (buf2, buf1, a); // buf2 = Offset+Spaces.
			l = 0; // l = Offset in line, relative to k.
			n = 0;
			while (buf2[n] != '\0')
				pMem[k + (l++)] = buf2[n++]; // Copy Offset. l = next empty place after spaces.
			// Write bytes and chars.
			for (j = 0; j < iBytesPerLine; j++)
			{
				if (a+j > DataArray.GetUpperBound ())
				{
					// Nonexistant byte.
					pMem[k + l + j*3    ] = ' ';
					pMem[k + l + j*3 + 1] = ' ';
					pMem[k + l + j*3 + 2] = ' ';
					// Nonexistant char.
					pMem[k + l + iBytesPerLine*3 + iCharSpace + j] = ' ';
				}
				else
				{
					// Write byte.
					sprintf (buf1, "%2.2x ", DataArray[a + j]);
					pMem[k + l + j*3    ] = buf1[0];
					pMem[k + l + j*3 + 1] = buf1[1];
					pMem[k + l + j*3 + 2] = buf1[2];
					// Write char.
					if( iCharacterSet == OEM_FIXED_FONT && DataArray[a + j] != 0 )
						pMem[k + l + iBytesPerLine*3 + iCharSpace + j] = DataArray[a + j];
					else if( (DataArray[a + j] >= 32 && DataArray[a + j] <= 126) || (DataArray[a + j]>=160 && DataArray[a + j] <= 255) || (DataArray[a + j] >= 145 && DataArray[a + j] <= 146) )
						pMem[k + l + iBytesPerLine*3 + iCharSpace + j] = DataArray[a + j];
					else
						pMem[k + l + iBytesPerLine*3 + iCharSpace + j] = '.';
				}
			}
			pMem[k + iCharsPerLine    ] = '\r';
			pMem[k + iCharsPerLine + 1] = '\n';
		}
		pMem[buflen-1] = '\0';
		// Remove wait cursor.
		SetCursor (LoadCursor (NULL, IDC_ARROW));
		if (iCopyHexdumpMode == BST_CHECKED)
		{
			// To clipboard.
			HGLOBAL hGlobal = GlobalAlloc (GHND, buflen);
			if (hGlobal != NULL)
			{
				char* pDest = (char*) GlobalLock (hGlobal);
				memcpy (pDest, pMem, buflen);
				GlobalUnlock (hGlobal);
				OpenClipboard (hwnd);
				EmptyClipboard ();
				SetClipboardData (CF_TEXT, hGlobal);
				CloseClipboard ();
			}
			else
				MessageBox (hwnd, "Not enough memory for hexdump to clipboard.", "Export hexdump", MB_OK | MB_ICONERROR);
		}
		else
		{
			// to file.
			char szFileName[_MAX_PATH];
			char szTitleName[_MAX_FNAME + _MAX_EXT];
			strcpy (szFileName, "hexdump.txt");
			OPENFILENAME ofn;
			ofn.lStructSize = sizeof (OPENFILENAME);
			ofn.hwndOwner = hwnd;
			ofn.hInstance = NULL;
			ofn.lpstrFilter = "All Files (*.*)\0*.*\0\0";
			ofn.lpstrCustomFilter = NULL;
			ofn.nMaxCustFilter = 0;
			ofn.nFilterIndex = 0;
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = _MAX_PATH;
			ofn.lpstrFileTitle = szTitleName;
			ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
			ofn.lpstrInitialDir = NULL;
			ofn.lpstrTitle = NULL;
			ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT;
			ofn.nFileOffset = 0;
			ofn.nFileExtension = 0;
			ofn.lpstrDefExt = NULL;
			ofn.lCustData = 0L;
			ofn.lpfnHook = NULL;
			ofn.lpTemplateName = NULL;
			if (GetSaveFileName (&ofn))
			{
				int filehandle;
				if ((filehandle = _open (szFileName, _O_RDWR|_O_CREAT|_O_TRUNC|_O_BINARY,_S_IREAD|_S_IWRITE)) != -1)
				{
					// Write file.
					if ((_write (filehandle, pMem, buflen-1)) != -1)
					{
						MessageBox (NULL, "Hexdump saved.", "Export hexdump", MB_OK | MB_ICONINFORMATION);
					}
					else
						MessageBox (NULL, "Could not save Hexdump.", "Export hexdump", MB_OK | MB_ICONERROR);
					_close (filehandle);
				}
				else
					MessageBox (NULL, "Could not save Hexdump.", "Export hexdump", MB_OK | MB_ICONERROR);
			}
			repaint ();
		}
		delete [] pMem;
	}
	return 1;
}

//-------------------------------------------------------------------
BOOL CALLBACK CopyHexdumpDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{	
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			char buf[16];
			sprintf (buf, "%x", iCopyHexdumpDlgStart);
			SetWindowText (GetDlgItem (hDlg, IDC_EDIT1), buf);
			sprintf (buf, "%x", iCopyHexdumpDlgEnd);
			SetWindowText (GetDlgItem (hDlg, IDC_EDIT2), buf);
			if (iCopyHexdumpMode == BST_CHECKED)
				CheckDlgButton (hDlg, IDC_RADIO2, BST_CHECKED);
			else
				CheckDlgButton (hDlg, IDC_RADIO1, BST_CHECKED);
			SetFocus (GetDlgItem (hDlg, IDC_EDIT1));
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				char buf1[16], buf2[16];
				if (GetDlgItemText (hDlg, IDC_EDIT1, buf1, 16)!=0 && GetDlgItemText (hDlg, IDC_EDIT2, buf2, 16))
				{
					sscanf (buf1, "%x", &iCopyHexdumpDlgStart);
					sscanf (buf2, "%x", &iCopyHexdumpDlgEnd);
					iCopyHexdumpMode = IsDlgButtonChecked (hDlg, IDC_RADIO2);
					EndDialog (hDlg, TRUE);
				}
				else
					EndDialog (hDlg, FALSE);
				return TRUE;
			}
		case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//-------------------------------------------------------------------
// On paste command.
int HexEditorWindow::CMD_edit_paste ()
{
//Pabs changed - line comment out
//	iPasteTimes = 1;
//	iPasteAsText = FALSE;
//end
	if( iInsertMode )
		iPasteMode = 2;
	else
		iPasteMode = 1;
	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_PASTEDIALOG), hwnd, (DLGPROC) PasteDlgProc))
	{
		switch( iPasteMode )
		{
		case 1: // Overwrite.
			{
				char* pcPastestring;
				// Create findstring.
				int destlen;
				if( iPasteAsText == TRUE )
				{
					destlen = strlen( pcPasteText );
					pcPastestring = new char[ destlen ];
					memcpy( pcPastestring, pcPasteText, destlen );
				}
				else
				{
					destlen = create_bc_translation (&pcPastestring, pcPasteText, strlen (pcPasteText), iCharacterSet, iBinaryMode);
				}
				if (destlen > 0)
				{
					// Enough space for writing?
					// DataArray.GetLength()-iCurByte = number of bytes from including curbyte to end.
//Pabs changed - "(iPasteSkip+destlen)" used to be "destlen"
					if (DataArray.GetLength()-iCurByte >= (iPasteSkip+destlen)*iPasteTimes)
//end
					{
						// Overwrite data.
						SetCursor (LoadCursor (NULL, IDC_WAIT));
						int i,k;
						for (k=0; k<iPasteTimes; k++)
						{
							for (i=0; i<destlen; i++)
							{
//Pabs changed - "(iPasteSkip+destlen)" used to be "destlen"
								DataArray[(iCurByte+k*(iPasteSkip+destlen))+i] = pcPastestring[i];
//end
							}
						}
						SetCursor (LoadCursor (NULL, IDC_ARROW));
						m_iFileChanged = TRUE;
						bFilestatusChanged = TRUE;
					}
					else
					{
						MessageBox (hwnd, "Not enough space for overwriting.", "Paste", MB_OK | MB_ICONERROR);
					}
					delete [] pcPastestring;
				}
				delete [] pcPasteText;
				repaint ();
				break;
			}

		case 2: // Insert.
			{
				char* pcPastestring;
				int destlen;
				if( iPasteAsText == TRUE )
				{
					destlen = strlen( pcPasteText );
					pcPastestring = new char[ destlen ];
					memcpy( pcPastestring, pcPasteText, destlen );
				}
				else
				{
					destlen = create_bc_translation( &pcPastestring, pcPasteText, strlen( pcPasteText ), iCharacterSet, iBinaryMode );
				}
				if( destlen > 0 )
				{
					// Insert at iCurByte. Bytes there will be pushed up.
					SetCursor( LoadCursor( NULL, IDC_WAIT ) );
//Pabs changed - line insert
					int i, k;
					for( k = 0,i=iCurByte; k < iPasteTimes; k++ ){
						if(!DataArray.InsertAtGrow(iCurByte,(unsigned char*)pcPastestring,0,destlen)){
							delete [] pcPastestring;
							delete [] pcPasteText;
							SetCursor (LoadCursor (NULL, IDC_ARROW));
							MessageBox (hwnd, "Not enough memory for inserting.", "Paste", MB_OK | MB_ICONERROR);
							update_for_new_datasize ();
							return FALSE;
						}
						m_iFileChanged = TRUE;
						bFilestatusChanged = TRUE;
						iCurByte+=destlen+iPasteSkip;
					}
					iCurByte=i;
//end
					// RK: removed old code that pabs commented out.
					m_iFileChanged = TRUE;
					bFilestatusChanged = TRUE;
					update_for_new_datasize ();
					delete [] pcPastestring;
					SetCursor (LoadCursor (NULL, IDC_ARROW));
				}
				else
				{
					delete [] pcPasteText;
					MessageBox( hwnd, "Tried to insert zero-length array.", "Paste", MB_OK | MB_ICONERROR);
					update_for_new_datasize ();
					return FALSE;
				}
				delete [] pcPasteText;
				repaint ();
				break;
			}

		default:
			break;
		}
	}
	return 0;
}

//-------------------------------------------------------------------
// On "enter decimal value" command.
int HexEditorWindow::CMD_edit_enterdecimalvalue ()
{
	iDecValDlgOffset = iCurByte;
	if (iCurByte <= LASTBYTE && iCurByte >= 0)
		iDecValDlgValue = (int) DataArray[iCurByte];
	else
		iDecValDlgValue = 0;
	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_DECIMALDIALOG), hwnd, (DLGPROC) EnterDecimalValueDlgProc))
	{
		if (iDecValDlgOffset<0 || iDecValDlgOffset>LASTBYTE)
		{
			MessageBox (hwnd, "Invalid start offset.", "Enter decimal value", MB_OK | MB_ICONERROR);
			return 0;
		}
		if (iDecValDlgOffset+iDecValDlgSize*iDecValDlgTimes > LASTBYTE)
		{
			MessageBox (hwnd, "Not enough space for writing decimal values.", "Enter decimal value", MB_OK | MB_ICONERROR);
			return 0;
		}
		SetCursor (LoadCursor (NULL, IDC_WAIT));
		int i, k = 0;
		for (i = 0; i < iDecValDlgTimes; i++)
		{
			if (iDecValDlgOffset + k > DataArray.GetUpperBound ())
			{
				MessageBox (hwnd, "Reached end of file prematurely.", "Enter decimal value", MB_OK | MB_ICONERROR);
				break;
			}

			if (iBinaryMode == LITTLEENDIAN_MODE)
			{
				switch (iDecValDlgSize)
				{
				case 1:
					DataArray[iDecValDlgOffset + k] = iDecValDlgValue;
					break;

				case 2:
					DataArray[iDecValDlgOffset + k] = (char) (iDecValDlgValue & 0xff);
					DataArray[iDecValDlgOffset + k+1] = (char) ((iDecValDlgValue & 0xff00) >> 8);
					break;

				case 4:
					DataArray[iDecValDlgOffset + k  ] = (char) (iDecValDlgValue & 0xff);
					DataArray[iDecValDlgOffset + k+1] = (char) ((iDecValDlgValue & 0xff00) >> 8);
					DataArray[iDecValDlgOffset + k+2] = (char) ((iDecValDlgValue & 0xff0000) >> 16);
					DataArray[iDecValDlgOffset + k+3] = (char) ((iDecValDlgValue & 0xff000000) >> 24);
					break;
				}
			}
			else
			{
				switch (iDecValDlgSize)
				{
				case 1:
					DataArray[iDecValDlgOffset + k] = iDecValDlgValue;
					break;

				case 2:
					DataArray[iDecValDlgOffset + k+1] = (char) (iDecValDlgValue & 0xff);
					DataArray[iDecValDlgOffset + k] = (char) ((iDecValDlgValue & 0xff00) >> 8);
					break;

				case 4:
					DataArray[iDecValDlgOffset + k+3] = (char) (iDecValDlgValue & 0xff);
					DataArray[iDecValDlgOffset + k+2] = (char) ((iDecValDlgValue & 0xff00) >> 8);
					DataArray[iDecValDlgOffset + k+1] = (char) ((iDecValDlgValue & 0xff0000) >> 16);
					DataArray[iDecValDlgOffset + k+0] = (char) ((iDecValDlgValue & 0xff000000) >> 24);
					break;
				}
			}
			k += iDecValDlgSize;
		}
		SetCursor (LoadCursor (NULL, IDC_ARROW));
		m_iFileChanged = TRUE;
		bFilestatusChanged = TRUE;
		repaint ();
	}
	return 1;
}

//-------------------------------------------------------------------
BOOL CALLBACK EnterDecimalValueDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			char buf[16];
			sprintf (buf, "%d", iDecValDlgValue);
			SetWindowText (GetDlgItem (hDlg, IDC_EDIT1), buf);
			sprintf (buf, "x%x", iDecValDlgOffset);
			SetWindowText (GetDlgItem (hDlg, IDC_EDIT2), buf);
			sprintf (buf, "1");
			SetWindowText (GetDlgItem (hDlg, IDC_EDIT3), buf);
			CheckDlgButton (hDlg, IDC_RADIO1, BST_CHECKED);
			SetFocus (GetDlgItem (hDlg, IDC_EDIT1));
			return FALSE;
		}

	case WM_COMMAND:
		if (IsDlgButtonChecked (hDlg, IDC_RADIO1) == BST_CHECKED)
			iDecValDlgSize = 1;
		else if (IsDlgButtonChecked (hDlg, IDC_RADIO2) == BST_CHECKED)
			iDecValDlgSize = 2;
		else if (IsDlgButtonChecked (hDlg, IDC_RADIO3) == BST_CHECKED)
			iDecValDlgSize = 4;
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				char buf[16];
				if (GetDlgItemText (hDlg, IDC_EDIT1, buf, 16) != 0)
					sscanf (buf, "%d", &iDecValDlgValue);
				if (GetDlgItemText (hDlg, IDC_EDIT2, buf, 16) != 0)
					sscanf (buf, "%x", &iDecValDlgOffset);
				int i;
				if (sscanf (buf, "x%x", &i) == 0)
				{
					if (sscanf (buf, "%d", &i) == 0)
					{
						i = -1;
					}
				}
				iDecValDlgOffset = i;
				if (GetDlgItemText (hDlg, IDC_EDIT3, buf, 16) != 0)
					sscanf (buf, "%d", &iDecValDlgTimes);
				EndDialog (hDlg, 1);
				return TRUE;
			}

		case IDCANCEL:
			EndDialog (hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//-------------------------------------------------------------------
BOOL CALLBACK PasteDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			char *buf;
			if (OpenClipboard (NULL))
			{
				HGLOBAL hClipMemory = GetClipboardData (CF_TEXT);
				if (hClipMemory != NULL)
				{
					int gsize = GlobalSize (hClipMemory);
					if (gsize > 0)
					{
						char* pClipMemory = (char*) GlobalLock (hClipMemory);
						buf = new char[gsize];
						memcpy (buf, pClipMemory, gsize);
						SetWindowText (GetDlgItem (hDlg, IDC_EDIT1), buf);
						delete [] buf;
					}
					GlobalUnlock (hClipMemory);
				}
				CloseClipboard ();
			}
			char buf2[16];
			sprintf (buf2, "%d", iPasteTimes);
			SetWindowText (GetDlgItem (hDlg, IDC_EDIT2), buf2);
//Pabs changed - line insert
			sprintf (buf2, "%d", iPasteSkip);
			SetWindowText (GetDlgItem (hDlg, IDC_EDIT3), buf2);
//end
			if( iPasteMode == 1 )
				CheckDlgButton (hDlg, IDC_RADIO1, BST_CHECKED);
			else
				CheckDlgButton (hDlg, IDC_RADIO2, BST_CHECKED);
			if( iPasteAsText == TRUE )
				CheckDlgButton (hDlg, IDC_CHECK1, BST_CHECKED);
			SetFocus (GetDlgItem (hDlg, IDC_EDIT1));
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				if (IsDlgButtonChecked (hDlg, IDC_CHECK1) == BST_CHECKED)
					iPasteAsText = TRUE;
				else
					iPasteAsText = FALSE;
				if (IsDlgButtonChecked (hDlg, IDC_RADIO1) == BST_CHECKED)
					iPasteMode = 1;
				else if( IsDlgButtonChecked( hDlg, IDC_RADIO2 ) == BST_CHECKED )
					iPasteMode = 2;
				char buf[64];
				int i;
				if (GetDlgItemText (hDlg, IDC_EDIT2, buf, 64) != 0)
				{
					if (sscanf (buf, "%d", &i) == 0)
					{
						MessageBox (NULL, "Number of times to paste not recognized.", "Paste", MB_OK | MB_ICONERROR);
						i = -1;
					}
				}
				if (i==-1 || i==0)
				{
					MessageBox (NULL, "Number of times to paste must be at least 1.", "Paste", MB_OK | MB_ICONERROR);
					EndDialog (hDlg, 0);
					return 0;
				}
				iPasteTimes = i;
//Pabs changed - line insert
				if (GetDlgItemText (hDlg, IDC_EDIT3, buf, 64) != 0)
				{
					if (sscanf (buf, "%d", &i) == 0)
					{
						MessageBox (NULL, "Number of bytes to skip not recognized.", "Paste", MB_OK | MB_ICONERROR);
						EndDialog (hDlg, 0);
						return 0;
					}
				}
				iPasteSkip = i;
//end
				iPasteMaxTxtLen = SendMessage (GetDlgItem (hDlg, IDC_EDIT1), EM_GETLIMITTEXT, 0, 0) + 1;
				pcPasteText = new char[iPasteMaxTxtLen];
				GetDlgItemText (hDlg, IDC_EDIT1, pcPasteText, iPasteMaxTxtLen);
				EndDialog (hDlg, 1);
				return TRUE;
			}
	
		case IDCANCEL:
			EndDialog (hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//-------------------------------------------------------------------
// Find char c from pointer to array src on, return it's position.
int HexEditorWindow::find_byte_pos (char* src, char c)
{
	int i=0;
	while (src[i] != c)
	{
		i++;
	}
	return i;
}

//-------------------------------------------------------------------
// dest must be set to right length before calling.
int HexEditorWindow::create_bc_translation (char* dest, char* src, int srclen, int charmode, int binmode)
{
	int i, di=0, bclen;
	for (i=0; i<srclen; i++)
	{
		if ((bclen = is_bytecode (&(src[i]), srclen-i)) > 0) // Get length of byte-code.
		{
			// Bytecode found.
			translate_bytecode (&(dest[di]), &(src[i]), srclen-i, binmode);
			di += bclen;
			i += find_byte_pos (&(src[i]), '>');
		}
		else // Normal character.
		{
			if (src[i] == '\\') // Special char "\<" or "\\"?
			{
				if (i+1 < srclen)
				{
					if (src[i+1] == '<')
					{
						dest[di++] = '<'; // Special char recognized.
						i++;
					}
					else if( src[i+1] == '\\' )
					{
						dest[di++] = '\\'; // Special char recognized.
						i++;
					}
					else
						dest[di++] = src[i]; // Unknown special char.
				}
				else
					dest[di++] = src[i]; // Not enough space for special char.
			}
			else
			{
				// No special char.
				switch (charmode)
				{
				case ANSI_SET:
					dest[di++] = src[i];
					break;

				case OEM_SET:
					dest[di++] = TranslateAnsiToOem (src[i]);
					break;
				}
			}
		}
	}
	return di;
}

//-------------------------------------------------------------------
// Get value of one code.
// Return: value of code.
// bytecode must be checked before.
int HexEditorWindow::translate_bytecode (char* dest, char* src, int srclen, int binmode)
{
	int i, k=0;
	char buf[50];
	for (i=4; i<srclen; i++)
	{
		if (src[i]=='>')
			break;
		else
		{
			buf[k++] = src[i];
		}
	}
	buf[k] = 0;
	int value;
	float fvalue;
	double dvalue;
	switch (src[2]) // Get value from text.
	{
	case 'd':
		sscanf (buf, "%d", &value);
		break;

	case 'h':
		sscanf (buf, "%x", &value);
		break;

	case 'l':
		sscanf (buf, "%f", &fvalue);
		break;

	case 'o':
		sscanf (buf, "%lf", &dvalue);
		break;
	}

	if (binmode == LITTLEENDIAN_MODE)
	{
		switch (src[1])
		{
		case 'b':
			dest[0] = (char) value;
			break;

		case 'w':
			dest[0] = (value & 0xff);
			dest[1] = (value & 0xff00)>>8;
			break;
		
		case 'l':
			dest[0] = (value & 0xff);
			dest[1] = (value & 0xff00)>>8;
			dest[2] = (value & 0xff0000)>>16;
			dest[3] = (value & 0xff000000)>>24;
			break;

		case 'f':
			*((float*)dest) = fvalue;
			break;

		case 'd':
			*((double*)dest) = dvalue;
			break;
		}
	}
	else // BIGENDIAN_MODE
	{
		switch (src[1])
		{
		case 'b':
			dest[0] = (char) value;
			break;

		case 'w':
			dest[0] = HIBYTE (LOWORD (value));
			dest[1] = LOBYTE (LOWORD (value));
			break;
		
		case 'l':
			dest[0] = HIBYTE (HIWORD (value));
			dest[1] = LOBYTE (HIWORD (value));
			dest[2] = HIBYTE (LOWORD (value));
			dest[3] = LOBYTE (LOWORD (value));
			break;

		case 'f':
			{
				char* p = (char*) &fvalue;
				int i;
				for (i=0; i<4; i++)
				{
					dest[i] = p[3-i];
				}
			}
			break;

		case 'd':
			{
				char* p = (char*) &dvalue;
				int i;
				for (i=0; i<8; i++)
				{
					dest[i] = p[7-i];
				}
			}
			break;
		}
	}
	return value;
}

//-------------------------------------------------------------------
// Get length of code.
int HexEditorWindow::calc_bctrans_destlen (char* src, int srclen)
{
	int i, destlen = 0, l, k;
	for (i=0; i<srclen; i++)
	{
		if ((l = is_bytecode (&(src[i]), srclen-i)) == 0)
		{
			if (src[i] == '\\')
			{
				if (i+1 < srclen)
				{
					if (src[i+1] == '<')
					{
						// Code for "<" alone without decoding.
						destlen++;
						i++;
					}
					else if( src[i+1] == '\\' )
					{
						// Code for "\\".
						destlen++;
						i++;
					}
					else
					{
						destlen++;
					}
				}
				else
				{
					destlen++;
				}
			}
			else
			{
				destlen++;
			}
		}
		else
		{
			destlen += l;
			for (k=i; i<srclen; k++)
			{
				if (src[k]=='>')
					break;
			}
			i = k;
		}
	}
	return destlen;
}

//-------------------------------------------------------------------
// Bytecode?
// Return = 0 if no bytecode
//		  = Length 1/2/4 if bytecode
int HexEditorWindow::is_bytecode (char* src, int len)
{
	int i=0;

	if (src[i] == '<')
	{
		if (i+1 < len)
		{
			switch (src[i+1])
			{
			case 'b': case 'w': case 'l': case 'f': case 'd':
				if (i+2 < len)
				{
					switch (src[i+2])
					{
						case 'd': case 'h': case 'l': case 'o':
							if (i+3 < len)
							{
								if (src[i+3] == ':')
								{
									int j,k;
									for (j=4; j < len; j++)
									{
										if (src[i+j] == '>')
											break;
									}
									if (j==4 || j==len)
										return FALSE;
									for (k=4; k<j; k++)
									{
										switch (src[i+2])
										{
										case 'd':
											if ((src[i+k]>='0' && src[i+k]<='9') || src[i+k]=='-')
												continue;
											else
												return FALSE; // Non-digit found.
											break;

										case 'h':
											if ((src[i+k]>='0' && src[i+k]<='9') ||
											 (src[i+k]>='a' && src[i+k]<='f'))
												continue;
											else
												return FALSE; // Non-hex-digit.
											break;

										case 'o': case 'l': // float or double.
											if ((src[i+k]>='0' && src[i+k]<='9') || src[i+k]=='-' || src[i+k]=='.' || src[i+k]=='e' || src[i+k]=='E')
												continue;
											else
												return FALSE;
											break;
										}
									}
									switch (src[i+1])
									{
									default:
									case 'b': return 1;
									case 'w': return 2;
									case 'l': return 4;
									case 'f': return 4;
									case 'd': return 8;
									}
								}
								else
									return FALSE; // No ':'.
							}
							else
								return FALSE; // No space for ':'.
							break;
						
						default:
							return FALSE; // Wrong second option.
					}
				}
				else
					return FALSE; // No space for option 2.
				break;
			
			default:
				return FALSE; // Wrong first option.
				break;
			}
		}
		else
			return FALSE; // No space for option 1;
	}
	else
		return FALSE; // No '<'.
}

//-------------------------------------------------------------------
// Create translation of bytecode-string.
// Return: Length of resulting string.
// ppd = pointer to pointer to result, must be delete[]-ed later.
// If the input string was empty, no translated array is created and zero is returned.
int HexEditorWindow::create_bc_translation (char** ppd, char* src, int srclen, int charmode, int binmode)
{
	int destlen = calc_bctrans_destlen (src, srclen);
	if (destlen > 0)
	{
		*ppd = new char[destlen];
		create_bc_translation (*ppd, src, srclen, charmode, binmode);
		return destlen;
	}
	else
	{
		// Empty input string => don't allocate anything and return 0.
		*ppd = NULL;
		return 0;
	}
}

//-------------------------------------------------------------------
// Translate an array of bytes to a text string using special syntax.
// Return: Length of string including zero-byte.
int HexEditorWindow::translate_bytes_to_BC (char* pd, unsigned char* src, int srclen)
{
	int i, k = 0;
	char buf[16];
	for (i=0; i<srclen; i++)
	{
		if (src[i] == '<')
		{
			pd[k++] = '\\';
			pd[k++] = '<';
		}
		else if( src[i] == '\\' )
		{
			pd[k++] = '\\';
			pd[k++] = '\\';
		}
		else if (src[i] >= 32 && src[i] < 127)
		{
			pd[k++] = src[i];
		}
		else if( src[i]==10 || src[i]==13 )
		{
			pd[k++] = src[i];
		}
		else
		{
			pd[k++] = '<';
			pd[k++] = 'b';
			pd[k++] = 'h';
			pd[k++] = ':';
			sprintf (buf, "%2.2x", src[i]);
			pd[k++] = buf[0];
			pd[k++] = buf[1];
			pd[k++] = '>';
		}
	}
	pd[k] = '\0';
	return k+1;
}

//-------------------------------------------------------------------
// Used with translate_bytes_to_BC.
// Return: Length of bytecode-string including zero-byte.
int HexEditorWindow::byte_to_BC_destlen (char* src, int srclen)
{
	int i, destlen = 1;
	for (i=0; i<srclen; i++)
	{
		if (src[i] == '<')
			destlen+=2; // Escapecode needed.
		else if( src[i] == '\\' )
			destlen+=2; // Escapecode needed.
		else if (src[i] >= 32 && src[i] < 127)
			destlen++; // Normal char.
		else if( src[i]==10 || src[i]==13 )
			destlen++; // LF/CR.
		else
			destlen+=7; // Escapecode needed.
	}
	return destlen;
}

//-------------------------------------------------------------------
// If filesize changes, scrollbars etc. must be adjusted.
void HexEditorWindow::update_for_new_datasize ()
{
	RECT r;
	GetClientRect (hwnd, &r);
	resize_window (r.right, r.bottom);
}

//-------------------------------------------------------------------
int HexEditorWindow::CMD_edit_cut ()
{
	if (bSelected) // If selecting...
	{
		if (iEndOfSelection >= iStartOfSelection)
		{
			iCutOffset = iStartOfSelection;
			iCutNumberOfBytes = iEndOfSelection-iStartOfSelection+1;
		}
		else
		{
			iCutOffset = iEndOfSelection;
			iCutNumberOfBytes = iStartOfSelection-iEndOfSelection+1;
		}
	}
	else // No selection: cut current byte.
	{
		iCutOffset = iCurByte;
		iCutNumberOfBytes = 1;
	}
	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_CUTDIALOG), hwnd, (DLGPROC) CutDlgProc))
	{
		// Can requested number be cut?
		// DataArray.GetLength ()-iCutOffset = number of bytes from current pos. to end.
		if( DataArray.GetLength() - iCutOffset >= iCutNumberOfBytes )
		{
			// OK
			int newlen = DataArray.GetLength () - iCutNumberOfBytes;
			// Cut to clipboard?
			switch (iCutMode)
			{
			case BST_CHECKED:
				{
					// Transfer to cipboard.
					int destlen = byte_to_BC_destlen ((char*) &(DataArray[iCutOffset]), iCutNumberOfBytes);
					HGLOBAL hGlobal = GlobalAlloc (GHND, destlen);
					if (hGlobal != NULL)
					{
						SetCursor (LoadCursor (NULL, IDC_WAIT));
						char* pd = (char*) GlobalLock (hGlobal);
						translate_bytes_to_BC (pd, &(DataArray[iCutOffset]), iCutNumberOfBytes);
						GlobalUnlock (hGlobal);
						OpenClipboard (hwnd);
						EmptyClipboard ();
						SetClipboardData (CF_TEXT, hGlobal);
						CloseClipboard ();
						SetCursor (LoadCursor (NULL, IDC_ARROW));
					}
					else
					{
						// Not enough memory for clipboard.
						MessageBox (NULL, "Not enough memory for cutting to clipboard.", "Cut", MB_OK | MB_ICONERROR);
						return 0;
					}
					break;
				}
				
			default:
				break;
			}
			// Cut data.
			if (DataArray.RemoveAt (iCutOffset, iCutNumberOfBytes) == FALSE)
			{
				MessageBox (NULL, "Could not cut data.", "Cut", MB_OK | MB_ICONERROR);
				return FALSE;
			}
			iCurByte = iCutOffset;
			if (iCurByte > LASTBYTE)
				iCurByte = LASTBYTE;
			if (iCurByte<0)
				iCurByte=0;
			m_iFileChanged = TRUE;
			bFilestatusChanged = TRUE;
			bSelected = FALSE;
			update_for_new_datasize ();
			repaint ();
		}
		else
		{
			// Too many bytes to cut.
			MessageBox (NULL, "Can't cut more bytes than are present.", "Cut", MB_OK | MB_ICONERROR);
		}
	}
	return 0;
}

//-------------------------------------------------------------------
BOOL CALLBACK CutDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			char buf[32];
			sprintf (buf, "x%x", iCutOffset);
			SetWindowText (GetDlgItem (hDlg, IDC_EDIT1), buf);
			sprintf (buf, "x%x", iCutOffset+iCutNumberOfBytes-1);
			SetWindowText (GetDlgItem (hDlg, IDC_EDIT2), buf);
			sprintf (buf, "%d", iCutNumberOfBytes);
			SetWindowText (GetDlgItem (hDlg, IDC_EDIT3), buf);
			SetFocus (GetDlgItem (hDlg, IDC_EDIT1));
			CheckDlgButton (hDlg, IDC_CHECK1, iCutMode);
			CheckDlgButton (hDlg, IDC_RADIO1, BST_CHECKED);
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				char buf[64];
				if (GetDlgItemText (hDlg, IDC_EDIT1, buf, 64) != 0)
				{
					int i;
					if (sscanf (buf, "x%x", &i) == 0)
					{
						if (sscanf (buf, "%d", &i) == 0)
						{
							MessageBox (NULL, "Start offset not recognized.", "Cut", MB_OK | MB_ICONERROR);
							i = -1;
						}
					}
					if (i==-1)
					{
						EndDialog (hDlg, 0);
						return 0;
					}
					iCutOffset = i;
				}

				switch (IsDlgButtonChecked (hDlg, IDC_RADIO1))
				{
				case BST_CHECKED: // Get end offset.
					{
						if (GetDlgItemText (hDlg, IDC_EDIT2, buf, 64) != 0)
						{
							int i;
							if (sscanf (buf, "x%x", &i) == 0)
							{
								if (sscanf (buf, "%d", &i) == 0)
								{
									MessageBox (NULL, "End offset not recognized.", "Cut", MB_OK | MB_ICONERROR);
									i = -1;
								}
							}
							if (i==-1)
							{
								EndDialog (hDlg, 0);
								return 0;
							}
							iCutNumberOfBytes = i-iCutOffset+1;
						}
					}
					break;

				default: // Get number of bytes.
					{
						if (GetDlgItemText (hDlg, IDC_EDIT3, buf, 64) != 0)
						{
							int i;
							if (sscanf (buf, "%d", &i) == 0)
							{
								MessageBox (NULL, "Number of bytes not recognized.", "Cut", MB_OK | MB_ICONERROR);
								i = -1;
							}
							if (i==-1)
							{
								EndDialog (hDlg, 0);
								return 0;
							}
							iCutNumberOfBytes = i;
						}
					}
				}

				iCutMode = IsDlgButtonChecked (hDlg, IDC_CHECK1);
				EndDialog (hDlg, 1);
				return TRUE;
			}
	
		case IDCANCEL:
			EndDialog (hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//-------------------------------------------------------------------
BOOL CALLBACK CopyDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			char buf[32];
			sprintf (buf, "x%x", iCopyStartOffset);
			SetWindowText (GetDlgItem (hDlg, IDC_EDIT1), buf);
			sprintf (buf, "x%x", iCopyEndOffset);
			SetWindowText (GetDlgItem (hDlg, IDC_EDIT2), buf);
			sprintf (buf, "%d", iCopyEndOffset-iCopyStartOffset+1);
			SetWindowText (GetDlgItem (hDlg, IDC_EDIT3), buf);
			CheckDlgButton (hDlg, IDC_RADIO1, BST_CHECKED);
			SetFocus (GetDlgItem (hDlg, IDC_EDIT1));
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				char buf[64];
				// Read start offset.
				if (GetDlgItemText (hDlg, IDC_EDIT1, buf, 64) != 0)
				{
					int i;
					if (sscanf (buf, "x%x", &i) == 0)
					{
						if (sscanf (buf, "%d", &i) == 0)
						{
							MessageBox (NULL, "Start offset not recognized.", "Copy", MB_OK | MB_ICONERROR);
							i = -1;
						}
					}
					if (i==-1)
					{
						EndDialog (hDlg, 0);
						return 0;
					}
					iCopyStartOffset = i;
				}

				switch (IsDlgButtonChecked (hDlg, IDC_RADIO1))
				{
				case BST_CHECKED: // Get end offset.
					{
						if (GetDlgItemText (hDlg, IDC_EDIT2, buf, 64) != 0)
						{
							int i;
							if (sscanf (buf, "x%x", &i) == 0)
							{
								if (sscanf (buf, "%d", &i) == 0)
								{
									MessageBox (NULL, "End offset not recognized.", "Copy", MB_OK | MB_ICONERROR);
									i = -1;
								}
							}
							if (i==-1)
							{
								EndDialog (hDlg, 0);
								return 0;
							}
							iCopyEndOffset = i;
						}
						break;
					}

				default: // Get number of bytes.
					{
						if (GetDlgItemText (hDlg, IDC_EDIT3, buf, 64) != 0)
						{
							int i;
							if (sscanf (buf, "%d", &i) == 0)
							{
								MessageBox (NULL, "Number of bytes not recognized.", "Copy", MB_OK | MB_ICONERROR);
								i = -1;
							}
							if (i==-1)
							{
								EndDialog (hDlg, 0);
								return 0;
							}
							iCopyEndOffset = iCopyStartOffset + i;
						}
						break;
					}
				}

				EndDialog (hDlg, 1);
				return TRUE;
			}

		case IDCANCEL:
			EndDialog (hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//-------------------------------------------------------------------
int HexEditorWindow::CMD_new ()
{
	if (m_iFileChanged == TRUE)
	{
		if (MessageBox (hwnd, "File was changed! New anyway?", "New", MB_YESNO | MB_ICONQUESTION) == IDNO)
			return 0;
	}
	bFileNeverSaved = TRUE;
	bSelected = FALSE;
	bLButtonIsDown = FALSE;
	m_iFileChanged = FALSE;
	bFilestatusChanged = TRUE;
	iVscrollMax = 0;
	iVscrollPos = 0;
	iVscrollInc = 0;
	iHscrollMax = 0;
	iHscrollPos = 0;
	iHscrollInc = 0;
	iCurLine = 0;
	iCurByte = 0;
	iCurNibble = 0;
	bPartialOpen=FALSE;
	// Delete old data.
	DataArray.ClearAll ();
	sprintf (filename, "Untitled");
	update_for_new_datasize ();
	repaint ();
	return 1;
}

//-------------------------------------------------------------------
int HexEditorWindow::CMD_save_as ()
{
	char szFileName[_MAX_PATH];
	char szTitleName[_MAX_FNAME + _MAX_EXT];
	strcpy (szFileName, filename);
	OPENFILENAME ofn;
	ofn.lStructSize = sizeof (OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = "All Files (*.*)\0*.*\0\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrFileTitle = szTitleName;
	ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = NULL;
	ofn.lCustData = 0L;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if (GetSaveFileName (&ofn))
	{
		int filehandle;

		// Check if file already exists first.
		if ((filehandle = _open (szFileName, _O_RDWR|_O_BINARY,_S_IREAD|_S_IWRITE)) != -1)
		{
			// File already exists: ask what is to be done.
			_close (filehandle);

			if( MessageBox (NULL, "A file of that name already exists!\n"
				"Do you want to overwrite it?",
				"Save as",
				MB_YESNO | MB_ICONWARNING ) == IDNO )
			{
				// Don't overwrite.
				return 1;
			}
		}

		if ((filehandle = _open (szFileName, _O_RDWR|_O_CREAT|_O_TRUNC|_O_BINARY,_S_IREAD|_S_IWRITE)) != -1)
		{
			SetCursor (LoadCursor (NULL, IDC_WAIT));
			if ((_write (filehandle, DataArray, DataArray.GetLength ())) != -1)
			{
				// File was saved.
				strcpy (filename, szFileName);
				m_iFileChanged = FALSE;
				bFilestatusChanged = TRUE;
				bFileNeverSaved = FALSE;
				bPartialOpen=FALSE;
				update_MRU();
			}
			else
				MessageBox (NULL, "Could not save file.", "Save as", MB_OK | MB_ICONERROR);
			SetCursor (LoadCursor (NULL, IDC_ARROW));
			_close (filehandle);
		}
		else
			MessageBox (NULL, "Could not save file.", "Save as", MB_OK | MB_ICONERROR);
	}
	repaint ();
	return 1;
}

//-------------------------------------------------------------------
int HexEditorWindow::CMD_save ()
{
	// File is partially loaded => must be saved partially or saved as.
	if (bPartialOpen)
	{
		int filehandle;
		if ((filehandle = _open (filename,_O_RDWR|_O_BINARY,_S_IREAD|_S_IWRITE)) != -1)
		{
			WaitCursor w1;
			if( _lseek( filehandle, iPartialOffset, 0 ) == -1 )
			{
				MessageBox( hwnd, "Could not seek in file.", "Save", MB_OK | MB_ICONERROR );
				_close( filehandle );
				return 0;
			}
			if( _write( filehandle, DataArray, DataArray.GetLength() ) == -1 )
			{
				MessageBox( hwnd, "Could not write data to file.", "Save", MB_OK | MB_ICONERROR );
			}
			_close (filehandle);
			m_iFileChanged = FALSE;
			bFilestatusChanged = TRUE;
		}
		else
		{
			MessageBox (NULL, "Could not save partially opened file.", "Save", MB_OK | MB_ICONERROR );
		}
		repaint ();
		return 1;
	}
	// File was not saved before => name must be chosen.
	if( bFileNeverSaved )
	{
		MessageBox (NULL, "Can't save because file is untitled.\nPlease choose \"Save As...\" from menu", "Save", MB_OK | MB_ICONINFORMATION);
		return 0;
	}
	int filehandle;
	if ((filehandle = _open (filename,_O_RDWR|_O_CREAT|_O_TRUNC|_O_BINARY,_S_IREAD|_S_IWRITE)) != -1)
	{
		SetCursor (LoadCursor (NULL, IDC_WAIT));
		_write (filehandle, DataArray, DataArray.GetLength ());
		_close (filehandle);
		SetCursor (LoadCursor (NULL, IDC_ARROW));
		m_iFileChanged = FALSE;
		bFilestatusChanged = TRUE;
		bPartialOpen=FALSE;
	}
	else
	{
		MessageBox (NULL, "Could not save file.", "Save", MB_OK | MB_ICONERROR);
	}
	repaint ();
	return 1;
}

//-------------------------------------------------------------------
int HexEditorWindow::CMD_open ()
{
	if (m_iFileChanged == TRUE)
	{
		if (MessageBox (hwnd, "File was changed! Open anyway?", "Open", MB_YESNO | MB_ICONQUESTION) == IDNO)
			return 0;
	}
	char szFileName[_MAX_PATH];
	char szTitleName[_MAX_FNAME + _MAX_EXT];
	szFileName[0] = '\0';
	OPENFILENAME ofn;
	ofn.lStructSize = sizeof (OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = "All Files (*.*)\0*.*\0\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrFileTitle = szTitleName;
	ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = NULL;
	ofn.lCustData = 0L;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if (GetOpenFileName (&ofn))
	{
		if (load_file (szFileName))
		{
			iVscrollMax = 0;
			iVscrollPos = 0;
			iVscrollInc = 0;
			iHscrollMax = 0;
			iHscrollPos = 0;
			iHscrollInc = 0;
			iCurLine = 0;
			iCurByte = 0;
			iCurNibble = 0;
			bSelected = FALSE;
			m_iFileChanged = FALSE;
			bFilestatusChanged = TRUE;
			bFileNeverSaved = FALSE;
			RECT r;
			GetClientRect (hwnd, &r);
			SendMessage (hwnd, WM_SIZE, 0, (r.bottom << 16) | r.right);
			InvalidateRect (hwnd, NULL, FALSE);
			UpdateWindow (hwnd);
		}					
	}
	return 1;
}

//-------------------------------------------------------------------
int HexEditorWindow::CMD_select_with_arrowkeys (int key)
{
	if (filename[0] == '\0' || iCurByte>LASTBYTE || iCurByte<0)
		return 0;
	switch (key)
	{
	case IDA_SELECTSTARTOFLINE:
		if (!bSelected)
		{
			bSelected = TRUE;
			iStartOfSelection = iCurByte;
			iEndOfSelection = iCurByte;
		}
		iEndOfSelection=iEndOfSelection/iBytesPerLine*iBytesPerLine;
		break;

	case IDA_SELECTENDOFLINE:
		if (!bSelected)
		{
			bSelected = TRUE;
			iStartOfSelection = iCurByte;
			iEndOfSelection = iCurByte;
		}
		iEndOfSelection=(iEndOfSelection/iBytesPerLine+1)*iBytesPerLine-1;
		if (iEndOfSelection>LASTBYTE)
			iEndOfSelection = LASTBYTE;
		break;

	case IDA_SELECTSTARTOFFILE:
		if (!bSelected)
		{
			bSelected = TRUE;
			iStartOfSelection = iCurByte;
		}
		iEndOfSelection=0;
		break;

	case IDA_SELECTENDOFFILE:
		if (!bSelected)
		{
			bSelected = TRUE;
			iStartOfSelection = iCurByte;
		}
		iEndOfSelection = LASTBYTE;
		break;

	case IDA_SELECTPAGEDOWN:
		if (!bSelected)
		{
			bSelected = TRUE;
			iStartOfSelection = iCurByte;
			iEndOfSelection = iCurByte;
		}
		iEndOfSelection += cyBuffer*iBytesPerLine;
		if (iEndOfSelection>LASTBYTE)
			iEndOfSelection=LASTBYTE;
		break;

	case IDA_SELECTPAGEUP:
		if (!bSelected)
		{
			bSelected = TRUE;
			iStartOfSelection = iCurByte;
			iEndOfSelection = iCurByte;
		}
		iEndOfSelection -= cyBuffer*iBytesPerLine;
		if (iEndOfSelection<0)
			iEndOfSelection=0;
		break;

	default:
		if (!bSelected)
		{
			// No selection yet.
			bSelected = TRUE;
			iStartOfSelection = iCurByte;
			iEndOfSelection = iCurByte;
			adjust_view_for_selection ();
			repaint ();
			return 1;
		}
		switch (key)
		{
		case IDA_SELECTLEFT:
			if (iEndOfSelection > 0)
				iEndOfSelection--;
			break;

		case IDA_SELECTRIGHT:
			if (iEndOfSelection < LASTBYTE)
				iEndOfSelection++;
			break;

		case IDA_SELECTUP:
			if (iEndOfSelection > iBytesPerLine)
				iEndOfSelection -= iBytesPerLine;
			else
				iEndOfSelection=0;
			break;

		case IDA_SELECTDOWN:
			if (iEndOfSelection+iBytesPerLine < LASTBYTE)
				iEndOfSelection += iBytesPerLine;
			else
				iEndOfSelection = LASTBYTE;
			break;

		default:
			break;
		}
	}
	iCurByte = iEndOfSelection;
	adjust_view_for_selection ();
	repaint ();
	return 1;
}

//-------------------------------------------------------------------
void HexEditorWindow::adjust_view_for_selection ()
{
	if( bSelected )
	{
		// EOS above current upper line?
		if( iEndOfSelection / iBytesPerLine < iCurLine )
			// => then set current line to EOS.
			iCurLine = iEndOfSelection / iBytesPerLine;

		// EOS under current last line?
		else if( iEndOfSelection/iBytesPerLine > iCurLine+cyBuffer - 1 )
		{
			// => then EOS is in new last line.
			iCurLine = iEndOfSelection / iBytesPerLine - cyBuffer + 1;
			if( iCurLine < 0 )
				iCurLine = 0;
		}
		adjust_vscrollbar();
	}
}

//-------------------------------------------------------------------
int HexEditorWindow::CMD_view_settings()
{
	TxtEditName = TexteditorName;
	iBPLSetting = iBytesPerLine;
	iAutomaticXAdjust = iAutomaticBPL;
	iOffsetLenSetting = iOffsetLen;
	bUnsignedViewSetting = bUnsignedView;
	bOpenReadOnlySetting = bOpenReadOnly;
	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_VIEWSETTINGSDIALOG), hwnd, (DLGPROC) ViewSettingsDlgProc))
	{
		TexteditorName = TxtEditName;
		if (iOffsetLen>0 && iOffsetLen<=8)
			iOffsetLen = iOffsetLenSetting;
		iBytesPerLine = iBPLSetting;
		iAutomaticBPL = iAutomaticXAdjust;
		bUnsignedView = bUnsignedViewSetting;
		bOpenReadOnly = bOpenReadOnlySetting;
		save_ini_data ();
		update_for_new_datasize ();
	}
	return 1;
}

//-------------------------------------------------------------------
BOOL CALLBACK ViewSettingsDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			char buf[32];
			sprintf (buf, "%d", iBPLSetting);
			SetWindowText (GetDlgItem (hDlg, IDC_EDIT1), buf);
			sprintf (buf, "%d", iOffsetLenSetting);
			SetWindowText (GetDlgItem (hDlg, IDC_EDIT2), buf);
			CheckDlgButton (hDlg, IDC_CHECK1, iAutomaticXAdjust);
			SetFocus (GetDlgItem (hDlg, IDC_EDIT1));
			if( bUnsignedViewSetting )
				CheckDlgButton( hDlg, IDC_RADIO1, BST_CHECKED );
			else
				CheckDlgButton( hDlg, IDC_RADIO2, BST_CHECKED );
			
			if( bOpenReadOnlySetting )
				CheckDlgButton( hDlg, IDC_CHECK5, BST_CHECKED );
			else
				CheckDlgButton( hDlg, IDC_CHECK5, BST_UNCHECKED );
			SetWindowText( GetDlgItem( hDlg, IDC_EDIT3 ), TxtEditName );
			SetWindowText( GetDlgItem( hDlg, IDC_EDIT4 ), BrowserName );
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				char buf[512];
				int i=-1;
				if (GetDlgItemText (hDlg, IDC_EDIT1, buf, 512) != 0)
				{
					if (sscanf (buf, "%d", &i) == 0)
					{
						MessageBox (NULL, "Number of bytes not recognized.", "View Settings", MB_OK | MB_ICONERROR);
						i = -1;
					}
				}
				if (i==-1)
				{
					EndDialog (hDlg, 0);
					return 0;
				}
				iBPLSetting = i;
				if (GetDlgItemText (hDlg, IDC_EDIT2, buf, 512) != 0)
				{
					if (sscanf (buf, "%d", &i) == 0)
					{
						MessageBox (NULL, "Length of offset not recognized.", "View Settings", MB_OK | MB_ICONERROR);
						i = -1;
					}
				}
				if (i==-1)
				{
					EndDialog (hDlg, 0);
					return 0;
				}
				iOffsetLenSetting = i;
				// Get the text editor path and name.
				if( GetDlgItemText( hDlg, IDC_EDIT3, buf, 512 ) != 0 )
				{
					TxtEditName.SetToString( buf );
				}
				else
				{
					MessageBox( NULL, "Field for text editor name was empty: name not changed.", "View settings", MB_OK | MB_ICONERROR );
				}
				// Get the Browser path and name.
				if( GetDlgItemText( hDlg, IDC_EDIT4, buf, 512 ) != 0 )
				{
					BrowserName = buf;
				}
				else
				{
					MessageBox( NULL, "Field for Internet Browser was empty: name not changed.", "View settings", MB_OK | MB_ICONERROR );
				}
				if (iBPLSetting < 1)
					iBPLSetting = 1;
				iAutomaticXAdjust = IsDlgButtonChecked (hDlg, IDC_CHECK1);
				if (IsDlgButtonChecked (hDlg, IDC_RADIO1))
					bUnsignedViewSetting = TRUE;
				else
					bUnsignedViewSetting = FALSE;
				
				if( IsDlgButtonChecked( hDlg, IDC_CHECK5 ) )
					bOpenReadOnlySetting = TRUE;
				else
					bOpenReadOnlySetting = FALSE;
				EndDialog (hDlg, 1);
				return TRUE;
			}
	
		case IDCANCEL:
			EndDialog (hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//-------------------------------------------------------------------
// Change the color indicated by pColor.
int HexEditorWindow::CMD_color_settings (COLORREF* pColor)
{
	CHOOSECOLOR cc;
	COLORREF crCustColors[16];
	cc.lStructSize = sizeof (CHOOSECOLOR);
	cc.hwndOwner = hwnd;
	cc.hInstance = NULL;
	cc.rgbResult = *pColor;
	cc.lpCustColors = crCustColors;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	cc.lCustData = 0L;
	cc.lpfnHook;
	cc.lpTemplateName = NULL;
	if (ChooseColor (&cc))
	{
		*pColor = cc.rgbResult;
		save_ini_data ();
		update_for_new_datasize ();
		repaint ();
	}
	return 0;
}

//-------------------------------------------------------------------
int HexEditorWindow::read_ini_data ()
{
	// Is there any data for frhed in the registry?
	HKEY key1;

	char keyname[64];
	sprintf( keyname, "Software\\frhed\\v1.0." SUB_RELEASE_NO "\\%d", g_iInstCount );
	LONG res = RegOpenKeyEx( HKEY_CURRENT_USER,
		keyname,
		0,
		KEY_ALL_ACCESS,
		&key1 );
	if( res == ERROR_SUCCESS )
	{
		// There is data: read it.
		DWORD datasize = sizeof( int );
		LONG res;
		res = RegQueryValueEx( key1, "iTextColorValue", NULL, NULL, (BYTE*) &iTextColorValue, &datasize );
		res = RegQueryValueEx( key1, "iBkColorValue", NULL, NULL, (BYTE*) &iBkColorValue, &datasize );
		res = RegQueryValueEx( key1, "iSepColorValue", NULL, NULL, (BYTE*) &iSepColorValue, &datasize );
		res = RegQueryValueEx( key1, "iSelTextColorValue", NULL, NULL, (BYTE*) &iSelTextColorValue, &datasize );
		res = RegQueryValueEx( key1, "iSelBkColorValue", NULL, NULL, (BYTE*) &iSelBkColorValue, &datasize );
		res = RegQueryValueEx( key1, "iBmkColor", NULL, NULL, (BYTE*) &iBmkColor, &datasize );

		res = RegQueryValueEx( key1, "iAutomaticBPL", NULL, NULL, (BYTE*) &iAutomaticBPL, &datasize );
		res = RegQueryValueEx( key1, "iBytesPerLine", NULL, NULL, (BYTE*) &iBytesPerLine, &datasize );
		res = RegQueryValueEx( key1, "iOffsetLen", NULL, NULL, (BYTE*) &iOffsetLen, &datasize );
		res = RegQueryValueEx( key1, "iCharacterSet", NULL, NULL, (BYTE*) &iCharacterSet, &datasize );
		res = RegQueryValueEx( key1, "iFontSize", NULL, NULL, (BYTE*) &iFontSize, &datasize );
		res = RegQueryValueEx( key1, "bOpenReadOnly", NULL, NULL, (BYTE*) &bOpenReadOnly, &datasize );

		char szPath[ _MAX_PATH + 1 ];
		datasize = _MAX_PATH + 1;
		res = RegQueryValueEx( key1, "TexteditorName", NULL, NULL, (BYTE*) &szPath, &datasize );
		TexteditorName = szPath;

		datasize = _MAX_PATH + 1;
		res = RegQueryValueEx( key1, "BrowserName", NULL, NULL, (BYTE*) &szPath, &datasize );
		BrowserName = szPath;

		res = RegQueryValueEx( key1, "iWindowShowCmd", NULL, NULL, (BYTE*) &iWindowShowCmd, &datasize );
		res = RegQueryValueEx( key1, "iWindowX", NULL, NULL, (BYTE*) &iWindowX, &datasize );
		res = RegQueryValueEx( key1, "iWindowY", NULL, NULL, (BYTE*) &iWindowY, &datasize );
		res = RegQueryValueEx( key1, "iWindowWidth", NULL, NULL, (BYTE*) &iWindowWidth, &datasize );
		res = RegQueryValueEx( key1, "iWindowHeight", NULL, NULL, (BYTE*) &iWindowHeight, &datasize );
		
		res = RegQueryValueEx( key1, "iMRU_count", NULL, NULL, (BYTE*) &iMRU_count, &datasize );
		int i;
		char fname[64];
		for( i = 1; i <= MRUMAX; i++ )
		{
			sprintf( fname, "MRU_File%d", i );
			datasize = _MAX_PATH + 1;
			res = RegQueryValueEx( key1, fname, NULL, NULL, (BYTE*) &szPath, &datasize );
			strcpy( &( strMRU[i-1][0] ), szPath );
		}

		// if( res != ERROR_SUCCESS )
		//	MessageBox( NULL, "Could not read value", "frhed", MB_OK );

		// Close registry.
		RegCloseKey( key1 );
	}
	else
	{
		// There is no data. Write with default values.
		MessageBox( NULL, "Frhed is being started for the first time\n"
			"and will be attempting to write to the registry.", "Initialize registry", MB_OK );
		save_ini_data();
	}
	return 0;
}

//-------------------------------------------------------------------
int HexEditorWindow::save_ini_data ()
{
	HKEY key1;
	
	char keyname[64];
	sprintf( keyname, "Software\\frhed\\v1.0." SUB_RELEASE_NO "\\%d", g_iInstCount );
	
	LONG res = RegCreateKey( HKEY_CURRENT_USER, keyname, &key1 );

	if( res == ERROR_SUCCESS )
	{
		RegSetValueEx( key1, "iTextColorValue", 0, REG_DWORD, (CONST BYTE*) &iTextColorValue, sizeof( int ) );
		RegSetValueEx( key1, "iBkColorValue", 0, REG_DWORD, (CONST BYTE*) &iBkColorValue, sizeof( int ) );
		RegSetValueEx( key1, "iSepColorValue", 0, REG_DWORD, (CONST BYTE*) &iSepColorValue, sizeof( int ) );
		RegSetValueEx( key1, "iSelTextColorValue", 0, REG_DWORD, (CONST BYTE*) &iSelTextColorValue, sizeof( int ) );
		RegSetValueEx( key1, "iSelBkColorValue", 0, REG_DWORD, (CONST BYTE*) &iSelBkColorValue, sizeof( int ) );
		RegSetValueEx( key1, "iBmkColor", 0, REG_DWORD, (CONST BYTE*) &iBmkColor, sizeof( int ) );

		RegSetValueEx( key1, "iAutomaticBPL", 0, REG_DWORD, (CONST BYTE*) &iAutomaticBPL, sizeof( int ) );
		RegSetValueEx( key1, "iBytesPerLine", 0, REG_DWORD, (CONST BYTE*) &iBytesPerLine, sizeof( int ) );
		RegSetValueEx( key1, "iOffsetLen", 0, REG_DWORD, (CONST BYTE*) &iOffsetLen, sizeof( int ) );
		RegSetValueEx( key1, "iCharacterSet", 0, REG_DWORD, (CONST BYTE*) &iCharacterSet, sizeof( int ) );
		RegSetValueEx( key1, "iFontSize", 0, REG_DWORD, (CONST BYTE*) &iFontSize, sizeof( int ) );
		RegSetValueEx( key1, "bOpenReadOnly", 0, REG_DWORD, (CONST BYTE*) &bOpenReadOnly, sizeof( int ) );

		RegSetValueEx( key1, "TexteditorName", 0, REG_SZ, (CONST BYTE*) (char*) TexteditorName, TexteditorName.StrLen() + 1 );
		RegSetValueEx( key1, "BrowserName", 0, REG_SZ, (CONST BYTE*) (char*) BrowserName, BrowserName.StrLen() + 1 );

		RegSetValueEx( key1, "iWindowShowCmd", 0, REG_DWORD, (CONST BYTE*) &iWindowShowCmd, sizeof( int ) );
		RegSetValueEx( key1, "iWindowX", 0, REG_DWORD, (CONST BYTE*) &iWindowX, sizeof( int ) );
		RegSetValueEx( key1, "iWindowY", 0, REG_DWORD, (CONST BYTE*) &iWindowY, sizeof( int ) );
		RegSetValueEx( key1, "iWindowWidth", 0, REG_DWORD, (CONST BYTE*) &iWindowWidth, sizeof( int ) );
		RegSetValueEx( key1, "iWindowHeight", 0, REG_DWORD, (CONST BYTE*) &iWindowHeight, sizeof( int ) );

		RegSetValueEx( key1, "iMRU_count", 0, REG_DWORD, (CONST BYTE*) &iMRU_count, sizeof( int ) );
		int i;
		char fname[ 64 ];
		for( i = 1; i <= MRUMAX; i++ )
		{
			sprintf( fname, "MRU_File%d", i );
			RegSetValueEx( key1, fname, 0, REG_SZ, (CONST BYTE*) &(strMRU[i-1][0]), strlen( &(strMRU[i-1][0]) ) + 1 );
		}
		
		// Close registry.
		RegCloseKey( key1 );
	}
	else
	{
		MessageBox( NULL, "Could not save preferences to registry.", "Frhed", MB_OK | MB_ICONERROR );
	}
	return 0;
}

//-------------------------------------------------------------------
BOOL CALLBACK AppendDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			char buf[32];
			sprintf (buf, "%d", iAppendbytes);
			SetWindowText (GetDlgItem (hDlg, IDC_EDIT1), buf);
			SetFocus (GetDlgItem (hDlg, IDC_EDIT1));
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				char buf[64];
				int i=-1;
				if (GetDlgItemText (hDlg, IDC_EDIT1, buf, 64) != 0)
				{
					if (sscanf (buf, "%d", &i) == 0)
					{
						MessageBox (NULL, "Number of bytes to append not recognized.", "Append", MB_OK | MB_ICONERROR);
						i = -1;
					}
				}
				if (i==-1)
				{
					EndDialog (hDlg, 0);
					return 0;
				}
				iAppendbytes = i;
				EndDialog (hDlg, 1);
				return TRUE;
			}
	
		case IDCANCEL:
			EndDialog (hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//-------------------------------------------------------------------
int HexEditorWindow::CMD_edit_append ()
{
	iAppendbytes = 1;
	iManipPos = iCurByte;
	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_APPENDDIALOG), hwnd, (DLGPROC) AppendDlgProc))
	{
		int i, oldupbound = DataArray.GetLength();
		SetCursor (LoadCursor (NULL, IDC_WAIT));
		if (DataArray.SetSize (DataArray.GetSize()+iAppendbytes) == FALSE)
		{
			MessageBox (NULL, "Not enough memory for appending.", "Append", MB_OK | MB_ICONERROR);
			return FALSE;
		}
		DataArray.SetUpperBound(DataArray.GetUpperBound()+iAppendbytes);
		for (i=0; i<iAppendbytes; i++)
			DataArray[oldupbound+i] = 0;
		SetCursor (LoadCursor (NULL, IDC_ARROW));
		m_iFileChanged = TRUE;
		bFilestatusChanged = TRUE;
		update_for_new_datasize ();
		repaint ();
	}
	return 1;
}

//-------------------------------------------------------------------
BOOL CALLBACK BitManipDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			char buf[64];
			sprintf( buf, "Manipulate bits at offset 0x%x=%d", iManipPos, iManipPos );
			SetWindowText( GetDlgItem( hDlg, IDC_STATIC1 ), buf );
			
			sprintf( buf, "Value: 0x%x , %d signed, %u unsigned.", (unsigned char) cBitValue, (signed char) cBitValue, (unsigned char) cBitValue );
			SetWindowText( GetDlgItem( hDlg, IDC_STATIC2 ), buf );

			SetFocus (GetDlgItem (hDlg, IDC_CHECK8));
			if (bitval (&cBitValue,0) != 0)
				CheckDlgButton (hDlg, IDC_CHECK1, BST_CHECKED);
			if (bitval (&cBitValue,1) != 0)
				CheckDlgButton (hDlg, IDC_CHECK2, BST_CHECKED);
			if (bitval (&cBitValue,2) != 0)
				CheckDlgButton (hDlg, IDC_CHECK3, BST_CHECKED);
			if (bitval (&cBitValue,3) != 0)
				CheckDlgButton (hDlg, IDC_CHECK4, BST_CHECKED);
			if (bitval (&cBitValue,4) != 0)
				CheckDlgButton (hDlg, IDC_CHECK5, BST_CHECKED);
			if (bitval (&cBitValue,5) != 0)
				CheckDlgButton (hDlg, IDC_CHECK6, BST_CHECKED);
			if (bitval (&cBitValue,6) != 0)
				CheckDlgButton (hDlg, IDC_CHECK7, BST_CHECKED);
			if (bitval (&cBitValue,7) != 0)
				CheckDlgButton (hDlg, IDC_CHECK8, BST_CHECKED);
			return FALSE;
		}

	case WM_COMMAND:
		{
			cBitValue = 0;
			if (IsDlgButtonChecked (hDlg, IDC_CHECK8) == BST_CHECKED)
				cBitValue += 128;
			if (IsDlgButtonChecked (hDlg, IDC_CHECK7) == BST_CHECKED)
				cBitValue += 64;
			if (IsDlgButtonChecked (hDlg, IDC_CHECK6) == BST_CHECKED)
				cBitValue += 32;
			if (IsDlgButtonChecked (hDlg, IDC_CHECK5) == BST_CHECKED)
				cBitValue += 16;
			if (IsDlgButtonChecked (hDlg, IDC_CHECK4) == BST_CHECKED)
				cBitValue += 8;
			if (IsDlgButtonChecked (hDlg, IDC_CHECK3) == BST_CHECKED)
				cBitValue += 4;
			if (IsDlgButtonChecked (hDlg, IDC_CHECK2) == BST_CHECKED)
				cBitValue += 2;
			if (IsDlgButtonChecked (hDlg, IDC_CHECK1) == BST_CHECKED)
				cBitValue += 1;
			switch (LOWORD (wParam))
			{
			case IDC_CHECK1: case IDC_CHECK2: case IDC_CHECK3: case IDC_CHECK4:
			case IDC_CHECK5: case IDC_CHECK6: case IDC_CHECK7: case IDC_CHECK8:
				{
					char buf[64];
					sprintf( buf, "Value: 0x%x , %d signed, %u unsigned.", (unsigned char) cBitValue, (signed char) cBitValue, (unsigned char) cBitValue );
					SetWindowText( GetDlgItem( hDlg, IDC_STATIC2 ), buf );
				}
				break;

			case IDOK:
				{
					EndDialog (hDlg, 1);
					return TRUE;
				}
		
			case IDCANCEL:
				EndDialog (hDlg, 0);
				return TRUE;
			}
			break;
		}
	}
	return FALSE;
}

//-------------------------------------------------------------------
int HexEditorWindow::CMD_manipulate_bits ()
{
	if (DataArray.GetLength() == 0)
	{
		MessageBox (NULL, "File is empty.", "Manipulate Bits", MB_OK | MB_ICONERROR);
		return 0;
	}
	if (iCurByte<0 || iCurByte>LASTBYTE)
	{
		MessageBox (NULL, "Must choose byte in the file.", "Manipulate Bits", MB_OK | MB_ICONERROR);
		return 0;
	}
	cBitValue = DataArray[iCurByte];
	iManipPos = iCurByte;
	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_MANIPBITSDIALOG), hwnd, (DLGPROC) BitManipDlgProc))
	{
		SetCursor (LoadCursor (NULL, IDC_WAIT));
		SetCursor (LoadCursor (NULL, IDC_ARROW));
		DataArray[iCurByte] = cBitValue;
		m_iFileChanged = TRUE;
		bFilestatusChanged = TRUE;
		repaint ();
	}
	return 1;
}

//-------------------------------------------------------------------
void HexEditorWindow::draw_client_border (HDC hdc)
{
	// Border for client-area.
	int DC_was_allocated = FALSE;
	if (hdc == 0)
	{
		hdc = GetDC (hwnd);
		DC_was_allocated = TRUE;
	}
	RECT r;
	r.left = 0;
	r.top = 0;
	r.right = cxClient;
	r.bottom = cyClient;
	DrawEdge (hdc, &r, EDGE_SUNKEN, BF_RECT);
	if (DC_was_allocated)
	{
		ReleaseDC (hwnd, hdc);
	}
}

//-------------------------------------------------------------------
int HexEditorWindow::CMD_character_set ()
{
	iCharacterSetting = iCharacterSet;
	iFontSizeSetting = iFontSize;
	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_CHARACTERSETDIALOG), hwnd, (DLGPROC) CharacterSetDlgProc))
	{
		iCharacterSet = iCharacterSetting;
		iFontSize = iFontSizeSetting;
		save_ini_data ();
		update_for_new_datasize ();
		kill_focus ();
		set_focus ();
	}
	return 0;
}

//-------------------------------------------------------------------
BOOL CALLBACK CharacterSetDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			char buf[16];
			sprintf (buf, "%d", iFontSizeSetting);
			SetWindowText (GetDlgItem (hDlg, IDC_EDIT1), buf);
			switch (iCharacterSetting)
			{
			case ANSI_FIXED_FONT:
				CheckDlgButton (hDlg, IDC_RADIO1, BST_CHECKED);
				break;

			case OEM_FIXED_FONT:
				CheckDlgButton (hDlg, IDC_RADIO2, BST_CHECKED);
				break;
			}
			SetFocus (GetDlgItem (hDlg, IDC_RADIO1));
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				int i;
				char buf[16];
				if (GetDlgItemText (hDlg, IDC_EDIT1, buf, 16) != 0)
				{
					if (sscanf (buf, "%d", &i) == 0)
					{
						MessageBox (NULL, "Font size not recognized.", "Character set", MB_OK | MB_ICONERROR);
						i = -1;
					}
				}
				if (i != -1)
					iFontSizeSetting = i;
				if (IsDlgButtonChecked (hDlg, IDC_RADIO1) == BST_CHECKED)
					iCharacterSetting = ANSI_FIXED_FONT;
				else
					iCharacterSetting = OEM_FIXED_FONT;
				EndDialog (hDlg, 1);
				return TRUE;
			}
	
		case IDCANCEL:
			EndDialog (hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//-------------------------------------------------------------------
int HexEditorWindow::CMD_on_deletekey ()
{
	iCutMode = BST_UNCHECKED;
	return CMD_edit_cut ();
}

//-------------------------------------------------------------------
int HexEditorWindow::CMD_toggle_insertmode ()
{
	iInsertMode = (iInsertMode) ? FALSE : TRUE;
	set_wnd_title();
	return 1;
}

//-------------------------------------------------------------------
int HexEditorWindow::CMD_on_backspace ()
{
	if (iInsertMode)
	{
		// INSERT-mode: If one exists delete previous byte.
		if (iCurByte > 0)
		{
			if (DataArray.RemoveAt (iCurByte-1, 1) == TRUE)
			{
				iCurByte--;
				update_for_new_datasize ();
			}
			else
				MessageBox (NULL, "Could not remove byte.", "Backspace key", MB_OK | MB_ICONERROR);
		}
	}
	else
	{
		// Only step back.
		if (iCurByte>0)
			iCurByte--;
		repaint ();
	}
	return 1;
}

//-------------------------------------------------------------------
int HexEditorWindow::CMD_select_all ()
{
	if (DataArray.GetLength() <= 0)
		return 0;
	bSelected = TRUE;
	iStartOfSelection = 0;
	iEndOfSelection = DataArray.GetUpperBound ();
	adjust_view_for_selection ();
	repaint ();
	return 1;
}

//-------------------------------------------------------------------
int HexEditorWindow::make_font ()
{
	if (hFont != NULL)
		DeleteObject (hFont);
	HDC hdc = GetDC (hwnd);
	int nHeight = -MulDiv(iFontSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	ReleaseDC (hwnd, hdc);
	int cset;
	if (iCharacterSet==ANSI_FIXED_FONT)
		cset = ANSI_CHARSET;
	else
		cset = OEM_CHARSET;
	hFont = CreateFont (nHeight,0,0,0,0,0,0,0,cset,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FIXED_PITCH | FF_DONTCARE,0);
	return 1;
}

//-------------------------------------------------------------------
int HexEditorWindow::CMD_properties ()
{
	char buf[1000], buf2[500], *pc;
	sprintf (buf, "File name and path: ");
	GetFullPathName (filename, 500, buf2, &pc);
	strcat (buf, buf2);
	if( bPartialOpen )
	{
		sprintf (buf2, "\nPartially opened at offset 0x%x = %d.\n"
			"Number of bytes read: %d = %d kilobytes.\n",
			iPartialOffset, iPartialOffset, DataArray.GetLength(), DataArray.GetLength()/1024);
		strcat (buf, buf2);
	}
	else
	{
		sprintf (buf2, "\nFile size: %d bytes = %d kilobytes.\n", DataArray.GetLength(), DataArray.GetLength()/1024);
		strcat (buf, buf2);
	}
	sprintf (buf2, "\nNumber of hexdump lines: %d.\n", iNumlines);
	strcat (buf, buf2);
	// MessageBox (NULL, buf, "File properties", MB_OK | MB_ICONINFORMATION);
	MessageCopyBox (NULL, buf, "File properties", MB_ICONINFORMATION, hwnd);
	return 1;
}

//-------------------------------------------------------------------
int HexEditorWindow::CMD_compare ()
{
	if (DataArray.GetLength() <= 0)
	{
		MessageBox (NULL, "Current file is empty.", "Compare", MB_OK | MB_ICONERROR);
		return 0;
	}
	char szTitleName[_MAX_FNAME + _MAX_EXT];
	OPENFILENAME ofn;
	ofn.lStructSize = sizeof (OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = "All Files (*.*)\0*.*\0\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrFileTitle = szTitleName;
	ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = "Choose file to compare with";
	ofn.Flags = OFN_HIDEREADONLY;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = NULL;
	ofn.lCustData = 0L;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if (GetOpenFileName (&ofn))
	{
		int filehandle;
		if ((filehandle = _open (szFileName,_O_RDONLY|_O_BINARY,_S_IREAD|_S_IWRITE)) != -1)
		{
			int filelen = _filelength (filehandle);
			iDestFileLen = filelen;
			iSrcFileLen = DataArray.GetLength()-iCurByte;
			char* cmpdata = new char[filelen];
			// Read data.
			if (_read (filehandle, cmpdata, filelen) != -1)
			{
				int diff;
				if ((diff = compare_arrays ((char*) &(DataArray[iCurByte]), DataArray.GetLength()-iCurByte, cmpdata, filelen)) == 0)
				{
					// No difference.
					MessageBox (NULL, "Data matches exactly.", "Compare", MB_OK | MB_ICONINFORMATION);
				}
				else
				{
					// Differences exist.
					intpair* pdiff = new intpair[diff];
					get_diffs ((char*) &(DataArray[iCurByte]), DataArray.GetLength()-iCurByte, cmpdata, filelen, pdiff);
					pdiffChoice = pdiff;
					iDiffNum = diff;
					if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_CHOOSEDIFFDIALOG), hwnd, (DLGPROC) ChooseDiffDlgProc))
					{
						iStartOfSelection = iCurByte+pdiff[iDiffNum].one;
						iEndOfSelection = iCurByte+pdiff[iDiffNum].two;
						bSelected = TRUE;
						iCurByte = iCurByte+pdiff[iDiffNum].one;
						adjust_view_for_selection ();
						repaint ();
					}
					delete [] pdiff;
				}
				_close (filehandle);
				delete [] cmpdata;
				return TRUE;
			}
			else
			{
				delete [] cmpdata;
				_close (filehandle);
				MessageBox (NULL, "Error while reading from file.", "Compare", MB_OK | MB_ICONERROR);
				return FALSE;
			}
		}
	}
	return 1;
}

//-------------------------------------------------------------------
// Return = number of differing bytes.
int HexEditorWindow::compare_arrays (char* ps, int sl, char* pd, int dl)
{
	int i, diff=0, type=1;
	// type=0 means differences, type=1 means equality at last char.
	for (i=0; i<sl && i<dl; i++)
	{
		switch (type)
		{
		case 0:
			if (ps[i]==pd[i])
			{
				diff++;
				type = 1;
			}
			break;

		case 1:
			if (ps[i]!=pd[i])
			{
				type = 0;
			}
			break;
		}
	}
	if (type == 0)
		diff++;
	return diff;
}

//-------------------------------------------------------------------
// Transfer offsets of differences to pdiff.
int HexEditorWindow::get_diffs (char* ps, int sl, char* pd, int dl, intpair* pdiff)
{
	int i, diff=0, type=1;
	// type=0 means differences, type=1 means equality at last char.
	for (i=0; i<sl && i<dl; i++)
	{
		switch (type)
		{
		case 0:
			// Working on area of difference at the moment.
			if (ps[i]==pd[i])
			{
				// Chars equal again.
				pdiff[diff].two = i-1; // Save end of area of difference.
				diff++;
				type = 1;
			}
			// else: chars still different.
			break;

		case 1:
			// Working on area of equality at the moment.
			if (ps[i]!=pd[i])
			{
				// Start of area of difference found.
				pdiff[diff].one = i; // Save start of area of difference.
				type = 0;
			}
			// else: chars still equal.
			break;
		}
	}
	if (type == 0) // If area of difference was at end of file.
	{
		pdiff[diff].two = i-1;
		diff++;
	}
	return diff;
}

//-------------------------------------------------------------------
BOOL CALLBACK ChooseDiffDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			char buf[100];
			int i;
			sprintf (buf, "%d areas of difference found.", iDiffNum);
			SetWindowText (GetDlgItem (hDlg, IDC_STATIC1), buf);
			sprintf (buf, "Remaining loaded data size: %d, size of file on disk: %d.", iSrcFileLen, iDestFileLen);
			SetWindowText (GetDlgItem (hDlg, IDC_STATIC2), buf);
			HWND hwndList = GetDlgItem (hDlg, IDC_LIST1);
			for (i=0; i<iDiffNum; i++)
			{
				sprintf (buf, "%d) 0x%x=%d to 0x%x=%d (%d bytes)", i+1, pdiffChoice[i].one, pdiffChoice[i].one,
					pdiffChoice[i].two, pdiffChoice[i].two, pdiffChoice[i].two-pdiffChoice[i].one+1);
				SendMessage (hwndList, LB_ADDSTRING, 0, (LPARAM) buf);
			}
			SendMessage (hwndList, LB_SETCURSEL, 0, 0);
			SetFocus (GetDlgItem (hDlg, IDC_LIST1));
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
			// By pabs.
			case IDCOPY:
				{//copy button was pressed
					int sumlen=1;//length of buffer initially is 1 for the '\0'
					int len=0;//length of current string
					char*buf=(char*)malloc(1);//buffer = '\0'
					buf[0]=0;//init buffer with '\0'
					char*bt=NULL;//temporary pointer - used so that if realloc returns NULL buf does not lose its value
					HWND hwndList = GetDlgItem (hDlg, IDC_LIST1);//get the list
					int num = SendMessage(hwndList,LB_GETCOUNT,0,0);//get the # items in the list
					for(int i=0;i<num;i++)
					{	//loop num times
						len=SendMessage(hwndList,LB_GETTEXTLEN,i,0)+2;//get sise of next line +2 is for '\r\n' at the end of each line
						sumlen+=len;//add len to the buffer sise
						bt = (char*)realloc(buf,sumlen);//resise buffer
						if(bt!=NULL)
							buf=bt;//realloc succesful overwrite buffer address
						else// if realloc returns NULL(not enough mem to re-alloc buffer)
						break;//exit loop without changing buffer address
					// the -1 is to counteract the initialisation of sumlen
					SendMessage(hwndList,LB_GETTEXT,i,(LPARAM)&buf[sumlen-len-1]);//get the string & add it to the end of the buffer
					strcat(buf,"\r\n");//add '\r\n' to the end of the line - this is '\r\n' rather than '\n' so that it can be pasted into notepad & dos programs
					}//end of the loop
					TextToClipboard(buf);//copy the stuff to the clip ( this function needs work to clean it up )(len=1+strlen)
					free(buf);//free the buffer mem
					return TRUE;//yes we did process the message
				}
				break;


		case IDOK:
			{
				HWND hwndList = GetDlgItem (hDlg, IDC_LIST1);
				iDiffNum = SendMessage (hwndList, LB_GETCURSEL, 0, 0);
				EndDialog (hDlg, 1);
				return TRUE;
			}
	
		case IDCANCEL:
			EndDialog (hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//-------------------------------------------------------------------
char HexEditorWindow::TranslateAnsiToOem (char c)
{
	char sbuf[2], dbuf[2];
	sbuf[0]=c;
	sbuf[1]=0;
	CharToOemBuff (sbuf, dbuf, 1);
	return dbuf[0];
}

//-------------------------------------------------------------------
int HexEditorWindow::CMD_binarymode ()
{
	iBinaryModeSetting = iBinaryMode;
	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_BINARYMODEDIALOG), hwnd, (DLGPROC) BinaryModeDlgProc))
	{
		iBinaryMode = iBinaryModeSetting;
		repaint ();
	}
	return 1;
}

//-------------------------------------------------------------------
BOOL CALLBACK BinaryModeDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			if (iBinaryModeSetting == LITTLEENDIAN_MODE)
				CheckDlgButton (hDlg, IDC_RADIO1, BST_CHECKED);
			else
				CheckDlgButton (hDlg, IDC_RADIO2, BST_CHECKED);
			SetFocus (GetDlgItem (hDlg, IDC_RADIO1));
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				if (IsDlgButtonChecked (hDlg, IDC_RADIO1) == BST_CHECKED)
					iBinaryModeSetting = LITTLEENDIAN_MODE;
				else
					iBinaryModeSetting = BIGENDIAN_MODE;
				EndDialog (hDlg, 1);
				return TRUE;
			}
	
		case IDCANCEL:
			EndDialog (hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//-------------------------------------------------------------------
int HexEditorWindow::timer (WPARAM w, LPARAM l)
{
	if (!bSelected || !bTimerSet)
		return 0;

	if (iMouseY > cyBuffer*cyChar)
	{
		// Lower border reached.
		if (iCurLine < LASTLINE)
			iCurLine++;
		adjust_vscrollbar ();
		repaint ();
	}
	else if (iMouseY < 0)
	{
		// Upper border reached.
		if (iCurLine > 0)
			iCurLine--;
		adjust_vscrollbar ();
		repaint ();
	}
	return 1;
}

//-------------------------------------------------------------------
int HexEditorWindow::CMD_select_block ()
{
	iEndOfSelSetting = iStartOfSelSetting = iCurByte;
	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_SELECT_BLOCK_DIALOG), hwnd, (DLGPROC) SelectBlockDlgProc))
	{
		if (iStartOfSelSetting<0 || iStartOfSelSetting>LASTBYTE || iEndOfSelSetting<0 || iEndOfSelSetting>LASTBYTE)
		{
			MessageBox (NULL, "Offset not valid.", "Select block", MB_OK | MB_ICONERROR);
			return 0;
		}
		iStartOfSelection = iStartOfSelSetting;
		iEndOfSelection = iEndOfSelSetting;
		bSelected = TRUE;
		adjust_view_for_selection ();
		repaint ();
	}
	return 0;
}

//-------------------------------------------------------------------
BOOL CALLBACK SelectBlockDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			char buf[128];
			sprintf (buf, "x%x", iStartOfSelSetting);
			SetWindowText (GetDlgItem (hDlg, IDC_EDIT1), buf);
			sprintf (buf, "x%x", iEndOfSelSetting);
			SetWindowText (GetDlgItem (hDlg, IDC_EDIT2), buf);
			SetFocus (GetDlgItem (hDlg, IDC_EDIT1));
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				char buf[128];
				int i=0;
				if (GetDlgItemText (hDlg, IDC_EDIT1, buf, 128) != 0)
				{
					if (sscanf (buf, "x%x", &iStartOfSelSetting) == 0)
					{
						if (sscanf (buf, "%d", &iStartOfSelSetting) == 0)
						{
							MessageBox (NULL, "Start offset not recognized.", "Select block", MB_OK | MB_ICONERROR);
							i = -1;
						}
					}
				}
				if (i==-1)
				{
					EndDialog (hDlg, 0);
					return 0;
				}
				if (GetDlgItemText (hDlg, IDC_EDIT2, buf, 128) != 0)
				{
					if (sscanf (buf, "x%x", &iEndOfSelSetting) == 0)
						if (sscanf (buf, "%d", &iEndOfSelSetting) == 0)
						{
							MessageBox (NULL, "End offset not recognized.", "Select block", MB_OK | MB_ICONERROR);
							i = -1;
						}
				}
				if (i==-1)
				{
					EndDialog (hDlg, 0);
					return 0;
				}
				EndDialog (hDlg, 1);
				return TRUE;
			}
	
		case IDCANCEL:
			EndDialog (hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//-------------------------------------------------------------------
int HexEditorWindow::update_MRU ()
{
	int i;
	for (i=0; i<iMRU_count; i++)
	{
		if (strcmp (&(strMRU[i][0]), filename)==0)  // Name already in list.
		{
			// Name already in list => Put name at beginning of list.
			char temp[_MAX_PATH+1];
			strcpy (temp, &(strMRU[0][0])); // Save Pos.1.
			// Put No.1 name at pos. of chosen name.
			int j;
			for (j=i; j>0; j--)
				strcpy (&(strMRU[j][0]), &(strMRU[j-1][0]));
			strcpy (&(strMRU[0][0]), filename); // Put chosen name at top.
			break;
		}
	}
	if (i==iMRU_count) // Name not yet in list.
	{
		if (iMRU_count<MRUMAX)
		{
			// Space available in list.
			// Put chosen name at top pos., push rest down.
			int j;
			iMRU_count++;
			for (j=iMRU_count-1; j>0; j--)
				strcpy (&(strMRU[j][0]), &(strMRU[j-1][0]));
			strcpy (&(strMRU[0][0]), filename);
		}
		else
		{
			// No more space in list.
			// Push all down, last pos. will be lost.
			int j;
			for (j=MRUMAX-1; j>0; j--)
				strcpy (&(strMRU[j][0]), &(strMRU[j-1][0]));
			strcpy (&(strMRU[0][0]), filename);
		}
	}
	save_ini_data ();
	return 1;
}

//-------------------------------------------------------------------
// Creates the MRU list and inserts it into the File menu.
int HexEditorWindow::make_MRU_list (HMENU menu)
{
	int i;
//Pabs changed - 14 used to be 9
	while (RemoveMenu (menu, 14, MF_BYPOSITION)==TRUE)
		;
	
	if (iMRU_count>0)
	{
		AppendMenu (menu, MF_SEPARATOR, 0, 0);
		char buf[_MAX_PATH+1+30];
		for (i=0; i<iMRU_count; i++)
		{
			sprintf (buf, "&%d %s", i+1, &(strMRU[i][0]));
			AppendMenu (menu, MF_ENABLED | MF_STRING, IDM_MRU1+i, buf);
		}
	}
	return 1;
}

//-------------------------------------------------------------------
int HexEditorWindow::CMD_MRU_selected (int cmd)
{
	if (cmd-IDM_MRU1+1>iMRU_count)
		return 0;

	if (file_is_loadable (&(strMRU[cmd-IDM_MRU1][0])))
	{
		if (m_iFileChanged == TRUE)
		{
			if (MessageBox (hwnd, "File was changed! Open anyway?", "Open", MB_YESNO | MB_ICONQUESTION) == IDNO)
				return 0;
		}
		if (load_file (&(strMRU[cmd-IDM_MRU1][0])))
		{
			iVscrollMax = 0;
			iVscrollPos = 0;
			iVscrollInc = 0;
			iHscrollMax = 0;
			iHscrollPos = 0;
			iHscrollInc = 0;
			iCurLine = 0;
			iCurByte = 0;
			iCurNibble = 0;
			m_iFileChanged = FALSE;
			bFilestatusChanged = TRUE;
			bFileNeverSaved = FALSE;
//Pabs changed -line insert
			bSelected = FALSE;
//end
			update_MRU ();
			update_for_new_datasize ();
		}
	}
	else
	{
		MessageBox (NULL, "This file could not be accessed and\n"
			"will be removed from the MRU list.", "MRU list", MB_OK | MB_ICONERROR);
		int j;
		for (j=cmd-IDM_MRU1; j<iMRU_count-1; j++)
			strcpy (&(strMRU[j][0]), &(strMRU[j+1][0]));
		iMRU_count--;
		save_ini_data ();
	}
	return 1;
}

//-------------------------------------------------------------------
int HexEditorWindow::CMD_add_bookmark ()
{
	if (DataArray.GetLength()<=0)
	{
		MessageBox (NULL, "Can not set bookmark in empty file.", "Add bookmark", MB_OK | MB_ICONERROR);
		return 0;
	}
	if (iBmkCount<BMKMAX)
	{
		iBmkOffset = iCurByte;
		pcBmkTxt[0]=0;
		if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_ADDBMK_DIALOG), hwnd, (DLGPROC) AddBmkDlgProc))
		{
			if (iBmkOffset>=0 && iBmkOffset<=DataArray.GetLength())
			{
				// Is there already a bookmark on this offset?
				int i;
				for( i = 0; i < iBmkCount; i++ )
				{
					if( pbmkList[ i ].offset == iBmkOffset )
					{
						MessageBox (NULL, "There already is a bookmark on that position.", "Add bookmark", MB_OK | MB_ICONERROR);
						return 0;
					}
				}
				// No bookmark on that position yet.
				pbmkList[iBmkCount].offset = iBmkOffset;
				if (strlen(pcBmkTxt)>0)
				{
					pbmkList[iBmkCount].name = new char[strlen(pcBmkTxt)+1];
					strcpy (pbmkList[iBmkCount].name, pcBmkTxt);
				}
				else
					pbmkList[iBmkCount].name = NULL;
				iBmkCount++;
				repaint();
			}
			else
			{
				MessageBox (NULL, "Can not set bookmark at that position.", "Add bookmark", MB_OK | MB_ICONERROR);
				return 0;
			}
		}
	}
	else
	{
		MessageBox (NULL, "Can not set any more bookmarks.", "Add bookmark", MB_OK | MB_ICONERROR);
	}
	return 0;
}

//-------------------------------------------------------------------
// Insert the bookmark list into the menu.
int HexEditorWindow::make_bookmark_list (HMENU menu)
{
	int i;
	while (RemoveMenu (menu, 3, MF_BYPOSITION)==TRUE)
		;

	if (iBmkCount>0)
	{
		AppendMenu (menu, MF_SEPARATOR, 0, 0);
		char buf[128];
		for (i=0; i<iBmkCount; i++)
		{
			if (pbmkList[i].name == NULL)
				sprintf (buf, "&%d 0x%x", i+1, pbmkList[i].offset);
			else
				sprintf (buf, "&%d 0x%x:%s", i+1, pbmkList[i].offset, pbmkList[i].name);
			if (pbmkList[i].offset <= DataArray.GetLength())
				AppendMenu (menu, MF_ENABLED | MF_STRING, IDM_BOOKMARK1+i, buf);
			else
				AppendMenu (menu, MF_GRAYED | MF_STRING, IDM_BOOKMARK1+i, buf);
		}
	}
	return 1;
}

//-------------------------------------------------------------------
int HexEditorWindow::CMD_goto_bookmark( int cmd )
{
	if( pbmkList[ cmd - IDM_BOOKMARK1 ].offset >= 0 && pbmkList[ cmd - IDM_BOOKMARK1 ].offset <= DataArray.GetLength() )
	{
		iCurByte = pbmkList[ cmd - IDM_BOOKMARK1 ].offset;
		update_for_new_datasize();
		adjust_vscrollbar();
		repaint();
	}
	else
	{
		MessageBox (NULL, "Bookmark points to invalid position.", "Go to bookmark", MB_OK | MB_ICONERROR);
	}
	return 0;
}

//-------------------------------------------------------------------
BOOL CALLBACK AddBmkDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	int i = -1;
	switch (iMsg)
	{
	case WM_INITDIALOG:
		char buf[32];
		sprintf (buf, "x%x", iBmkOffset);
		SetWindowText (GetDlgItem (hDlg, IDC_EDIT1), buf);
		SetFocus (GetDlgItem (hDlg, IDC_EDIT1));
		return FALSE;

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			char buf[16];
			if (GetDlgItemText (hDlg, IDC_EDIT1, buf, 16) != 0)
			{
				if (sscanf (buf, "x%x", &i) == 0)
				{
					if (sscanf (buf, "%d", &i) == 0)
					{
						MessageBox (NULL, "Start offset not recognized.", "Add bookmark", MB_OK | MB_ICONERROR);
						i = -1;
					}
				}
				if (i==-1)
				{
					EndDialog (hDlg, 0);
					return 0;
				}
				iBmkOffset = i;
			}
			GetDlgItemText (hDlg, IDC_EDIT2, pcBmkTxt, BMKTEXTMAX);
			EndDialog (hDlg, 1);
			return TRUE;

		case IDCANCEL:
			EndDialog (hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//-------------------------------------------------------------------
int HexEditorWindow::CMD_remove_bkm ()
{
	if (iBmkCount==0)
	{
		MessageBox (NULL, "No bookmarks to remove.", "Remove bookmark", MB_OK | MB_ICONERROR);
		return 0;
	}
	pbmkRemove = pbmkList;
	iRemBmk = iBmkCount;
	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_REMOVEBMK_DIALOG), hwnd, (DLGPROC) RemoveBmkDlgProc))
	{
		if (pbmkList[iRemBmk].name != NULL)
			delete [] (pbmkList[iRemBmk].name);
		int i;
		for (i=iRemBmk; i<iBmkCount-1; i++)
			pbmkList[i] = pbmkList[i+1];
		iBmkCount--;
		repaint();
	}
	return 1;
}

//-------------------------------------------------------------------
BOOL CALLBACK RemoveBmkDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			char buf[128];
			int i;
			HWND hwndList = GetDlgItem (hDlg, IDC_LIST1);
			for (i=0; i<iRemBmk; i++)
			{
				if (pbmkRemove[i].name == NULL)
					sprintf (buf, "%d) 0x%x", i+1, pbmkRemove[i].offset);
				else
					sprintf (buf, "%d) 0x%x:%s", i+1, pbmkRemove[i].offset, pbmkRemove[i].name);
				SendMessage (hwndList, LB_ADDSTRING, 0, (LPARAM) buf);
			}
			SendMessage (hwndList, LB_SETCURSEL, 0, 0);
			SetFocus (GetDlgItem (hDlg, IDC_LIST1));
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				HWND hwndList = GetDlgItem (hDlg, IDC_LIST1);
				iRemBmk = SendMessage (hwndList, LB_GETCURSEL, 0, 0);
				EndDialog (hDlg, 1);
				return TRUE;
			}
	
		case IDCANCEL:
			EndDialog (hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//-------------------------------------------------------------------
int HexEditorWindow::CMD_clear_all_bmk ()
{
	if( MessageBox( hwnd, "Really clear all bookmarks?", "Clear all bookmarks", MB_YESNO | MB_ICONQUESTION ) == IDYES )
	{
		int i;
		for (i=0; i<iBmkCount; i++)
			if (pbmkList[i].name != NULL)
				delete [] (pbmkList[i].name);
		iBmkCount = 0;
		return 1;
	}
	return 1;
}

//-------------------------------------------------------------------
int HexEditorWindow::CMD_open_partially ()
{
	if (m_iFileChanged == TRUE)
	{
		if (MessageBox (hwnd, "File was changed! Open partially anyway?", "Open", MB_YESNO | MB_ICONQUESTION) == IDNO)
			return 0;
	}
	char szFileName[_MAX_PATH];
	char szTitleName[_MAX_FNAME + _MAX_EXT];
	szFileName[0] = '\0';
	OPENFILENAME ofn;
	ofn.lStructSize = sizeof (OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = "All Files (*.*)\0*.*\0\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrFileTitle = szTitleName;
	ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = NULL;
	ofn.lCustData = 0L;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if (GetOpenFileName (&ofn))
	{
		int filehandle, filelen;
		if ((filehandle = _open (szFileName,_O_RDONLY|_O_BINARY,_S_IREAD|_S_IWRITE)) != -1)
		{
			filelen = _filelength (filehandle);
			_close (filehandle);
		}
		iStartPL = 0;
		iNumBytesPl = filelen;
		iPLFileLen = filelen;
		if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_OPEN_PARTIAL_DIALOG), hwnd, (DLGPROC) OpenPartiallyDlgProc))
		{
			if (iStartPL+iNumBytesPl<=filelen)
			{
				if ((filehandle = _open (szFileName,_O_RDONLY|_O_BINARY,_S_IREAD|_S_IWRITE)) != -1)
				{
					DataArray.ClearAll ();
					if (DataArray.SetSize (iNumBytesPl) == TRUE)
					{
						DataArray.SetUpperBound (iNumBytesPl-1);
						_lseek (filehandle, iStartPL, 0);
						iPartialOffset = iStartPL;
						if (_read (filehandle, DataArray, iNumBytesPl) != -1)
						{
							_close (filehandle);
							// If read-only mode on opening is enabled:
							if( bOpenReadOnly )
								bReadOnly = TRUE;
							else
								bReadOnly = FALSE;
							strcpy (filename, szFileName);
							bFileNeverSaved = FALSE;
							iVscrollMax = 0;
							iVscrollPos = 0;
							iVscrollInc = 0;
							iHscrollMax = 0;
							iHscrollPos = 0;
							iHscrollInc = 0;
							iCurLine = 0;
							iCurByte = 0;
							iCurNibble = 0;
							m_iFileChanged = FALSE;
							bFilestatusChanged = TRUE;
							bFileNeverSaved = FALSE;
							bPartialOpen=TRUE;
							RECT r;
							GetClientRect (hwnd, &r);
							SendMessage (hwnd, WM_SIZE, 0, (r.bottom << 16) | r.right);
							InvalidateRect (hwnd, NULL, FALSE);
							UpdateWindow (hwnd);
							return TRUE;
						}
						else
						{
							_close (filehandle);
							MessageBox (NULL, "Error while reading from file.", "Open partially", MB_OK | MB_ICONERROR);
							return FALSE;
						}
					}
					else
					{
						MessageBox (NULL, "Not enough memory to load file.", "Open partially", MB_OK | MB_ICONERROR);
						return FALSE;
					}
				}
				else
				{
					char buf[500];
					sprintf (buf, "Error code 0x%x occured while opening file %s.", errno, szFileName);
					MessageBox (NULL, buf, "Open partially", MB_OK | MB_ICONERROR);
					return FALSE;
				}
			}
			else
			{
				MessageBox (NULL, "Too many bytes to load.", "Open partially", MB_OK | MB_ICONERROR);
				return 0;
			}
		}
	}
	return 1;
}

//-------------------------------------------------------------------
BOOL CALLBACK OpenPartiallyDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			char buf[128];
			sprintf (buf, "x%x", iStartPL);
			SetWindowText (GetDlgItem (hDlg, IDC_EDIT1), buf);
			sprintf (buf, "Size of file: %d. Load how many bytes:", iPLFileLen);
			SetWindowText (GetDlgItem (hDlg, IDC_STATIC2), buf);
			sprintf (buf, "%d", iNumBytesPl);
			SetWindowText (GetDlgItem (hDlg, IDC_EDIT2), buf);
			SetFocus (GetDlgItem (hDlg, IDC_EDIT1));
			CheckDlgButton( hDlg, IDC_RADIO1, BST_CHECKED );
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				char buf[128];
				int i=0;

				if (GetDlgItemText (hDlg, IDC_EDIT1, buf, 128) != 0)
				{
					if (sscanf (buf, "x%x", &iStartPL) == 0)
					{
						if (sscanf (buf, "%d", &iStartPL) == 0)
						{
							i = -1;
						}
					}
				}
				
				// Only complain about wrong offset in start offset editbox if loading from start.
				if( i==-1 && IsDlgButtonChecked( hDlg, IDC_RADIO1 ) == BST_CHECKED )
				{
					MessageBox (NULL, "Start offset not recognized.", "Open partially", MB_OK | MB_ICONERROR);
					EndDialog (hDlg, 0);
					return 0;
				}
				
				if (GetDlgItemText (hDlg, IDC_EDIT2, buf, 128) != 0)
				{
					if (sscanf (buf, "%d", &iNumBytesPl) == 0)
					{
						i = -1;
					}
				}
				
				if (i==-1)
				{
					MessageBox (NULL, "Number of bytes not recognized.", "Open partially", MB_OK | MB_ICONERROR);
					EndDialog (hDlg, 0);
					return 0;
				}

				if( IsDlgButtonChecked( hDlg, IDC_RADIO2 ) == BST_CHECKED )
				{
					// Load from end of file: arguments must be adapted.
					iStartPL = iPLFileLen - iNumBytesPl;
					if( iStartPL < 0 )
					{
						MessageBox (NULL, "Specified number of bytes to load\ngreater than file size.", "Open partially", MB_OK | MB_ICONERROR);
						EndDialog (hDlg, 0);
						return 0;
					}
				}

				EndDialog (hDlg, 1);
				return TRUE;
			}

		case IDCANCEL:
			EndDialog (hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//-------------------------------------------------------------------
int HexEditorWindow::CMD_fast_paste ()
{
//Pabs changed - line comment out
//	iPasteTimes = 1;
//	iPasteAsText = FALSE;
//end
	// If iInsertMode is TRUE, then insert, don't overwrite.
	if( iInsertMode )
		iPasteMode = 2;
	else
		iPasteMode = 1;
	if (DialogBox (hInstance, MAKEINTRESOURCE (IDD_FASTPASTE_DIALOG), hwnd, (DLGPROC) FastPasteDlgProc))
	{
		switch( iPasteMode )
		{
		case 1: // Overwrite.
			{
				char* pcPastestring;
				// Create findstring.
				int destlen;
				if( iPasteAsText == TRUE )
				{
					destlen = strlen( pcPasteText );
					pcPastestring = new char[ destlen ];
					memcpy( pcPastestring, pcPasteText, destlen );
				}
				else
				{
					destlen = create_bc_translation (&pcPastestring, pcPasteText, strlen (pcPasteText), iCharacterSet, iBinaryMode);
				}
				if (destlen > 0)
				{
					// Enough space for writing?
					// DataArray.GetLength()-iCurByte = number of bytes from including curbyte to end.
//Pabs changed - "(iPasteSkip+destlen)" used to be "destlen"
					if (DataArray.GetLength()-iCurByte >= (iPasteSkip+destlen)*iPasteTimes)
//end
					{
						// Overwrite data.
						SetCursor (LoadCursor (NULL, IDC_WAIT));
						int i,k;
						for (k=0; k<iPasteTimes; k++)
						{
							for (i=0; i<destlen; i++)
							{
//Pabs changed - "(iPasteSkip+destlen)" used to be "destlen"
								DataArray[(iCurByte+k*(iPasteSkip+destlen))+i] = pcPastestring[i];
//end
							}
						}
						SetCursor (LoadCursor (NULL, IDC_ARROW));
						m_iFileChanged = TRUE;
						bFilestatusChanged = TRUE;
					}
					else
					{
						MessageBox (hwnd, "Not enough space for overwriting.", "Paste", MB_OK | MB_ICONERROR);
					}
					delete [] pcPastestring;
				}
				delete [] pcPasteText;
				repaint ();
				break;
			}

		case 2: // Insert.
			{
				char* pcPastestring;
				int destlen;
				if( iPasteAsText == TRUE )
				{
					destlen = strlen( pcPasteText );
					pcPastestring = new char[ destlen ];
					memcpy( pcPastestring, pcPasteText, destlen );
				}
				else
				{
					destlen = create_bc_translation (&pcPastestring, pcPasteText, strlen (pcPasteText), iCharacterSet, iBinaryMode);
				}
				if (destlen > 0)
				{
					// Insert at iCurByte. Byte there will be pushed up.
					SetCursor (LoadCursor (NULL, IDC_WAIT));
//Pabs changed - line insert
					int i, k;
					for( k = 0,i=iCurByte; k < iPasteTimes; k++ ){
						if(!DataArray.InsertAtGrow(iCurByte,(unsigned char*)pcPastestring,0,destlen)){
							delete [] pcPastestring;
							delete [] pcPasteText;
							SetCursor (LoadCursor (NULL, IDC_ARROW));
							MessageBox (hwnd, "Not enough memory for inserting.", "Paste", MB_OK | MB_ICONERROR);
							update_for_new_datasize ();
							return FALSE;
						}
						m_iFileChanged = TRUE;
						bFilestatusChanged = TRUE;
						iCurByte+=destlen+iPasteSkip;
					}
					iCurByte=i;
//end
					// RK: removed old code that pabs commented out.
					m_iFileChanged = TRUE;
					bFilestatusChanged = TRUE;
					update_for_new_datasize ();
					delete [] pcPastestring;
					SetCursor (LoadCursor (NULL, IDC_ARROW));
				}
				delete [] pcPasteText;
				repaint ();
				break;
			}

		default:
			break;
		}
	}
	return 0;
}

//-------------------------------------------------------------------
BOOL CALLBACK FastPasteDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		{
			char buf2[16];
			sprintf (buf2, "%d", iPasteTimes);
			SetWindowText (GetDlgItem (hDlg, IDC_EDIT2), buf2);
//Pabs changed - line insert
			sprintf (buf2, "%d", iPasteSkip);
			SetWindowText (GetDlgItem (hDlg, IDC_EDIT3), buf2);
//end
			// Depending on INS or OVR mode, set the radio button.
			if( iPasteMode == 1 )
				CheckDlgButton (hDlg, IDC_RADIO1, BST_CHECKED);
			else
				CheckDlgButton (hDlg, IDC_RADIO2, BST_CHECKED);
			if( iPasteAsText == TRUE )
				CheckDlgButton (hDlg, IDC_CHECK1, BST_CHECKED);
			SetFocus (GetDlgItem (hDlg, IDC_RADIO1));
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				if (IsDlgButtonChecked (hDlg, IDC_CHECK1) == BST_CHECKED)
					iPasteAsText = TRUE;
				else
					iPasteAsText = FALSE;
				if (IsDlgButtonChecked (hDlg, IDC_RADIO1) == BST_CHECKED)
					iPasteMode = 1;
				else
					iPasteMode = 2;
				char buf[64];
				int i;
				if (GetDlgItemText (hDlg, IDC_EDIT2, buf, 64) != 0)
				{
					if (sscanf (buf, "%d", &i) == 0)
					{
						MessageBox (NULL, "Number of times to paste not recognized.", "Paste", MB_OK | MB_ICONERROR);
						i = -1;
					}
				}
				if (i<=0)
				{
					MessageBox (NULL, "Number of times to paste must be at least 1.", "Paste", MB_OK | MB_ICONERROR);
					EndDialog (hDlg, 0);
					return 0;
				}
				iPasteTimes = i;
//Pabs changed - line insert
				if (GetDlgItemText (hDlg, IDC_EDIT3, buf, 64) != 0)
				{
					if (sscanf (buf, "%d", &i) == 0)
					{
						MessageBox (NULL, "Number of bytes to skip not recognized.", "Paste", MB_OK | MB_ICONERROR);
						EndDialog (hDlg, 0);
						return 0;
					}
				}
				iPasteSkip = i;
//end
				if (OpenClipboard (NULL))
				{
					HGLOBAL hClipMemory = GetClipboardData (CF_TEXT);
					if (hClipMemory != NULL)
					{
						int gsize = GlobalSize (hClipMemory);
						if (gsize > 0)
						{
							char* pClipMemory = (char*) GlobalLock (hClipMemory);
							pcPasteText = new char[gsize];
							memcpy (pcPasteText, pClipMemory, gsize);
						}
						GlobalUnlock (hClipMemory);
					}
					CloseClipboard ();
				}
				EndDialog (hDlg, 1);
				return TRUE;
			}
	
		case IDCANCEL:
			EndDialog (hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//-------------------------------------------------------------------
// Handler for WM_DROPFILES
int HexEditorWindow::dropfiles( HANDLE hDrop )
{
	char lpszFile[ _MAX_PATH+1 ];
	DragQueryFile( (HDROP) hDrop, 0, lpszFile, _MAX_PATH+1 );
	DragFinish( (HDROP) hDrop ); // handle to memory to free
	SetForegroundWindow( hwnd );
	if( m_iFileChanged == TRUE )
	{
		if( MessageBox (hwnd, "File was changed! Open anyway?", "frhed", MB_YESNO | MB_ICONQUESTION) == IDNO )
			return 0;
	}

	char lpszTarget[ MAX_PATH ];
	// Is this a link file?
	HRESULT hres = ResolveIt( hwnd, lpszFile, lpszTarget );
	if( SUCCEEDED( hres ) )
	{
		// Trying to open a link file: decision by user required.
		int ret = MessageBox( hwnd,
			"You are trying to open a link file.\n"
			"Click on Yes if you want to open the file linked to,\n"
			"or click on No if you want to open the link file itself.\n"
			"Choose Cancel if you want to abort opening.",
			"frhed", MB_YESNOCANCEL | MB_ICONQUESTION );
		switch( ret )
		{
		case IDYES:
			if( load_file( lpszTarget ) )
			{
				iVscrollMax = 0;
				iVscrollPos = 0;
				iVscrollInc = 0;
				iHscrollMax = 0;
				iHscrollPos = 0;
				iHscrollInc = 0;
				iCurLine = 0;
				iCurByte = 0;
				iCurNibble = 0;
				m_iFileChanged = FALSE;
				bFilestatusChanged = TRUE;
				update_for_new_datasize();
			}					
			break;

		case IDNO:
			if( load_file( lpszFile ) )
			{
				iVscrollMax = 0;
				iVscrollPos = 0;
				iVscrollInc = 0;
				iHscrollMax = 0;
				iHscrollPos = 0;
				iHscrollInc = 0;
				iCurLine = 0;
				iCurByte = 0;
				iCurNibble = 0;
				m_iFileChanged = FALSE;
				bFilestatusChanged = TRUE;
				update_for_new_datasize();
			}					
			break;

		case IDCANCEL:
			break;
		}
	}
	else
	{
		if( load_file( lpszFile ) )
		{
			iVscrollMax = 0;
			iVscrollPos = 0;
			iVscrollInc = 0;
			iHscrollMax = 0;
			iHscrollPos = 0;
			iHscrollInc = 0;
			iCurLine = 0;
			iCurByte = 0;
			iCurNibble = 0;
			m_iFileChanged = FALSE;
			bFilestatusChanged = TRUE;
			update_for_new_datasize();
		}
	}
	return 1;
}

//-------------------------------------------------------------------
int HexEditorWindow::CMD_apply_template()
{
	if( DataArray.GetLength() == 0 )
	{
		MessageBox( NULL, "File is empty.", "Template error", MB_OK | MB_ICONERROR );
		return FALSE;
	}

	// Get name of template file.
	char szTemplateName[_MAX_PATH];
	char szTitleName[_MAX_FNAME + _MAX_EXT];
	szTemplateName[0] = '\0';
	OPENFILENAME ofn;
	ofn.lStructSize = sizeof (OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = "Template files (*.tpl)\0*.tpl\0\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = szTemplateName;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrFileTitle = szTitleName;
	ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = "Choose template file";
	ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = NULL;
	ofn.lCustData = 0L;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if( GetOpenFileName( &ofn ) )
	{
		// szTemplateName contains name of chosen tpl file.
		apply_template( szTemplateName );
	}
	return TRUE;
}

//-------------------------------------------------------------------
int HexEditorWindow::apply_template( char* pcTemplate )
{
	// Load template file.
	int filehandle;
	if( ( filehandle = _open( pcTemplate, _O_RDONLY|_O_BINARY, _S_IREAD|_S_IWRITE ) ) != -1 )
	{
		int tpl_filelen = _filelength( filehandle );
		if( tpl_filelen > 0 )
		{
			char* pcTpl = new char[ tpl_filelen ];
			if( pcTpl != NULL && _read( filehandle, pcTpl, tpl_filelen ) != -1 )
			{
				// Template file is loaded into pcTpl.
				SimpleArray<char> TplResult;
				TplResult.SetSize( 1, 100 );
				// Print filename and current offset to output.
				TplResult.AppendArray( "File: ", 6 );
				TplResult.AppendArray( filename, strlen( filename ) );
				TplResult.AppendArray( "\xd\xa", 2 );
				TplResult.AppendArray( "Template file: ", 15 );
				TplResult.AppendArray( pcTemplate, strlen( pcTemplate ) );
				TplResult.AppendArray( "\xd\xa", 2 );
				TplResult.AppendArray( "Applied at offset: ", 19 );
				char buf[16];
				sprintf( buf, "%d\xd\xa\xd\xa", iCurByte );
				TplResult.AppendArray( buf, strlen( buf ) );
				// Apply template on file in memory.
				apply_template_on_memory( pcTpl, tpl_filelen, TplResult );
				TplResult.Append( '\0' );
				// Display template data.
				pcTmplText = TplResult;
				DialogBox( hInstance, MAKEINTRESOURCE( IDD_TMPL_RESULT_DIALOG ), hwnd, (DLGPROC) TmplDisplayDlgProc );
			}
			else
				MessageBox( NULL, "Template file could not be loaded.", "Template error", MB_OK | MB_ICONERROR );
			// Delete template data.
			if( pcTpl != NULL )
				delete [] pcTpl;
		}
		else
			MessageBox( NULL, "Template file is empty.", "Template error", MB_OK | MB_ICONERROR );
		_close( filehandle );
	}
	else
	{
		char buf[500];
		sprintf( buf, "Error code 0x%x occured while\nopening template file %s.", errno, pcTemplate );
		MessageBox( NULL, buf, "Template error", MB_OK | MB_ICONERROR );
		return FALSE;
	}
	return TRUE;
}

//-------------------------------------------------------------------
// Applies the template code in pcTpl of length tpl_len on the current file
// from the current offset and outputs the result to the ResultArray.
int HexEditorWindow::apply_template_on_memory( char* pcTpl, int tpl_len, SimpleArray<char>& ResultArray )
{
	// Use source code in pcTpl to decipher data in file.
	int index = 0, fpos = iCurByte;
	// While there is still code left...
	while( index < tpl_len )
	{
		// Read in the var type.
		if( ignore_non_code( pcTpl, tpl_len, index ) == TRUE )
		{
			// index now points to first code character.
			// Get var type.
			char cmd[ TPL_TYPE_MAXLEN ]; // This holds the variable type, like byte or word.
			if( read_tpl_token( pcTpl, tpl_len, index, cmd ) == TRUE )
			{
				// cmd holds 0-terminated var type, index set to position of first space-
				// character after the type. Now test if valid type was given.
				//---- type BYTE ---------------------------------
				if( strcmp( cmd, "BYTE" ) == 0 || strcmp( cmd, "char" ) == 0 )
				{
					// This is a byte/char.
					if( ignore_non_code( pcTpl, tpl_len, index ) == TRUE )
					{
						// Enough space for a byte?
						if( DataArray.GetLength() - fpos >= 1 )
						{
							// Read var name.
							char name[ TPL_NAME_MAXLEN ];
							// index is set to a non-space character by last call to ignore_non_code.
							// Therefore the variable name can be read into buffer name.
							read_tpl_token( pcTpl, tpl_len, index, name );
							// Write variable type and name to output.
							ResultArray.AppendArray( cmd, strlen(cmd) );
							ResultArray.Append( ' ' );
							ResultArray.AppendArray( name, strlen(name) );
							// Write value to output.
							char buf[ TPL_NAME_MAXLEN + 200];
							if( DataArray[fpos] != 0 )
								sprintf( buf, " = %d (signed) = %u (unsigned) = 0x%x = \'%c\'\xd\xa", (int) (signed char) DataArray[fpos], DataArray[fpos], DataArray[fpos], DataArray[fpos] );
							else
								sprintf( buf, " = %d (signed) = %u (unsigned) = 0x%x\xd\xa", (int) (signed char) DataArray[fpos], DataArray[fpos], DataArray[fpos] );
							ResultArray.AppendArray( buf, strlen(buf) );
							// Increase pointer for next variable.
							fpos += 1;
						}
						else
						{
							ResultArray.AppendArray( "ERROR: not enough space for byte-size datum.", 45 );
							return FALSE;
						}
					}
					else
					{
						// No non-spaces after variable type up to end of array, so
						// no space for variable name.
						ResultArray.AppendArray( "ERROR: missing variable name.", 29 );
						return FALSE;
					}
				}
				else if( strcmp( cmd, "WORD" ) == 0 || strcmp( cmd, "short" ) == 0 )
				{
					// This is a word.
					if( ignore_non_code( pcTpl, tpl_len, index ) == TRUE )
					{
						// Enough space for a word?
						if( DataArray.GetLength() - fpos >= 2 )
						{
							// Read var name.
							char name[ TPL_NAME_MAXLEN ];
							read_tpl_token( pcTpl, tpl_len, index, name );
							// Write variable type to output.
							ResultArray.AppendArray( cmd, strlen(cmd) );
							ResultArray.Append( ' ' );
							// Write variable name to output.
							ResultArray.AppendArray( name, strlen(name) );
							WORD wd;
							// Get value depending on binary mode.
							if( iBinaryMode == LITTLEENDIAN_MODE )
							{
								wd = *( (WORD*)( &DataArray[ fpos ] ) );
							}
							else // BIGENDIAN_MODE
							{
								int i;
								for( i=0; i<2; i++ )
									((char*)&wd)[ i ] = DataArray[ fpos + 1 - i ];
							}
							char buf[ TPL_NAME_MAXLEN + 200 ];
							sprintf( buf, " = %d (signed) = %u (unsigned) = 0x%x\xd\xa", (int) (signed short) wd, wd, wd );
							ResultArray.AppendArray( buf, strlen(buf) );
							fpos += 2;
						}
						else
						{
							ResultArray.AppendArray( "ERROR: not enough space for WORD.", 34 );
							return FALSE;
						}
					}
					else
					{
						ResultArray.AppendArray( "ERROR: missing variable name.", 29 );
						return FALSE; // No more code: missing name.
					}
				}
				else if( strcmp( cmd, "DWORD" ) == 0 || strcmp( cmd, "int" ) == 0 ||
					strcmp( cmd, "long" ) == 0 || strcmp( cmd, "LONG" ) == 0 )
				{
					// This is a longword.
					if( ignore_non_code( pcTpl, tpl_len, index ) == TRUE )
					{
						// Enough space for a longword?
						if( DataArray.GetLength() - fpos >= 4 )
						{
							// Read var name.
							char name[ TPL_NAME_MAXLEN ];
							read_tpl_token( pcTpl, tpl_len, index, name );
							// Write variable type to output.
							ResultArray.AppendArray( cmd, strlen(cmd) );
							ResultArray.Append( ' ' );
							// Write variable name to output.
							ResultArray.AppendArray( name, strlen(name) );
							DWORD dw;
							// Get value depending on binary mode.
							if( iBinaryMode == LITTLEENDIAN_MODE )
							{
								dw = *( (DWORD*)( &DataArray[ fpos ] ) );
							}
							else // BIGENDIAN_MODE
							{
								int i;
								for( i=0; i<4; i++ )
									((char*)&dw)[ i ] = DataArray[ fpos + 3 - i ];
							}
							char buf[ TPL_NAME_MAXLEN + 200 ];
							sprintf( buf, " = %d (signed) = %u (unsigned) = 0x%x\xd\xa", (signed long) dw, (unsigned long) dw, dw );
							ResultArray.AppendArray( buf, strlen(buf) );
							fpos += 4;
						}
						else
						{
							ResultArray.AppendArray( "ERROR: not enough space for DWORD.", 34 );
							return FALSE;
						}
					}
					else
					{
						ResultArray.AppendArray( "ERROR: missing variable name.", 29 );
						return FALSE; // No more code: missing name.
					}
				}
				else if( strcmp( cmd, "float" ) == 0 )
				{
					// This is a float.
					if( ignore_non_code( pcTpl, tpl_len, index ) == TRUE )
					{
						// Enough space for a float?
						if( DataArray.GetLength() - fpos >= 4 )
						{
							// Read var name.
							char name[ TPL_NAME_MAXLEN ];
							read_tpl_token( pcTpl, tpl_len, index, name );
							// Write variable type to output.
							ResultArray.AppendArray( cmd, strlen(cmd) );
							ResultArray.Append( ' ' );
							// Write variable name to output.
							ResultArray.AppendArray( name, strlen(name) );
							float f;
							// Get value depending on binary mode.
							if( iBinaryMode == LITTLEENDIAN_MODE )
							{
								f = *( (float*)( &DataArray[ fpos ] ) );
							}
							else // BIGENDIAN_MODE
							{
								int i;
								for( i=0; i<4; i++ )
									((char*)&f)[ i ] = DataArray[ fpos + 3 - i ];
							}
							char buf[ TPL_NAME_MAXLEN + 200 ];
							sprintf( buf, " = %f = 0x%x\xd\xa", f, (unsigned long) *((int*) &f) );
							ResultArray.AppendArray( buf, strlen(buf) );
							fpos += 4;
						}
						else
						{
							ResultArray.AppendArray( "ERROR: not enough space for float.", 34 );
							return FALSE;
						}
					}
					else
					{
						ResultArray.AppendArray( "ERROR: missing variable name.", 29 );
						return FALSE; // No more code: missing name.
					}
				}
				else if( strcmp( cmd, "double" ) == 0 )
				{
					// This is a double.
					if( ignore_non_code( pcTpl, tpl_len, index ) == TRUE )
					{
						// Enough space for a double?
						if( DataArray.GetLength() - fpos >= 8 )
						{
							// Read var name.
							char name[ TPL_NAME_MAXLEN ];
							read_tpl_token( pcTpl, tpl_len, index, name );
							// Write variable type to output.
							ResultArray.AppendArray( cmd, strlen(cmd) );
							ResultArray.Append( ' ' );
							// Write variable name to output.
							ResultArray.AppendArray( name, strlen(name) );
							double d;
							// Get value depending on binary mode.
							if( iBinaryMode == LITTLEENDIAN_MODE )
							{
								d = *( (double*)( &DataArray[ fpos ] ) );
							}
							else // BIGENDIAN_MODE
							{
								int i;
								for( i=0; i<8; i++ )
									((char*)&d)[ i ] = DataArray[ fpos + 7 - i ];
							}
							char buf[ TPL_NAME_MAXLEN + 200 ];
							sprintf( buf, " = %g\xd\xa", d );
							ResultArray.AppendArray( buf, strlen(buf) );
							fpos += 8;
						}
						else
						{
							ResultArray.AppendArray( "ERROR: not enough space for double.", 35 );
							return FALSE;
						}
					}
					else
					{
						ResultArray.AppendArray( "ERROR: missing variable name.", 29 );
						return FALSE; // No more code: missing name.
					}
				}
				else
				{
					ResultArray.AppendArray( "ERROR: Unknown variable type \"", 30 );
					ResultArray.AppendArray( cmd, strlen( cmd ) );
					ResultArray.Append( '\"' );
					return FALSE;
				}
			}
			else
			{
				// After the type there is only the array end. Therefore
				// no space for a variable name.
				ResultArray.AppendArray( "ERROR: Missing variable name.", 29 );
				return FALSE;
			}
		}
		else
		{
			// No non-spaces up to the end of the array.
			break;
		}
	}
	// No more code left in pcTpl.
	char buf[128];
	sprintf( buf, "\xd\xa-> Length of template = %d bytes.\xd\xa", fpos - iCurByte );
	ResultArray.AppendArray( buf, strlen( buf ) );
	return TRUE;
}

//-------------------------------------------------------------------
// This will set index to the position of the next non-space-character.
// Return is FALSE if there are no non-spaces left up to the end of the array.
int HexEditorWindow::ignore_non_code( char* pcTpl, int tpl_len, int& index )
{
	while( index < tpl_len )
	{
		// If code found, return.
		switch( pcTpl[ index ] )
		{
		case ' ': case '\t': case 0x0d: case 0x0a:
			break;

		default:
			return TRUE;
		}
		index++;
	}
	return FALSE;
}

//-------------------------------------------------------------------
// Writes all non-space characters from index to dest and closes dest
// with a zero-byte. index is set to position of the first space-
// character. Return is false íf there is only the array end after the
// keyword. In that case index is set to tpl_len.
int HexEditorWindow::read_tpl_token( char* pcTpl, int tpl_len, int& index, char* dest )
{
	int i = 0;
	while( index + i < tpl_len )
	{
		switch( pcTpl[ index + i ] )
		{
		case ' ': case '\t': case 0x0d: case 0x0a:
			dest[i] = '\0';
			index += i;
			return TRUE;

		default:
			dest[i] = pcTpl[ index + i ];
		}
		i++;
	}
	dest[i] = '\0';
	index += i;
	return FALSE;
}

//-------------------------------------------------------------------
// TmplDisplayDlgProc
BOOL CALLBACK TmplDisplayDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			SetWindowText( GetDlgItem( hDlg, IDC_EDIT1 ), pcTmplText );
			SetFocus( GetDlgItem( hDlg, IDC_EDIT1 ) );
			return FALSE;
		}

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			{
				EndDialog (hDlg, 1);
				return TRUE;
			}
	
		case IDCANCEL:
			EndDialog (hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//-------------------------------------------------------------------
int HexEditorWindow::CMD_goto()
{
	if( filename[0] == '\0' )
		return 0;
	if( DialogBox( hInstance, MAKEINTRESOURCE(IDD_GOTODIALOG), hwnd, (DLGPROC) GoToDlgProc ) )
	{
		int offset, i = 0, r = 0;
		if( pcGotoDlgBuffer[0] == '+' || pcGotoDlgBuffer[0] == '-' )
		{
			// Relative jump. Read offset from 2nd character on.
			r = 1;
		}

		if( sscanf( &pcGotoDlgBuffer[r], "x%x", &offset ) == 0 )
		{
			// No fields assigned.
			if( sscanf ( &pcGotoDlgBuffer[r], "%d", &offset ) == 0 )
			{
				// No fields assigned: invalid offset.
				i = -1;
			}
		}

		if( i==-1 )
		{
			MessageBox (NULL, "Offset not recognized.", "Go to", MB_OK | MB_ICONERROR);
			return 0;
		}

		if( r == 1 )
		{
			// Relative jump.
			if ( pcGotoDlgBuffer[0] == '-' )
				r = -1;

			if( iCurByte + r * offset >= 0 && iCurByte + r * offset <= LASTBYTE )
			{
				iCurByte = iCurByte + r * offset;
				iCurLine = BYTELINE;
				adjust_vscrollbar();
				repaint();
			}
			else
				MessageBox( hwnd, "Invalid offset.", "Go to", MB_OK | MB_ICONERROR );
		}
		else
		{
			// Absolute jump.
			if( offset >= 0 && offset <= LASTBYTE )
			{
				iCurByte = offset;
				iCurLine = BYTELINE;
				adjust_vscrollbar();
				repaint();
			}
			else
				MessageBox( hwnd, "Invalid offset.", "Go to", MB_OK | MB_ICONERROR );
		}
	}
	return 1;
}

//-------------------------------------------------------------------
// Resolve link files for opening from command line.
// Copied from compiler documentation.
HRESULT ResolveIt( HWND hwnd, LPCSTR lpszLinkFile, LPSTR lpszPath )
{ 
	HRESULT hres; 
	IShellLink* psl; 
	char szGotPath[MAX_PATH]; 
	char szDescription[MAX_PATH]; 
	WIN32_FIND_DATA wfd; 

	*lpszPath = 0; // assume failure 

	// Get a pointer to the IShellLink interface. 
	CoInitialize( NULL );
	hres = CoCreateInstance( CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&psl );
	if (SUCCEEDED(hres))
	{ 
		IPersistFile* ppf; 

		// Get a pointer to the IPersistFile interface. 
		hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf); 
		if (SUCCEEDED(hres))
		{ 
			WORD wsz[MAX_PATH]; 

			// Ensure that the string is Unicode. 
			MultiByteToWideChar(CP_ACP, 0, lpszLinkFile, -1, (LPWSTR)wsz, MAX_PATH); 

			// Load the shortcut. 
			hres = ppf->Load((LPWSTR)wsz, STGM_READ); 
			if (SUCCEEDED(hres))
			{ 
				// Resolve the link. 
				hres = psl->Resolve(hwnd, SLR_ANY_MATCH); 
				if (SUCCEEDED(hres))
				{ 
					// Get the path to the link target. 
					hres = psl->GetPath(szGotPath, MAX_PATH, (WIN32_FIND_DATA *)&wfd, SLGP_SHORTPATH ); 
					if (!SUCCEEDED(hres) )
					{
						// application-defined function 
					}

					// Get the description of the target. 
					hres = psl->GetDescription(szDescription, MAX_PATH); 
					if (!SUCCEEDED(hres)) 
					{
						// HandleErr(hres); 
					}
					lstrcpy(lpszPath, szGotPath); 
				} 
			} 
		// Release the pointer to the IPersistFile interface. 
		ppf->Release(); 
		} 
	// Release the pointer to the IShellLink interface. 
	psl->Release(); 
	}
	else
	{
	}
	CoUninitialize();
	return hres; 
} 

//-------------------------------------------------------------------
void HexEditorWindow::CMD_colors_to_default()
{
	if( MessageBox( NULL, "Really reset colors to default values?", "frhed", MB_YESNO | MB_ICONQUESTION ) == IDYES )
	{
		iBmkColor = RGB( 255, 0, 0 );
		iSelBkColorValue = RGB( 255, 255, 0 );
		iSelTextColorValue = RGB( 0, 0, 0 );
		iTextColorValue = RGB( 0, 0, 0 );
		iBkColorValue = RGB( 255, 255, 255 );
		iSepColorValue = RGB( 192, 192, 192 );
		save_ini_data();
		repaint();
	}
}

//-------------------------------------------------------------------
// Find next occurance of the current findstring.
int HexEditorWindow::CMD_findnext()
{
	// If there is selected data then make it the data to find.
	if( bSelected )
	{
		int sel_start, select_len;
		if( iEndOfSelection < iStartOfSelection )
		{
			sel_start = iEndOfSelection;
			select_len = iStartOfSelection - iEndOfSelection + 1;
		}
		else
		{
			sel_start = iStartOfSelection;
			select_len = iEndOfSelection - iStartOfSelection + 1;
		}

		iFindDlgBufLen = byte_to_BC_destlen( (char*) &DataArray[sel_start], select_len );
		iFindDlgLastLen = iFindDlgBufLen;
		if( pcFindDlgBuffer != NULL )
			delete [] pcFindDlgBuffer;
		pcFindDlgBuffer = new char[iFindDlgBufLen];
		translate_bytes_to_BC( pcFindDlgBuffer, &DataArray[sel_start], select_len );
	}

	// Is there a findstring? (Initmenupopup actually filters this already).
	if( pcFindDlgBuffer != NULL )
	{
		// There is a findstring. Create its translation.
		char* pcFindstring;
		int srclen = strlen( pcFindDlgBuffer );
		int destlen = create_bc_translation( &pcFindstring, pcFindDlgBuffer, srclen, iCharacterSet, iBinaryMode );
		if( destlen > 0 )
		{
			int i;
			char (*cmp) (char);

			if( iFindDlgMatchCase == BST_CHECKED )
				cmp = equal;
			else
				cmp = lower_case;

			SetCursor( LoadCursor( NULL, IDC_WAIT ) );

			i = find_bytes( (char*) &(DataArray[iCurByte + 1]), DataArray.GetLength () - iCurByte - 1, pcFindstring, destlen, 1, cmp );
			if( i != -1 )
				iCurByte += i + 1;

			SetCursor( LoadCursor( NULL, IDC_ARROW ) );
			if( i != -1 )
			{
				/* OLD
				// Caret will be vertically centered if line of found string is not visible.
				if( iCurByte/iBytesPerLine < iCurLine || iCurByte/iBytesPerLine > iCurLine + cyBuffer )
					iCurLine = max( 0, iCurByte/iBytesPerLine-cyBuffer/2 );
				adjust_vscrollbar();
				*/

				bSelected = TRUE;
				iStartOfSelection = iCurByte;
				iEndOfSelection = iCurByte + destlen - 1;
				adjust_view_for_selection();
				repaint();
			}
			else
				MessageBox( hwnd, "Could not find any more occurances.", "Find next", MB_OK | MB_ICONERROR );
			delete [] pcFindstring;
		}
		else
			MessageBox( hwnd, "Findstring is zero bytes long.", "Find next", MB_OK | MB_ICONERROR );
	}
	else
	{
		// There is no findstring.
		MessageBox( hwnd, "String to find not specified.", "Find next", MB_OK | MB_ICONERROR );
	}
	return 0;
}

//-------------------------------------------------------------------
// Find previous occurance of the current findstring.
int HexEditorWindow::CMD_findprev()
{
	// If there is selected data then make it the data to find.
	if( bSelected )
	{
		int sel_start, select_len;
		if( iEndOfSelection < iStartOfSelection )
		{
			sel_start = iEndOfSelection;
			select_len = iStartOfSelection - iEndOfSelection + 1;
		}
		else
		{
			sel_start = iStartOfSelection;
			select_len = iEndOfSelection - iStartOfSelection + 1;
		}

		iFindDlgBufLen = byte_to_BC_destlen( (char*) &DataArray[sel_start], select_len );
		iFindDlgLastLen = iFindDlgBufLen;
		if( pcFindDlgBuffer != NULL )
			delete [] pcFindDlgBuffer;
		pcFindDlgBuffer = new char[iFindDlgBufLen];
		translate_bytes_to_BC( pcFindDlgBuffer, &DataArray[sel_start], select_len );
	}

	// Is there a findstring? (Initmenupopup actually filters this already).
	if( pcFindDlgBuffer != NULL )
	{
		// There is a findstring. Create its translation.
		char* pcFindstring;
		int srclen = strlen( pcFindDlgBuffer );
		int destlen = create_bc_translation( &pcFindstring, pcFindDlgBuffer, srclen, iCharacterSet, iBinaryMode );
		if( destlen > 0 )
		{
			int i;
			char (*cmp) (char);

			if( iFindDlgMatchCase == BST_CHECKED )
				cmp = equal;
			else
				cmp = lower_case;

			SetCursor( LoadCursor( NULL, IDC_WAIT ) );
			{
				// Search the array starting at index 0 to the current byte,
				// plus the findstring-length minus 1. If
				// you are somewhere in the middle of the findstring with the caret
				// and you choose "find previous" you usually want to find the beginning
				// of the findstring in the file.
				i = find_bytes( (char*) &(DataArray[0]),
					min( iCurByte + (destlen - 1), DataArray.GetLength() ),
					pcFindstring, destlen, -1, cmp );
				if (i != -1)
					iCurByte = i;
			}
			SetCursor( LoadCursor( NULL, IDC_ARROW ) );
			if( i != -1 )
			{
				/* OLD
				// Caret will be vertically centered if line of found string is not visible.
				if( iCurByte/iBytesPerLine < iCurLine || iCurByte/iBytesPerLine > iCurLine + cyBuffer )
					iCurLine = max( 0, iCurByte/iBytesPerLine-cyBuffer/2 );
				adjust_vscrollbar();
				*/

				bSelected = TRUE;
				iStartOfSelection = iCurByte;
				iEndOfSelection = iCurByte + destlen - 1;
				adjust_view_for_selection();
				repaint();
			}
			else
				MessageBox( hwnd, "Could not find any more occurances.", "Find previous", MB_OK | MB_ICONERROR );
			delete [] pcFindstring;
		}
		else
			MessageBox( hwnd, "Findstring is zero bytes long.", "Find previous", MB_OK | MB_ICONERROR );
	}
	else
	{
		// There is no findstring.
		MessageBox( hwnd, "String to find not specified.", "Find previous", MB_OK | MB_ICONERROR );
	}
	return 0;
}

//-------------------------------------------------------------------
// Handler for the "Open in text editor" command.
int HexEditorWindow::CMD_summon_text_edit()
{
	if( filename != NULL )
	{
		HINSTANCE hi = ShellExecute( hwnd, "open", TexteditorName, filename, NULL, SW_SHOWNORMAL );
		if( (int) hi <= 32 )
			MessageBox( hwnd, "An error occured when calling the text editor.", "Open in text editor", MB_OK | MB_ICONERROR );
	}
	else
	{
		MessageBox( hwnd, "Filename is NULL.", "Open in text editor", MB_OK | MB_ICONERROR );
	}
	return 0;
}

//-------------------------------------------------------------------
// Process and route all window messages.
inline int HexEditorWindow::OnWndMsg( HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	// First decide if bInsertingHex mode has to be turned off.
	// It might have been set to true in HexEditorWindow::character().
	if( bInsertingHex )
	{
		switch( iMsg )
		{
			case WM_SIZE: case WM_KILLFOCUS: case WM_LBUTTONDOWN:
			case WM_LBUTTONUP: case WM_VSCROLL:
			case WM_HSCROLL: case WM_COMMAND: case WM_DROPFILES:
			case WM_CLOSE: case WM_DESTROY:
				bInsertingHex = FALSE;
				break;

			case WM_KEYDOWN:
				switch( wParam )
				{
				case VK_END:
				case VK_HOME:
				case VK_LEFT:
				case VK_RIGHT:
				case VK_UP:
				case VK_DOWN:
				case VK_PRIOR:
				case VK_NEXT:
					bInsertingHex = FALSE;
					break;

				default:
					break;
				}

			default:
				break;
		}
	}

	switch( iMsg )
	{
	case WM_CREATE:
		at_window_create( hwnd, hMainInstance );
		return 0;

	case WM_SIZE:
		resize_window( LOWORD( lParam ), HIWORD( lParam ) );
		return 0;

	case WM_SETFOCUS:
		set_focus();
		return 0;

	case WM_KILLFOCUS:
		kill_focus();
		return 0;

	case WM_LBUTTONDOWN:
		lbuttondown( LOWORD( lParam ), HIWORD( lParam ) );
		return 0;

	case WM_LBUTTONUP:
		lbuttonup( LOWORD( lParam ), HIWORD( lParam ) );
		return 0;

	case WM_MOUSEMOVE:
		mousemove( (int) (short) LOWORD( lParam ), (int) (short) HIWORD( lParam ) );
		return 0;
	
	case WM_KEYDOWN:
		keydown( wParam );
		return 0;

	case WM_CHAR:
		character( wParam );
		return 0;

	case WM_VSCROLL:
		vscroll( LOWORD( wParam ), HIWORD( wParam ) );
		return 0;

	case WM_HSCROLL:
		hscroll( LOWORD( wParam ), HIWORD( wParam ) );
		return 0;

	case WM_PAINT:
		paint();
		return 0;

	case WM_COMMAND:
		command( LOWORD( wParam ) );
		return 0;

	case WM_INITMENUPOPUP:
		initmenupopup( wParam, lParam );
		return 0;

	case WM_TIMER:
		timer( wParam, lParam );
		return 0;

	case WM_DROPFILES:
		dropfiles( (HANDLE) wParam ); // handle of internal drop structure.
		return 0;

	case WM_CLOSE:
		close();
		return 0;

	case WM_DESTROY:
		destroy_window();
		PostQuitMessage( 0 );
		return 0;
	}
	return DefWindowProc (hwnd, iMsg, wParam, lParam);
}

//-------------------------------------------------------------------
// Install "Open in frhed" command in Windows Explorer context menu.
// [HKEY_CLASSES_ROOT\*\shell\Open in frhed\command]

void HexEditorWindow::CMD_explorersettings()
{
	HKEY key1;
	LONG res = RegOpenKeyEx( HKEY_CLASSES_ROOT,
		"*\\shell\\Open in frhed\\command",
		0,
		KEY_ALL_ACCESS,
		&key1 );
	if( res == ERROR_SUCCESS )
	{
		// The key already exists.
		RegCloseKey( key1 );
		if( MessageBox( NULL,
			"Frhed is already installed in the Explorer context menu.\n"
			"Do you want to remove it from there?",
			"Install in context menu",
			MB_ICONQUESTION | MB_YESNO ) == IDYES )
		{
			// Remove the key.
			if( RegDeleteKey( HKEY_CLASSES_ROOT, "*\\shell\\Open in frhed\\command" ) ==  ERROR_SUCCESS
				&& RegDeleteKey( HKEY_CLASSES_ROOT, "*\\shell\\Open in frhed" ) ==  ERROR_SUCCESS )
			{
				MessageBox( NULL, "Frhed was removed from the Explorer context menu.", "Install in context menu", MB_OK | MB_ICONINFORMATION );
			}
			else
			{
				MessageBox( NULL, "Frhed could not be removed from the Explorer context menu.", "Install in context menu", MB_OK | MB_ICONERROR );
			}
		}
	}
	else
	{
		// The key doesn't exist yet.
		// Ask if it should be created.

		if( MessageBox( NULL,
			"Do you want a \"Open in frhed\" command to appear in the Explorer context menu?",
			"Install in context menu", MB_ICONQUESTION | MB_YESNO ) == IDYES )
		{
			HKEY key1;
			LONG res = RegCreateKey( HKEY_CLASSES_ROOT,
				"*\\shell\\Open in frhed\\command",
				&key1 );
			if( res == ERROR_SUCCESS )
			{
				char exepath[ _MAX_PATH ];
				strcpy( exepath, _pgmptr );
				SimpleString str1 = exepath;
				str1 += " %1";
				RegSetValue( key1, NULL, REG_SZ, str1, str1.StrLen() + 1 );
				MessageBox( NULL, "Frhed is now installed in the Explorer context menu.\n"
					"To remove frhed from the context menu choose this menu item again.",
					"Install in context menu", MB_OK | MB_ICONINFORMATION );
			}
			else
			{
				MessageBox( NULL, "Frhed could not be installed in the Explorer context menu", "Install in context menu", MB_OK | MB_ICONERROR );
			}
		}
	}
}

//-------------------------------------------------------------------
// Create a text representation of an array of bytes and save it in
// a SimpleString object.
int	HexEditorWindow::transl_binary_to_text( SimpleString& dest, char* src, int len )
{
	// How long will the text representation of array of bytes be?
	int destlen = byte_to_BC_destlen( src, len );
	strToReplaceData.SetSize( destlen );
	strToReplaceData.ExpandToSize();
	if( (char*) strToReplaceData != NULL )
	{
		translate_bytes_to_BC( (char*) strToReplaceData, (unsigned char*) src, len );
		return TRUE;
	}
	else
		return FALSE;
}

//-------------------------------------------------------------------
void HexEditorWindow::CMD_replace()
{
	// If there is selected data then make it the data to find.
	if( bSelected )
	{
		int sel_start, select_len;
		if( iEndOfSelection < iStartOfSelection )
		{
			sel_start = iEndOfSelection;
			select_len = iStartOfSelection - iEndOfSelection + 1;
		}
		else
		{
			sel_start = iStartOfSelection;
			select_len = iEndOfSelection - iStartOfSelection + 1;
		}

		if( transl_binary_to_text( strToReplaceData, (char*) &DataArray[sel_start], select_len ) )
		{
		}
		else
		{
			MessageBox( NULL, "Could not use selection as replace target.", "Replace", MB_OK );
			return;
		}
	}

	// Open the dialogue box.
	// While the dialogue box is opened don't mark the current position.
	bDontMarkCurrentPos = TRUE;
	if( DialogBox( hInstance, MAKEINTRESOURCE(IDD_REPLACEDIALOG), hwnd, (DLGPROC) ReplaceDlgProc ) )
	{
	}
	bDontMarkCurrentPos = FALSE;
}

//-------------------------------------------------------------------
BOOL CALLBACK ReplaceDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		if( iPasteAsText )
			CheckDlgButton (hDlg, IDC_USETRANSLATION_CHECK, BST_UNCHECKED);
		else
			CheckDlgButton (hDlg, IDC_USETRANSLATION_CHECK, BST_CHECKED);
		if( strToReplaceData.StrLen() != 0 )
			SetWindowText( GetDlgItem ( hDlg, IDC_TO_REPLACE_EDIT ), strToReplaceData );
		SetFocus( GetDlgItem( hDlg, IDC_TO_REPLACE_EDIT ) );
		return FALSE;

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			EndDialog (hDlg, 0);
			return TRUE;

		case IDCANCEL:
			{
				if( IsDlgButtonChecked( hDlg, IDC_USETRANSLATION_CHECK ) == BST_CHECKED )
					iPasteAsText = FALSE;
				else
					iPasteAsText = TRUE;
				EndDialog (hDlg, 0);
				return TRUE;
			}

		case IDC_FINDPREVIOUS_BUTTON:
			{
				char (*cmp) (char);
				if( IsDlgButtonChecked( hDlg, IDC_MATCHCASE_CHECK ) == BST_CHECKED )
					cmp = equal;
				else
					cmp = lower_case;

				int buflen = SendMessage( GetDlgItem (hDlg, IDC_TO_REPLACE_EDIT), EM_GETLIMITTEXT, 0, 0 );
				char* buf = new char[ buflen ];
				GetDlgItemText( hDlg, IDC_TO_REPLACE_EDIT, buf, buflen );
				strToReplaceData = buf;
				if( ! hexwnd.find_and_select_data( strToReplaceData, -1, TRUE, cmp ) )
					MessageBox( hDlg, "Could not find data.", "Replace/Find backward", MB_OK | MB_ICONERROR );
				delete [] buf;
			}
			break;

		case IDC_FINDNEXT_BUTTON:
			{
				char (*cmp) (char);
				if( IsDlgButtonChecked( hDlg, IDC_MATCHCASE_CHECK ) == BST_CHECKED )
					cmp = equal;
				else
					cmp = lower_case;

				int buflen = SendMessage( GetDlgItem (hDlg, IDC_TO_REPLACE_EDIT), EM_GETLIMITTEXT, 0, 0 );
				char* buf = new char[ buflen ];
				GetDlgItemText( hDlg, IDC_TO_REPLACE_EDIT, buf, buflen );
				strToReplaceData = buf;
				if( ! hexwnd.find_and_select_data( strToReplaceData, 1, TRUE, cmp ) )
				{
					MessageBox( hDlg, "Could not find data.", "Replace/Find forward", MB_OK | MB_ICONERROR );
					break;
				}
				delete [] buf;
			}
			break;

		case IDC_FOLLOCC_BUTTON:
			{
				char (*cmp) (char);
				if( IsDlgButtonChecked( hDlg, IDC_MATCHCASE_CHECK ) == BST_CHECKED )
					cmp = equal;
				else
					cmp = lower_case;

				// Replace all following occurances of the findstring.
				int occ_num = 0;

				int buflen = SendMessage( GetDlgItem (hDlg, IDC_TO_REPLACE_EDIT), EM_GETLIMITTEXT, 0, 0 );
				char* buf = new char[ buflen ];
				if( buf != NULL )
					GetDlgItemText( hDlg, IDC_TO_REPLACE_EDIT, buf, buflen );
				else
				{
					MessageBox( hDlg, "Could not store data to replace.", "Replace/Replace all following", MB_OK | MB_ICONERROR );
					break;
				}
				strToReplaceData = buf;
				delete [] buf;

				buflen = SendMessage( GetDlgItem( hDlg, IDC_REPLACEWITH_EDIT ), EM_GETLIMITTEXT, 0, 0 );
				buf = new char[ buflen ];
				if( buf != NULL )
					GetDlgItemText( hDlg, IDC_REPLACEWITH_EDIT, buf, buflen );
				else
				{
					MessageBox( hDlg, "Could not store data to replace with.", "Replace/Replace all following", MB_OK | MB_ICONERROR );
					break;
				}
				strReplaceWithData = buf;
				delete [] buf;

				if( IsDlgButtonChecked( hDlg, IDC_USETRANSLATION_CHECK ) == BST_CHECKED )
					iPasteAsText = FALSE;
				else
					iPasteAsText = TRUE;

				SetCursor( LoadCursor( NULL, IDC_WAIT ) );
				do
				{
					if( ! hexwnd.find_and_select_data( strToReplaceData, 1, FALSE, cmp ) )
					{
						break;
					}
					occ_num++;
					hexwnd.replace_selected_data( strReplaceWithData, FALSE );
				} while( TRUE );
				SetCursor( LoadCursor( NULL, IDC_ARROW ) );

				char tbuf[32];
				sprintf( tbuf, "%d occurances replaced.", occ_num );
				MessageBox( hDlg, tbuf, "Replace/Replace all following", MB_OK | MB_ICONERROR );

				hexwnd.set_wnd_title();
				hexwnd.adjust_view_for_selection();
				hexwnd.update_for_new_datasize();
				hexwnd.repaint();
			}
			break;

		case IDC_PREVOCC_BUTTON:
			{
				// Replace all previous occurances of the findstring.
				char (*cmp) (char);
				if( IsDlgButtonChecked( hDlg, IDC_MATCHCASE_CHECK ) == BST_CHECKED )
					cmp = equal;
				else
					cmp = lower_case;

				int occ_num = 0;

				int buflen = SendMessage( GetDlgItem (hDlg, IDC_TO_REPLACE_EDIT), EM_GETLIMITTEXT, 0, 0 );
				char* buf = new char[ buflen ];
				if( buf != NULL )
					GetDlgItemText( hDlg, IDC_TO_REPLACE_EDIT, buf, buflen );
				else
				{
					MessageBox( hDlg, "Could not store data to replace.", "Replace/Replace all following", MB_OK | MB_ICONERROR );
					break;
				}
				strToReplaceData = buf;
				delete [] buf;

				buflen = SendMessage( GetDlgItem( hDlg, IDC_REPLACEWITH_EDIT ), EM_GETLIMITTEXT, 0, 0 );
				buf = new char[ buflen ];
				if( buf != NULL )
					GetDlgItemText( hDlg, IDC_REPLACEWITH_EDIT, buf, buflen );
				else
				{
					MessageBox( hDlg, "Could not store data to replace with.", "Replace/Replace all following", MB_OK | MB_ICONERROR );
					break;
				}
				strReplaceWithData = buf;
				delete [] buf;

				if( IsDlgButtonChecked( hDlg, IDC_USETRANSLATION_CHECK ) == BST_CHECKED )
					iPasteAsText = FALSE;
				else
					iPasteAsText = TRUE;

				SetCursor( LoadCursor( NULL, IDC_WAIT ) );
				do
				{
					if( ! hexwnd.find_and_select_data( strToReplaceData, -1, FALSE, cmp ) )
					{
						break;
					}
					occ_num++;
					hexwnd.replace_selected_data( strReplaceWithData, FALSE );
				} while( TRUE );
				SetCursor( LoadCursor( NULL, IDC_ARROW ) );

				char tbuf[32];
				sprintf( tbuf, "%d occurances replaced.", occ_num );
				MessageBox( hDlg, tbuf, "Replace/Replace all following", MB_OK | MB_ICONERROR );

				hexwnd.set_wnd_title();
				hexwnd.adjust_view_for_selection();
				hexwnd.update_for_new_datasize();
				hexwnd.repaint();
			}
			break;

		case IDC_REPLACE_BUTTON:
			{
				if( IsDlgButtonChecked( hDlg, IDC_USETRANSLATION_CHECK ) == BST_CHECKED )
					iPasteAsText = FALSE;
				else
					iPasteAsText = TRUE;
				int buflen = SendMessage( GetDlgItem (hDlg, IDC_TO_REPLACE_EDIT), EM_GETLIMITTEXT, 0, 0 );
				char* buf = new char[ buflen ];
				if( buf != NULL )
				{
					if( GetDlgItemText( hDlg, IDC_TO_REPLACE_EDIT, buf, buflen ) != 0)
					{
						strToReplaceData = buf;
						delete [] buf;
						if( (char*) strToReplaceData != NULL )
						{
							buflen = SendMessage( GetDlgItem( hDlg, IDC_REPLACEWITH_EDIT ), EM_GETLIMITTEXT, 0, 0 );
							buf = new char[ buflen ];
							GetDlgItemText( hDlg, IDC_REPLACEWITH_EDIT, buf, buflen );
							strReplaceWithData = buf;
							delete [] buf;
							hexwnd.replace_selected_data( strReplaceWithData );
						}
						else
						{
							MessageBox( hDlg, "Could not store data to replace.", "Replace", MB_OK | MB_ICONERROR );
						}
						break;
					}
					else
					{
						delete [] buf;
						MessageBox( hDlg, "Could not read out data to replace.", "Replace", MB_OK | MB_ICONERROR );
					}
				}
				else
				{
					MessageBox( hDlg, "Could not allocate memory for data to replace.", "Replace", MB_OK | MB_ICONERROR );
				}
			}
			break;
		}
		break;
	}
	return FALSE;
}

//-------------------------------------------------------------------
int HexEditorWindow::find_and_select_data( SimpleString& finddata, int finddir, int do_repaint, char (*cmp) (char) )
{
	char* tofind;
	int destlen = create_bc_translation( &tofind, 
		(char*) finddata, 
		finddata.StrLen(), 
		iCharacterSet, 
		iBinaryMode 
		);

	int i;
	if( finddir == 1 )
	{
		i = find_bytes( (char*) &(DataArray[iCurByte + 1]),
				DataArray.GetLength() - iCurByte - 1,
				tofind,	destlen, 1, cmp );
		if( i != -1 )
			iCurByte += i + 1;
	}
	else
	{
		i = find_bytes( (char*) &(DataArray[0]),
					min( iCurByte + (destlen - 1), DataArray.GetLength() ),
					tofind, destlen, -1, cmp );
		if( i != -1 )
			iCurByte = i;
	}

	if( i != -1 )
	{
		// NEW: Select found interval.
		bSelected = TRUE;
		iStartOfSelection = iCurByte;
		iEndOfSelection = iCurByte + destlen - 1;
		if( do_repaint )
		{
			adjust_view_for_selection();
			repaint();
		}
	}
	else
	{
		if( tofind != NULL )
			delete [] tofind;
		return FALSE;
	}

	if( tofind != NULL )
		delete [] tofind;
	return TRUE;
}

//-------------------------------------------------------------------
// SimpleString replacedata contains data to replace with.
int HexEditorWindow::replace_selected_data( SimpleString& replacedata, int do_repaint )
{
	if( bSelected )
	{
		if( replacedata.IsEmpty() )
		{
			// Selected data is to be deleted, since replace-with data is empty string.
			if( DataArray.Replace( iGetStartOfSelection(),
				iGetEndOfSelection() - iGetStartOfSelection() + 1,
				NULL, 0	) == TRUE )
			{
				bSelected = FALSE;
				bFilestatusChanged = TRUE;
				m_iFileChanged = TRUE;
				iCurByte = iStartOfSelection;
				if( do_repaint )
				{
					update_for_new_datasize();
					repaint();
				}
				return TRUE;
			}
			else
			{
				MessageBox( NULL, "Could not delete selected data.", "Replace", MB_OK | MB_ICONERROR );
				return FALSE;
			}
		}

		// Replace with non-zero-length data.
		if( iPasteAsText )
		{
			int a = iGetStartOfSelection(), b = iGetEndOfSelection();
			if( DataArray.Replace( iGetStartOfSelection(),
				iGetEndOfSelection() - iGetStartOfSelection() + 1,
				(unsigned char*) (char*) replacedata, replacedata.StrLen()
				) == TRUE )
			{
				iEndOfSelection = iStartOfSelection + replacedata.StrLen() - 1;
				if( do_repaint )
				{
					update_for_new_datasize();
					repaint();
				}
				bFilestatusChanged = TRUE;
				m_iFileChanged = TRUE;
				return TRUE;
			}
			else
			{
				MessageBox( NULL, "Replacing failed.", "Replace", MB_OK | MB_ICONERROR );
				return FALSE;
			}
		}
		else
		{
			// Input string contains special-syntax-coded binary data.
			SimpleArray<char> out;
			if( transl_text_to_binary( replacedata, out ) == TRUE )
			{
				int a = iGetStartOfSelection(), b = iGetEndOfSelection();
				if( DataArray.Replace( iGetStartOfSelection(),
					iGetEndOfSelection() - iGetStartOfSelection() + 1,
					(unsigned char*) (char*) out, out.GetLength()
					) == TRUE )
				{
					bFilestatusChanged = TRUE;
					m_iFileChanged = TRUE;
					iEndOfSelection = iStartOfSelection + out.GetLength() - 1;
					if( do_repaint )
					{
						update_for_new_datasize();
						repaint();
					}
					return TRUE;
				}
				else
				{
					MessageBox( NULL, "Replacing failed.", "Replace", MB_OK | MB_ICONERROR );
					return FALSE;
				}
			}
			else
			{
				MessageBox( NULL, "Could not translate text to binary.", "Replace", MB_OK | MB_ICONERROR );
				return FALSE;
			}
		}
	}
	else
	{
		MessageBox( NULL, "Data to replace must be selected.", "Replace", MB_OK | MB_ICONERROR );
	}
	return FALSE;
}

//-------------------------------------------------------------------
// Translate the text in the string to binary data and store in the array.
int HexEditorWindow::transl_text_to_binary( SimpleString& in, SimpleArray<char>& out )
{
	char* pcOut;
	int destlen = create_bc_translation( &pcOut,
		(char*) in,
		in.StrLen(),
		iCharacterSet,
		iBinaryMode
		);

	if( destlen != 0 )
	{
		out.Adopt( pcOut, destlen - 1, destlen );
		return TRUE;
	}
	else
	{
		// The string was empty.

	}
	return FALSE;
}

//-------------------------------------------------------------------
// Following code by pabs.

//see CMD_fw below
static unsigned char input(const int& index){
	return buf[index];
}

//see CMD_fw below
unsigned char file(const int& index){
	unsigned char x;
	_lseek(FWFile, index, SEEK_SET);
	_read(FWFile,&x,1);
	return x;
}
char aa=0;
//convert a string of hex digits to a string of chars
void hexstring2charstring(){
	// RK: removed definition of variable "a".
	char* pcTemp;//needed so sscanf starts in the right place
	aa=0;
	int i,ii=strlen(pcFWText);
	if(ii%2){//if number of hex digits is odd
		//concatenate them
		for(i=0;i<ii;i++)pcFWText[ii+i]=pcFWText[i];
		pcFWText[ii*2]=0;aa=1;
	}
	for(i=ii=0;pcFWText[i]!='\0';i+=2){
		pcTemp=&pcFWText[i];//update start pos

		// RK: next two lines changed, would crash when compiled with VC++ 4.0.
		/*
		sscanf(pcTemp,"%2x",&a);//get byte from the hexstring
		buf[ii]=a;//store it
		*/
		// Replaced with this line:
		sscanf(pcTemp,"%2x",&buf[ii]);//get byte from the hexstring

		ii++;
	}//for
	buflen=ii;//store length
/*	pcFWText[(aa?ii:ii*2)]=*/buf[ii]='\0';//terminate strings so they are good for use
//	^
//	|
//	access violation so i do it in the dlgproc
}//func

//used to delete non-hex chars after the user pastes into the hexbox
void deletenonhex(HWND hEd){
	GetWindowText(hEd,pcFWText,FW_MAX);
	int ii=0;
	for(int i =0;pcFWText[i]!='\0';i++){
		if(isxdigit(pcFWText[i])){pcFWText[ii]=pcFWText[i];ii++;}
	}
	pcFWText[ii]='\0';
	SetWindowText(hEd,pcFWText);
}

void HexEditorWindow::CMD_revert(){
	if (load_file (filename)){
		iVscrollMax = iVscrollPos = iVscrollInc = iHscrollMax = iHscrollPos = 
		iHscrollInc = iCurLine = iCurByte = iCurNibble = 0;
		m_iFileChanged = FALSE;
		bFilestatusChanged = TRUE;
		bFileNeverSaved = FALSE;
		bSelected=FALSE;
		RECT r;
		GetClientRect (hwnd, &r);
		SendMessage (hwnd, WM_SIZE, 0, (r.bottom << 16) | r.right);
		InvalidateRect (hwnd, NULL, FALSE);
		UpdateWindow (hwnd);
	}					
}

//fill with command
void HexEditorWindow::CMD_fw(){
	tmpstart=iStartOfSelection;tmpend=iEndOfSelection;
	//dlgproc opens file or fills buffer from user input
	if (DialogBox(hInstance, MAKEINTRESOURCE (IDD_FILL_WITH), hwnd, (DLGPROC) FillWithDlgProc)){
		SetCursor(LoadCursor(NULL,IDC_WAIT));
		unsigned char (*fnc)(const int&);
		long i,ii,iimax;
		if (curtyp){//1-file
			fnc=file;
			iimax = FWFilelen;
		}//if
		else {//0-input
			fnc=input;
			iimax=buflen;
		}//else
		if (iStartOfSelection>iEndOfSelection)swap(tmpstart,tmpend);//make sure start<=end
		switch(asstyp){
			case 0:{
				for(ii=0,i=tmpstart;i<=tmpend;i++){
					DataArray[(int)i]=fnc(ii);
					ii++;
					ii%=iimax;
				}//for
			}break;
			case 1:{
				for(ii=0,i=tmpstart;i<=tmpend;i++){
					DataArray[(int)i]|=fnc(ii);
					ii++;
					ii%=iimax;
				}//for
			}break;
			case 2:{
				for(ii=0,i=tmpstart;i<=tmpend;i++){
					DataArray[(int)i]&=fnc(ii);
					ii++;
					ii%=iimax;
				}//for
			}break;
			case 3:{
				for(ii=0,i=tmpstart;i<=tmpend;i++){
					DataArray[(int)i]^=fnc(ii);
					ii++;
					ii%=iimax;
				}//for
			}break;
		}
		if (curtyp) _close(FWFile);//close file
		m_iFileChanged = TRUE;//mark as changed
		bFilestatusChanged = TRUE;
		SetCursor (LoadCursor (NULL, IDC_ARROW));
		repaint ();//you tell me
	}//if dlgbox
}

//hex box msg handler
LRESULT CALLBACK HexProc(HWND hEdit, UINT iMsg, WPARAM wParam, LPARAM lParam){
	char a;
	LONG i=0;
	if(iMsg==WM_CHAR){
		a=(TCHAR) wParam;//only enter chars if they are hex digits or backspace
		// RK: Changed parameter 1, was "(WNDPROC)oldproc", in next 3 CallWindowProc calls.
		if(isxdigit(a)||(a==0x8))return CallWindowProc((WNDPROC)oldproc, hEdit, iMsg, wParam, lParam);
		else {MessageBeep(MB_ICONEXCLAMATION);return 0L;}
	}
	else if (iMsg==WM_PASTE){
		i= CallWindowProc((WNDPROC)oldproc, hEdit, iMsg, wParam, lParam);//paste as usual
		deletenonhex(hEdit);//but delete non-hex chars
	}
	else return CallWindowProc((WNDPROC)oldproc, hEdit, iMsg, wParam, lParam);//use default proc otherwise
	return i;
}

//init stuff
void inittxt(HWND hDlg){
	if (curtyp){//1-file
		SetDlgItemText(hDlg,IDC_SI, "???");
		SetDlgItemText(hDlg,IDC_IFS, "???");
		SetDlgItemText(hDlg,IDC_R, "???");
	}
	else{//0-input
		char bufff[250];
		sprintf(bufff,"%d=0x%x",buflen,buflen);
		SetDlgItemText(hDlg,IDC_SI, bufff);
		if(buflen){
			int tteemmpp=(1+abs(tmpstart-tmpend))/buflen;
			sprintf(bufff,"%d=0x%x",tteemmpp,tteemmpp);SetDlgItemText(hDlg,IDC_IFS, bufff);
			SendDlgItemMessage(hDlg,IDC_IFS,WM_SETFONT,(WPARAM) hfdef,MAKELPARAM(TRUE, 0));
			tteemmpp=(1+abs(tmpstart-tmpend))%buflen;
			sprintf(bufff,"%d=0x%x",tteemmpp,tteemmpp);SetDlgItemText(hDlg,IDC_R, bufff);
		}
		else{
			SetDlgItemText(hDlg,IDC_IFS, "\xa5");//set to infinity symbol
			SendDlgItemMessage(hDlg,IDC_IFS,WM_SETFONT,(WPARAM) hfon,MAKELPARAM(TRUE, 0));
			SetDlgItemText(hDlg,IDC_R, "0=0x0");
		}
	}
}
//fillwithdlgbox msg handler
BOOL CALLBACK FillWithDlgProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam){
	switch (iMsg){
		case WM_INITDIALOG:{
			HWND hEditt=GetDlgItem(hDlg, IDC_HEX);//get the handle to the hex edit box
			SendMessage(hEditt,EM_SETLIMITTEXT, (WPARAM) FW_MAX,0);//limit the amount of text the user can enter
			SetWindowText(hEditt, pcFWText);//init hex text
			SetFocus(hEditt);//give the hex box focus
			EnableWindow(hEditt,!curtyp);
			oldproc = GetWindowLong (hEditt, GWL_WNDPROC);//save the old proc for use in the new proc
			SetWindowLong(hEditt,GWL_WNDPROC,(LONG)HexProc);//override the old proc to be HexProc
			EnableWindow(GetDlgItem(hDlg, IDC_HEXSTAT),!curtyp);

			HWND typ = GetDlgItem(hDlg, IDC_TYPE);
			SendMessage(typ,CB_ADDSTRING ,0,(LPARAM) (LPCTSTR) "Input");
			SendMessage(typ,CB_ADDSTRING ,0,(LPARAM) (LPCTSTR) "File");
			SendMessage(typ,CB_SETCURSEL,(WPARAM)curtyp,0);//set cursel to previous

			//en/disable filename box and browse button
			HWND fn=GetDlgItem(hDlg, IDC_FN);
			SetWindowText(fn,szFWFileName);
			EnableWindow(fn,curtyp);
			EnableWindow(GetDlgItem(hDlg, IDC_BROWSE),curtyp);
			EnableWindow(GetDlgItem(hDlg, IDC_FILESTAT),curtyp);

			//init all the readonly boxes down below
			char bufff[250];
			int tteemmpp=1+abs(tmpstart-tmpend);
			sprintf(bufff,"%d=0x%x",tmpstart,tmpstart);
			SetDlgItemText(hDlg, IDC_STS,bufff);
			sprintf(bufff,"%d=0x%x",tmpend,tmpend);
			SetDlgItemText(hDlg, IDC_ES,bufff);
			sprintf(bufff,"%d=0x%x",tteemmpp,tteemmpp);
			SetDlgItemText(hDlg, IDC_SS,bufff);
			hfdef = (HFONT) SendDlgItemMessage(hDlg,IDC_R,WM_GETFONT,0,0);
			if((hfon = CreateFont(16,0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"Symbol"))==NULL)PostQuitMessage(1);
			inittxt(hDlg);
			switch(asstyp){
				case 0:CheckDlgButton (hDlg, IDC_EQ, BST_CHECKED);break;
				case 1:CheckDlgButton (hDlg, IDC_OR, BST_CHECKED);break;
				case 2:CheckDlgButton (hDlg, IDC_AND, BST_CHECKED);break;
				case 3:CheckDlgButton (hDlg, IDC_XOR, BST_CHECKED);break;
			}
			return 0;//stop the system from setting focus to the control handle in (HWND) wParam because we already set focus above
		}
		break;
		case WM_COMMAND:{
			switch (LOWORD (wParam)){
				case IDOK:{//ok pressed
					int i;
					if(curtyp){//1-file
						GetDlgItemText(hDlg,IDC_FN,szFWFileName,_MAX_PATH);//get file name
						if((FWFile=_open(szFWFileName,_O_RDONLY|_O_BINARY))==-1){//if there is error opening
							MessageBox(hDlg,"Error opening file","Error", MB_OK | MB_ICONERROR);//tell user but don't close dlgbox
							return 1;//didn't process this message
						}//if
						if((FWFilelen=_filelength(FWFile))==0){//if filelen is zero 
							MessageBox (hDlg, "Can't fill a selection with a file of zero size.", "Error", MB_OK | MB_ICONERROR);//tell user but don't close dlgbox
							_close(FWFile);//close file
							return 1;//didn't process this message
						}//if
						else if (FWFilelen==-1){//error returned by _filelength
							MessageBox(hDlg,"Error opening file","Error", MB_OK | MB_ICONERROR);//tell user but don't close dlgbox
							_close(FWFile);//close file
							return 1;//didn't process this message
						}//elseif
					}
					else{//0-input
						if (!buflen){//no hex input
							MessageBox (hDlg, "Can't fill a selection with a string of zero size.", "Error", MB_OK | MB_ICONERROR);//tell user but don't close dlgbox
							return 1;//didn't process this message
						}//if
						if ((i=(GetDlgItemText (hDlg, IDC_HEX, pcFWText, FW_MAX) == 0))||(i==FW_MAX-1)){//error
							MessageBox (hDlg, "Too great a number of bytes to fill with or some other error.", "Error", MB_OK | MB_ICONERROR);//tell user but don't close dlgbox
							return 1;//didn't process this message
						}//if
						hexstring2charstring();//just in case
						pcFWText[(aa?buflen:buflen*2)]='\0';//access violation if i do it in the above function

					}//else
					if(BST_CHECKED == IsDlgButtonChecked(hDlg,IDC_EQ))asstyp=0;
					else if(BST_CHECKED == IsDlgButtonChecked(hDlg,IDC_OR))asstyp=1;
					else if(BST_CHECKED == IsDlgButtonChecked(hDlg,IDC_AND))asstyp=2;
					else if(BST_CHECKED == IsDlgButtonChecked(hDlg,IDC_XOR))asstyp=3;
					DeleteObject(hfon);// won't need till next time
					DeleteObject(hfdef);
					EndDialog (hDlg, 1);//tell CMD_fw to carry out the fill with operation
					return 0;//did process this message
				}//ok
				break;
				case IDCANCEL:{//cancel pressed
					DeleteObject(hfon);// won't need till next time
					EndDialog (hDlg, 0);//tell CMD_fw not to carry out the fill with operation
					return 0;//did process this message
				}//cancel
				break;
				case IDC_TYPE:{
					if(HIWORD(wParam)==CBN_SELCHANGE){//thing to fill selection with changes
						curtyp = (char)SendMessage(GetDlgItem(hDlg, IDC_TYPE),CB_GETCURSEL,0,0);//get cursel
						EnableWindow(GetDlgItem(hDlg, IDC_FN),curtyp);//en/disable fnamebox and browse button
						EnableWindow(GetDlgItem(hDlg, IDC_BROWSE),curtyp);
						EnableWindow(GetDlgItem(hDlg, IDC_FILESTAT),curtyp);
						curtyp=!curtyp;//flip it for the others
						EnableWindow(GetDlgItem(hDlg, IDC_HEX),curtyp);//en/disable hexboxand relateds
						EnableWindow(GetDlgItem(hDlg, IDC_HEXSTAT),curtyp);
						curtyp=!curtyp;//restore original value -not for below -accurate value needed elsewhere
						//set text in boxes down below
						inittxt(hDlg);
					}
				}
				break;
				case IDC_BROWSE:{
					//prepare OPENFILENAME for the file open common dlg box
					szFWFileName[0] = '\0';
					OPENFILENAME ofn;
					ofn.lStructSize = sizeof (OPENFILENAME);
					ofn.hwndOwner = hDlg;
					ofn.hInstance = NULL;
					ofn.lpstrFilter = "All Files (*.*)\0*.*\0\0";
					ofn.lpstrCustomFilter = NULL;
					ofn.nMaxCustFilter = 0;
					ofn.nFilterIndex = 0;
					ofn.lpstrFile = szFWFileName;
					ofn.nMaxFile = _MAX_PATH;
					ofn.lpstrFileTitle = NULL;
					ofn.lpstrInitialDir = NULL;
					ofn.lpstrTitle = NULL;
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
					ofn.lpstrDefExt = NULL;
					ofn.lCustData = 0L;
					ofn.lpfnHook = NULL;
					ofn.lpTemplateName = NULL;
					//show open dlgbox and if file good save name & path in edit box
					if (GetOpenFileName(&ofn))SetDlgItemText(hDlg,IDC_FN,ofn.lpstrFile);
					return 0;//did process this message
				}//browse
				break;
				case IDC_HEX:{
					if(HIWORD(wParam)==EN_UPDATE){//hexedit updated
						GetWindowText(GetDlgItem(hDlg, IDC_HEX), pcFWText, FW_MAX);//gettext
						hexstring2charstring();//convert to char string
						//set text in boxes down below
						inittxt(hDlg);
						return 0;//did process this message
					}//if
					return 1;//didn't process this message
				}//edit1
				break;
				default:return 1;//didn't process this message
			}//switch
		}//wm_command
		break;
	}//switch
	return FALSE;
}//fwdlgproc

void HexEditorWindow::CMD_deletefile(){
	if(IDYES==MessageBox(NULL,"Are you sure you want to delete this file","frhed",MB_ICONERROR|MB_YESNO)){
		if(IDYES==MessageBox(NULL,"Are you really realy sure you want to delete this file","frhed",MB_ICONERROR|MB_YESNO)){
			if(DeleteFile(filename)){m_iFileChanged = FALSE;CMD_new();}//tricky-tricky
			else MessageBox (NULL, "Could not delete file.", "Delete file", MB_OK | MB_ICONERROR);
		}
	}
}

void HexEditorWindow::CMD_insertfile(){
	char szFileName[_MAX_PATH];
	char szTitleName[_MAX_FNAME + _MAX_EXT];
	szFileName[0] = '\0';
	OPENFILENAME ofn;
	ofn.lStructSize = sizeof (OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = "All Files (*.*)\0*.*\0\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrFileTitle = szTitleName;
	ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = NULL;
	ofn.lCustData = 0L;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if (GetOpenFileName (&ofn))
	{
		// RK: don't allow inserting same file we're editing right now.
		if( strcmp( szFileName, filename ) == 0 )
		{
			MessageBox( NULL, "Can't insert file into itself.","Insert file", MB_OK | MB_ICONERROR );
			return;
		}

		int fhandle;
		if((fhandle=_open(szFileName,_O_RDONLY|_O_BINARY))!=-1){
			int inslen;
			if((inslen=_filelength (fhandle))!= -1){
				if(inslen){
					unsigned char* insbuf=new unsigned char[inslen];
					if(insbuf!=NULL){
						if (_read (fhandle, insbuf, inslen)!=-1){
							if(!DataArray.InsertAtGrow(iCurByte,insbuf,0,inslen))MessageBox(NULL,"Could not insert data","Insert file",MB_OK | MB_ICONERROR);
							// RK: changed "true" to TRUE.
							else{
								// RK: Added a call to update_for_new_datasize().
								m_iFileChanged = TRUE;
								bFilestatusChanged = TRUE;
								update_for_new_datasize();
								repaint();
							}
						}
						else MessageBox(NULL,"Could not read data from file","Insert file",MB_OK | MB_ICONERROR);
						delete []insbuf;
					}
					else MessageBox(NULL,"Not enough memory to read data from file","Insert file",MB_OK | MB_ICONERROR);
				}
			}
			// RK: Spelling of "size"!
			else MessageBox(NULL,"Error checking file size","Insert file",MB_OK | MB_ICONERROR);
			_close (fhandle);
		}
		else MessageBox(NULL,"Error opening file","Insert file",MB_OK | MB_ICONERROR);
	}
}

void HexEditorWindow::CMD_saveselas(){
	char szFileName[_MAX_PATH];
	char szTitleName[_MAX_FNAME + _MAX_EXT];
	OPENFILENAME ofn;
	szTitleName[0] =szFileName[0] = '\0';
	ofn.lStructSize = sizeof (OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = "All Files (*.*)\0*.*\0\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrFileTitle = szTitleName;
	ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = NULL;
	ofn.lCustData = 0L;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if (GetSaveFileName (&ofn))
	{
		int filehandle;
		if ((filehandle = _open (szFileName, _O_RDWR|_O_CREAT|_O_TRUNC|_O_BINARY,_S_IREAD|_S_IWRITE)) != -1)
		{
			SetCursor (LoadCursor (NULL, IDC_WAIT));
			if(iStartOfSelection>iEndOfSelection)swap(iStartOfSelection,iEndOfSelection);
			if (!((_write (filehandle, &DataArray[iStartOfSelection], iEndOfSelection-iStartOfSelection+1)) != -1))
				MessageBox (NULL, "Could not save file.", "Save as", MB_OK | MB_ICONERROR);
			SetCursor (LoadCursor (NULL, IDC_ARROW));
			_close (filehandle);
		}
		else
			MessageBox (NULL, "Could not save file.", "Save as", MB_OK | MB_ICONERROR);
	}
	repaint ();
}

void MessageCopyBox(HWND hWnd, LPTSTR lpText, LPCTSTR lpCaption, UINT uType, HWND hwnd)
{
	int len=strlen(lpText);//get the # bytes needed to store the string (not counting '\0')
	//& get where we have to put a '\0' character later
	// RK: Added "?" to end of string.
	strcat(lpText,"\nDo you want the above output to be copied to the clipboard?\n");
	if(IDYES==MessageBox (hWnd, lpText, lpCaption, MB_YESNO | uType))
	{
		//user wants to copy output
		lpText[len]=0;//Remove the line added above
		len++;//1 extra for '\0'
		HGLOBAL hGlobal = GlobalAlloc (GHND, len); //alloc clip mem
		if (hGlobal != NULL)//succesful in above
		{
			SetCursor (LoadCursor (NULL, IDC_WAIT));//tell user to wait
			char* pd = (char*) GlobalLock (hGlobal);// get pointer to clip data
			if(pd)
			{
				//succesfuly got pointer
				strcpy(pd,lpText);//copy lpText into global mem
				GlobalUnlock (hGlobal);//unlock global mem
				if(OpenClipboard(hwnd))
				{
					// open clip
					EmptyClipboard(); //empty clip
					SetClipboardData (CF_TEXT, hGlobal);//copy to clip
					CloseClipboard (); //close clip
				}
				else //failed to open clip
					MessageBox (hWnd,"Cannot get access to clipboard","Copy",MB_OK | MB_ICONERROR);
			}
			else
			{//failed to get pointer to global mem
				GlobalFree(hGlobal);
				MessageBox (hWnd,"Cannot lock clipboard","Copy",MB_OK | MB_ICONERROR);
			}
			SetCursor (LoadCursor (NULL, IDC_ARROW));//user can stop waiting
		}
		else// failed to allocate global mem
			MessageBox (hWnd, "Not enough memory for copying.", "Copy", MB_OK | MB_ICONERROR);
	}
//user doesn't want to copy output
}
