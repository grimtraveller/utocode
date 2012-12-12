""
"@file			zuohaitao.vim
"@brief			gvim config file
"@author		zuohaitao
"@date			2011-08-28
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
"		2012/12/12	
"					suport markdown format file(*.md,*.markdown)
"		2012/12/04
"					map <silent><leader>fh add c/c++ file header comment
"					map <silent><leader>fc add c/c++ function comment
"
"		2012/09/29
"					<F2> <C-F2> mark (use visualmark.vim) 
"					add ruller
"					add auto columns=100
"					add indent_guides configure
"					add python(.py) file is supported
"		2012/08/24
"					add comment to explain how to convert 'tab' to 'space'
"		2012/07/31 
"					objc(.m) file is supported
"		2011/08/28 
"					fix comment function in macvim
"		2011/08/18
"					fix tags path support jump win32 API
"					fix comment
"					recode make file header comment
"					recode make function comment
"
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
"		test all in vim in linux 
"		test all in macvim 
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
function! z:win32_unicode_file()
	let fn = bufname("%") 
	if (".reg" == strpart(fn,len(fn)-4, 4))
		set encoding=utf-8
		set fileencodings=ucs-bom,utf-8,cp936,big5,latin1
		au QuickfixCmdPost make call z:QfMakeConv()
	endif
	if (".xml" == strpart(fn,len(fn)-4, 4))
		set encoding=utf-8
		set fileencodings=ucs-bom,utf-8,cp936,big5,latin1
		au QuickfixCmdPost make call z:QfMakeConv()
	endif
	if (".m" == strpart(fn,len(fn)-2, 2))
		set encoding=utf-8
		set fileencodings=ucs-bom,utf-8,cp936,big5,latin1
		au QuickfixCmdPost make call z:QfMakeConv()
	endif
	if (".sln" == strpart(fn,len(fn)-4, 4))
		set encoding=utf-8
		set fileencodings=ucs-bom,utf-8,cp936,big5,latin1
		au QuickfixCmdPost make call z:QfMakeConv()
	endif
	if (".py" == strpart(fn,len(fn)-3, 3))
		set encoding=utf-8
		set fileencodings=ucs-bom,utf-8,cp936,big5,latin1
		au QuickfixCmdPost make call z:QfMakeConv()
	endif
	if (".cgi" == strpart(fn,len(fn)-4, 4))
		set encoding=utf-8
		set fileencodings=ucs-bom,utf-8,cp936,big5,latin1
		au QuickfixCmdPost make call z:QfMakeConv()
	endif
	if (".utf8.txt" == strpart(fn,len(fn)-9, 9))
		set encoding=utf-8
		set fileencodings=ucs-bom,utf-8,cp936,big5,latin1
		au QuickfixCmdPost make call z:QfMakeConv()
	endif
	if ("blog" == strpart(fn,len(fn)-4, 4))
		set encoding=utf-8
		set fileencodings=ucs-bom,utf-8,cp936,big5,latin1
		au QuickfixCmdPost make call z:QfMakeConv()
	endif
	if (".md" == strpart(fn, len(fn)-3, 3))
		set encoding=utf-8
		set fileencodings=ucs-bom,utf-8,cp936,big5,latin1
		au QuickfixCmdPost make call z:QfMakeConv()
	endif
	if (".markdown" == strpart(fn, len(fn)-9, 9))
		set encoding=utf-8
		set fileencodings=ucs-bom,utf-8,cp936,big5,latin1
		au QuickfixCmdPost make call z:QfMakeConv()
	endif
endfunction

if (has("linux")||has("mac"))
	set encoding=utf-8
	set fileencodings=ucs-bom,utf-8,cp936,big5,latin1
	set ambiwidth=double
elseif has("win32")
	call z:win32_unicode_file()
endif

"Set shell to be bash
if (has("unix")||has("mac"))
	set shell=bash
elseif has("win32")
	"use default shell because set shell=cmd will be cause confilict with taglist 
	"more than see taglist.vim in line 2268
	"set shell=cmd
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
if has("gui_running")
	set cc=80 "ruller
endif
set columns=100
if has("mac")
	set macmeta
endif

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

" Tag list
if has("win32")                "set ctags path in windows OS
	let Tlist_Ctags_Cmd="\"".$VIM."\\ctags.exe\""
elseif has("linux")              "set ctags path in linux OS
	let Tlist_Ctags_Cmd = '/usr/bin/ctags'
elseif has("mac")
	let Tlist_Ctags_Cmd = '/usr/local/bin/ctags'
endif
let Tlist_Show_One_File = 1	"show the current file tag ,not show the other files
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
let g:winManagerWindowLayout = "TagList,BufExplorer,FileExplorer"
let g:winManagerWidth = 30
let g:defaultExplorer = 0

"""""""""""""keyboard map
"IDE keyboard map
nmap <silent><F1> :WMToggle<CR>
function! z:comment()
	if (s:cmfmt == "")
		echo "file type dose not support comment."
		return
	endif
	let l = getline(".")
	let l = substitute(l, "^", s:cmfmt, "")
	call setline(".", l)
endfunction
function! z:uncomment()
	let l = getline(".")
	let i = 0
	while ( i < strlen(l))
		let c = strpart(l, i, 1)
		if ((" " == c) || ("\t" == c))
			let i = i + 1
			continue
		else
			let c = strpart(l, i, strlen(s:cmfmt))
			if (s:cmfmt == c)
				let l = substitute(l, s:cmfmt, "", "")
				call setline(".", l)
			endif
			break
		endif
	endwhile
endfunction
let s:cmfmt=""
au BufNewFile,BufRead, *.md set filetype=markdown
au BufNewFile,BufRead, *.py	let s:cmfmt="#"
au BufNewFile,BufRead, *.py call py:setting()
au BufNewFile,BufRead, *.c,*.cpp,*.h,*.m	let s:cmfmt="//"
au BufNewFile,BufRead, *.vim let s:cmfmt="\""
au BufNewFile,BufRead, *.bat let s:cmfmt="REM "
au BufNewFile,BufRead, *.sh let s:cmfmt="#"
au BufNewFile,BufRead, *.m set filetype=objc
"comment keyboard map
if has("win32")
	map <silent><C-K><C-C>	:call z:comment()<CR>
	map <silent><C-K><C-U>	:call z:uncomment()<CR>
elseif has("mac")
	map <M-k><M-c> :call z:comment()<CR>
	map <M-k><M-u> :call z:uncomment()<CR>
endif
function! z:cfh()
"comment file header ONLY support for C filetype!
	let c = "/**"
	call append(0, c)
	let c = " @file\t\t".bufname("%")
	call append(1, c)
	let c = " @brief\t\t"
	call append(2, c)
	let c = " @details\t"
	call append(3, c)
	let c = " @author\tzuohaitao"
	call append(4, c)
	let c = " @date\t\t".strftime("%Y-%m-%d")
	call append(5, c)
	let c = " warning\t"
	call append(6, c)
	let c = " bug\t\t"
	call append(7, c)
	let c = "**/"
	call append(8, c)
	let p = [0, 0, 0, -1]
	call setpos(".", p)
	let p = [0, 3, 13, -1]
	call setpos(".", p)
endfunction
map <silent><leader>fh :call z:cfh()<CR>a
function! z:cfc()
"comment function ONLY support for C filetype!
	let p = line(".") - 1
	let c = "/**"
	call append(p, c)
	let c = " @name\t".bufname("%")
	call append(p+1, c)
	let c = " @brief\t"
	call append(p+2, c)
	let c = " @param\t [I/O] -"
	call append(p+3, c)
	let c = " return\t"
	call append(p+4, c)
	let c = "**/"
	call append(p+5, c)
	let cp = [0, p, 0, -1]
	call setpos(".", p)
	let cp = [0, p+3, 8, -1]
	call setpos(".", cp)
endfunction
map <silent><leader>fc :call z:cfc()<CR>a
map <silent><leader>2h <ESC>:runtime syntax/2html.vim<ESC>:%s/\(<body.*\)/\1\r<br>\r<table width=100% bgcolor="#000000" border=1>\r<tr>\r<td><font color="#ffffff">\r<ESC>:%s/\(.*\)\(<\/body>\)/\1<\/font><\/td>\r<\/tr>\r<\/table>\r\2/<ESC>:wq<ESC>
"set ctags
if (has("mac")|| has("linux"))
	"$>cd /usr/include
	"$>sudo ctags -R .
	set tags+=/usr/include/tags
elseif (has("win32"))
	"cd $VIM
	"ctag -R C:\Program Files\Microsoft Visual Studio 9.0\VC
	"ctag -R C:\Program Files\Microsoft SDKs
	let $T = $VIM
	let $T = substitute($T, " ", "\\\\\\\\\\\\ ", "")
	let $T = $VIM."\\tags"
	set tags+=$T
endif

"auto fill 
filetype plugin indent on
set completeopt=longest,menu
"ctrl+x ctrl+o

"set python ide
function! py:setting()
	set expandtab
	if has("mac")
		map <F5> <ESC>:exe "!python3 %"<cr>
	elseif has("win32")
		map <F5> <ESC>:exe "!C:\\Python32\\python.exe %"<cr>
	endif
endfunction

"cd the dir where current file is in
if "" !=bufname("%")
	cd %:h
endif

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
"fold
"set foldmethod=marker " code fold
"zF make the fold mark
"zc make the code between the operator
"zo expand the fold code
"tabnew tabn<PageNumber>
"tab <-> space
":set expandtab
":retab!
":set noexpandtab
",fh add c file header comment
",fc add c file function comment
