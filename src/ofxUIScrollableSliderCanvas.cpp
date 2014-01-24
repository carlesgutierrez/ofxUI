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

#include "ofxUIScrollableSliderCanvas.h"
#include "ofxUI.h"

ofxUIScrollableSliderCanvas::~ofxUIScrollableSliderCanvas()
{
    delete sRect;
}

ofxUIScrollableSliderCanvas::ofxUIScrollableSliderCanvas(float x, float y, float w, float h) : ofxUICanvas(x,y,w,h)
{
    initScrollable();
}

ofxUIScrollableSliderCanvas::ofxUIScrollableSliderCanvas(float x, float y, float w, float h, ofxUICanvas *sharedResources) : ofxUICanvas(x,y,w,h,sharedResources)
{
    initScrollable();
}

ofxUIScrollableSliderCanvas::ofxUIScrollableSliderCanvas() : ofxUICanvas()
{
    initScrollable();
}

ofxUIScrollableSliderCanvas::ofxUIScrollableSliderCanvas(ofxUICanvas *sharedResources) : ofxUICanvas(sharedResources)
{
    initScrollable();
}

void ofxUIScrollableSliderCanvas::initScrollable()
{
    kind = OFX_UI_WIDGET_SCROLLABLECANVAS;
    sRect = new ofxUIRectangle(rect->x, rect->y, rect->getWidth(), rect->getHeight());	
	
    paddedRect->setParent(sRect);
    isScrolling = false;
    vel.set(0);
    pos.set(0);
    ppos.set(0);
    acc.set(0);
    damping = .75;
    scrollX = false;
    scrollY = true;
    
    nearTop = false;
    nearBot = false;
    nearRight = false;
    nearLeft = false;
    
    hitWidget = false;
    stickyDistance = 32;
    hit = false;
    snapping = true;
#ifdef OFX_UI_TARGET_TOUCH
    touchId = -1;
#endif
	
	
}

void ofxUIScrollableSliderCanvas::setupScrollBar(string _name, float _min, float _max, int _lowvalue, int _highvalue, int _w, int _h, int _x, int _y, int _size){

	//Scroll bar // name, _min, _max, lowvalue, highvalue, w,						h,		x,		y,	OFX_UI_FONT_SMALL
	//	name,	0,	400,	380,	400,		25(minimum suported),	390,	CanvasW, 0, OFX_UI_FONT_SMALL
	//setGUI_SrollSlider(scrollBarname, 0, CanvasH, CanvasH-20, CanvasH, 25, CanvasH, xInit+CanvasW, 0, OFX_UI_FONT_SMALL);
	setGUI_SrollSlider( _name, _min, _max, _lowvalue, _highvalue, _w, _h, _x, _y, _size);
}

void ofxUIScrollableSliderCanvas::setDamping(float _damping)
{
    damping = _damping;
}

void ofxUIScrollableSliderCanvas::setSnapping(bool _snapping)
{
    snapping = _snapping;
}

void ofxUIScrollableSliderCanvas::setScrollArea(float x, float y, float w, float h)
{
    sRect->x = x;
    sRect->y = y;
    sRect->setWidth(w);
    sRect->setHeight(h);
    paddedRect->setWidth(w+padding*2);
    paddedRect->setHeight(h+padding*2);
}

void ofxUIScrollableSliderCanvas::setScrollAreaToScreen()
{
    sRect->x = 0;
    sRect->y = 0;
    sRect->setWidth(ofGetWidth());
    sRect->setHeight(ofGetHeight());
}

void ofxUIScrollableSliderCanvas::setScrollAreaToScreenWidth()
{
    sRect->x = 0;
    sRect->setWidth(ofGetWidth());
}

void ofxUIScrollableSliderCanvas::setScrollAreaToScreenHeight()
{
    sRect->y = 0;
    sRect->setHeight(ofGetHeight());
}

void ofxUIScrollableSliderCanvas::setScrollAreaHeight(float _height)
{
    sRect->setHeight(_height);
    paddedRect->setHeight(_height+padding*2);
}

void ofxUIScrollableSliderCanvas::setScrollAreaWidth(float _width)
{
    sRect->setWidth(_width);
    paddedRect->setWidth(_width+padding*2);
}

void ofxUIScrollableSliderCanvas::setScrollableDirections(bool _scrollX, bool _scrollY)
{
    scrollX = _scrollX;
    scrollY = _scrollY;
}

void ofxUIScrollableSliderCanvas::setStickDistance(float _stickyDistance)
{
    stickyDistance = _stickyDistance;
}

void ofxUIScrollableSliderCanvas::dampenX()
{
    if(nearRight || nearLeft)
    {
        acc.x -=vel.x*(.5);
    }
    else
    {
        acc.x -=vel.x*(1.0-damping);
    }
}

void ofxUIScrollableSliderCanvas::dampenY()
{
    if(nearTop || nearBot)
    {
        acc.y -=vel.y*(.5);
    }
    else
    {
        acc.y -=vel.y*(1.0-damping);
    }
}

void ofxUIScrollableSliderCanvas::update()
{
    if(!isScrolling)
    {
        if(scrollX && snapping)
        {
            float dxLeft = rect->x - sRect->x;
            float dxRight = (sRect->x+sRect->getWidth()) - (rect->x+rect->getWidth());
            
            if(fabs(dxLeft) < stickyDistance)
            {
                nearLeft = true;
            }
            else if(fabs(dxRight) < stickyDistance)
            {
                nearRight = true;
            }
            else
            {
                nearLeft = false;
                nearRight = false;
            }
            
            if(dxLeft > 0)
            {
                acc.x += (-dxLeft)/10.0;
                dampenX();
            }
            else if(nearLeft)
            {
                acc.x += (-dxLeft)/10.0;
                dampenX();
            }
            else if(dxRight > 0)
            {
                acc.x += (dxRight)/10.0;
                dampenX();
            }
            else if(nearRight)
            {
                acc.x += (dxRight)/10.0;
                dampenX();
            }
        }
        
        if(scrollY && snapping)
        {
            float dyTop = rect->y - sRect->y;
            float dyBot = (sRect->y+sRect->getHeight()) - (rect->y+rect->getHeight());
			//float dyTop = rectInternal->y - sRectInternal->y;
			//float dyBot = (sRectInternal->y+sRectInternal->getHeight()) - (rectInternal->y+rectInternal->getHeight());
            
			//cout << "dyTop = " << dyTop << endl;
			//cout << "dyBot = " << dyBot << endl;
			
			if(fabs(dyBot) < stickyDistance)
            {
                nearTop = false;
                nearBot = true;
            }
            if(fabs(dyTop) < stickyDistance)
            {
                nearTop = true;
                nearBot = false;
            }
            
            if(dyTop > 0)
            {
                acc.y += (-dyTop)/10.0;
                dampenY();
            }
            else if(nearTop)
            {
                acc.y += (-dyTop)/10.0;
                dampenY();
            }
            
            if(dyBot > 0)
            {
                acc.y += (dyBot)/10.0;
                dampenY();
            }
            else if(nearBot)
            {
                acc.y += (dyBot)/10.0;
                dampenY();
            }
            
            nearTop = false;
            nearBot = false;
        }
        
        acc.limit(50);
        vel +=acc;
        vel.limit(100);
        if(scrollX && fabs(vel.x) > 1.0) rect->x += floor(vel.x);
        if(scrollY && fabs(vel.y) > 1.0) rect->y += floor(vel.y);
        
        vel *=damping;
        acc.set(0);
    }
    
    for(vector<ofxUIWidget *>::iterator it = widgets.begin(); it != widgets.end(); ++it)
    {
        (*it)->update();
    }
}

void ofxUIScrollableSliderCanvas::drawBack()
{
    if(draw_back)
    {
        ofxUIFill();
        ofxUISetColor(color_back);
        sRect->draw();
    }
}

void ofxUIScrollableSliderCanvas::drawOutline()
{
    if(draw_outline)
    {
        ofNoFill();
        ofxUISetColor(color_outline);
        sRect->draw();
    }
}

void ofxUIScrollableSliderCanvas::drawOutlineHighlight()
{
    if(draw_outline_highlight)
    {
        ofNoFill();
        ofxUISetColor(color_outline_highlight);
        sRect->draw();
    }
}

void ofxUIScrollableSliderCanvas::drawFill()
{
    if(draw_fill)
    {
        ofxUIFill();
        ofxUISetColor(color_fill);
        sRect->draw();
    }
}

void ofxUIScrollableSliderCanvas::drawFillHighlight()
{
    if(draw_fill_highlight)
    {
        ofxUIFill();
        ofxUISetColor(color_fill_highlight);
        sRect->draw();
    }
}

void ofxUIScrollableSliderCanvas::drawPadded()
{
    if(draw_padded_rect && !embedded)
    {
        ofxUIFill();
        ofxUISetColor(color_padded_rect);
        paddedRect->draw();
    }
}

void ofxUIScrollableSliderCanvas::drawPaddedOutline()
{
    if(draw_padded_rect_outline && !embedded)
    {
        ofNoFill();
        ofxUISetColor(color_padded_rect_outline);
        paddedRect->draw();
    }
}

void ofxUIScrollableSliderCanvas::draw()
{
    ofxUIPushStyle();
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofxUISetRectMode(OFX_UI_RECTMODE_CORNER);
    ofSetLineWidth(1.0);
    
    drawPadded();
    
    drawPaddedOutline();
    
    drawBack();
    
    drawFill();
    
    drawFillHighlight();
    
    drawOutline();
    
    drawOutlineHighlight();
    
    for(vector<ofxUIWidget *>::reverse_iterator it = widgets.rbegin(); it != widgets.rend(); ++it)
    {
        if((*it)->isVisible() && (*it)->getRect()->rInside(*sRect))
        {
            (*it)->draw();
        }
    }
    
    ofxUIPopStyle();
	
	//c
	gui_slider->draw();
}

void ofxUIScrollableSliderCanvas::setPosition(int x, int y)
{
    rect->x = x;
    rect->y = y;
    sRect->x = x;
    sRect->y = y;
}

void ofxUIScrollableSliderCanvas::setDimensions(float _width, float _height)
{
    sRect->setWidth(MIN(_width, ofGetWidth() - sRect->getX()));
    sRect->setHeight(MIN(_height, ofGetHeight() - sRect->getY()));
    rect->setWidth(_width);
    rect->setHeight(_height);
    paddedRect->width = rect->width+padding*2;
    paddedRect->height = rect->height+padding*2;
}


void ofxUIScrollableSliderCanvas::drawScrollableRect()
{
    sRect->draw();
}

#ifdef OFX_UI_TARGET_TOUCH

void ofxUIScrollableSliderCanvas::touchDown(float x, float y, int id)
{
    if(sRect->inside(x, y))
    {
        for(vector<ofxUIWidget *>::iterator it = widgets.begin(); it != widgets.end(); ++it)
        {
            if((*it)->isHit(x, y))
            {
                if((*it)->isDraggable())
                {
                    hitWidget = true;
                }
                (*it)->touchDown(x, y, id);
            }
        }
    }
    
    if(sRect->inside(x, y) && id == 0)
    {
        hit = true;
        isScrolling = false;
        vel.set(0);
    }
}

void ofxUIScrollableSliderCanvas::touchMoved(float x, float y, int id)
{
    for(vector<ofxUIWidget *>::iterator it = widgets.begin(); it != widgets.end(); ++it)
    {
        if((*it)->isVisible())	(*it)->touchMoved(x, y, id);
    }
    
    if(hit && id == 0)
    {
        if(!hitWidget)
        {
            if(isScrolling != true)
            {
                isScrolling = true;
                ppos = ofPoint(x, y);
                vel.set(0);
            }
            else
            {
                pos = ofPoint(x, y);
                vel = pos-ppos;
                if(scrollX) rect->x +=vel.x;
                if(scrollY) rect->y +=vel.y;
                ppos = pos;
            }
        }
    }
}

void ofxUIScrollableSliderCanvas::touchUp(float x, float y, int id)
{
    for(vector<ofxUIWidget *>::iterator it = widgets.begin(); it != widgets.end(); ++it)
    {
        if((*it)->isVisible())	(*it)->touchUp(x, y, id);
    }
    
    hit = false;
    hitWidget = false;
    if(isScrolling)
    {
        isScrolling = false;
        pos = ofPoint(x, y);
    }
}

void ofxUIScrollableSliderCanvas::touchCancelled(float x, float y, int id)
{
    for(vector<ofxUIWidget *>::iterator it = widgets.begin(); it != widgets.end(); ++it)
    {
        if((*it)->isVisible())	(*it)->touchUp(x, y, id);
    }
    
    hit = false;
    hitWidget = false;
    if(isScrolling)
    {
        isScrolling = false;
        pos = ofPoint(x, y);
    }
}

#endif

void ofxUIScrollableSliderCanvas::mouseDragged(int x, int y, int button)
{
    for(vector<ofxUIWidget *>::iterator it = widgets.begin(); it != widgets.end(); ++it)
    {
        if((*it)->isVisible())	(*it)->mouseDragged(x, y, button);
    }
    
    if(hit)
    {
        if(!hitWidget)
        {
            if(isScrolling != true)
            {
                isScrolling = true;
                ppos = ofPoint(x,y);
                vel.set(0,0);
            }
            else
            {
                pos = ofPoint(x, y);
                vel = pos-ppos;
                if(scrollX) rect->x +=vel.x;
                if(scrollY) rect->y +=vel.y;
                ppos = pos;
            }
        }
    }
}

void ofxUIScrollableSliderCanvas::mousePressed(int x, int y, int button)
{
    if(sRect->inside(x, y))
    {
        hit = true;
        for(vector<ofxUIWidget *>::iterator it = widgets.begin(); it != widgets.end(); ++it)
        {
            if((*it)->isVisible())
            {
                if((*it)->isHit(x, y))
                {
                    if((*it)->isDraggable())
                    {
                        hitWidget = true;
                    }
                    (*it)->mousePressed(x, y, button);
                }
            }
        }
    }
    
    isScrolling = false;
    vel.set(0,0);
}

void ofxUIScrollableSliderCanvas::mouseReleased(int x, int y, int button)
{
    for(vector<ofxUIWidget *>::iterator it = widgets.begin(); it != widgets.end(); ++it)
    {
        if((*it)->isVisible()) (*it)->mouseReleased(x, y, button);
    }
    
    hit = false;
    hitWidget = false;
    if(isScrolling)
    {
        isScrolling = false;
        pos = ofPoint(x,y);
    }
}

ofxUIRectangle *ofxUIScrollableSliderCanvas::getSRect()
{
    return sRect;
}


bool ofxUIScrollableSliderCanvas::isHit(int x, int y)
{
    if(isEnabled() && sRect->inside(x, y))
    {
        return true;
    }
    else
    {
        for (map<string, ofxUIWidget*>::iterator it=widgetsAreModal.begin() ; it != widgetsAreModal.end(); it++ )
        {
            if((*it).second->isVisible() && (*it).second->isHit(x, y))
            {
                return true;
            }
        }
    }
    return false;
}

void ofxUIScrollableSliderCanvas::setMappedScrollPos(float _posScroll){
	
	if(_posScroll >= 0 && _posScroll <= 1){
		posScrollbar = _posScroll;
	}
}

/*
void ofxUIScrollableSliderCanvas::calcHeightContends(vector<ofxUIWidget*> _auxwidgets){
	float sizeH = 0;
	for( int i = 0; i < _auxwidgets.size(); i++){
		
		float val = _auxwidgets[i]->getRect()->getHeight();
		sizeH += val;
		//cout << "HSize()= " << val << endl;
	}		
	
	heightContends = (int)sizeH;
	
	return sizeH;
}
 */


ofVec2f ofxUIScrollableSliderCanvas::calcHeightContends(vector<ofxUIWidget*> _auxwidgets)
{
	ofVec2f maximums = ofVec2f(0,0);
    float maxWidth = 0;
    float maxHeight = 0;
    
    for(vector<ofxUIWidget *>::iterator it = _auxwidgets.begin(); it != _auxwidgets.end(); ++it)
    {
        if((*it)->isVisible())
        {
            ofxUIRectangle* wr = (*it)->getRect();
            ofxUIRectangle* wrp = (*it)->getPaddingRect();
            float widgetwidth = wr->getX()+wrp->getWidth() - rect->getX();
            float widgetheight = wr->getY()+wrp->getHeight() - rect->getY();
            
            float widgetpaddingwidth = wrp->getX()+wrp->getWidth() - rect->getX();
            float widgetpaddingheight = wrp->getY()+wrp->getHeight() - rect->getY();
            
            
            if(widgetwidth > maxWidth)
            {
                maxWidth = widgetwidth;
            }
            else if(widgetpaddingwidth > maxWidth)
            {
                maxWidth = widgetpaddingwidth;
            }
            
            if(widgetheight > maxHeight)
            {
                maxHeight = widgetheight;
            }
            else if(widgetpaddingheight > maxHeight)
            {
                maxHeight = widgetpaddingheight;
            }
        }
    }
	maximums = ofVec2f(maxWidth, maxHeight);	
	return maximums;
}

void ofxUIScrollableSliderCanvas::updateScrollPosition(){
	if(scrollY){
		
		//get  vector<ofxUIWidget*> getWidgets()
		vector<ofxUIWidget*> auxwidgets = getWidgets();
		ofVec2f maxims = calcHeightContends(auxwidgets); 
		int sizeHContend = maxims.y;// y is heigh, x is width
		cout << "sizeHContend=" << maxims.y << endl;
		
		float posmap = ofxUIMap(posScrollbar, 0, 1, 100, -sizeHContend-100, true); 
		
		//finally move the canvas to direct pos finded between the maximum and minimum
		rect->y = posmap;		
	}
}

int ofxUIScrollableSliderCanvas::getHeightContends(){
	return heightContends;
}

//c
//--------------------------------------------------------------
void ofxUIScrollableSliderCanvas::setGUI_SrollSlider(string name, float _min, float _max, int lowvalue, int highvalue, int w, int h, int x, int y, int _size){	
	// Canvas for Slider
	//gui_slider = new ofxUICanvas(x+2, y, w, h);
	gui_slider = new ofxUICanvas(x+2, y, w, h);
	
	//ofxUIScrollSlider::ofxUIScrollSlider(string _name, float _min, float _max, float _valuelow, float _valuehigh, float w, float h,
	//float x, float y, int _size) : ofxUIWidgetWithLabel()
	
	//gui_slider->addWidgetLeft(new ofxUIScrollSlider(name, _min, _max, lowvalue, highvalue, w, h, x, y, OFX_UI_FONT_SMALL));
	gui_slider->addWidgetLeft(new ofxUIScrollSlider(name, 0, 400, 380, 400, 20, 390, 0, 0, OFX_UI_FONT_SMALL));
	gui_slider->setDrawPaddingOutline(false);  // draw border
	
	ofAddListener(gui_slider->newGUIEvent,this,&ofxUIScrollableSliderCanvas::guiEvent);
}

//--------------------------------------------------------------
void ofxUIScrollableSliderCanvas::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();
	
	//cout << "got event from: " << name << endl;
	
	if(name == "S"){
		ofxUIScrollSlider* scrollSlider =  (ofxUIScrollSlider *)e.widget;
		float mapvalscroll = scrollSlider->getPosScrollBar();
		setMappedScrollPos(mapvalscroll);
		updateScrollPosition();		
	}
}