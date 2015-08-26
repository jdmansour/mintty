Mintty is the [Cygwin](http://cygwin.com) Terminal emulator, 
also available for [MSYS](http://mingw.org/wiki/MSYS).

:warning: Note: this is a fork of mintty, for the original sources see [mintty/mintty](https://github.com/mintty/mintty/).

Added features in this fork:

  * Added support for glass / blur-behind in Windows 10.  Now supports glass in Vista, 7, 8 (with (Aero Glass)[http://www.glass8.eu/]), and 10.
  * When using glass, the alpha channel is now set properly.  That means text is finally legible, the default background is transparent, and the foreground is opaque.

Known issues:

  * The current way of drawing text with alpha is a bit wasteful, for each drawing a temporary bitmap is created.  This can be improved in future by using only one single temporary bitmap with the size of the window.
  * Also, it is a bit hackish - we use ExtTextOut which doesn't understand alpha.  Then we go through the bitmap and set pixels which are the background color to transparent.  This might result in antialiasing artifacts around letters if the background is made very transparent.
  * The line cursors are not drawn properly with glass, as I don't set the alpha channel yet.
  * Windows 10 is not yet detected.  You currently have to edit the `update_glass` function in `winmain.c` to choose the correct blurring method.

Future intended features:

  * Fix the above bugs.
  * Correct the non-glass transparency modes, too.  Make text always 100% opaque, and only the background color transparent.  Currently, everything (including the titlebar) is made transparent.
  * Improve the transparency settings.  Currently, we have:
      
        (*) Off   ( ) Low   ( ) Med.   ( ) High   ( ) Glass

    It would be better to have something like:

        Transparency: ( ) Off   ( ) Plain   (*) Glass
        Opacity:      |================|----|  80%

  * Windows 10: Tablet mode conflicts with fullscreen mode.  In tablet mode, there should be no fullscreen/non-fullscreen switch.  Instead, the window will always have no titlebar, but a (autohide) titlebar replacement should be added.  The application should behave like a "Metro" app in this mode, and seamlessly switch back to regular behavior in desktop mode.  I have to investigate if there is a API, or some public source code for this kind of titlebar replacement.

#### Overview ####

For an introduction, see the [Mintty homepage](http://mintty.github.io/).

#### Bugs and Enhancements ####

Please report bugs or suggest enhancements via the [issue tracker](https://github.com/mintty/mintty/issues).

Bugs that were reported to the previous repository at Google code before June, 2015, have been migrated here.

  * Mind! Before reporting an issue about character interaction with an application, please check the issue also with at least one other terminal (xterm, urxvt), and maybe the Cygwin Console. 

#### Contribution ####

If you consider to suggest a patch or contribute to mintty otherwise, discuss your proposal in an issue first, or on the Cygwin mailing list, or with the maintainer.

  * Repository policy: No unsolicited pull requests!
