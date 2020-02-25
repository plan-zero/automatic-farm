@Echo Off
Set "CDev=%1"
Set "RKey=HKEY_LOCAL_MACHINE\HARDWARE\DEVICEMAP\SERIALCOMM"
Set "CNum="
For /F "Skip=1 Tokens=3" %%A In (
    'Reg Query "%RKey%" /V "\Device\%CDev%" 2^>Nul') Do Set CNum=%%A
If Not Defined CNum Exit /B
Rem Your commands below here
Echo %CNum%
Timeout 1 >Nul

