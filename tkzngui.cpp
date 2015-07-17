#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

#include "tkzngui.hpp"


#define WND_CLASS_NAME TEXT("TkznguiWindowClass")

namespace tkzngui{
  EventListManager::EventListManager(){
    events.clear();

  }
  void EventListManager::Add(Event &event){
    for(auto it = events.begin(); it != events.end(); it++){
      if((*it).uMsg==event.uMsg){
        (*it).f_continue=event.f_continue;
        (*it).funcptr=event.funcptr;
        return;
      }
    }
    events.push_back( event );
  }
  void EventListManager::Add(UINT uMsg,bool f_continue,int (*funcptr)(Window& ,HWND , UINT , WPARAM , LPARAM )){
    Event event;
    event.uMsg = uMsg;
    event.f_continue = f_continue;
    event.funcptr = funcptr;
    Add( event );
  }
  void EventListManager::Del(Event &event){
    for(auto it = events.begin(); it != events.end(); it++){
      if((*it).uMsg==event.uMsg){
        events.erase(it);
        return;
      }
    }
  }
  int EventListManager::Let(Window &window, HWND hWnd, UINT uMsg, 
                    WPARAM wParam, LPARAM lParam ){
    for(auto it = events.begin(); it != events.end(); it++){
      if((*it).uMsg == uMsg ){
        
        (*it).funcptr(window, hWnd, uMsg, wParam, lParam );
        return (*it).f_continue;
      }
    }
    return true;
  }
  class Tkzngui{
    public:
    int max_width_;
    int max_height_;
    HPEN hPen_;
    HBRUSH hBrush_;
    HINSTANCE hInstance_;
    std::list<Window> list_of_windows_;
    EventListManager global_events;
    EventListManager through_global_events;
    
    Tkzngui();
    
  };
  
  Tkzngui::Tkzngui(){
    max_width_   = GetSystemMetrics( SM_CXMAXIMIZED );
    max_height_  = GetSystemMetrics( SM_CYMAXIMIZED );
    hPen_ = CreatePen( PS_SOLID, 1, RGB( 0x00, 0x00, 0x00 ) );
    //SelectObject( hMemDC, hPen_ );
    hBrush_ = CreateSolidBrush( RGB( 0xFF, 0xFF, 0xFF ) );
    //SelectObject( hMemDC, hBrush_ ); 
  }
  Tkzngui tkzngui;
  
  Window::Window(){
    hWnd_ = CreateWindowEx(
      0, 
      WND_CLASS_NAME,
      TEXT("Title"),
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 
      CW_USEDEFAULT,
      640,
      480,
      0,
      0,
      tkzngui.hInstance_,
      0);

    if(!hWnd_) {
    return;
    }
    HDC      hDC;
    HBITMAP    hBitmap;
    hDC         = GetDC( hWnd_ );
    hMemDC_     = CreateCompatibleDC( hDC );
    hBitmap     = CreateCompatibleBitmap( hDC,
                  tkzngui.max_width_,tkzngui.max_height_ );
    SelectObject( hMemDC_, hBitmap );
    ReleaseDC( hWnd_, hDC );
    RECT lprc = {0,0,tkzngui.max_width_,tkzngui.max_height_};
    FillRect( hMemDC_, &lprc, tkzngui.hBrush_ );
    tkzngui.list_of_windows_.push_back((*this));

  }
  void Window::Visualize(){
    ShowWindow(hWnd_, SW_SHOW );
  }
  Event::Event():uMsg(0),f_continue(false),funcptr(nullptr){}
  
  LRESULT CALLBACK WindowProc( HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);
  
  int  ExitEvent(Window& ,HWND , UINT , WPARAM , LPARAM );
  int  PaintEvent(Window& ,HWND , UINT , WPARAM , LPARAM );
  
  int InitTkzngui(){
    InitTkzngui( GetModuleHandle(0) ,0 ,0, 0 );
  }
  int InitTkzngui(HINSTANCE hInstance, HINSTANCE hPrevInstance,
            LPSTR lpCmdLine, int nCmdShow ){
    tkzngui.hInstance_=hInstance;
    WNDCLASSEX wcl;
    
    wcl.cbSize = sizeof(WNDCLASSEX);
    wcl.hInstance = hInstance;
    wcl.lpszClassName = WND_CLASS_NAME;
    wcl.lpfnWndProc = WindowProc;
    wcl.style = 0;
    wcl.hIcon = 0;
    wcl.hIconSm = 0;
    wcl.hCursor = LoadCursor(0, IDC_ARROW);
    wcl.lpszMenuName = 0;
    wcl.cbClsExtra = 0;
    wcl.cbWndExtra = 0;
    wcl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

    if(!RegisterClassEx(&wcl)) {
      return FALSE;
    }
    
    tkzngui.global_events.Add(WM_DESTROY,false,&ExitEvent);
    
    tkzngui.global_events.Add(WM_PAINT, false, &PaintEvent);
    
    
  }
  
  int ExitEvent(Window &window, HWND hWnd, UINT uMsg, 
                    WPARAM wParam, LPARAM lParam ){
    PostQuitMessage( 0 );
  }
  int PaintEvent(Window &window, HWND hWnd, UINT uMsg, 
                    WPARAM wParam, LPARAM lParam ){
    PAINTSTRUCT     ps;
    HDC             hDC;
    hDC = BeginPaint( hWnd, &ps );
    BitBlt( hDC, 0, 0, 
      tkzngui.max_width_,tkzngui.max_height_, 
      window.hMemDC_, 0, 0, SRCCOPY );
    EndPaint( hWnd, &ps );
    
  }
  
  void Stop(void){
    MSG msg;
    BOOL bRet;
    //while( ( bRet = GetMessage( &msg, hWnd, 0, 0 ) ) != 0) { 
    while( ( bRet = GetMessage( &msg, 0, 0, 0 ) ) != 0) { 
      if (bRet == -1) {
        break;
      }
      else {
        DispatchMessage( &msg ); 
      }
    }
  }
  



  LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg,
                WPARAM wParam, LPARAM lParam){
    
    auto it = tkzngui.list_of_windows_.begin();
    for(;it != tkzngui.list_of_windows_.end() ; it++){
      if ((*it).hWnd_ == hWnd) break;
    }
    if(it == tkzngui.list_of_windows_.end()) {
      return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    (*it).through_events.Let((*it) ,hWnd, uMsg, wParam, lParam);
    if(!(*it).events.Let((*it) ,hWnd, uMsg, wParam, lParam))
      return 0;

    tkzngui.through_global_events.Let((*it) ,hWnd, uMsg, wParam, lParam);
    if(!tkzngui.global_events.Let((*it) ,hWnd, uMsg, wParam, lParam))
      return 0;
      
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }
};
