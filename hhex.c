#include "xhextris.h"

void xhextrisHex(int row, int column, int type) 
{
  static int memo_type=NOPIECE;

  DEBUGPRINT(("xhextrisHex: row %d, column %d, type %d memtype %d\n",
	      row,column,type, memo_type));

  if (type != memo_type) {
    XSetForeground(dpy, hexgc, PieceColors[type].pixel);
    memo_type=type;
  }
  /* sous XDarwinRootless: ca ne recopie pas la contre-forme */
  //      XDrawString(dpy, win, hexgc, 
  XDrawImageString(dpy, win, hexgc, 
		   COORDCOL(column) + HEXWIDTH,
		   COORDLIG(row,column),
		   (type==NOPIECE) ? "}" : "|",
		   1);
}
