#!/bin/bash
# set parameter
edge=(1 5 9 13 17)
eyeb=(18 20 22 23 25 27) #eyeblow
eyel=(37 38 39 40 41 42) #eye(L)
eyer=(43 44 45 46 47 48) #eye(R)
nose=(28 31 32 34 36)    
mouth=(49 52 55 58)
offset=3

newland=("${edge[@]} ${eyeb[@]} ${eyel[@]} ${eyer[@]} ${nose[@]} ${mouth[@]}")
for i in ${newland[@]}; do
  setline="${setline} -e $((i+$offset))p"
done

if [ $# -ne 3 ]; then
  echo "Thin out Face Landmark (.pts)"
  echo "Usage: ./thinout.sh [path_prefix] [out_folder] [img_ext]"
else
  path_prefix=${1%/}
  out_prefix=${2%/}
  img_ext=$3
  files=(`ls -1 ${path_prefix}/*.pts | xargs -i basename {} .pts`)
  if [ ! -d ${path_prefix} ]; then
    echo " <ERROR> ${path_prefix} is not found, aborded."
    exit -1
  fi
  if [ -d ${out_prefix} ]; then
    read -p "Erase ${out_prefix} ? > " str
    case "$str" in
      [Yy]|[Yy][Ee][Ss])
        rm -rf ${out_prefix}
        mkdir ${out_prefix}
        ;;
      [Nn]|[Nn][Oo])
        echo "Aborted."
        exit -1;
        ;;
      *)
      echo "Aborted"
      exit -1;;
    esac
  else
    mkdir ${out_prefix}
  fi
    baseatL="$((${#edge[@]}+${#eyeb[@]}))"
    baseatR="$((${#edge[@]}+${#eyeb[@]}+${#eyel[@]}))"
    echo -e "sed ${setline} '\${inpts}' >> '\${outpts}'"
    sleep 3
    for i in "${files[@]}" ; do
      inpts="${path_prefix}/${i}.pts"
      outpts="${out_prefix}/${i}.pts"
      echo "version: 1" > ${outpts}
      echo "n_points:  32" >> ${outpts}
      echo "{" >> ${outpts}
      sedcom="sed -n ${setline}"
      echo "${inpts} >> ${outpts}"
      ${sedcom} ${inpts} >> ${outpts}
      echo -n "}" >> ${outpts}
      cp "${path_prefix}/${i}.${img_ext}" "${out_prefix}/"
    done
      echo -e "done.\n"
      echo -e "\e[31m<Important>\e[m to Train, Replace Below"
      echo -e "\e[36mdouble\e[m \e[33mCalculateError\e[m(\e[32mcv::Mat_<double>&\e[m \e[36mground_truth_shape\e[m, \e[32mcv::Mat_<double>&\e[m \e[36mpredicted_shape\e[m) ( in utils.cpp )"
      echo -e "\ttemp = \e[36mground_truth_shape.\e[32mrowRange\e[m(\e[31m${baseatL}, $((${baseatL}+${#eyel[@]}-1))\e[m)-\e[36mground_truth_shape.\e[32mrowRange\e[m(\e[31m${baseatR}, $((${baseatR}+${#eyel[@]}-1))\e[m);"
fi
