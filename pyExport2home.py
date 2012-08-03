import os
import shutil 
if __name__== '__main__':
    destdir = os.path.abspath('.')+os.sep+'home' 
    if os.path.isdir(destdir):
            shutil.rmtree(destdir)

    export_files = os.popen("svn status -q").readlines()
    for i in range(0, len(export_files)):
        filename = export_files[i][8:len(export_files[i])-1]
        srcfile = os.path.abspath('.') + os.sep + filename
        destfile = destdir + os.sep + filename
        print(srcfile+'\n'+'\t\t'+'->'+destfile)
        if os.path.isdir(os.path.dirname(destfile)) == False:
            os.makedirs(os.path.dirname(destfile))
        shutil.copyfile(srcfile, destfile)
    cmd = 'mkdir ' + destdir
    os.system(cmd)
    cmd = 'XCOPY /S /E /Y ' + destdir + ' g:\home\\'
    os.system(cmd)
#    cmd = 'RMDIR /S /Q '+ destdir
#    os.system(cmd)

