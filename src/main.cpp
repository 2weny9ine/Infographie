#include "ofMain.h"
#include "application.h"

//========================================================================
int main( ){

    ofGLFWWindowSettings windowSettings;

    windowSettings.resizable = false;
   
    windowSettings.setGLVersion(3, 3);
  
    ofCreateWindow(windowSettings);

    ofRunApp(new Application());
}
