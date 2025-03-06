#include "Application.h"

Application::Application()
    : time_current(0),
    time_elapsed(0),
    time_last(0),
    isDrawingMode(false),
    backgroundColor(ofColor::black)
{}

Application::~Application()
{
    // Cleanup code if needed
}

Application& Application::getInstance()
{
    static Application instance;
    return instance;
}

void Application::setup()
{
    ofSetWindowTitle("Infographie");
    ofLog() << "Application démarre...";

    scene.setup(&user_camera_movement.camera, &gui);
    user_camera_movement.setup(scene);

    gui.setup(&scene);
    gui.top_left->setImage(scene.img);

    sceneFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

    scene.cursor.setState(CursorState::DEFAULT);

    backgroundColor = ofColor(31);
}

void Application::update()
{
    time_current = ofGetElapsedTimef();
    time_elapsed = time_current - time_last;
    time_last = time_current;

    user_camera_movement.update(time_elapsed);
    scene.update();

    backgroundColor = gui.top_right->getBackgroundColor();
    scene.updateDrawingProperties(gui.top_right->getStrokeColor(),
                                  gui.top_right->getFillColor(),
                                  gui.top_right->getLineWidth(),
                                  gui.top_right->isOutlineEnabled());

    scene.setPrimitiveType(gui.top_right->getSelectedPrimitive());

    isDrawingMode = gui.top_right->isDrawingEnabled();
    scene.isDrawingMode = isDrawingMode;
}

void Application::draw()
{
    ofSetBackgroundColor(backgroundColor);
    scene.draw();
    scene.img->createHistogram();
    if (gui.top_left->histogramEnabled())
    {
        int posX = gui.top_left->getX() + 10;
        int posY = gui.top_left->getY() + gui.top_left->getHeight() + 10;
        int widthImg = 256;
        int heightImg = 100;
        if (scene.img && scene.img->hasImage())
        {
            scene.img->drawHistogram(posX, posY, widthImg, heightImg);
        }
        else
        {
            ofPushStyle();
            ofSetColor(50, 50, 50);
            ofDrawRectangle(posX, posY, widthImg, heightImg);
            ofSetColor(255);
            ofDrawBitmapString("Importer une image pour Voir!", posX + 10, posY + heightImg / 2);
        }
    }

    if (gui.top_left->colorFilterEnabled())
    {
        ofColor rgbColor = gui.top_left->getRGBColor();
        ofColor hsbColor = gui.top_left->getHSBColor();
        scene.img->colorFilter(rgbColor, hsbColor);
    }
    else
    {
        for (auto& imgObj : scene.img->images)
        {
            imgObj->applyUserColor = false;
        }
    }

    scene.img->imageExport("exportImage", "png");
    scene.drawCursor();
}

void Application::windowResized(int w, int h)
{
    windowResizedHandler.handleWindowResized(w, h);
}

void Application::keyPressed(int key)
{
    this->keyPressedHandler.handleKeyPressed(key);
}

void Application::keyReleased(int key)
{
    keyReleasedHandler.handleKeyReleased(key);
}

void Application::dragEvent(ofDragInfo dragInfo)
{
    dragEventHandler.processDragEvent(dragInfo);
}

void Application::mouseMoved(int x, int y)
{
    mouseMovedHandler.handleMouseMoved(x, y);
}

void Application::mouseDragged(int x, int y, int button)
{
    mouseDraggedHandler.handleMouseDragged(x, y, button);
}

void Application::mousePressed(int x, int y, int button)
{
    mouseClickedHandler.handleMousePressed(x, y, button);
}

void Application::mouseReleased(int x, int y, int button)
{
    mouseClickedHandler.handleMouseReleased(x, y, button);
}

void Application::mouseEntered(int x, int y)
{
    scene.mouse_current_x = x;
    scene.mouse_current_y = y;
    scene.cursor.setState(CursorState::DEFAULT);
}

void Application::mouseExited(int x, int y)
{
    scene.mouse_current_x = x;
    scene.mouse_current_y = y;
    scene.cursor.setState(CursorState::HIDDEN);
}

void Application::exit()
{
    ofLog() << "<app::exit>";
}

// Getters
GUI& Application::getGui() { return gui; }
Scene& Application::getScene() { return scene; }
Renderer& Application::getRenderer() { return renderer; }
User_Camera_Movement& Application::getUserCameraMovement() { return user_camera_movement; }
ofFbo& Application::getSceneFbo() { return sceneFbo; }
ofPixels& Application::getFboPixels() { return fboPixels; }
float Application::getTimeCurrent() const { return time_current; }
float Application::getTimeElapsed() const { return time_elapsed; }
float Application::getTimeLast() const { return time_last; }
bool Application::getIsDrawingMode() const { return isDrawingMode; }
ofColor Application::getBackgroundColor() const { return backgroundColor; }

// Setters
void Application::setScene(const Scene& scene) { this->scene = scene; }
void Application::setRenderer(const Renderer& renderer) { this->renderer = renderer; }
void Application::setUserCameraMovement(const User_Camera_Movement& user_camera_movement) { this->user_camera_movement = user_camera_movement; }
void Application::setSceneFbo(const ofFbo& sceneFbo) { this->sceneFbo = sceneFbo; }
void Application::setFboPixels(const ofPixels& fboPixels) { this->fboPixels = fboPixels; }
void Application::setTimeCurrent(float time_current) { this->time_current = time_current; }
void Application::setTimeElapsed(float time_elapsed) { this->time_elapsed = time_elapsed; }
void Application::setTimeLast(float time_last) { this->time_last = time_last; }
void Application::setIsDrawingMode(bool isDrawingMode) { this->isDrawingMode = isDrawingMode; }
void Application::setBackgroundColor(const ofColor& backgroundColor) { this->backgroundColor = backgroundColor; }
