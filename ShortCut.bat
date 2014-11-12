Sub bat
 echo off & cls
 pushd "%~dp0"
 echo create_shortcut
 start wscript -e:vbs "%~f0"
 popd
End Sub

Sub shortcut(target, location)
 Set WshShell = WScript.CreateObject("WScript.Shell")
 strCurrDir = WshShell.currentdirectory
 strDesktop = WshShell.SpecialFolders("Desktop")
 set oShellLink = WshShell.CreateShortcut(location)
 oShellLink.TargetPath = target 
 oShellLink.WindowStyle = 3
 oShellLink.IconLocation = oShellLink.TargetPath & ", 0"
 oShellLink.WorkingDirectory = strCurrDir
 oShellLink.Save 
End Sub

rem oShellLink.Description = "test link"
