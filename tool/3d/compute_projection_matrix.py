import math

# Generate the projection matrix
# See http://www.problemkaputt.de/gbatek.htm#ds3dmatrixexamplesprojection
# But actually I used https://www.songho.ca/opengl/gl_projectionmatrix.html#perspective

def toF12(value):
    return round(value * (1 << 12))

def print_add_command(value):
    print("\tGEOM_COMM_MTX_LOAD_4x4 = " + hex(toF12(value)) + ";")

n = 1.0
f = 100.0

fac = 1/256

t = (192.0 * fac)/2
b = -(192.0 * fac)/2

r = (256.0 * fac)/2
l = -(256.0 * fac)/2

print_add_command(n/l)
print_add_command(0)
print_add_command(0)
print_add_command(0)

print_add_command(0)
print_add_command(n/t)
print_add_command(0)
print_add_command(0)

print_add_command(0)
print_add_command(0)
print_add_command(-(f+n)/(f-n))
print_add_command((-2*f*n/(f-n)))

print_add_command(0)
print_add_command(0)
print_add_command(-1)
print_add_command(0)

# Ortho

#print_add_command(2.0/(r-l))
#print_add_command(0)
#print_add_command(0)
#print_add_command(0)

#print_add_command(0)
#print_add_command(2.0/(t-b))
#print_add_command(0)
#print_add_command(0)

#print_add_command(0)
#print_add_command(0)
#print_add_command(2.0/(n-f))
#print_add_command(0)

#print_add_command((l+r)/(l-r))
#print_add_command((b+t)/(b-t))
#print_add_command((n+f)/(n-f))
#print_add_command(1.0)