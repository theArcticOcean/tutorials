import sys, fitz  # import the bindings

if __name__ == '__main__':
    doc = fitz.open( sys.argv[1] )
    zoom_x = 2.0  # horizontal zoom
    zoom_y = 2.0  # vertical zoom
    mat = fitz.Matrix(zoom_x, zoom_y)  # zoom factor 2 in each dimension
    for page in doc:  # iterate through the pages
        pix = page.get_pixmap(matrix=mat)  # render page to an image
        pix.save("../output/page-%i.png" % page.number)  # store image as a PNG
