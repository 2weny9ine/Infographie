#pragma once
#include <string>
#include "ofMain.h"

class Scene;

/// ---------------------------------------------------------------------------
/// ExportScene – ray-trace exporter (MIRROR, GLASS, NONE) avec gestion d’opacité
/// Fallback ofSaveScreen si tous les objets sont NONE.
/// ---------------------------------------------------------------------------
class ExportScene {
public:
    explicit ExportScene(Scene& scn);
    ~ExportScene() = default;

    void setExportTriggered(bool triggered);
    void setExportDuration(float seconds);
    void exportFrames(const std::string& base,
                      const std::string& ext);

private:
    void beforeExport();
    void afterExport();
    void renderRaytracedFrame(const std::string& base,
                              const std::string& ext);

    Scene& scene;
    bool  exportTriggered = false;
    float exportDuration = 1.0f;
    int   captureCount = 0;
    float startTime = 0.0f;
    float lastCaptureTime = 0.0f;
};
