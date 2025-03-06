#include "ofMain.h"
#include "application.h"
#include "Renderer.h"

//========================================================================
int main( ){

    ofGLFWWindowSettings windowSettings;

    windowSettings.resizable = true;

    windowSettings.setGLVersion(3, 3);

    ofCreateWindow(windowSettings);

    ofRunApp(new Application());
}
