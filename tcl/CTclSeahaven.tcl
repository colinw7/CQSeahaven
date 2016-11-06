proc createGUI { } {
  frame .menubar -relief raised -borderwidth 2

  menubutton .menubar.file -text "File" -menu .menubar.file.menu
  menubutton .menubar.move -text "Move" -menu .menubar.move.menu
  menubutton .menubar.help -text "Help" -menu .menubar.help.menu

  pack .menubar.file -side left
  pack .menubar.move -side left
  pack .menubar.help -side right

  menu .menubar.file.menu
  menu .menubar.move.menu
  menu .menubar.help.menu

  .menubar.file.menu add command -label "New Game" -command { newGame }
  .menubar.file.menu add command -label "Exit"     -command { exit }

  .menubar.move.menu add command -label "Solve" -command { solve }

  .menubar.help.menu add command -label "Help" -command { help }

  pack .menubar -side top -fill x

  #----

  frame .frame

  #----

  global seahaven

  set seahaven .frame.seahaven

  CTclSeahaven $seahaven -width 800 -height 600

  pack .frame.seahaven -side top -anchor w -fill both -expand true

  #----

  pack .frame -side left -fill both -expand true
}

proc newGame { } {
  global seahaven

  $seahaven newgame
}

proc solve { } {
  global seahaven

  $seahaven solve
}

createGUI
