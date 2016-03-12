import os, os.path

# get number of files in script directory
numImages = len(os.listdir('./POVScripts'))

file = open("pov_animate.pov", 'w+')

str = "#if(frame_number={}) \n\
#include \"POVScripts/img{}.pov\"\n\
#end\n"

for i in range(0, numImages):
	file.write(str.format(i + 1, "{0:0>6d}".format(i)))
	
file.close()
