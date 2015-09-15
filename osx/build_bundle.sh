#!/bin/sh

BUNDLE_PATH='bin/Blue Pink Metronome.app'
OUTPUT_PATH="$BUNDLE_PATH/Contents"
BINARY_PATH="$OUTPUT_PATH/MacOS"

mkdir -p "$BINARY_PATH"
cp bin/bpm "$BINARY_PATH"
cp osx/Info.plist "$OUTPUT_PATH"
macdeployqt "$BUNDLE_PATH" -always-overwrite

# fix Qt framework path from the Qt libraries
# macdeployqt would be nicer if it did this by itself

QTDIR="$(pkg-config --libs Qt5Core | cut -d ' ' -f 1 | cut -c 3-)"
MODULES=(QtCore QtGui QtWidgets QtMultimedia QtNetwork)

fixpaths() {
  echo "Fixing framework paths in library $(basename "$1")..."

  for module in "${MODULES[@]}"; do
    install_name_tool -change "$QTDIR/${module}.framework/$module" \
      "@executable_path/../Frameworks/${module}.framework/$module" \
      "$1"
  done 
}

for framework in "${MODULES[@]}"; do
  fixpaths "$OUTPUT_PATH/Frameworks/${framework}.framework/$framework"
done

for plugin in "$OUTPUT_PATH"/PlugIns/*/*.dylib; do
  fixpaths "$plugin"
done
