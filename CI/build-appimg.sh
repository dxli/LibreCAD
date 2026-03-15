#!/usr/bin/env bash
#
# LibreCAD AppImage Build Script (2025–2026 compatible)
# ──────────────────────────────────────────────────────
# Places ALL .qm files (LibreCAD + Qt translations) into:
#    usr/share/librecad/qm/
#
# Requirements:
#   - Run from LibreCAD git root after successful build
#   - unix/librecad and unix/resources/plugins/*.so must exist
#   - ImageMagick optional (for 256×256 icon)
#
# Usage:
#   ./build-librecad-appimage.sh
#   ./build-librecad-appimage.sh clean     # remove artifacts
#

set -euo pipefail

# ────────────────────────────────────────────────────────────────
# 1. Basic validation & clean mode
# ────────────────────────────────────────────────────────────────

if [[ ! -d "unix" || ! -f "unix/librecad" ]]; then
    cat << 'EOF' >&2
Error: This script must be run from the LibreCAD source root directory
       and unix/librecad must exist.

Please build LibreCAD first, for example:

    qmake
    make -j$(nproc)

EOF
    exit 1
fi

if [[ $# -eq 1 && "$1" == "clean" ]]; then
    rm -rf appdir LibreCAD*.AppImage 2>/dev/null || true
    echo "Cleaned: removed appdir/ and LibreCAD*.AppImage files"
    exit 0
fi

# ────────────────────────────────────────────────────────────────
# 2. Prepare clean appdir structure
# ────────────────────────────────────────────────────────────────

echo "→ Creating fresh appdir structure …"
rm -rf appdir

mkdir -p appdir/usr/{bin,lib/librecad}
mkdir -p appdir/usr/share/{applications,metainfo,doc/librecad}
mkdir -p appdir/usr/share/icons/hicolor/{scalable/apps,256x256/apps}
mkdir -p appdir/usr/share/librecad/{qm,fonts,library,patterns}

# ────────────────────────────────────────────────────────────────
# 3. Strip binaries (reduces size ~10–20 %)
# ────────────────────────────────────────────────────────────────

echo "→ Stripping binaries …"
strip --strip-unneeded unix/librecad unix/resources/plugins/*.so 2>/dev/null || true

# ────────────────────────────────────────────────────────────────
# 4. Copy core application files
# ────────────────────────────────────────────────────────────────

echo "→ Copying executable, plugins and resources …"

cp unix/librecad                        appdir/usr/bin/
cp unix/resources/plugins/*.so          appdir/usr/lib/librecad/   2>/dev/null || true

# ────────────────────────────────────────────────────────────────
# 5. Copy ALL translations → single unified folder
# ────────────────────────────────────────────────────────────────

echo "→ Collecting all .qm files into usr/share/librecad/qm/ …"

qm_target="appdir/usr/share/librecad/qm"
mkdir -p "$qm_target"

# LibreCAD translations
echo "  - LibreCAD own translations"
cp unix/*.qm                          "$qm_target/"  2>/dev/null || true
cp -r unix/resources/qm/*             "$qm_target/"  2>/dev/null || true

# Qt translations — try several common system locations
echo "  - Qt system translations"
qt_paths=(
    "/usr/share/qt5/translations"
    "/usr/share/qt6/translations"
    "/usr/lib/x86_64-linux-gnu/qt5/translations"
    "/usr/lib/qt5/translations"
    "/usr/share/qt/translations"
    "/usr/lib/qt6/translations"
)

qt_source=""
for p in "${qt_paths[@]}"; do
    if [[ -d "$p" && -n "$(compgen -G "$p"/*.qm 2>/dev/null)" ]]; then
        qt_source="$p"
        break
    fi
done

if [[ -n "$qt_source" ]]; then
    echo "      found: $qt_source"
    cp "$qt_source"/*.qm "$qm_target/" 2>/dev/null || true
else
    echo "      WARNING: No Qt translations folder found"
    echo "      Searched: ${qt_paths[*]}"
    echo "      → UI strings will be English only unless Qt .qm files are added manually"
fi

# ────────────────────────────────────────────────────────────────
# 6. Copy remaining support files
# ────────────────────────────────────────────────────────────────

echo "→ Copying fonts, patterns, library, doc, desktop & metainfo …"

cp desktop/librecad.desktop                        appdir/usr/share/applications/
cp desktop/org.librecad.librecad.appdata.xml       appdir/usr/share/metainfo/  2>/dev/null || true

cp -r librecad/support/doc/*                       appdir/usr/share/doc/librecad/     2>/dev/null || true
cp -r librecad/support/{fonts,library,patterns}    appdir/usr/share/librecad/

# Icons
cp CI/librecad.svg                                 appdir/usr/share/icons/hicolor/scalable/apps/librecad.svg

if command -v convert >/dev/null 2>&1; then
    echo "→ Generating 256×256 PNG icon …"
    convert -resize 256x256 CI/librecad.svg \
        appdir/usr/share/icons/hicolor/256x256/apps/librecad.png 2>/dev/null || true
else
    echo "→ Note: ImageMagick not found → no 256×256 PNG icon created"
fi

# ────────────────────────────────────────────────────────────────
# 7. Download latest linuxdeploy tools (if missing)
# ────────────────────────────────────────────────────────────────

echo "→ Downloading/updating linuxdeploy tools …"

tools=(
    "https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage"
    "https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage"
)

for url in "${tools[@]}"; do
    fname=$(basename "$url")
    if [[ ! -f "$fname" || ! -x "$fname" ]]; then
        echo "    downloading $fname"
        wget -q --show-progress -c "$url"
        chmod +x "$fname"
    fi
done

# ────────────────────────────────────────────────────────────────
# 8. Build AppImage
# ────────────────────────────────────────────────────────────────

echo "→ Building AppImage …"

ARCH=x86_64 ./linuxdeploy-x86_64.AppImage \
    --appdir appdir \
    -e appdir/usr/bin/librecad \
    -d appdir/usr/share/applications/librecad.desktop \
    -i appdir/usr/share/icons/hicolor/scalable/apps/librecad.svg \
    --plugin qt \
    --output appimage

# ────────────────────────────────────────────────────────────────
# 9. Final report
# ────────────────────────────────────────────────────────────────

echo ""
echo "═══════════════════════════════════════════════════════════════"
echo "  Build completed"
echo "═══════════════════════════════════════════════════════════════"
ls -lh LibreCAD*.AppImage 2>/dev/null || echo "No .AppImage found – check errors above"

echo ""
echo "Quick validation commands:"
echo "  ./LibreCAD*.AppImage"
echo "  ./LibreCAD*.AppImage --appimage-extract"
echo "  find squashfs-root/usr/share/librecad/qm -name '*.qm' | sort"
echo ""
echo "You should see both librecad_*.qm and qt*_*.qm files in the same folder."
echo ""