# Copyright (C) 2020 Coding Night Romania
# 
# This file is part of automatic-farm.
# 
# automatic-farm is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# automatic-farm is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with automatic-farm.  If not, see <http://www.gnu.org/licenses/>.


triggerAlarm = False
alarmRunning = False
alarmSnooze = 0 #in seconds
alarmTriggerTime = ''
alarmOff = True


def manageAlarms():
    global triggerAlarm
    global alarmRunning
    global alarmSnooze
    global alarmTriggerTime
    global alarmOff
    if alarmOff == True:
        logMessage("Sistemul de alarma este oprit")
        return
    if triggerAlarm == True:
        logMessage("Alarm has been set!")
        triggerAlarm = False
        alarmRunning = True
        alarmSnooze = 720
        alarmTriggerTime = strftime("%Y-%m-%d %H:%M:%S", localtime())
        #starting alarm and snooze it to 20 minutes. This will run for 1 hour
    if alarmRunning == True:
        if alarmSnooze == 0:
            alarmRunning = False
            for number in phonebook:
                output = sendSMS(number, "Alarma pornita in data" + alarmTriggerTime + " a expirat!")
                value = extractResponse(output, 'result')
                if value == modem_success_string:
                    logMessage("Alarm message has been sent to " + number)
                else:
                    logMessage("Failed to send alarm message to " + number)
        elif  alarmSnooze % 240 == 0:
            #send a sms right here
            for number in phonebook:
                output = sendSMS(number, "Miscare detectata la aluni. Alarma a pornit la data " + alarmTriggerTime)
                value = extractResponse(output, 'result')
                if value == modem_success_string:
                    logMessage("Alarm message has been sent to " + number)
                else:
                    logMessage("Failed to send alarm message to " + number)
        logMessage("Alarm scheduled: " + str(alarmSnooze))
        alarmSnooze -= 1
