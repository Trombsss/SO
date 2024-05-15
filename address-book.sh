#!/bin/bash

file="address-book-database.csv"

if [[ $1 == "view" ]]
then
	column -t -s "," $file | head -n 1
	tail -n +2  $file | column -t -s "," | sort -d -k 1

elif [[ $1 == "search" ]]
then
	IFS=$'\n'

	for line in $(tail -n +2 "$file")
	do
		i=0
		if [[ $(echo "$line" | grep -in "$2") ]]
		then
			echo -e "\n"
			IFS=","
			for row in $(echo "$line")
			do
				i=$(( i+1 ))
				printf $(head -n 1 $file | cut -d"," -f $i | sed 's/./\U&/')
				printf ": $row\n"
			done
			IFS=$'\n'
		fi
	done

elif [[ $1 == "insert" ]]
then
	IFS=$'\n'
	touch fileA.csv
	fileA="fileA.csv"

	echo "Inserire i dati"

        for line in $(head -n 1 "$file")
        do
		i=0
		flag=1
		IFS=","
		for row in $(echo "$line")
		do
			read x

			if [[ $i -ne 3 ]]
			then
				i=$(( i+1 ))
				printf $(head -n 1 "$file" | cut -d"," -f $i | sed 's/./\U&/')
				printf ": "
				printf "$x\n"

				if [[ $i -ne 6 ]]
				then
					printf "%s" "$x," >> "$fileA"
				else
					printf "%s\n" "$x" >> "$fileA"
				fi
			else
				i=$(( i+1 ))
				if grep -q "$x" "$file"
				then
					echo "Mail gia' presente"
					flag=0
					break
				else
					printf $(head -n 1 "$file" | cut -d"," -f $i | sed 's/./\U&/')
                              		printf ": "
                                	printf "$x\n"

                                	if [[ $i -ne 6 ]]
                                	then
                                        	printf "%s" "$x," >> "$fileA"
                                	else
                                        	printf "%s\n" "$x" >> "$fileA"
                                	fi
				fi
			fi
		done

		if (( "$flag" == 1 ))
		then
			cat "%s" $fileA >> "$file"
		fi

		rm -r "$fileA"
		IFS=$'\n'
	done

elif [[ $1 == "delete" ]]
then
flag=0
	for (( c=1; c<=$(tail -n +2 "$file"| wc -l); c++ ))
	do
		linea=$(tail -n +2 "$file" | sed -n "${c}"p)
		if [[ $(echo "$2" | grep -i "@") && $(echo "$linea" | grep -i "$2") ]]
		then
			sed -i "/$linea/d" "$file"
			echo "Deleted"
			flag=1
			break
		fi

	done
if [[ $flag == 0 ]]
then
	echo "Cannot find any record"
fi

fi
