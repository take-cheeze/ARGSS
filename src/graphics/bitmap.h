/////////////////////////////////////////////////////////////////////////////
// ARGSS - Copyright (c) 2009 - 2010, Alejandro Marzini (vgvgf)
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//	* Redistributions of source code must retain the above copyright notice,
//	this list of conditions and the following disclaimer.
//	* Redistributions in binary form must reproduce the above copyright
//	notice, this list of conditions and the following disclaimer in the
//	documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
/////////////////////////////////////////////////////////////////////////////

#ifndef _BITMAP_H_
#define _BITMAP_H_

///////////////////////////////////////////////////////////
// Headers
///////////////////////////////////////////////////////////
#include <map>
#include <vector>
#include <string>
#include "aruby.h"
#include "opengl.h"
#include <boost/cstdint.hpp>

class Tone;
class Color;
class Rect;

///////////////////////////////////////////////////////////
/// Bitmap class
///////////////////////////////////////////////////////////
class Bitmap {
public:
	Bitmap();
	Bitmap(int iwidth, int iheight);
	Bitmap(VALUE iid, std::string const& filename);
	Bitmap(VALUE iid, int iwidth, int iheight);
	Bitmap(Bitmap* source, Rect const& src_rect);
	~Bitmap();

	static bool IsDisposed(VALUE id);
	static void New(VALUE id, std::string const& filename);
	static void New(VALUE id, int width, int height);
	static Bitmap* Get(VALUE id);
	static void Dispose(VALUE id);
	static void RefreshBitmaps();
	static void DisposeBitmaps();

	int GetWidth() const;
	int GetHeight() const;
	Color GetPixel(int x, int y) const;
	Rect GetRect() const;

	void Copy(int x, int y, Bitmap* source, Rect const& src_rect);
	void Blit(int x, int y, Bitmap* source, Rect const& src_rect, int opacity);
	void StretchBlit(Rect const& dst_rect, Bitmap* src_bitmap, Rect const& src_rect, int opacity);
	void FillRect(Rect const& rect, Color const& color);
	void Clear();
	void Clear(Color const& color);
	void SetPixel(int x, int y, Color const& color);
	void HueChange(double hue);
	void SatChange(double saturation);
	void LumChange(double luminance);
	void HSLChange(double h, double s, double l);
	void HSLChange(double h, double s, double l, Rect const& rect);
	void TextDraw(Rect const& rect, std::string const& text, int align);
	Rect GetTextSize(std::string const& text);
	void GradientFillRect(Rect const& rect, Color const& color1, Color const& color2, bool vertical);
	void ClearRect(Rect const& rect);
	void Blur();
	void RadialBlur(int angle, int division);

	void ToneChange(Tone const& tone);
	void OpacityChange(int opacity, int bush_depth = 0);
	void Flip(bool flipx, bool flipy);
	void Zoom(double zoom_x, double zoom_y);
	Bitmap* Resample(int scalew, int scaleh, Rect src_rect);
	void Rotate(float angle);

	void Changed();
	void Refresh();
	void BindBitmap();
	uint32_t* GetPixels();

protected:
	VALUE id;

	GLuint gl_bitmap;
	int width, height;

	std::vector<uint32_t> pixels;

private:
	static std::map<VALUE, Bitmap*> bitmaps;
};

#endif
