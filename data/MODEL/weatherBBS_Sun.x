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
 143;
 0.57125;0.17318;-6.47965;,
 0.57125;7.55002;-4.07531;,
 12.33308;7.55002;-2.88168;,
 15.08191;0.17318;-4.58181;,
 13.95079;7.55002;0.00000;,
 17.98970;0.17318;0.00000;,
 12.33308;7.55002;2.88168;,
 15.08191;0.17318;4.58181;,
 0.57124;7.55002;4.07531;,
 0.57124;0.17318;6.47965;,
 -11.19060;7.55002;2.88168;,
 -14.18621;0.17318;4.58181;,
 -13.37143;7.55002;0.00000;,
 -17.75638;0.17318;0.00000;,
 -11.19060;7.55002;-2.88168;,
 -14.18621;0.17318;-4.58181;,
 0.57125;0.17318;0.00000;,
 -39.81063;18.38732;0.00000;,
 39.81063;18.38732;0.00000;,
 39.81063;69.72278;0.00000;,
 -39.81063;69.72278;0.00000;,
 5.57411;52.18122;-0.02382;,
 11.05642;57.47541;-0.02382;,
 13.74310;54.88091;-0.02382;,
 8.26079;49.58672;-0.02382;,
 13.76018;54.87966;0.02382;,
 8.27789;49.58550;0.02382;,
 11.07350;57.47422;0.02382;,
 5.59120;52.18001;0.02382;,
 -0.03068;51.75768;0.00000;,
 2.07806;51.18484;-0.00146;,
 -0.03068;51.18484;-0.00206;,
 2.95152;51.18484;0.00000;,
 2.07806;51.18484;0.00146;,
 -0.03068;51.18484;0.00206;,
 -2.13941;51.18484;0.00146;,
 -3.01287;51.18484;0.00000;,
 -2.13941;51.18484;-0.00146;,
 3.86575;49.55352;-0.00269;,
 -0.03068;49.55352;-0.00381;,
 5.47971;49.55352;0.00000;,
 3.86575;49.55352;0.00269;,
 -0.03068;49.55352;0.00381;,
 -3.92711;49.55352;0.00269;,
 -5.54106;49.55352;0.00000;,
 -3.92711;49.55352;-0.00269;,
 5.06025;47.11208;-0.00352;,
 -0.03068;47.11208;-0.00498;,
 7.16898;47.11208;0.00000;,
 5.06025;47.11208;0.00352;,
 -0.03068;47.11208;0.00498;,
 -5.12161;47.11208;0.00352;,
 -7.23034;47.11208;0.00000;,
 -5.12161;47.11208;-0.00352;,
 5.47971;44.23220;-0.00381;,
 -0.03068;44.23220;-0.00539;,
 7.76218;44.23220;0.00000;,
 5.47971;44.23220;0.00381;,
 -0.03068;44.23220;0.00539;,
 -5.54106;44.23220;0.00381;,
 -7.82354;44.23220;0.00000;,
 -5.54106;44.23220;-0.00381;,
 5.06025;41.35231;-0.00352;,
 -0.03068;41.35231;-0.00498;,
 7.16898;41.35231;0.00000;,
 5.06025;41.35231;0.00352;,
 -0.03068;41.35231;0.00498;,
 -5.12161;41.35231;0.00352;,
 -7.23034;41.35231;0.00000;,
 -5.12161;41.35231;-0.00352;,
 3.86575;38.91089;-0.00269;,
 -0.03068;38.91089;-0.00381;,
 5.47971;38.91089;0.00000;,
 3.86575;38.91089;0.00269;,
 -0.03068;38.91089;0.00381;,
 -3.92711;38.91089;0.00269;,
 -5.54106;38.91089;0.00000;,
 -3.92711;38.91089;-0.00269;,
 2.07806;37.27955;-0.00146;,
 -0.03068;37.27955;-0.00206;,
 2.95152;37.27955;0.00000;,
 2.07806;37.27955;0.00146;,
 -0.03068;37.27955;0.00206;,
 -2.13941;37.27955;0.00146;,
 -3.01287;37.27955;0.00000;,
 -2.13941;37.27955;-0.00146;,
 -0.03068;36.70671;0.00000;,
 -1.93045;60.86293;-0.02382;,
 1.86910;60.86293;-0.02382;,
 1.86910;53.37578;-0.02382;,
 -1.93045;53.37578;-0.02382;,
 1.86910;60.86293;0.02382;,
 1.86910;53.37578;0.02382;,
 -1.93045;60.86293;0.02382;,
 -1.93045;53.37578;0.02382;,
 -1.93045;34.92579;-0.02382;,
 1.86910;34.92579;-0.02382;,
 1.86910;27.43863;-0.02382;,
 -1.93045;27.43863;-0.02382;,
 1.86910;34.92579;0.02382;,
 1.86910;27.43863;0.02382;,
 -1.93045;34.92579;0.02382;,
 -1.93045;27.43863;0.02382;,
 17.15513;46.28584;-0.02382;,
 17.15513;42.61664;-0.02382;,
 9.40198;42.61664;-0.02382;,
 9.40198;46.28584;-0.02382;,
 17.15513;42.61664;0.02382;,
 9.40198;42.61664;0.02382;,
 17.15513;46.28584;0.02382;,
 9.40198;46.28584;0.02382;,
 -9.67995;46.28584;-0.02382;,
 -9.67995;42.61664;-0.02382;,
 -17.43310;42.61664;-0.02382;,
 -17.43310;46.28584;-0.02382;,
 -9.67995;42.61664;0.02382;,
 -17.43310;42.61664;0.02382;,
 -9.67995;46.28584;0.02382;,
 -17.43310;46.28584;0.02382;,
 -4.87902;52.39007;-0.02382;,
 -7.56571;49.79557;-0.02382;,
 -13.04801;55.08979;-0.02382;,
 -10.36133;57.68429;-0.02382;,
 -7.58280;49.79437;0.02382;,
 -13.06510;55.08857;0.02382;,
 -4.89611;52.38883;0.02382;,
 -10.37841;57.68309;0.02382;,
 -11.37034;31.20309;-0.02382;,
 -14.05702;33.79760;-0.02382;,
 -8.57472;39.09180;-0.02382;,
 -5.88803;36.49729;-0.02382;,
 -14.05829;33.81409;0.02382;,
 -8.57598;39.10829;0.02382;,
 -11.37160;31.21959;0.02382;,
 -5.88929;36.51379;0.02382;,
 11.30898;31.20309;-0.02382;,
 5.82668;36.49729;-0.02382;,
 8.51337;39.09180;-0.02382;,
 13.99567;33.79760;-0.02382;,
 8.51463;39.10829;0.02382;,
 13.99694;33.81409;0.02382;,
 5.82794;36.51379;0.02382;,
 11.31025;31.21959;0.02382;;
 
 133;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 4;11,10,12,13;,
 4;13,12,14,15;,
 4;15,14,1,0;,
 3;16,0,3;,
 3;16,3,5;,
 3;16,5,7;,
 3;16,7,9;,
 3;16,9,11;,
 3;16,11,13;,
 3;16,13,15;,
 3;16,15,0;,
 4;2,12,10,4;,
 4;2,1,14,12;,
 4;10,8,6,4;,
 4;17,18,19,20;,
 4;20,19,18,17;,
 4;21,22,23,24;,
 4;24,23,25,26;,
 4;26,25,27,28;,
 4;28,27,22,21;,
 4;28,21,24,26;,
 4;22,27,25,23;,
 3;29,30,31;,
 3;29,32,30;,
 3;29,33,32;,
 3;29,34,33;,
 3;29,35,34;,
 3;29,36,35;,
 3;29,37,36;,
 3;29,31,37;,
 4;31,30,38,39;,
 4;30,32,40,38;,
 4;32,33,41,40;,
 4;33,34,42,41;,
 4;34,35,43,42;,
 4;35,36,44,43;,
 4;36,37,45,44;,
 4;37,31,39,45;,
 4;39,38,46,47;,
 4;38,40,48,46;,
 4;40,41,49,48;,
 4;41,42,50,49;,
 4;42,43,51,50;,
 4;43,44,52,51;,
 4;44,45,53,52;,
 4;45,39,47,53;,
 4;47,46,54,55;,
 4;46,48,56,54;,
 4;48,49,57,56;,
 4;49,50,58,57;,
 4;50,51,59,58;,
 4;51,52,60,59;,
 4;52,53,61,60;,
 4;53,47,55,61;,
 4;55,54,62,63;,
 4;54,56,64,62;,
 4;56,57,65,64;,
 4;57,58,66,65;,
 4;58,59,67,66;,
 4;59,60,68,67;,
 4;60,61,69,68;,
 4;61,55,63,69;,
 4;63,62,70,71;,
 4;62,64,72,70;,
 4;64,65,73,72;,
 4;65,66,74,73;,
 4;66,67,75,74;,
 4;67,68,76,75;,
 4;68,69,77,76;,
 4;69,63,71,77;,
 4;71,70,78,79;,
 4;70,72,80,78;,
 4;72,73,81,80;,
 4;73,74,82,81;,
 4;74,75,83,82;,
 4;75,76,84,83;,
 4;76,77,85,84;,
 4;77,71,79,85;,
 3;79,78,86;,
 3;78,80,86;,
 3;80,81,86;,
 3;81,82,86;,
 3;82,83,86;,
 3;83,84,86;,
 3;84,85,86;,
 3;85,79,86;,
 4;87,88,89,90;,
 4;88,91,92,89;,
 4;91,93,94,92;,
 4;93,87,90,94;,
 4;93,91,88,87;,
 4;90,89,92,94;,
 4;95,96,97,98;,
 4;96,99,100,97;,
 4;99,101,102,100;,
 4;101,95,98,102;,
 4;101,99,96,95;,
 4;98,97,100,102;,
 4;103,104,105,106;,
 4;104,107,108,105;,
 4;107,109,110,108;,
 4;109,103,106,110;,
 4;109,107,104,103;,
 4;106,105,108,110;,
 4;111,112,113,114;,
 4;112,115,116,113;,
 4;115,117,118,116;,
 4;117,111,114,118;,
 4;117,115,112,111;,
 4;114,113,116,118;,
 4;119,120,121,122;,
 4;120,123,124,121;,
 4;123,125,126,124;,
 4;125,119,122,126;,
 4;125,123,120,119;,
 4;122,121,124,126;,
 4;127,128,129,130;,
 4;128,131,132,129;,
 4;131,133,134,132;,
 4;133,127,130,134;,
 4;133,131,128,127;,
 4;130,129,132,134;,
 4;135,136,137,138;,
 4;138,137,139,140;,
 4;140,139,141,142;,
 4;142,141,136,135;,
 4;142,135,138,140;,
 4;136,141,139,137;;
 
 MeshMaterialList {
  4;
  133;
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
  1,
  1,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   7.000000;
   0.110000;0.110000;0.110000;;
   0.060000;0.060000;0.060000;;
  }
  Material {
   0.000000;0.949000;0.651000;0.200000;;
   100.000000;
   1.000000;1.000000;1.000000;;
   0.000000;0.949000;0.651000;;
  }
  Material {
   0.715200;0.295200;0.000000;0.500000;;
   38.000000;
   0.400000;0.400000;0.400000;;
   0.339720;0.140220;0.000000;;
  }
  Material {
   0.329600;0.542400;0.800000;0.600000;;
   22.000000;
   0.220000;0.220000;0.220000;;
   0.082400;0.135600;0.200000;;
  }
 }
 MeshNormals {
  109;
  0.000548;0.290273;-0.956944;,
  0.491898;0.384401;-0.781199;,
  0.885657;0.464341;0.000000;,
  0.491898;0.384401;0.781199;,
  0.000548;0.290273;0.956944;,
  -0.445876;0.383018;0.809007;,
  -0.718929;0.422310;0.552083;,
  -0.445876;0.383018;-0.809007;,
  0.000000;-1.000000;-0.000000;,
  -0.718929;0.422310;-0.552083;,
  0.000000;1.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;-1.000000;,
  0.671005;-0.694850;-0.258719;,
  0.000000;-0.000000;1.000000;,
  -0.000000;0.006149;-0.999981;,
  -0.000000;0.002333;-0.999997;,
  0.000978;0.003989;-0.999992;,
  0.001670;0.005644;-0.999983;,
  0.000978;0.003989;0.999992;,
  -0.000000;0.002333;0.999997;,
  -0.000978;0.003988;0.999992;,
  -0.001670;0.005644;0.999983;,
  -0.000978;0.003988;-0.999992;,
  -0.000000;0.000775;-1.000000;,
  0.000978;0.001323;-0.999999;,
  0.001669;0.001870;-0.999997;,
  0.000978;0.001323;0.999999;,
  -0.000000;0.000775;1.000000;,
  -0.000978;0.001322;0.999999;,
  -0.001669;0.001870;0.999997;,
  -0.000978;0.001322;-0.999999;,
  -0.000000;0.000311;-1.000000;,
  0.000978;0.000530;-0.999999;,
  0.001669;0.000750;-0.999998;,
  0.000978;0.000530;0.999999;,
  -0.000000;0.000311;1.000000;,
  -0.000978;0.000530;0.999999;,
  -0.001669;0.000750;0.999998;,
  -0.000978;0.000530;-0.999999;,
  -0.000000;0.000000;-1.000000;,
  0.000978;0.000000;-1.000000;,
  0.001669;0.000000;-0.999999;,
  0.000978;0.000000;1.000000;,
  -0.000000;0.000000;1.000000;,
  -0.000978;0.000000;1.000000;,
  -0.001669;0.000000;0.999999;,
  -0.000978;0.000000;-1.000000;,
  -0.000000;-0.000311;-1.000000;,
  0.000978;-0.000530;-0.999999;,
  0.001669;-0.000750;-0.999998;,
  0.000978;-0.000530;0.999999;,
  -0.000000;-0.000311;1.000000;,
  -0.000978;-0.000530;0.999999;,
  -0.001669;-0.000750;0.999998;,
  -0.000978;-0.000530;-0.999999;,
  -0.000000;-0.000775;-1.000000;,
  0.000978;-0.001323;-0.999999;,
  0.001669;-0.001870;-0.999997;,
  0.000978;-0.001323;0.999999;,
  -0.000000;-0.000775;1.000000;,
  -0.000978;-0.001323;0.999999;,
  -0.001669;-0.001870;0.999997;,
  -0.000978;-0.001323;-0.999999;,
  -0.000000;-0.002333;-0.999997;,
  0.000978;-0.003988;-0.999992;,
  0.001670;-0.005643;-0.999983;,
  0.000978;-0.003988;0.999992;,
  -0.000000;-0.002333;0.999997;,
  -0.000978;-0.003988;0.999992;,
  -0.001670;-0.005643;0.999983;,
  -0.000978;-0.003988;-0.999992;,
  -0.000000;-0.006149;-0.999981;,
  1.000000;0.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.671070;-0.694911;-0.258388;,
  -0.671078;0.694922;-0.258338;,
  0.671076;0.694920;-0.258348;,
  -0.671109;0.694954;0.258172;,
  -0.676843;-0.700890;0.225026;,
  0.676895;0.700936;-0.224726;,
  -0.000000;0.006149;0.999981;,
  0.001670;0.005644;0.999983;,
  -0.001670;0.005644;-0.999983;,
  0.001669;0.001870;0.999997;,
  -0.001669;0.001870;-0.999997;,
  0.001669;0.000750;0.999998;,
  -0.001669;0.000750;-0.999998;,
  0.001669;0.000000;0.999999;,
  -0.001669;0.000000;-0.999999;,
  0.001669;-0.000750;0.999998;,
  -0.001669;-0.000750;-0.999998;,
  0.001669;-0.001870;0.999997;,
  -0.001669;-0.001870;-0.999997;,
  0.001670;-0.005643;0.999983;,
  -0.001670;-0.005643;-0.999983;,
  -0.000000;-0.006149;0.999981;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.671056;0.694894;0.258471;,
  0.676862;-0.700918;0.224881;,
  -0.676861;0.700907;-0.224919;,
  0.671062;-0.694906;0.258420;,
  -0.676895;-0.700945;0.224698;,
  0.676881;0.700931;-0.224784;,
  -0.671050;-0.694893;0.258485;,
  0.676907;-0.700958;0.224621;,
  -0.676881;0.700931;-0.224784;;
  133;
  4;0,0,1,1;,
  4;1,1,2,2;,
  4;2,2,3,3;,
  4;3,3,4,4;,
  4;4,4,5,5;,
  4;5,5,6,6;,
  4;9,9,7,7;,
  4;7,7,0,0;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  4;10,10,10,10;,
  4;10,10,10,10;,
  4;10,10,10,10;,
  4;11,11,11,11;,
  4;12,12,12,12;,
  4;13,13,13,13;,
  4;14,14,14,14;,
  4;15,15,15,15;,
  4;79,79,79,79;,
  4;80,80,80,80;,
  4;81,81,81,81;,
  3;16,18,17;,
  3;16,19,18;,
  3;82,20,83;,
  3;82,21,20;,
  3;82,22,21;,
  3;82,23,22;,
  3;16,24,84;,
  3;16,17,24;,
  4;17,18,26,25;,
  4;18,19,27,26;,
  4;83,20,28,85;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;84,24,32,86;,
  4;24,17,25,32;,
  4;25,26,34,33;,
  4;26,27,35,34;,
  4;85,28,36,87;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,31,39,38;,
  4;86,32,40,88;,
  4;32,25,33,40;,
  4;33,34,42,41;,
  4;34,35,43,42;,
  4;87,36,44,89;,
  4;36,37,45,44;,
  4;37,38,46,45;,
  4;38,39,47,46;,
  4;88,40,48,90;,
  4;40,33,41,48;,
  4;41,42,50,49;,
  4;42,43,51,50;,
  4;89,44,52,91;,
  4;44,45,53,52;,
  4;45,46,54,53;,
  4;46,47,55,54;,
  4;90,48,56,92;,
  4;48,41,49,56;,
  4;49,50,58,57;,
  4;50,51,59,58;,
  4;91,52,60,93;,
  4;52,53,61,60;,
  4;53,54,62,61;,
  4;54,55,63,62;,
  4;92,56,64,94;,
  4;56,49,57,64;,
  4;57,58,66,65;,
  4;58,59,67,66;,
  4;93,60,68,95;,
  4;60,61,69,68;,
  4;61,62,70,69;,
  4;62,63,71,70;,
  4;94,64,72,96;,
  4;64,57,65,72;,
  3;65,66,73;,
  3;66,67,73;,
  3;95,68,97;,
  3;68,69,97;,
  3;69,70,97;,
  3;70,71,97;,
  3;96,72,73;,
  3;72,65,73;,
  4;13,13,13,13;,
  4;74,74,74,74;,
  4;15,15,15,15;,
  4;98,98,98,98;,
  4;99,99,99,99;,
  4;75,75,75,75;,
  4;13,13,13,13;,
  4;74,74,74,74;,
  4;15,15,15,15;,
  4;98,98,98,98;,
  4;99,99,99,99;,
  4;75,75,75,75;,
  4;13,13,13,13;,
  4;75,75,75,75;,
  4;15,15,15,15;,
  4;99,99,99,99;,
  4;74,74,74,74;,
  4;98,98,98,98;,
  4;13,13,13,13;,
  4;75,75,75,75;,
  4;15,15,15,15;,
  4;99,99,99,99;,
  4;74,74,74,74;,
  4;98,98,98,98;,
  4;13,13,13,13;,
  4;76,76,76,76;,
  4;15,15,15,15;,
  4;100,100,100,100;,
  4;101,101,101,101;,
  4;102,102,102,102;,
  4;13,13,13,13;,
  4;77,77,77,77;,
  4;15,15,15,15;,
  4;103,103,103,103;,
  4;104,104,104,104;,
  4;105,105,105,105;,
  4;13,13,13,13;,
  4;78,78,78,78;,
  4;15,15,15,15;,
  4;106,106,106,106;,
  4;107,107,107,107;,
  4;108,108,108,108;;
 }
}