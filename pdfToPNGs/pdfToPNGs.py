import sys, fitz  # import the bindings
if sys.version_info >= (3, 6):
    import zipfile
else:
    import zipfile36 as zipfile
from zipfile import ZipFile

def GenerateZip( pathList, zipPath ):
    zipObj = ZipFile( zipPath, 'w' )
    for path in pathList:
        zipObj.write( path )
        print( path )
    zipObj.close()

# Useage: pyPath PDFPath zipPath
if __name__ == '__main__':
    doc = fitz.open( sys.argv[1] )
    zipPath = sys.argv[2]
    zoom_x = 2.0  # horizontal zoom
    zoom_y = 2.0  # vertical zoom
    mat = fitz.Matrix(zoom_x, zoom_y)  # zoom factor 2 in each dimension
    pathList = []
    for page in doc:  # iterate through the pages
        pix = page.get_pixmap(matrix=mat)  # render page to an image
        path = "./output/page-%i.png" % page.number;
        pathList.append( path )
        pix.save( path )  # store image as a PNG
    GenerateZip( pathList, zipPath )