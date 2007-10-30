/*
 * CoveredCalc
 *
 * Copyright (c) 2004-2007 CoveredCalc Project Contributors
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <AppKit.h>
#include <InterfaceKit.h>

class BeFontSizeApp : public BApplication
{
public:
	BeFontSizeApp()
		:	BApplication("application/x-vnd.Hironytic-BeFontSize")
	{
	}

	virtual void ReadyToRun()
	{
		font_height fh;
		be_plain_font->GetHeight(&fh);
		
		float height = fh.ascent + fh.descent;
		float width = be_plain_font->StringWidth("abcdefghijklmnopqrstuvwxyz");
			
		printf("designFontWidth = %f,\n", width);
		printf("designFontHeight = %f,\n", height);

		be_app->PostMessage(B_QUIT_REQUESTED, be_app);
	}
};

int main()
{
	BeFontSizeApp app;
	app.Run();
	return 0;
}
