import bpy

#Output path - change!
output_data = open("testobject.txt", "w")

output_data.write("[VERTEX]\n")
for mesh in bpy.data.meshes:
    for vertex in mesh.vertices:
        output_data.write(str(vertex.co.x) + "," + str(vertex.co.y) + "," + str(vertex.co.z) + "\n")
output_data.write("[/VERTEX]\n")

output_data.write("[INDEX]\n")
for mesh in bpy.data.meshes:
    for poly in mesh.polygons:
        output_data.write(str(poly.vertices[0]) + "," + str(poly.vertices[1]) + "," + str(poly.vertices[2]) + "\n")
output_data.write("[/INDEX]\n")

output_data.write("[NORMAL]\n")
for mesh in bpy.data.meshes:
    for poly in mesh.polygons:
        output_data.write(str(poly.normal[0]) + "," + str(poly.normal[1]) + "," + str(poly.normal[2]) + "\n")
output_data.write("[/NORMAL]")

output_data.close()
