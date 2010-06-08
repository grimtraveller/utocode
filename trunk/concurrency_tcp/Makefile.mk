###########################################
#@file		
#@brief	
#@details
#	usage:
#		nmake all /f Makefile.mk
#		nmake clean /f Makefile.mk
#@author	zuohaitao
#@date		2010-06-08 ÐÇÆÚ¶þ 
#@warning	
#@bug		
###########################################
all:
	cl concurrency_server.c /Zi  /DWIN32
	cl concurrency_client.c /Zi /DWIN32
clean:
	del *.exe *.pdb *.obj *.ilk
