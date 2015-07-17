#include "tkzngui.hpp"
using namespace tkzngui;

int main(void){
  InitTkzngui();
  Window window1;
  window1.Visualize();

  Window window2;
  window2.Visualize();
  

  Stop();
  return 0;
}








//window
//title
//pos
//size
//style
//state show active hidden etc.

//button
//input

//checkbox
//radiobutton
//listbox
//combobox
//ProgressBar

//mesbox
//groupbox
//hr
//tab
//text

//color
//pensize
//penmode
//line
//circle
//fcircle
//rect
//frect
//pos
//print
//font



  //window1.visualize();
  //window1.pos(x,y);
  //window1.size(x,y);
  //window1.style();
  //
  //tkzngui::button button1;
  //button1.visualize();
  //button1.pos(x,y);
  //button1.size(x,y);
  //button1.setparent(window1);