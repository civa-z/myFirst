#!/bin/bash

echo $@
if [ $# != 3 ]
    then
    echo Parameter number is $#
fi

name_from=$1
name_to=$2

for i in {1..32}
do
    name_middle=`printf "%02d\n" $i`
    for j in {1..3}
    do
        let j_from=j
        let j_to=j+3
        end_name_from=`printf "%02d\n" $j_from`
        end_name_to=`printf "%02d\n" $j_to`
        from=${name_from}_${name_middle}_${end_name_from}
        to=${name_to}_${name_middle}_${end_name_to}
        echo mv $from $to
        mv $from $to
    done
done
