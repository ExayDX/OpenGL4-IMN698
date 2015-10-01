#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

#include "Viewer.h"

int main(void)
{
	Viewer* aViewer = Viewer::getInstance(); 
	aViewer->loop(); 
	aViewer->Destroy(); 
}