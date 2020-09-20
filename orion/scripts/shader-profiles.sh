#!/bin/bash

shader_type() {
	if [[ $1 = "v" ]]; then echo "vertex";
	elif [[ $1 = "f" ]]; then echo "fragment";
	elif [[ $1 = "c" ]]; then echo "compute";
	else echo "<unknown>";
	fi
}

shader_prefix() {
	if [[ $1 = "v" ]]; then echo "vs";
	elif [[ $1 = "f" ]]; then echo "fs";
	elif [[ $1 = "c" ]]; then echo "cs";
	else echo "<unknown>";
	fi
}

profile_opts() {
	PROFILE=$1
	SHADER_TYPE=$2

	if [[ $PROFILE = "dx9" ]]; then
		if [[ $SHADER_TYPE = "v" ]]; then echo "--platform windows -p vs_3_0 -O 3";
		elif [[ $SHADER_TYPE = "f" ]]; then echo "--platform windows -p ps_3_0 -O 3";
		else echo "<unsupported-shader-type>"
		fi
	
	elif [[ $PROFILE = "dx11" ]]; then
		if [[ $SHADER_TYPE = "v" ]]; then echo "--platform windows -p vs_5_0 -O 3";
		elif [[ $SHADER_TYPE = "f" ]]; then echo "--platform windows -p ps_5_0 -O 3";
		elif [[ $SHADER_TYPE = "c" ]]; then echo "--platform windows -p cs_5_0 -O 1";
		else echo "<unsupported-shader-type>"
		fi

	elif [[ $PROFILE = "glsl" ]]; then
		if [[ $SHADER_TYPE = "v" ]]; then echo "--platform linux -p 120";
		elif [[ $SHADER_TYPE = "f" ]]; then echo "--platform linux -p 120"
		elif [[ $SHADER_TYPE = "c" ]]; then echo "--platform linux -p 430"
		else echo "<unsupported-shader-type>"
		fi

	elif [[ $PROFILE = "metal" ]]; then
		if [[ $SHADER_TYPE = "v" ]]; then echo "--platform osx -p metal"
		elif [[ $SHADER_TYPE = "f" ]]; then echo "--platform osx -p metal"
		elif [[ $SHADER_TYPE = "c" ]]; then echo "--platform osx -p metal"
		else echo "<unsupported-shader-type>"
		fi

	else
		echo "<unknown-profile>"
	fi
}

