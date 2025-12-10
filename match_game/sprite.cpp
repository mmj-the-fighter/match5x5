#include "sprite.h"

void Sprite::Set(spn::Image* img)
{
    dragging = false;
    dragStartX = 0;
    dragStartY = 0;
    image = img; 
    sprW = image->GetCanvas()->GetWidth();
    sprH = image->GetCanvas()->GetHeight();
    sprX = sprY = 0;
}

Sprite::Sprite()
{
    image = NULL;
    dragging = false;
    dragStartX = 0;
    dragStartY = 0;
    sprX = sprY = sprW = sprH = 0;
}

void Sprite::Display(spn::Canvas* canvas)
{
    if (image == NULL) {
        return;
    }
    canvas->EnableAlphaBlending(true);
    canvas->DrawImage(image, sprX, sprY);
}

void Sprite::SetPosition(int x, int y)
{
    sprX = x;
    sprY = y;
}

bool Sprite::IsPointInsideSprite(int x, int y) {
    return (x >= sprX) && (x <= sprX + sprW)
        && (y >= sprY) && (y <= sprY + sprH);
}

bool Sprite::OnLmbDown(float x, float y) {
    if(IsPointInsideSprite(x, y)) {
        dragging = true;
        dragStartX = x;
        dragStartY = y;
        return true;
    }
    else {
        dragging = false;
        return false;
    }
}

bool Sprite::OnMouseMoved(float x, float y, float relx, float rely) {
    if(dragging)
    {
        sprX += relx;
        sprY += rely;
        return true;
    }
    else {
        return false;
    }
}

bool Sprite::OnLmbUp() {
    if(dragging) {
        dragging = false;
        return true;
    }
    else {
        return false;
    }
}

