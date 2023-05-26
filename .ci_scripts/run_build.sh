#!/usr/bin/env bash

set -e

case "$BUILD_TARGET" in
"vita")
	docker exec vitasdk /bin/bash -c "cd build && make -j4"
	;;
"switch")
	docker exec switchdev /bin/bash -c "cd build && make -j4"
	;;
"mac")
	cp -r res/music ./build/assets/music
	cd build
	make -j4
	make install
	echo "Creating disk image"
	hdiutil create -volname 479BC -srcfolder 479BC.app -ov -format UDZO 479BC.dmg
	if [[ "$GITHUB_REF" =~ ^refs/tags/v ]]
	then
		zip -r 479BC.zip 479BC.dmg assets manual
	else
		zip -r 479BC.zip 479BC.dmg
	fi
	;;
"appimage")
	cp -r res/music ./build/assets/music
	cd build
	make -j4
	make DESTDIR=AppDir install
	cd ..
	./.ci_scripts/package_appimage.sh
	if [[ "$GITHUB_REF" =~ ^refs/tags/v ]]	
	then
		zip -r 479BC.zip . -i 479BC.AppImage assets manual
	else
		zip -r 479BC.zip . -i 479BC.AppImage
	fi
	;;
"linux")
    if [ -d res/packed_assets ]
	then
	    cp -r res/packed_assets ./build/assets
	else
		cp -r res/assets ./build
	fi
	cp -r res/music ./build/assets/music
	cd build && make -j4
	if [[ "$GITHUB_REF" =~ ^refs/tags/v ]]
	then
		zip -r 479BC.zip 479BC assets manual
	else
		zip -r 479BC.zip 479BC
	fi
	;;
"android")
	cd android
	if [ ! -f 479BC.keystore ]
	then
		COMMAND=assembleDebug
	else
		COMMAND=assembleRelease
	fi
	echo "Running ./gradlew $COMMAND"
	TERM=dumb ./gradlew $COMMAND
	if [ -f 479BC/build/outputs/apk/release/479BC-release.apk ]
	then
		cp 479BC/build/outputs/apk/release/479BC-release.apk ../build/479BC.apk
	fi
	;;
"emscripten")
	cd build && make -j4
	;;
*)
	cd build && make -j4 && make
	;;
esac
