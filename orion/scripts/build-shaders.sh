#!/bin/bash
	
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd $DIR

. shader-profiles.sh

main() {
	echo "Starting shader compilation (`pwd`/`basename $0`)"
	
	SHADER_DIR="../shaders"
	
	ls $SHADER_DIR
	
	for f in $SHADER_DIR/*/ ; do
		if [[ "$f" = "*" ]]; then continue; fi
		if [[ -d "$f" && ! -L "$f" ]]; then
			SHADER_NAME=${f#"$SHADER_DIR/"}
			SHADER_NAME=${SHADER_NAME%"/"}
			
			VS="$SHADER_DIR/$SHADER_NAME/vs_$SHADER_NAME.sc"
			FS="$SHADER_DIR/$SHADER_NAME/fs_$SHADER_NAME.sc"
	
			if [[ -f $VS ]]; then
				build_all_vs_if_present "$SHADER_NAME" "$VS" "$@"
			fi

			if [[ -f $FS ]]; then
				build_all_fs_if_present "$SHADER_NAME" "$FS" "$@"
			fi
		fi
	done
}

shader_type() {
	if [[ $1 = "v" ]]; then echo "vertex";
	elif [[ $1 = "f" ]]; then echo "fragment";
	elif [[ $1 = "c" ]]; then echo "compute";
	else echo "<unknown>";
	fi
}

build_if_present() {
#echo "ARGS- $@"
	SHADER_PROFILE=$1
	SHADER_TYPE_CODE=$2
	SHADER_NAME=$3
	SHADER_PATH=$4
	ARGS="$@"

	SHADER_TYPE=$(shader_type $SHADER_TYPE_CODE)
	SHADER_PREFIX=$(shader_prefix $SHADER_TYPE_CODE)

	if [[ "$ARGS" == *"profile::$SHADER_PROFILE"* ]] || [[ "$ARGS" == *"profile::all"* ]] ; then
		TARGET="../runtime/shaders/$SHADER_PROFILE/${SHADER_PREFIX}_${SHADER_NAME}.bin"
		PROFILE_OPTS=$(profile_opts $SHADER_PROFILE)

		echo "Compiling '$SHADER_NAME' $SHADER_TYPE shader for $SHADER_PROFILE ($SHADER_PATH) to '$TARGET'"
		../../tool-bin/shadercRelease.exe -f "$SHADER_PATH" -o "$TARGET" --type $SHADER_TYPE_CODE -i ../../src/ $PROFILE_OPTS
	fi
}

build_all_if_present() {	
	build_if_present dx9 $@
	build_if_present dx11 $@
	build_if_present glsl $@
	build_if_present metal $@
}


build_all_vs_if_present() {
	build_all_if_present "v" $@
}

build_all_fs_if_present() {
	build_all_if_present "f" $@
}


main "$@"

