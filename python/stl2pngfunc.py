from trianglezslice import TriZSlice
import re

# load the stl files into trianglebarmeshes
# slice and return parameters(images, pixel size, min x and min y)
transmaps = { "unit":lambda t: t, "swapyz": lambda t: (t[0], -t[2], t[1]) }

def pngname(optionoutputfile, i, z):
    if re.search("%.*?d(?i)", optionoutputfile):
        return optionoutputfile % i
    if re.search("%.*?f(?i)", optionoutputfile):
        return optionoutputfile % z
    return optionoutputfile

def stl2png(stlfile, nlayers, image_width, image_height, outfiles, func=None):
    tzs = TriZSlice(True)
    tzs.LoadSTLfile(stlfile, transmaps["unit"])
    
    extra = "5%"
    tzs.SetExtents(extra)
    x_pixel_size, y_pixel_size, x0, y0 = tzs.BuildPixelGridStructures(image_width, image_height)
    
    for i in range(nlayers):
        z = tzs.zlo + (tzs.zhi - tzs.zlo)*(i + 0.5)/nlayers
        tzs.SliceToPNG(z, pngname(outfiles, i, z))
        # return current frame number
        if(func != None):
            func(i)
    # added by Yao        
    return x_pixel_size, y_pixel_size, x0, y0  
          