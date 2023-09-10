#include "widget.h"

Widget::Widget(int id, RECT rect) : rect{ rect.left, rect.top, rect.right, rect.bottom } {
	this->id = id;
}

void Dashboard::replace(int id, RECT rect)
{
	this->widgets.at(id)->rect = rect;
}

void Widget::replace(Dashboard* dashboard, int id, RECT rect)
{
	dashboard->replace(id, rect);
}