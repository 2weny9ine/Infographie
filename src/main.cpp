#include "ofMain.h"
#include "Application.h"
#include "Renderer.h"

int main()
{
    ofGLFWWindowSettings windowSettings;
    windowSettings.resizable = true;
    windowSettings.setGLVersion(3, 3);

    ofCreateWindow(windowSettings);

    ofRunApp(&Application::getInstance());
}
