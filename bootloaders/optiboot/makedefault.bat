@REM this Windows bat file will only start the makedefault script with bash
@REM we must use the date from the cagwin package instead of DOS time!
@REM @time /T > default.time
@c:\cygwin64\bin\date > default.time
@bash makedefault
@c:\cygwin64\bin\date >> default.time
@REM @time /T >> default.time
@echo "makedefault runs between:"
@cat default.time
@rm default.time
