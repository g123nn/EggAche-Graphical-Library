//
// User Interface of EggAche Graphical Library
// By LJN-BOT Man, 2015
//

#ifndef EGGACHE_GL
#define EGGACHE_GL

//========================================================================================
// Ignore this Section...
//========================================================================================

typedef void (* ONCREATE) ();
typedef void (* ONCLOSE)();
typedef void (* ONPAINT)();
typedef void (* ONRESIZE)(int, int);
typedef void (* ONCLICK)(int, int);
typedef void (* ONPRESS)(char);
typedef void (* ONTIMER)();

//========================================================================================
// Event Driver
//========================================================================================

//
// Hint: In your code, just let g_fnOn* = (your function's name).
//		 Your function will be called when the Event happen.
//

// Create Window Event
extern ONCREATE g_fnOnCreate;
// Called when the Window is being Created
// Your function's format:
// void *** ();

// Close Window Event
extern ONCLOSE g_fnOnClose;
// Called when the Window is about to Closed
// Your function's format:
// void *** ();

// Paint Window Event
extern ONPAINT g_fnOnPaint;
// Called when the Window needs to be refresh
// Your function's format:
// void *** ();

// Resize Window Event
extern ONRESIZE g_fnOnResize;
// Called when the Window is being Resized or Created
// Your function's format:
// void *** (int width, int height);
// width, height = the new size of the Client Rect
//                 (Window size - Border size)
// p.s. it's not the same as the params in NewWindow ()

// Mouse Click Event
extern ONCLICK g_fnOnClick;
// Called when the mouse clicks
// Your function's format:
// void *** (int x, int y);
// x, y = the position of the click point

// Key Press Event
extern ONPRESS g_fnOnPress;
// Called when you input a character
// Your function's format:
// void *** (char ch);
// ch = the Ascii number of the key

//========================================================================================
// Create Window and Use Message Loop
//========================================================================================

// Starter Function
int HiWindow (
	const char *szCap,		// the Caption String
	int width,				// Width of the new window (0 = Use Default)
	int height);			// Height of the new window (0 = Use Default)
// Return value:
// 1 = Successfully Created, 0 = Failed
// Hint:
// width, height include the size of borders and caption...
// Strongly recommend you use 0 to have default size, and get size in g_fnOnResize

//========================================================================================
// Simple Message Box
//========================================================================================

// Message Box
void MsgBox (
	const char *szHint,		// the Text String
	const char *szCap);		// the Caption String

//========================================================================================
// Amazing Timer Functions~~~
//========================================================================================

// Create Timer
int NewTimer (
	int id,					// the ID of your new Timer (0 ~ 99)
	int time,				// the Tick of your new Timer (Millisecond)
	ONTIMER fnTick);		// the *** of "void *** ()" (To receive the Notification)
// Return value:
// 1 = Successfully Created, 0 = Failed

// Kill Timer
int FreeTimer (
	int id);				// the ID of Timer to be Killed (0 ~ 99)
// Return value:
// 1 = Successfully Killed, 0 = Failed

//========================================================================================
// Paper
//========================================================================================

typedef struct _Paper *Paper;

// New Paper
Paper NewPaper ();
// Return value:
// Paper: Successfully Created, NULL: Failed

// Copy Paper
int DuplicatePaper (
	Paper pDst,				// To-Paper
	Paper pSrc);			// From-Paper
// Return value:
// 1 = Successfully Copyed, 0 = Failed

// Print a Paper to current window
int PrintPaper (
	Paper paper);			// Source Paper
// Return value:
// 1 = Successfully Painted, 0 = Failed

// Delete a Paper when it's not used
int FreePaper (
	Paper paper);			// Paper to Free
// Return value:
// 1 = Successfully Freed, 0 = Not Valid Paper

//========================================================================================
// Egg for Animation
//========================================================================================

typedef struct _Egg *Egg;

// New Egg
Egg EggNew (
	int x,					// Initial x Position
	int y,					// Initial y Position
	int width,				// Width of the new Egg
	int height);			// Height of the new Egg
// Return value:
// Egg: Successfully Created, NULL: Failed

Egg EggFromBmp (
	int x,					// Initial x Position
	int y,					// Initial y Position
	int width,				// Width of the new Egg (0 = Use Bitmap Size)
	int height,				// Height of the new Egg (0 = Use Bitmap Size)
	const char *szPath);	// "path/name.xxx" of the source file
// Return value:
// Egg: Successfully Created, NULL: Failed

Egg EggFromBmpEx (
	int x,					// Initial x Position
	int y,					// Initial y Position
	int width,				// Width of the new Egg (0 = Use Bitmap Size)
	int height,				// Height of the new Egg (0 = Use Bitmap Size)
	int xSrc,				// x in the source Bitmap
	int ySrc,				// y in the source Bitmap
	int wSrc,				// Width in the source Bitmap (0 = Use Bitmap Size)
	int hSrc,				// Height in the source Bitmap (0 = Use Bitmap Size)
	const char *szPath,		// "path/name.xxx" of the source file
	int colorMask);			// color of mask (-1 = Not Used)
// Return value:
// Egg: Successfully Created, NULL: Failed
// Hint:
// The color of colorMask will be set to Transparent
// If not used ( = -1), the Egg is Opaque
// This function can stretch the Region in Bitmap
//    (xSrc, ySrc)-------------(xSrc + wSrc, ySrc)
//         |                            |
//         |                            |
//         |                            |
//         |                            |
// (xSrc, ySrc + hSrc)-------(xSrc + wSrc, ySrc + hSrc)
// into the new Egg

// Copy Egg
Egg EggCopy (
	Egg egg,				// Source Egg
	int x,					// Initial x Position
	int y);					// Initial y Position
// Return value:
// Egg: Successfully Copied, NULL: Failed

// Paint an Egg to a Paper
int EggPaint (
	Paper paper,			// Dest Papar
	Egg egg);				// Source Egg
// Return value:
// 1 = Successfully Freed, 0 = Not Valid Paper

// Delete an Egg when it's not used
int EggFree (
	Egg egg);				// Egg to Free
// Return value:
// 1 = Successfully Freed, 0 = Not Valid Paper

// Move Egg or Get its Position
void EggMove (Egg egg, int x, int y);  // Move (x, y)
void EggPlace (Egg egg, int x, int y);  // Move To (x, y)

int EggGetX (Egg egg);
int EggGetY (Egg egg);

//========================================================================================
// Painting Functions
//========================================================================================

// Hint:
// P* functions = Setting/Painting on a Paper
// E* functions = Setting/Painting in an Egg
//
// We use pixel coordinate system like
// (0, 0)------------------>x
//   |
//   |
//   |
//   |
//   y

// Return value:
// 1 = Successfully Drawed, 0 = Failed

// Customize your own Pen and Brush
int PSetPen (Paper paper, int width, int color, int fNULL);
int ESetPen (Egg egg, int width, int color, int fNULL);
int PSetBrush (Paper paper, int color, int fNULL);
int ESetBrush (Egg egg, int color, int fNULL);
// fNULL = is this Pen/Brush Transparent
int GetColor (int r, int g, int b);  // for color parameters above

// Draw Line
int PDrawLine (Paper paper, int xBeg, int yBeg, int xEnd, int yEnd);
int EDrawLine (Egg egg, int xBeg, int yBeg, int xEnd, int yEnd);
// Hint: Draw a line from (xBeg, yBeg) to (xEnd, yEnd)

// Draw Rectangle
int PDrawRect (Paper paper, int xBeg, int yBeg, int xEnd, int yEnd);
int EDrawRect (Egg egg, int xBeg, int yBeg, int xEnd, int yEnd);
// Hint: Draw a Rectangle like
//   (xBeg, yBeg)-----------------(xEnd, yBeg)
//        |                            |
//        |                            |
//        |                            |
//        |                            |
//   (xBeg, yEnd)-----------------(xEnd, yEnd)

// Draw Ellipse
int PDrawElps (Paper paper, int xBeg, int yBeg, int xEnd, int yEnd);
int EDrawElps (Egg egg, int xBeg, int yBeg, int xEnd, int yEnd);
// Hint: Draw the Ellipse in such a bounding rect box

// Draw round conner Rectangle
int PDrawRdRt (Paper paper, int xBeg, int yBeg,
			   int xEnd, int yEnd, int wElps, int hElps);
int EDrawRdRt (Egg egg, int xBeg, int yBeg,
			   int xEnd, int yEnd, int wElps, int hElps);
// Hint: Draw a round conner Rectangle
//       wElps, hElps = the width/height of the conner Ellipse

// Draw Text
int PDrawTxt (Paper paper, int xBeg, int yBeg, const char *szText);
int EDrawTxt (Egg egg, int xBeg, int yBeg, const char *szText);
// Hint: Draw the szText with the upper left point (xBeg, yBeg)

// Draw Bitmap
int PDrawBitmap (
	Paper paper,			// Dest Papar
	int xBeg,				// Left position on the Paper
	int yBeg,				// Top position on the Paper
	int width,				// Width of the Bitmap (0 = Full size)
	int height,				// Height of the Bitmap (0 = Full size)
	const char *szPath);	// "path/name.xxx" of the source file

#endif  //EGGACHE_GL
