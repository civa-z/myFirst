

if [[ $# != 2 ]]; then
    echo Wrong parameter number: $0 $*
    echo Examples:
    echo "    ./reprocess_result_data.sh LabeledData -1"
    exit
fi

sourcefolder=${1%/*}
destinationfolder=${sourcefolder}_1
removeID=$2
echo sourcefolder: $sourcefolder
echo destinationfolder: $destinationfolder
echo removeID: $removeID
echo


function reprocess_result()
{
    sf=$1
    df=$2
    sf_labelfile=$1/label.txt
    df_labelfile=$2/label.txt
    remove_id=$3
    mkdir -p $df
    echo "Start process: "$sf | tee -a log.txt
    cat $sf_labelfile | sed "s///g" | awk -v awk_sf=$sf -v awk_df=$df -v awk_var=$remove_id '
        BEGIN{counter=0; delete_number=0;}
        {
            if ($1 != awk_var)
            {
                counter++;
                cnn14_file_r="0000"FNR;
                cnn14_file_r=substr(cnn14_file_r, length(cnn14_file_r) - 4);
                cnn14_file_d="0000"counter;
                cnn14_file_d=substr(cnn14_file_d, length(cnn14_file_d) - 4);
                cmd="cp "awk_sf"/"cnn14_file_r".txt "awk_df"/"cnn14_file_d".txt";
                system(cmd);
            }
            else
            {
                delete_number++;
                print "delete index: "FNR;
            }
        }
        END{print "Total deleted number: "delete_number"\n"; }' | tee -a log.txt

    sed "/^"$remove_id"/g" $sf_labelfile | grep -v "^$" > $df_labelfile
}


rm log.txt
for subject in `ls $sourcefolder`
do
    for gesture in `ls ${sourcefolder}/${subject}`
    do
        for repeat in `ls ${sourcefolder}/${subject}/${gesture}`
        do
            sf=${sourcefolder}/${subject}/${gesture}/${repeat}
            df=${destinationfolder}/${subject}/${gesture}/${repeat}
            reprocess_result $sf $df $removeID
        done
    done
done

