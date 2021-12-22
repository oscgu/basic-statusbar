# basic-statusbar

A very basic statusbar for dwm I quickly threw together for my fresh install :)

# Prerequisites

Make sure you have applied the libxft-bgra patch and this removed
from ```dwm.c``` for emojis to render

```
FcBool iscol;

if(FcPatternGetBool(xfont->pattern, FC_COLOR, 0, &iscol) == FcResultMatch && iscol) {

XftFontClose(drw->dpy, xfont);

return NULL;

}
```

# Setting it up

Enter the folder and  ```make``` to compile it

Add this to your .xinitrc

```
exec /basic-statusbar/statusbar
```
