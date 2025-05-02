#pragma once
#include <string>

class Scene;

class ExportScene {
public:
    explicit ExportScene(Scene& scn);

    void setExportTriggered(bool trig);
    void setExportDuration(float seconds);
    void exportFrames(const std::string& base,
                      const std::string& ext);

private:
    void beforeExport();
    void afterExport();
    void renderRaytracedFrame(const std::string& base,
                              const std::string& ext);
    void updateWindowTitle(float now);

    Scene& scene;
    bool        exportTriggered;
    unsigned    captureCount;
    float       startTime;
    float       lastCaptureTime;
    float       exportDuration;

    std::string originalTitle;
    int         lastPercent;
};
