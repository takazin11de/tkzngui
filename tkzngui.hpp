#include <windows.h>
#include <list>

namespace tkzngui{
    int InitTkzngui();
    int InitTkzngui(HINSTANCE , HINSTANCE , LPSTR , int );
    void Stop(void);
    
    class Window;
    class Event;
    
    class EventListManager{
      public:
      std::list<Event> events;
      void Add(Event&);
      void Add(UINT uMsg,bool f_continue,int (*funcptr)(Window& ,HWND , UINT , WPARAM , LPARAM ));
      void Del(Event&);
      int Let(Window& ,HWND , UINT , WPARAM , LPARAM );
      EventListManager();
    };
    
    class Window{
      public:
      HWND hWnd_;
      HDC hMemDC_;
      Window();
      void Visualize();
      EventListManager events;
      EventListManager through_events;
    };
    
    class Event{
      public:
      UINT uMsg;
      bool f_continue;
      int (*funcptr)(Window& ,HWND , UINT , WPARAM , LPARAM );
      Event();
    };
    
};
