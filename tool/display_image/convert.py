from PIL import Image
import sys

img = Image.open(sys.argv[1])
out = open(sys.argv[2], "wb")

img = img.convert("RGB")
# weird workaround
#out.write(bytes([0]))

width, height = img.size
assert(width == 256)
assert(height == 256)

pixels = img.load()
for y in range(height):
    for x in range(width):
        (r, g, b) = pixels[x, y]
        color = 0x8000 | ((b >> 3) << 10) | ((g >> 3) << 5) | (r >> 3)
        out.write(bytes([color & 0xFF, color >> 8]))
        
out.close()