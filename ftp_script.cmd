@echo off
set TRIES=3
set INTERVAL=10
:retry
set sFolderName=%date:~6,4%-%date:~3,2%-%date:~0,2%-%TIME:~0,2%-%TIME:~3,2%
set sToday=%date:~6,4%-%date:~3,2%-%date:~0,2%

rem !!! PVE-1
set sServerName=PVE-1
set sFolderPref=DayZ_2302
md %sServerName%
md %sServerName%\%sFolderPref%-Mpmissions
md %sServerName%\%sFolderPref%-Profile

"C:\Program Files (x86)\WinSCP\WinSCP.com" ^
  /log="WinSCPLogs\WinSCP_%sServerName%.log" /logsize=2*20M /ini=nul ^
  /command ^
	"option reconnecttime 30" ^
    "open ftp://(логин):(пароль)@(ip)/ -passive=on -timeout=15" ^
	"option batch continue" ^
	"get -neweronly /%sFolderPref%-Profile/*.adm %~dp0%sServerName%\ADM\" ^
	"get -neweronly /%sFolderPref%-Profile/*.rpt %~dp0%sServerName%\RPT\" ^
	"get -neweronly /%sFolderPref%-Profile/CarLog.log %~dp0%sServerName%\CarLog\" ^
	"get -neweronly /%sFolderPref%-Profile/CarLog/*.log %~dp0%sServerName%\CarLog\" ^
	"get -neweronly /%sFolderPref%-Profile/TraderLog.log %~dp0%sServerName%\TraderLog\" ^
	"get -neweronly /%sFolderPref%-Profile/TraderLog/*.log %~dp0%sServerName%\TraderLog\" ^
	"get -neweronly /%sFolderPref%-Profile/DC_Banking/PlayerDatabase/*.json %~dp0%sServerName%\Bank\" ^
	"get -neweronly /%sFolderPref%-Profile/Depositary_System/Logs/*.log %~dp0%sServerName%\Garage\Log\" ^
	"get -neweronly /%sFolderPref%-Profile/Depositary_System/PlayerData/*.json %~dp0%sServerName%\Garage\Data\" ^
	"get -neweronly /%sFolderPref%-Profile/AtmosferaSkills/*.json %~dp0%sServerName%\AtmosferaSkills\" ^
	"get -neweronly /%sFolderPref%-Profile/AtmosferaSkills/Players/*.json %~dp0%sServerName%\AtmosferaSkills\Players\" ^
	"get -neweronly /%sFolderPref%-Profile/AtmosferaCarStamp/*.json %~dp0%sServerName%\AtmosferaCarStamp\" ^
	"get -neweronly /%sFolderPref%-Profile/AtmosferaPlayersStamp/*.json %~dp0%sServerName%\AtmosferaPlayersStamp\" ^
	"get -neweronly /%sFolderPref%-Profile/AtmosferaDeathStamp/*.json %~dp0%sServerName%\AtmosferaDeathStamp\" ^
	"get -neweronly -resume -resumesupport=on /%sFolderPref%-Profile/adm_*.log %~dp0%sServerName%\MoreAdminLog\" ^
	"get -neweronly -resume -resumesupport=on /%sFolderPref%-Profile/BRDK_MODS/DeathBox/*.log %~dp0%sServerName%\DeathBox\" ^
	"option batch off" ^
    "exit"

set WINSCP_RESULT=%ERRORLEVEL%
if %WINSCP_RESULT% equ 0 (
  echo Success %sServerName%
  echo %WINSCP_RESULT% > %~dp0%sServerName%\res.txt
) else (
	set /A TRIES=%TRIES%-1
   if %TRIES% gtr 1 (
	   echo Failed, retrying in %INTERVAL% seconds...
	   timeout /t %INTERVAL%
	   goto retry
   ) else (
	   echo Failed, aborting
	   exit /b 1
   )
echo %WINSCP_RESULT% > %~dp0%sServerName%\res.txt
)

rem exit /b %WINSCP_RESULT%
