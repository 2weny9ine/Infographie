#include "WindowResizedEventHandler.h"
#include "Application.h"

WindowResizedEventHandler::WindowResizedEventHandler() {}

void WindowResizedEventHandler::handleWindowResized(int w, int h)
{
    Application& app = Application::getInstance();
    app.getSceneFbo().allocate(w, h, GL_RGBA);
    ofLogNotice() << "Window resized to: " << w << "x" << h;
}
