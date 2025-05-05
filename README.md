# Infographie - Application de construction et rendu de scènes 2D/3D

## 🎯 Objectif du projet

Cette application permet de construire, éditer et rendre des scènes visuelles 2D/3D de manière interactive. Développée en **C++** avec **OpenFrameworks** et **OpenGL**, elle offre une interface graphique intuitive pour :

- Manipuler des images, objets 3D et primitives vectorielles
- Appliquer des transformations (translation, rotation, échelle)
- Importer des modèles 3D et images par glisser-déposer
- Gérer les textures, la lumière, les matériaux, la caméra
- Effectuer des rendus avancés (tone mapping, PBR, ray tracing)

## 🧩 Fonctionnalités principales

### 🖼️ Manipulation d’images
- Importation/exportation d’images
- Application de filtres RGB/HSB
- Affichage d’histogrammes
- Sélection multiple et coloration dynamique

### ✏️ Dessins vectoriels
- Création interactive de points, lignes, rectangles, cercles, triangles
- Curseur dynamique selon l’action (move, rotate, resize)
- GUI complète pour configuration (épaisseur, couleurs, fond)

### 🔄 Transformations interactives
- Sélection d’objets ou groupe d’objets
- Translation, rotation, redimensionnement en temps réel
- Graphe de scène avec suppression dynamique

### 🔺 Géométrie et objets 3D
- Primitives : cube, sphère, cylindre, cône
- Importation de modèles : .obj, .fbx, .dae
- Bounding box et duplication d’objets (Ctrl+D)

### 🎥 Caméra
- Contrôle caméra avec clavier/souris
- Caméra liée à l’export d’image

### 🎨 Texture et Filtrage
- Juste le Sphere pour le moment.
- 4 textures disponibles : brique, bois, arbre, damier
- Filtres d’image (emboss, sharpen, edge detect)
- Mappage tonal avec gamma/exposure contrôlés sur les images

### 💡 Illumination
- Shaders : Lambert, Gouraud, Phong, Blinn-Phong
- Lumières multiples : directionnelle, ambiante, spot, souris
- Matériaux : mat, plastique, métallique
- Shader Toon (non-réaliste)

### 🧮 Topologie
- Courbes de Bézier interactives
- Gestion dynamique des points de contrôle

### ⚡ Ray Tracing
- Réflexion, réfraction, Fresnel
- Export HDR avec suivi console en temps réel

### 🔬 Illumination PBR
- Shader Cook-Torrance avec textures PBR
- Métallicité, rugosité, tone mapping intégré

## 🛠️ Technologies

- **Langage** : C++
- **Framework** : OpenFrameworks
- **Rendu** : OpenGL
- **Interface** : ofxDatGui
- **Shaders** : GLSL
- **JSON** : nlohmann/json

## 🚀 Installation & Compilation

1. Télécharger [OpenFrameworks](https://openframeworks.cc/)
2. Cloner le projet :  
   `git clone https://github.com/2weny9ine/Infographie.git`
3. Cloner ofxDatGui dans le dossier `/addons` :  
   `git clone https://github.com/braitsch/ofxDatGui.git`
4. Copier le projet dans `/apps/myApps`
5. Ouvrir avec le **Project Generator**, ajouter :
   - `ofxDatGui`, `ofxGui`, `ofxAssimpModelLoader`
6. Supprimer `ofApp.cpp/.h`, puis compiler avec **Visual Studio** (Windows) ou **Xcode** (macOS)

## 🧱 Architecture du projet

- `Application/` : classe principale (singleton)
- `Scene/` : gestion des objets, transformations, rendu
- `Modules/` : GUI, caméra, curseur, gestion d’image
- `Objects/` : hiérarchie d’objets (ImageObject, Sphere…)
- `Events/` : gestion des interactions (drag, select…)

## 🎥 Vidéo de démonstration
[YouTube Demo](https://youtu.be/HSAwE9ngsjg)


## 📁 Ressources

- Textures : [ambientcg.com](https://ambientcg.com/), [FreePBR](https://freepbr.com)
- Modèles 3D : [Free3D](https://free3d.com)
- Images : [Adobe Stock](https://stock.adobe.com), [Torque News](https://torquenews.com)
- Génération assistée : ChatGPT (pour aide au débogage et structuration)

---

