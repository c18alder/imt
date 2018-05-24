#! /bin/bash

if [[ $# -ne 1 ]] then
	echo "Syntax: $0 <EXTRA_PARAMS>"
	echo "        EXTRA_PARAMS: comma(,)-separated string of equals(=)-separated name-value pairs"
	echo "        Example: A=12,B=123,C=abcd"
	echo "        Limitations: commas not allowed anywhere, equals not allowed in names"
	exit 1
fi

EXTRA_PARAMS_IN="$1"

recompile(){
    local NODE_NAME="$1"
    local NODE_DIR="$2"
    
    export CONTIKI_APP="$NODE_NAME"
    
    cd "$NODE_DIR"   
	make -j clean TARGET=cooja
	
	export LIBNAME="mtype-${CONTIKI_APP}"
	#export LIBNAME="${CONTIKI_APP}"
	
	export AR="ar"
	export AR_COMMAND_1="ar rcf obj_cooja/${LIBNAME}.a"
	export AR_COMMAND_2=""
	export CC="gcc"
	export CLASSNAME="Lib1"
	export COOJA_SOURCEDIRS=""
	export COOJA_SOURCEFILES=""
	export EXTRA_CC_ARGS="-I'/usr/lib/jvm/java-8-oracle/include' -I'/usr/lib/jvm/java-8-oracle/include/linux' -fno-builtin-printf -fPIC"
	export LD="ld"
	export LINK_COMMAND_1="gcc -I'/usr/lib/jvm/java-8-oracle/include' -I'/usr/lib/jvm/java-8-oracle/include/linux' -shared -Wl,-Map=obj_cooja/${LIBNAME}.map -o obj_cooja/${LIBNAME}.cooja"
	export LINK_COMMAND_2=""
	export OBJCOPY="objcopy"
	export SYMBOLS=""
	export EXTRA_PARAMS="${EXTRA_PARAMS_IN}"
		
	#strace -v -s 10000 -e execve make -j ${CONTIKI_APP}.cooja TARGET=cooja --trace
	make -j ${CONTIKI_APP}.cooja TARGET=cooja DEFINES="${EXTRA_PARAMS}"
}

CURRENT_DIR=$(pwd)
recompile "receiver" "${CURRENT_DIR}/../src/nodes/receiver" "${EXTRA_PARAMS_IN}"
recompile "sender" "${CURRENT_DIR}/../src/nodes/sender" "${EXTRA_PARAMS_IN}"

