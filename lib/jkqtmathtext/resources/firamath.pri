contains(DEFINES, NO_FIRAMATH_FONTS) {
  warning("NO_FIRAMATH_FONTS was defined ==> we do not link Fira Math fonts as ressource ... Think about installing them separately on the target machine")
} else {
  message("Happy joy, we are linking XITS fonts as a ressource")
  RESOURCES += $$PWD/firamath.qrc
}
