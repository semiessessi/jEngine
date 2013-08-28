; Name etc...
Name "jEngine Installer"
OutFile "Setup.exe"
InstallDir $PROGRAMFILES\jEngine
Icon "..\jen\j++.ico"

InstType "32-bit"
InstType "64-bit"

; Vista shit
RequestExecutionLevel user

Page directory
Page components
Page instfiles

SectionGroup "Engine Core"
Section "x86 Build"
  SectionIn 1
  SetOutPath $INSTDIR
  File "..\jen\autoexec.cfg"
  File "..\jen\gamestart.cfg"
  
  File "..\jen\freetype6.dll"
  File "..\jen\OpenAL32.dll"
  File "..\jen\jShared.dll"
  
  File "..\jen\jEngine.exe"
  
  ;File "..\jen\runtime files\x86\*.dll"
  ;File "..\jen\runtime files\x86\*.manifest"
SectionEnd

Section "x64 Build"
  SectionIn 2
  SetOutPath $INSTDIR
  
  File "..\jen\freetype6.dll"
  File "..\jen\OpenAL32.dll"
  File "..\jen\jShared.dll"
  
  File "..\jen\runtime files\amd64\*.exe"
  ;File "..\jen\runtime files\amd64\*.dll"
  ;File "..\jen\runtime files\amd64\*.manifest"
SectionEnd
SectionGroupEnd

Section "Start Menu Folder"
    SectionIn 1 2
    CreateShortCut "$SMPROGRAMS\jEngine\jEngine.lnk" "$INSTDIR\jEngine.exe"
SectionEnd

Section "-Data Files"
  SectionIn 1 2
  
  File "..\jen\autoexec.cfg"
  File "..\jen\gamestart.cfg"
  
  SetOutPath $INSTDIR\Fonts
  File "..\jen\Fonts\Default.ttf"
  File "..\jen\Fonts\Mono.ttf"
  File "..\jen\Fonts\Serif.ttf"
  CopyFiles /Silent "Mono.ttf" "Console.ttf"
  
  SetOutPath $INSTDIR\Graphics
  File /r "..\jen\Graphics\"
  
  SetOutPath $INSTDIR\Menus
  File /r "..\jen\Menus\"
  
  SetOutPath $INSTDIR\Strings
  File /r "..\jen\Strings\"
SectionEnd