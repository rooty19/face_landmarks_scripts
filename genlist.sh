
#!/bin/bash
path_prefix=$1
img_ext=$2
listname=$3
listfile="${listname}_list.txt"

if [ $# -ne 3 ]; then
	echo "Usage : ./genlist.sh [path_prefix] [img_ext] [listname]"
else
	if [ -e ${listfile} ] ; then
		rm ${listfile}
	fi
	files=(`ls -1 ${path_prefix}/*.pts | xargs -i basename {} .pts`)
	for file_name in "${files[@]}" ; do
		echo "${file_name}.${img_ext} ${file_name}.pts >> ${listfile}"
		echo "${file_name}.${img_ext} ${file_name}.pts" >> ${listfile}
	done
fi
