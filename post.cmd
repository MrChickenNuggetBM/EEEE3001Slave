@echo off
setlocal enabledelayedexpansion

:: Incrementing X
set "file=version.txt"
if not exist !file! echo 1>!file!
set /p X=<version.txt
set /a X+=1
echo !X!>!file!

:: Git commands
git add *
echo > git add *
git commit -m "v!X!"
echo > git commit -m "v!X!"
git push
echo > git push

:: Display the incremented version
echo Updated to version !X! ??

endlocal
