all: anjalogo-hires.png anjalogo.png index.html

anjalogo.png: ../anjalogo.png
	cp ../anjalogo.png anjalogo.png

anjalogo-hires.png: ../anjalogo-hires.png
	cp ../anjalogo-hires.png anjalogo-hires.png

index.html: html.py index.xml
	./html.py < index.xml | tidy > index.html
