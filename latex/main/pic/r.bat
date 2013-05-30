@echo off
for %%i in (*.mp) do mp %%i
if [%1]==[] (
  del *.log
  del *.mpx
)
cd ..
r.bat
cd pics
