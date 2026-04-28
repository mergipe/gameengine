BUILD_TYPE=release
if [ -n "$1" ]; then
    BUILD_TYPE="$1"
fi
rm -rf build
cmake --preset "$BUILD_TYPE" -S .
cmake --build --preset "$BUILD_TYPE"
