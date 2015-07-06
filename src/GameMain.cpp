#include <graphics.h>
#include "Gobang.h"

using namespace Gobang;

int main()
{
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
	circle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 300);
	getch();
	return 0;
}
