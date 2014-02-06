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

#include "ofxUITextArea.h"
#include "ofxUI.h"

ofxUITextArea::ofxUITextArea(string _name, string _textstring, float w, float h, float x, float y, int _size) : ofxUIWidgetWithLabel()
{	
    init(_name, _textstring, w, h, x, y, _size);
}

ofxUITextArea::ofxUITextArea(string _name, wstring _textstring, float w, float h, float x, float y, int _size) : ofxUIWidgetWithLabel()
{	
    init(_name, _textstring, w, h, x, y, _size);
}

void ofxUITextArea::init(string _name, string _textstring, float w, float h, float x, float y, int _size)
{
    initRect(x,y,w,h);
    name = string(_name);
    kind = OFX_UI_WIDGET_TEXTAREA;
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
	bwstring = false;
}


void ofxUITextArea::init(string _name, wstring _textstring, float w, float h, float x, float y, int _size)
{
    initRect(x,y,w,h);
    name = string(_name);
    kind = OFX_UI_WIDGET_TEXTAREA;
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
	bwstring = true;
}

void ofxUITextArea::drawBack()
{
    if(draw_back)
    {
        ofFill();
        ofSetColor(color_back);
        rect->draw();
    }
}

void ofxUITextArea::drawFill()
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

string ofxUITextArea::getTextString()
{
    return textstring;
}


void ofxUITextArea::setTextString(string s)
{
    textLines.clear();
    textstring = s;
    formatTextString();
}

//--------------------------------------------------------------
wstring ofxUITextArea::toWstring(string _text){
	wstring resultwtext;
	//convert it to a wstring
	resultwtext.resize(_text.size());
	mbstowcs(&resultwtext[0], &_text[0], _text.size());
	
	return resultwtext;
}


void ofxUITextArea::formatTextString()
{
    float rectWidthLimit = rect->getWidth()-padding*6;
    float rectHeightLimit = rect->getHeight()-label->getLineHeight()-padding;
	
	cout << "rectHeightLimit "			<< rectHeightLimit			<< endl;
	cout << "rect->getHeight() "		<< rect->getHeight()		<< endl;
	cout << "label->getLineHeight() "	<< label->getLineHeight()	<< endl;
	cout << "padding "					<< padding					<< endl;	
	
    bool overheight = false;
    
    lineHeight = label->getStringHeight("1");
    lineSpaceSize = padding*2;
    
    offsetY = floor(padding*.125);
    
    if(label->getStringWidth(textstring) <= rectWidthLimit)
    {
        if(textstring.size() > 0)
        {
            textLines.push_back(textstring);
        }
    }
    else
    {
        float tempWidth;
        float tempHeight;
        textLines.clear();
        string line = "";
		line.clear();
        size_t i=0;
        
        while (i < textstring.size() && !overheight) //if not at the end of the string && not over the rect's height
        {			

            tempWidth = label->getStringWidth(toWstring(line)); // getStringBoundingBox // getStringWidth
			
            if(tempWidth < rectWidthLimit)
            {
				//TODO Fisrt cahracter is suspicuous to do brake the code 
				cout << "Actual Line = "  << line << endl;
                line+=textstring.at(i);
                i++;
                if(i == textstring.size())
                {
                    textLines.push_back(line);
					cout << "Final push_back(line)= " << line << endl;
					for(int j=0;j<textLines.size();j++){cout << "Resume line[" << j << "]=" << textLines[j] << endl;}
                }
            }
            else
            {
                bool notFound = true;
                
                while (notFound && !overheight)
                {
					cout << "while notFound && !overheight i[" << i << "] searching-> " << &textstring.at(i) << endl;
					
					//This comparisons are not working at all ! LOL !
					cout << "compare 303= " << (strncmp(&textstring.at(i), "\303",4) == 0) << "is = " << &textstring.at(i) << endl;
					cout << "compare 345= " << (strncmp(&textstring.at(i), "\345",4) == 0) << "is = " << &textstring.at(i) << endl;
					cout << "compare 351= " << (strncmp(&textstring.at(i), "\351",4) == 0) << "is = " << &textstring.at(i) << endl;
					cout << "compare 261= " << (strncmp(&textstring.at(i), "\261",4) == 0) << "is = " << &textstring.at(i) << endl;
					cout << "compare 123= " << (strncmp(&textstring.at(i), "\261",4) == 0) << "is = " << &textstring.at(i) << endl;
					cout << "compare 261= " << (strncmp(&textstring.at(i), "\261",4) == 0) << "is = " << &textstring.at(i) << endl;
					
					//Go for spaces
					if(strncmp(&textstring.at(i), " ",1) == 0 )
                    {
                        tempHeight = (textLines.size()+1)*(lineHeight+lineSpaceSize);
                                                cout << "tempHeight " << tempHeight << endl;
                                                cout << "rectHeightLimit " << rectHeightLimit << endl;
                        if(!autoSize && tempHeight >= rectHeightLimit)
                        {
                            textLines.push_back(line);
							cout << " ' ' push_back(line)= " << line << endl;
                            textLines[textLines.size()-1]+="...";
                            overheight = true;
                        }
                        notFound = false;
                        if(!overheight)
                        {
						   textLines.push_back(line);
							cout << " ' ' push_back(line)= " << line << endl;
						   line.clear();
						   i++;
                        }
                    }
					else if(strncmp(&textstring.at(i), "\303",1) == 0 ){
						cout << "***is ñ" << endl;
						cout << "Before to fix line= " << line << endl;
						line.erase(line.end()-1);
						i--;

						textLines.push_back(line);
						line.clear();
						
						notFound = false;
					}
					else if(strncmp(&textstring.at(i), "\343",1) == 0 || strncmp(&textstring.at(i), "\345",1) == 0 || strncmp(&textstring.at(i), "\351",1)){
						cout << "***is Japanses character line***" << endl;
						cout << "Before to fix line= " << line << endl;
						
						//fix charcater found
						//one back
						if(strncmp(&textstring.at(i), "\343",1) == 0 || strncmp(&textstring.at(i), "\345",1) == 0 || strncmp(&textstring.at(i), "\351",1)){
							line.erase(line.end()-1);
							i--;
							line.erase(line.end()-1);
							i--;	
						}

						//save line
						cout << "Fixed line= " << line << endl;
						textLines.push_back(line);
						line.clear();
						//Stop while	
						notFound = false;
					}
                    else
                    {
                        i--;
						cout << "erasing line size = " << line.size() << "line is =" << line << endl;
                        line.erase(line.end()-1);
                    }
                }
            }
        }
    }
    
    if(autoSize)
    {
        rect->setHeight((lineHeight+lineSpaceSize)*textLines.size()-lineSpaceSize);
    }
    
    if(overheight)
    {
        rect->setHeight(MAX(rect->getHeight(),(lineHeight+lineSpaceSize)*textLines.size()-lineSpaceSize));
    }
}

void ofxUITextArea::setParent(ofxUIWidget *_parent)
{
    parent = _parent;
    formatTextString();
    calculatePaddingRect();
}

void ofxUITextArea::setDrawShadow(bool _drawShadow)
{
    drawShadow = _drawShadow;
}