/*
 * $Id: SVGReaderExtListener.java,v 1.2 2004/02/27 16:30:31 stuart_hc Exp $ $Name: V116 $
*/
package com.xerox.VTM.svg;


import com.xerox.VTM.glyphs.Glyph;

public interface SVGReaderExtListener {

    public void glyphCreated( Glyph glyph, String title );
}
