#!/bin/bash

set -e

mkdir deploy
build_dir="$(pwd)/build"

VERSION=$(cat res/version.txt)
if [[ "$GITHUB_REF" =~ ^refs/tags/v ]]
then
  REPO=release
elif [[ "$GITHUB_REF" == "refs/heads/master" ]]
then
  REPO=development
elif [[ "$GITHUB_REF" == "refs/heads/release" ]]
then
  REPO=experimental
elif [[ "$GITHUB_REF" =~ ^refs/pull/ ]]
then
  PR_ID=${GITHUB_REF##refs/pull/}
  PR_ID=${PR_ID%%/merge}
  VERSION=pr-$PR_ID-$VERSION
else
  echo "Unknown branch type $GITHUB_REF - skipping upload"
  exit
fi

DEPLOY_FILE=
case "$DEPLOY" in
"linux")
  PACKAGE=linux
  DEPLOY_FILE=tiberius-$VERSION-linux-x86_64.zip
  cp "${build_dir}/tiberius.zip" "deploy/$DEPLOY_FILE"
  ;;
"vita")
  PACKAGE=vita
  DEPLOY_FILE=tiberius-$VERSION-vita.vpk
  cp "${build_dir}/tiberius.vpk" "deploy/$DEPLOY_FILE"
  ;;
"switch")
  PACKAGE=switch
  DEPLOY_FILE=tiberius-$VERSION-switch.nro
  cp "${build_dir}/tiberius.nro" "deploy/$DEPLOY_FILE"
  ;;
"appimage")
  PACKAGE=linux-appimage
  DEPLOY_FILE=tiberius-$VERSION-linux.AppImage
  cp "${build_dir}/tiberius.AppImage" "deploy/$DEPLOY_FILE"
  ;;
"mac")
  PACKAGE=mac
  DEPLOY_FILE=tiberius-$VERSION-mac.dmg
  cp "${build_dir}/tiberius.dmg" "deploy/$DEPLOY_FILE"
  ;;
"android")
  PACKAGE=android
  if [ -f "${build_dir}/tiberius.apk" ]
  then
    DEPLOY_FILE=tiberius-$VERSION-android.apk
    cp "${build_dir}/tiberius.apk" "deploy/$DEPLOY_FILE"
  fi
  ;;
"emscripten")
  PACKAGE=emscripten
  if [ -f "${build_dir}/tiberius.html" ]
  then
    DEPLOY_FILE=tiberius-$VERSION-emscripten.html
    cp "${build_dir}/tiberius.html" "deploy/$DEPLOY_FILE"
  fi
  ;;
*)
  echo "Unknown deploy type $DEPLOY - skipping upload"
  exit
  ;;
esac

if [ ! -z "$SKIP_UPLOAD" ]
then
  echo "Build is configured to skip deploy - skipping upload"
  exit
fi

if [ -z "$REPO" ] || [ -z "$DEPLOY_FILE" ]
then
  echo "No repo or deploy file found - skipping upload"
  exit
fi

if [ -z "$UPLOAD_TOKEN" ]
then
  echo "No upload token found - skipping upload"
  exit
fi

curl -u "$UPLOAD_TOKEN" -T deploy/$DEPLOY_FILE https://tiberius.josecadete.net/upload/$REPO/$PACKAGE/$VERSION/$DEPLOY_FILE
echo "Uploaded. URL: https://tiberius.josecadete.net/$REPO.html"
