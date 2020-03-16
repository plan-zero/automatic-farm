rem Copyright (C) 2020 Coding Night Romania
rem 
rem This file is part of automatic-farm.
rem 
rem automatic-farm is free software: you can redistribute it and/or modify
rem it under the terms of the GNU General Public License as published by
rem the Free Software Foundation, either version 3 of the License, or
rem (at your option) any later version.
rem 
rem automatic-farm is distributed in the hope that it will be useful,
rem but WITHOUT ANY WARRANTY; without even the implied warranty of
rem MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
rem GNU General Public License for more details.
rem 
rem You should have received a copy of the GNU General Public License
rem along with automatic-farm.  If not, see <http://www.gnu.org/licenses/>.



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

