xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 20;
 0.00000;0.85500;-4.50000;,
 0.00000;18.59400;0.00000;,
 2.64503;0.85500;-3.64058;,
 4.27976;0.85500;-1.39058;,
 4.27976;0.85500;1.39058;,
 2.64503;0.85500;3.64058;,
 0.00000;0.85500;4.50000;,
 -2.64503;0.85500;3.64058;,
 -4.27976;0.85500;1.39058;,
 -4.27975;0.85500;-1.39058;,
 -2.64503;0.85500;-3.64058;,
 0.00000;0.85500;0.00000;,
 -6.59861;1.72969;-6.59861;,
 6.59861;1.72969;-6.59861;,
 6.59861;-0.01969;-6.59861;,
 -6.59861;-0.01969;-6.59861;,
 6.59861;1.72969;6.59861;,
 6.59861;-0.01969;6.59861;,
 -6.59861;1.72969;6.59861;,
 -6.59861;-0.01969;6.59861;;
 
 26;
 3;0,1,2;,
 3;2,1,3;,
 3;3,1,4;,
 3;4,1,5;,
 3;5,1,6;,
 3;6,1,7;,
 3;7,1,8;,
 3;8,1,9;,
 3;9,1,10;,
 3;10,1,0;,
 3;11,0,2;,
 3;11,2,3;,
 3;11,3,4;,
 3;11,4,5;,
 3;11,5,6;,
 3;11,6,7;,
 3;11,7,8;,
 3;11,8,9;,
 3;11,9,10;,
 3;11,10,0;,
 4;12,13,14,15;,
 4;13,16,17,14;,
 4;16,18,19,17;,
 4;18,12,15,19;,
 4;18,16,13,12;,
 4;15,14,17,19;;
 
 MeshMaterialList {
  1;
  26;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.000000;0.065600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  20;
  0.000000;0.245890;-0.969298;,
  0.569739;0.245890;-0.784179;,
  0.921857;0.245890;-0.299530;,
  0.921857;0.245890;0.299530;,
  0.569739;0.245890;0.784179;,
  0.000000;0.245890;0.969298;,
  -0.569739;0.245890;0.784179;,
  -0.921857;0.245890;0.299529;,
  -0.921857;0.245890;-0.299530;,
  -0.569739;0.245890;-0.784178;,
  0.779766;0.266466;-0.566534;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  0.297844;0.266466;0.916670;,
  -0.963844;0.266466;-0.000000;,
  -0.300398;0.234533;-0.924530;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;;
  26;
  3;0,10,1;,
  3;1,10,2;,
  3;2,10,3;,
  3;3,15,4;,
  3;4,15,5;,
  3;5,15,6;,
  3;6,16,7;,
  3;7,16,8;,
  3;8,16,9;,
  3;9,17,0;,
  3;11,11,11;,
  3;11,11,11;,
  3;11,11,11;,
  3;11,11,11;,
  3;11,11,11;,
  3;11,11,11;,
  3;11,11,11;,
  3;11,11,11;,
  3;11,11,11;,
  3;11,11,11;,
  4;12,12,12,12;,
  4;13,13,13,13;,
  4;14,14,14,14;,
  4;18,18,18,18;,
  4;19,19,19,19;,
  4;11,11,11,11;;
 }
}
