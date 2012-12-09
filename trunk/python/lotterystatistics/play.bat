@echo off
set PYTHONHOME=C:\python27
set PATH=C:\Python27;C:\Python27\Scripts;%PATH%
python doublecolorballs.py "双色球历史开奖数据(更新至2012年3月25日).xls" %1 %2
