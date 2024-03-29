#ifndef __WINDOW_H
#define __WINDOW_H 1
extern void setCustomClipping(int enabled, int x0, int y0, int x1, int y1); // should be in vectrexInterface.h

#ifndef FALSE
#define TRUE (0==0)
#define FALSE (0!=0)
#endif

#define VECTREX_DEFAULT 0
// rotate 90 degrees anticlockwise for each step:
#define VECTREX_VERTICAL 1
#define VECTREX_HORIZONTAL 2
#define VECTREX_INVERTED 3
#define VECTREX_HORIZONTAL_INVERTED 4
#define NO_CLIP FALSE

void v_set_hardware_orientation(int orientation);
void v_line(int xl, int yb, int xr, int yt, int col);
void v_window(int xl, int yb, int xr, int yt, int clip_to_window);
void v_clip(int xl, int yb, int xr, int yt);

#endif
