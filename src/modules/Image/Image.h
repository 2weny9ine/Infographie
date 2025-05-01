// Image.h
#pragma once

#include <vector>
#include <string>
#include "ofMain.h"
#include "Scene.h"
#include "ImageObject.h"
#include "modules/ExportScene/ExportScene.h"

class Image {
public:
    /// Constructeur : référence à la scène où seront ajoutées les images
    explicit Image(Scene& scn);

    /// Charge une image depuis \c path et l’ajoute à la scène
    void importImage(const std::string& path);

    /// Positionne toutes les images en grille
    void showImage();

    /// Supprime toutes les images de la scène et libère la mémoire
    void clearImages();

    /// Construit l’histogramme R/G/B de l’image sélectionnée
    void createHistogram(ImageObject* selectedImage);

    /// Affiche l’histogramme construit par createHistogram()
    void drawHistogram(int x, int y, int width, int height);

    /// Si une et une seule image est sélectionnée, dessine son histogramme
    void selectedHistogram(Scene& scene, int x, int y, int w, int h);

    /// Applique un filtre couleur (mélange RGB+HSB) aux images sélectionnées
    void colorFilterSelected(Scene& scene,
                             const ofColor& rgbColor,
                             const ofColor& hsbColor);

    /// Exporte une séquence de frames (wrapper pour ExportScene)
    void imageExport(const std::string& name,
                     const std::string& extension);

    /// Alias explicite pour imageExport()
    void exportFrames(const std::string& name,
                      const std::string& extension);

    /// Active ou désactive le mode export (démarre/arrête la capture)
    void triggerExport(bool triggered);

    /// Définit la durée de l’export en secondes
    void setExportDuration(float duration);

    /// Indique si au moins une image est chargée
    bool hasImage() const { return !images.empty(); }

    /// Accès en lecture seule à la liste des objets ImageObject
    const std::vector<ImageObject*>& getImages() const { return images; }

private:
    std::vector<ImageObject*> images;
    Scene& scene;
    ExportScene               exporter;

    std::vector<int>          histogramR, histogramG, histogramB;
};
