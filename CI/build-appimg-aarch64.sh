#!/usr/bin/env bash
#
# LibreCAD AppImage Build Script – arm64/aarch64 (updated 2026)
# ──────────────────────────────────────────────────────────────
# All .qm files — both LibreCAD's and Qt's — go into:
#    usr/share/librecad/qm/
#
# Run on arm64 system after build (qmake && make).
# Supports: clean mode
#

set -euo pipefail

# ────────────────────────────────────────────────────────────────
# 1. Validation & clean mode
# ────────────────────────────────────────────────────────────────

if [[ ! -d "unix" || ! -f "unix/librecad" ]]; then
    cat << 'EOF' >&2
Error: Run from LibreCAD git root with unix/librecad already built.

Example:
    qmake
    make -j$(nproc)
EOF
    exit 1
fi

if [[ $(uname -m) != "aarch64" ]]; then
    echo "Warning: Script designed for aarch64 — current: $(uname -m)"
    echo "Continue? (y/N)"
    read -r ans && [[ "$ans" != [yY] ]] && exit 1
fi

if [[ $# -eq 1 && "$1" == "clean" ]]; then
    rm -rf appdir LibreCAD*.AppImage 2>/dev/null || true
    echo "Cleaned appdir/ and *.AppImage"
    exit 0
fi

# ────────────────────────────────────────────────────────────────
# 2. Prepare appdir
# ────────────────────────────────────────────────────────────────

echo "→ Creating fresh appdir …"
rm -rf appdir

mkdir -p appdir/usr/{bin,lib/librecad}
mkdir -p appdir/usr/share/{applications,metainfo,doc/librecad}
mkdir -p appdir/usr/share/icons/hicolor/{scalable/apps,256x256/apps}
mkdir -p appdir/usr/share/librecad/{qm,fonts,library,patterns}

# ────────────────────────────────────────────────────────────────
# 3. Strip
# ────────────────────────────────────────────────────────────────

echo "→ Stripping …"
strip --strip-unneeded unix/librecad unix/resources/plugins/*.so 2>/dev/null || true

# ────────────────────────────────────────────────────────────────
# 4. Core files
# ────────────────────────────────────────────────────────────────

echo "→ Copying binary + plugins …"
cp unix/librecad                        appdir/usr/bin/
cp unix/resources/plugins/*.so          appdir/usr/lib/librecad/  2>/dev/null || true

# ────────────────────────────────────────────────────────────────
# 5. Translations → unified qm/ folder
# ────────────────────────────────────────────────────────────────

qm_dir="appdir/usr/share/librecad/qm"
mkdir -p "$qm_dir"

echo "→ Copying translations to $qm_dir …"

# A. LibreCAD translations
echo "  • LibreCAD .qm files"
find unix -type f -iname "*.qm" -exec cp -v {} "$qm_dir/" \; 2>/dev/null || true
cp -rv unix/resources/qm/* "$qm_dir/" 2>/dev/null || true

# B. Qt translations — force copy to same folder
echo "  • Qt system .qm files"
qt_search_paths=(
    "/usr/share/qt5/translations"
    "/usr/lib/aarch64-linux-gnu/qt5/translations"
    "/usr/share/qt/translations"
    "/usr/lib/qt5/translations"
)

qt_found=""
for p in "${qt_search_paths[@]}"; do
    if [[ -d "$p" ]]; then
        shopt -s nullglob
        qm_files=("$p"/*.qm)
        if (( ${#qm_files[@]} > 0 )); then
            qt_found="$p"
            echo "      → found in $p (${#qm_files[@]} files)"
            cp -v "$p"/*.qm "$qm_dir/" 2>/dev/null || true
            break
        fi
    fi
done

if [[ -z "$qt_found" ]]; then
    echo "      WARNING: No Qt translations folder found"
    echo "      Searched: ${qt_search_paths[*]}"
    echo "      → Qt UI may stay in English — check your distro's qtbase5-dev / qttranslations5 package"
fi

# ────────────────────────────────────────────────────────────────
# 6. Other assets
# ────────────────────────────────────────────────────────────────

echo "→ Copying resources, desktop, icons …"

cp desktop/librecad.desktop             appdir/usr/share/applications/
cp desktop/org.librecad.librecad.appdata.xml appdir/usr/share/metainfo/ 2>/dev/null || true

cp -r librecad/support/doc/*            appdir/usr/share/doc/librecad/     2>/dev/null || true
cp -r librecad/support/{fonts,library,patterns} appdir/usr/share/librecad/

cp CI/librecad.svg                      appdir/usr/share/icons/hicolor/scalable/apps/librecad.svg

if command -v convert >/dev/null 2>&1; then
    echo "→ 256×256 icon …"
    convert -resize 256x256 CI/librecad.svg \
        appdir/usr/share/icons/hicolor/256x256/apps/librecad.png || true
    ls -l appdir/usr/share/icons/hicolor/256x256/apps/librecad.png
fi

ls -l appdir/usr/share/applications/librecad.desktop

# ────────────────────────────────────────────────────────────────
# 7. linuxdeploy tools (aarch64)
# ────────────────────────────────────────────────────────────────

echo "→ Fetching linuxdeploy (aarch64) …"

tools=(
    "https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-aarch64.AppImage"
    "https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-aarch64.AppImage"
)

for url in "${tools[@]}"; do
    f=$(basename "$url")
    [[ -x "$f" ]] || {
        echo "  Downloading $f"
        wget -c --no-verbose "$url" || exit 2
        chmod +x "$f"
    }
done

# ────────────────────────────────────────────────────────────────
# 8. Build AppImage
# ────────────────────────────────────────────────────────────────

echo "→ Building AppImage …"

ARCH=aarch64 ./linuxdeploy-aarch64.AppImage \
    --appdir appdir \
    -e appdir/usr/bin/librecad \
    -d appdir/usr/share/applications/librecad.desktop \
    -i appdir/usr/share/icons/hicolor/scalable/apps/librecad.svg \
    --plugin qt \
    --output appimage

# ────────────────────────────────────────────────────────────────
# 9. Report
# ────────────────────────────────────────────────────────────────

echo ""
echo "═══════════════════════════════════════════════"
echo "  Done – arm64 AppImage ready"
echo "═══════════════════════════════════════════════"
ls -lh LibreCAD*.AppImage 2>/dev/null || echo "No AppImage – check logs"

echo ""
echo "Verify .qm files are together:"
echo "  ./LibreCAD*.AppImage --appimage-extract"
echo "  find squashfs-root/usr/share/librecad/qm -name '*.qm' | sort -u"
echo ""
echo "You should see both librecad_* / yourapp_*.qm  and  qtbase_* / qt_*.qm"
