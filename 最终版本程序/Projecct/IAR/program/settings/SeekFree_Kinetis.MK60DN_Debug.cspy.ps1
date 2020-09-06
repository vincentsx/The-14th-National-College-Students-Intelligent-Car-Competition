﻿param([String]$debugfile = "");

# This powershell file has been generated by the IAR Embedded Workbench
# C - SPY Debugger, as an aid to preparing a command line for running
# the cspybat command line utility using the appropriate settings.
#
# Note that this file is generated every time a new debug session
# is initialized, so you may want to move or rename the file before
# making changes.
#
# You can launch cspybat by typing Powershell.exe -File followed by the name of this batch file, followed
# by the name of the debug file (usually an ELF / DWARF or UBROF file).
#
# Read about available command line parameters in the C - SPY Debugging
# Guide. Hints about additional command line parameters that may be
# useful in specific cases :
#   --download_only   Downloads a code image without starting a debug
#                     session afterwards.
#   --silent          Omits the sign - on message.
#   --timeout         Limits the maximum allowed execution time.
#


if ($debugfile -eq "")
{
& "D:\Program Files (x86)\IAR\common\bin\cspybat" -f "G:\比赛\第十四届恩智浦\14信标\正式比赛\最终版本\Projecct\IAR\program\settings\SeekFree_Kinetis.MK60DN_Debug.general.xcl" --backend -f "G:\比赛\第十四届恩智浦\14信标\正式比赛\最终版本\Projecct\IAR\program\settings\SeekFree_Kinetis.MK60DN_Debug.driver.xcl" 
}
else
{
& "D:\Program Files (x86)\IAR\common\bin\cspybat" -f "G:\比赛\第十四届恩智浦\14信标\正式比赛\最终版本\Projecct\IAR\program\settings\SeekFree_Kinetis.MK60DN_Debug.general.xcl" --debug_file=$debugfile --backend -f "G:\比赛\第十四届恩智浦\14信标\正式比赛\最终版本\Projecct\IAR\program\settings\SeekFree_Kinetis.MK60DN_Debug.driver.xcl" 
}
