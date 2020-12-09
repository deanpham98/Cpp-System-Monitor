#include "ncurses_display.h"
#include "system.h"
#include "format.h"
#include "linux_parser.h"

int main() {
//   LinuxParser::ProcessCpuUtil(7);
  System system;
  NCursesDisplay::Display(system, 10);
}