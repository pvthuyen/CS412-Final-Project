import glob
import cv2
import numpy as np

for file_path in glob.glob("groundtruths/*_query.txt"):
	f = open(file_path, 'r+')
	line = f.read().split()
	image_path, x, y, w, h = line[0], int(float(line[1])), int(float(line[2])), int(float(line[3])), int(float(line[4]))
	image_path = "images/" + image_path[5:] + ".jpg"
	image = cv2.imread(image_path)
	image = image[y:h, x:w, :]
	output_path = file_path.split('/')
	output_path = "queries/" + output_path[1].replace('txt', 'png')
	cv2.imwrite(output_path, image)