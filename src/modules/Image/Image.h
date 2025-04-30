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
    /// Constructeur : r�f�rence � la sc�ne o� seront ajout�es les images
    explicit Image(Scene& scn);

    /// Charge une image depuis \c path et l�ajoute � la sc�ne
    void importImage(const std::string& path);

    /// Positionne toutes les images en grille
    void showImage();

    /// Supprime toutes les images de la sc�ne et lib�re la m�moire
    void clearImages();

    /// Construit l�histogramme R/G/B de l�image s�lectionn�e
    void createHistogram(ImageObject* selectedImage);

    /// Affiche l�histogramme construit par createHistogram()
    void drawHistogram(int x, int y, int width, int height);

    /// Si une et une seule image est s�lectionn�e, dessine son histogramme
    void selectedHistogram(Scene& scene, int x, int y, int w, int h);

    /// Applique un filtre couleur (m�lange RGB+HSB) aux images s�lectionn�es
    void colorFilterSelected(Scene& scene,
                             const ofColor& rgbColor,
                             const ofColor& hsbColor);

    /// Exporte une s�quence de frames (wrapper pour ExportScene)
    void imageExport(const std::string& name,
                     const std::string& extension);

    /// Alias explicite pour imageExport()
    void exportFrames(const std::string& name,
                      const std::string& extension);

    /// Active ou d�sactive le mode export (d�marre/arr�te la capture)
    void triggerExport(bool triggered);

    /// D�finit la dur�e de l�export en secondes
    void setExportDuration(float duration);

    /// Indique si au moins une image est charg�e
    bool hasImage() const { return !images.empty(); }

    /// Acc�s en lecture seule � la liste des objets ImageObject
    const std::vector<ImageObject*>& getImages() const { return images; }

private:
    std::vector<ImageObject*> images;
    Scene& scene;
    ExportScene               exporter;

    std::vector<int>          histogramR, histogramG, histogramB;
};
