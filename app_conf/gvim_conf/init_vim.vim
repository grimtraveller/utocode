""""":source %""""""""""""""""
"protected if want to modify 'set noreadonly'"
"set readonly

if has("mac")
	let w = getcwd()
	cd %:h
	let s:zuohaitao = readfile("zuohaitao.vim")
	call writefile(s:zuohaitao, $HOME."/.zuohaitao.vim")
	let s:vimrc_f = $HOME."/.vimrc"
	let s:vimrc = readfile(s:vimrc_f)
	call add(s:vimrc, "source $VIMRUNTIME/vimrc_example.vim")
	call add(s:vimrc, "source ~/.zuohaitao.vim")
	call writefile(s:vimrc, s:vimrc_f)
	if (!isdirectory("/tmp"))
		call mkdir("/tmp")
	endif
	if (!isdirectory("/undo"))
		call mkdir("/undo")
	endif
	"install taglist
	let s:taglist = readfile("taglist.vim")
	call writefile(s:taglist, $HOME . "/.vim/plugin/taglist.vim")
	let s:taglist = readfile("taglist.txt")
	call writefile(s:taglist, $HOME . "/.vim/doc/taglist.txt")
	"BufExplorer
	let s:bufexplorer = readfile("bufexplorer.vim")
	call writefile(s:bufexplorer, $HOME . "/.vim/plugin/bufexplorer.vim")
	let s:bufexplorer = readfile("bufexplorer.txt")
	call writefile(s:bufexplorer, $HOME . "/.vim/doc/bufexplorer.txt")

	"winManager
	let s:winmanager = readfile("winfileexplorer.vim")
	call writefile(s:winmanager, $HOME . "/.vim/plugin/winfileexplorer.vim")
	let s:winmanager = readfile("winmanager.vim")
	call writefile(s:winmanager, $HOME . "/.vim/plugin/winmanager.vim")
	let s:winmanager = readfile("wintagexplorer.vim")
	call writefile(s:winmanager, $HOME . "/.vim/plugin/wintagexplorer.vim")
	let s:winmanager = readfile("winmanager.txt")
	call writefile(s:winmanager, $HOME . "/.vim/doc/winmanager.txt")
	"tetris
	let s:tetris = readfile("tetris.vim")
	call writefile(s:tetris, $HOME . "/.vim/plugin/tetris.vim")
	"txtbrowser
	let s:txtbrowser = readfile("txtbrowser.vim")
	call writefile(s:txtbrowser, $HOME . "/.vim/plugin/txtbrowser.vim")
	let s:txtbrowser = readfile("txtbrowser.cnx")
	call writefile(s:txtbrowser, $HOME . "/.vim/doc/txtbrowser.cnx")
	let s:txtbrowser = readfile("txtbrowser.txt")
	call writefile(s:txtbrowser, $HOME . "/.vim/doc/txtbrowser.txt")
	let s:txtbrowser = readfile("txt.vim")
	call writefile(s:txtbrowser, $HOME . "/.vim/syntax/txt.vim")
	"visualmark
	let s:visualmark = readfile("visualmark.vim")
	call writefile(s:visualmark, $HOME . "/.vim/plugin/visualmark.vim")
	
	
"drawit
	e DrawIt.vba
	source %
	e init_vim.vim
	helptags $VIMRUNTIME/doc
elseif has("win32")
	"copy zuohaitao.vim $VIM/_zuohaitao.vim
	let w = getcwd()
	cd %:h
	let s:zuohaitao = readfile("zuohaitao.vim")
	call writefile(s:zuohaitao, $VIM . "/_zuohaitao.vim")
	"add to vimrc file
	let s:vimrc_f = $VIM . "/_vimrc"
	let s:vimrc = readfile(s:vimrc_f)
	call add(s:vimrc, "source $VIM/_zuohaitao.vim")
	call writefile(s:vimrc, s:vimrc_f)
	"delete $VIMRUNTIME/lang directory to fix encoding bug in win32
	cd $VIMRUNTIME
	!rmdir /S /Q "lang"
	cd- 
	"make the directory to save temp files
	if (!isdirectory("C:\\tmp"))
		call mkdir("C:\\tmp")
	endif
	"make the directory to save undo files
	if (!isdirectory("C:\\undo"))
		call mkdir("C:\\undo")
	endif
	"add ctags applaction
	let s:ctags_exe = readfile("ctags.exe","b")
	call writefile(s:ctags_exe, $VIM . "/ctags.exe", "b")

	"extract archive
	!"C:\\Program Files\\7-Zip\\7z.exe" e bufexplorer.zip -y
	!"C:\\Program Files\\7-Zip\\7z.exe" e taglist_45.zip -y
	!"C:\\Program Files\\7-Zip\\7z.exe" e txtbrowser-1.3.5.zip -y
	!"C:\\Program Files\\7-Zip\\7z.exe" e winmanager.zip -y
	!"C:\\Program Files\\7-Zip\\7z.exe" e DrawItvba.gz -y
	!"C:\\Program Files\\7-Zip\\7z.exe" e indent-guides-1.6.tar.gz -y
	!"C:\\Program Files\\7-Zip\\7z.exe" e indent-guides-1.6.tar -y
	"install taglist
	let s:taglist = readfile("taglist.vim")
	call writefile(s:taglist, $VIM . "/vimfiles/plugin/taglist.vim")
	let s:taglist = readfile("taglist.txt")
	call writefile(s:taglist, $VIM . "/vimfiles/doc/taglist.txt")
	"BufExplorer
	let s:bufexplorer = readfile("bufexplorer.vim")
	call writefile(s:bufexplorer, $VIM . "/vimfiles/plugin/bufexplorer.vim")
	let s:bufexplorer = readfile("bufexplorer.txt")
	call writefile(s:bufexplorer, $VIM . "/vimfiles/doc/bufexplorer.txt")

	"winManager
	let s:winmanager = readfile("winfileexplorer.vim")
	call writefile(s:winmanager, $VIM . "/vimfiles/plugin/winfileexplorer.vim")
	let s:winmanager = readfile("winmanager.vim")
	call writefile(s:winmanager, $VIM . "/vimfiles/plugin/winmanager.vim")
	let s:winmanager = readfile("wintagexplorer.vim")
	call writefile(s:winmanager, $VIM . "/vimfiles/plugin/wintagexplorer.vim")
	let s:winmanager = readfile("winmanager.txt")
	call writefile(s:winmanager, $VIM . "/vimfiles/plugin/winmanager.txt")
	"tetris
	let s:tetris = readfile("tetris.vim")
	call writefile(s:tetris, $VIM . "/vimfiles/plugin/tetris.vim")
	"txtbrowser
	let s:txtbrowser = readfile("txtbrowser.vim")
	call writefile(s:txtbrowser, $VIM . "/vimfiles/plugin/txtbrowser.vim")
	let s:txtbrowser = readfile("txtbrowser.cnx")
	call writefile(s:txtbrowser, $VIM . "/vimfiles/doc/txtbrowser.cnx")
	let s:txtbrowser = readfile("txtbrowser.txt")
	call writefile(s:txtbrowser, $VIM . "/vimfiles/doc/txtbrowser.txt")
	let s:txtbrowser = readfile("txt.vim")
	call writefile(s:txtbrowser, $VIM . "/vimfiles/syntax/txt.vim")
	"visualmark
	let s:visualmark = readfile("visualmark.vim")
	call writefile(s:visualmark, $VIM . "/vimfiles/plugin/visualmark.vim")


	"drawit
	e DrawIt.vba
	source %
	e init_vim.vim
	helptags $VIMRUNTIME/doc
endif "win32
set noreadonly
