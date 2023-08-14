rem ..\..\build_windows\x64\Debug\db_database_load.exe -b ../../../../data/tsv-full/ -h db-filtered/ -n name.basics.tsv
rem ..\..\build_windows\x64\Debug\db_database_load.exe -b ../../../../data/tsv-full/ -h db-filtered/ -t title.basics.tsv 
..\..\build_windows\x64\Debug\db_database_load.exe -b ../../../../data/tsv-filtered/ -h db-filtered/ -p title.principals.filtered-1.tsv
..\..\build_windows\x64\Debug\db_database_load.exe -b ../../../../data/tsv-filtered/ -h db-filtered/ -p title.principals.filtered-2.tsv
..\..\build_windows\x64\Debug\db_database_load.exe -b ../../../../data/tsv-filtered/ -h db-filtered/ -p title.principals.filtered-3.tsv
rem ..\..\build_windows\x64\Debug\db_database_load.exe -b ../../../../data/tsv-filtered/ -h db-filtered/ -p title.principals.filtered-4.tsv

rem skipping these
..\..\build_windows\x64\Debug\db_database_load.exe -b ../../../../data/tsv-filtered/ -h db-filtered/ -p title.principals.filtered-5.tsv
..\..\build_windows\x64\Debug\db_database_load.exe -b ../../../../data/tsv-filtered/ -h db-filtered/ -p title.principals.filtered-6.tsv
..\..\build_windows\x64\Debug\db_database_load.exe -b ../../../../data/tsv-filtered/ -h db-filtered/ -p title.principals.filtered-7.tsv
..\..\build_windows\x64\Debug\db_database_load.exe -b ../../../../data/tsv-filtered/ -h db-filtered/ -p title.principals.filtered-8.tsv

rem ..\..\build_windows\x64\Debug\db_database_load.exe -b ../../../../data/tsv-filtered/ -h db-filtered/ -p title.principals.filtered-9.tsv
rem ..\..\build_windows\x64\Debug\db_database_load.exe -b ../../../../data/tsv-filtered/ -h db-filtered/ -p title.principals.filtered-10.tsv
