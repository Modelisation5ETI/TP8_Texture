##*MultiTexturing*

-----
**CMake Build instructions**

-----
 ***WARNING : NVIDIA driver is hardcoded in CMakeLists.txt***
 - Out-of-source build :
 ```
git clone git@github.com:Modelisation5ETI/TP8_Texture.git && \
mkdir TP8_Texture-build && cd TP8_Texture-build  && \
cmake ../TP8_Texture  && \
make -j
 
 ```
  - QtCreator :
 ```
git clone git@github.com:Modelisation5ETI/TP8_Texture.git  && \
mkdir TP8_Texture-build && cd TP8_Texture-build  && \
qtcreator ../TP8_Texture
 
 ```
  *Then configure project using QtCreator API* : 
   - *Set the build directory to TP8_Texture-build/*
   - *Run CMake and build project*


-----
**Implementation**

-----

 - ### Parallax
 <img src="./Screenshots/parallax.png" alt="Parallax" width="200" height="200" />

 ```
 ./Parallax
 ```
 Use arrows UP/DOWN to increase/decrease depth.
 
 - ### Bidon
 <img src="./Screenshots/bidon.png" alt="Bidon" width="200" height="200" />

 ```
 ./Bidon
 ```
 Use arrow LEFT/RIGHT to rotate camera.

 - ### Caillou
 <img src="./Screenshots/caillou.png" alt="Caillou" width="200" height="200" />

 ```
 ./Caillou
 ```
 Use arrow LEFT/RIGHT to rotate camera.

