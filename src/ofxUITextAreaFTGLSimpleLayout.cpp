/**********************************************************************************
 
 Copyright (C) 2012 Syed Reza Ali (www.syedrezaali.com)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
 **********************************************************************************/

#include "ofxUITextAreaFTGLSimpleLayout.h"
#include "ofxUI.h"

ofxUITextAreaFTGLSimpleLayout::ofxUITextAreaFTGLSimpleLayout(string _name, string _textstring, float w, float h, float x, float y, int _size) : ofxUIWidgetWithLabel()
{	
    init(_name, _textstring, w, h, x, y, _size);
}

ofxUITextAreaFTGLSimpleLayout::ofxUITextAreaFTGLSimpleLayout(string _name, wstring _textstring, float w, float h, float x, float y, int _size) : ofxUIWidgetWithLabel()
{	
    init(_name, _textstring, w, h, x, y, _size);
}

void ofxUITextAreaFTGLSimpleLayout::init(string _name, wstring _textstring, float w, float h, float x, float y, int _size)
{
    initRect(x,y,w,h);
    name = string(_name);
    kind = OFX_UI_WIDGET_TEXTAREAFTGLSIMPLELAYOUT;
    textwstring = _textstring;
    setDrawFill(true);
    setDrawBack(false);
    drawShadow = false;
    drawLabel = false;
    
    label = new ofxUILabel(padding*2.0,0,(name+" LABEL"), _size);
    addEmbeddedWidget(label);
    label->setVisible(drawLabel);
    
	if(h == 0)
	{
        autoSize = true;
    }
    else
    {
        autoSize = false;
    }
	
	//c
	bwstring = false;
}


void ofxUITextAreaFTGLSimpleLayout::init(string _name, string _textstring, float w, float h, float x, float y, int _size)
{
    initRect(x,y,w,h);
    name = string(_name);
    kind = OFX_UI_WIDGET_TEXTAREAFTGLSIMPLELAYOUT;
    textstring = _textstring;
    setDrawFill(true);
    setDrawBack(false);
    drawShadow = false;
    drawLabel = false;
    
    label = new ofxUILabel(padding*2.0,0,(name+" LABEL"), _size);
    addEmbeddedWidget(label);
    label->setVisible(drawLabel);
    
    if(h == 0)
    {
        autoSize = true;
    }
    else
    {
        autoSize = false;
    }
	
	//c
	bwstring = true;
}

void ofxUITextAreaFTGLSimpleLayout::drawBack()
{
    if(draw_back)
    {
        ofFill();
        ofSetColor(color_back);
        rect->draw();
    }
}

void ofxUITextAreaFTGLSimpleLayout::drawFill()
{
    if(draw_fill)
    {
        if(drawShadow)
        {
            ofSetColor(color_back);
            for(unsigned int i = 0; i < textLines.size(); i++)
            {
                label->drawStringShadow(rect->getX(), rect->getY()+(lineHeight+lineSpaceSize)*(i+1)-lineSpaceSize, textLines[i]);
            }
        }
        
        ofSetColor(color_fill);
        for(unsigned int i = 0; i < textLines.size(); i++)
        {
            label->drawString(rect->getX(), rect->getY()+(lineHeight+lineSpaceSize)*(i+1)-lineSpaceSize, textLines[i]);
        }
    }
}

wstring ofxUITextAreaFTGLSimpleLayout::getTextString()
{
    return textwstring;
}


void ofxUITextAreaFTGLSimpleLayout::setTextString(string s)
{
    textLines.clear();
    textstring = s;
    formatTextString();
}

void ofxUITextAreaFTGLSimpleLayout::formatTextString()
{

	float rectWidthLimit = rect->getWidth()-padding*6;
    float rectHeightLimit = rect->getHeight()-label->getLineHeight()-padding;
	
	//setLineLength
	ofxUILabel* l =label->getLabelWidget();
	ofxFTGLSimpleLayout *f;
	f= l ->ofxUIWidget::getFont();
	
	ofxUIRectangle r;
	r= *l ->ofxUIWidget::getRect();
	f->setLineLength(rect->getWidth()-padding*6); // if it crashed do padding*something
	
	
//	cout << "rectHeightLimit "			<< rectHeightLimit			<< endl;
//	cout << "rect->getHeight() "		<< rect->getHeight()		<< endl;
//	cout << "label->getLineHeight() "	<< label->getLineHeight()	<< endl;
//	cout << "padding "					<< padding					<< endl;
	
    bool overheight = false;
    
    lineHeight = label->getStringHeight("1");
    lineSpaceSize = padding*2;
    
    offsetY = floor(padding*.125);
    
//	cout << "*********************************************************" << endl;
//	cout << "TEXTSTRING width=" << label->getStringWidth(textstring) << endl;;
//	cout << "rectWidthLimit ="	<< rectWidthLimit	<< endl;
	
	
    if(label->getStringWidth(textstring) <= rectWidthLimit)
    {
        if(textstring.size() > 0)
        {
            textLines.push_back(textstring);
        }
    }
    else
    {
		/*
        float tempWidth;
        float tempHeight;
        textLines.clear();
        string line = "";
        size_t i=0;
        
//		cout << "*********************************************************" << endl;
//		cout << "TEXTSTRING =" << textstring;
//		cout << "label->getStringWidth() "	<< label->getStringWidth(line)	<< endl;
		//if it's not at the end of the string && not over the rect's height
        while (i < textstring.size() && !overheight)
        {
//				cout << "label->getStringWidth() "	<< label->getStringWidth(line)	<< endl;
			cout << "*********************************************************" << endl;
			cout << "TEXTSTRING =" << textstring << endl;
			cout << "getStringWidth ="	<< label->getStringWidth(line)	<< endl;
			
            tempWidth = label->getStringWidth(line);

            if(tempWidth < rectWidthLimit)
            {
                line+=textstring.at(i);
                i++;
                if(i == textstring.size())
                {
                    textLines.push_back(line);
					cout << "*********************************************************" << endl;
					for(int j=0;j<textLines.size();j++){cout << "line[" << j << "]=" << textLines[j] << endl;}
                }
            }
            else
            {
				// if space char is not found
                bool notFound = true;
				
				// while
                while (notFound && !overheight)
                {
					//c
					//cout << "notFound && !overheight i[" << i << "] searching-> " << &textstring.at(i) << endl;

					cout << "char = "<< i << endl;
					cout << "searching-> " << &textstring.at(i) << endl;
					int char_ = strncmp(&textstring.at(i), " ",1);
					cout << "code = "<< char_ << endl;

					// search for space character
                    if(strncmp(&textstring.at(i), " ",1) == 0)
                    {
                        tempHeight = (textLines.size()+1)*(lineHeight+lineSpaceSize);
                                                cout << "tempHeight " << tempHeight << endl;
                                                cout << "rectHeightLimit " << rectHeightLimit << endl;

						// if not in autosize and height greater than rect limit
                        if(!autoSize && tempHeight >= rectHeightLimit)
                        {
                            textLines.push_back(line);
                            textLines[textLines.size()-1]+="...";
                            overheight = true;
                        }
						//
                        notFound = false;
                        if(!overheight)
                        {
							// add a line in text area
                            textLines.push_back(line);
                            line.clear();
                            i++;
                        }
                    }
					// if no space found
                    else{
						cout << "lineSize = "<< line.size() << endl;
                        i--;
                        line.erase(line.end()-1);
                    }
                }
            }
        }
		 	*/
    }

    if(autoSize)
    {
//        rect->setHeight((lineHeight+lineSpaceSize)*textLines.size()-lineSpaceSize);
		ofRectangle rec = f->getStringBoundingBox(textstring, 0, 0);
		ofxUIRectangle *re = new ofxUIRectangle (rec.x,rec.y, rec.width, rec. height);
		float lineHeight= f->getLineHeight();
		rect->setHeight((rec.y*-1)+lineHeight);
/*		cout << "autosize" << endl;
		cout << "getStringBoundingBox =" << f->getStringBoundingBox(textstring, 0, 0) << endl;
		cout << "x =" << rec.x<< endl;
		cout << "y =" << rec.y<< endl;
		cout << "width =" << rec.width<< endl;
		cout << "height =" << rec.height<< endl;
*/    }

    if(overheight)
    {
        rect->setHeight(MAX(rect->getHeight(),(lineHeight+lineSpaceSize)*textLines.size()-lineSpaceSize));
		cout << "overheight" << endl;
    }

}

void ofxUITextAreaFTGLSimpleLayout::setParent(ofxUIWidget *_parent)
{
    parent = _parent;
	formatTextString();
    calculatePaddingRect();
}

void ofxUITextAreaFTGLSimpleLayout::setDrawShadow(bool _drawShadow)
{
    drawShadow = _drawShadow;
}



















