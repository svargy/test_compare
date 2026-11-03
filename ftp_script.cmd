@echo off
setlocal enabledelayedexpansion
chcp 65001 >nul

:: Запоминаем время старта скрипта
set "START_TIME=%TIME%"

set TRIES=3
set INTERVAL=10

:: --- НАСТРОЙКИ СЕРВЕРА ---
set "sServerName=PVE-1"
set "sFolderPref=DayZ_2302"
set "FTP_CONN=ftp://(логин):(пароль)@(ip)/"

:retry
:: Создаем структуру папок заранее
md "%sServerName%\ADM" 2>nul
md "%sServerName%\RPT" 2>nul
md "%sServerName%\CarLog" 2>nul
md "%sServerName%\TraderLog" 2>nul
md "%sServerName%\Bank" 2>nul
md "%sServerName%\Garage\Log" 2>nul
md "%sServerName%\Garage\Data" 2>nul
md "%sServerName%\AtmosferaSkills\Players" 2>nul
md "%sServerName%\AtmosferaCarStamp" 2>nul
md "%sServerName%\AtmosferaPlayersStamp" 2>nul
md "%sServerName%\AtmosferaDeathStamp" 2>nul
md "%sServerName%\MoreAdminLog" 2>nul
md "%sServerName%\DeathBox" 2>nul

set "SCRIPT_FILE=%temp%\winscp_cmd_%sServerName%.txt"

:: Открываем соединение и ЖЕСТКО ОТКЛЮЧАЕМ КЭШИРОВАНИЕ папок (параметр Cache=0)
echo open %FTP_CONN% -passive=on -timeout=15 -rawsettings Cache=0 > "%SCRIPT_FILE%"
echo option reconnecttime 30 >> "%SCRIPT_FILE%"
echo option batch continue >> "%SCRIPT_FILE%"

:: Список команд GET напрямую (без циклов, как в оригинале)
echo get -neweronly "/%sFolderPref%-Profile/*.adm" "%~dp0%sServerName%\ADM\" >> "%SCRIPT_FILE%"
echo get -neweronly "/%sFolderPref%-Profile/*.rpt" "%~dp0%sServerName%\RPT\" >> "%SCRIPT_FILE%"
echo get -neweronly "/%sFolderPref%-Profile/CarLog.log" "%~dp0%sServerName%\CarLog\" >> "%SCRIPT_FILE%"
echo get -neweronly "/%sFolderPref%-Profile/CarLog/*.log" "%~dp0%sServerName%\CarLog\" >> "%SCRIPT_FILE%"
echo get -neweronly "/%sFolderPref%-Profile/TraderLog.log" "%~dp0%sServerName%\TraderLog\" >> "%SCRIPT_FILE%"
echo get -neweronly "/%sFolderPref%-Profile/TraderLog/*.log" "%~dp0%sServerName%\TraderLog\" >> "%SCRIPT_FILE%"
echo get -neweronly "/%sFolderPref%-Profile/KR_BANKING/PlayerDataBase/*.json" "%~dp0%sServerName%\Bank\" >> "%SCRIPT_FILE%"
echo get -neweronly "/%sFolderPref%-Profile/Depositary_System/Logs/*.log" "%~dp0%sServerName%\Garage\Log\" >> "%SCRIPT_FILE%"
echo get -neweronly "/%sFolderPref%-Profile/Depositary_System/PlayerData/*.json" "%~dp0%sServerName%\Garage\Data\" >> "%SCRIPT_FILE%"
echo get -neweronly "/%sFolderPref%-Profile/AtmosferaSkills/*.json" "%~dp0%sServerName%\AtmosferaSkills\" >> "%SCRIPT_FILE%"
echo get -neweronly "/%sFolderPref%-Profile/AtmosferaSkills/Players/*.json" "%~dp0%sServerName%\AtmosferaSkills\Players\" >> "%SCRIPT_FILE%"
echo get -neweronly "/%sFolderPref%-Profile/AtmosferaCarStamp/*.json" "%~dp0%sServerName%\AtmosferaCarStamp\" >> "%SCRIPT_FILE%"
echo get -neweronly "/%sFolderPref%-Profile/AtmosferaPlayersStamp/*.json" "%~dp0%sServerName%\AtmosferaPlayersStamp\" >> "%SCRIPT_FILE%"
echo get -neweronly "/%sFolderPref%-Profile/AtmosferaDeathStamp/*.json" "%~dp0%sServerName%\AtmosferaDeathStamp\" >> "%SCRIPT_FILE%"

:: Команды с докачкой
echo get -neweronly -resume -resumesupport=on "/%sFolderPref%-Profile/adm_*.log" "%~dp0%sServerName%\MoreAdminLog\" >> "%SCRIPT_FILE%"
echo get -neweronly -resume -resumesupport=on "/%sFolderPref%-Profile/BRDK_MODS/DeathBox/*.log" "%~dp0%sServerName%\DeathBox\" >> "%SCRIPT_FILE%"

echo exit >> "%SCRIPT_FILE%"

md "WinSCPLogs" 2>nul
"C:\Program Files (x86)\WinSCP\WinSCP.com" /log="WinSCPLogs\WinSCP_%sServerName%.log" /logsize=2*20M /ini=nul /script="%SCRIPT_FILE%"

set WINSCP_RESULT=%ERRORLEVEL%
del "%SCRIPT_FILE%" 2>nul

:: Если код 0 или 1 (некоторые файлы отсутствовали) — это успех
if %WINSCP_RESULT% leq 1 (
    echo [УСПЕХ] Сервер %sServerName% обработан.
    echo 0 > "%~dp0%sServerName%\res.txt"
) else (
    set /A TRIES=%TRIES%-1
    if !TRIES! gtr 0 (
        echo [СБОЙ] Ошибка FTP. Повтор через %INTERVAL% сек... [Попыток осталось: !TRIES!]
        timeout /t %INTERVAL%
        goto retry
    ) else (
        echo [ОШИБКА] Не удалось подключиться к FTP.
        echo %WINSCP_RESULT% > "%~dp0%sServerName%\res.txt"
        exit /b 1
    )
)

:: Расчет времени выполнения
set "END_TIME=%TIME%"
for /f "tokens=1-4 delims=:.," %%a in ("%START_TIME%") do set /a "start=(((%%a*60)+1%%b-100)*60+1%%c-100)*100+1%%d-100"
for /f "tokens=1-4 delims=:.," %%a in ("%END_TIME%") do set /a "end=(((%%a*60)+1%%b-100)*60+1%%c-100)*100+1%%d-100"
set /a "elapsed=end-start"
set /a "el_min=elapsed/6000", "el_sec=(elapsed%%6000)/100"
if %el_min% lss 10 set "el_min=0%el_min%"
if %el_sec% lss 10 set "el_sec=0%el_sec%"

echo Время выполнения скрипта: %el_min%:%el_sec%
