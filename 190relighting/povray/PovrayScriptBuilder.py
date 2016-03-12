# import library for working with images
# from __future__ import print_function	# for debugging
from PIL import Image
import numpy as np

# string to write
povScript = "#include \"colors.inc\" \n\
#include \"textures.inc\" \n\
camera {{ \n\
    location <0, 2, -3> \n\
    look_at <0, 1, 3> \n\
}} \n\
sphere {{ \n\
    <0, 1, 3> 2 \n\
    texture {{ \n\
        pigment {{color Blue}} \n\
    }} \n\
}} \n\
plane {{ \n\
  <0, -8, 0>, -1 //surface normal, displacement \n\
  //alternative: y, -1 \n\
  pigment {{ \n\
    hexagon \n\
    pigment {{ Jade }}, \n\
    pigment {{ White_Marble }} \n\
  }} \n\
}} \n\
light_source {{ \n\
  <{}, {}, {}> \n\
  color <{}, {}, {}> \n\
}}"

cubemap_back = Image.open("PRTCubemap1/cubemap_back.bmp")
cubemap_front = Image.open("PRTCubemap1/cubemap_front.bmp")
cubemap_bottom = Image.open("PRTCubemap1/cubemap_bottom.bmp")
cubemap_top = Image.open("PRTCubemap1/cubemap_top.bmp")
cubemap_right = Image.open("PRTCubemap1/cubemap_right.bmp")
cubemap_left = Image.open("PRTCubemap1/cubemap_left.bmp")

# offset to make even number of pixels centered
offset = 0.5

# counter
counter = 0

length = cubemap_left.size[0]
lo = -1 * length / 2 + 0.5
hi = length / 2 - 0.5

# loop through top image
y = hi
rgb_im = cubemap_top.convert('RGB')

for x in np.arange(lo, hi + 1, 1):
	for z in np.arange(lo, hi + 1, 1):
		r, g, b = rgb_im.getpixel((x + hi, z + hi))
		#clamp colors
		r /= 255.0
		g /= 255.0
		b /= 255.0
		str = povScript.format(x, y, z, r, g, b)
		
		# open a file object to write to
		filename = "POVScripts/img{0:0>6d}.pov".format(counter)
		file = open(filename, 'w+')
		file.write(str)
		file.close()
		counter += 1

# loop through bottom
y = lo
rgb_im = cubemap_bottom.convert('RGB')

for x in np.arange(lo, hi + 1, 1):
	for z in np.arange(lo, hi + 1, 1):
		r, g, b = rgb_im.getpixel((x + hi, z + hi))
		#clamp colors
		r /= 255.0
		g /= 255.0
		b /= 255.0
		str = povScript.format(x, y, z, r, g, b)
		
		# open a file object to write to
		filename = "POVScripts/img{0:0>6d}.pov".format(counter)
		file = open(filename, 'w+')
		file.write(str)
		file.close()
		counter += 1
		
# loop through left
x = lo
rgb_im = cubemap_left.convert('RGB')

for y in np.arange(lo, hi + 1, 1):
	for z in np.arange(lo, hi + 1, 1):
		r, g, b = rgb_im.getpixel((y + hi, z + hi))
		#clamp colors
		r /= 255.0
		g /= 255.0
		b /= 255.0
		str = povScript.format(x, y, z, r, g, b)
		
		# open a file object to write to
		filename = "POVScripts/img{0:0>6d}.pov".format(counter)
		file = open(filename, 'w+')
		file.write(str)
		file.close()
		counter += 1
		
# loop through right
x = hi
rgb_im = cubemap_right.convert('RGB')

for y in np.arange(lo, hi + 1, 1):
	for z in np.arange(lo, hi + 1, 1):
		r, g, b = rgb_im.getpixel((y + hi, z + hi))
		#clamp colors
		r /= 255.0
		g /= 255.0
		b /= 255.0
		str = povScript.format(x, y, z, r, g, b)
		
		# open a file object to write to
		filename = "POVScripts/img{0:0>6d}.pov".format(counter)
		file = open(filename, 'w+')
		file.write(str)
		file.close()
		counter += 1

# loop through back
z = hi
rgb_im = cubemap_back.convert('RGB')

for x in np.arange(lo, hi + 1, 1):
	for y in np.arange(lo, hi + 1, 1):
		r, g, b = rgb_im.getpixel((x + hi, y + hi))
		#clamp colors
		r /= 255.0
		g /= 255.0
		b /= 255.0
		str = povScript.format(x, y, z, r, g, b)
		
		# open a file object to write to
		filename = "POVScripts/img{0:0>6d}.pov".format(counter)
		file = open(filename, 'w+')
		file.write(str)
		file.close()
		counter += 1
		
# loop through front
z = lo
rgb_im = cubemap_front.convert('RGB')

for x in np.arange(lo, hi + 1, 1):
	for y in np.arange(lo, hi + 1, 1):
		r, g, b = rgb_im.getpixel((x + hi, y + hi))
		#clamp colors
		r /= 255.0
		g /= 255.0
		b /= 255.0
		str = povScript.format(x, y, z, r, g, b)
		
		# open a file object to write to
		filename = "POVScripts/img{0:0>6d}.pov".format(counter)
		file = open(filename, 'w+')
		file.write(str)
		file.close()
		counter += 1