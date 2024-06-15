clang++ \
-std=c++17 -w -g \
\
-I/home/jim/vcpkg/installed/x64-linux/include \
-L/home/jim/vcpkg/installed/x64-linux/lib \
$(find . -type f -iregex .*\\.cpp) \
\
-o bin/gin \
\
-lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -ltmxparser -lz -ltinyxml2