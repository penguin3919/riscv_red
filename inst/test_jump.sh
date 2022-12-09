#!/bin/bash

sed -i '' 's/\t/ /g' $1
sed -i '' 's/,/, /g' $1

line=32
arr=("20011538" "20011380" "2001140c")
linen=()
#sed -n 4p $1
line_num=$(sed -n '$=' $1)
#echo $line_num
string=$(sed -n "$line"p $1)
string2=($string)
#echo ${string2[5]}
    #if [[ ${string2[5]} =~ '2001'* ]]
        #echo ${string2[5]}
    #then
        #echo "True"
    #fi

#echo ${string2[5]}

for (( i=1; i<="$line_num"; i++  ))
do
    string=$(sed -n "$i"p $1)
    string2=($string)
    #if [[ ${string2[5]} =~ '2001'* ]]||[[ ${string2[3]} =~ '2001'* ]]
        #echo ${string2[5]}
    if [[ ${string2[5]} =~ '2001'* ]]
    then
        #echo ${string2[5]}
        arr+=("${string2[5]}")
    fi

    if [[ ${string2[3]} =~ '2001'* ]]
    then
        #echo ${string2[3]}
        arr+=("${string2[3]}")
    fi
done

arr=($(echo "${arr[@]}" |tr ' ' '\n'| sort -u|tr '\n' ' '))
arr_size=${#arr[@]}
echo "$arr_size"

#for (( i=0; i<"$arr_size"; i++ ))
: '
for i in "$arr_size"
do
    echo $i
done
 '
 #1
 for ((i=0; i<arr_size; i++))
do

for (( k=1; k<="$line_num"; k++  ))
do
    string=$(sed -n "$k"p $1)
    string2=($string)
    #echo ${string2[0]}
    if [[ ${string2[0]} =~ ${arr[$i]} ]]
    then
        #echo ${string2[3]}
        linen+=($k)
        break
    fi
done
done
lin_size=${linen[@]}
#echo "$arr_size"
#for (( i=0; i<"$arr_size"; i++ ))
for i in "$lin_size"
do
    echo $i
done
#2
for ((i=0; i<arr_size; i++))
do
sed -i '' "s/${arr[$i]}/"jump$i"/g" $1
done

#3
lin_size=${#linen[@]}
for ((i=0; i<lin_size; i++))
do
    string=$(sed -n "${linen[$i]}"p $1)
    string2=($string)
    string3="jump"$i": "${string2[2]}
    str4=${linen[$i]}

    sed -i '' ""$str4"s/${string2[2]}/$string3/" $1

done

#hex

hex=()

for (( i=1; i<="$line_num"; i++ ))
do
    string=$(sed -n "$i"p $1)
    string2=($string)
    for (( k=4; k<10; k++ ))
    do
    if [[ ${string2[k]} =~ '0x'[[:xdigit:]] ]]
    then
        #echo ${string2[5]}
        hex+=("${string2[k]}")
        break
    fi
done

done

hex_line=${#hex[@]}
#echo ${hex[@]}

for (( i=0; i<hex_line; i++ ))
do
    hex_temp=${hex[$i]}
    hex_temp=$(($hex_temp))
    #echo $hex_temp
    sed -i '' "s/${hex[$i]}/$hex_temp/" $1
done

#replace

for (( i=1; i<="$line_num"; i++  ))
do
    string=$(sed -n "$i"p $1)
    string2=($string)


    for (( k=2; k<10; k++ ))
   #mv
    do
        #echo ${string2[k]}
    if [[ ${string2[k]} =~ mv  ]]
    then
        #echo ${string2[5]}
        #echo ${string2[k]}
        string3=${string2[k+1]}" x0,"
        sed -i '' ""$i"s/mv/add/" $1
        sed -i '' ""$i"s/${string2[k+1]}/$string3/" $1
        #echo $string3
        break
    fi
   #li
    if [[ ${string2[k]} =~ ^[l][i] ]]
    then
        #echo ${string2[5]}
        #echo ${string2[k]}
        string3=${string2[k+1]}" x0,"
        sed -i '' ""$i"s/li/addi/" $1
        sed -i '' ""$i"s/${string2[k+1]}/$string3/" $1
        #echo $string3
        break
    fi
    #ret
    if [[ ${string2[k]} =~ ret  ]]
    then
        #echo ${string2[5]}
        #echo ${string2[k]}
        string3="jalr x0, 0(x1)"
        sed -i '' ""$i"s/${string2[k]}/$string3/" $1
        #echo $string3
        break
    fi
   #j
   if [[ ${string2[k]} =~ [j]$ ]]
    then
        #echo ${string2[5]}
        #echo ${string2[k]}
        string3="jal x0,"
        sed -i '' ""$i"s/${string2[k]}/$string3/" $1
        #echo $string3
        break
    fi
done
done

sed -i '' 's/zero/x0/g' $1
sed -i '' 's/sp/x2/g' $1
sed -i '' 's/gp/x3/g' $1
sed -i '' 's/ra/x1/g' $1
sed -i '' 's/t0/x5/g' $1
sed -i '' 's/t1/x6/g' $1
sed -i '' 's/t2/x7/g' $1
sed -i '' 's/s0/x8/g' $1
sed -i '' 's/s1/x9/g' $1
sed -i '' 's/a0/x10/g' $1
sed -i '' 's/a1/x11/g' $1
sed -i '' 's/a2/x12/g' $1
sed -i '' 's/a3/x13/g' $1
sed -i '' 's/a4/x14/g' $1
sed -i '' 's/a5/x15/g' $1
sed -i '' 's/a6/x16/g' $1
sed -i '' 's/a7/x17/g' $1
sed -i '' 's/s2/x18/g' $1
sed -i '' 's/s3/x19/g' $1
sed -i '' 's/s4/x20/g' $1
sed -i '' 's/s5/x21/g' $1
sed -i '' 's/s6/x22/g' $1
sed -i '' 's/s7/x23/g' $1
sed -i '' 's/s8/x24/g' $1
sed -i '' 's/s9/x25/g' $1
sed -i '' 's/s10/x26/g' $1
sed -i '' 's/s11/x27/g' $1
sed -i '' 's/t3/x28/g' $1
sed -i '' 's/t4/x29/g' $1
sed -i '' 's/t5/x30/g' $1
sed -i '' 's/t6/x31/g' $1

