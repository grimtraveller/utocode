""
"@file			zuohaitao.vim
"@brief			gvim config file
"@author		zuohaitao
"@date			2011-08-16
"@version		2.0
"@details
"	Windows OS
"		1. Windows copy this file to $VIM
"		2. Edit $(VIM)/_vimrc, add "source $VIM/_zuohaitao.vim"
"	Linux
"		1. Copy this file to ~ and rename .zuohaitao.vim
"		2. Edit ~/.vimrc, add "source ~/.zuohaitao.vim"
"	enjoy it
" history
"		2011/08/12
"					change for mac os X
"					reconstruction
"					for support python IDE
"					fix problem show unicode file in win32 is not well
"					reconstruction map keyboard
"		2010/06/05 modify _fl porting in win32
"		2010/03/28 map _fn for function comment
"					map _fl for file header comment
"		2009/06/22 set tags comment
"		2009-06-03 
"			add auto complementation in line 132-134		
"		2009-02-28 fix in linux don't read some file in line 24 'encoding=gbk' 
"		2008/12/06 set tags 
"
"Todo:
"		test _fl in linux
"		map _fl using variant to optimize
"
""			
"Set character set
function! z:QfMakeConv()
    let qflist = getqflist()
    for i in qflist
        let i.text = iconv(i.text, "cp936", "utf-8")
    endfor
    call setqflist(qflist)
endfunction
"set encoding=utf-8
"set fileencodings=ucs-bom,utf-8,cp936,big5,latin1
function! z:win32_unicode_file()
	echo "z:win32_unicode_file"
	set encoding=utf-8
	set fileencodings=ucs-bom,utf-8,cp936,big5,latin1
	e %
	au QuickfixCmdPost make call z:QfMakeConv()
endfunction
if (has("linux")||has("mac"))
	set encoding=utf-8
	set fileencodings=ucs-bom,utf-8,cp936,big5,latin1
	set ambiwidth=double
elseif has("win32")
au BufNewFile,BufRead *.reg	call z:win32_unicode_file()
endif

"Set shell to be bash
if (has("unix")||has("mac"))
	set shell=bash
elseif has("win32")
	set shell=cmd
endif
"set For All (Linux & Windows & mac)
set showmode "set current mode
syntax on "syntax highlight
set tabstop=4 shiftwidth=4 sts=4 "tab=4 byte
set ai "autoindent
set noexpandtab "close replace tab to 4 byte blank
set nocompatible	"set normal ,use vim feature
set hlsearch "rearch result is highlight
set helplang=cn " set help file is chinese
set cindent " C indent format
set backspace=indent,eol,start " backspace is indent, eol, start
set linespace=5	"linespace
set number	"line number
set imcmdline	"the Input Method is always	
set laststatus=2 "always a status line

"color style
if has("gui_running")
	colorscheme pablo	
endif
"backup
if (has("mac")||has("linux"))
	set backupdir=/tmp
elseif has("win32")
	set backupdir=c:\tmp "save template directory
endif
"undo
if (version >= 703)
	set undofile
	if (has("mac")||has("linux"))
		set undodir=~/.undo
	elseif has("win32")
		set undodir=C:\undo
	endif
	set cryptmethod=blowfish
endif
"Set mapleader repleace \xx to ,xx
let mapleader = ","



""""""""""""""""""""""""""""""
"Plugin Setting 
""""""""""""""""""""""""""""""

" Tag list (ctags)
if has("win32")                "set ctags path in windows OS
	"let Tlist_Ctags_Cmd = 'ctags'
	let Tlist_Ctags_Cmd='ctags.exe'
elseif has("linux")              "set ctags path in linux OS
	let Tlist_Ctags_Cmd = '/usr/bin/ctags'
elseif has("mac")
	let Tlist_Ctags_Cmd = '/usr/local/bin/ctags'
endif
"   let Tlist_Show_One_File = 1	"show the current file tag ,not show the other files
let Tlist_Exit_OnlyWindow = 1	"if taglist window is the last window exit the vim after the window is close.
let  Tlist_Compact_Format = 1	"In compact display mode, do not display help
" netrw setting
nmap <silent> <leader>fe :Sexplore!<cr>
let g:netrw_winsize = 30
" BufExplorer
let g:bufExplorerDefaultHelp=0       " Do not show default help.
let g:bufExplorerShowRelativePath=1  " Show relative paths.
let g:bufExplorerSortBy='mru'        " Sort by most recently used.
let g:bufExplorerSplitRight=0        " Split left.
let g:bufExplorerSplitVertical=1     " Split vertically.
let g:bufExplorerSplitVertSize = 30  " Split width
let g:bufExplorerUseCurrentWindow=1  " Open in new window.
" winManager setting
let g:winManagerWindowLayout = "BufExplorer,FileExplorer|TagList"
let g:winManagerWidth = 30
let g:defaultExplorer = 1 
"cscope
"set cscopequickfix=s-,c-,d-,i-,t-,e-


"keyboard map
nmap <silent><F2> :WMToggle<cr><C-W><C-F>
function! z:comment(cflg)
	if (g:cmflg="")
		return
	let s=line("v")
	let c=getline(s)
	let n=substitute(c, "^", cflg, "")
	echo n
endfunction
let g:cmflg=""
au BufNewFile,BufRead *.py	let g:cmflg="#"
au BufNewFile,BufRead *.c,*.cpp,*.h	let g:cmflg="//"
vmap <silent><F3>	:cal z:comment(g:cmflg)<CR>
if (has("unix")||has("linux"))
	map <silent><leader>fh <ESC>i/**<ESC>o<ESC>a @file<TAB><ESC>o@brief<TAB><ESC>o@details<TAB><ESC>o@author<TAB>zuohaitao<ESC>o@date<TAB><TAB><ESC>:r !date +\%F<ESC>k<s-j><ESC>o@warning<TAB><ESC>o@bug<TAB><TAB><ESC>o*/<ESC>3k5x4k^$a
elseif (has("win32"))
	map <silent><leader>fh <ESC>i/**<ESC>o<ESC>a @file<TAB><ESC>o@brief<TAB><ESC>o@details<TAB><ESC>o@author<TAB>zuohaitao<ESC>o@date<ESC>i<ESC>:r !date /T<ESC>k<s-j>xi<TAB><ESC>11l4xo@warning<TAB><ESC>o@bug<TAB><ESC>o/<ESC>7k6l<ESC>:r !echo %:t<ESC>k<s-j>j$a
endif
map <silent><leader>fl <ESC>^i/**<ESC>o<ESC>a @name<TAB><ESC>o@brief<TAB><ESC>o@param<TAB> [I/O]  - <ESC>o@return   - <ESC>o/<ESC>4k6la
map <silent><leader>2h <ESC>:runtime syntax/2html.vim<ESC>:%s/\(<body.*\)/\1\r<br>\r<table width=100% bgcolor="#000000" border=1>\r<tr>\r<td><font color="#ffffff">\r<ESC>:%s/\(.*\)\(<\/body>\)/\1<\/font><\/td>\r<\/tr>\r<\/table>\r\2/<ESC>:wq<ESC>



"set tags
if (has("mac")|| has("linux"))
	"$>cd /usr/include
	"$>sudo ctags -R .
	set tags+=/usr/include/tags
elseif (has("win32"))
	"ctag -R C:\Program Files\Microsoft Visual Studio 9.0\VC
	"ctag -R C:\Program Files\Microsoft SDKs
	set tags+="C:\\Program Files\\Microsoft Visual Studio 9.0\\VC\\tags"
	set tags+="C:\\Program Files\\Microsoft SDKs\\tags"
endif

"auto fill 
filetype plugin indent on
set completeopt=longest,menu
"ctrl+x ctrl+o

"set python ide
au BufNewFile,BufRead *.py	call py:setting()
function! py:setting()
	set expandtab
	map <F5> <ESC>:exe "!python3 %"<cr>
endfunction

"
"
"""""""""""""""""""
"""""some useful command"""""
"read unicode in vim set encoding=utf-8
":%!xxd :%xxd -r
"auto ident code gg=G

"del comment '88 //'
":n,m s/\/\//
":%s/\/\//
"add comment '(^- -)'
":n,m s/^/\/\/
":%s/^/\/\/
":%s/old/new/g
":%s/^\/\///g
"set fileformat=doc set fileformat=unix
"
"set foldmethod=marker " code fold
"zF make the fold mark
"zc make the code between the operator
"zo expand the fold code

