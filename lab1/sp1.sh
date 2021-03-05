#!/bin/bash
echo "Создание образа CD-диска"
echo "С помощью данного скрипта пользователь может создать образ CD-диска на основе выбранного каталога"
echo "Разработчик: Антипов Родион 727-1"
while true
do
	while true
	do
		echo "Введите путь к каталогу:"
		read dirPath
		if [ -d $dirPath ]
		then
			break
		else
			echo "Каталог не найден!"
		fi
	done
	echo "Введите название образа:"
	read imageName
	if [ -f $imageName.iso ]
	then
		echo "Файл $imageName.iso уже существует, образ будет назван $imageName$(date +%Y-%m-%d-%H-%M).iso"
		imageName="$imageName$(date +%Y-%m-%d-%H-%M)"
	fi
	imageName="$imageName.iso"
	if genisoimage -o $imageName $dirPath
	then
		echo "Образ создан"
	else
		echo "При создании образа возникла ошибка"
	fi
	echo "Образы в текущем каталоге:"
	ls *.iso
	echo "Монтируем образ $imageName"
	mkdir /mnt/iso_image
	mount $imageName /mnt/iso_image
	echo "Содержимое каталога, на основе которого был создан образ:"
	ls $dirPath
	echo "Содержимое образа $imageName:"
	ls /mnt/iso_image
	umount /mnt/iso_image
	rmdir /mnt/iso_image
	echo "Завершить выполнения скрипта (y/n)"
	read reply
	if [ $reply = "y" ]
	then
		break
	fi
done
