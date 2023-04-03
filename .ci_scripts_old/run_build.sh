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
	cp -r res/maps ./build	
	cp -r res/manual ./build
	cd build
	make -j4
	make install
	echo "Creating disk image"
	hdiutil create -volname tiberius -srcfolder tiberius.app -ov -format UDZO tiberius.dmg
	if [[ "$GITHUB_REF" =~ ^refs/tags/v ]]
	then
		zip -r tiberius.zip tiberius.dmg maps manual
	else
		zip -r tiberius.zip tiberius.dmg
	fi
	;;
"appimage")
	cp -r res/maps ./build	
	cp -r res/manual ./build	
	cd build
	make -j4
	make DESTDIR=AppDir install
	cd ..
	./.ci_scripts/package_appimage.sh
	if [[ "$GITHUB_REF" =~ ^refs/tags/v ]]	
	then
		zip -r tiberius.zip . -i tiberius.AppImage maps manual
	else
		zip -r tiberius.zip . -i tiberius.AppImage
	fi
	;;
"linux")
    if [ -d res/packed_assets ]
	then
	    cp -r res/packed_assets ./build/assets
	else
		cp -r res/assets ./build
	fi
	cp -r res/maps ./build	
	cp -r res/manual ./build	
	cd build && make -j4
	if [[ "$GITHUB_REF" =~ ^refs/tags/v ]]
	then
		zip -r tiberius.zip tiberius assets maps manual
	else
		zip -r tiberius.zip tiberius
	fi
	;;
"android")
	cd android
	if [ ! -f tiberius.keystore ]
	then
		COMMAND=assembleDebug
	else
		COMMAND=assembleRelease
	fi
	echo "Running ./gradlew $COMMAND"
	TERM=dumb ./gradlew $COMMAND
	if [ -f tiberius/build/outputs/apk/release/tiberius-release.apk ]
	then
		cp tiberius/build/outputs/apk/release/tiberius-release.apk ../build/tiberius.apk
	fi
	;;
"emscripten")
	cd build && make -j4
	;;
*)
	cd build && make -j4 && make
	;;
esac
