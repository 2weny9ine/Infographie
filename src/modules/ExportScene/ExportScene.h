// modules/ExportScene/ExportScene.h
#pragma once

#include <string>

class Scene;

/// Module g�rant l'export (capture d'�cran raytrac�e ou normale) d'une sc�ne OF
class ExportScene {
public:
    explicit ExportScene(Scene& scn);
    ~ExportScene() = default;

    void setExportTriggered(bool triggered);
    void setExportDuration(float duration);

    /// � appeler chaque frame : fait soit un raytrace (si miroir trouv�), 
    /// soit un ofSaveScreen() classique.
    void exportFrames(const std::string& name,
                      const std::string& extension);

private:
    Scene& scene;
    bool  exportTriggered = false;
    float exportDuration = 1.0f;
    int   captureCount = 0;
    float startTime = 0.0f;
    float lastCaptureTime = 0.0f;

    void beforeExport();
    void afterExport();

    void renderRaytracedFrame(const std::string& name,
                              const std::string& extension);
};
