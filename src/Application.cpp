#include "Application.h"
#include <algorithm>

void Application::setup() {
    ofSetWindowTitle("Infographie");
    ofLog() << "Application démarre...";
    
    scene.setup(&user_camera_movement.camera);
    gui.top_left.setImage(scene.img);

    user_camera_movement.setup(scene);
}

void Application::update() {
    time_current = ofGetElapsedTimef();
    time_elapsed = time_current - time_last;
    time_last = time_current;
    
    user_camera_movement.update(time_elapsed);
    
    scene.update();
}

void Application::draw() {
    scene.draw();
    scene.img->createHistogram();
    gui.top_left.draw();
    
    //topLeftGui.setImage(scene.img);
    //scene.img->drawHistogram(50, ofGetHeight() - 300, 256, 200);
    scene.img->imageExport("exportImage", "png");
}



void Application::windowResized(int w, int h) {
    ofLog() << "Fenêtre redimensionnée : " << w << "x" << h;
}

void Application::keyPressed(int key)
{
    switch (key)
    {
        case OF_KEY_LEFT:
            user_camera_movement.move_left = true;
            break;
        case OF_KEY_UP:
            user_camera_movement.move_forward = true;
            break;
        case OF_KEY_RIGHT:
            user_camera_movement.move_right = true;
            break;
        case OF_KEY_DOWN:
            user_camera_movement.move_backwards = true;
            break;
            
        case 'a':
            user_camera_movement.move_left = true;
            break;
        case 'w':
            user_camera_movement.move_forward = true;
            break;
        case 'd':
            user_camera_movement.move_right = true;
            break;
        case 's':
            user_camera_movement.move_backwards = true;
            break;
        case 'e':
            user_camera_movement.move_upwards = true;
            break;
        case 'q':
            user_camera_movement.move_downwards = true;
            break;
            
        default:
            break;
    }
}

void Application::keyReleased(int key)
{
    switch (key)
    {
        case OF_KEY_LEFT:
            user_camera_movement.move_left = false;
            break;
        case OF_KEY_UP:
            user_camera_movement.move_forward = false;
            break;
        case OF_KEY_RIGHT:
            user_camera_movement.move_right = false;
            break;
        case OF_KEY_DOWN:
            user_camera_movement.move_backwards = false;
            break;
            
        case 'a':
            user_camera_movement.move_left = false;
            break;
        case 'w':
            user_camera_movement.move_forward = false;
            break;
        case 'd':
            user_camera_movement.move_right = false;
            break;
        case 's':
            user_camera_movement.move_backwards = false;
            break;
        case 'e':
            user_camera_movement.move_upwards = false;
            break;
        case 'q':
            user_camera_movement.move_downwards = false;
            break;
        case 'l':
            scene.dispatch_locators(scene.locator_count, std::min(ofGetWidth(), ofGetHeight()));
            break;
        case ' ':
            scene.img->imageExport("exportImage", "png");
            scene.img->setExportTriggered(true);
            break;
        default:
            break;
    }
}

void Application::dragEvent(ofDragInfo dragInfo) {
    ofLogNotice("Application::dragEvent") << "Drag Event: "
    << "Files: " << dragInfo.files.size()
    << ", Position: (" << dragInfo.position.x << ", " << dragInfo.position.y << ")";
    
    DragEvent dragEventHandler;
    
    dragEventHandler.processDragEvent(dragInfo, &scene);
}

void Application::mouseMoved(int x, int y)
{
    scene.mouse_current_x = x;
    scene.mouse_current_y = y;
}

void Application::mouseDragged(int x, int y, int button)
{
    scene.mouse_current_x = x;
    scene.mouse_current_y = y;
}

void Application::mousePressed(int x, int y, int button)
{
    scene.resetSelection();
    
    
    scene.is_mouse_button_pressed = true;
    
    scene.mouse_current_x = x;
    scene.mouse_current_y = y;
    
    scene.mouse_press_x = x;
    scene.mouse_press_y = y;
    
    //Test en envoyant un rayon partant de la caméra vers l'avant à l'infini, renvoyant le premier objet "hit".
    
    //glm::vec3 rayOrigin = user_camera_movement.camera.getPosition();
    //glm::vec3 rayDir = user_camera_movement.camera.screenToWorld(glm::vec3(x, y, 0)) - rayOrigin;
    
    //rayDir = glm::normalize(rayDir);
    
    for (int index = 0; index < scene.locator_buffer_head; ++index)
    {
        
        glm::vec3 locatorPos = glm::vec3(scene.locators[index].position[0], scene.locators[index].position[1], scene.locators[index].position[2]);
        glm::vec3 screenPos = scene.camera->worldToScreen(locatorPos);
        
        float tolerance = 20.0f;
        if (glm::distance(glm::vec2(x, y), glm::vec2(screenPos.x, screenPos.y)) < tolerance) {
            scene.locators[index].isSelected = !scene.locators[index].isSelected;//Toggle la selection
        }
    }
}

void Application::mouseReleased(int x, int y, int button)
{
    scene.is_mouse_button_pressed = false;
    
    scene.mouse_current_x = x;
    scene.mouse_current_y = y;
    
    //Sélection tout ce qui est dans la boite de sélection si celle-ci est assez grande
    float distance = sqrt(pow(x - scene.mouse_press_x, 2) + pow(y - scene.mouse_press_y, 2));
    
    if(distance > 20.0f)
        scene.selectAllInBounds(scene.mouse_press_x, scene.mouse_press_y, x, y);
}

void Application::mouseEntered(int x, int y)
{
    scene.mouse_current_x = x;
    scene.mouse_current_y = y;
}

void Application::mouseExited(int x, int y)
{
    scene.mouse_current_x = x;
    scene.mouse_current_y = y;
}

void Application::exit()
{
    ofLog() << "<app::exit>";
}
