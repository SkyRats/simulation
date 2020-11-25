#!/usr/bin/env python

def buildLink(i,tamanho):
    f.write("   <link name=\"link_" + str(i + 1) + "\">\n")
    f.write("    <pose>0 0 " + str(i * tamanho) + "</pose>\n")
    #Bobao lembre que esta faltando a Inercia
    f.write("    <collision name=\"" + str(i + 1) + "\">\n")
    f.write("      <pose>0 0 0</pose>\n")
    f.write("      <geometry>\n")
    f.write("       <cylinder>\n")
    f.write("        <radius>0.012</radius>\n")
    f.write("        <length>" + str(tamanho) + "</length>\n")
    f.write("       </cylinder>\n")
    f.write("      </geometry>\n")
    f.write("     </collision>\n")
    f.write("     <visual name=\"" + str(i + 1) + "\">\n")
    f.write("      <pose>0 0 0</pose>\n")
    f.write("      <geometry>\n")
    f.write("       <cylinder>\n")
    f.write("        <radius>0.012</radius>\n")
    f.write("        <length>" + str(tamanho) + "</length>\n")
    f.write("       </cylinder>\n")
    f.write("      </geometry>\n")
    f.write("     </visual>\n")
    f.write("   </link>\n")

def buildJoint (i, tamanho):
    f.write("   <joint name=\"joint " + str(i) + "\" type=\"ball\">\n")
    f.write("    <parent>link_"+ str(i + 1) + "</parent>\n")
    f.write("    <child>link_" + str(i) + "</child>\n")
    f.write("    <pose>0 0 " + str(i * tamanho) + "</pose>\n")
    f.write("    <physics>\n")
    f.write("    </physics>\n")
    f.write("   </joint>\n")



tamanho = float(input("Por favor, insira o tamanho de cada link: "))
quantidade = int(input("Por favor, insira a quantidade de links: "))

f = open("./Rope/model.sdf", "w")
f.write("<?xml version=\"1.0\"?>")
f.write("<sdf version=\"1.5\">\n")
f.write(" <model name=\"Corda\">\n")
f.write("  <self_collide>true</self_collide>\n")
for i in range(quantidade):
    if i == 0:
        buildLink(i, tamanho)
    else:
        buildLink(i, tamanho)
        buildJoint(i, tamanho)

f.write("</model>\n")
f.write("</sdf>\n")
f.close()
