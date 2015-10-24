
//#include <gl/glew.h>

#include "Viewer.h"

int main(void)
{
	Viewer* aViewer = Viewer::getInstance(); 
	aViewer->loop(); 
	aViewer->Destroy(); 
}