#!/bin/bash

echo $(date '+%d.%m.%Y')
echo "Choose an option:"

options=("Add" "Search" "Sort" "Quit")
select opt in "${options[@]}"
do
    case $opt in
        "Add")
            read -p "Group: " group
            read -p "Last name: " last_name
            read -p "Semester: " semester
            read -p "Subject: " subject
            read -p "Grade: " grade
            sh add.sh $group $last_name $semester $subject $grade
            echo "Done."
            ;;
        "Search")
            read -p "String: " string
            sh search.sh $string
            ;;
        "Sort")
            read -p "Column number [1-5]: " column
            sh sort.sh $column
            ;;
        "Quit")
            break
            ;;
        *) echo "Invalid option.";;
    esac
done