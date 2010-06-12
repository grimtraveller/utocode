""
"@file			zuohaitao.vim
"@brief			gvim config file
"@author		zuohaitao
"@date			2010-03-28
"@version		1.3
"@details
"	Windows OS
"		1. Windows copy this file to $VIM
"		2. Edit $(VIM)/_vimrc, add "source $VIM/_zuohaitao.vim"
"	Linux
"		1. Copy this file to ~ and rename .zuohaitao.vim
"		2. Edit ~/.vimrc, add "source ~/.zuohaitao.vim"
"	enjoy it
" history
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
"Set character in linux
if (has("unix")||has("linux"))
	set encoding=utf-8
	"set encoding=gbk
	set fileencodings=ucs-bom,utf-8,chinese,gb2312,default
	set ambiwidth=double
endif
"Set shell to be bash
if has("unix")
	set shell=bash
elseif has("linux")
	set shell=bash
elseif has("win32")
	"I have to run win32 python without cygwin
	"set shell=D:\bin\cygwin\cygwin.bat
endif
"For All (Linux & Windows)
set showmode "显示当前模式
syntax on "语法高亮
set tabstop=4 shiftwidth=4 sts=4 "tabs数
set ai "自动对齐
set noexpandtab "关闭tab自动转换空格
set nocompatible	"通用设置 使用vim特性
set hlsearch "高亮显示匹配
set helplang=cn " 设置帮助语言为中文
set cindent " C语言缩进
set backspace=indent,eol,start " 退格键能够删除空行、缩进等
if has("gui_running")
	colorscheme pablo	"颜色
endif
if (has("unix")||has("linux"))
	set backupdir=~/tmp
elseif has("win32")
	set backupdir=c:\tmp "临时文件存放目录
endif
"Set mapleader
let mapleader = ","
""""""""""""""""""""""""""""""
" Tag list (ctags)
""""""""""""""""""""""""""""""
map <silent> <leader>tl :TlistToggle<cr>
if has("win32")                "设定windows系统中ctags程序的位置
	"let Tlist_Ctags_Cmd = 'ctags'
	let Tlist_Ctags_Cmd='c:\ctags\ctags.exe'
elseif has("linux")              "设定linux系统中ctags程序的位置
	let Tlist_Ctags_Cmd = '/usr/bin/ctags'
endif

"   let Tlist_Show_One_File = 1	"不同时显示多个文件的tag，只显示当前文件的
   let Tlist_Exit_OnlyWindow = 1	"如果taglist窗口是最后一个窗口，则退出vim
""""""""""""""""""""""""""""""
" netrw setting
""""""""""""""""""""""""""""""
nmap <silent> <leader>fe :Sexplore!<cr>
let g:netrw_winsize = 30
""""""""""""""""""""""""""""""
" BufExplorer
""""""""""""""""""""""""""""""
let g:bufExplorerDefaultHelp=0       " Do not show default help.
let g:bufExplorerShowRelativePath=1  " Show relative paths.
let g:bufExplorerSortBy='mru'        " Sort by most recently used.
let g:bufExplorerSplitRight=0        " Split left.
let g:bufExplorerSplitVertical=1     " Split vertically.
let g:bufExplorerSplitVertSize = 30  " Split width
let g:bufExplorerUseCurrentWindow=1  " Open in new window.
""""""""""""""""""""""""""""""
" winManager setting
""""""""""""""""""""""""""""""
let g:winManagerWindowLayout = "BufExplorer,FileExplorer|TagList"
let g:winManagerWidth = 30
let g:defaultExplorer = 1 
nmap <C-W><C-F> :FirstExplorerWindow<cr>
nmap <C-W><C-B> :BottomExplorerWindow<cr>
nmap <silent> <leader>wm :WMToggle<cr>
""""""""""""""""""""""""""""""""
"cscope
""""""""""""""""""""""""""""""""
set cscopequickfix=s-,c-,d-,i-,t-,e-
"book & blog
" blog book
au BufNewFile,BufRead *.blog,*.book		call z:book_blog()
func! z:book_blog()
	highlight mytitle	guifg=#1291A9	gui=bold
	highlight mysection	guifg=#72cfdc
	highlight Normal	guifg=#ffffff
	highlight myhide	guifg=#000000
	highlight mycomment	guifg=#808080
	highlight myimportant	guifg=#4499ee
	syntax clear
	syntax match mytitle /《 .* 》/
	syntax match mysection /[.*]/
	syntax match comment /#\-.*/	
	syntax match myhide /*</ contained
	syntax match myhide />\*/ contained
	syntax match myimportant /*<.*>\*/ contains=myhide
endfunc
"template
au BufNewFile,BufRead *.template	call z:template()
func! z:template()
	highlight replace guifg=#808080
	syntax clear
	syntax match replace /$.*\$/
	syntax match replace /《.*》/
endfunc
"map <F2> <ESC>^i//<ESC>
map <F12> <ESC>:runtime syntax/2html.vim<ESC>:%s/\(<body.*\)/\1\r<br>\r<table width=100% bgcolor="#000000" border=1>\r<tr>\r<td><font color="#ffffff">\r<ESC>:%s/\(.*\)\(<\/body>\)/\1<\/font><\/td>\r<\/tr>\r<\/table>\r\2/<ESC>:wq<ESC>

map _fn <ESC>^i/**<ESC>o<ESC>i @name<TAB><TAB><ESC>o@brief<TAB><ESC>o@param<TAB> [I/O] <ESC>o@return<TAB><ESC>o*/<ESC>o<ESC>
if (has("unix")||has("linux"))
	map _fl <ESC>i/**<ESC>o<ESC>i @file<TAB><TAB><ESC>o@brief<TAB><ESC>o@details<TAB><ESC>o@author<TAB>zuohaitao<ESC>o@date<TAB><TAB><ESC>:r !date +\%F<ESC>k<s-j><ESC>o@warning<TAB><ESC>o@bug<TAB><TAB><ESC>o*/<ESC>3k5x4k^$a
elseif (has("win32"))
	map _fl <ESC>i/**<ESC>o<ESC>a @file<TAB><ESC>o@brief<TAB><ESC>o@details<TAB><ESC>o@author<TAB>zuohaitao<ESC>o@date<ESC>i<ESC>:r !date /T<ESC>k<s-j>xi<TAB><ESC>11l4xo@warning<TAB><ESC>o@bug<TAB><ESC>o/<ESC>7k6l<ESC>:r !echo %:t<ESC>k<s-j>j$a
	
endif

if (has("unix")||has("linux"))
	"$>cd /usr/include
	"$>sudo ctags -R .
	set tags+=/usr/include/tags
endif

"auto fill 
filetype plugin indent on
set completeopt=longest,menu
"ctrl+x ctrl+o
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

