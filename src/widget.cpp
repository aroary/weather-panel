class Widget
{
public:
	Widget(int, int, int, int);
	~Widget();

private:
	int x;
	int y;
	int w;
	int h;
};

Widget::Widget(int x, int y, int w, int h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

Widget::~Widget()
{
}