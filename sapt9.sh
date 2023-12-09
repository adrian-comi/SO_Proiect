#!/bin/bash


if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <character>"
    exit 1
fi

character=$1
counter=0

while IFS= read -r line; do               
    [[ $line == *"$character"* ]] &&
    [[ $line =~ ^[[:upper:]] ]] &&            
    [[ $line =~ [a-zA-Z0-9\ \!\?\.]+$ ]] &&    
    [[ $line =~ (\.|\!|\?)$ ]] &&              
    [[ ! $line =~ ,\ și ]] && ((counter++))
done

echo $counter
