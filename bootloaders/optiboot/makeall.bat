@REM this Windows bat file will only start the makeall script with bash
@REM we must use the date of the cygwin package instead of the DOS date!
@REM @time /T > all.time
@c:\cygwin64\bin\date > all.time
@bash makeall
@c:\cygwin64\bin\date >> all.time
@REM @time /T >> all.time
@echo "makeall runs between:"
@cat all.time
@rm all.time
