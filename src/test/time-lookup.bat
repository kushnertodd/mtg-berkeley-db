@echo %time%
FOR /L %%i IN (0,1,1000) DO ..\..\..\build_windows\x64\Debug\db_database_read.exe -h db-filtered/ -t tt0060522
@echo %time%
