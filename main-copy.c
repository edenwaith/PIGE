/* main.c */

#include <Carbon/Carbon.h>

#include "main.h"

void Initialize(void);	/* function prototypes */
void EventLoop(void);
void MakeWindow(void);
void MakeMenu(void);
void DoEvent(EventRecord *event);
void DoMenuCommand(long menuResult);
void DoAboutBox(void);
void DrawWindow(WindowRef window);
static OSErr QuitAppleEventHandler(const AppleEvent *appleEvt, AppleEvent* reply, UInt32 refcon);

Boolean		gQuitFlag;	/* global */

int main(int argc, char *argv[])
{
	Initialize();
	MakeWindow();
	MakeMenu();
	EventLoop();

	return 0;
}
 
void Initialize()	/* Initialize some managers */
{
    OSErr	err;
        
    InitCursor();

    err = AEInstallEventHandler( kCoreEventClass, kAEQuitApplication, NewAEEventHandlerUPP((AEEventHandlerProcPtr)QuitAppleEventHandler), 0, false );
    if (err != noErr)
        ExitToShell();
}

static OSErr QuitAppleEventHandler( const AppleEvent *appleEvt, AppleEvent* reply, UInt32 refcon )
{
    gQuitFlag =  true;
    
    return noErr;
}

void EventLoop()
{
    Boolean	gotEvent;
    EventRecord	event;
        
    gQuitFlag = false;
	
    do
    {
        gotEvent = WaitNextEvent(everyEvent,&event,32767,nil);
        if (gotEvent)
            DoEvent(&event);
    } while (!gQuitFlag);
    
    ExitToShell();					
}

void MakeWindow()	/* Put up a window */
{
    Rect	wRect;
    WindowRef	myWindow;
    
    SetRect(&wRect,50,50,600,200); /* left, top, right, bottom */
    myWindow = NewCWindow(nil, &wRect, "\pHello", true, zoomNoGrow, (WindowRef) -1, true, 0);
    
    if (myWindow != nil)
        SetPort(GetWindowPort(myWindow));  /* set port to new window */
    else
	ExitToShell();					
}

void MakeMenu()		/* Put up a menu */
{
    Handle	menuBar;
    MenuRef	menu;
    long	response;
    OSErr	err;
	
    menuBar = GetNewMBar(rMenuBar);	/* read menus into menu bar */
    if ( menuBar != nil )
    {
        SetMenuBar(menuBar);	/* install menus */
        // AppendResMenu(GetMenuHandle(mApple), 'DRVR');
        
        err = Gestalt(gestaltMenuMgrAttr, &response);
	if ((err == noErr) && (response & gestaltMenuMgrAquaLayoutMask))
        {
            menu = GetMenuHandle( mFile );
            DeleteMenuItem( menu, iQuit );
            DeleteMenuItem( menu, iQuitSeparator );
        }
        
        DrawMenuBar();
    }
    else
    	ExitToShell();
}

void DoEvent(EventRecord *event)
{
    short	part;
    Boolean	hit;
    char	key;
    Rect	tempRect;
    WindowRef	whichWindow;
        
    switch (event->what) 
    {
        case mouseDown:
            part = FindWindow(event->where, &whichWindow);
            switch (part)
            {
                case inMenuBar:  /* process a moused menu command */
                    DoMenuCommand(MenuSelect(event->where));
                    break;
                    
                case inSysWindow:
                    break;
                
                case inContent:
                    if (whichWindow != FrontWindow()) 
                        SelectWindow(whichWindow);
                    break;
                
                case inDrag:	/* pass screenBits.bounds */
                    GetRegionBounds(GetGrayRgn(), &tempRect);
                    DragWindow(whichWindow, event->where, &tempRect);
                    break;
                    
                case inGrow:
                    break;
                    
                case inGoAway:
                    DisposeWindow(whichWindow);
                    ExitToShell();
                    break;
                    
                case inZoomIn:
                case inZoomOut:
                    hit = TrackBox(whichWindow, event->where, part);
                    if (hit) 
                    {
                        SetPort(GetWindowPort(whichWindow));   // window must be current port
                        EraseRect(GetWindowPortBounds(whichWindow, &tempRect));   // inval/erase because of ZoomWindow bug
                        ZoomWindow(whichWindow, part, true);
                        InvalWindowRect(whichWindow, GetWindowPortBounds(whichWindow, &tempRect));	
                    }
                    break;
                }
                break;
		
                case keyDown:
		case autoKey:
                    key = event->message & charCodeMask;
                    if (event->modifiers & cmdKey)
                        if (event->what == keyDown)
                            DoMenuCommand(MenuKey(key));
		case activateEvt:	       /* if you needed to do something special */
                    break;
                    
                case updateEvt:
			DrawWindow((WindowRef) event->message);
			break;
                        
                case kHighLevelEvent:
			AEProcessAppleEvent( event );
			break;
		
                case diskEvt:
			break;
	}
}

void DoMenuCommand(long menuResult)
{
    short	menuID;		/* the resource ID of the selected menu */
    short	menuItem;	/* the item number of the selected menu */
	
    menuID = HiWord(menuResult);    /* use macros to get item & menu number */
    menuItem = LoWord(menuResult);
	
    switch (menuID) 
    {
        case mApple:
            switch (menuItem) 
            {
                case iAbout:
                    DoAboutBox();
                    break;
                    
                case iQuit:
                    ExitToShell();
                    break;
				
                default:
                    break;
            }
            break;
        
        case mFile:
            break;
		
        case mEdit:
            break;
    }
    HiliteMenu(0);	/* unhighlight what MenuSelect (or MenuKey) hilited */
}

void DrawWindow(WindowRef window)
{
    Rect		tempRect;
    GrafPtr		curPort;
	
    GetPort(&curPort);
    SetPort(GetWindowPort(window));
    BeginUpdate(window);
    EraseRect(GetWindowPortBounds(window, &tempRect));
    DrawControls(window);
    DrawGrowIcon(window);
    EndUpdate(window);
    SetPort(curPort);
}

void DoAboutBox(void)
{
   (void) Alert(kAboutBox, nil);  // simple alert dialog box
}
