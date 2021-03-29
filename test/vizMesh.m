%data = dlmread("meshData.txt");
%a = readmatrix("mesh.out", 'FileType', '');

verts = readmatrix("meshVerts.txt");
indices = readmatrix("meshIndices.txt");

data = readmatrix("meshData.txt");

numtris = 2500 -1;

x = verts (1:numtris*3, 1);
y = verts (1:numtris*3, 2);
z = verts (1:numtris*3, 3);
c = verts (1:numtris*3, 4);

connectivity = 1 + indices(1:numtris, 1:3);

trisurf(connectivity, x, y, z, c);
colorbar;
axis square;
axis off;
%tris = reshape (0:35, 12, 3);
%tris = tris.';
%title (sprintf ("facecolor = %s", get (trisurf (tris, x, y, z, "facecolor", "interp"), "facecolor")));